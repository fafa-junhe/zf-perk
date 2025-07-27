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
#define ZF_LEAP_COMBAT -20
#define ZF_LEAP_COOLDOWN 4
#define ZF_LEAP_FORCE 900.0
#define ZF_LEAP_FORCE_SCOUT 1500.0

// Data slots
#define LEAP_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap LeapPerk < ZombieBasePerk {
    public LeapPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        LeapPerk sm = view_as<LeapPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, LeapPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, LeapPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, LeapPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, LeapPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, LeapPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, LeapPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, LeapPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, LeapPerkFupdateCondStats);

        // Initialize data
        sm.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(LEAP_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk LeapPerkFnew(int client) {
    return new LeapPerk(client);
}

FUNCTION(LeapPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Leap");
}

FUNCTION(LeapPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "大跳飞向空中");
}

FUNCTION(LeapPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你的攻击力与防御力降低,但不受坠落伤害。发医生语音来施展大跳,冷却时间4秒。“起飞!”");
}

FUNCTION(LeapPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_LEAP_COMBAT);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_LEAP_COMBAT);
}

FUNCTION_FLOAT(LeapPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (victim == _inst.client && (damagetype & DMG_FALL)) {
        return 0.0; // Immune to fall damage
    }
    return damage; // No change for other damage types
}

FUNCTION(LeapPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.perk_timer > 0 || !(GetEntityFlags(client) & FL_ONGROUND)) {
        return;
    }

    float force = (TF2_GetPlayerClass(client) == TFClass_Scout) ? ZF_LEAP_FORCE_SCOUT : ZF_LEAP_FORCE;
    
    float vel[3];
    GetEntPropVector(client, Prop_Data, "m_vecVelocity", vel);
    vel[2] += force;
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, vel);

    _inst.perk_timer = ZF_LEAP_COOLDOWN;
}

FUNCTION(LeapPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
    }
}

FUNCTION(LeapPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.perk_timer > 0) {
        Format(buffer, maxlen, "Leap Cooldown: %ds", _inst.perk_timer);
    } else {
        Format(buffer, maxlen, "Leap Ready");
    }
}
