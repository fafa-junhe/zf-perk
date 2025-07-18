# 幸存者职业: 英雄 (Heroic)

## 描述

**简短描述:**
> 英雄(Heroic)——活到最后获得暴击

**详细描述:**
> 你有攻击力和防御力加成. 
> 你的每个击杀和助攻都会增加你的暴击时间. 
> 当你是最后一个幸存者时,你就会获得对应时间的暴击. 
> 
> “晚安,好运.” 
> 推荐职业: 士兵、火焰兵、爆破手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HEROIC` | 6 | Perk的ID |
| `HEROIC_COMBAT` | 15 | 永久的攻击力和防御力加成 (+15%) |
| `HEROIC_CRITTIME_INIT` | 30 | 初始暴击时间 (秒) |
| `HEROIC_CRITTIME_KILL` | 3 | 每次击杀增加的暴击时间 (秒) |
| `HEROIC_CRITTIME_KILL_ACTIVE` | 0 | 暴击激活状态下，每次击杀增加的暴击时间 (秒) |
| `HEROIC_CRITTIME_ASSIST` | 1 | 每次助攻增加的暴击时间 (秒) |
| `HEROIC_CRITTIME_ASSIST_ACTIVE` | 0 | 暴击激活状态下，每次助攻增加的暴击时间 (秒) |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了英雄职业 (`usingSurPerk(client, ZF_PERK_HEROIC)`), 则会应用以下永久性的属性修改：
        *   攻击力 (`ZFStatAtt`) 增加 `HEROIC_COMBAT` (15)。
        *   防御力 (`ZFStatDef`) 增加 `HEROIC_COMBAT` (15)。
    *   同时，玩家的暴击时间 (`zf_perkTimer`) 会被初始化为 `HEROIC_CRITTIME_INIT` (30)。

2.  **暴击时间积累**:
    *   在 `perk_OnPlayerDeath` 函数中，当一个僵尸被英雄玩家击杀或助攻时：
        *   如果暴击状态未激活 (`zf_perkState[killer] == 0`)：
            *   击杀增加 `HEROIC_CRITTIME_KILL` (3) 秒暴击时间。
            *   助攻增加 `HEROIC_CRITTIME_ASSIST` (1) 秒暴击时间。
        *   如果暴击状态已激活 (`zf_perkState[killer] == 1`)：
            *   击杀增加 `HEROIC_CRITTIME_KILL_ACTIVE` (0) 秒暴击时间。
            *   助攻增加 `HEROIC_CRITTIME_ASSIST_ACTIVE` (0) 秒暴击时间。
        *   这意味着一旦成为最后的幸存者，将无法再通过击杀或助攻来增加暴击时间。

3.  **最后生还者激活**:
    *   在 `updateCondStats` 函数中，每秒检查当前幸存者数量 (`validSurCount`)。
    *   如果幸存者数量为1，并且该幸存者是英雄玩家，则会激活暴击状态 (`zf_perkState[thisSur] = 1`)。
    *   激活时，玩家会获得 `zf_perkTimer` 中积累的所有暴击时间，并触发 `fxKritzStart` 特效和红色光环。
    *   暴击效果会持续消耗 `zf_perkTimer`，直到为0，届时特效和光环也会消失。

## 事件处理

*   **`updateClientPermStats`**: 在玩家重生时，给予永久的攻防加成并初始化暴击时间。
*   **`perk_OnPlayerDeath`**: 当玩家击杀或助攻僵尸时，根据暴击是否激活来积累暴击时间。
*   **`updateCondStats`**:
    *   检查是否为最后生还者，如果是则激活暴击状态。
    *   管理和消耗已激活的暴击时间。
    *   更新HUD，显示当前积累的暴击时间。
