#if defined __SickPerk_included
#endinput
#endif
#define __SickPerk_included

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
#define ZF_SICK_DAMAGE 5.0
#define ZF_SICK_DAMAGE_RADIUS 150.0
#define ZF_SICK_MAX_DIST 1000.0
#define ZF_SICK_COOLDOWN 15.0
#define ZF_SICK_LIFETIME 15.0
#define ZF_SICK_SPIT_COUNT 5
#define ZF_SICK_SPIT_INTERVAL 0.2
#define ZF_SICK_PROJECTILE_SPEED 900.0
#define ZF_SICK_PROJECTILE_MODEL "models/props_junk/garbage_milkcarton002a.mdl"
// Data slots
#define SICK_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define SICK_DATA_POOLS_HANDLE (PERK_DATA_START_INDEX + 1)
#define SICK_DATA_SPIT_COUNT (PERK_DATA_START_INDEX + 2)
#define SICK_DATA_SPIT_TIMER_HANDLE (PERK_DATA_START_INDEX + 3)
#define SICK_DATA_PROJECTILES_HANDLE (PERK_DATA_START_INDEX + 4)

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
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, SickPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, SickPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, SickPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, SickPerkFonPlayerSpawn);

        sm.cooldown = 0.0;
        sm.pools = new DataPack();
        sm.spit_timer_handle = null; // 初始化计时器句柄
        sm.projectiles = new DataPack();

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
    property Handle spit_timer_handle {
        public get() {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_TIMER_HANDLE);
            return view_as<Handle>(this.ReadCell());
        }
        public set(Handle value) {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_TIMER_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }

    property DataPack projectiles {
        public get() {
            this.Position = view_as<DataPackPos>(SICK_DATA_PROJECTILES_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(SICK_DATA_PROJECTILES_HANDLE);
            this.WriteCell(view_as<int>(value));
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
    strcopy(buffer, maxlen, "SickPerk_shortdesc");
}

FUNCTION(SickPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "SickPerk_desc");
}

FUNCTION(SickPerk, updatePermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_SICK_DEFEND);
}

// 实现函数
FUNCTION(SickPerk, onPlayerSpawn)) {
    // 如果 pools 句柄是 null (因为死亡被删除了)，就重新创建一个
    if (_inst.pools == null) {
        _inst.pools = new DataPack();
    }
    if (_inst.projectiles == null) {
        _inst.projectiles = new DataPack();
    }
    // 重置吐痰计数器
    _inst.spit_count = 0;
    if (_inst.spit_timer_handle != null) {
        KillTimer(_inst.spit_timer_handle);
        _inst.spit_timer_handle = null;
    }
}

FUNCTION(SickPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    // --- 优化: 清理逻辑简化 ---
    DataPack pools = _inst.pools;
    if (pools != null && pools != INVALID_HANDLE) {
        pools.Reset();
        while (pools.IsReadable()) {
            int pool_entity = pools.ReadCell();
            if (IsValidEdict(pool_entity)) {
                AcceptEntityInput(pool_entity, "Kill");
            }
            pools.ReadFloat();
        }
        // Don't delete the datapack, just clear it for reuse.
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
    // --- 修正: 同样需要杀掉吐痰计时器 ---
    if (_inst.spit_timer_handle != null) {
        KillTimer(_inst.spit_timer_handle);
        _inst.spit_timer_handle = null;
    }
}

FUNCTION(SickPerk, onCallForMedic)) {
    if (_inst.cooldown > 0.0) {
        return;
    }
 // --- 修正: 确保没有正在进行的吐痰序列 ---
    if (_inst.spit_count > 0 || _inst.spit_timer_handle != null) {
        return;
    }
    _inst.cooldown = ZF_SICK_COOLDOWN;
    _inst.spit_count = ZF_SICK_SPIT_COUNT;
    
    // --- 修正: 创建一个重复的计时器，并保存句柄 ---
    DataPack pack = new DataPack();
    pack.WriteCell(GetClientUserId(_inst.client));
    _inst.spit_timer_handle = CreateTimer(ZF_SICK_SPIT_INTERVAL, timer_SpitProjectile, pack, TIMER_REPEAT);
}
public Action timer_SpitProjectile(Handle timer, DataPack pack) {
    pack.Reset();
    int userid = pack.ReadCell();
    int client = GetClientOfUserId(userid);

    // --- 修正: 增加更严格的检查 ---
    if (!validLivingZom(client)) {
        pack.Reset(); // 确保DataPack在删除前被重置
        delete pack;
        return Plugin_Stop;
    }

    BasePerk perk = g_hPerks[client];
    if (perk == null) {
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }
    
    char perkName[32];
    perk.getName(perkName, sizeof(perkName));
    if (!StrEqual(perkName, "Sick")) {
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }
    
    SickPerk sick_perk = view_as<SickPerk>(perk);
    if (sick_perk.spit_count <= 0) {
        sick_perk.spit_timer_handle = null; // 清理句柄
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }

    int projectile = CreateEntityByName("tf_projectile_jar");
    if (projectile == -1) {
        return Plugin_Continue;
    }

    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    eye_ang[0] += GetRandomFloat(-20.0, -10.0); // Pitch (-90.0 is up, 90.0 is down)
    eye_ang[1] += GetRandomFloat(-6.0, 6.0);    // Yaw
    DispatchSpawn(projectile);
    SetEntPropEnt(projectile, Prop_Send, "m_hOwnerEntity", client);
    SetEntProp(projectile, Prop_Send, "m_iTeamNum", zomTeam());
    SetEntityModel(projectile, ZF_SICK_PROJECTILE_MODEL);
    SetEntProp(projectile, Prop_Send, "m_nSkin", 1);

    float vec_forward[3];
    GetAngleVectors(eye_ang, vec_forward, NULL_VECTOR, NULL_VECTOR);
    
    // --- 修正开始 ---
    
    // 1. 计算生成点 (spawn_pos)
    float spawn_offset[3];
    // 复制前向向量到 offset
    spawn_offset = vec_forward; 
    // 原地缩放 offset
    ScaleVector(spawn_offset, 32.0); 
    
    float spawn_pos[3];
    // spawn_pos = eye_pos + spawn_offset
    AddVectors(eye_pos, spawn_offset, spawn_pos); 
    
    // 2. 计算速度向量 (velocity_vec)
    float velocity_vec[3];
    // 复制前向向量到 velocity_vec
    velocity_vec = vec_forward; 
    
    // 原地缩放 velocity_vec
    ScaleVector(velocity_vec, ZF_SICK_PROJECTILE_SPEED);
    
    // 3. 在 spawn_pos 处生成，并赋予速度 velocity_vec
    TeleportEntity(projectile, spawn_pos, NULL_VECTOR, velocity_vec);
    
    // --- 修正结束 ---
    
    // 将投射物添加到追踪列表
    DataPack projectiles = sick_perk.projectiles;
    projectiles.WriteCell(projectile);
    projectiles.WriteFloat(spawn_pos[0]);
    projectiles.WriteFloat(spawn_pos[1]);
    projectiles.WriteFloat(spawn_pos[2]);
    
    sick_perk.spit_count--;
    
    // 当 spit_count 减为0时，这是最后一次，所以返回 Stop
    if (sick_perk.spit_count <= 0) {
        sick_perk.spit_timer_handle = null; // 清理句柄
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }
    
    return Plugin_Continue;
}

