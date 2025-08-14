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
#define ZF_SWARMING_RESPAWNTIME 3.0

methodmap SwarmingPerk < ZombieBasePerk {
    public SwarmingPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        SwarmingPerk sm = view_as<SwarmingPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, SwarmingPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SwarmingPerkFupdatePermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, SwarmingPerkFonPlayerSpawn);
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
    strcopy(buffer, maxlen, "SwarmingPerk_shortdesc");
}

FUNCTION(SwarmingPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "SwarmingPerk_desc");
}

FUNCTION_INT(SwarmingPerk, getCategory))
{
    return 4;
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

