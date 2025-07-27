#if defined __SwarmingPerk_included
#endinput
#endif
#define __SwarmingPerk_included

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
#include "../../zf_util_fx.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_SWARMING_COMBAT -20
#define ZF_SWARMING_RADIUSSQ (400 * 400)
#define ZF_SWARMING_SPEED 50
#define ZF_SWARMING_RESPAWNTIME 0.5

methodmap SwarmingPerk < ZombieBasePerk {
    public SwarmingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        SwarmingPerk sm = view_as<SwarmingPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SwarmingPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, SwarmingPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, SwarmingPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SwarmingPerkFupdatePermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, SwarmingPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, SwarmingPerkFonDeath);

        return sm;
    }
}

stock BasePerk SwarmingPerkFnew(int client) {
    return new SwarmingPerk(client);
}

FUNCTION(SwarmingPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Swarming");
}

FUNCTION(SwarmingPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "快速复活僵尸");
}

FUNCTION(SwarmingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你有移速加成,但你的攻击和防御力降低。你能使自己与身边的队友快速重生。");
}

FUNCTION(SwarmingPerk, updatePermStats)) {
    int client = _inst.client;
    addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_SWARMING_COMBAT);
    addStat(client, ZFStatDef, ZFStatTypePerm, ZF_SWARMING_COMBAT);
    addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_SWARMING_SPEED);
}

FUNCTION(SwarmingPerk, onPlayerSpawn)) {
    createAura(_inst.client, ZFPART_AURAFLIES, AttachBase);
}

FUNCTION(SwarmingPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    // Self-respawn
    if (victim == client) {
        CreateTimer(ZF_SWARMING_RESPAWNTIME, timer_RespawnPlayer, GetClientUserId(victim), TIMER_FLAG_NO_MAPCHANGE);
    }
    // Aura respawn for teammates
    else if (isZom(victim) && victim != client) {
        float pos_victim[3], pos_client[3];
        GetClientAbsOrigin(victim, pos_victim);
        GetClientAbsOrigin(client, pos_client);

        if (GetVectorDistance(pos_victim, pos_client, true) <= ZF_SWARMING_RADIUSSQ) {
            CreateTimer(ZF_SWARMING_RESPAWNTIME, timer_RespawnPlayer, GetClientUserId(victim), TIMER_FLAG_NO_MAPCHANGE);
        }
    }
}

public Action timer_RespawnPlayer(Handle timer, any userid) {
    int client = GetClientOfUserId(userid);
    if (client > 0 && !IsPlayerAlive(client)) {
        TF2_RespawnPlayer(client);
    }
    return Plugin_Stop;
}
