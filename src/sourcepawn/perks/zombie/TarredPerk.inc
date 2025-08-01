#if defined __TarredPerk_included
#endinput
#endif
#define __TarredPerk_included

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
#define ZF_TARRED_MAX_DIST 1000.0

// Data slots
#define TARRED_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define TARRED_DATA_POOLS_HANDLE (PERK_DATA_START_INDEX + 1)
#define TARRED_DATA_PROJECTILES_HANDLE (PERK_DATA_START_INDEX + 2)

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
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, TarredPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, TarredPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, TarredPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, TarredPerkFonPlayerSpawn);

        sm.cooldown = 0.0;
        sm.pools = new DataPack();
        sm.projectiles = new DataPack();

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

    property DataPack projectiles {
        public get() {
            this.Position = view_as<DataPackPos>(TARRED_DATA_PROJECTILES_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(TARRED_DATA_PROJECTILES_HANDLE);
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
    strcopy(buffer, maxlen, "TarredPerk_shortdesc");
}

FUNCTION(TarredPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "TarredPerk_desc");
}

FUNCTION(TarredPerk, onPlayerSpawn)) {
    if (_inst.pools == null) {
        _inst.pools = new DataPack();
    }
    if (_inst.projectiles == null) {
        _inst.projectiles = new DataPack();
    }
}

FUNCTION(TarredPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    DataPack pools = _inst.pools;
    if (pools != null && pools != INVALID_HANDLE) {
        pools.Reset();
        while (pools.IsReadable()) {
            int pool_entity = pools.ReadCell();
            if (IsValidEdict(pool_entity)) {
                AcceptEntityInput(pool_entity, "Kill");
            }
            pools.ReadFloat(); // Keep pack structure consistent
        }
        pools.Reset();
    }

    DataPack projectiles = _inst.projectiles;
    if (projectiles != null && projectiles != INVALID_HANDLE) {
        projectiles.Reset();
        while (projectiles.IsReadable()) {
            int projectile_entity = projectiles.ReadCell();
            if (IsValidEdict(projectile_entity)) {
                AcceptEntityInput(projectile_entity, "Kill");
            }
            projectiles.ReadFloat(); // x
            projectiles.ReadFloat(); // y
            projectiles.ReadFloat(); // z
        }
        projectiles.Reset();
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
    
    float spawn_offset[3];
    spawn_offset = vec_forward;
    ScaleVector(spawn_offset, 32.0);
    
    float spawn_pos[3];
    AddVectors(eye_pos, spawn_offset, spawn_pos);
    
    float velocity_vec[3];
    velocity_vec = vec_forward;
    ScaleVector(velocity_vec, ZF_TARRED_PROJECTILE_SPEED);
    
    TeleportEntity(projectile, spawn_pos, NULL_VECTOR, velocity_vec);
    
    TarredPerk tarred_perk = view_as<TarredPerk>(_inst);
    DataPack projectiles = tarred_perk.projectiles;
    projectiles.WriteCell(projectile);
    projectiles.WriteFloat(spawn_pos[0]);
    projectiles.WriteFloat(spawn_pos[1]);
    projectiles.WriteFloat(spawn_pos[2]);
}

stock void createTarPool(int owner, int projectile, const float pos[3], const float hitAng[3]) {
    if (!validLivingZom(owner)) return;

    TarredPerk tarred_perk = view_as<TarredPerk>(g_hPerks[owner]);
    if (tarred_perk == null) return;

    fxCreateParticle("effects/slime_splash_01.pcf", owner, AttachNone, ZF_SICK_LIFETIME, pos);

    float hitAngTmp[3] = {0.0, 0.0, 0.0};
    hitAngTmp[0] = 90.0 + hitAng[0]; // Adjust for ZFMDL_HAYBALE
    hitAngTmp[1] = hitAng[1];
    hitAngTmp[2] = hitAng[2];
    int pool_entity = fxCreateModelStatic(ZFMDL_HAYBALE, owner, true, false, pos, hitAngTmp, {25, 25, 25});
    if (IsValidEdict(pool_entity)) {
        tarred_perk.pools.WriteCell(pool_entity);
        tarred_perk.pools.WriteFloat(GetGameTime());
    }

    if (IsValidEdict(projectile)) {
        AcceptEntityInput(projectile, "Kill");
    }
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
Format(buffer, maxlen, "%t", "TarredPerk_Status_Cooldown", _inst.cooldown);
    } else {
Format(buffer, maxlen, "%t", "TarredPerk_Status_Ready");
    }
}



FUNCTION(TarredPerk, onGameFrame)) {
    // --- 投射物碰撞检测 ---
    DataPack projectiles = _inst.projectiles;
    if (projectiles == null || projectiles == INVALID_HANDLE) return;

    DataPack new_projectiles = new DataPack();
    projectiles.Reset();
    float prev_pos[3], current_pos[3], hit_pos[3], hit_vec[3];

    while (projectiles.IsReadable()) {
        int projectile = projectiles.ReadCell();
        prev_pos[0] = projectiles.ReadFloat();
        prev_pos[1] = projectiles.ReadFloat();
        prev_pos[2] = projectiles.ReadFloat();

        if (!IsValidEdict(projectile)) {
            continue; // 投射物已失效
        }

        GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", current_pos);

        // 检查最大距离
        float owner_pos[3];
        GetClientAbsOrigin(_inst.client, owner_pos);
        if (GetVectorDistance(owner_pos, current_pos) > ZF_TARRED_MAX_DIST) {
            AcceptEntityInput(projectile, "Kill");
            continue;
        }

        if (doItemCollide(projectile, prev_pos, hit_pos, hit_vec)) {
            float hit_ang[3];
            GetVectorAngles(hit_vec, hit_ang);
            createTarPool(_inst.client, projectile, hit_pos, hit_ang);
            // 不再追踪此投射物
        } else {
            // 未碰撞，继续追踪
            new_projectiles.WriteCell(projectile);
            new_projectiles.WriteFloat(current_pos[0]);
            new_projectiles.WriteFloat(current_pos[1]);
            new_projectiles.WriteFloat(current_pos[2]);
        }
    }

    delete _inst.projectiles;
    _inst.projectiles = new_projectiles;
}
