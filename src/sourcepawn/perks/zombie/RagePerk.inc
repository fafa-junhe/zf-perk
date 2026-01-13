#if defined __RagePerk_included
#endinput
#endif
#define __RagePerk_included

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
#define ZF_RAGE_COOLDOWN 20
#define ZF_RAGE_SPEED 150
#define ZF_RAGE_SPEED_DURATION 10
#define ZF_RAGE_HEALTHPCT_TOUSE 0.80
#define ZF_RAGE_HEALTHPCT_ONUSE 0.5

// Data slots
#define RAGE_DATA_STATE (PERK_DATA_START_INDEX + 0)
#define RAGE_DATA_TIMER (PERK_DATA_START_INDEX + 1)

methodmap RagePerk < ZombieBasePerk {
    public RagePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        RagePerk sm = view_as<RagePerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, RagePerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, RagePerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, RagePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, RagePerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, RagePerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, RagePerkFonPlayerSpawn);

        // Initialize data
        sm.Position = view_as<DataPackPos>(RAGE_DATA_STATE);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(RAGE_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_state {
        public get() {
            this.Position = view_as<DataPackPos>(RAGE_DATA_STATE);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(RAGE_DATA_STATE);
            this.WriteCell(value);
        }
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(RAGE_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(RAGE_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk RagePerkFnew(int client) {
    return new RagePerk(client);
}

FUNCTION(RagePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Rage");
}

FUNCTION(RagePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RagePerk_shortdesc");
}

FUNCTION(RagePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "RagePerk_desc");
}

FUNCTION_INT(RagePerk, getCategory))
{
    return 1;
}

FUNCTION(RagePerk, onRemove)) {
    int client = _inst.client;
    if (_inst.perk_state == 1) {
        if (validAura(client)) removeAura(client);
        resetClientStatType(client, ZFStatTypeCond);
    }
}

FUNCTION(RagePerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.perk_timer > 0 || _inst.perk_state == 1) {
        return;
    }

    float health_pct = float(GetClientHealth(client)) / float(clientMaxHealth(client));
    if (health_pct < ZF_RAGE_HEALTHPCT_TOUSE) {
        PrintHintText(client, "%t", "RagePerk_Health_Too_Low");
        return;
    }

    _inst.perk_state = 1;
    _inst.perk_timer = ZF_RAGE_COOLDOWN;
    
    int health_bonus = RoundFloat(float(GetClientHealth(client)) * ZF_RAGE_HEALTHPCT_ONUSE);
    int currentHealth = GetClientHealth(client);
    SetEntityHealth(client, currentHealth + health_bonus);
    addStatTempStack(client, ZFStatSpeed, ZF_RAGE_SPEED, ZF_RAGE_SPEED_DURATION);
    createAura(client, ZFPART_AURAINRED, AttachBase);
    PrintHintText(client, "%t", "RagePerk_Activated");
}

FUNCTION(RagePerk, onPeriodic)) {
    if (_inst.perk_timer > 0 && _inst.perk_state == 0) {
        _inst.perk_timer--;
    }
}

FUNCTION(RagePerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;
    if (_inst.perk_state == 1) {
        float health_pct = float(GetClientHealth(client)) / float(clientMaxHealth(client));
        if (health_pct < ZF_RAGE_HEALTHPCT_TOUSE) { // Rage disappears when health is low
            _inst.perk_state = 0;
            if (validAura(client)) removeAura(client);
            resetClientStatType(client, ZFStatTypeCond);
            PrintHintText(client, "%t", "RagePerk_Expired");
        } else {
            Format(buffer, maxlen, "%t", "RagePerk_Status_Active");
        }
    }
    
    if (_inst.perk_state == 0) {
        if (_inst.perk_timer > 0) {
            Format(buffer, maxlen, "%t", "RagePerk_Status_Cooldown", _inst.perk_timer);
        } else {
            Format(buffer, maxlen, "%t", "RagePerk_Status_Ready");
        }
    }
}

FUNCTION(RagePerk, onPlayerSpawn)) {
    int client = _inst.client;
    // If player died while raging, clean up the effects
    if (_inst.perk_state == 1) {
        if (validAura(client)) removeAura(client);
        resetClientStatType(client, ZFStatTypeCond);
        _inst.perk_state = 0;
    }
    // Reset cooldown on new life
    _inst.perk_timer = 0;
}
