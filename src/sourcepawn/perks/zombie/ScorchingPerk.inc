#if defined __ScorchingPerk_included
#endinput
#endif
#define __ScorchingPerk_included

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
#include <tf2>

// Defines from all_perks.md
#define ZF_SCORCHING_ATTACK -50
#define ZF_SCORCHING_SPEED 50
#define ZF_SCORCH_DURATION 10.0

methodmap ScorchingPerk < ZombieBasePerk {
    public ScorchingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ScorchingPerk sm = view_as<ScorchingPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ScorchingPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ScorchingPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ScorchingPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, ScorchingPerkFupdatePermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, ScorchingPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, ScorchingPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, ScorchingPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_TOUCH, ScorchingPerkFonPlayerTouch);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, ScorchingPerkFonPlayerRunCmd);

        return sm;
    }
}

stock BasePerk ScorchingPerkFnew(int client) {
    return new ScorchingPerk(client);
}

FUNCTION(ScorchingPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Scorching");
}

FUNCTION(ScorchingPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "免疫火焰,攻击点燃敌人");
}

FUNCTION(ScorchingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "免疫火焰伤害,攻击点燃敌人。");
}

FUNCTION(ScorchingPerk, updatePermStats)) {
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_SCORCHING_SPEED);
}

FUNCTION(ScorchingPerk, onPeriodic)) {
    int client = _inst.client;
    if (!(GetEntityFlags(client) & FL_INWATER)) {
        TF2_IgnitePlayer(client, client, 2.0); // Constantly re-ignite
    }
}

FUNCTION_FLOAT(ScorchingPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {

    // Case 1: This perk's owner is the attacker. Apply damage modification.
    if (attacker == _inst.client) {
        float new_damage = damage * (100.0 + float(ZF_SCORCHING_ATTACK)) / 100.0;
        return new_damage;
    }
    // Case 2: This perk's owner is the victim and is taking fire damage. Grant immunity.
    else if (victim == _inst.client && (damagetype & DMG_BURN)) {
        return 0.0;
    }

    // Default case: Do not interfere with the damage event.
    return damage;
}

FUNCTION(ScorchingPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker == _inst.client && isSur(victim) && (damagetype & DMG_CLUB)) {
        TF2_IgnitePlayer(victim, _inst.client, ZF_SCORCH_DURATION);
    }
}

FUNCTION(ScorchingPerk, onPlayerTouch), int other) {
    int client = _inst.client;
    if (isSur(other) && !isPyro(other)) {
        TF2_IgnitePlayer(other, client, ZF_SCORCH_DURATION);
    }
}

FUNCTION(ScorchingPerk, onPlayerRunCmd), int &buttons) {
    int client = _inst.client;
    int weapon = GetEntPropEnt(client, Prop_Data, "m_hActiveWeapon");
    if (weapon != -1) {
        char classname[64];
        GetEdictClassname(weapon, classname, sizeof(classname));
        if (StrContains(classname, "tf_weapon_bonk") != -1) {
            if (buttons & IN_ATTACK2) {
                buttons &= ~IN_ATTACK2;
            }
        }
    }
}
