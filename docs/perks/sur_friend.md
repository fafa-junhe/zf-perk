# 幸存者职业: 伙计 (Friend)

## 描述

**简短描述:**
> 伙计(Friend)——和伙伴获得加成

**详细描述:**
> 在准备时间内,对着目标发医生语音,可将目标设为伙伴.(未选择则由系统产生) 
> 靠近同伴时,你获得攻击力和生命回复加成. 
> 当你和同伴击杀或助攻时,你的暴击时间就会增加. 
> 当同伴死亡后,你就会获得对应时间的暴击.
> 
> “两人搭配,干活不累!” 
> 推荐职业: 任何

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_FRIEND` | 5 | Perk的ID |
| `ZF_FRIEND_ATTACK` | 25 | 靠近伙伴时的攻击力加成 (+25%) |
| `ZF_FRIEND_REGEN` | 10 | 靠近伙伴时的生命恢复 (每秒) |
| `ZF_FRIEND_CRITTIME_INIT` | 0 | 初始暴击时间 (秒) |
| `ZF_FRIEND_CRITTIME_KILL` | 4 | 每次击杀增加的暴击时间 (秒) |
| `ZF_FRIEND_CRITTIME_ASSIST` | 2 | 每次助攻增加的暴击时间 (秒) |
| `ZF_FRIEND_RADIUSSQ` | 300*300 | 伙伴加成生效的半径（平方） |

## 核心逻辑

1.  **选择伙伴**:
    *   在 `perk_OnCallForMedic` 函数中，如果玩家在准备阶段 (`roundState() <= RoundGrace`) 对着另一名幸存者呼叫医生，该幸存者将被选为伙伴。
    *   在 `perk_OnGraceEnd` 函数中，如果玩家没有手动选择伙伴，系统会通过 `doFriendSelect` 函数随机选择一名活着的幸存者作为伙伴。
    *   伙伴关系存储在 `zf_perkState[client]` 中，暴击时间存储在 `zf_perkTimer[client]` 中。
    *   选择伙伴后，会在伙伴头上创建一个 `ZFSPR_DOMINATED` 图标，仅自己可见。

2.  **伙伴加成**:
    *   在 `updateCondStats` 函数中，每秒检查玩家和伙伴的状态。
    *   如果伙伴存活 (`validLivingSur(zf_perkState[thisSur])`) 并且两者距离在 `ZF_FRIEND_RADIUSSQ` 内，则双方都会获得：
        *   `ZF_FRIEND_ATTACK` 的攻击力加成。
        *   `ZF_FRIEND_REGEN` 的生命恢复。

3.  **暴击时间积累**:
    *   在 `perk_OnPlayerDeath` 函数中，当一个僵尸被击杀时：
        *   如果击杀者是“伙计”玩家，并且其伙伴是助攻者，则“伙计”玩家的暴击时间 (`zf_perkTimer`) 增加 `ZF_FRIEND_CRITTIME_KILL`。
        *   如果助攻者是“伙计”玩家，并且其伙伴是击杀者，则“伙计”玩家的暴击时间 (`zf_perkTimer`) 增加 `ZF_FRIEND_CRITTIME_ASSIST`。

4.  **伙伴阵亡**:
    *   在 `updateCondStats` 函数中，如果检测到伙伴已死亡 (`!validLivingSur(zf_perkState[thisSur])`)：
        *   玩家会立即获得 `zf_perkTimer` 中积累的所有暴击时间。
        *   触发 `fxKritzStart` 特效，并创建一个红色的光环 `ZFPART_AURAOUTRED`。
        *   暴击效果会持续消耗 `zf_perkTimer`，直到为0。

## 事件处理

*   **`perk_OnCallForMedic`**: 在准备阶段用于手动选择伙伴。
*   **`perk_OnGraceEnd`**: 如果没有手动选择，则自动分配一个随机伙伴。
*   **`perk_OnPlayerDeath`**: 当玩家或其伙伴击杀/助攻时，积累暴击时间。
*   **`updateCondStats`**:
    *   检查伙伴距离并提供光环加成。
    *   检查伙伴是否存活，如果阵亡则激活暴击。
    *   管理和消耗暴击时间。
    *   更新HUD显示暴击时间。
*   **`perk_OnSetTransmit`**: 控制伙伴头上的图标只对“伙计”玩家本人可见。
