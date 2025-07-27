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
#define ZF_VAMPIRIC_LIFESTEAL_RATIO 0.5  // 50% of damage dealt is returned as health
#define ZF_VAMPIRIC_REGEN 5             // Health regenerated per second

methodmap VampiricPerk < ZombieBasePerk {
    public VampiricPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VampiricPerk sm = view_as<VampiricPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, VampiricPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, VampiricPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, VampiricPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, VampiricPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, VampiricPerkFonPeriodic);

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
    strcopy(buffer, maxlen, "攻击吸取生命值");
}

FUNCTION(VampiricPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "攻击敌人时会恢复少量生命值。");
}

FUNCTION(VampiricPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (!validLivingSur(victim)) {
        return;
    }

    int client = _inst.client;
    int health_to_heal = RoundFloat(damage * ZF_VAMPIRIC_LIFESTEAL_RATIO);
    
    if (health_to_heal > 0) {
        SetEntityHealth(client, GetClientHealth(client) + health_to_heal);
    }
}

FUNCTION(VampiricPerk, onPeriodic)) {
    int client = _inst.client;
    int current_health = GetClientHealth(client);
    int max_health = GetEntProp(client, Prop_Data, "m_iMaxHealth");
    if (current_health < max_health) {
        SetEntityHealth(client, current_health + ZF_VAMPIRIC_REGEN);
    }
}
