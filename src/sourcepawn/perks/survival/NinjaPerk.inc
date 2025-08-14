
#if defined __NinjaPerk_included
    #endinput
#endif
#define __NinjaPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/entity_prop_stocks.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/string.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_NINJA_ATTACK                -(20)
#define ZF_NINJA_SPEED                 50
#define ZF_NINJA_DURATION_DECOY_ACTIVE 25
#define ZF_NINJA_DURATION_DECOY_DECAY  5
#define ZF_NINJA_DURATION_COOLDOWN     30
#define ZF_NINJA_FALLDMG_RESIST        50
#define ZF_NINJA_FORCE                 600.0

#define NINJA_DATA_TIMER               (PERK_DATA_START_INDEX + 0)
#define NINJA_DATA_DECOY_ACTIVE_TIMER  (PERK_DATA_START_INDEX + 1)
#define NINJA_DATA_DECOY_POOF_TIMER    (PERK_DATA_START_INDEX + 2)
#define NINJA_DATA_PERK_STATE          (PERK_DATA_START_INDEX + 3)
#define NINJA_DATA_DECOY_POS           (PERK_DATA_START_INDEX + 4)    // Size: 3
#define NINJA_DATA_DECOY_ANG           (PERK_DATA_START_INDEX + 7)    // Size: 3
#define NINJA_DATA_DECOY_ENT           (PERK_DATA_START_INDEX + 10)
#define VTABLE_DO_NINJA_DECOY_PLACE    (PERK_DATA_START_INDEX + 11)
#define VTABLE_DO_NINJA_DECOY_POOF     (PERK_DATA_START_INDEX + 12)

methodmap NinjaPerk < SurvivorBasePerk
{

public     NinjaPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        NinjaPerk        sm      = view_as<NinjaPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, NinjaPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, NinjaPerkFonPlayerRunCmd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, NinjaPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, NinjaPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, NinjaPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, NinjaPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, NinjaPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, NinjaPerkFonRemove);

        sm.Position = view_as<DataPackPos>(NINJA_DATA_TIMER);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ACTIVE_TIMER);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POOF_TIMER);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_PERK_STATE);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POS);
        float empty_vec[3];
        sm.WriteCellArray(empty_vec, 3);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ANG);
        sm.WriteCellArray(empty_vec, 3);
        sm.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ENT);
        sm.WriteCell(0);

        PERK_REGISTER_VTABLE(sm, VTABLE_DO_NINJA_DECOY_PLACE, NinjaPerkFdoNinjaDecoyPlace);
        PERK_REGISTER_VTABLE(sm, VTABLE_DO_NINJA_DECOY_POOF, NinjaPerkFdoNinjaDecoyPoof);

        return sm;
    }

    property int perk_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_TIMER);
            this.WriteCell(value);
        }
    }

    property int decoy_active_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ACTIVE_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ACTIVE_TIMER);
            this.WriteCell(value);
        }
    }

    property int decoy_poof_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POOF_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POOF_TIMER);
            this.WriteCell(value);
        }
    }

    property int perk_state
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_PERK_STATE);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_PERK_STATE);
            this.WriteCell(value);
        }
    }

public     void getDecoyPos(float buffer[3])
    {
        this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POS);
        this.ReadCellArray(buffer, 3);
    }

public     void setDecoyPos(const float value[3])
    {
        this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_POS);
        this.WriteCellArray(value, 3);
    }

public     void getDecoyAng(float buffer[3])
    {
        this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ANG);
        this.ReadCellArray(buffer, 3);
    }

public     void setDecoyAng(const float value[3])
    {
        this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ANG);
        this.WriteCellArray(value, 3);
    }

    property int decoy_ent
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ENT);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(NINJA_DATA_DECOY_ENT);
            this.WriteCell(value);
        }
    }

public     void doNinjaDecoyPlace()
    {
        PERK_IMPL_EVENT_VOID(VTABLE_DO_NINJA_DECOY_PLACE);
    }

public     void doNinjaDecoyPoof()
    {
        PERK_IMPL_EVENT_VOID(VTABLE_DO_NINJA_DECOY_POOF);
    }
}

stock SurvivorBasePerk NinjaPerkFnew(int client)
{
    return new NinjaPerk(client);
}

FUNCTION(NinjaPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Ninja");
}

FUNCTION(NinjaPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "NinjaPerk_shortdesc");
}

FUNCTION(NinjaPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "NinjaPerk_desc");
}



FUNCTION_INT(NinjaPerk, getCategory))
{
    return 5;
}

FUNCTION(NinjaPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_NINJA_ATTACK);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_NINJA_SPEED);
}

FUNCTION(NinjaPerk, onPlayerRunCmd), int buttons, int impulse, float vel[3], float[] angles, int weapon)
{
    if ((buttons & IN_JUMP))
    {
        if ((GetEntityFlags(_inst.client) & FL_ONGROUND))
        {
            GetEntPropVector(_inst.client, Prop_Data, "m_vecVelocity", vel);
            vel[2] = ZF_NINJA_FORCE;
            TeleportEntity(_inst.client, NULL_VECTOR, NULL_VECTOR, vel);
        }
    }
}

FUNCTION(NinjaPerk, updateCondStats), char[] perk_str, int maxlen_perk_str)
{
    if ((_inst.perk_state == 1))
    {
        Format(perk_str, maxlen_perk_str, "%t", "NinjaPerk_Status_DecoyActive", _inst.decoy_active_timer);
    }
    else {
        if ((_inst.perk_timer > 0))
        {
            Format(perk_str, maxlen_perk_str, "%t", "NinjaPerk_Status_Cooldown", _inst.perk_timer);
        }
        else {
            Format(perk_str, maxlen_perk_str, "%t", "NinjaPerk_Status_Ready");
        }
    }
}

