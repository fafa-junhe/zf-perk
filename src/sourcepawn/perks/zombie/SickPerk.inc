#if defined __SickPerk_included
#endinput
#endif
#define __SickPerk_included

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
#define ZF_SICK_DEFEND -75
#define ZF_SICK_DAMAGE 15.0
#define ZF_SICK_DAMAGE_RADIUS 150.0
#define ZF_SICK_MAX_DIST 1000.0
#define ZF_SICK_COOLDOWN 15.0
#define ZF_SICK_LIFETIME 35.0
#define ZF_SICK_SPIT_COUNT 5
#define ZF_SICK_SPIT_INTERVAL 0.2
#define ZF_SICK_PROJECTILE_SPEED 900.0
#define ZF_SICK_PROJECTILE_MODEL "models/props_junk/garbage_milkcarton002a.mdl"

// Data slots
#define SICK_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define SICK_DATA_POOLS_HANDLE (PERK_DATA_START_INDEX + 1)
#define SICK_DATA_SPIT_COUNT (PERK_DATA_START_INDEX + 2)

methodmap SickPerk < ZombieBasePerk {
    public SickPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        SickPerk sm = view_as<SickPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SickPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, SickPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, SickPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SickPerkFupdatePermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, SickPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, SickPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, SickPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, SickPerkFonDeath);

        sm.cooldown = 0.0;
        sm.pools = new DataPack();

        return sm;
    }

    property float cooldown {
        public get() {
            this.Position = view_as<DataPackPos>(SICK_DATA_COOLDOWN);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(SICK_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }

    property DataPack pools {
        public get() {
            this.Position = view_as<DataPackPos>(SICK_DATA_POOLS_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(SICK_DATA_POOLS_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }
    
    property int spit_count {
        public get() {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_COUNT);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_COUNT);
            this.WriteCell(value);
        }
    }
}

stock BasePerk SickPerkFnew(int client) {
    return new SickPerk(client);
}

FUNCTION(SickPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Sick");
}

FUNCTION(SickPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "吐出有害的酸液");
}

FUNCTION(SickPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你的防御力大幅降低,但可以发医生语音来吐出酸液。酸液会持续35秒或直到你死亡。");
}

FUNCTION(SickPerk, updatePermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_SICK_DEFEND);
}

FUNCTION(SickPerk, onDeath)) {
    DataPack pools = _inst.pools;
    if (pools != null) {
        pools.Reset();
        DataPackPos total_bytes = pools.Position;
        pools.Reset();
        while (pools.Position < total_bytes) {
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

FUNCTION(SickPerk, onCallForMedic)) {
    if (_inst.cooldown > 0.0) {
        return;
    }

    _inst.cooldown = ZF_SICK_COOLDOWN;
    _inst.spit_count = ZF_SICK_SPIT_COUNT;
    
    CreateTimer(0.1, timer_SpitProjectile, GetClientUserId(_inst.client), TIMER_FLAG_NO_MAPCHANGE);
}

public Action timer_SpitProjectile(Handle timer, any userid) {
    int client = GetClientOfUserId(userid);
    if (!validLivingZom(client)) return Plugin_Stop;

    BasePerk perk = g_hPerks[client];
    char perkName[32];
    perk.getName(perkName, sizeof(perkName));
    if (perk == null || !StrEqual(perkName, "Sick")) return Plugin_Stop;
    
    SickPerk sick_perk = view_as<SickPerk>(perk);
    if (sick_perk.spit_count <= 0) return Plugin_Stop;

    int projectile = CreateEntityByName("tf_projectile_jar");
    if (projectile == -1) return Plugin_Continue;

    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    DispatchSpawn(projectile);
    SetEntPropEnt(projectile, Prop_Send, "m_hOwnerEntity", client);
    SetEntProp(projectile, Prop_Send, "m_iTeamNum", zomTeam());
    SetEntityModel(projectile, ZF_SICK_PROJECTILE_MODEL);
    SetEntProp(projectile, Prop_Send, "m_nSkin", 1); // Green skin for milk carton

    float vec_forward[3];
    GetAngleVectors(eye_ang, vec_forward, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(vec_forward, ZF_SICK_PROJECTILE_SPEED);
    
    TeleportEntity(projectile, eye_pos, NULL_VECTOR, vec_forward);
    SDKHook(projectile, SDKHook_StartTouch, OnSpitTouch);
    
    sick_perk.spit_count--;
    if (sick_perk.spit_count > 0) {
        return Plugin_Continue;
    }
    
    return Plugin_Stop;
}

public void OnSpitTouch(int projectile, int other) {
    if (projectile <= 0 || !IsValidEntity(projectile)) return;

    int owner = GetEntPropEnt(projectile, Prop_Send, "m_hOwnerEntity");
    if (!validLivingZom(owner)) return;
    
    BasePerk perk = g_hPerks[owner];
    char perkName[32];
    perk.getName(perkName, sizeof(perkName));
    if (perk == null || !StrEqual(perkName, "Sick")) return;
    SickPerk sick_perk = view_as<SickPerk>(perk);

    float pos[3];
    GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", pos);

    float owner_pos[3];
    GetClientAbsOrigin(owner, owner_pos);
    float offset[3];
    SubtractVectors(pos, owner_pos, offset);
    int pool_entity = fxCreateParticle("effects/slime_splash_01.pcf", owner, AttachNone, 2.0, offset);
    if (IsValidEdict(pool_entity)) {
        sick_perk.pools.WriteCell(pool_entity);
        sick_perk.pools.WriteFloat(GetGameTime());
    }

    RemoveEdict(projectile);
    SDKUnhook(projectile, SDKHook_StartTouch, OnSpitTouch);
}

FUNCTION(SickPerk, onPeriodic)) {
    if (_inst.cooldown > 0.0) {
        _inst.cooldown = fMax(0.0, _inst.cooldown - 1.0);
    }

    DataPack pools = _inst.pools;
    if (pools == null) return;

    DataPackPos total_bytes = pools.Position;
    DataPack new_pools = new DataPack();
    pools.Reset();
    while (pools.Position < total_bytes) {
        int pool_entity = pools.ReadCell();
        float creation_time = pools.ReadFloat();

        if (!IsValidEdict(pool_entity) || (GetGameTime() - creation_time) > ZF_SICK_LIFETIME) {
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
                float distance = GetVectorDistance(pool_pos, sur_pos);
                if (distance <= ZF_SICK_DAMAGE_RADIUS) {
                    float dist_zom_pool[3];
                    GetClientAbsOrigin(_inst.client, dist_zom_pool);
                    float damage_dist = GetVectorDistance(dist_zom_pool, pool_pos);
                    float damage = ZF_SICK_DAMAGE * (damage_dist / ZF_SICK_MAX_DIST);
                    SDKHooks_TakeDamage(j, _inst.client, _inst.client, damage, DMG_POISON);
                }
            }
        }
    }
    delete _inst.pools;
    _inst.pools = new_pools;
}

FUNCTION(SickPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown > 0.0) {
        Format(buffer, maxlen, "Spit Cooldown: %.0fs", _inst.cooldown);
    } else {
        Format(buffer, maxlen, "Spit Ready");
    }
}
