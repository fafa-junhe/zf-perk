#if defined __ResourcefulPerk_included
#endinput
#endif
#define __ResourcefulPerk_included


#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_RESOURCEFUL_AMMOPCT 0.20
#define ZF_RESOURCEFUL_ATTACK 25
#define ZF_RESOURCEFUL_DEFEND 25
#define ZF_RESOURCEFUL_HEALTH 25
#define ZF_RESOURCEFUL_HEALTH_OVERHEAL 15
#define ZF_RESOURCEFUL_METAL 25
#define ZF_RESOURCEFUL_PICKUP_DURATION 10

methodmap ResourcefulPerk < SurvivorBasePerk {
    public ResourcefulPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        ResourcefulPerk sm = view_as<ResourcefulPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ResourcefulPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ResourcefulPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ResourcefulPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, ResourcefulPerkFonPlayerDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_AMMO_PICKUP, ResourcefulPerkFonAmmoPickup);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_MED_PICKUP, ResourcefulPerkFonMedPickup);

        return sm;
    }
}

stock SurvivorBasePerk ResourcefulPerkFnew(int client) {
    return new ResourcefulPerk(client);
}

FUNCTION(ResourcefulPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Resourceful");
}

FUNCTION(ResourcefulPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Friends with pickups");
}

FUNCTION(ResourcefulPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Your kills give you ammo, health, and metal. Ammo packs give you a temporary attack bonus, and medkits give you a temporary defense bonus.");
}

FUNCTION(ResourcefulPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (killer == _inst.client && validZom(victim)) {
        addHealth(_inst.client, ZF_RESOURCEFUL_HEALTH, ZF_RESOURCEFUL_HEALTH_OVERHEAL);
        addResAmmoPct(_inst.client, 0, ZF_RESOURCEFUL_AMMOPCT);
        addResAmmoPct(_inst.client, 1, ZF_RESOURCEFUL_AMMOPCT);
        addMetal(_inst.client, ZF_RESOURCEFUL_METAL);
    }
}

FUNCTION(ResourcefulPerk, onAmmoPickup), int pickup) {
    addStatTempStack(_inst.client, ZFStatAtt, ZF_RESOURCEFUL_ATTACK, ZF_RESOURCEFUL_PICKUP_DURATION);
}

FUNCTION(ResourcefulPerk, onMedPickup), int pickup) {
    addStatTempStack(_inst.client, ZFStatDef, ZF_RESOURCEFUL_DEFEND, ZF_RESOURCEFUL_PICKUP_DURATION);
}