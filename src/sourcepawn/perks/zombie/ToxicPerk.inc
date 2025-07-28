#if defined __ToxicPerk_included
#endinput
#endif
#define __ToxicPerk_included

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

// Defines from all_perks.md
#define ZF_TOXIC_DURATION 3.0
#define ZF_TOXIC_SPEED -25
#define ZF_TOXIC_ROF -25

methodmap ToxicPerk < ZombieBasePerk {
    public ToxicPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ToxicPerk sm = view_as<ToxicPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ToxicPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ToxicPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ToxicPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, ToxicPerkFonDealDamagePost);

        return sm;
    }
}

stock BasePerk ToxicPerkFnew(int client) {
    return new ToxicPerk(client);
}

FUNCTION(ToxicPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Toxic");
}

FUNCTION(ToxicPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ToxicPerk_shortdesc");
}

FUNCTION(ToxicPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ToxicPerk_desc");
}

FUNCTION(ToxicPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (validLivingSur(victim) && (damagetype & DMG_CLUB)) {
        // Apply poison effect
        addStatTempStack(victim, ZFStatSpeed, ZF_TOXIC_SPEED, RoundToCeil(ZF_TOXIC_DURATION));
        addStatTempStack(victim, ZFStatRof, ZF_TOXIC_ROF, RoundToCeil(ZF_TOXIC_DURATION));
    }
}

