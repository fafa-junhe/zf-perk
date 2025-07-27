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

methodmap StirCrazyPerk < SurvivorBasePerk {
    public StirCrazyPerk(any client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        StirCrazyPerk sm = view_as<StirCrazyPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, StirCrazyPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, StirCrazyPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, StirCrazyPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, StirCrazyPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, StirCrazyPerkFupdateCondStats);

        return sm;
    }
}

stock SurvivorBasePerk StirCrazyPerkFnew(any client) {
    return new StirCrazyPerk(client);
}


FUNCTION(StirCrazyPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Stir-Crazy");
}

FUNCTION(StirCrazyPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Speed determines attack power");
}

FUNCTION(StirCrazyPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Your movement speed gives you an attack power bonus. The faster you move, the higher your attack power.");
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
