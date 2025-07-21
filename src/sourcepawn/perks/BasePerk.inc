// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/BasePerk.py

#if defined __BasePerk_included
#endinput
#endif
#define __BasePerk_included

#include <datapack>

methodmap BasePerk < DataPack {
    public BasePerk(int client) {
        DataPack sm_base = new DataPack();
        BasePerk sm = view_as<BasePerk>(sm_base);

        sm.Position = DP(0);
        sm.WriteCell(client);
        sm.Position = DP(1);
        sm.WriteFunction(BasePerk_getName);
        sm.Position = DP(2);
        sm.WriteFunction(BasePerk_getShortdesc);
        sm.Position = DP(3);
        sm.WriteFunction(BasePerk_getDesc);
        sm.Position = DP(4);
        sm.WriteFunction(BasePerk_onPlayerRunCmd);
        sm.Position = DP(5);
        sm.WriteFunction(BasePerk_onAmmoPickup);
        sm.Position = DP(6);
        sm.WriteFunction(BasePerk_onCalcIsAttackCritical);
        sm.Position = DP(7);
        sm.WriteFunction(BasePerk_onCallForMedic);
        sm.Position = DP(8);
        sm.WriteFunction(BasePerk_onGameFrame);
        sm.Position = DP(9);
        sm.WriteFunction(BasePerk_onGraceEnd);
        sm.Position = DP(10);
        sm.WriteFunction(BasePerk_onMedPickup);
        sm.Position = DP(11);
        sm.WriteFunction(BasePerk_onPeriodic);
        sm.Position = DP(12);
        sm.WriteFunction(BasePerk_onPlayerDeath);
        sm.Position = DP(13);
        sm.WriteFunction(BasePerk_onPlayerSpawn);
        sm.Position = DP(14);
        sm.WriteFunction(BasePerk_onRemove);
        sm.Position = DP(15);
        sm.WriteFunction(BasePerk_onSetTransmit);
        sm.Position = DP(16);
        sm.WriteFunction(BasePerk_onTakeDamage);
        sm.Position = DP(17);
        sm.WriteFunction(BasePerk_onTakeDamagePost);
        sm.Position = DP(18);
        sm.WriteFunction(BasePerk_onTouch);
        sm.Position = DP(19);
        sm.WriteFunction(BasePerk_updateClientPermStats);
        sm.Position = DP(20);
        sm.WriteFunction(BasePerk_updateCondStats);
        sm.Position = DP(21);
        sm.WriteFunction(BasePerk_doItemThrow);

        return sm;
    }

    property int client {
        public get() {
            this.Position = DP(0);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = DP(0);
            this.WriteCell(value);
        }
    }

    public void getName(char[] buffer, int maxlen) {
        this.Position = DP(1);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushStringEx(buffer, maxlen, SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK);
        Call_PushCell(maxlen);
        Call_Finish();
    }

    public void getShortdesc(char[] buffer, int maxlen) {
        this.Position = DP(2);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushStringEx(buffer, maxlen, SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK);
        Call_PushCell(maxlen);
        Call_Finish();
    }

    public void getDesc(char[] buffer, int maxlen) {
        this.Position = DP(3);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushStringEx(buffer, maxlen, SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK);
        Call_PushCell(maxlen);
        Call_Finish();
    }

    public void onPlayerRunCmd(int buttons, int impulse, any vel, any angles, int weapon) {
        this.Position = DP(4);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(buttons);
        Call_PushCell(impulse);
        Call_PushCell(vel);
        Call_PushCell(angles);
        Call_PushCell(weapon);
        Call_Finish();
    }

    public void onAmmoPickup(int pickup) {
        this.Position = DP(5);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(pickup);
        Call_Finish();
    }

    public void onCalcIsAttackCritical() {
        this.Position = DP(6);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onCallForMedic() {
        this.Position = DP(7);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onGameFrame() {
        this.Position = DP(8);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onGraceEnd() {
        this.Position = DP(9);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onMedPickup(int pickup) {
        this.Position = DP(10);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(pickup);
        Call_Finish();
    }

    public void onPeriodic() {
        this.Position = DP(11);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onPlayerDeath(int victim, int killer, int assist, int inflictor, int damagetype) {
        this.Position = DP(12);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(victim);
        Call_PushCell(killer);
        Call_PushCell(assist);
        Call_PushCell(inflictor);
        Call_PushCell(damagetype);
        Call_Finish();
    }

    public void onPlayerSpawn() {
        this.Position = DP(13);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onRemove() {
        this.Position = DP(14);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void onSetTransmit(int entity, int client) {
        this.Position = DP(15);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(entity);
        Call_PushCell(client);
        Call_Finish();
    }

    public void onTakeDamage(int victim, int attacker, int inflictor, float damage, int damagetype) {
        this.Position = DP(16);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(victim);
        Call_PushCell(attacker);
        Call_PushCell(inflictor);
        Call_PushFloat(damage);
        Call_PushCell(damagetype);
        Call_Finish();
    }

    public void onTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype) {
        this.Position = DP(17);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(victim);
        Call_PushCell(attacker);
        Call_PushCell(inflictor);
        Call_PushFloat(damage);
        Call_PushCell(damagetype);
        Call_Finish();
    }

    public void onTouch(int toucher, int touchee) {
        this.Position = DP(18);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(toucher);
        Call_PushCell(touchee);
        Call_Finish();
    }

    public void updateClientPermStats() {
        this.Position = DP(19);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void updateCondStats() {
        this.Position = DP(20);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish();
    }

    public void doItemThrow(char[] model, float force, any color) {
        this.Position = DP(21);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushString(model);
        Call_PushFloat(force);
        Call_PushCell(color);
        Call_Finish();
    }
}

stock BasePerk BasePerk_new(int client) {
    return new BasePerk(client);
}


public void BasePerk_getName(BasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

public void BasePerk_getShortdesc(BasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

public void BasePerk_getDesc(BasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Please select one perk to check info");
}

public void BasePerk_onPlayerRunCmd(BasePerk _inst, int buttons, int impulse, any vel, any angles, int weapon) {
    /**
     * 在玩家执行指令（如跳跃、攻击）时触发。
     */
}

public void BasePerk_onAmmoPickup(BasePerk _inst, int pickup) {
    /**
     * 当玩家拾取弹药包时触发。
     */
}

public void BasePerk_onCalcIsAttackCritical(BasePerk _inst) {
    /**
     * 在系统计算某次攻击是否为暴击时触发。
     */
}

public void BasePerk_onCallForMedic(BasePerk _inst) {
    /**
     * 当玩家按下“呼叫医生”键时触发。
     */
}

public void BasePerk_onGameFrame(BasePerk _inst) {
    /**
     * 在游戏的每一帧（或一个非常短的时间间隔）执行。
     */
}

public void BasePerk_onGraceEnd(BasePerk _inst) {
    /**
     * 在回合开始的准备时间结束后触发。
     */
}

public void BasePerk_onMedPickup(BasePerk _inst, int pickup) {
    /**
     * 当玩家拾取医疗包时触发。
     */
}

public void BasePerk_onPeriodic(BasePerk _inst) {
    /**
     * 周期性（通常是每秒）为所有玩家执行一次的函数。
     */
}

public void BasePerk_onPlayerDeath(BasePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    /**
     * 当任何玩家或僵尸死亡时触发。
     */
}

public void BasePerk_onPlayerSpawn(BasePerk _inst) {
    /**
     * 在玩家重生时触发。
     */
}

public void BasePerk_onRemove(BasePerk _inst) {
    /**
     * 在玩家取消选择此职业时触发
     */
}

public void BasePerk_onSetTransmit(BasePerk _inst, int entity, int client) {
    /**
     * 在确定一个实体是否应该对特定客户端可见时触发。
     */
}

public void BasePerk_onTakeDamage(BasePerk _inst, int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 在伤害事件发生时（伤害计算前）触发。
     */
}

public void BasePerk_onTakeDamagePost(BasePerk _inst, int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 在伤害事件发生后（伤害计算后）触发。
     */
}

public void BasePerk_onTouch(BasePerk _inst, int toucher, int touchee) {
    /**
     * 当一个实体触摸到另一个实体时触发。
     */
}

public void BasePerk_updateClientPermStats(BasePerk _inst) {
    /**
     * 更新玩家的永久属性，通常在重生时调用。
     */
}

public void BasePerk_updateCondStats(BasePerk _inst) {
    /**
     * 周期性（通常每秒）更新玩家的状态、冷却时间和HUD。
     */
}

public void BasePerk_doItemThrow(BasePerk _inst, char[] model, float force, any color) {
    /**
     * 执行投掷物品的逻辑。
     */
}

