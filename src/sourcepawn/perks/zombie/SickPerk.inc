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
#define ZF_SICK_DEFEND               -75
#define ZF_SICK_DAMAGE               3.0
#define ZF_SICK_DAMAGE_RADIUS        150.0
#define ZF_SICK_MAX_DIST             2000.0
#define ZF_SICK_COOLDOWN             15.0
#define ZF_SICK_LIFETIME             15.0
#define ZF_SICK_SPIT_COUNT           5
#define ZF_SICK_SPIT_INTERVAL        0.2
#define ZF_SICK_PROJECTILE_SPEED     1600.0
#define ZF_SICK_PROJECTILE_MODEL     "models/props_halloween/hwn_flask_vial.mdl"
// Data slots
#define SICK_DATA_COOLDOWN           (PERK_DATA_START_INDEX + 0)
#define SICK_DATA_POOLS_HANDLE       (PERK_DATA_START_INDEX + 1)
#define SICK_DATA_SPIT_COUNT         (PERK_DATA_START_INDEX + 2)
#define SICK_DATA_SPIT_TIMER_HANDLE  (PERK_DATA_START_INDEX + 3)
#define SICK_DATA_PROJECTILES_HANDLE (PERK_DATA_START_INDEX + 4)

methodmap SickPerk < ZombieBasePerk
{

public     SickPerk(int client)
    {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        SickPerk       sm      = view_as<SickPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, SickPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SickPerkFupdatePermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, SickPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, SickPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, SickPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, SickPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, SickPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, SickPerkFonPlayerSpawn);

        sm.cooldown          = 0.0;
        sm.pools             = new DataPack();
        sm.spit_timer_handle = null;    // 初始化计时器句柄
        sm.projectiles       = new DataPack();

        return sm;
    }

    property float cooldown
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_COOLDOWN);
            return this.ReadFloat();
        }

public         set(float value)
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }

    property DataPack pools
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_POOLS_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }

public         set(DataPack value)
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_POOLS_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }

    property int spit_count
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_COUNT);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_COUNT);
            this.WriteCell(value);
        }
    }
    property Handle spit_timer_handle
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_TIMER_HANDLE);
            return view_as<Handle>(this.ReadCell());
        }

public         set(Handle value)
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_SPIT_TIMER_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }

    property DataPack projectiles
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_PROJECTILES_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }

public         set(DataPack value)
        {
            this.Position = view_as<DataPackPos>(SICK_DATA_PROJECTILES_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }
}

stock BasePerk SickPerkFnew(int client)
{
    return new SickPerk(client);
}

FUNCTION(SickPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Sick");
}

FUNCTION(SickPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "SickPerk_shortdesc");
}

FUNCTION(SickPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "SickPerk_desc");
}

FUNCTION_INT(SickPerk, getCategory))
{
    return 1;
}

FUNCTION(SickPerk, updatePermStats))
{
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_SICK_DEFEND);
}

// 实现函数
FUNCTION(SickPerk, onPlayerSpawn))
{
    // 如果 pools 句柄是 null (因为死亡被删除了)，就重新创建一个
    if (_inst.pools == null)
    {
        _inst.pools = new DataPack();
    }
    if (_inst.projectiles == null)
    {
        _inst.projectiles = new DataPack();
    }
    // 重置吐痰计数器
    _inst.spit_count = 0;
    _inst.cooldown   = 0.0;    // 重置冷却时间
    if (_inst.spit_timer_handle != null)
    {
        KillTimer(_inst.spit_timer_handle);
        _inst.spit_timer_handle = null;
    }
}

FUNCTION(SickPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    // --- 优化: 清理逻辑简化 ---
    DataPack pools = _inst.pools;
    if (pools != null && pools != INVALID_HANDLE)
    {
        pools.Reset();
        while (pools.IsReadable())
        {
            int pool_entity = pools.ReadCell();
            if (IsValidEdict(pool_entity))
            {
                AcceptEntityInput(pool_entity, "Kill");
            }
            pools.ReadFloat();
        }
        // Don't delete the datapack, just clear it for reuse.
        pools.Reset();
    }

    DataPack projectiles = _inst.projectiles;
    if (projectiles != null && projectiles != INVALID_HANDLE)
    {
        projectiles.Reset();
        while (projectiles.IsReadable())
        {
            int projectile_entity = projectiles.ReadCell();
            if (IsValidEdict(projectile_entity))
            {
                AcceptEntityInput(projectile_entity, "Kill");
            }
            projectiles.ReadFloat();    // x
            projectiles.ReadFloat();    // y
            projectiles.ReadFloat();    // z
        }
        projectiles.Reset();
    }
    // --- 修正: 同样需要杀掉吐痰计时器 ---
    if (_inst.spit_timer_handle != null)
    {
        KillTimer(_inst.spit_timer_handle);
        _inst.spit_timer_handle = null;
    }
}

