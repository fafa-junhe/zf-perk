#if defined __MarkedPerk_included
#endinput
#endif
#define __MarkedPerk_included

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
#define ZF_MARKED_ATTACK_ON_MARK 200
#define ZF_MARKED_ATTACK_OFF_MARK -10
#define ZF_MARKED_MIN_SURVIVORS 1
#define ZF_MARKED_TIMER 10

// Data slots
#define MARKED_DATA_TARGET (PERK_DATA_START_INDEX + 0)
#define MARKED_DATA_TIMER (PERK_DATA_START_INDEX + 1)

methodmap MarkedPerk < ZombieBasePerk {
    public MarkedPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        MarkedPerk sm = view_as<MarkedPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, MarkedPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, MarkedPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, MarkedPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GRACE_END, MarkedPerkFonGraceEnd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, MarkedPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, MarkedPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, MarkedPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, MarkedPerkFonRemove);

        // Initialize data
        sm.Position = view_as<DataPackPos>(MARKED_DATA_TARGET);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(MARKED_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int target {
        public get() {
            this.Position = view_as<DataPackPos>(MARKED_DATA_TARGET);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(MARKED_DATA_TARGET);
            this.WriteCell(value);
        }
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(MARKED_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(MARKED_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk MarkedPerkFnew(int client) {
    return new MarkedPerk(client);
}

FUNCTION(MarkedPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Marked");
}

FUNCTION(MarkedPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "瞄准特定目标");
}

FUNCTION(MarkedPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "系统会随机选择一名幸存者作为你的目标。你对目标能造成极高伤害,但是对其他人造成较低伤害。当前目标死亡后,若剩余的幸存者超过1个,10秒后将自动选择一个新目标。");
}

FUNCTION(MarkedPerk, onGraceEnd)) {
    doMarkedSelect(_inst);
}

FUNCTION(MarkedPerk, onRemove)) {
    int client = _inst.client;
    if (validIcon(client)) {
        removeIcon(client);
    }
}

FUNCTION_FLOAT(MarkedPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker == _inst.client && isSur(victim)) {
        if (victim == _inst.target) {
            return damage * (1.0 + (ZF_MARKED_ATTACK_ON_MARK / 100.0));
        } else {
            return damage * (1.0 + (ZF_MARKED_ATTACK_OFF_MARK / 100.0));
        }
    }
    return damage;
}

FUNCTION(MarkedPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
        if (_inst.perk_timer == 0) {
            doMarkedSelect(_inst);
        }
    }
}

FUNCTION(MarkedPerk, updateCondStats), char[] buffer, int maxlen) {
    int target = _inst.target;
    if (target > 0) {
        if (!validLivingSur(target)) {
            if (validIcon(_inst.client)) {
                removeIcon(_inst.client);
            }
            _inst.target = 0;
            if (local_getSurCount() > ZF_MARKED_MIN_SURVIVORS) {
                _inst.perk_timer = ZF_MARKED_TIMER;
                Format(buffer, maxlen, "New Target in %ds", _inst.perk_timer);
            } else {
                Format(buffer, maxlen, "No targets left");
            }
        } else {
            char targetName[MAX_NAME_LENGTH];
            GetClientName(target, targetName, sizeof(targetName));
            Format(buffer, maxlen, "Target: %s", targetName);
        }
    } else if (_inst.perk_timer > 0) {
        Format(buffer, maxlen, "New Target in %ds", _inst.perk_timer);
    } else {
        Format(buffer, maxlen, "Selecting target...");
    }
}

int local_getSurCount() {
    int count = 0;
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            count++;
        }
    }
    return count;
}

void doMarkedSelect(MarkedPerk inst) {
    int survivors[MAXPLAYERS];
    int count = 0;
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            survivors[count++] = i;
        }
    }

    if (count > 0) {
        int target = survivors[GetRandomInt(0, count - 1)];
        inst.target = target;
        // Remove old icon before creating a new one
        if (validIcon(inst.client)) {
            removeIcon(inst.client);
        }
        createIcon(inst.client, target, ZFSPR_CROSSHAIR);
    } else {
        inst.target = -1; // No targets available
    }
}
