#if defined __PhantasmPerk_included
#endinput
#endif
#define __PhantasmPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include <tf2>
#include <entity_prop_stocks>

// --- Defines from new_perks.md ---
#define ZF_PHANTASM_HEALTH_PENALTY -25
#define ZF_PHANTASM_ATTACK_PENALTY -25
#define ZF_PHANTASM_WALK_DURATION 4.0
#define ZF_PHANTASM_COOLDOWN 12.0

// --- Data Slots ---
#define PHANTASM_DATA_WALK_TIMER    (PERK_DATA_START_INDEX + 0)
#define PHANTASM_DATA_COOLDOWN      (PERK_DATA_START_INDEX + 1)

methodmap PhantasmPerk < ZombieBasePerk {
    public PhantasmPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        PhantasmPerk sm = view_as<PhantasmPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, PhantasmPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, PhantasmPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, PhantasmPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, PhantasmPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, PhantasmPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, PhantasmPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, PhantasmPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, PhantasmPerkFonPlayerSpawn);

        sm.walk_timer = 0.0;
        sm.cooldown = 0.0;

        return sm;
    }

    property float walk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(PHANTASM_DATA_WALK_TIMER);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(PHANTASM_DATA_WALK_TIMER);
            this.WriteFloat(value);
        }
    }

    property float cooldown {
        public get() {
            this.Position = view_as<DataPackPos>(PHANTASM_DATA_COOLDOWN);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(PHANTASM_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }
}

stock BasePerk PhantasmPerkFnew(int client) {
    return new PhantasmPerk(client);
}

FUNCTION(PhantasmPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Phantasm");
}

FUNCTION(PhantasmPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "PhantasmPerk_shortdesc");
}

FUNCTION(PhantasmPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "PhantasmPerk_desc");
}

FUNCTION(PhantasmPerk, onPlayerSpawn)) {
    _inst.walk_timer = 0.0;
    _inst.cooldown = 0.0;
    // Ensure player is solid on spawn
    SetEntityMoveType(_inst.client, MOVETYPE_WALK);
    SetEntityRenderMode(_inst.client, RENDER_NORMAL);
    SetEntityRenderColor(_inst.client, 255, 255, 255, 255);
}

FUNCTION(PhantasmPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_PHANTASM_HEALTH_PENALTY);
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_PHANTASM_ATTACK_PENALTY);
}

FUNCTION(PhantasmPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.cooldown <= 0.0) {
        _inst.cooldown = ZF_PHANTASM_COOLDOWN;
        _inst.walk_timer = GetGameTime() + ZF_PHANTASM_WALK_DURATION;

        SetEntityMoveType(client, MOVETYPE_NOCLIP);
        SetEntityRenderMode(client, RENDER_TRANSCOLOR);
        SetEntityRenderColor(client, 255, 255, 255, 150);
        
        // Prevent attacking
        SetEntProp(client, Prop_Send, "m_fFlags", GetEntProp(client, Prop_Send, "m_fFlags") | FL_FROZEN);

        PrintHintText(client, "你进入了虚空行走状态!");
    }
}

FUNCTION(PhantasmPerk, onPeriodic)) {
    int client = _inst.client;
    if (_inst.cooldown > 0.0) {
        _inst.cooldown -= 1.0;
    }

    if (_inst.walk_timer > 0.0 && GetGameTime() >= _inst.walk_timer) {
        _inst.walk_timer = 0.0;
        SetEntityMoveType(client, MOVETYPE_WALK);
        SetEntityRenderMode(client, RENDER_NORMAL);
        SetEntityRenderColor(client, 255, 255, 255, 255);
        SetEntProp(client, Prop_Send, "m_fFlags", GetEntProp(client, Prop_Send, "m_fFlags") & ~FL_FROZEN);
        PrintHintText(client, "虚空行走结束。");
    }
}

FUNCTION(PhantasmPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.walk_timer > 0.0) {
        Format(buffer, maxlen, "虚空行走: %.1fs", _inst.walk_timer - GetGameTime());
    } else if (_inst.cooldown > 0.0) {
        Format(buffer, maxlen, "冷却: %.0fs", _inst.cooldown);
    } else {
        Format(buffer, maxlen, "虚空行走已准备就绪");
    }
}