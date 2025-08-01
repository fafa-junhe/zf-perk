#if defined __CorruptorPerk_included
    #endinput
#endif
#define __CorruptorPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include <sdkhooks>

// Perk constants from new_perks.md
#define CORRUPTOR_SPEED                 -10    // "移动速度降低"
#define CORRUPTOR_BUILDING_DAMAGE_BONUS 50     // "对工程师的建筑造成额外伤害"
#define CORRUPTOR_HIT_COUNT             1
#define CORRUPTOR_CORRUPTED_LIFETIME    15.0

// Data slots
#define CORRUPTOR_DATA_TARGET_BUILDING  (PERK_DATA_START_INDEX + 0)
#define CORRUPTOR_DATA_LAST_HIT_TIME    (PERK_DATA_START_INDEX + 1)
#define CORRUPTOR_DATA_HIT_COUNT        (PERK_DATA_START_INDEX + 2)

methodmap CorruptorPerk < ZombieBasePerk
{

public     CorruptorPerk(int client)
    {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CorruptorPerk  sm      = view_as<CorruptorPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CorruptorPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CorruptorPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CorruptorPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CorruptorPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_BUILDING_TAKE_DAMAGE, CorruptorPerkFonBuildingTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CorruptorPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, CorruptorPerkFonPlayerSpawn);

        sm.target_building = -1;
        sm.last_hit_time   = 0.0;
        sm.hit_count       = 0;

        return sm;
    }

    property int target_building
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_TARGET_BUILDING);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_TARGET_BUILDING);
            this.WriteCell(value);
        }
    }

    property float last_hit_time
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_LAST_HIT_TIME);
            return this.ReadFloat();
        }

public         set(float value)
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_LAST_HIT_TIME);
            this.WriteFloat(value);
        }
    }

    property int hit_count
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_HIT_COUNT);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_HIT_COUNT);
            this.WriteCell(value);
        }
    }
}

stock BasePerk CorruptorPerkFnew(int client)
{
    return new CorruptorPerk(client);
}

FUNCTION(CorruptorPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Corruptor");
}

FUNCTION(CorruptorPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "CorruptorPerk_shortdesc");
}

FUNCTION(CorruptorPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "CorruptorPerk_desc");
}

FUNCTION(CorruptorPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, CORRUPTOR_SPEED);
}

FUNCTION(CorruptorPerk, onPlayerSpawn))
{
    _inst.target_building = -1;
    _inst.last_hit_time   = 0.0;
    _inst.hit_count       = 0;
}

stock bool IsBuilding(int entity)
{
    if (entity <= 0 || !IsValidEntity(entity))
    {
        return false;
    }
    char classname[64];
    GetEdictClassname(entity, classname, sizeof(classname));
    return (StrContains(classname, "obj_", false) != -1);
}

stock void doCorruptBuilding(int owner, int building)
{
    // Get original sentry's properties
    float vOrigin[3];
    getEntityPos(building, vOrigin);
    int level = GetEntProp(building, Prop_Send, "m_nModelIndex");

    // Destroy the original building
    AcceptEntityInput(building, "Kill");

    // Create a new sentry
    int newSentry = CreateEntityByName("obj_sentrygun");

    if (newSentry == -1) return;

    // Prepare data for the setup timer
    DataPack pack = new DataPack();
    pack.WriteCell(newSentry);
    pack.WriteCell(owner);
    pack.WriteCell(level);
    pack.WriteFloat(vOrigin[0]);
    pack.WriteFloat(vOrigin[1]);
    pack.WriteFloat(vOrigin[2]);

    // Use a timer to finish setup after the entity has spawned
    CreateTimer(0.1, timer_FinishSentrySetup, pack, TIMER_FLAG_NO_MAPCHANGE);
}

