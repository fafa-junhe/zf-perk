#if defined __StirCrazyPerk_included
#endinput
#endif
#define __StirCrazyPerk_included

#include "../../../../include/entity.inc"
#include "../../../../include/float.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_STIRCRAZY_MAX_POINTS 5
#define ZF_STIRCRAZY_DIST_MIN 150.0
#define ZF_STIRCRAZY_DIST_MAX 750.0
#define ZF_STIRCRAZY_ATTACK 30
#define ZF_STIRCRAZY_SPEED_ON_KILL 1
#define ZF_STIRCRAZY_DEFEND -15

methodmap StirCrazyPerk < SurvivorBasePerk {
    public StirCrazyPerk(any client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        StirCrazyPerk sm = view_as<StirCrazyPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, StirCrazyPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, StirCrazyPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, StirCrazyPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, StirCrazyPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, StirCrazyPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk StirCrazyPerkFnew(any client) {
    return new StirCrazyPerk(client);
}


FUNCTION(StirCrazyPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StirCrazy");
}

FUNCTION(StirCrazyPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StirCrazyPerk_shortdesc");
}

FUNCTION(StirCrazyPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "StirCrazyPerk_desc");
}

FUNCTION_INT(StirCrazyPerk, getCategory))
{
    return 1;
}

FUNCTION(StirCrazyPerk, onPlayerSpawn)) {
    float currentPos[3];
    GetClientAbsOrigin(_inst.client, currentPos);

    for (int i = 0; i < ZF_STIRCRAZY_MAX_POINTS; i++) {
        zf_perkPos[_inst.client][i] = currentPos;
    }
}

FUNCTION(StirCrazyPerk, updateCondStats)) {
    int client = _inst.client;

    // Update position buffer
    float currentPos[3];
    GetClientAbsOrigin(client, currentPos);
    int buffer_idx = zf_frameCounter % ZF_STIRCRAZY_MAX_POINTS;
    zf_perkPos[client][buffer_idx] = currentPos;

    // Calculate average position
    float avgPos[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < ZF_STIRCRAZY_MAX_POINTS; i++) {
        AddVectors(avgPos, zf_perkPos[client][i], avgPos);
    }
    ScaleVector(avgPos, 1.0 / float(ZF_STIRCRAZY_MAX_POINTS));

    // Calculate distance and apply bonus
    float dist = GetVectorDistance(currentPos, avgPos);
    if (dist >= ZF_STIRCRAZY_DIST_MIN) {
        float factor = (dist - ZF_STIRCRAZY_DIST_MIN) / (ZF_STIRCRAZY_DIST_MAX - ZF_STIRCRAZY_DIST_MIN);
        factor = fMin(1.0, fMax(0.0, factor));
        int attack_bonus = RoundToCeil(float(ZF_STIRCRAZY_ATTACK) * factor);
        addStat(client, ZFStatAtt, ZFStatTypeCond, attack_bonus);
    }
}

FUNCTION(StirCrazyPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (validZom(victim)) {
        addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_STIRCRAZY_SPEED_ON_KILL);
    }
}

FUNCTION(StirCrazyPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_STIRCRAZY_DEFEND);
}
