#if defined __ChargerPerk_included
    #endinput
#endif
#define __ChargerPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/vector.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "../../../../include/sdkhooks.inc"
#include "../../../../include/sdktools_functions.inc"

// --- Constants ---
#define CHARGER_DEF_BONUS         25        // 防御力加成
#define CHARGER_ATT_PENALTY       -15       // 攻击力惩罚
#define CHARGER_SPEED_PENALTY     -20       // 移速惩罚
#define CHARGER_CHARGE_FORCE      1000.0    // 冲锋力量
#define CHARGER_CHARGE_COOLDOWN   12        // 冲锋冷却时间 (秒)
#define CHARGER_WALL_DAMAGE       50.0      // 撞墙伤害
#define CHARGER_STUN_DURATION     3.0       // 撞墙眩晕时间
#define CHARGER_MAX_DURATION      5.0       // 最大冲锋持续时间

// --- Data Slots ---
#define CHARGER_DATA_TIMER        (PERK_DATA_START_INDEX + 0)    // Cooldown
#define CHARGER_DATA_STATE        (PERK_DATA_START_INDEX + 1)    // 0: idle, 1: charging, 2: carrying
#define CHARGER_DATA_VICTIM       (PERK_DATA_START_INDEX + 2)    // The grabbed survivor
#define CHARGER_DATA_CHARGE_TIMER (PERK_DATA_START_INDEX + 3)    // Max duration timer
#define VTABLE_DO_CHARGE_END      (PERK_DATA_START_INDEX + 4)    // Custom vtable for ending the charge

methodmap ChargerPerk < ZombieBasePerk
{

public     ChargerPerk(int client)
    {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ChargerPerk    sm      = view_as<ChargerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ChargerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ChargerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ChargerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, ChargerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, ChargerPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, ChargerPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, ChargerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TOUCH, ChargerPerkFonTouch);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, ChargerPerkFonPlayerRunCmd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, ChargerPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, ChargerPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_DO_CHARGE_END, ChargerPerkFdoChargeEnd);

        // Initialize data
        sm.perk_timer   = 0;
        sm.perk_state   = 0;
        sm.victim       = -1;
        sm.charge_timer = 0.0;

        return sm;
    }

    property int perk_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_TIMER);
            this.WriteCell(value);
        }
    }
    property int perk_state
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_STATE);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_STATE);
            this.WriteCell(value);
        }
    }
    property int victim
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_VICTIM);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_VICTIM);
            this.WriteCell(value);
        }
    }
    property float charge_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_CHARGE_TIMER);
            return this.ReadFloat();
        }

public         set(float value)
        {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_CHARGE_TIMER);
            this.WriteFloat(value);
        }
    }

public     void doChargeEnd(bool hitWall)
    {
        START_VTABLE_CALL(VTABLE_DO_CHARGE_END);
        Call_PushCell(hitWall);
        Call_Finish();
    }
}

public bool TraceFilterCharger(int entity, int contentsMask, any data)
{
    int client = data;
    if (entity == client)
        return false;

    BasePerk perk_base = g_hPerks[client];
    if (perk_base != null)
    {
        ChargerPerk perk = view_as<ChargerPerk>(perk_base);
        if (perk != null && entity == perk.victim)
        {
            return false;
        }
    }

    if (entity > 0 && entity <= MaxClients)
        return false;

    return true;
}

stock BasePerk ChargerPerkFnew(int client)
{
    return new ChargerPerk(client);
}

FUNCTION(ChargerPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Charger");
}
FUNCTION(ChargerPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ChargerPerk_shortdesc");
}
FUNCTION(ChargerPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ChargerPerk_desc");
}

FUNCTION(ChargerPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, CHARGER_DEF_BONUS);
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, CHARGER_ATT_PENALTY);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, CHARGER_SPEED_PENALTY);
}

