#if defined __TarredPerk_included
#endinput
#endif
#define __TarredPerk_included

#include <sdkhooks>
#include <sdktools>
#include <tf2_stocks>
#include <float>
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "ZombieBasePerk.inc"

// Defines from all_perks.md
#define ZF_TARRED_MAX_ITEMS 5
#define ZF_TARRED_DURATION_MELEE 10
#define ZF_TARRED_DURATION_SLICK 30
#define ZF_TARRED_ROF -20
#define ZF_TARRED_SPEED_MELEE -40
#define ZF_TARRED_SPEED_SLICK -30
#define ZF_TARRED_TIMER 30
#define ZF_TARRED_RADIUS 75.0
#define ZF_TARRED_PROJECTILE_SPEED 900.0
#define ZF_TARRED_PROJECTILE_MODEL "models/props_junk/garbage_milkcarton002a.mdl"

// Data slots
#define TARRED_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define TARRED_DATA_POOLS_HANDLE (PERK_DATA_START_INDEX + 1)

methodmap TarredPerk < ZombieBasePerk {
    public TarredPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        TarredPerk sm = view_as<TarredPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, TarredPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, TarredPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, TarredPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, TarredPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, TarredPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, TarredPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, TarredPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, TarredPerkFonDeath);

        sm.cooldown = 0.0;
        sm.pools = new DataPack();

        return sm;
    }

    property float cooldown {
        public get() {
            this.Position = view_as<DataPackPos>(TARRED_DATA_COOLDOWN);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(TARRED_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }

    property DataPack pools {
        public get() {
            this.Position = view_as<DataPackPos>(TARRED_DATA_POOLS_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(TARRED_DATA_POOLS_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }
}

stock BasePerk TarredPerkFnew(int client) {
    return new TarredPerk(client);
}

FUNCTION(TarredPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Tarred");
}

FUNCTION(TarredPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "吐出减速的焦油");
}

FUNCTION(TarredPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "发医生语音来吐出焦油。焦油能降低幸存者的移动速度与攻击速度,持续30秒或直到你死亡。你的近战攻击附带焦油效果。");
}

FUNCTION(TarredPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    DataPack pools = _inst.pools;
    if (pools != null) {
        pools.Reset();
        while (pools.IsReadable()) {
            int pool_entity = pools.ReadCell();
            if (IsValidEdict(pool_entity)) {
                AcceptEntityInput(pool_entity, "Kill");
            }
            pools.ReadFloat(); // Keep pack structure consistent
        }
        delete pools;
        _inst.pools = null;
    }
}

FUNCTION(TarredPerk, onCallForMedic)) {
    if (_inst.cooldown > 0.0) {
        return;
    }
    _inst.cooldown = float(ZF_TARRED_TIMER);
    
    int client = _inst.client;
    int projectile = CreateEntityByName("tf_projectile_jar");
    if (projectile == -1) return;

    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    DispatchSpawn(projectile);
    SetEntPropEnt(projectile, Prop_Send, "m_hOwnerEntity", client);
    SetEntProp(projectile, Prop_Send, "m_iTeamNum", zomTeam());
    SetEntityModel(projectile, ZF_TARRED_PROJECTILE_MODEL);

    float vec_forward[3];
    GetAngleVectors(eye_ang, vec_forward, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(vec_forward, ZF_TARRED_PROJECTILE_SPEED);
    
    TeleportEntity(projectile, eye_pos, NULL_VECTOR, vec_forward);
    SDKHook(projectile, SDKHook_StartTouch, OnTarredSpitTouch);
}

public void OnTarredSpitTouch(int projectile, int other) {
    if (projectile <= 0 || !IsValidEntity(projectile)) return;

    int owner = GetEntPropEnt(projectile, Prop_Send, "m_hOwnerEntity");
    if (!validLivingZom(owner)) return;
    
    BasePerk perk = g_hPerks[owner];
    char perkName[32];
    perk.getName(perkName, sizeof(perkName));
    if (perk == null || !StrEqual(perkName, "Tarred")) return;
    TarredPerk tarred_perk = view_as<TarredPerk>(perk);

    float pos[3];
    GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", pos);

    int pool_entity = fxCreateParticle("effects/tar_splash_01.pcf", owner, AttachNone, float(ZF_TARRED_DURATION_SLICK), pos);
    if (IsValidEdict(pool_entity)) {
        tarred_perk.pools.WriteCell(pool_entity);
        tarred_perk.pools.WriteFloat(GetGameTime());
    }

    RemoveEdict(projectile);
    SDKUnhook(projectile, SDKHook_StartTouch, OnTarredSpitTouch);
}

FUNCTION(TarredPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (isSur(victim) && (damagetype & DMG_CLUB)) {
        addStatTempStack(victim, ZFStatSpeed, ZF_TARRED_SPEED_MELEE, ZF_TARRED_DURATION_MELEE);
    }
}

FUNCTION(TarredPerk, onPeriodic)) {
    if (_inst.cooldown > 0.0) {
        _inst.cooldown = fMax(0.0, _inst.cooldown - 1.0);
    }

    DataPack pools = _inst.pools;
    if (pools == null) return;

    DataPack new_pools = new DataPack();
    pools.Reset();
    while (pools.IsReadable()) {
        int pool_entity = pools.ReadCell();
        float creation_time = pools.ReadFloat();

        if (!IsValidEdict(pool_entity) || (GetGameTime() - creation_time) > ZF_TARRED_DURATION_SLICK) {
            if (IsValidEdict(pool_entity)) AcceptEntityInput(pool_entity, "Kill");
            continue;
        }

        new_pools.WriteCell(pool_entity);
        new_pools.WriteFloat(creation_time);

        float pool_pos[3];
        GetEntPropVector(pool_entity, Prop_Data, "m_vecOrigin", pool_pos);

        for (int j = 1; j <= MaxClients; j++) {
            if (validLivingSur(j)) {
                float sur_pos[3];
                GetClientAbsOrigin(j, sur_pos);
                if (GetVectorDistance(pool_pos, sur_pos) <= ZF_TARRED_RADIUS) {
                    addStat(j, ZFStatSpeed, ZFStatTypeCond, ZF_TARRED_SPEED_SLICK);
                    addStat(j, ZFStatRof, ZFStatTypeCond, ZF_TARRED_ROF);
                }
            }
        }
    }
    delete _inst.pools;
    _inst.pools = new_pools;
}

FUNCTION(TarredPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown > 0.0) {
        Format(buffer, maxlen, "Spit Cooldown: %.0fs", _inst.cooldown);
    } else {
        Format(buffer, maxlen, "Spit Ready");
    }
}


