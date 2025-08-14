#if defined __LeapPerk_included
    #endinput
#endif
#define __LeapPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_LEAP_COMBAT      -20
#define ZF_LEAP_COOLDOWN    6
#define ZF_LEAP_FORCE       1300.0
#define ZF_LEAP_FORCE_SCOUT 1900.0

// Data slots
#define LEAP_DATA_TIMER     (PERK_DATA_START_INDEX + 0)

methodmap LeapPerk < ZombieBasePerk
{

public     LeapPerk(int client)
    {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        LeapPerk       sm      = view_as<LeapPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, LeapPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, LeapPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, LeapPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, LeapPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, LeapPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, LeapPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, LeapPerkFonPlayerSpawn);

        // Initialize data
        sm.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk LeapPerkFnew(int client)
{
    return new LeapPerk(client);
}

FUNCTION(LeapPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Leap");
}

FUNCTION(LeapPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "LeapPerk_shortdesc");
}

FUNCTION(LeapPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "LeapPerk_desc");
}

FUNCTION_INT(LeapPerk, getCategory))
{
    return 5;
}

FUNCTION(LeapPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_LEAP_COMBAT);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_LEAP_COMBAT);
}

FUNCTION(LeapPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    if (victim == _inst.client && damagetype & DMG_FALL)
    {
        addHealth(_inst.client, RoundToNearest(damage), false);    // Immune to fall damage
    }
}

FUNCTION(LeapPerk, onCallForMedic))
{
    int client = _inst.client;
    if (_inst.perk_timer > 0 || !(GetEntityFlags(client) & FL_ONGROUND))
    {
        return;
    }

    float force = (TF2_GetPlayerClass(client) == TFClass_Scout) ? ZF_LEAP_FORCE_SCOUT : ZF_LEAP_FORCE;

    // 1. 获取玩家当前的视角角度
    float ang[3];
    GetClientEyeAngles(client, ang);

    // 2. 将角度转换为前向向量 (Forward Vector)
    float fwd_vec[3];
    GetAngleVectors(ang, fwd_vec, NULL_VECTOR, NULL_VECTOR);

    // 3. 将冲力分解
    // 假设我们想要一个大约45度的跳跃角
    // 我们可以把冲力平均分配给前方和上方
    // (更精确的三角函数是 sin(45)*force 和 cos(45)*force，但直接除以根号2或乘以0.707更简单)
    float forward_force = force * 0.707;
    float upward_force  = force * 0.707;

    // 4. 计算最终的速度向量
    float leap_velocity[3];
    // 前方分量
    leap_velocity[0] = fwd_vec[0] * forward_force;
    leap_velocity[1] = fwd_vec[1] * forward_force;
    // 上方分量
    leap_velocity[2] = upward_force;

    // 5. 将这个力应用到玩家身上
    // 注意：这不是与当前速度相加，而是直接设置一个新速度，效果更明显
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, leap_velocity);
    fxCreateSoundToAll(ZFSND_PUSHBACK, client)

    _inst.perk_timer = ZF_LEAP_COOLDOWN;
}

FUNCTION(LeapPerk, onPeriodic))
{
    if (_inst.perk_timer > 0)
    {
        _inst.perk_timer--;
    }
}

FUNCTION(LeapPerk, updateCondStats), char[] buffer, int maxlen)
{
    if (_inst.perk_timer > 0)
    {
        Format(buffer, maxlen, "%t", "LeapPerk_Status_Cooldown", _inst.perk_timer);
    }
    else {
        Format(buffer, maxlen, "%t", "LeapPerk_Status_Ready");
    }
}

FUNCTION(LeapPerk, onPlayerSpawn))
{
    // Reset cooldown on new life
    _inst.perk_timer = 0;
}
