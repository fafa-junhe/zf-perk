# 幸存者职业: 慈善家 (Charitable)

## 描述

**简短描述:**
> 慈善家(Charitable)——击杀换取礼物

**详细描述:**
> 你的每个击杀和助攻都会增加礼物点数. 
> 发医生语音来消耗礼物点数,放出礼物,可被其他的幸存者捡起. 
> 礼物能给予生命值回复和短暂的攻击力加成. 
> 
> “人人都有礼物拿!” 
> 推荐职业: 士兵、爆破手、狙击手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_CHARITABLE` | 3 | Perk的ID |
| `ZF_CHARITABLE_MAX_ITEMS` | 5 | 最多可同时存在的礼物数量 |
| `ZF_CHARITABLE_POINTS_ASSIST` | 2 | 每个助攻获得的点数 |
| `ZF_CHARITABLE_POINTS_KILL` | 2 | 每个击杀获得的点数 |
| `ZF_CHARITABLE_POINTS_GIFT` | 4 | 扔出一个礼物所需的点数 |
| `ZF_CHARITABLE_GIFT_BONUS_HEALTH` | 75 | 礼物被捡起时，慈善家获得的的生命值 |
| `ZF_CHARITABLE_GIFT_BONUS_MIN` | 10 | 捡起礼物的玩家获得的最小属性加成 |
| `ZF_CHARITABLE_GIFT_BONUS_MAX` | 30 | 捡起礼物的玩家获得的最大属性加成 |
| `ZF_CHARITABLE_GIFT_DURATION` | 20 | 属性加成的持续时间 (秒) |

## 核心逻辑

1.  **点数积累**:
    *   在 `perk_OnPlayerDeath` 函数中，当一个僵尸被击杀时：
        *   如果慈善家是击杀者 (`killer`)，其 `zf_perkState` (礼物点数) 增加 `ZF_CHARITABLE_POINTS_KILL` (2)。
        *   如果慈善家是助攻者 (`assist`)，其 `zf_perkState` 增加 `ZF_CHARITABLE_POINTS_ASSIST` (2)。

2.  **扔出礼物**:
    *   在 `perk_OnCallForMedic` 函数中，当慈善家玩家呼叫医生时，会触发扔礼物逻辑。
    *   **条件检查**:
        *   玩家拥有的礼物点数 (`zf_perkState`) 必须足够兑换一个礼物 (`>= ZF_CHARITABLE_POINTS_GIFT`)。
    *   **扔出过程**:
        *   如果满足条件，则调用 `doItemThrow` 函数，从玩家的视角扔出一个模型为 `ZFMDL_PRESENT` 的礼物实体。
        *   消耗 `ZF_CHARITABLE_POINTS_GIFT` (4) 点数。
        *   为礼物实体挂载 `perk_OnCharitableGiftTouched` 钩子，用于处理拾取事件。

## 事件处理

*   **`perk_OnPlayerDeath`**: 当僵尸死亡时，为击杀或助攻的慈善家增加礼物点数。
*   **`perk_OnCallForMedic`**: 玩家呼叫医生时，如果点数足够，则消耗点数并扔出礼物。
*   **`perk_OnCharitableGiftTouched`**: 当礼物实体被其他玩家接触时触发。
    *   **条件检查**: 拾取者不能是扔出礼物的慈善家本人。
    *   **效果**:
        *   **对拾取者**: 随机选择一项属性 (`ZFStat`)，并在 `ZF_CHARITABLE_GIFT_BONUS_MIN` 和 `ZF_CHARITABLE_GIFT_BONUS_MAX` 之间随机一个加成值，通过 `addStatTempStack` 给予一个持续 `ZF_CHARITABLE_GIFT_DURATION` 秒的临时属性加成。
        *   **对慈善家**: 给予 `ZF_CHARITABLE_GIFT_BONUS_HEALTH` (75) 点生命值回复。
    *   **清理**: 礼物实体被销毁。
*   **`updateCondStats`**: 每秒更新一次，在HUD上显示玩家当前拥有的礼物数量。