stock void createAcidPool(int owner, int projectile, const float pos[3], const float hitAng[3]) {
    if (!validLivingZom(owner)) return;

    SickPerk sick_perk = view_as<SickPerk>(g_hPerks[owner]);
    if (sick_perk == null) return;

    fxCreateParticle("effects/slime_splash_01.pcf", owner, AttachNone, ZF_SICK_LIFETIME, pos);
    float hitAngTmp[3] = {0.0, 0.0, 0.0};
    hitAngTmp[0] = 90.0 + hitAng[0]; // Adjust for ZFMDL_HAYBALE
    hitAngTmp[1] = hitAng[1];
    hitAngTmp[2] = hitAng[2];
    int pool_entity = fxCreateModelStatic(ZFMDL_HAYBALE, owner, true, false, pos, hitAngTmp, {255, 215, 0});
    
    if (IsValidEdict(pool_entity)) {
        sick_perk.pools.WriteCell(pool_entity);
        sick_perk.pools.WriteFloat(GetGameTime());
    } 

    if (IsValidEdict(projectile)) {
        AcceptEntityInput(projectile, "Kill");
    }
}

public Action Timer_RemoveProjectile(Handle timer, any entity) {
    if (IsValidEntity(entity)) {
        RemoveEdict(entity);
    }
    return Plugin_Stop;
}

FUNCTION(SickPerk, onPeriodic)) {
    if (_inst.cooldown > 0.0) {
        _inst.cooldown = fMax(0.0, _inst.cooldown - 1.0);
    }

    DataPack pools = _inst.pools;
    if (pools == null || pools == INVALID_HANDLE) return;

    // --- 优化: 使用临时DataPack来重建，而不是边读边写 ---
    DataPack new_pools = new DataPack();
    pools.Reset();
    while (pools.IsReadable()) {
        int pool_entity = pools.ReadCell();
        float creation_time = pools.ReadFloat();

        if (!IsValidEdict(pool_entity) || (GetGameTime() - creation_time) > ZF_SICK_LIFETIME) {
            if (IsValidEdict(pool_entity)) {
                AcceptEntityInput(pool_entity, "Kill");
            }
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
                if (GetVectorDistance(pool_pos, sur_pos) <= ZF_SICK_DAMAGE_RADIUS) {
                    float zom_pos[3];
                    GetClientAbsOrigin(_inst.client, zom_pos);
                    // 确保伤害不会超过基础值
                    float damage = ZF_SICK_DAMAGE;
                    SDKHooks_TakeDamage(j, _inst.client, _inst.client, damage, DMG_POISON);
                }
            }
        }
    }
    delete _inst.pools;
    _inst.pools = new_pools;

}

FUNCTION(SickPerk, onGameFrame)) {
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

        // --- 修复: 增加实体类型检查 ---
        // 检查实体类名，确保它仍然是我们期望的投射物，防止因实体索引重用而导致崩溃。
        char classname[64];
        GetEdictClassname(projectile, classname, sizeof(classname));
        if (!StrEqual(classname, "tf_projectile_jar")) {
            continue; // 实体已不再是投射物，跳过
        }

        GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", current_pos);

        // 检查最大距离
        float owner_pos[3];
        GetClientAbsOrigin(_inst.client, owner_pos);
        if (GetVectorDistance(owner_pos, current_pos) > ZF_SICK_MAX_DIST) {
            AcceptEntityInput(projectile, "Kill");
            continue;
        }

        if (doItemCollide(projectile, prev_pos, hit_pos, hit_vec)) {
            float hit_ang[3];
            GetVectorAngles(hit_vec, hit_ang);
            createAcidPool(_inst.client, projectile, hit_pos, hit_ang);
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

FUNCTION(SickPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown > 0.0) {
Format(buffer, maxlen, "%t", "SickPerk_Status_Cooldown", _inst.cooldown);
    } else {
Format(buffer, maxlen, "%t", "SickPerk_Status_Ready");
    }
}
