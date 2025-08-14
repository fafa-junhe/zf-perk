#if defined __CatalystPerk_included
#endinput
#endif
#define __CatalystPerk_included

#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_util_fx.inc"

// Defines
#define CATALYST_DEFENSE_PENALTY -50
#define CATALYST_RADIUS 450.0
#define CATALYST_EXPLOSION_DAMAGE 20.0
#define CATALYST_EXPLOSION_RADIUS 150.0

methodmap CatalystPerk < ZombieBasePerk {
    public CatalystPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CatalystPerk sm = view_as<CatalystPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, CatalystPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CatalystPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, CatalystPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CatalystPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, CatalystPerkFonRemove);

        return sm;
    }
}

stock BasePerk CatalystPerkFnew(int client) {
    return new CatalystPerk(client);
}

FUNCTION(CatalystPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Catalyst");
}

FUNCTION(CatalystPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CatalystPerk_shortdesc");
}

FUNCTION(CatalystPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CatalystPerk_desc");
}

FUNCTION_INT(CatalystPerk, getCategory))
{
    return 4; // Aura
}

FUNCTION(CatalystPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, CATALYST_DEFENSE_PENALTY);
}

FUNCTION(CatalystPerk, onPlayerSpawn)) {
    int client = _inst.client;
    createAura(client, ZFPART_AURACATALYST, AttachBase);
}

FUNCTION(CatalystPerk, onRemove)) {
    int client = _inst.client;
    if (validAura(client)) {
        removeAura(client);
    }
}

FUNCTION(CatalystPerk, updateCondStats), char[] buffer, int maxlen) {
    Format(buffer, maxlen, "%t", "CatalystPerk_Status_Active");
}