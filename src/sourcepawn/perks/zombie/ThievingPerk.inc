#if defined __ThievingPerk_included
#endinput
#endif
#define __ThievingPerk_included

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
#define ZF_THIEVING_AMMO_STEAL 10
#define ZF_THIEVING_AMMO_STEAL_CROUCH 25

methodmap ThievingPerk < ZombieBasePerk {
    public ThievingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ThievingPerk sm = view_as<ThievingPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ThievingPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ThievingPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ThievingPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, ThievingPerkFonDealDamagePost);

        return sm;
    }
}

stock BasePerk ThievingPerkFnew(int client) {
    return new ThievingPerk(client);
}

FUNCTION(ThievingPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Thieving");
}

FUNCTION(ThievingPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "攻击偷取弹药");
}

FUNCTION(ThievingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "攻击敌人偷取弹药,潜行状态下攻击敌人会偷取更多弹药。");
}

FUNCTION(ThievingPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (!validLivingSur(victim) || !(damagetype & DMG_CLUB)) {
        return;
    }

    int client = _inst.client;
    int amount_to_steal = (GetClientButtons(client) & IN_DUCK) ? ZF_THIEVING_AMMO_STEAL_CROUCH : ZF_THIEVING_AMMO_STEAL;

    int active_weapon = GetEntPropEnt(victim, Prop_Send, "m_hActiveWeapon");
    if (active_weapon != -1) {
        int ammo_type = GetEntProp(active_weapon, Prop_Send, "m_iPrimaryAmmoType");
        if (ammo_type != -1) {
            int current_ammo = GetEntProp(victim, Prop_Send, "m_iAmmo", _, ammo_type);
            int new_ammo = current_ammo - amount_to_steal;
            if (new_ammo < 0) {
                new_ammo = 0;
            }
            SetEntProp(victim, Prop_Send, "m_iAmmo", new_ammo, _, ammo_type);
        }
    }
}
