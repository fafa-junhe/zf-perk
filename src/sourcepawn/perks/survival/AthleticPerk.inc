#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_ATHLETIC_ATTACK -(40)
#define ZF_ATHLETIC_CRIT -(100)
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100

methodmap AthleticPerk < SurvivorBasePerk {
    public AthleticPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AthleticPerk sm = view_as<AthleticPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, AthleticPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, AthleticPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, AthleticPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, AthleticPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk AthleticPerkFnew(int client) {
    return new AthleticPerk(client);
}


FUNCTION(AthleticPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Athletic");
}

FUNCTION(AthleticPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AthleticPerk_shortdesc");
}

FUNCTION(AthleticPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AthleticPerk_desc");
}

FUNCTION(AthleticPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(_inst.client, ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}

