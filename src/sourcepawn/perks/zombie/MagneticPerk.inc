#if defined __MagneticPerk_included
#endinput
#endif
#define __MagneticPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/entity_prop_stocks.inc"
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
#define ZF_MAGNETIC_RADIUSSQ (500.0 * 500.0)

methodmap MagneticPerk < ZombieBasePerk {
    public MagneticPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        MagneticPerk sm = view_as<MagneticPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, MagneticPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, MagneticPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, MagneticPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, MagneticPerkFonPeriodic);

        return sm;
    }
}

stock BasePerk MagneticPerkFnew(int client) {
    return new MagneticPerk(client);
}

FUNCTION(MagneticPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Magnetic");
}

FUNCTION(MagneticPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "瘫痪附近建筑");
}

FUNCTION(MagneticPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你能使附近的步哨和地雷失效。“I will murder your toys as well.”");
}

FUNCTION(MagneticPerk, onPeriodic)) {
    // This logic is now handled globally in zf_perk.inc's periodic update
    // based on the user's feedback. This perk now primarily serves as a marker.
}
