// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/AthleticPerk.py

#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../zf_perk.inc"
#include "../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>

#define ZF_ATHLETIC_ATTACK -(40)
#define ZF_ATHLETIC_CRIT -(100)
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100

methodmap AthleticPerk < SurvivorBasePerk {
    public AthleticPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AthleticPerk sm = view_as<AthleticPerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteFunction(AthleticPerk_getName);
        sm.Position = DP(2);
        sm.WriteFunction(AthleticPerk_getShortdesc);
        sm.Position = DP(3);
        sm.WriteFunction(AthleticPerk_getDesc);
        sm.Position = DP(19);
        sm.WriteFunction(AthleticPerk_updateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk AthleticPerk_new(int client) {
    return new AthleticPerk(client);
}


public void AthleticPerk_getName(AthleticPerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Athletic");
}

public void AthleticPerk_getShortdesc(AthleticPerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Faster movement and ROF");
}

public void AthleticPerk_getDesc(AthleticPerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Faster movement and ROF!!");
}

public void AthleticPerk_updateClientPermStats(AthleticPerk _inst) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(_inst.client, ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}

