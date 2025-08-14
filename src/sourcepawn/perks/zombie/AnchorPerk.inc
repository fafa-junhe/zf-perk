#if defined __AnchorPerk_included
#endinput
#endif
#define __AnchorPerk_included

#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines
#define ANCHOR_DEFEND_BONUS 40
#define ANCHOR_REGEN_BONUS 40
#define ANCHOR_COOLDOWN 8

// Data slots
#define ANCHOR_DATA_STATE (PERK_DATA_START_INDEX + 0) // 0: normal, 1: anchored
#define ANCHOR_DATA_TIMER (PERK_DATA_START_INDEX + 1) // Cooldown timer

methodmap AnchorPerk < ZombieBasePerk {
    public AnchorPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        AnchorPerk sm = view_as<AnchorPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, AnchorPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AnchorPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AnchorPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AnchorPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, AnchorPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, AnchorPerkFonPlayerSpawn);

        // Initialize data
        sm.Position = view_as<DataPackPos>(ANCHOR_DATA_STATE);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(ANCHOR_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_state {
        public get() {
            this.Position = view_as<DataPackPos>(ANCHOR_DATA_STATE);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(ANCHOR_DATA_STATE);
            this.WriteCell(value);
        }
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(ANCHOR_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(ANCHOR_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk AnchorPerkFnew(int client) {
    return new AnchorPerk(client);
}

FUNCTION(AnchorPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Anchor");
}

FUNCTION(AnchorPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AnchorPerk_shortdesc");
}

FUNCTION(AnchorPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AnchorPerk_desc");
}

FUNCTION_INT(AnchorPerk, getCategory))
{
    return 2; // Defensive
}

FUNCTION(AnchorPerk, onRemove)) {
    int client = _inst.client;
    // If anchored, revert movement changes
    if (_inst.perk_state == 1) {
        SetEntityMoveType(client, MOVETYPE_WALK);
        resetClientStatType(client, ZFStatTypeCond);
    }
}

FUNCTION(AnchorPerk, onPlayerSpawn)) {
    int client = _inst.client;
    // If player died while anchored, clean up
    if (_inst.perk_state == 1) {
        SetEntityMoveType(client, MOVETYPE_WALK);
        resetClientStatType(client, ZFStatTypeCond);
        _inst.perk_state = 0;
    }
    // Reset cooldown on new life
    _inst.perk_timer = 0;
}

FUNCTION(AnchorPerk, onCallForMedic)) {
    int client = _inst.client;

    // Cooldown check only applies when trying to anchor, not when un-anchoring
    if (_inst.perk_state == 0 && _inst.perk_timer > 0) {
        PrintHintText(client, "%t", "AnchorPerk_Hint_Cooldown", _inst.perk_timer);
        return;
    }

    // Toggle state
    _inst.perk_state = 1 - _inst.perk_state;

    if (_inst.perk_state == 1) { // Just anchored
        SetEntityMoveType(client, MOVETYPE_NONE);
        PrintHintText(client, "%t", "AnchorPerk_Hint_Anchored");
    } else { // Just un-anchored
        SetEntityMoveType(client, MOVETYPE_WALK);
        _inst.perk_timer = ANCHOR_COOLDOWN;
        resetClientStatType(client, ZFStatTypeCond); // Immediately remove stats
        PrintHintText(client, "%t", "AnchorPerk_Hint_Released");
    }
}

FUNCTION(AnchorPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
    }
}

FUNCTION(AnchorPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;

    if (_inst.perk_state == 1) { // Is anchored
        addStat(client, ZFStatDef, ZFStatTypeCond, ANCHOR_DEFEND_BONUS);
        addHealth(client, ANCHOR_REGEN_BONUS);
        Format(buffer, maxlen, "%t", "AnchorPerk_Status_Active");
    } else { // Not anchored
        if (_inst.perk_timer > 0) {
            Format(buffer, maxlen, "%t", "AnchorPerk_Status_Cooldown", _inst.perk_timer);
        } else {
            Format(buffer, maxlen, "%t", "AnchorPerk_Status_Ready");
        }
    }
}