FUNCTION_FLOAT(NinjaPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    if ((victim != _inst.client))
    {
        return damage;
    }
    if ((damagetype & DMG_FALL))
    {
        return (damage * (1.0 - (ZF_NINJA_FALLDMG_RESIST / 100.0)));
    }
    if (((_inst.perk_state == 1)) && (validClient(attacker)) && (validZom(attacker)) && ((damagetype & DMG_CLUB)))
    {
        _inst.doNinjaDecoyPlace();
        _inst.decoy_poof_timer = ZF_NINJA_DURATION_DECOY_DECAY;
        return 0.0;
    }
    return damage;
}

FUNCTION(NinjaPerk, onCallForMedic))
{
    if (((_inst.perk_timer == 0)) && ((GetEntityFlags(_inst.client) & FL_ONGROUND)))
    {
        _inst.perk_timer         = ZF_NINJA_DURATION_COOLDOWN;
        _inst.decoy_active_timer = ZF_NINJA_DURATION_DECOY_ACTIVE;
        _inst.perk_state         = 1;
        float __temp_decoy_pos[3];
        GetEntPropVector(_inst.client, Prop_Send, "m_vecOrigin", __temp_decoy_pos);
        _inst.setDecoyPos(__temp_decoy_pos);
        float __temp_decoy_ang[3];
        GetClientEyeAngles(_inst.client, __temp_decoy_ang);
        _inst.setDecoyAng(__temp_decoy_ang);
        createAura(_inst.client, "particles/medic_heal_blu.pcf", AttachBase);
        PrintHintText(_inst.client, "%t", "NinjaPerk_Hint_DecoyPlaced", ZF_NINJA_DURATION_DECOY_ACTIVE);
    }
}

FUNCTION(NinjaPerk, onPeriodic))
{
    if ((_inst.perk_timer > 0))
    {
        _inst.perk_timer -= 1;
        if ((_inst.perk_timer == 0))
        {
            PrintHintText(_inst.client, "%t", "NinjaPerk_Hint_Ready");
        }
    }
    if ((_inst.decoy_active_timer > 0))
    {
        _inst.decoy_active_timer -= 1;
        if (((_inst.decoy_active_timer == 0)) && ((_inst.perk_state == 1)))
        {
            _inst.perk_state = 0;
            if (validAura(_inst.client))
            {
                removeAura(_inst.client);
            }
            PrintHintText(_inst.client, "%t", "NinjaPerk_Hint_DecoyExpired");
        }
    }
    if ((_inst.decoy_poof_timer > 0))
    {
        _inst.decoy_poof_timer -= 1;
        if ((_inst.decoy_poof_timer == 0))
        {
            _inst.doNinjaDecoyPoof();
        }
    }
}
// 修改 doNinjaDecoyPlace，它现在不需要返回任何东西，
// 因为诱饵实体由 onTakeDamage 创建和管理
FUNCTION(NinjaPerk, doNinjaDecoyPlace))
{
    int client       = _inst.client;

    // 1. 在原地创建诱饵
    int decoy_entity = doItemPlace(client, ZFMDL_CUTOUT[TF2_GetPlayerClass(client)]);
    _inst.decoy_ent  = decoy_entity;    // 存储实体ID

    // 2. 获取传送点并验证
    float __vec_decoy_pos[3];
    _inst.getDecoyPos(__vec_decoy_pos);

    // 如果传送点无效 (0, 0, z)，则取消传送并重置技能
    if (__vec_decoy_pos[0] == 0.0 && __vec_decoy_pos[1] == 0.0)
    {
        PrintHintText(client, "%t", "NinjaPerk_Hint_InvalidPoint");
        // 清理刚刚创建的诱饵
        if (IsValidEntity(decoy_entity))
        {
            fxPuffSmall(decoy_entity);
            fxDeleteModel(decoy_entity);
            _inst.decoy_ent = -1;
        }

        // 重置技能状态，但不进入冷却，允许玩家重新设置
        _inst.perk_state         = 0;
        _inst.decoy_active_timer = 0;
        _inst.perk_timer         = 0;    // 重置冷却时间
        if (validAura(client))
        {
            removeAura(client);
        }
        return;
    }

    // 3. 传送玩家
    float __vec_decoy_ang[3];
    _inst.getDecoyAng(__vec_decoy_ang);
    float vec_zero[3] = { 0.0, 0.0, 0.0 };
    TeleportEntity(client, __vec_decoy_pos, __vec_decoy_ang, vec_zero);

    // 4. 清理状态
    if (validAura(client))
    {
        removeAura(client);
    }
    _inst.perk_state         = 0;
    _inst.decoy_active_timer = 0;
    PrintHintText(client, "%t", "NinjaPerk_Hint_Escaped");
}

// 修改 doNinjaDecoyPoof 来直接移除实体
FUNCTION(NinjaPerk, doNinjaDecoyPoof))
{
    int decoy_entity = _inst.decoy_ent;
    if (IsValidEntity(decoy_entity))
    {
        fxPuffSmall(decoy_entity);      // 使用核心框架的特效函数
        fxDeleteModel(decoy_entity);    // 使用核心框架的删除函数
        _inst.decoy_ent = -1;           // 清空记录
    }
}

FUNCTION(NinjaPerk, onRemove))
{
    // Ensure the decoy is cleaned up when the perk is removed
    int decoy_entity = _inst.decoy_ent;
    if (IsValidEntity(decoy_entity))
    {
        fxDeleteModel(decoy_entity);
        _inst.decoy_ent = -1;    // Clear the record
    }
}