# 僵尸职业: 零号僵尸 (Alpha)

## 描述

**简短描述:**
> 零号僵尸(Alpha)——召唤僵尸随从

**详细描述:**
> 你能通过击杀幸存者或为随从助攻,使得死去的人类成为自己的随从. 
> 附近的每个僵尸和随从都能让你获得生命恢复和攻击加成. 
> 发医生语音可以召唤最多5个随从到身边,冷却时间15秒. 
> 
> “来自黑暗寒冬的仆人们、士兵们!听从我的召唤!” 
> 推荐职业: 侦察兵、机枪手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_ALPHA` | 1 | Perk的ID |
| `ZF_ALPHA_RADIUSSQ` | (500 * 500) | 提供攻击和恢复加成的光环半径（平方） |
| `ZF_ALPHA_ATTACK` | 5 | 每个附近的非随从僵尸提供的攻击加成 |
| `ZF_ALPHA_ATTACK_MINION` | 10 | 每个附近的随从僵尸提供的攻击加成 |
| `ZF_ALPHA_REGEN` | 4 | 每个附近的非随从僵尸提供的生命恢复加成 |
| `ZF_ALPHA_REGEN_MINION` | 12 | 每个附近的随从僵尸提供的生命恢复加成 |
| `ZF_ALPHA_SUMMON_LIMIT` | 5 | 一次可以召唤的最大随从数量 |
| `ZF_ALPHA_TIMER_MINION` | 15 | 召唤技能的冷却时间（秒） |

## 核心逻辑

1.  **光环效果**:
    *   在 `updateClientPermEffects` 函数中，如果玩家是零号僵尸，会创建一个蓝色的光环 (`ZFPART_AURAINBLU`)。

2.  **属性加成**:
    *   在 `updateCondStats` 函数中，会周期性地计算零号僵尸的属性加成：
        *   根据 `ZF_ALPHA_RADIUSSQ` 范围内存在的普通僵尸和随从僵尸数量，分别提供攻击力 (`ZF_ALPHA_ATTACK`, `ZF_ALPHA_ATTACK_MINION`) 和生命恢复 (`ZF_ALPHA_REGEN`, `ZF_ALPHA_REGEN_MINION`) 加成。

3.  **随从转化**:
    *   在 `perk_OnPlayerDeath` 事件中，当一个幸存者被零号僵尸击杀或助攻击杀时，该幸存者会在重生后成为零号僵尸的随从 (`zf_perkAlphaMaster[victim] = killer`)。

## 事件处理

*   **`perk_OnCallForMedic` (玩家按“E”键)**:
    *   如果玩家是零号僵尸，并且技能不在冷却中，会触发 `doAlphaSummon` 函数。
    *   `doAlphaSummon` 会将最多 `ZF_ALPHA_SUMMON_LIMIT` 个随从传送到零号僵尸身边。
    *   技能进入冷却，冷却时间为 `ZF_ALPHA_TIMER_MINION` 秒。

*   **`updateCondStats` (周期性更新)**:
    *   处理召唤技能的冷却倒计时。
    *   更新HUD，显示技能是否准备就绪。