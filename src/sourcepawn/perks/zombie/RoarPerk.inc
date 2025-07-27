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
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, RoarPerkFonTakeDamagePost);
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
    strcopy(buffer, maxlen, "咆哮击退幸存者");
}

FUNCTION(RoarPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "发医生语音来激活咆哮。咆哮造成击退效果并暂时降低幸存者防御力,冷却时间15秒。“哈!”");
}

FUNCTION(RoarPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.perk_timer > 0 || !(GetEntityFlags(client) & FL_ONGROUND)) {
        return;
    }

    _inst.perk_timer = ZF_ROAR_COOLDOWN;
    
    applyDamageRadialAtClient(client, 1, RoundFloat(ZF_ROAR_RADIUS));
    fxPuffBig(client);
    ClientCommand(client, "voicemenu 2 1");
}

FUNCTION(RoarPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker != _inst.client || !validLivingSur(victim) || inflictor != attacker) {
        return;
    }

    int client = _inst.client;
    bool isHeavyAttacker = TF2_GetPlayerClass(client) == TFClass_Heavy;
    float force = isHeavyAttacker ? ZF_ROAR_FORCE_HEAVY : ZF_ROAR_FORCE;
    int duration = isHeavyAttacker ? ZF_ROAR_DURATION_HEAVY : ZF_ROAR_DURATION;

    float pos[3];
    GetClientAbsOrigin(client, pos);
    float sur_pos[3];
    GetClientAbsOrigin(victim, sur_pos);

    float knockback_dir[3];
    MakeVectorFromPoints(pos, sur_pos, knockback_dir);
    NormalizeVector(knockback_dir, knockback_dir);
    ScaleVector(knockback_dir, force);
    
    float current_vel[3];
    GetEntPropVector(victim, Prop_Data, "m_vecVelocity", current_vel);
    AddVectors(current_vel, knockback_dir, current_vel);
    TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, current_vel);

    addStatTempStack(victim, ZFStatDef, ZF_ROAR_DEFEND, duration);
}


FUNCTION(RoarPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
    }
}

FUNCTION(RoarPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.perk_timer > 0) {
        Format(buffer, maxlen, "Roar Cooldown: %ds", _inst.perk_timer);
    } else {
        Format(buffer, maxlen, "Roar Ready");
    }
}
