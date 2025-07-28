
#if defined __TantrumPerk_included
#endinput
#endif
#define __TantrumPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_TANTRUM_ACTIVE 15
#define ZF_TANTRUM_COOLDOWN 30
#define ZF_TANTRUM_SPEED -(100)

#define TANTRUM_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap TantrumPerk < SurvivorBasePerk {
    public TantrumPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        TantrumPerk sm = view_as<TantrumPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, TantrumPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, TantrumPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, TantrumPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, TantrumPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, TantrumPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, TantrumPerkFupdateCondStats);

        sm.Position = view_as<DataPackPos>(TANTRUM_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(TANTRUM_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(TANTRUM_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk TantrumPerkFnew(int client) {
    return new TantrumPerk(client);
}


FUNCTION(TantrumPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Tantrum");
}

FUNCTION(TantrumPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "TantrumPerk_shortdesc");
}

FUNCTION(TantrumPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "TantrumPerk_desc");
}

FUNCTION(TantrumPerk, onCallForMedic)) {
    if ((_inst.perk_timer == 0)) {
        _inst.perk_timer = (ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN);
        fxKritzStart(_inst.client);
        PrintHintText(_inst.client, "You are enraged! CRITICAL HITS!");
    }
}

FUNCTION(TantrumPerk, onPeriodic)) {
    if ((_inst.perk_timer > 0)) {
        _inst.perk_timer -= 1;
        if ((_inst.perk_timer == ZF_TANTRUM_COOLDOWN)) {
            fxKritzStop(_inst.client);
            PrintHintText(_inst.client, "You are exhausted.");
        }
        else {
            if ((_inst.perk_timer == 0)) {
                PrintHintText(_inst.client, "Tantrum ability is ready.");
            }
        }
    }
}

FUNCTION(TantrumPerk, updateCondStats)) {
    if ((_inst.perk_timer > ZF_TANTRUM_COOLDOWN)) {
        addStat(_inst.client, ZFStatCrit, ZFStatTypeCond, 100);
    }
    else {
        if ((_inst.perk_timer > 0)) {
            addStat(_inst.client, ZFStatSpeed, ZFStatTypeCond, ZF_TANTRUM_SPEED);
        }
    }
}

