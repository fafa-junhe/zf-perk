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
#define ZF_VINDICTIVE_DAMAGE 75.0

methodmap VindictivePerk < ZombieBasePerk {
    public VindictivePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VindictivePerk sm = view_as<VindictivePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, VindictivePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, VindictivePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, VindictivePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, VindictivePerkFonDeath);

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
    strcopy(buffer, maxlen, "死亡时对凶手造成伤害");
}

FUNCTION(VindictivePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "死后对杀死你的敌人造成一次伤害。");
}

FUNCTION(VindictivePerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (validLivingSur(killer)) {
        SDKHooks_TakeDamage(killer, _inst.client, _inst.client, ZF_VINDICTIVE_DAMAGE, DMG_BLAST);
    }
}