public Action timer_FinishSentrySetup(Handle timer, DataPack pack)
{
    pack.Reset();
    int   sentry = pack.ReadCell();
    int   owner  = pack.ReadCell();
    int   level  = pack.ReadCell();
    float vOrigin[3];
    vOrigin[0] = pack.ReadFloat();
    vOrigin[1] = pack.ReadFloat();
    vOrigin[2] = pack.ReadFloat();

    if (!IsValidEntity(sentry))
    {
        delete pack;
        return Plugin_Stop;
    }

    // Set position and angles

    // Set properties
    DispatchSpawn(sentry);
    TeleportEntity(sentry, vOrigin, NULL_VECTOR, NULL_VECTOR);
    SetEntityModel(sentry, "models/buildables/sentry1.mdl");

    SetEntProp(sentry, Prop_Send, "m_bPlacing", 0, 2);
    SetEntProp(sentry, Prop_Send, "m_bBuilding", 0, 2);
    SetEntProp(sentry, Prop_Send, "m_bDisabled", 0, 2);
    SetEntProp(sentry, Prop_Send, "m_iObjectType", 3);
    SetEntProp(sentry, Prop_Send, "m_iState", 1);
    SetEntProp(sentry, Prop_Send, "m_iTeamNum", 3);
    SetEntProp(sentry, Prop_Send, "m_iUpgradeMetal", 150);
    SetEntProp(sentry, Prop_Send, "m_bHasSapper", 0, 2);
    SetEntProp(sentry, Prop_Send, "m_hBuilder", owner, 1);
    SetEntProp(sentry, Prop_Send, "m_bServerOverridePlacement", 1, 1);
    SetEntPropFloat(sentry, Prop_Send, "m_flPercentageConstructed", 1.0);
    SetEntProp(sentry, Prop_Send, "m_iUpgradeLevel", 1, 4);    // 1-3
    SetEntProp(sentry, Prop_Send, "m_nModelIndex", level, 2);
    SetEntProp(sentry, Prop_Send, "m_iAmmoShells", 100, 4);
    SetEntProp(sentry, Prop_Send, "m_iMaxHealth", 150, 4);
    SetEntProp(sentry, Prop_Send, "m_nSkin", 1);

    // Create a timer to destroy the new building
    DataPack packDestroy = new DataPack();
    packDestroy.WriteCell(sentry);
    CreateTimer(CORRUPTOR_CORRUPTED_LIFETIME, timer_DestroyBuilding, packDestroy, TIMER_FLAG_NO_MAPCHANGE);

    PrintHintText(owner, "%t", "CorruptorPerk_Hint_Corrupted");

    delete pack;
    return Plugin_Stop;
}

public Action timer_DestroyBuilding(Handle timer, DataPack pack)
{
    pack.Reset();
    int building = pack.ReadCell();
    if (IsValidEntity(building))
    {
        AcceptEntityInput(building, "Kill");
    }
    delete pack;
    return Plugin_Stop;
}

FUNCTION_FLOAT(CorruptorPerk, onBuildingTakeDamage), int iBuilding, int iAttacker, int iInflictor, float flDamage, int iDamagetype, int iWeapon, float flDamageForce[3], float vecDamagePosition[3])
{
    int client = _inst.client;
    if (client != iAttacker) return flDamage;

    char classname[64];
    GetEdictClassname(iBuilding, classname, sizeof(classname));
    if (!StrEqual(classname, "obj_sentrygun"))
    {
        return flDamage;
    }

    // Apply bonus damage
    SDKHooks_TakeDamage(iBuilding, client, client, flDamage * (CORRUPTOR_BUILDING_DAMAGE_BONUS / 100.0), DMG_CLUB);
    PrintHintText(client, "%t", "CorruptorPerk_Hint_BonusDamage");

    // Corruption logic
    if (iBuilding != _inst.target_building)
    {
        _inst.target_building = iBuilding;
        _inst.hit_count       = 1;
    }
    else {
        _inst.hit_count++;
    }

    _inst.last_hit_time = GetGameTime();

    if (_inst.hit_count >= CORRUPTOR_HIT_COUNT)
    {
        doCorruptBuilding(client, iBuilding);
        _inst.target_building = -1;
        _inst.hit_count       = 0;
    }
    return flDamage;
}

FUNCTION(CorruptorPerk, updateCondStats), char[] buffer, int maxlen)
{
    if (_inst.target_building != -1)
    {
        if (GetGameTime() - _inst.last_hit_time > 1.5)
        {
            _inst.target_building = -1;
            _inst.hit_count       = 0;
        }
        else {
            Format(buffer, maxlen, "腐化进度: %d/%d", _inst.hit_count, CORRUPTOR_HIT_COUNT);
        }
    }
}
