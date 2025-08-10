// Generated from D:\workspace\code\tf2\zf\src\pythons\perks\BasePerk.py

#if defined __BasePerk_included
#endinput
#endif
#define __BasePerk_included

#include <datapack>
#include "../perk_vtable.inc"
#include "../perk_macros.inc"
#include "../zf_perk.inc"

methodmap BasePerk < DataPack {
    public BasePerk(int client) {
        DataPack sm_base = new DataPack();
        for (int i = 0; i < 64; i++) {
            sm_base.WriteCell(0);
        }
        sm_base.Reset();
        BasePerk sm = view_as<BasePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, BasePerk_getName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, BasePerk_getShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, BasePerk_getDesc);
        sm.Position = view_as<DataPackPos>(PERK_DATA_CLIENT);
        sm.WriteCell(client);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, BasePerk_onPlayerRunCmd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_AMMO_PICKUP, BasePerk_onAmmoPickup);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALC_IS_ATTACK_CRITICAL, BasePerk_onCalcIsAttackCritical);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, BasePerk_onCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, BasePerk_onGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GRACE_END, BasePerk_onGraceEnd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_MED_PICKUP, BasePerk_onMedPickup);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, BasePerk_onPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, BasePerk_onDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, BasePerk_onKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, BasePerk_onAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, BasePerk_onPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, BasePerk_onRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_SET_TRANSMIT, BasePerk_onSetTransmit);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, BasePerk_onTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, BasePerk_onTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TOUCH, BasePerk_onTouch);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, BasePerk_updateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, BasePerk_updateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_DO_ITEM_THROW, BasePerk_doItemThrow);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, BasePerk_onDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_BUILDING_TAKE_DAMAGE, BasePerk_onBuildingTakeDamage);

        return sm;
    }

    public void getName(char[] buffer, int maxlen) {
        PERK_IMPL_GET_STRING(VTABLE_GET_NAME, buffer, maxlen);
    }

    public void getShortdesc(char[] buffer, int maxlen) {
        PERK_IMPL_GET_STRING(VTABLE_GET_SHORT_DESC, buffer, maxlen);
    }

    public void getDesc(char[] buffer, int maxlen) {
        PERK_IMPL_GET_STRING(VTABLE_GET_LONG_DESC, buffer, maxlen);
    }

    property int client {
        public get() {
            this.Position = view_as<DataPackPos>(PERK_DATA_CLIENT);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(PERK_DATA_CLIENT);
            this.WriteCell(value);
        }
    }

    public void onPlayerRunCmd(int buttons, int impulse, float vel[3], float angles[3], int weapon) {
        PERK_IMPL_onPlayerRunCmd(buttons, impulse, vel, angles, weapon);
    }

    public void onAmmoPickup(int pickup) {
        PERK_IMPL_EVENT_CELL(VTABLE_ON_AMMO_PICKUP, pickup);
    }

    public void onCalcIsAttackCritical() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_CALC_IS_ATTACK_CRITICAL);
    }

    public void onCallForMedic() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_CALL_FOR_MEDIC);
    }

    public void onGameFrame() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_GAME_FRAME);
    }

    public void onGraceEnd() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_GRACE_END);
    }

    public void onMedPickup(int pickup) {
        PERK_IMPL_EVENT_CELL(VTABLE_ON_MED_PICKUP, pickup);
    }

    public void onPeriodic() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_PERIODIC);
    }

    public void onDeath(int victim, int killer, int assist, int inflictor, int damagetype) {
        PERK_IMPL_onDeath(victim, killer, assist, inflictor, damagetype);
    }

    public void onKill(int victim, int killer, int assist, int inflictor, int damagetype) {
        PERK_IMPL_onKill(victim, killer, assist, inflictor, damagetype);
    }

    public void onAssistKill(int victim, int killer, int assist, int inflictor, int damagetype) {
        PERK_IMPL_onAssistKill(victim, killer, assist, inflictor, damagetype);
    }

    public void onPlayerSpawn() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_PLAYER_SPAWN);
    }

    public void onRemove() {
        PERK_IMPL_EVENT_VOID(VTABLE_ON_REMOVE);
    }

    public void onSetTransmit(int entity, int client) {
        PERK_IMPL_EVENT_CELL_CELL(VTABLE_ON_SET_TRANSMIT, entity, client);
    }

    public float onTakeDamage(int victim, int attacker, int inflictor, float damage, int damagetype) {
        PERK_IMPL_onTakeDamage(victim, attacker, inflictor, damage, damagetype);
    }

    public void onTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype) {
        PERK_IMPL_onTakeDamagePost(victim, attacker, inflictor, damage, damagetype);
    }

    public void onTouch(int toucher, int touchee) {
        PERK_IMPL_EVENT_CELL_CELL(VTABLE_ON_TOUCH, toucher, touchee);
    }

    public void updateClientPermStats() {
        PERK_IMPL_EVENT_VOID(VTABLE_UPDATE_CLIENT_PERM_STATS);
    }

    public void updateCondStats(char[] perk_str, int maxlen_perk_str) {
        PERK_IMPL_updateCondStats(perk_str, maxlen_perk_str);
    }

    public void doItemThrow(char[] model, float force, float color[3]) {
        PERK_IMPL_doItemThrow(model, force, color);
    }

    public void onDealDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype) {
        PERK_IMPL_onDealDamagePost(victim, attacker, inflictor, damage, damagetype);
    }

    public void onBuildingTakeDamage(int iBuilding, int &iAttacker, int &iInflictor, float &flDamage, int &iDamagetype, int &iWeapon, float flDamageForce[3], float vecDamagePosition[3]) {
        PERK_IMPL_onBuildingTakeDamage(iBuilding, iAttacker, iInflictor, flDamage, iDamagetype, iWeapon, flDamageForce, vecDamagePosition);
    }
}

