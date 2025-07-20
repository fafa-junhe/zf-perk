// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/AthleticPerk.py

#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#define DP(%1) view_as<DataPackPos>(%1)
#include "../../zf_perk.inc"
#include "../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>

methodmap AthleticPerk < SurvivorBasePerk {
    public AthleticPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AthleticPerk sm = view_as<AthleticPerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteString("okkk");
        sm.Position = DP(17);
        sm.WriteFunction(AthleticPerk_updateClientPermStats);

        return sm;
    }
}


public void AthleticPerk_updateClientPermStats(AthleticPerk _inst) {
    addStat(_inst.client, ZFStat.ZFStatAtt, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(_inst.client, ZFStat.ZFStatCrit, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(_inst.client, ZFStat.ZFStatRof, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(_inst.client, ZFStat.ZFStatSpeed, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}