FUNCTION(ChargerPerk, onCallForMedic))
{
    int client = _inst.client;
    if (_inst.perk_timer > 0 || _inst.perk_state != 0 || !(GetEntityFlags(client) & FL_ONGROUND))
    {
        return;
    }

    _inst.perk_state   = 1;
    _inst.perk_timer   = CHARGER_CHARGE_COOLDOWN;
    _inst.charge_timer = GetGameTime() + CHARGER_MAX_DURATION;

    float ang[3];
    GetClientEyeAngles(client, ang);
    ang[0] = 0.0;    // Prevent charging into the ground
    float vel[3];
    GetAngleVectors(ang, vel, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(vel, CHARGER_CHARGE_FORCE);
    vel[2] = 500.0;

    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, vel);
    TF2_AddCondition(client, TFCond_Charging, CHARGER_MAX_DURATION);
    TF2_AddCondition(client, TFCond_ImmuneToPushback, CHARGER_MAX_DURATION);
    TF2_AddCondition(client, TFCond_RestrictToMelee, CHARGER_MAX_DURATION);    // 禁止攻击
    SetVariantInt(1);
    AcceptEntityInput(client, "SetForcedTauntCam");

    PrintHintText(client, "%t", "ChargerPerk_Hint_Charging");
}

FUNCTION(ChargerPerk, onTouch), int toucher, int touchee)
{
    int client = _inst.client;
    if (toucher != client || _inst.perk_state != 1 || !validClient(touchee) || validZom(touchee))
    {
        return;
    }

    _inst.victim     = touchee;
    _inst.perk_state = 2;

    // Parent the victim to the charger to make them move together

    // Make victim non-solid and carry them
    TF2_StunPlayer(touchee, CHARGER_MAX_DURATION, 0.0, TF_STUNFLAGS_BIGBONK, client);

    float org[3];
    GetClientAbsOrigin(toucher, org);

    float ang[3];
    GetClientAbsAngles(toucher, ang);

    float fwdd[3];
    GetAngleVectors(ang, fwdd, NULL_VECTOR, NULL_VECTOR);    // 计算面对方向的单位向量

    ScaleVector(fwdd, -80.0);      // 放大为 150 单位距离
    AddVectors(org, fwdd, org);    // 加到原坐标上

    TeleportEntity(touchee, org, NULL_VECTOR, NULL_VECTOR);    // 移动 touchee

    PrintHintText(client, "%t", "ChargerPerk_Hint_Grabbed");
    PrintHintText(touchee, "%t", "ChargerPerk_Hint_GrabbedVictim");
}

FUNCTION(ChargerPerk, onPlayerRunCmd), int buttons, int impulse, float vel[3], float angles[3], int weapon)
{
    if (_inst.perk_state == 0)
    {
        return;
    }

    int client = _inst.client;

    if (_inst.perk_state == 2)
    {
        float org[3];
        GetClientAbsOrigin(client, org);

        float ang[3];
        GetClientAbsAngles(client, ang);

        float fwdd[3];
        GetAngleVectors(ang, fwdd, NULL_VECTOR, NULL_VECTOR);    // 计算面对方向的单位向量

        ScaleVector(fwdd, -80.0);      // 放大为 -50 单位距离
        AddVectors(org, fwdd, org);    // 加到原坐标上

        TeleportEntity(_inst.victim, org, NULL_VECTOR, NULL_VECTOR);    // 移动 touchee
    }
    // --- Sustained charge movement & direction ---
    float charge_ang[3];
    GetClientEyeAngles(client, charge_ang);
    charge_ang[0] = 0.0;    // Keep charge horizontal

    float charge_vel[3];
    GetAngleVectors(charge_ang, charge_vel, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(charge_vel, CHARGER_CHARGE_FORCE);
    charge_vel[2] = 150.0;    // Add a slight upward velocity to climb slopes
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, charge_vel);
    // --- Wall detection logic ---
    float start_pos[3], end_pos[3], fwd[3];
    GetEntPropVector(client, Prop_Send, "m_vecOrigin", start_pos);

    fwd = charge_vel;
    NormalizeVector(fwd, fwd);

    end_pos = start_pos;
    ScaleVector(fwd, 45.0);    // Check 45 units ahead
    AddVectors(end_pos, fwd, end_pos);

    // We trace a hull instead of a ray to allow for moving on slopes.
    // The hull is a small box that checks for obstacles in a volume, not just a single point.
    // Raise the bottom of the hull to avoid hitting the ground on slopes
    float mins[3] = { -12.0, -12.0, 5.0 };
    float maxs[3] = { 12.0, 12.0, 40.0 };

    Handle trace = TR_TraceHullFilterEx(start_pos, end_pos, mins, maxs, MASK_SOLID, TraceFilterCharger, client);
    if (TR_DidHit(trace))
    {
        CloseHandle(trace);
        _inst.doChargeEnd(true);
        return;
    }
    CloseHandle(trace);
}