stock BasePerk BasePerk_new(int client) {
    return new BasePerk(client);
}


public void BasePerk_getName(BasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

public void BasePerk_getShortdesc(BasePerk _inst, char[] buffer, int maxlen) {
    char name[64];
    _inst.getName(name, sizeof(name));
    if (StrEqual(name, "Unselected"))
    {
        strcopy(buffer, maxlen, "ZF_WithoutPerks");
    }
    else
    {
        Format(buffer, maxlen, "%sPerk_shortdesc", name);
    }
}

public void BasePerk_getDesc(BasePerk _inst, char[] buffer, int maxlen) {
    char name[64];
    _inst.getName(name, sizeof(name));
    if (StrEqual(name, "Unselected"))
    {
        strcopy(buffer, maxlen, "ZF_NoPerk_desc");
    }
    else
    {
        Format(buffer, maxlen, "%sPerk_desc", name);
    }
}

public void BasePerk_onPlayerRunCmd(BasePerk _inst, int buttons, int impulse, float vel[3], float angles[3], int weapon) {
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

public void BasePerk_onDeath(BasePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    /**
     * 当拥有此技能的玩家死亡时触发。
     */
}

public void BasePerk_onKill(BasePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    /**
     * 当此职业的玩家造成击杀时触发。
     */
}

public void BasePerk_onAssistKill(BasePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    /**
     * 当此职业的玩家造成助攻时触发。
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

public float BasePerk_onTakeDamage(BasePerk _inst, int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 在伤害事件发生时（伤害计算前）触发。
     */
    return damage;
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

public void BasePerk_updateCondStats(BasePerk _inst, char[] perk_str, int maxlen_perk_str) {
    /**
     * 周期性（通常每秒）更新玩家的状态、冷却时间和HUD。
     */
}

public void BasePerk_doItemThrow(BasePerk _inst, char[] model, float force, float color[3]) {
    /**
     * 执行投掷物品的逻辑。
     */
}

public void BasePerk_onDealDamagePost(BasePerk _inst, int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 在伤害事件发生后（伤害计算后）触发。
     */
}

public void BasePerk_onBuildingTakeDamage(BasePerk _inst, int iBuilding, int iAttacker, int iInflictor, float flDamage, int iDamagetype, int iWeapon, float flDamageForce[3], float vecDamagePosition[3]) {
    /**
     * 当玩家的建筑受到伤害时触发。
     */
}
