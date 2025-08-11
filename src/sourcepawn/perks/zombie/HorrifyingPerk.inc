#if defined __HorrifyingPerk_included
#endinput
#endif
#define __HorrifyingPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/sourcemod.inc"
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
#define HORRIFYING_ATTACK -8
#define HORRIFYING_ATTACK_HEAVY -8
#define HORRIFYING_DEFEND -15
#define HORRIFYING_DEFEND_HEAVY -15
#define HORRIFYING_DURATION 10
#define HORRIFYING_DURATION_HEAVY 20
#define HORRIFYING_ROF -25

methodmap HorrifyingPerk < ZombieBasePerk {
    public HorrifyingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        HorrifyingPerk sm = view_as<HorrifyingPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, HorrifyingPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, HorrifyingPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, HorrifyingPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, HorrifyingPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, HorrifyingPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, HorrifyingPerkFonRemove);

        return sm;
    }
}

stock BasePerk HorrifyingPerkFnew(int client) {
    return new HorrifyingPerk(client);
}

FUNCTION(HorrifyingPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Horrifying");
}

FUNCTION(HorrifyingPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "HorrifyingPerk_shortdesc");
}

FUNCTION(HorrifyingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "HorrifyingPerk_desc");
}

FUNCTION(HorrifyingPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, HORRIFYING_ATTACK);
    addStat(_inst.client, ZFStatRof, ZFStatTypePerm, HORRIFYING_ROF);
    createAura(_inst.client, ZFPART_AURAOUTBLU, AttachBase);
}

FUNCTION(HorrifyingPerk, onRemove)) {
    if (validAura(_inst.client)) {
        removeAura(_inst.client);
    }
}

FUNCTION(HorrifyingPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker != _inst.client || !isSur(victim) || !(damagetype & DMG_CLUB)) {
        return;
    }
    if (victim == attacker) {
        return;
    }
    bool isHeavyAttacker = TF2_GetPlayerClass(attacker) == TFClass_Heavy;
    int duration = isHeavyAttacker ? HORRIFYING_DURATION_HEAVY : HORRIFYING_DURATION;

    int attack_debuff = isHeavyAttacker ? HORRIFYING_ATTACK_HEAVY : HORRIFYING_ATTACK;
    int defend_debuff = isHeavyAttacker ? HORRIFYING_DEFEND_HEAVY : HORRIFYING_DEFEND;
    
    addStatTempStack(victim, ZFStatAtt, attack_debuff, duration);
    addStatTempStack(victim, ZFStatDef, attack_debuff, duration);

    addStatTempStack(victim, ZFStatSpeed, defend_debuff, duration);
    addStatTempStack(victim, ZFStatCrit, defend_debuff, duration);
    TF2_StunPlayer(victim, 0.3, 0.0, TF_STUNFLAGS_GHOSTSCARE, attacker);

}
