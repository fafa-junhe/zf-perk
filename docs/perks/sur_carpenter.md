# 幸存者职业: 木工 (Carpenter)

## 描述

**简短描述:**
> 木工(Carpenter)——建造障碍物

**详细描述:**
> 你有防御力加成,但是你的攻击力减弱. 
> 发医生语音来建造一个500点生命值的路障,敌我均可破坏. 
> 冷却时间25秒.同时最多拥有4个路障. 
> 
> “此路不通。” 
> 推荐职业: 火焰兵、工程师、医生

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_CARPENTER` | 2 | Perk的ID |
| `CARPENTER_ATTACK` | -40 | 攻击力惩罚 (-40%) |
| `CARPENTER_DEFEND` | 25 | 防御力加成 (+25%) |
| `CARPENTER_BARRICADE_HEALTH` | 500 | 每个路障的生命值 |
| `CARPENTER_COOLDOWN` | 25 | 放置路障后的冷却时间 (秒) |
| `CARPENTER_MAX_ITEMS` | 4 | 最多可同时存在的路障数量 |
| `CARPENTER_DROP_RADSQ_BARRICADE` | 250*250 | 放置路障时，附近不能有其他路障的半径（平方） |
| `CARPENTER_DROP_RADSQ_CLIENT` | 150*150 | 放置路障时，附近不能有其他玩家的半径（平方） |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了木工职业 (`usingSurPerk(client, ZF_PERK_CARPENTER)`), 则会应用以下永久性的属性修改：
        *   攻击力 (`ZFStatAtt`) 减少 `CARPENTER_ATTACK` (-40)。
        *   防御力 (`ZFStatDef`) 增加 `CARPENTER_DEFEND` (25)。

2.  **建造路障**:
    *   在 `perk_OnCallForMedic` 函数中，当木工玩家呼叫医生时，会触发建造逻辑。
    *   **条件检查**:
        *   冷却时间 (`zf_perkTimer[client]`) 必须为0。
        *   玩家必须在地面上 (`isGrounded`) 并且处于蹲伏状态 (`isCrouching`)。
        *   检查附近是否有其他玩家 (`clientsNearby`) 或其他路障 (`barricadesNearby`)，以避免重叠。
    *   **建造过程**:
        *   如果满足所有条件，则调用 `doCarpenterBuild` 函数。
        *   该函数会在玩家面前创建一个模型为 `ZFMDL_FENCE` 的路障实体。
        *   路障的生命值通过 `setItemMetadata` 设置为 `CARPENTER_BARRICADE_HEALTH` (500)。
        *   为路障实体挂载 `perk_OnFenceTakeDamage` 钩子，用于处理伤害。
        *   重置冷却时间为 `CARPENTER_COOLDOWN` (25秒)。

## 事件处理

*   **`perk_OnCallForMedic`**: 玩家呼叫医生时，触发建造路障的逻辑。
*   **`perk_OnFenceTakeDamage`**: 当路障实体受到伤害时触发。
    *   减少路障的生命值。
    *   如果生命值低于或等于0，则摧毁路障并播放音效和特效。
    *   如果生命值大于0，则更新路障的渲染颜色以显示其损坏程度。
*   **`updateCondStats`**: 每秒更新一次。
    *   减少冷却时间计时器 `zf_perkTimer`。
    *   更新HUD信息，显示路障是否准备就绪或已达数量上限。
