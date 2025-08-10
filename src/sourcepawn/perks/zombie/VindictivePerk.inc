#if defined __VindictivePerk_included
#endinput
#endif
#define __VindictivePerk_included

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
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include <sdktools_functions>

// Defines from all_perks.md
#define ZF_VINDICTIVE_KILL_DAMAGE_BONUS 20
#define ZF_VINDICTIVE_KILL_RESIST_BONUS 20
#define ZF_VINDICTIVE_ASSIST_DAMAGE_BONUS 10
#define ZF_VINDICTIVE_ASSIST_RESIST_BONUS 10
#define ZF_VINDICTIVE_KILL_SPEED_BONUS 20
#define ZF_VINDICTIVE_KILL_CRIT_BONUS 20
#define ZF_VINDICTIVE_ASSIST_SPEED_BONUS 10
#define ZF_VINDICTIVE_ASSIST_CRIT_BONUS 10

methodmap VindictivePerk < ZombieBasePerk {
    public VindictivePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VindictivePerk sm = view_as<VindictivePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, VindictivePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, VindictivePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, VindictivePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, VindictivePerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, VindictivePerkFonAssistKill);

        return sm;
    }
}

stock BasePerk VindictivePerkFnew(int client) {
    return new VindictivePerk(client);
}

FUNCTION(VindictivePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Vindictive");
}

FUNCTION(VindictivePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VindictivePerk_shortdesc");
}

FUNCTION(VindictivePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VindictivePerk_desc");
}

FUNCTION(VindictivePerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_VINDICTIVE_KILL_DAMAGE_BONUS);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_VINDICTIVE_KILL_RESIST_BONUS);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_VINDICTIVE_KILL_SPEED_BONUS);
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_VINDICTIVE_KILL_CRIT_BONUS);
}

FUNCTION(VindictivePerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_VINDICTIVE_ASSIST_DAMAGE_BONUS);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_VINDICTIVE_ASSIST_RESIST_BONUS);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_VINDICTIVE_ASSIST_SPEED_BONUS);
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_VINDICTIVE_ASSIST_CRIT_BONUS);
}
