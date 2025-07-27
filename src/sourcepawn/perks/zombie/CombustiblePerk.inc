#if defined __CombustiblePerk_included
#endinput
#endif
#define __CombustiblePerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sdktools_functions.inc"
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
#define ZF_COMBUSTIBLE_DAMAGE 120
#define ZF_COMBUSTIBLE_DAMAGE_HEAVY 200
#define ZF_COMBUSTIBLE_DEFEND -200
#define ZF_COMBUSTIBLE_RADIUS 300
#define ZF_COMBUSTIBLE_RESPAWNTIME 4.5

methodmap CombustiblePerk < ZombieBasePerk {
    public CombustiblePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CombustiblePerk sm = view_as<CombustiblePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CombustiblePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CombustiblePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CombustiblePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CombustiblePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, CombustiblePerkFonPlayerDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, CombustiblePerkFonPlayerRunCmd);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CombustiblePerkFupdateCondStats);

        return sm;
    }
}

stock BasePerk CombustiblePerkFnew(int client) {
    return new CombustiblePerk(client);
}

FUNCTION(CombustiblePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Combustible");
}

FUNCTION(CombustiblePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "尸如其名");
}

FUNCTION(CombustiblePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你的防御力大幅降低。被远程武器击杀后,你会爆炸并造成伤害。你不可以使用隐身手表或者原子能饮料。(远程武器包括一切非近战武器)");
}

FUNCTION(CombustiblePerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_COMBUSTIBLE_DEFEND);
}

FUNCTION(CombustiblePerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim != _inst.client || !isSur(killer)) {
        return;
    }

    // Explode if killed by a non-melee weapon
    if (!(damagetype & DMG_CLUB)) {
        int damage = (TF2_GetPlayerClass(victim) == TFClass_Heavy) ? ZF_COMBUSTIBLE_DAMAGE_HEAVY : ZF_COMBUSTIBLE_DAMAGE;
        
        float pos[3];
        GetClientAbsOrigin(victim, pos);
        
        // Create an explosion at the player's location
        applyDamageRadialAtClient(victim, damage, ZF_COMBUSTIBLE_RADIUS, true);             
        
        fxExplosionBig(victim);
        
        // Force respawn
        CreateTimer(ZF_COMBUSTIBLE_RESPAWNTIME, Timer_RespawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
    }
}

public Action Timer_RespawnClient(Handle timer, any client) {
    if (client > 0 && IsClientInGame(client) && !IsPlayerAlive(client)) {
        TF2_RespawnPlayer(client);
    }
    return Plugin_Stop;
}

FUNCTION(CombustiblePerk, onPlayerRunCmd), int buttons, int impulse, float vel[3], float[] angles, int weapon) {
    int client = _inst.client;
    if (weapon != -1) {
        char weapon_name[64];
        GetEdictClassname(weapon, weapon_name, sizeof(weapon_name));

        if (StrContains(weapon_name, "tf_weapon_invis") != -1 || StrContains(weapon_name, "tf_weapon_bonk") != -1) {
            // Prevent usage by stripping the weapon
            RemovePlayerItem(client, weapon);
            TF2_RemoveCondition(client, TFCond_Bonked);
        }
    }
}

FUNCTION(CombustiblePerk, updateCondStats), char[] buffer, int maxlen) {
    // Visual hint for the player
    fxExplosionTiny(_inst.client);
    strcopy(buffer, maxlen, "Ready to explode on death");
}
