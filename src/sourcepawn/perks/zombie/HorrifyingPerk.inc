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
#define HORRIFYING_ATTACK -20
#define HORRIFYING_ATTACK_HEAVY -30
#define HORRIFYING_DEFEND 0
#define HORRIFYING_DEFEND_HEAVY 0
#define HORRIFYING_ROF_HEAVY -10
#define HORRIFYING_PENALTYPCT_KILL 0.75
#define HORRIFYING_PENALTYPCT_ASSIST 0.25
#define HORRIFYING_DURATION 15
#define HORRIFYING_DURATION_HEAVY 30

methodmap HorrifyingPerk < ZombieBasePerk {
    public HorrifyingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        HorrifyingPerk sm = view_as<HorrifyingPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, HorrifyingPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, HorrifyingPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, HorrifyingPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, HorrifyingPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, HorrifyingPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, HorrifyingPerkFonPlayerDeath);
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
    strcopy(buffer, maxlen, "攻击削弱人类");
}

FUNCTION(HorrifyingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你的攻击力降低,但你的攻击能降低幸存者的攻击力、防御力和攻击速度。减益效果持续15秒。你死亡后,这个效果也随即消失。");
}

FUNCTION(HorrifyingPerk, onPlayerSpawn)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, HORRIFYING_ATTACK);
    createAura(_inst.client, ZFPART_AURAOUTBLU, AttachBase);
}

FUNCTION(HorrifyingPerk, onRemove)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, -HORRIFYING_ATTACK);
    if (validAura(_inst.client)) {
        removeAura(_inst.client);
    }
}

FUNCTION(HorrifyingPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker != _inst.client || !isSur(victim) || !(damagetype & DMG_CLUB)) {
        return;
    }

    bool isHeavyAttacker = TF2_GetPlayerClass(attacker) == TFClass_Heavy;
    int duration = isHeavyAttacker ? HORRIFYING_DURATION_HEAVY : HORRIFYING_DURATION;

    int attack_debuff = isHeavyAttacker ? HORRIFYING_ATTACK_HEAVY : HORRIFYING_ATTACK;
    int defend_debuff = isHeavyAttacker ? HORRIFYING_DEFEND_HEAVY : HORRIFYING_DEFEND;
    
    addStatTempStack(victim, ZFStatAtt, attack_debuff, duration);
    addStatTempStack(victim, ZFStatDef, defend_debuff, duration);

    if (isHeavyAttacker) {
        addStatTempStack(victim, ZFStatRof, HORRIFYING_ROF_HEAVY, duration);
    }
}

FUNCTION(HorrifyingPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim != _inst.client) {
        return;
    }

    // When the horrifying zombie dies, scale down the debuffs on the survivors who killed/assisted
    if (validSur(killer)) {
        scaleStatTempPct(killer, ZFStatAtt, HORRIFYING_PENALTYPCT_KILL);
        scaleStatTempPct(killer, ZFStatDef, HORRIFYING_PENALTYPCT_KILL);
        scaleStatTempPct(killer, ZFStatRof, HORRIFYING_PENALTYPCT_KILL);
    }
    if (validSur(assist) && assist != killer) {
        scaleStatTempPct(assist, ZFStatAtt, HORRIFYING_PENALTYPCT_ASSIST);
        scaleStatTempPct(assist, ZFStatDef, HORRIFYING_PENALTYPCT_ASSIST);
        scaleStatTempPct(assist, ZFStatRof, HORRIFYING_PENALTYPCT_ASSIST);
    }
}
