#if defined __StaticFieldPerk_included
#endinput
#endif
#define __StaticFieldPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_util_base.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_fx.inc"
#include <sdkhooks>
#include <vector>

#define STATIC_FIELD_RADIUS 250.0
#define STATIC_FIELD_DAMAGE_FACTOR 0.02 // 伤害系数，每秒根据速度造成伤害
#define STATIC_FIELD_AURA_PARTICLE "utaunt_electric_mist_parent"

#define STATIC_FIELD_DATA_AURA_FX (PERK_DATA_START_INDEX + 0)

methodmap StaticFieldPerk < ZombieBasePerk {
    public StaticFieldPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        StaticFieldPerk sm = view_as<StaticFieldPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, StaticFieldPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, StaticFieldPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, StaticFieldPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, StaticFieldPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, StaticFieldPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, StaticFieldPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, StaticFieldPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, StaticFieldPerkFupdateCondStats);

        sm.aura_fx = -1;
        return sm;
    }

    property int aura_fx {
        public get() {
            this.Position = view_as<DataPackPos>(STATIC_FIELD_DATA_AURA_FX);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(STATIC_FIELD_DATA_AURA_FX);
            this.WriteCell(value);
        }
    }
}

stock BasePerk StaticFieldPerkFnew(int client) {
    return new StaticFieldPerk(client);
}

FUNCTION(StaticFieldPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StaticField");
}

FUNCTION(StaticFieldPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StaticFieldPerk_shortdesc");
}

FUNCTION(StaticFieldPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StaticFieldPerk_desc");
}

FUNCTION(StaticFieldPerk, onPlayerSpawn)) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
    }
    _inst.aura_fx = fxCreateParticle(STATIC_FIELD_AURA_PARTICLE, _inst.client, AttachBase, -1.0);
}

FUNCTION(StaticFieldPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
}

FUNCTION(StaticFieldPerk, onRemove)) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
}

FUNCTION(StaticFieldPerk, onPeriodic)) {
    int client = _inst.client;
    float owner_pos[3];
    GetClientAbsOrigin(client, owner_pos);

    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            float sur_pos[3];
            GetClientAbsOrigin(i, sur_pos);

            if (GetVectorDistance(owner_pos, sur_pos) <= STATIC_FIELD_RADIUS) {
                float velocity[3];
                GetEntPropVector(i, Prop_Data, "m_vecVelocity", velocity);
                float speed = GetVectorLength(velocity);

                if (speed > 50.0) { // 避免对微小移动造成伤害
                    float damage = speed * STATIC_FIELD_DAMAGE_FACTOR;
                    SDKHooks_TakeDamage(i, client, client, damage, DMG_SHOCK);
                }
            }
        }
    }
}

FUNCTION(StaticFieldPerk, updateCondStats), char[] buffer, int maxlen) {
Format(buffer, maxlen, "%t", "StaticFieldPerk_Status_Active");
}