FUNCTION(SickPerk, onCallForMedic))
{
    if (_inst.cooldown > 0.0)
    {
        return;
    }
    // --- 修正: 确保没有正在进行的吐痰序列 ---
    if (_inst.spit_count > 0 || _inst.spit_timer_handle != null)
    {
        return;
    }
    _inst.cooldown   = ZF_SICK_COOLDOWN;
    _inst.spit_count = ZF_SICK_SPIT_COUNT;

    // --- 修正: 创建一个重复的计时器，并保存句柄 ---
    DataPack pack    = new DataPack();
    pack.WriteCell(GetClientUserId(_inst.client));
    _inst.spit_timer_handle = CreateTimer(ZF_SICK_SPIT_INTERVAL, timer_SpitProjectile, pack, TIMER_REPEAT);
}

public Action timer_SpitProjectile(Handle timer, DataPack pack)
{
    pack.Reset();
    int userid = pack.ReadCell();
    int client = GetClientOfUserId(userid);

    // --- 修正: 增加更严格的检查 ---
    if (!validLivingZom(client))
    {
        pack.Reset();    // 确保DataPack在删除前被重置
        delete pack;
        return Plugin_Stop;
    }

    BasePerk perk = g_hPerks[client];
    if (perk == null)
    {
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }

    if (!isClientPerkNameEquals(client, "Sick"))
    {
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }

    SickPerk sick_perk = view_as<SickPerk>(perk);
    if (sick_perk.spit_count <= 0)
    {
        sick_perk.spit_timer_handle = null;    // 清理句柄
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }

    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    eye_ang[0] += GetRandomFloat(-20.0, -10.0);    // Pitch (-90.0 is up, 90.0 is down)
    eye_ang[1] += GetRandomFloat(-6.0, 6.0);       // Yaw

    int color[3]   = { 100, 255, 100 };    // Sickly green

    int item_index = getFreeItemIndex(client, ZF_SICK_SPIT_COUNT);
    if (item_index == -1)
    {
        // This should ideally not happen if logic is correct, but as a safeguard:
        PrintHintText(client, "Too many projectiles active.");
        return Plugin_Continue;
    }

    int projectile = fxCreateModelThrown(ZF_SICK_PROJECTILE_MODEL, client, eye_pos, eye_ang, ZF_SICK_PROJECTILE_SPEED, color);

    if (projectile == -1)
    {
        return Plugin_Continue;
    }

    setItemMetadata(projectile, item_index);    // Store our item index in the item's metadata
    zf_item[client][item_index] = projectile;

    // 将投射物添加到追踪列表
    DataPack projectiles        = sick_perk.projectiles;
    projectiles.WriteCell(projectile);
    projectiles.WriteFloat(eye_pos[0]);
    projectiles.WriteFloat(eye_pos[1]);
    projectiles.WriteFloat(eye_pos[2]);

    sick_perk.spit_count--;

    // 当 spit_count 减为0时，这是最后一次，所以返回 Stop
    if (sick_perk.spit_count <= 0)
    {
        sick_perk.spit_timer_handle = null;    // 清理句柄
        pack.Reset();
        delete pack;
        return Plugin_Stop;
    }

    return Plugin_Continue;
}

stock void createAcidPool(int owner, int projectile, const float pos[3], const float hitAng[3])
{
    if (!validLivingZom(owner)) return;

    SickPerk sick_perk = view_as<SickPerk>(g_hPerks[owner]);
    if (sick_perk == null) return;

    fxCreateParticle("effects/slime_splash_01.pcf", owner, AttachNone, ZF_SICK_LIFETIME, pos);
    float hitAngTmp[3] = { 0.0, 0.0, 0.0 };
    hitAngTmp[0]       = 90.0 + hitAng[0];    // Adjust for ZFMDL_HAYBALE
    hitAngTmp[1]       = hitAng[1];
    hitAngTmp[2]       = hitAng[2];
    int pool_entity    = fxCreateModelStatic(ZFMDL_HAYBALE, owner, true, false, pos, hitAngTmp, { 255, 215, 0 });

    if (IsValidEdict(pool_entity))
    {
        sick_perk.pools.WriteCell(pool_entity);
        sick_perk.pools.WriteFloat(GetGameTime());
    }

    // Clean up the projectile from zf_item array
    int item_index = getItemMetadata(projectile);
    if (item_index >= 0 && item_index < ZF_SICK_SPIT_COUNT)
    {
        zf_item[owner][item_index] = -1;
    }

    if (IsValidEdict(projectile))
    {
        AcceptEntityInput(projectile, "Kill");
    }
}

