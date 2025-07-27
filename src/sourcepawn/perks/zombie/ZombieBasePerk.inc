#if defined __ZombieBasePerk_included
#endinput
#endif
#define __ZombieBasePerk_included

#include "../BasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

methodmap ZombieBasePerk < BasePerk {
    public ZombieBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        ZombieBasePerk sm = view_as<ZombieBasePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ZombieBasePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ZombieBasePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ZombieBasePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, ZombieBasePerkFonDealDamagePost);

        return sm;
    }
}

stock BasePerk ZombieBasePerkFnew(int client) {
    return new ZombieBasePerk(client);
}


FUNCTION(ZombieBasePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

FUNCTION(ZombieBasePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Without perks");
}

FUNCTION(ZombieBasePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Please select one perk to check info");
}

FUNCTION(ZombieBasePerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 当玩家造成伤害时触发。
     */
}

