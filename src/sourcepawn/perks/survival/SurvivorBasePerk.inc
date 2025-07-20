// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/SurvivorBasePerk.py

#if defined __SurvivorBasePerk_included
#endinput
#endif
#define __SurvivorBasePerk_included

#define DP(%1) view_as<DataPackPos>(%1)
#include "../BasePerk.inc"
#include <datapack>

methodmap SurvivorBasePerk < BasePerk {
    public SurvivorBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        SurvivorBasePerk sm = view_as<SurvivorBasePerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteString("nooooo");
        sm.Position = DP(20);
        sm.WriteFunction(SurvivorBasePerk_onCharitableGiftTouched);

        return sm;
    }

    public void onCharitableGiftTouched(int entity, int other) {
        this.Position = DP(20);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(entity);
        Call_PushCell(other);
        Call_Finish();
    }
}


public void SurvivorBasePerk_onCharitableGiftTouched(SurvivorBasePerk _inst, int entity, int other) {
    /**
     * 当慈善家扔出的礼物被触摸时触发。
     */
}

