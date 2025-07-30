#if defined __RoarPerk_included
#endinput
#endif
#define __RoarPerk_included

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
#include <sdktools_functions>

// Defines from all_perks.md
#define ZF_ROAR_COOLDOWN 15
#define ZF_ROAR_DURATION 20
#define ZF_ROAR_DURATION_HEAVY 60
#define ZF_ROAR_FORCE 1200.0
#define ZF_ROAR_FORCE_HEAVY 3000.0
#define ZF_ROAR_RADIUS 450.0
#define ZF_ROAR_DEFEND -25

// Data slots
#define ROAR_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap RoarPerk < ZombieBasePerk {
    public RoarPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        RoarPerk sm = view_as<RoarPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, RoarPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, RoarPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, RoarPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, RoarPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, RoarPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, RoarPerkFupdateCondStats);

        // Initialize data
        sm.Position = view_as<DataPackPos>(ROAR_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(ROAR_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(ROAR_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk RoarPerkFnew(int client) {
    return new RoarPerk(client);
}

FUNCTION(RoarPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Roar");
}

FUNCTION(RoarPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RoarPerk_shortdesc");
}

FUNCTION(RoarPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RoarPerk_desc");
}

FUNCTION(RoarPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.perk_timer > 0 || !(GetEntityFlags(client) & FL_ONGROUND)) {
        return;
    }

    _inst.perk_timer = ZF_ROAR_COOLDOWN;
    
    // --- 播放特效和音效 ---
    fxPuffBig(client);
    ClientCommand(client, "voicemenu 2 1");
    
    // --- 计算数值 ---
    float force = isHeavy(client) ? ZF_ROAR_FORCE_HEAVY : ZF_ROAR_FORCE;
    int duration = isHeavy(client) ? ZF_ROAR_DURATION_HEAVY : ZF_ROAR_DURATION;
    
    float roar_pos[3];
    GetClientAbsOrigin(client, roar_pos);

    // --- 遍历范围内的幸存者 ---
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            float sur_pos[3];
            GetClientAbsOrigin(i, sur_pos);

            if (GetVectorDistance(roar_pos, sur_pos) <= ZF_ROAR_RADIUS) {
                // --- 对范围内的每个幸存者施加效果 ---
                
                // 1. 施加防御力减益
                addStatTempStack(i, ZFStatDef, ZF_ROAR_DEFEND, duration);

                // 2. 计算并施加击退
                float knockback_dir[3];
                SubtractVectors(sur_pos, roar_pos, knockback_dir); // 正确方向：目标 - 源头
                NormalizeVector(knockback_dir, knockback_dir);
                
                // 给予一个向上的力，防止玩家被推到地里
                knockback_dir[2] = 0.5;
                NormalizeVector(knockback_dir, knockback_dir);
                
                ScaleVector(knockback_dir, force);
                
                // 直接设置速度，而不是相加，效果更可控
                TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, knockback_dir);
            }
        }
    }
}


FUNCTION(RoarPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
    }
}

FUNCTION(RoarPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.perk_timer > 0) {
Format(buffer, maxlen, "%t", "RoarPerk_Status_Cooldown", _inst.perk_timer);    } else {
Format(buffer, maxlen, "%t", "RoarPerk_Status_Ready");    }
}
