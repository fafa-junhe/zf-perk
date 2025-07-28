
#if defined __CowardlyPerk_included
#endinput
#endif
#define __CowardlyPerk_included

#include "../../../../include/halflife.inc"
#include "../../../../include/sdktools_sound.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_COWARDLY_DEFEND 50
#define ZF_COWARDLY_SPEED 200
#define ZF_COWARDLY_DURATION_SCARED 5
#define ZF_COWARDLY_DURATION_COOLDOWN 30

#define COWARDLY_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap CowardlyPerk < SurvivorBasePerk {
    public CowardlyPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        CowardlyPerk sm = view_as<CowardlyPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CowardlyPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CowardlyPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CowardlyPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, CowardlyPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, CowardlyPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CowardlyPerkFupdateCondStats);
        sm.Position = view_as<DataPackPos>(COWARDLY_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(COWARDLY_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(COWARDLY_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk CowardlyPerkFnew(int client) {
    return new CowardlyPerk(client);
}


FUNCTION(CowardlyPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Cowardly");
}

FUNCTION(CowardlyPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CowardlyPerk_shortdesc");
}

FUNCTION(CowardlyPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CowardlyPerk_desc");
}

FUNCTION_FLOAT(CowardlyPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if ((_inst.client != victim)) {
        return damage;
    }
    if (((_inst.perk_timer == 0)) && (validClient(attacker)) && (validZom(attacker))) {
        if ((damagetype & (1 << 3))) {
            _inst.perk_timer = (ZF_COWARDLY_DURATION_SCARED + ZF_COWARDLY_DURATION_COOLDOWN);
            fxCreateSoundToClient("player/vo/yikes.wav", _inst.client);
            PrintHintText(_inst.client, "You panicked!");
            return 0.0;
        }
    }
    return damage;
}

FUNCTION(CowardlyPerk, onPeriodic)) {
    if ((_inst.perk_timer > 0)) {
        _inst.perk_timer -= 1;
        if ((_inst.perk_timer == ZF_COWARDLY_DURATION_COOLDOWN)) {
            PrintHintText(_inst.client, "Panic has worn off.");
        }
        else {
            if ((_inst.perk_timer == 0)) {
                PrintHintText(_inst.client, "You are no longer panicked and can use your ability again.");
            }
        }
    }
}

FUNCTION(CowardlyPerk, updateCondStats), char[] buffer, int maxlen) {
    if ((_inst.perk_timer > ZF_COWARDLY_DURATION_COOLDOWN)) {
        addStat(_inst.client, ZFStatDef, ZFStatTypeCond, ZF_COWARDLY_DEFEND);
        addStat(_inst.client, ZFStatSpeed, ZFStatTypeCond, ZF_COWARDLY_SPEED);
    } else if (_inst.perk_timer == 0) {
        Format(buffer, maxlen, "(随时准备跑路)");
    }
}

