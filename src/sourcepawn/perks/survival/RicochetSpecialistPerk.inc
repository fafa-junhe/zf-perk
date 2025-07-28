#if defined __RicochetSpecialistPerk_included
#endinput
#endif
#define __RicochetSpecialistPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdktools_tempents>

#define RICOCHET_CHANCE 50
#define RICOCHET_DAMAGE_MULTIPLIER_BULLET 0.7 // The "passive" 30% damage reduction for bullets
#define RICOCHET_BOUNCE_DAMAGE_MULTIPLIER 0.5 // Each bounce deals half the damage of the previous hit
#define RICOCHET_MAX_BOUNCES 2
#define RICOCHET_SEARCH_RADIUS 400.0
#define RICOCHET_SEARCH_RADIUS_SQ (RICOCHET_SEARCH_RADIUS * RICOCHET_SEARCH_RADIUS)

#define RICOCHET_DATA_BOUNCE_COUNT (PERK_DATA_START_INDEX + 0)

methodmap RicochetSpecialistPerk < SurvivorBasePerk {
    public RicochetSpecialistPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        RicochetSpecialistPerk sm = view_as<RicochetSpecialistPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, RicochetSpecialistPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, RicochetSpecialistPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, RicochetSpecialistPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, RicochetSpecialistPerkFonDealDamagePost);

        sm.Position = view_as<DataPackPos>(RICOCHET_DATA_BOUNCE_COUNT);
        sm.WriteCell(0);

        static bool s_bInitialized = false;
        if (!s_bInitialized) {
            PrecacheModel("sprites/laserbeam.vmt", true);
            s_bInitialized = true;
        }

        return sm;
    }

    property int bounce_count {
        public get() {
            this.Position = view_as<DataPackPos>(RICOCHET_DATA_BOUNCE_COUNT);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(RICOCHET_DATA_BOUNCE_COUNT);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk RicochetSpecialistPerkFnew(int client) {
    return new RicochetSpecialistPerk(client);
}

FUNCTION(RicochetSpecialistPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RicochetSpecialist");
}

FUNCTION(RicochetSpecialistPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RicochetSpecialistPerk_shortdesc");
}

FUNCTION(RicochetSpecialistPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RicochetSpecialistPerk_desc");
}

FUNCTION(RicochetSpecialistPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    // Only ricochet off zombies with bullet damage. Non-bullet damage is unaffected.
    if (attacker != _inst.client || !isZom(victim) || !(damagetype & DMG_BULLET)) {
        if (attacker == _inst.client) {
            _inst.bounce_count = 0;
        }
        return;
    }

    if (_inst.bounce_count >= RICOCHET_MAX_BOUNCES) {
        _inst.bounce_count = 0;
        return;
    }

    if (GetRandomInt(1, 100) <= RICOCHET_CHANCE) {
        float victim_pos[3];
        GetClientAbsOrigin(victim, victim_pos);

        int next_target = -1;
        float min_dist_sq = RICOCHET_SEARCH_RADIUS_SQ;

        for (int i = 1; i <= MaxClients; i++) {
            if (i == victim || !validClient(i) || !isZom(i) || !IsPlayerAlive(i)) continue;

            float other_pos[3];
            GetClientAbsOrigin(i, other_pos);
            float dist_sq = GetVectorDistance(victim_pos, other_pos, true);

            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                next_target = i;
            }
        }

        if (next_target != -1) {
            _inst.bounce_count++;
            
            float bounce_damage;
            if (_inst.bounce_count == 1) {
                // First bounce: apply the 30% reduction, then halve it.
                bounce_damage = damage * RICOCHET_DAMAGE_MULTIPLIER_BULLET * RICOCHET_BOUNCE_DAMAGE_MULTIPLIER;
            } else {
                // Subsequent bounces: just halve the previous bounce's damage.
                bounce_damage = damage * RICOCHET_BOUNCE_DAMAGE_MULTIPLIER;
            }

            float next_target_pos[3];
            GetClientAbsOrigin(next_target, next_target_pos);
            
            int beamSprite = PrecacheModel("sprites/laserbeam.vmt");
            TE_SetupBeamPoints(victim_pos, next_target_pos, beamSprite, 0, 0, 10, 0.3, 5.0, 5.0, 10, 0.0, {0, 255, 255, 255}, 10);
            TE_SendToAll();

            PrintHintText(attacker, "子弹已弹射!");

            DataPack hPack = new DataPack();
            hPack.WriteCell(next_target);
            hPack.WriteCell(attacker);
            hPack.WriteCell(inflictor);
            hPack.WriteFloat(bounce_damage);
            hPack.WriteCell(damagetype);
            CreateTimer(0.01, Ricochet_DelayedDamage, hPack);
        } else {
            _inst.bounce_count = 0;
        }
    } else {
        _inst.bounce_count = 0;
    }
}

public Action Ricochet_DelayedDamage(Handle timer, DataPack hPack) {
    hPack.Position = DP(0);
    int victim = hPack.ReadCell();
    int attacker = hPack.ReadCell();
    int inflictor = hPack.ReadCell();
    float damage = hPack.ReadFloat();
    int damagetype = hPack.ReadCell();

    if (validClient(victim) && IsPlayerAlive(victim)) {
        // The damage passed to SDKHooks_TakeDamage will be the input for the *next* bounce
        SDKHooks_TakeDamage(victim, attacker, inflictor, damage, damagetype);
    }

    delete hPack;
    return Plugin_Stop;
}