FUNCTION(ChargerPerk, onPeriodic))
{
    if (_inst.perk_timer > 0)
    {
        _inst.perk_timer--;
    }

    if (_inst.perk_state != 0 && GetGameTime() > _inst.charge_timer)
    {
        PrintHintText(_inst.client, "%t", "ChargerPerk_Hint_Finished");
        _inst.doChargeEnd(false);
    }
}

FUNCTION(ChargerPerk, updateCondStats), char[] buffer, int maxlen)
{
    switch (_inst.perk_state)
    {
        case 1: Format(buffer, maxlen, "%t", "ChargerPerk_Status_Charging");

        case 2: Format(buffer, maxlen, "%t", "ChargerPerk_Status_Carrying");

        default:
        {
            if (_inst.perk_timer > 0)
            {
                Format(buffer, maxlen, "%t", "ChargerPerk_Status_Cooldown", _inst.perk_timer);
            }
            else {
                Format(buffer, maxlen, "%t", "ChargerPerk_Status_Ready");
            }
        }
    }
}

FUNCTION(ChargerPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    if (_inst.perk_state != 0)
    {
        _inst.doChargeEnd(false);
    }
}

FUNCTION(ChargerPerk, onRemove))
{
    if (_inst.perk_state != 0)
    {
        _inst.doChargeEnd(false);
    }
}

FUNCTION(ChargerPerk, doChargeEnd), bool hitWall)
{
    int client = _inst.client;
    int victim = _inst.victim;

    SetEntityMoveType(client, MOVETYPE_WALK);

    TF2_RemoveCondition(client, TFCond_Charging);
    TF2_RemoveCondition(client, TFCond_ImmuneToPushback);
    TF2_RemoveCondition(client, TFCond_RestrictToMelee);
    SetVariantInt(0);
    AcceptEntityInput(client, "SetForcedTauntCam");

    float zero_vel[3] = { 0.0, 0.0, 0.0 };
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, zero_vel);

    int valid_victim = validClient(victim) && IsPlayerAlive(victim);
    if (valid_victim)
    {
        // Restore collision and state
        AcceptEntityInput(victim, "ClearParent");
        SetEntityMoveType(victim, MOVETYPE_WALK);
        TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, zero_vel);
        TF2_RemoveCondition(victim, TFCond_Dazed);

        if (hitWall)
        {
            PrintHintText(victim, "%t", "ChargerPerk_Hint_WallHitVictim");
            SDKHooks_TakeDamage(victim, client, client, CHARGER_WALL_DAMAGE, DMG_CRUSH);
            TF2_StunPlayer(victim, CHARGER_STUN_DURATION, 0.0, TF_STUNFLAGS_BIGBONK, client);
        }
        else {
            PrintHintText(victim, "%t", "ChargerPerk_Hint_Released");
        }
    }

    if (hitWall)
    {
        PrintHintText(client, "%t", "ChargerPerk_Hint_WallHit");
        SDKHooks_TakeDamage(client, client, client, CHARGER_WALL_DAMAGE, DMG_CRUSH);
        TF2_StunPlayer(client, CHARGER_STUN_DURATION, 0.0, TF_STUNFLAGS_BIGBONK, client);
    }

    // Reset perk state at the very end
    _inst.perk_state   = 0;
    _inst.victim       = -1;
    _inst.charge_timer = 0.0;
}