public Action Timer_RemoveProjectile(Handle timer, any entity)
{
    if (IsValidEntity(entity))
    {
        RemoveEdict(entity);
    }
    return Plugin_Stop;
}

FUNCTION(SickPerk, onPeriodic))
{
    if (_inst.cooldown > 0.0)
    {
        _inst.cooldown = fMax(0.0, _inst.cooldown - 1.0);
    }

    DataPack pools = _inst.pools;
    if (pools == null || pools == INVALID_HANDLE) return;

    // --- 优化: 使用临时DataPack来重建，而不是边读边写 ---
    DataPack new_pools = new DataPack();
    pools.Reset();
    while (pools.IsReadable())
    {
        int   pool_entity   = pools.ReadCell();
        float creation_time = pools.ReadFloat();

        if (!IsValidEdict(pool_entity) || (GetGameTime() - creation_time) > ZF_SICK_LIFETIME)
        {
            if (IsValidEdict(pool_entity))
            {
                AcceptEntityInput(pool_entity, "Kill");
            }
            continue;
        }

        new_pools.WriteCell(pool_entity);
        new_pools.WriteFloat(creation_time);

        float pool_pos[3];
        GetEntPropVector(pool_entity, Prop_Data, "m_vecOrigin", pool_pos);

        for (int j = 1; j <= MaxClients; j++)
        {
            if (validLivingSur(j))
            {
                float sur_pos[3];
                GetClientAbsOrigin(j, sur_pos);
                if (GetVectorDistance(pool_pos, sur_pos) <= ZF_SICK_DAMAGE_RADIUS)
                {
                    fxCreateSoundToClient(ZFSND_SICK, j);
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

FUNCTION(SickPerk, onGameFrame))
{
    // --- Projectile collision detection ---
    DataPack projectiles = _inst.projectiles;
    if (projectiles == null || projectiles == INVALID_HANDLE)
    {
        return;
    }

    DataPack kept_projectiles = new DataPack();
    projectiles.Reset();
    float prev_pos[3], current_pos[3], hit_pos[3], hit_vec[3];

    if (projectiles.IsReadable())
    {
    }

    while (projectiles.IsReadable())
    {
        int projectile = projectiles.ReadCell();
        prev_pos[0]    = projectiles.ReadFloat();
        prev_pos[1]    = projectiles.ReadFloat();
        prev_pos[2]    = projectiles.ReadFloat();

        if (!IsValidEdict(projectile))
        {
            continue;    // 投射物已失效
        }

        // 检查模型名称，确保它仍然是我们期望的投射物
        if (!HasEntProp(projectile, Prop_Data, "m_ModelName"))
        {
            continue;
        }
        char model_name[PLATFORM_MAX_PATH];
        GetEntPropString(projectile, Prop_Data, "m_ModelName", model_name, sizeof(model_name));
        if (strcmp(model_name, ZF_SICK_PROJECTILE_MODEL) != 0)
        {
            continue;
        }

        GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", current_pos);
        // 检查最大距离
        float owner_pos[3];
        GetClientAbsOrigin(_inst.client, owner_pos);
        if (GetVectorDistance(owner_pos, current_pos) > ZF_SICK_MAX_DIST)
        {
            AcceptEntityInput(projectile, "Kill");
            continue;
        }

        if (doItemCollide(projectile, prev_pos, hit_pos, hit_vec))
        {
            float hit_ang[3];
            GetVectorAngles(hit_vec, hit_ang);
            createAcidPool(_inst.client, projectile, hit_pos, hit_ang);
            // 不再追踪此投射物
        }
        else {
            // 未碰撞，继续追踪
            kept_projectiles.WriteCell(projectile);
            kept_projectiles.WriteFloat(current_pos[0]);
            kept_projectiles.WriteFloat(current_pos[1]);
            kept_projectiles.WriteFloat(current_pos[2]);
        }
    }

    // Clear the original pack and copy the kept projectiles back into it
    projectiles.Reset();
    kept_projectiles.Reset();
    while (kept_projectiles.IsReadable())
    {
        projectiles.WriteCell(kept_projectiles.ReadCell());
        projectiles.WriteFloat(kept_projectiles.ReadFloat());
        projectiles.WriteFloat(kept_projectiles.ReadFloat());
        projectiles.WriteFloat(kept_projectiles.ReadFloat());
    }
    delete kept_projectiles;
}

FUNCTION(SickPerk, updateCondStats), char[] buffer, int maxlen)
{
    if (_inst.cooldown > 0.0)
    {
        Format(buffer, maxlen, "%t", "SickPerk_Status_Cooldown", _inst.cooldown);
    }
    else {
        Format(buffer, maxlen, "%t", "SickPerk_Status_Ready");
    }
}
