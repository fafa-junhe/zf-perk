#pragma newdecls required

#if defined _ZF_BASE_PERK_INC
  #endinput
#endif
#define _ZF_BASE_PERK_INC

#include <sourcemod>
#include <sdktools>
#include <tf2>
#include <tf2_stocks>
#include "zf_util_base.inc" // 包含一些基础工具函数

/**
 * 所有Perk的基类.
 * 使用 methodmap 模拟一个包含数据和方法的对象.
 * 它继承自 StringMap, 用来存储该Perk实例的所有状态数据.
 */
methodmap base_perk < StringMap
{
    /**
     * 构造函数.
     * 当创建一个新的Perk实例时调用.
     * @param owner   拥有此Perk的玩家的客户端ID.
     */
    public base_perk(int owner)
    {
        StringMap sm = new StringMap();
        sm.SetValue("owner", owner);

        // 初始化所有统计数据的默认值
        // 分为永久(Perm), 条件(Cond), 和临时(Temp)三类
        char key[64];
        for (int i = 0; i < view_as<int>(ZFStat_Count); i++)
        {
            Format(key, sizeof(key), "stat_%d_perm", i);
            sm.SetValue(key, 0);
            Format(key, sizeof(key), "stat_%d_cond", i);
            sm.SetValue(key, 0);
            Format(key, sizeof(key), "stat_%d_temp", i);
            sm.SetValue(key, 0);
            Format(key, sizeof(key), "stat_%d_temp_duration", i);
            sm.SetValue(key, 0);
        }

        return view_as<base_perk>(sm);
    }

    /**
     * 析构函数 (概念上).
     * 在 'delete perk' 时调用，用于清理特效、计时器等资源.
     */
    public void onRemove()
    {
        // 默认实现为空. 子类应重写此函数来清理自己创建的资源.
    }

    // --- 核心属性 ---

    public int getOwner()
    {
        int owner;
        this.GetValue("owner", owner);
        return owner;
    }

    // --- 统计数据获取 ---

    /**
     * 获取一个统计项的总值 (永久 + 条件 + 临时).
     * @param stat    要获取的统计项 (ZFStat enum).
     * @return        统计项的总值.
     */
    public int getStat(ZFStat stat)
    {
        char key[64];
        int total = 0;
        int value;

        Format(key, sizeof(key), "stat_%d_perm", stat);
        this.GetValue(key, value);
        total += value;

        Format(key, sizeof(key), "stat_%d_cond", stat);
        this.GetValue(key, value);
        total += value;

        Format(key, sizeof(key), "stat_%d_temp", stat);
        this.GetValue(key, value);
        total += value;

        return total;
    }

    // --- 事件处理函数 (虚拟函数) ---
    // 这些函数由核心插件在特定游戏事件发生时调用.
    // 每个具体的Perk类都需要重写这些函数来实现自己的逻辑.

    /**
     * 当玩家重生时调用.
     */
    public void onSpawn() {}

    /**
     * 当玩家受到伤害时调用 (伤害计算前).
     */
    public void onTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {}

    /**
     * 当玩家受到伤害时调用 (伤害计算后).
     */
    public void onTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype) {}

    /**
     * 当玩家死亡时调用.
     */
    public void onDeath(int victim, int killer, int assist) {}

    /**
     * 当玩家呼叫医生时调用.
     * @return Plugin_Continue or Plugin_Handled
     */
    public Action onCallForMedic()
    {
        return Plugin_Continue;
    }

    /**
     * 每秒调用的周期性更新函数.
     * 用于处理需要持续更新的逻辑, 例如光环效果、计时器等.
     */
    public void onPeriodicUpdate()
    {
        // 基类处理临时属性的持续时间递减
        char key_temp[64], key_dur[64];
        int duration;
        for (int i = 0; i < view_as<int>(ZFStat_Count); i++)
        {
            Format(key_dur, sizeof(key_dur), "stat_%d_temp_duration", i);
            this.GetValue(key_dur, duration);
            if (duration > 0)
            {
                duration--;
                this.SetValue(key_dur, duration);
                if (duration == 0)
                {
                    Format(key_temp, sizeof(key_temp), "stat_%d_temp", i);
                    this.SetValue(key_temp, 0);
                }
            }
        }
    }

    /**
     * 在每个游戏帧上调用.
     * 用于处理需要高频率更新的逻辑.
     */
    public void onGameFrame() {}
}