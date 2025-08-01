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
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TOUCH, ScorchingPerkFonTouch);
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
    strcopy(buffer, maxlen, "ScorchingPerk_shortdesc");
}

FUNCTION(ScorchingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ScorchingPerk_desc");
}

FUNCTION(ScorchingPerk, updatePermStats)) {
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_SCORCHING_SPEED);
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_SCORCHING_ATTACK);
}

FUNCTION(ScorchingPerk, onPeriodic)) {
    int client = _inst.client;
    if (!(GetEntityFlags(client) & FL_INWATER)) {
        TF2_IgnitePlayer(client, client, 2.0); // Constantly re-ignite
    }
}

FUNCTION_FLOAT(ScorchingPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    // If this perk's owner is the victim and is taking fire damage, grant immunity.
    if (victim == _inst.client && (damagetype & DMG_BURN)) {
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

FUNCTION(ScorchingPerk, onTouch), int toucher, int touchee) {
    // When the Scorching zombie touches a survivor
    if (toucher == _inst.client && isSur(touchee)) {
        // Ignite them if they are not a pyro
        if (!isPyro(touchee)) {
            TF2_IgnitePlayer(touchee, toucher, ZF_SCORCH_DURATION);
        }
        
        // Remove bonk effect from the survivor
        if (isBonked(touchee)) {
            TF2_RemoveCondition(touchee, TFCond_Bonked);
        }
    }
}

FUNCTION(ScorchingPerk, onPlayerRunCmd), int &buttons, int &impulse, float vel[3], float angles[3], int &weapon) {
    int client = _inst.client;
    
    // Prevent using Bonk drink
    int currentActiveWeapon = GetEntPropEnt(client, Prop_Data, "m_hActiveWeapon");
    if (currentActiveWeapon != -1) {
        char classname[64];
        GetEdictClassname(currentActiveWeapon, classname, sizeof(classname));
        if (StrContains(classname, "tf_weapon_bonk") != -1) {
            if (buttons & IN_ATTACK2) {
                buttons &= ~IN_ATTACK2;
            }
        }
    }
}
