// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/SurvivorBasePerk.py

#if defined __SurvivorBasePerk_included
#endinput
#endif
#define __SurvivorBasePerk_included

#include "../BasePerk.inc"
#include <datapack>

methodmap SurvivorBasePerk < BasePerk {
    public SurvivorBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        SurvivorBasePerk sm = view_as<SurvivorBasePerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteFunction(SurvivorBasePerk_getName);
        sm.Position = DP(2);
        sm.WriteFunction(SurvivorBasePerk_getShortdesc);
        sm.Position = DP(3);
        sm.WriteFunction(SurvivorBasePerk_getDesc);
        sm.Position = DP(22);
        sm.WriteFunction(SurvivorBasePerk_onCharitableGiftTouched);

        return sm;
    }

    public void onCharitableGiftTouched(int entity, int other) {
        this.Position = DP(22);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(entity);
        Call_PushCell(other);
        Call_Finish();
    }
}

stock BasePerk SurvivorBasePerk_new(int client) {
    return new SurvivorBasePerk(client);
}


public void SurvivorBasePerk_getName(SurvivorBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

public void SurvivorBasePerk_getShortdesc(SurvivorBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Without perks");
}

public void SurvivorBasePerk_getDesc(SurvivorBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Please select one perk to check info");
}

public void SurvivorBasePerk_onCharitableGiftTouched(SurvivorBasePerk _inst, int entity, int other) {
    /**
     * 当慈善家扔出的礼物被触摸时触发。
     */
}

