#if defined __VampiricPerk_included
#endinput
#endif
#define __VampiricPerk_included

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
#define ZF_VAMPIRIC_LIFESTEAL_RATIO 0.6  // 50% of damage dealt is returned as health
#define ZF_VAMPIRIC_REGEN 10             // Health regenerated per second
#define ZF_VAMPIRIC_DEFEND 10
#define ZF_VAMPIRIC_ATTACK -20

methodmap VampiricPerk < ZombieBasePerk {
    public VampiricPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VampiricPerk sm = view_as<VampiricPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, VampiricPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, VampiricPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, VampiricPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, VampiricPerkFupdateClientPermStats);

        return sm;
    }
}

stock BasePerk VampiricPerkFnew(int client) {
    return new VampiricPerk(client);
}

FUNCTION(VampiricPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Vampiric");
}

FUNCTION(VampiricPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VampiricPerk_shortdesc");
}

FUNCTION(VampiricPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VampiricPerk_desc");
}

FUNCTION_INT(VampiricPerk, getCategory))
{
    return 1;
}

FUNCTION(VampiricPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_VAMPIRIC_DEFEND);
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_VAMPIRIC_ATTACK);
}

FUNCTION(VampiricPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (!validLivingSur(victim)) {
        return;
    }

    int client = _inst.client;
    int health_to_heal = RoundToCeil(damage * ZF_VAMPIRIC_LIFESTEAL_RATIO);
    
    if (health_to_heal > 0) {
        addHealth(client, health_to_heal, true); // Allow overheal from lifesteal
    }
}

FUNCTION(VampiricPerk, onPeriodic)) {
    // Passive regen should not overheal. The addHealth function handles the max health check.
    addHealth(_inst.client, ZF_VAMPIRIC_REGEN, false);
}
