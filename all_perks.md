# 幸存者职业: 运动员 (Athletic)

## 描述

**简短描述:**
> 运动员(Athletic)——高移动力

**详细描述:**
> 你的移动力和攻击速度大幅增加. 
> 但是你的攻击力降低,而且无法造成随机暴击. 
> 
> “嘿!你们根本追不上我!” 
> 推荐职业: 火焰兵、爆破骑士、医生

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_ATHLETIC` | 1 | Perk的ID |
| `ZF_ATHLETIC_ATTACK` | -40 | 攻击力惩罚 (-40%) |
| `ZF_ATHLETIC_CRIT` | -100 | 暴击率惩罚 (-100%) |
| `ZF_ATHLETIC_ROF` | 100 | 攻击速度加成 (+100%) |
| `ZF_ATHLETIC_SPEED` | 100 | 移动速度加成 (+100%) |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了运动员职业 (`usingSurPerk(client, ZF_PERK_ATHLETIC)`), 则会应用以下永久性的属性修改：
        *   攻击力 (`ZFStatAtt`) 减少 `ZF_ATHLETIC_ATTACK` (-40)。
        *   暴击率 (`ZFStatCrit`) 减少 `ZF_ATHLETIC_CRIT` (-100)，即无法暴击。
        *   攻击速度 (`ZFStatRof`) 增加 `ZF_ATHLETIC_ROF` (100)。
        *   移动速度 (`ZFStatSpeed`) 增加 `ZF_ATHLETIC_SPEED` (100)。

## 事件处理

该职业没有特定的事件处理逻辑，其功能完全通过永久性的属性修改实现。
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
# 幸存者职业: 懦夫 (Cowardly)

## 描述

**简短描述:**
> 懦夫(Cowardly)——随时准备逃跑

**详细描述:**
> 当你被攻击时,自动激活你的被动能力“恐慌”.  
> 恐慌会给你防御力和速度加成. 
> 恐慌能力持续时间5秒,冷却时间30秒. 
> 
> “别守着了,快逃命吧!” 
> 推荐职业: 工程师、医生、狙击手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_COWARDLY` | 4 | Perk的ID |
| `ZF_COWARDLY_DEFEND` | 50 | 恐慌状态下的防御力加成 (+50%) |
| `ZF_COWARDLY_SPEED` | 200 | 恐慌状态下的速度加成 (+200%) |
| `ZF_COWARDLY_DURATION_SCARED` | 5 | 恐慌状态的持续时间 (秒) |
| `ZF_COWARDLY_DURATION_COOLDOWN` | 30 | 恐慌能力的冷却时间 (秒) |

## 核心逻辑

1.  **触发恐慌**:
    *   在 `perk_OnTakeDamage` 函数中，当懦夫玩家被僵尸的**近战攻击**击中时，会触发恐慌逻辑。
    *   **条件检查**:
        *   玩家的 `zf_perkTimer` 必须为0，表示技能不在冷却中。
    *   **激活**:
        *   如果满足条件，`zf_perkTimer` 会被设置为 `ZF_COWARDLY_DURATION_SCARED + ZF_COWARDLY_DURATION_COOLDOWN` (5 + 30 = 35秒)。
        *   立即给予玩家 `ZF_COWARDLY_DEFEND` 和 `ZF_COWARDLY_SPEED` 的属性加成。
        *   播放 `fxYikes` 特效并提示玩家。
        *   该次攻击的伤害将被完全免疫 (`damage = 0.0`)。

2.  **恐慌状态管理**:
    *   在 `updateCondStats` 函数中，每秒对 `zf_perkTimer` 进行更新。
    *   **恐慌期间** (`zf_perkTimer > ZF_COWARDLY_DURATION_COOLDOWN`): 玩家会持续获得防御和速度加成。
    *   **恐慌结束** (`zf_perkTimer == ZF_COWARDLY_DURATION_COOLDOWN`): 提示玩家恐慌状态结束，此时属性加成消失。
    *   **冷却结束** (`zf_perkTimer == 0`): 提示玩家技能准备就绪。

## 事件处理

*   **`perk_OnTakeDamage`**: 当被僵尸近战攻击时，如果技能未在冷却，则触发“恐慌”状态，免疫当次伤害，并进入持续5秒的增益状态和30秒的冷却期。
*   **`updateCondStats`**: 每秒更新一次。
    *   管理恐慌状态的计时器。
    *   在恐慌期间提供属性加成。
    *   在状态变化时向玩家发送提示信息。
    *   更新HUD，显示技能是否准备就绪。
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
# 幸存者职业: 牧师 (Holy)

## 描述

**简短描述:**
> 牧师(Holy)——蹲着治疗幸存者

**详细描述:**
> 你的攻击力降低. 
> 当蹲下不动时,你可以治疗自己和周围的幸存者. 
> 
> “圣光赐予我胜利!” 
> 推荐职业: 缺少医生的队伍

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HOLY` | 7 | Perk的ID |
| `ZF_HOLY_ATTACK` | -25 | 攻击力惩罚 (-25%) |
| `ZF_HOLY_RADIUSSQ` | 400*400 | 治疗光环的半径（平方） |
| `ZF_HOLY_REGEN` | 10 | 每秒为范围内的幸存者恢复的生命值 |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `perk_OnPlayerSpawn` (通过 `updateClientPermStats` 间接调用) 中，如果玩家选择了牧师职业，会应用 `ZF_HOLY_ATTACK` 的攻击力惩罚。但是，代码中 `updateClientPermStats` 函数并没有为牧师添加这个永久惩罚，这可能是一个BUG或者被遗漏的实现。根据代码注释和职业描述，这里应该是有一个攻击力惩罚的。

2.  **治疗光环**:
    *   在 `updateCondStats` 函数中，每秒检查牧师玩家的状态。
    *   **条件检查**:
        *   玩家必须在地面上 (`isGrounded`)。
        *   玩家必须处于蹲伏状态 (`isCrouching`)。
        *   玩家必须没有移动 (`isNotMoving`)。
    *   **治疗效果**:
        *   如果满足所有条件，治疗光环被激活。
        *   光环会对自身以及在 `ZF_HOLY_RADIUSSQ` 范围内的所有其他幸存者，每秒恢复 `ZF_HOLY_REGEN` (10) 点生命值。
        *   同时，会显示一个 `ZFPART_AURAGLOWBEAMS` 的光环特效。
    *   如果不满足条件，光环特效会被隐藏。

## 事件处理

*   **`updateClientPermEffects`**: 在玩家重生时，为牧师玩家创建一个 `ZFPART_AURAGLOWBEAMS` 的光环特效实体，但默认是隐藏的。
*   **`updateCondStats`**:
    *   核心的治疗逻辑所在地。
    *   根据玩家是否“蹲下不动”来决定是否激活治疗光环并显示/隐藏特效。
    *   对范围内的所有幸存者（包括自己）进行治疗。
# 幸存者职业：主宰者 (Juggernaut)

## 介绍

**名称**: 主宰者 (Juggernaut)

**简短描述**:
> 主宰者(Juggernaut)——攻击力高,速度慢

**详细描述**:
> 你的攻击力大幅提高,但你的防御力和移动速度降低.
> 你免疫掉落伤害.
> 如果你落在僵尸附近,僵尸会受到轻微的伤害,并产生击退和击晕效果.
> 
> “(译者不会玩MOAB游戏)”
> 推荐职业: 士兵、爆破手、狙击手

## 参数

| 参数名                          | 默认值 | 备注                                                               |
| ------------------------------- | ------ | ------------------------------------------------------------------ |
| `ZF_JUGGERNAUT_ATTACK`          | 50     | 攻击力加成                                                         |
| `ZF_JUGGERNAUT_DEFEND`          | -50    | 防御力惩罚                                                         |
| `ZF_JUGGERNAUT_SPEED`           | -100   | 移动速度惩罚                                                       |
| `ZF_JUGGERNAUT_FORCE`           | 500.0  | 坠落或近战攻击时对附近僵尸的击退力量                               |
| `ZF_JUGGERNAUT_RADIUS`          | 150    | 坠落伤害造成眩晕的半径                                             |
| `ZF_JUGGERNAUT_STUN_DURATION`   | 1.0    | 近战攻击或坠落伤害造成的眩晕持续时间                               |
| `ZF_JUGGERNAUT_STUN_SLOWDOWN`   | 1.0    | 近战攻击或坠落伤害造成的眩晕减速效果                               |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   攻击力 (`ZFStatAtt`) 增加 `ZF_JUGGERNAUT_ATTACK` (50)
    -   防御力 (`ZFStatDef`) 增加 `ZF_JUGGERNAUT_DEFEND` (-50)
    -   移动速度 (`ZFStatSpeed`) 增加 `ZF_JUGGERNAUT_SPEED` (-100)

### `perk_OnTakeDamage`

-   **对自己**:
    -   免疫自我伤害 (例如火箭跳)。
    -   免疫坠落伤害。
    -   当因坠落而本应受到伤害时，会对半径 `ZF_JUGGERNAUT_RADIUS` (150) 内的僵尸造成1点伤害，并触发一次大型烟雾效果 (`fxPuffBig`)。
-   **对僵尸 (近战攻击)**:
    -   当使用近战武器攻击僵尸时，会对僵尸施加一个大小为 `ZF_JUGGERNAUT_FORCE` (500.0) 的击退效果。
-   **对僵尸 (坠落冲击)**:
    -   当主宰者从高处落下，对僵尸造成范围伤害时（通过 `applyDamageRadialAtClient` 触发 `env_explosion` 伤害类型），会对范围内的僵尸造成以下效果：
        -   眩晕 `ZF_JUGGERNAUT_STUN_DURATION` (1.0) 秒。
        -   施加 `ZF_JUGGERNAUT_STUN_SLOWDOWN` (1.0) 的减速。
        -   施加 `ZF_JUGGERNAUT_FORCE` (500.0) 的击退。# 幸存者职业：领袖 (Leader)

## 介绍

**名称**: 领袖 (Leader)

**简短描述**:
> 领袖(Leader)——放置增益旗帜

**详细描述**:
> 你有暴击加成, 也会给附近的幸存者提供攻击力和防御力加成.
> 发医生语音来放置一个旗帜, 靠近你的旗帜的幸存者都会获得攻击力和防御力加成.
> 旗帜持续90秒, 冷却时间150秒.
>
> “我需要重新集结队伍.”
> 推荐职业: 士兵、爆破手、工程师

## 参数

| 参数名                            | 默认值     | 备注                                                       |
| --------------------------------- | ---------- | ---------------------------------------------------------- |
| `ZF_LEADER_SELF_CRIT`             | 15         | 领袖自身的暴击加成                                         |
| `ZF_LEADER_OTHERS_ATTACK`         | 15         | 对附近幸存者的被动攻击力光环加成                           |
| `ZF_LEADER_OTHERS_RADIUSSQ`       | 122500     | 被动光环的半径（350\*350）                                 |
| `ZF_LEADER_RALLY_SELF_ATTACK`     | 5          | 每个在旗帜范围内的幸存者为领袖提供的攻击力加成             |
| `ZF_LEADER_RALLY_SELF_DEFEND`     | 5          | 每个在旗帜范围内的幸存者为领袖提供的防御力加成             |
| `ZF_LEADER_RALLY_OTHERS_ATTACK`   | 15         | 旗帜为范围内的其他幸存者提供的攻击力加成                   |
| `ZF_LEADER_RALLY_OTHERS_DEFEND`   | 15         | 旗帜为范围内的其他幸存者提供的防御力加成                   |
| `ZF_LEADER_RALLY_DURATION`        | 90         | 旗帜的持续时间（秒）                                       |
| `ZF_LEADER_RALLY_COOLDOWN`        | 150        | 放置旗帜的冷却时间（秒）                                   |
| `ZF_LEADER_RALLY_RADIUSSQ`        | 160000     | 旗帜效果的半径（400\*400）                                 |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   暴击率 (`ZFStatCrit`) 增加 `ZF_LEADER_SELF_CRIT` (15)。

### `updateClientPermEffects`

-   为领袖玩家创建一个红色的光环效果 (`ZFPART_AURAINRED`)。

### `updateCondStats`

-   **被动光环**:
    -   周期性检查，为半径 `ZF_LEADER_OTHERS_RADIUSSQ` 内的幸存者（不包括领袖自己）提供 `ZF_LEADER_OTHERS_ATTACK` (15) 的攻击力加成。
-   **旗帜光环**:
    -   如果领袖放置了旗帜 (`zf_item[thisSur][0]` 有效)，则：
        -   为旗帜半径 `ZF_LEADER_RALLY_RADIUSSQ` 内的其他幸存者提供 `ZF_LEADER_RALLY_OTHERS_ATTACK` (15) 的攻击力加成和 `ZF_LEADER_RALLY_OTHERS_DEFEND` (15) 的防御力加成。
        -   根据旗帜范围内的幸存者数量，为领袖自己提供额外的攻击和防御加成（每个幸存者 `ZF_LEADER_RALLY_SELF_ATTACK` 和 `ZF_LEADER_RALLY_SELF_DEFEND`）。
-   **计时器**:
    -   管理旗帜的持续时间和冷却时间。
    -   当旗帜消失或冷却结束时，会向玩家发送提示信息。
    -   HUD会显示旗帜是否准备就绪。

### `perk_OnCallForMedic`

-   当领袖玩家按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上并且处于蹲伏状态。
    -   如果满足条件，则在玩家位置放置一个旗帜模型 (`ZFMDL_FLAG`)。
    -   设置 `ZF_LEADER_RALLY_COOLDOWN` (150) 秒的冷却时间。
    -   旗帜在 `ZF_LEADER_RALLY_DURATION` (90) 秒后消失。# 幸存者职业：忍者 (Ninja)

## 介绍

**名称**: 忍者 (Ninja)

**简短描述**:
> 忍者(Ninja)——放置撤退用诱饵

**详细描述**:
> 你的移动力大幅增加, 但是你的攻击力降低.
> 发医生语言来放置一个撤退点.当你受到攻击时, 你会被传送到撤退点.
> 撤退点持续15秒,撤退点冷却时间30秒.
>
> “虽然没有飞镖,但是我会这个!”
> 推荐职业: 爆破骑士、工程师、医生

## 参数

| 参数名                           | 默认值 | 备注                                                       |
| -------------------------------- | ------ | ---------------------------------------------------------- |
| `ZF_NINJA_ATTACK`                | -40    | 攻击力惩罚                                                 |
| `ZF_NINJA_SPEED`                 | 50     | 移动速度加成                                               |
| `ZF_NINJA_DURATION_DECOY_ACTIVE` | 15     | 撤退点的有效持续时间（秒）                                 |
| `ZF_NINJA_DURATION_DECOY_DECAY`  | 5      | 诱饵消失前的持续时间（秒）                                 |
| `ZF_NINJA_DURATION_COOLDOWN`     | 30     | 诱饵使用后的冷却时间（秒）                                 |
| `ZF_NINJA_FALLDMG_RESIST`        | 50     | 坠落伤害抗性百分比                                         |
| `ZF_NINJA_FORCE`                 | 600.0  | 跳跃力量                                                   |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   攻击力 (`ZFStatAtt`) 增加 `ZF_NINJA_ATTACK` (-40)。
    -   移动速度 (`ZFStatSpeed`) 增加 `ZF_NINJA_SPEED` (50)。

### `OnPlayerRunCmd`

-   当玩家按下跳跃键时，如果玩家在地面上且未被眩晕，会以 `ZF_NINJA_FORCE` (600.0) 的力量进行一次大跳。

### `perk_OnTakeDamage`

-   **坠落伤害**:
    -   受到的坠落伤害会减少 `ZF_NINJA_FALLDMG_RESIST` (50)%。
-   **被近战攻击**:
    -   当被僵尸近战攻击时，如果撤退点已激活 (`zf_perkState[client] == 1`)：
        -   玩家会被传送到预设的撤退点。
        -   在玩家原位置生成一个诱饵模型 (`ZFMDL_CUTOUT`)。
        -   撤退点被消耗。
        -   诱饵在 `ZF_NINJA_DURATION_DECOY_DECAY` (5) 秒后消失。
        -   此次攻击不造成伤害。

### `perk_OnCallForMedic`

-   当忍者玩家按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上且未蹲下。
    -   如果满足条件，则在玩家当前位置设置一个撤退点（通过 `zf_aura` 实现）。
    -   设置 `zf_perkState` 为1，表示撤退点已激活。
    -   撤退点在 `ZF_NINJA_DURATION_DECOY_ACTIVE` (15) 秒后失效。
    -   技能进入总共 `ZF_NINJA_DURATION_DECOY_ACTIVE + ZF_NINJA_DURATION_COOLDOWN` (45) 秒的冷却。

### `updateCondStats`

-   管理撤退点和技能的计时器。
-   当撤退点失效或技能冷却结束时，会向玩家发送提示信息。
-   HUD会显示技能是否准备就绪。# 幸存者职业：治安官 (Nonlethal)

## 介绍

**名称**: 治安官 (Nonlethal)

**简短描述**:
> 治安官(Nonlethal)——低威力子弹,击退僵尸

**详细描述**:
> 使用子弹类武器时,你的攻击力降低,但能造成击退效果.
>
> “离我远点!”
> 推荐职业: 工程师、狙击手

## 参数

| 参数名                       | 默认值 | 备注                               |
| ---------------------------- | ------ | ---------------------------------- |
| `ZF_NONLETHAL_ATTACK_BULLET` | -90    | 使用子弹类武器时的攻击力惩罚       |
| `ZF_NONLETHAL_FORCE`         | 75.0   | 每次子弹攻击施加的基础击退力量     |

## 逻辑处理

### `perk_OnTakeDamage`

-   当一个僵尸被一个拥有此职业的幸存者攻击时：
    -   如果攻击是子弹类型 (`attackWasBullet`)，则该幸存者的攻击力会受到 `ZF_NONLETHAL_ATTACK_BULLET` (-90) 的惩罚。这意味着子弹伤害会变得非常低。

### `perk_OnTakeDamagePost`

-   在伤害计算之后，如果攻击是子弹类型 (`attackWasBullet`)：
    -   会对被击中的僵尸施加一个击退效果。
    -   击退的力量大小为 `ZF_NONLETHAL_FORCE` (75.0) 乘以造成的最终伤害值。这意味着即使伤害很低，也能产生显著的击退效果。# 幸存者职业：计划通 (Resourceful)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 计划通 (Resourceful)

**简短描述**:
> /BUG/计划通(Resourceful)——和补给品做朋友

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你的每个击杀都会为你补充子弹、生命值和金属.
> 弹药包会给你临时的攻击力加成,医疗包则是防御力加成.

## 参数

| 参数名                          | 默认值 | 备注                                         |
| ------------------------------- | ------ | -------------------------------------------- |
| `ZF_RESOURCEFUL_AMMOPCT`        | 0.20   | 每次击杀获得的弹药百分比                     |
| `ZF_RESOURCEFUL_ATTACK`         | 25     | 拾取弹药包时的临时攻击力加成                 |
| `ZF_RESOURCEFUL_DEFEND`         | 25     | 拾取医疗包时的临时防御力加成                 |
| `ZF_RESOURCEFUL_HEALTH`         | 25     | 每次击杀获得的生命值（最高至上限）           |
| `ZF_RESOURCEFUL_HEALTH_OVERHEAL`| 15     | 每次击杀获得的额外生命值（可过量治疗）       |
| `ZF_RESOURCEFUL_METAL`          | 25     | 每次击杀获得的金属量                         |
| `ZF_RESOURCEFUL_PICKUP_DURATION`| 10     | 拾取物品后临时加成的持续时间（秒）           |

## 逻辑处理

### `perk_OnPlayerDeath`

-   当拥有此职业的幸存者击杀一个僵尸时：
    -   回复 `ZF_RESOURCEFUL_HEALTH` (25) 点生命值，并可额外获得 `ZF_RESOURCEFUL_HEALTH_OVERHEAL` (15) 点过量治疗。
    -   主武器和副武器弹药按比例 (`ZF_RESOURCEFUL_AMMOPCT`, 20%) 回复。
    -   回复 `ZF_RESOURCEFUL_METAL` (25) 点金属。

### `perk_OnAmmoPickup`

-   当拥有此职业的幸存者拾取弹药包时：
    -   主武器和副武器弹药回满。
    -   金属回满。
    -   获得一个持续 `ZF_RESOURCEFUL_PICKUP_DURATION` (10) 秒的 `ZF_RESOURCEFUL_ATTACK` (25) 攻击力加成。

### `perk_OnMedPickup`

-   当拥有此职业的幸存者拾取医疗包时：
    -   生命值回满。
    -   获得一个持续 `ZF_RESOURCEFUL_PICKUP_DURATION` (10) 秒的 `ZF_RESOURCEFUL_DEFEND` (25) 防御力加成。# 幸存者职业：利他主义者 (Selfless)

## 介绍

**名称**: 利他主义者 (Selfless)

**简短描述**:
> 利他主义者(Selfless)——拉僵尸垫背

**详细描述**:
> 你死后会爆炸,造成成吨的伤害.
>
> “幸存者们,这是我最后的波纹了,收下吧!”
> 推荐职业: 任何

## 参数

| 参数名               | 默认值 | 备注             |
| -------------------- | ------ | ---------------- |
| `ZF_SELFLESS_DAMAGE` | 10000  | 死亡爆炸的伤害   |
| `ZF_SELFLESS_RADIUS` | 5000   | 死亡爆炸的半径   |

## 逻辑处理

### `perk_OnPlayerDeath`

-   当拥有此职业的幸存者被僵尸击杀时：
    -   会在该幸存者的位置触发一次爆炸。
    -   爆炸对半径 `ZF_SELFLESS_RADIUS` (5000) 内的僵尸造成 `ZF_SELFLESS_DAMAGE` (10000) 点伤害。
    -   同时会触发一个大型爆炸视觉效果 (`fxExplosionBig`)。# 幸存者职业：仓鼠 (Stash)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 仓鼠 (Stash)

**简短描述**:
> /BUG/仓鼠(Stash)——放置藏身处

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你可以通过蹲下来发医生语音来放置一个藏身处.
> 藏身处需要远离其他幸存者或藏身处.
> 藏身处会为你补充弹药、生命,并提供临时加成.
> 藏身处持续时间30秒,冷却时间30秒.

## 参数

| 参数名                       | 默认值   | 备注                                                       |
| ---------------------------- | -------- | ---------------------------------------------------------- |
| `STASH_GRAB_ATTACK_DURATION` | 45       | 拾取藏身处后临时攻击加成的持续时间（秒）                   |
| `STASH_GRAB_ATTACK_PERM`     | 10       | 拾取藏身处后的永久攻击力加成                               |
| `STASH_GRAB_ATTACK_TEMP`     | 100      | 拾取藏身处后的临时攻击力加成                               |
| `STASH_GRAB_HEALTH`          | 200      | 拾取藏身处获得的生命值                                     |
| `STASH_COOLDOWN`             | 30       | 藏身处准备就绪后，可以放置新藏身处前的冷却时间（秒）       |
| `STASH_WARMUP`               | 40       | 放置藏身处后，其变为可用状态的准备时间（秒）               |
| `STASH_GRAB_RADSQ`           | 2500     | 拾取藏身处的半径（50\*50）                                 |
| `STASH_DROP_RADSQ_STASH`     | 40000    | 放置新藏身处时，附近不能有其他藏身处的半径（200\*200）     |
| `STASH_DROP_RADSQ_CLIENT`    | 640000   | 放置新藏身处时，附近不能有其他幸存者的半径（800\*800）     |

## 逻辑处理

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上、处于蹲伏状态。
    -   检查附近 `STASH_DROP_RADSQ_CLIENT` 范围内是否有其他幸存者。
    -   检查附近 `STASH_DROP_RADSQ_STASH` 范围内是否有其他藏身处。
    -   如果所有条件满足，则在玩家位置放置一个补给箱模型 (`ZFMDL_SUPPLYCRATE`) 作为藏身处。
    -   设置一个总共 `STASH_WARMUP + STASH_COOLDOWN` (70) 秒的计时器。

### `updateCondStats`

-   **计时器管理**:
    -   计时器启动后，经过 `STASH_WARMUP` (40) 秒，藏身处变为“准备就绪”状态，并向玩家发送提示。
    -   计时器走完后，玩家可以放置新的藏身处。
-   **拾取逻辑**:
    -   当藏身处“准备就绪”后 (`zf_perkTimer[thisSur] <= STASH_COOLDOWN`)，如果玩家进入其 `STASH_GRAB_RADSQ` 范围内：
        -   玩家获得 `STASH_GRAB_HEALTH` (200) 点生命（可过量治疗）。
        -   弹药和金属回满。
        -   获得一个永久的 `STASH_GRAB_ATTACK_PERM` (10) 攻击力加成。
        -   获得一个持续 `STASH_GRAB_ATTACK_DURATION` (45) 秒的 `STASH_GRAB_ATTACK_TEMP` (100) 临时攻击力加成。
        -   藏身处被消耗并移除。# 幸存者职业：狂热者 (Stir-Crazy)

## 介绍

**名称**: 狂热者 (Stir-Crazy)

**简短描述**:
> 狂热者(Stir-Crazy)——速度决定攻击力

**详细描述**:
> 你的移动速度会给你提供攻击力加成,速度越快,攻击力就越高.
>
> “这个职业曾经能获得200%的攻击力加成.”
> 推荐职业: 火焰兵、爆破骑士

## 参数

| 参数名                   | 默认值 | 备注                                                               |
| ------------------------ | ------ | ------------------------------------------------------------------ |
| `ZF_STIRCRAZY_MAX_POINTS`| 5      | 用于计算平均位置的先前位置点的数量                                 |
| `ZF_STIRCRAZY_DIST_MIN`  | 150    | 开始应用加成的、与平均位置的最小距离                               |
| `ZF_STIRCRAZY_DIST_MAX`  | 750    | 达到此距离时获得最大加成，超过此距离加成不再增加                   |
| `ZF_STIRCRAZY_ATTACK`    | 30     | 达到 `ZF_STIRCRAZY_DIST_MAX` 距离时获得的最大攻击力加成            |

## 逻辑处理

### `perk_OnPlayerSpawn`

-   当玩家重生时，会用玩家当前的位置初始化一个包含 `ZF_STIRCRAZY_MAX_POINTS` (5) 个位置点的缓冲区。

### `updateCondStats`

-   该职业的核心逻辑在 `updateCondStats` 中周期性执行：
    1.  **更新位置缓冲区**: 将玩家当前位置存入一个循环缓冲区 (`zf_perkPos`)。
    2.  **计算平均位置**: 计算缓冲区中所有位置点的平均位置。
    3.  **计算距离**: 计算玩家当前位置与算出的平均位置之间的距离。
    4.  **应用攻击加成**:
        -   如果计算出的距离大于 `ZF_STIRCRAZY_DIST_MIN` (150)，则开始提供攻击力加成。
        -   加成的大小与距离成正比，当距离达到 `ZF_STIRCRAZY_DIST_MAX` (750) 时，获得最大的 `ZF_STIRCRAZY_ATTACK` (30) 攻击力加成。
        -   加成系数的计算方式为 `min(1.0, 距离 / ZF_STIRCRAZY_DIST_MAX)`。# 幸存者职业：供应商 (Supplier)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 供应商 (Supplier)

**简短描述**:
> /BUG/供应商(Supplier)——放置弹药补给箱

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你的弹药会周期性的补充,并且拥有双倍备弹量.
> 发医生语音来放置一个补给箱,其他幸存者可以从中补给弹药.
> 冷却时间10秒.

## 参数

| 参数名                           | 默认值 | 备注                                                               |
| -------------------------------- | ------ | ------------------------------------------------------------------ |
| `ZF_SUPPLIER_MAX_ITEMS`          | 2      | 最多可以同时存在的补给箱数量                                       |
| `ZF_SUPPLIER_TIMER`              | 10     | 放置补给箱的冷却时间（秒）                                         |
| `ZF_SUPPLIER_RADIUSSQ`           | 5625   | 拾取补给的半径（75\*75）                                           |
| `ZF_SUPPLIER_UPDATERATE`         | 10     | 自身弹药自动补充的周期（秒）                                       |
| `ZF_SUPPLIER_SELF_DEFEND`        | 25     | 永久防御力加成                                                     |
| `ZF_SUPPLIER_AMMOPCT_RESLIMIT`   | 2.0    | 主武器和副武器的备弹量上限倍率（双倍备弹）                         |
| `ZF_SUPPLIER_AMMOPCT_SELF`       | 0.10   | 每次自我补充时获得的弹药百分比                                     |
| `ZF_SUPPLIER_AMMOPCT_OTHER`      | 0.25   | 其他幸存者从补给箱中获得的弹药百分比                               |
| `ZF_SUPPLIER_RESUPPLY_COUNT`     | 4      | 每个补给箱可被使用的次数                                           |
| `ZF_SUPPLIER_ATTACK`             | 25     | 当补给被拾取时，供应商获得的临时攻击力加成                         |
| `ZF_SUPPLIER_DURATION`           | 10     | 供应商获得临时攻击力加成的持续时间（秒）                           |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   防御力 (`ZFStatDef`) 增加 `ZF_SUPPLIER_SELF_DEFEND` (25)。

### `updateCondStats`

-   **自我补给**:
    -   每隔 `ZF_SUPPLIER_UPDATERATE` (10) 秒，会自动为供应商补充 `ZF_SUPPLIER_AMMOPCT_SELF` (10%) 的弹药和金属，上限为 `ZF_SUPPLIER_AMMOPCT_RESLIMIT` (2.0) 倍。
-   **补给箱逻辑**:
    -   检查其他幸存者是否在补给箱的 `ZF_SUPPLIER_RADIUSSQ` 范围内。
    -   如果幸存者弹药未满，他们会自动拾取补给，回复 `ZF_SUPPLIER_AMMOPCT_OTHER` (25%) 的弹药和金属。
    -   每次有幸存者拾取补给，供应商会获得一个持续 `ZF_SUPPLIER_DURATION` (10) 秒的 `ZF_SUPPLIER_ATTACK` (25) 攻击力加成。
    -   每个补给箱有 `ZF_SUPPLIER_RESUPPLY_COUNT` (4) 次使用次数，用完后消失。
-   **计时器与HUD**:
    -   管理放置补给箱的冷却时间，并在HUD上显示是否准备就绪。

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   检查冷却时间、是否在地面、是否蹲伏以及是否有可用的物品栏位。
    -   如果满足条件，则在玩家位置放置一个补给箱模型 (`ZFMDL_SUPPLYCRATE`)。
    -   设置 `ZF_SUPPLIER_TIMER` (10) 秒的冷却时间。# 幸存者职业：暴脾气 (Tantrum)

## 介绍

**名称**: 暴脾气 (Tantrum)

**简短描述**:
> 暴脾气(Tantrum)——短时间内获得暴击

**详细描述**:
> 发医生语音来激活愤怒.
> 愤怒给予你15秒的暴击,结束后进入30秒的疲惫状态.
> 疲惫状态下,移动速度大幅降低.
>
> “我想这个职业的确很解压.”
> 推荐职业: 士兵、火焰兵、爆破手

## 参数

| 参数名                | 默认值 | 备注                               |
| --------------------- | ------ | ---------------------------------- |
| `ZF_TANTRUM_ACTIVE`   | 15     | 愤怒（100%暴击）状态的持续时间（秒） |
| `ZF_TANTRUM_COOLDOWN` | 30     | 疲惫（速度惩罚）状态的持续时间（秒） |
| `ZF_TANTRUM_SPEED`    | -100   | 疲惫状态下的移动速度惩罚           |

## 逻辑处理

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   如果技能冷却完毕 (`zf_perkTimer[client] == 0`)，则激活技能。
    -   设置一个总共 `ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN` (45) 秒的计时器。
    -   在 `ZF_TANTRUM_ACTIVE` (15) 秒内，玩家获得100%暴击 (`addCondKritz`)。
    -   触发暴击视觉效果 (`fxKritzStart`)。

### `updateCondStats`

-   **计时器管理**:
    -   当计时器在 `(0, ZF_TANTRUM_COOLDOWN]` 区间时，玩家处于疲惫状态，移动速度受到 `ZF_TANTRUM_SPEED` (-100) 的惩罚。
    -   当计时器在 `(ZF_TANTRUM_COOLDOWN, ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN]` 区间时，玩家处于愤怒状态，获得暴击。
    -   当愤怒状态结束时，暴击效果停止 (`fxKritzStop`)，并提示玩家进入疲惫状态。
    -   当疲惫状态结束时，提示玩家技能准备就绪。
-   **HUD**:
    -   HUD会显示技能是否准备就绪。# 幸存者职业：陷阱大师 (Trapper)

## 介绍

**名称**: 陷阱大师 (Trapper)

**简短描述**:
> 陷阱大师(Trapper)——放置地雷

**详细描述**:
> 发医生语音来放置地雷,僵尸碰到后会自动爆炸,同时最多拥有5个地雷.
> 冷却时间20秒.
>
> “EDD Mounted, let them come.”
> 推荐职业: 任何

## 参数

| 参数名                 | 默认值 | 备注                                                       |
| ---------------------- | ------ | ---------------------------------------------------------- |
| `ZF_TRAPPER_MAX_ITEMS` | 5      | 最多可以同时存在的地雷数量                                 |
| `ZF_TRAPPER_DAMAGE`    | 200    | 地雷造成的伤害                                             |
| `ZF_TRAPPER_RADIUS`    | 150    | 地雷触发和造成伤害的半径                                   |
| `ZF_TRAPPER_RADIUSSQ`  | 40000  | 地雷触发和造成伤害的半径（平方值, 200\*200），代码中实际用于检测 |
| `ZF_TRAPPER_TIMER`     | 20     | 放置地雷的冷却时间（秒）                                   |

## 逻辑处理

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上、处于蹲伏状态。
    -   检查当前地雷数量是否已达到 `ZF_TRAPPER_MAX_ITEMS` (5) 的上限。
    -   如果满足条件，则在玩家位置放置一个地雷模型 (`ZFMDL_MINE`)。
    -   设置 `ZF_TRAPPER_TIMER` (20) 秒的冷却时间。

### `updateCondStats`

-   **地雷检测与引爆**:
    -   周期性检查每个已放置的地雷。
    -   检测 `ZF_TRAPPER_RADIUSSQ` (40000) 范围内是否有僵尸。
    -   **引爆**: 如果有僵尸进入范围，并且该僵尸不是“磁化僵尸”，地雷会引爆：
        -   点燃目标僵尸。
        -   在 `ZF_TRAPPER_RADIUS` (150) 半径内造成 `ZF_TRAPPER_DAMAGE` (200) 点范围伤害。
        -   触发大型爆炸视觉效果 (`fxExplosionBig`)。
        -   地雷被消耗并移除。
    -   **失效**: 如果有“磁化僵尸”进入范围，地雷会暂时失效，并产生火花效果 (`fxSpark`)。
    -   **待机**: 如果没有僵尸在附近，地雷会发出“滴答”声 (`ZFSND_TICK`)。
-   **计时器与HUD**:
    -   管理放置地雷的冷却时间，并在HUD上显示是否准备就绪。# 幸存者职业：肉盾 (Turtle)

## 介绍

**名称**: 肉盾 (Turtle)

**简短描述**:
> 肉盾(Turtle)——防御力高,速度慢

**详细描述**:
> 你的防御力大幅增加,但你的攻击力降低,而且无法造成随机暴击.
> Spy无法背刺你.
>
> “你们僵尸就这点能耐?”
> 推荐职业: 士兵、火焰兵、爆破骑士

## 参数

| 参数名                    | 默认值 | 备注                               |
| ------------------------- | ------ | ---------------------------------- |
| `ZF_TURTLE_ATTACK`        | -50    | 攻击力惩罚                         |
| `ZF_TURTLE_DEFEND`        | 75     | 防御力加成                         |
| `ZF_TURTLE_SPEED`         | -100   | 移动速度惩罚                       |
| `ZF_TURTLE_STUN_DURATION` | 1.0    | 当僵尸背刺失败时，对其造成的眩晕时间（秒） |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   攻击力 (`ZFStatAtt`) 增加 `ZF_TURTLE_ATTACK` (-50)。
    -   防御力 (`ZFStatDef`) 增加 `ZF_TURTLE_DEFEND` (75)。
    -   移动速度 (`ZFStatSpeed`) 增加 `ZF_TURTLE_SPEED` (-100)。

### `perk_OnTakeDamage`

-   当一个拥有此职业的幸存者被一个僵尸（间谍）尝试背刺时 (`attackWasBackstab`)：
    -   该次背刺攻击的伤害将被完全免疫（伤害设置为0.0）。
    -   攻击方（僵尸）会被眩晕 `ZF_TURTLE_STUN_DURATION` (1.0) 秒。
    -   会向攻击方和被攻击方发送提示信息，告知背刺被防住。
    -   触发火花视觉效果 (`fxSpark`)。# 幸存者职业：智者 (Wise)

## 介绍

**名称**: 智者 (Wise)

**简短描述**:
> 智者(Wise)——击杀提高属性

**详细描述**:
> 你的每个击杀和助攻都能使你的攻击力永久增加.
> 每当你被近战武器攻击后,你的防御力就会永久增加.
>
> “从战斗中学习.”
> 推荐职业: 任何

## 参数

| 参数名                 | 默认值 | 备注                                     |
| ---------------------- | ------ | ---------------------------------------- |
| `ZF_WISE_ATTACK_KILL`  | 1      | 每次击杀获得的永久攻击力加成             |
| `ZF_WISE_ATTACK_ASSIST`| 0      | 每次助攻获得的永久攻击力加成             |
| `ZF_WISE_DEFEND`       | 1      | 每次被近战攻击后获得的永久防御力加成     |
| `ZF_WISE_DEFEND_LIMIT` | 20     | 通过被攻击获得的防御力加成的上限         |

## 逻辑处理

### `perk_OnPlayerDeath`

-   当拥有此职业的幸存者**击杀**一个僵尸时：
    -   其永久攻击力 (`ZFStatAtt`, `ZFStatTypePerm`) 增加 `ZF_WISE_ATTACK_KILL` (1)。
-   当拥有此职业的幸存者**助攻击杀**一个僵尸时：
    -   其永久攻击力 (`ZFStatAtt`, `ZFStatTypePerm`) 增加 `ZF_WISE_ATTACK_ASSIST` (0)。

### `perk_OnTakeDamage`

-   当拥有此职业的幸存者被僵尸**近战攻击**时 (`attackWasMelee`)：
    -   如果其通过此方式获得的防御力加成尚未达到 `ZF_WISE_DEFEND_LIMIT` (20) 的上限：
        -   其永久防御力 (`ZFStatDef`, `ZFStatTypePerm`) 增加 `ZF_WISE_DEFEND` (1)。# 幸存者职业：禅师 (Zenlike)

## 介绍

**名称**: 禅师 (Zenlike)

**简短描述**:
> 禅师(Zenlike)——蹲着增加暴击率

**详细描述**:
> 当蹲下不动时,你的生命值会慢慢恢复,并不断增加你的暴击率.
> 每攻击一次,暴击几率都会降低25%.
> (注意:Engi用扳手敲打建筑也会降低暴击率.)
>
> “出来吧,百分百暴击火箭!”
> 推荐职业: 士兵

## 参数

| 参数名                | 默认值 | 备注                                     |
| --------------------- | ------ | ---------------------------------------- |
| `ZF_ZENLIKE_CRIT_INC` | 3      | 蹲下不动时每秒增加的暴击率               |
| `ZF_ZENLIKE_CRIT_DEC` | 25     | 每次攻击后降低的暴击率                   |
| `ZF_ZENLIKE_HEAL`     | 1      | 蹲下不动时每秒恢复的中毒效果量           |
| `ZF_ZENLIKE_REGEN`    | 1      | 蹲下不动时每秒恢复的生命值               |

## 逻辑处理

### `updateCondStats`

-   当拥有此职业的幸存者在地面上、蹲下且没有移动时：
    -   暴击率 (`zf_perkState`) 每秒增加 `ZF_ZENLIKE_CRIT_INC` (3)，上限为100。
    -   每秒回复 `ZF_ZENLIKE_REGEN` (1) 点生命值。
    -   每秒移除 `ZF_ZENLIKE_HEAL` (1) 点中毒效果。
-   增加的暴击率会作为条件性暴击加成 (`ZFStatCrit`, `ZFStatTypeCond`) 应用于玩家。

### `perk_OnCalcIsAttackCritical`

-   当玩家进行一次暴击计算的攻击时（通常是每次开火），会触发一个0.1秒后的计时器 `perk_tZenlikeAttack`。

### `perk_tZenlikeAttack` (Timer Callback)

-   该计时器执行时，会从玩家当前的暴击率 (`zf_perkState`) 中减去 `ZF_ZENLIKE_CRIT_DEC` (25)，下限为0。# 僵尸职业: 零号僵尸 (Alpha)

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
    *   更新HUD，显示技能是否准备就绪。# 僵尸职业: 自爆僵尸 (Combustible)

## 描述

**简短描述:**
> 自爆僵尸(Combustible)——尸如其名

**详细描述:**
> 你的防御力大幅降低. 
> 被远程武器击杀后,你会爆炸并造成伤害. 
> 你不可以使用隐身手表或者原子能饮料. 
> (远程武器包括一切非近战武器) 
> 
> *将Boomer先推开再攻击* 
> 推荐职业: 侦察兵、机枪手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_COMBUSTIBLE` | 2 | Perk的ID |
| `ZF_COMBUSTIBLE_DAMAGE` | 120 | 爆炸伤害 |
| `ZF_COMBUSTIBLE_DAMAGE_HEAVY` | 200 | 作为机枪手(Heavy)时的爆炸伤害 |
| `ZF_COMBUSTIBLE_DEFEND` | -200 | 防御力惩罚 (-200%) |
| `ZF_COMBUSTIBLE_RADIUS` | 300 | 爆炸半径 |
| `Float:ZF_COMBUSTIBLE_RESPAWNTIME` | 4.5 | 爆炸后的重生时间（秒） |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了自爆僵尸，会应用永久性的防御力惩罚 (`ZF_COMBUSTIBLE_DEFEND`)。

2.  **视觉特效**:
    *   在 `updateCondStats` 函数中，会周期性地在玩家身上产生微小的爆炸特效 (`fxExplosionTiny`)，以提示其职业特性。

## 事件处理

*   **`OnPlayerRunCmd` (玩家输入指令)**:
    *   阻止玩家使用原子能饮料 (`IN_ATTACK` + `ZFWEAP_BONK`) 和隐身手表 (`IN_ATTACK2`)，并给出提示。

*   **`perk_OnPlayerDeath` (玩家死亡)**:
    *   如果自爆僵尸被**非近战武器**击杀 (`!attackWasMelee`)，则会触发爆炸。
    *   爆炸伤害根据职业 (`isHeavy`) 分为 `ZF_COMBUSTIBLE_DAMAGE` 和 `ZF_COMBUSTIBLE_DAMAGE_HEAVY`。
    *   爆炸通过 `applyDamageRadialAtClient` 对 `ZF_COMBUSTIBLE_RADIUS` 范围内的敌人造成伤害。
    *   触发一次大型爆炸视觉特效 (`fxExplosionBig`)。
    *   通过计时器 `perk_tSpawnClient` 在 `ZF_COMBUSTIBLE_RESPAWNTIME` 秒后强制重生该玩家。# 僵尸职业: 惊吓僵尸 (Horrifying)

## 描述

**简短描述:**
> 惊吓僵尸(Horrifying)——攻击削弱人类

**详细描述:**
> 你的攻击力降低,但你的攻击能降低幸存者的攻击力、防御力和攻击速度. 
> 减益效果持续15秒.你死亡后,这个效果也随即消失. 
> 
> “敲骨吸髓.” 
> 推荐职业: 机枪手、间谍

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HORRIFYING` | 3 | Perk的ID |
| `HORRIFYING_ATTACK` | -20 | 对非机枪手幸存者施加的攻击力惩罚 |
| `HORRIFYING_ATTACK_HEAVY` | -30 | 对机枪手幸存者施加的攻击力惩罚 |
| `HORRIFYING_DEFEND` | 0 | 对非机枪手幸存者施加的防御力惩罚 |
| `HORRIFYING_DEFEND_HEAVY` | 0 | 对机枪手幸存者施加的防御力惩罚 |
| `HORRIFYING_ROF_HEAVY` | -10 | 对机枪手幸存者施加的攻击速度惩罚 |
| `HORRIFYING_DURATION` | 15 | 惩罚持续时间（秒） |
| `HORRIFYING_DURATION_HEAVY` | 30 | 对机枪手幸存者的惩罚持续时间（秒） |
| `Float:HORRIFYING_PENALTYPCT_KILL` | 0.75 | 击杀惊吓僵尸后，惩罚效果减少的百分比 |
| `Float:HORRIFYING_PENALTYPCT_ASSIST` | 0.25 | 助攻击杀惊吓僵尸后，惩罚效果减少的百分比 |

## 核心逻辑

1.  **光环效果**:
    *   在 `updateClientPermEffects` 函数中，如果玩家是惊吓僵尸，会创建一个蓝色的外层光环 (`ZFPART_AURAOUTBLU`)。

## 事件处理

*   **`perk_OnTakeDamage` (当玩家造成伤害时)**:
    *   当惊吓僵尸通过**近战攻击**伤害一名幸存者时，会触发削弱效果。
    *   该效果通过 `addStatTempStack` 给幸存者施加一个临时的、可叠加的属性惩罚。
    *   惩罚的数值和持续时间取决于惊吓僵尸是否为机枪手 (`isHeavy`)，分别对应 `_HEAVY` 后缀的参数。
    *   惩罚的属性包括：攻击力 (`ZFStatAtt`)、防御力 (`ZFStatDef`) 和攻击速度 (`ZFStatRof`)。

*   **`perk_OnPlayerDeath` (当玩家死亡时)**:
    *   当惊吓僵尸被一名幸存者击杀或助攻击杀时，会减少该幸存者身上由惊吓僵尸施加的临时属性惩罚。
    *   通过 `scaleStatTempPct` 函数，将幸存者身上的负面状态（攻击、防御、攻速）按 `HORRIFYING_PENALTYPCT_KILL` 或 `HORRIFYING_PENALTYPCT_ASSIST` 的百分比进行缩减，从而减轻debuff效果。# 僵尸职业: 猎手僵尸 (Hunter)

## 描述

**简短描述:**
> 猎手僵尸(Hunter)——手动放置重生点

**详细描述:**
> 发医生语音来放置你的重生点. 
> 从自己的重生点重生时,你的重生时间较短,并获得临时的攻击加成. 
> 每次重生后,你只能放置一次重生点. 
> 注意!幸存者可以摧毁你的重生点. 
> 
> “你将成为我的猎物!” 
> 推荐职业: 任何

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HUNTER` | 4 | Perk的ID |
| `ZF_HUNTER_ATTACK` | 50 | 在重生点重生时获得的临时攻击加成 |
| `ZF_HUNTER_DURATION` | 10 | 临时攻击加成的持续时间（秒） |
| `ZF_HUNTER_RADIUSSQ` | (85 * 85) | 幸存者摧毁重生点的半径（平方） |
| `Float:ZF_HUNTER_RESPAWNTIME` | 5.5 | 死亡后的快速重生时间（秒） |

## 核心逻辑

1.  **放置重生点**:
    *   在 `perk_OnCallForMedic` 事件中，如果玩家是猎手僵尸且本回合尚未放置过重生点 (`zf_perkState[client] == 0`)，则可以在当前位置创建一个重生点。
    *   重生点通过一个视觉光环 (`ZFPART_AURAVORTEXBLU`) 表示，并记录下当前的观察角度 (`zf_perkPos[client][0]`) 用于重生。
    *   放置后，状态 `zf_perkState[client]` 会被设为1，防止重复放置。

2.  **在重生点重生**:
    *   在 `perk_OnPlayerSpawn` 事件中，如果猎手僵尸拥有一个有效的重生点 (`validAura(client)`)，他将被传送到该点。
    *   传送后，玩家会获得 `ZF_HUNTER_ATTACK` 的临时攻击力加成，持续 `ZF_HUNTER_DURATION` 秒。
    *   同时，为了兼容性，会移除玩家的重生保护。

3.  **重生点被摧毁**:
    *   在 `updateCondStats` 的周期性检查中，如果一个幸存者进入了某个重生点 `ZF_HUNTER_RADIUSSQ` 范围，该重生点将被移除 (`removeAura`)，并向双方玩家显示提示信息。

## 事件处理

*   **`perk_OnCallForMedic` (玩家按“E”键)**:
    *   触发放置重生点的逻辑。

*   **`perk_OnPlayerSpawn` (玩家重生)**:
    *   触发在重生点重生的逻辑。

*   **`perk_OnPlayerDeath` (玩家死亡)**:
    *   如果猎手僵尸死亡，会通过 `showAura` 重新显示重生点光环（如果存在）。
    *   同时，会创建一个计时器，在 `ZF_HUNTER_RESPAWNTIME` 秒后尝试重生玩家（此逻辑与其他快速重生职业共享）。

*   **`updateCondStats` (周期性更新)**:
    *   触发重生点被摧毁的逻辑检查。# 僵尸职业: 飞跃僵尸 (Leap)

## 描述

**简短描述:**
> 飞跃僵尸(Leap)——大跳飞向空中

**详细描述:**
> 你的攻击力与防御力降低,但不受坠落伤害. 
> 发医生语音来施展大跳,冷却时间4秒. 
> 
> “起飞!” 
> 推荐职业: 侦察兵、间谍

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_LEAP` | 5 | Perk的ID |
| `ZF_LEAP_COMBAT` | -20 | 攻击力和防御力的永久惩罚 (-20) |
| `ZF_LEAP_COOLDOWN` | 4 | 大跳技能的冷却时间（秒） |
| `Float:ZF_LEAP_FORCE` | 900.0 | 大跳的力度 |
| `Float:ZF_LEAP_FORCE_SCOUT` | 1500.0 | 侦察兵(Scout)的大跳力度 |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了飞跃僵尸，会应用永久性的攻击力和防御力惩罚 (`ZF_LEAP_COMBAT`)。

2.  **免疫坠落伤害**:
    *   在 `perk_OnTakeDamage` 事件中，会检查伤害类型。如果是坠落伤害 (`attackWasSelfFall`)，则将伤害设置为0。

## 事件处理

*   **`perk_OnCallForMedic` (玩家按“E”键)**:
    *   如果技能不在冷却中 (`zf_perkTimer[client] == 0`)，且玩家在地面上 (`isGrounded`) 并且没有隐身 (`!isCloaked`)，则触发大跳。
    *   大跳通过 `fxJump` 函数实现，力度根据玩家是否为侦察兵 (`isScout`) 分别使用 `ZF_LEAP_FORCE_SCOUT` 或 `ZF_LEAP_FORCE`。
    *   触发后，技能进入冷却，时间为 `ZF_LEAP_COOLDOWN` 秒。

*   **`updateCondStats` (周期性更新)**:
    *   处理大跳技能的冷却倒计时。
    *   更新HUD，显示技能是否准备就绪。# 僵尸职业: 磁化僵尸 (Magnetic)

## 描述

**简短描述:**
> 磁化僵尸(Magnetic)——瘫痪附近建筑

**详细描述:**
> 你能使附近的步哨和地雷失效. 
> 
> “I will murder your toys as well.” 
> 推荐职业: 任何

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_MAGNETIC` | 6 | Perk的ID |
| `ZF_MAGNETIC_RADIUSSQ` | (500 * 500) | 使步哨枪失效的光环半径（平方） |

## 核心逻辑

1.  **免于锁定**:
    *   在 `perk_OnPlayerSpawn` 事件中，为磁化僵尸添加 `TF_COND_NO_TARGET` 状态，这使得步哨枪不会主动攻击它。

2.  **瘫痪建筑**:
    *   该职业的核心逻辑在 `updateCondStats` 函数中周期性执行。
    *   **瘫痪步哨枪**:
        *   函数会遍历地图上所有的步哨枪 (`obj_sentrygun`)。
        *   对于每个步哨枪，它会检查是否有任何一个磁化僵尸进入了其 `ZF_MAGNETIC_RADIUSSQ` 范围。
        *   如果有，该步哨枪的 `m_bDisabled` 属性会被设为1，使其停止工作，并显示火花特效 (`fxSpark`)。
        *   当磁化僵尸离开范围后，该属性会被重新设为0，步哨枪恢复正常。
    *   **瘫痪地雷**:
        *   在“陷阱大师 (Trapper)”职业的逻辑中，当检查地雷是否引爆时，会同时判断附近是否有磁化僵尸。
        *   如果地雷在磁化僵尸的 `ZF_MAGNETIC_RADIUSSQ` 范围内，地雷将被视为“已禁用” (`mineDisabled = true`)，即使有僵尸踩上去也不会爆炸。

## 事件处理

该职业的功能完全通过被动光环和周期性检查实现，没有特定的主动事件处理逻辑。# 僵尸职业: 标记僵尸 (Marked)

## 描述

**简短描述:**
> 标记僵尸(Marked)——瞄准特定目标

**详细描述:**
> 系统会随机选择一名幸存者作为你的目标.
> 你对目标能造成极高伤害,但是对其他人造成较低伤害. 
> 当前目标死亡后,若剩余的幸存者超过1个,10秒后将自动选择一个新目标. 
> 
> “目标已经标记出来了!” 
> 推荐职业: 侦察兵、机枪手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_MARKED` | 7 | Perk的ID |
| `ZF_MARKED_ATTACK_ON_MARK` | 200 | 对标记目标造成的额外攻击加成 |
| `ZF_MARKED_ATTACK_OFF_MARK` | -10 | 对非标记目标造成的攻击惩罚 |
| `ZF_MARKED_MIN_SURVIVORS` | 1 | 触发标记所需的最少幸存者数量 |
| `ZF_MARKED_TIMER` | 10 | 重新选择标记目标的冷却时间（秒） |

## 核心逻辑

1.  **选择目标**:
    *   在 `perk_OnGraceEnd` (准备时间结束) 或 `doMarkedSelect` 函数被调用时，系统会开始选择目标。
    *   如果存活的幸存者数量不少于 `ZF_MARKED_MIN_SURVIVORS`，系统会从存活的幸存者中随机选择一个作为目标，并将其ID存入 `zf_perkState[client]`。
    *   一个感叹号图标 (`ZFSPR_EXCLAMATION`) 会被创建并显示在被标记的幸存者头上，但这个图标只对该标记僵尸可见。
    *   标记僵尸和被标记的幸存者都会收到相应的提示信息。

2.  **伤害调整**:
    *   在 `perk_OnTakeDamage` 事件中，当标记僵尸用**近战攻击**伤害一名幸存者时：
        *   如果受害者是被标记的目标 (`zf_perkState[attacker] == victim`)，则攻击力会增加 `ZF_MARKED_ATTACK_ON_MARK`。
        *   如果受害者不是被标记的目标，则攻击力会减少 `ZF_MARKED_ATTACK_OFF_MARK`。

3.  **重新选择目标**:
    *   在 `updateCondStats` 的周期性检查中，会持续监控被标记目标的状态。
    *   如果被标记的幸存者死亡 (`!validLivingSur(zf_perkState[thisZom])`)，系统会清除标记图标，并将 `zf_perkState[thisZom]` 设为0，然后启动一个 `ZF_MARKED_TIMER` 秒的冷却计时器。
    *   计时器结束后，会自动调用 `doMarkedSelect` 来选择一个新的目标。

## 事件处理

*   **`perk_OnGraceEnd`**: 游戏回合正式开始时，为标记僵尸选择第一个目标。
*   **`perk_OnTakeDamage`**: 处理对标记目标和非标记目标的伤害调整。
*   **`updateCondStats`**: 监控目标存活状态，并在其死亡后启动重新选择目标的流程。
*   **`perk_OnSetTransmit`**: 控制标记图标只对特定的标记僵尸可见。# 狂怒僵尸 (Rage)

## 简介

> *“Taaaaaaaaaank!”*

**狂怒僵尸** 是一个能在短时间内大幅度提升自身生存能力和机动性的职业。通过激活“愤怒”能力，它会变成一个临时的“坦克”，拥有更高的生命值和移动速度，能够冲散幸存者的防线或承受大量伤害。

## 详细说明

狂怒僵尸的核心能力是 **愤怒**。

- **激活方式**: 按下 `医生语音` 键 (默认 `V` 键) 来激活。
- **激活条件**: 你的当前生命值必须在 **80%** 以上才能成功激活此能力。
- **能力效果**:
    - 你的最大生命值会立刻提升 **50%** (总计达到150%)。
    - 你的移动速度会增加 **100%**。
- **效果终止**: 当你的生命值因受到伤害而降低到 **80%** 以下时，愤怒效果会立即消失，你的生命值和速度恢复正常。
- **冷却时间**: 技能结束后，会进入 **20秒** 的冷却时间。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_RAGE_COOLDOWN` | `20` | 技能冷却时间（秒）。 |
| `ZF_RAGE_SPEED` | `100` | 激活愤怒状态时获得的速度加成百分比。 |
| `ZF_RAGE_HEALTHPCT_TOUSE` | `0.80` | 激活技能所需的最低生命值百分比。 |
| `ZF_RAGE_HEALTHPCT_ONUSE` | `0.50` | 激活技能时增加的生命值百分比。 |

## 事件处理逻辑

- **`perk_OnCallForMedic`**:
    - 检查玩家是否为狂怒僵尸。
    - 验证冷却时间 (`zf_perkTimer`) 是否为0。
    - 验证玩家生命值百分比是否高于 `ZF_RAGE_HEALTHPCT_TOUSE`。
    - 如果所有条件满足，则激活愤怒状态：
        - 设置 `zf_perkState` 为 `1` (激活状态)。
        - 设置 `zf_perkTimer` 为 `ZF_RAGE_COOLDOWN`。
        - 增加速度和生命值。
        - 播放视觉特效 (`fxPowerup`, `ZFPART_AURABURNINGORANGE`)。

- **`updateCondStats` (每秒执行)**:
    - 如果愤怒状态未激活 (`zf_perkState == 0`)，则减少冷却计时器 `zf_perkTimer`。
    - 如果愤怒状态已激活 (`zf_perkState == 1`)：
        - 持续检查生命值百分比。如果低于 `ZF_RAGE_HEALTHPCT_TOUSE`，则重置状态 (`zf_perkState = 0`) 并移除视觉光环。
        - 否则，持续应用速度加成。

- **`perk_OnPlayerSpawn`**:
    - 重置 `zf_perkState` 和 `zf_perkTimer`，确保每次重生时技能都处于可用状态。

- **`perk_OnPlayerDeath`**:
    - 移除愤怒状态的视觉光环特效。

## 推荐职业

- **机枪手**: 机枪手本身的高血量与愤怒状态的生命加成相结合，可以创造出一个极难被杀死的巨兽，能够有效吸收成吨的伤害。# 咆哮僵尸 (Roar)

## 简介

> *“哈!”*

**咆哮僵尸** 是一种区域控制型职业，能够通过强大的声波攻击来扰乱幸存者的阵型。它的咆哮不仅能将幸存者推开，还能在短时间内削弱他们的防御力，为其他僵尸创造进攻机会。

## 详细说明

咆哮僵尸的核心能力是 **咆哮**。

- **激活方式**: 按下 `医生语音` 键 (默认 `V` 键) 来激活。
- **激活条件**:
    - 必须站在地面上。
    - 不能处于隐身状态。
- **能力效果**:
    - 对半径 **450** 单位内的所有幸存者造成一次范围攻击。
    - **击退**: 幸存者会被强大的力量推开。
    - **震慑 (Dazed)**: 幸存者的防御力会被暂时降低。震慑效果的持续时间取决于距离和咆哮僵尸的职业（机枪手会造成更长时间的震慑）。
- **冷却时间**: **15秒**。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_ROAR_COOLDOWN` | `15` | 技能冷却时间（秒）。 |
| `ZF_ROAR_DURATION` | `20` | 非机枪手僵尸造成的“震慑”状态的基础持续时间。 |
| `ZF_ROAR_DURATION_HEAVY` | `60` | 机枪手僵尸造成的“震慑”状态的基础持续时间。 |
| `ZF_ROAR_FORCE` | `1200.0` | 非机枪手僵尸造成的击退力量。 |
| `ZF_ROAR_FORCE_HEAVY` | `3000.0` | 机枪手僵尸造成的击退力量。 (代码中为 `3000.0`，但注释建议值为 `2000.0`) |
| `ZF_ROAR_RADIUS` | `450` | 咆哮影响的半径范围。 |
| `ZF_DAZE_DEFEND` | `-40` | “震慑”状态下幸存者受到的防御力惩罚。 |

## 事件处理逻辑

- **`perk_OnCallForMedic`**:
    - 检查玩家是否为咆哮僵尸且技能冷却完毕。
    - 验证玩家是否站在地上且未隐身。
    - 若满足条件，则触发一次范围伤害 (`applyDamageRadialAtClient`)。这个伤害本身很小，其主要目的是为了触发 `perk_OnTakeDamage` 事件来施加击退和减益效果。
    - 设置 `zf_perkTimer` 为 `ZF_ROAR_COOLDOWN` 并播放音效。

- **`perk_OnTakeDamage`**:
    - 监听由咆哮僵尸触发的环境爆炸伤害 (`attackWasEnvExplosion`)。
    - 计算击退力量 (`force`) 和震慑持续时间 (`duration`)，该值会根据幸存者与咆哮僵尸之间的距离进行衰减。
    - 对范围内的幸存者施加击退效果 (`fxKnockback`)。
    - 对范围内的幸存者施加“震慑”状态 (`ZFCondIntimidated`)。

- **`updateCondStats` (每秒执行)**:
    - 减少技能冷却计时器 `zf_perkTimer`。
    - 更新HUD，显示技能是否准备就绪。
    - 如果幸存者处于 `ZFCondIntimidated` 状态，则应用 `ZF_DAZE_DEFEND` 的防御惩罚。

- **`perk_OnPlayerSpawn`**:
    - 重置 `zf_perkTimer`，确保每次重生时技能都处于可用状态。

## 推荐职业

- **任何职业**: 咆哮僵尸的能力不依赖于特定的基础职业，其强大的控场能力适用于任何情况，尤其是在狭窄空间或需要突破幸存者防线时。# 火焰僵尸 (Scorching)

## 简介

> *“孙哥我火了!”*

**火焰僵尸** 是一个持续对周围造成威胁的职业。它自身免疫火焰伤害，并且能将火焰带给敌人。无论是通过近战攻击还是简单的身体接触，它都能点燃幸存者，制造混乱并造成持续伤害。

## 详细说明

火焰僵尸的核心机制围绕着 **火焰**。

- **被动能力**:
    - **火焰免疫**: 你不会受到任何来源的火焰伤害（包括来自幸存者火焰兵的攻击和地图环境的火焰）。
    - **引火烧身**: 你会一直处于燃烧状态（除非在水中），这使得任何接触到你的幸存者都会被点燃。
    - **近战点燃**: 你的近战攻击会点燃被击中的幸存者。
- **属性调整**:
    - **攻击力**: 你的近战攻击力降低 **50%**。但是，由火焰造成的后续燃烧伤害不受此影响。
    - **移动速度**: 你的移动速度增加 **50%**。
- **限制**:
    - 你无法使用 **原子能饮料 (Bonk! Atomic Punch)**。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_SCORCHING_ATTACK` | `-50` | 使用该职业时的近战攻击力惩罚百分比。 |
| `ZF_SCORCHING_SPEED` | `50` | 使用该职业时获得的速度加成百分比。 |

## 事件处理逻辑

- **`updateClientPermStats`**:
    - 在 `else if(validZom(client))` 分支下，为火焰僵尸应用永久的速度加成 (`ZF_SCORCHING_SPEED`)。

- **`perk_OnTakeDamage`**:
    - **对幸存者**: 当火焰僵尸用近战攻击幸存者时，除了基础的攻击力惩罚外，还会通过 `TF2_IgnitePlayer` 点燃目标。对于非近战的火焰伤害（例如，由自己身上的火焰蔓延造成的燃烧），则不应用攻击力惩罚。
    - **对自己**: 当火焰僵尸受到伤害时，如果伤害类型是火焰 (`attackWasFire`)，则该次伤害无效（伤害值设为0.0），实现火焰免疫。

- **`perk_OnTouch`**:
    - 当火焰僵尸（`toucher`）接触到幸存者（`touchee`）时，如果火焰僵尸自身处于燃烧状态，则会点燃幸存者。

- **`OnPlayerRunCmd`**:
    - 检查玩家是否为火焰僵尸，并且是侦察兵职业。
    - 如果玩家试图使用原子能饮料（`IN_ATTACK` + `ZFWEAP_BONK`），则阻止该次攻击输入，并向玩家显示提示信息。

- **`perk_OnPlayerSpawn`**:
    - 在玩家重生时，通过 `TF2_IgnitePlayer(client, client)` 将自己点燃，以确保“引火烧身”的被动能力生效。

## 推荐职业

- **侦察兵**: 侦察兵的高机动性与火焰僵尸的速度加成相结合，可以使其快速接近幸存者，利用身体接触和快速的近战攻击来点燃多个目标，最大化其骚扰和持续伤害的能力。# 吐酸僵尸 (Sick)

## 简介

> *古怪的嚎叫声*

**吐酸僵尸** 是一个典型的区域封锁职业，类似于《求生之路》中的Spitter。它以大幅牺牲自身防御力为代价，换来了吐出大片腐蚀性酸液的能力，能够对占据关键位置的幸存者造成持续性范围伤害。

## 详细说明

吐酸僵尸的核心能力是 **吐酸**。

- **激活方式**: 按下 `医生语音` 键 (默认 `V` 键) 来激活。
- **激活条件**:
    - 不能处于隐身状态。
- **能力效果**:
    - 你会快速连续吐出 **5** 颗酸液弹。
    - 酸液弹在碰到任何表面后，会形成一滩酸液。
    - 幸存者站在酸液上会持续受到伤害。
- **持续与冷却**:
    - 酸液滩的持续时间为 **15秒**，或者在你死亡后会立即消失。
    - 技能的冷却时间也是 **15秒**。
- **属性调整**:
    - **防御力**: 你的防御力大幅降低 **75%**，使你变得非常脆弱。

**注意**: 职业的详细说明中提到“酸液造成的伤害与你和酸液之间的距离成正比”，但根据代码实现，酸液滩对其半径范围内的所有幸存者造成的伤害是 **固定值**，并不会随距离变化。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_SICK_MAX_ITEMS` | `5` | 一次可以激活的酸液弹/酸液滩的最大数量。 |
| `ZF_SICK_DEFEND` | `-75` | 使用该职业时的防御力惩罚百分比。 |
| `ZF_SICK_DAMAGE` | `15` | 酸液滩每秒对范围内的幸存者造成的基础伤害。 |
| `ZF_SICK_DAMAGE_RADIUS` | `150` | 酸液滩造成伤害的半径范围。 |
| `ZF_SICK_TIMER` | `15` | 技能的冷却时间（秒），同时也是酸液滩的持续时间。 |

## 事件处理逻辑

- **`updateClientPermStats`**:
    - 在 `else if(validZom(client))` 分支下，为吐酸僵尸应用永久的防御力惩罚 (`ZF_SICK_DEFEND`)。

- **`perk_OnCallForMedic`**:
    - 检查玩家是否为吐酸僵尸、技能是否冷却完毕以及是否未隐身。
    - 若满足条件，则设置 `zf_perkTimer` 为 `ZF_SICK_TIMER`。
    - 通过创建多个 `perk_tSickSpit` 定时器，连续发射 `ZF_SICK_MAX_ITEMS` 数量的酸液弹。

- **`perk_tSickSpit` (定时器回调)**:
    - 实际执行投掷酸液弹 (`doItemThrow`) 的逻辑。

- **`perk_OnGameFrame` (每帧执行)**:
    - 持续检测飞行中的酸液弹。
    - 当酸液弹发生碰撞时 (`doItemCollide`)，移除飞行物并在碰撞点生成一个静态的酸液滩实体 (`doItemImpact`)。

- **`updateCondStats` (每秒执行)**:
    - 减少技能冷却计时器 `zf_perkTimer`。当计时器归零时，移除所有现存的酸液滩。
    - 对每一个激活的酸液滩，使用 `applyDamageRadial` 对其半径内的幸存者造成伤害。

- **`perk_OnPlayerSpawn` / `perk_OnPlayerDeath`**:
    - 重置技能计时器并移除所有酸液滩，确保状态正确。

## 推荐职业

- **侦察兵**: 侦察兵的机动性可以帮助脆弱的吐酸僵尸快速到达有利位置，从意想不到的角度吐出酸液，然后迅速撤离，最大化其区域封锁的效果。# 招魂僵尸 (Swarming)

## 简介

> *“嘿!速生不是投票关掉了吗?”*

**招魂僵尸** 是一个强大的辅助型职业，它能极大地增强僵尸团队的持续作战能力。通过其光环效果，它能让被击杀的队友几乎瞬间重生，形成一波又一波无穷无尽的尸潮，压垮幸存者的防线。

## 详细说明

招魂僵尸的核心机制是 **快速重生光环**。

- **被动能力**:
    - **重生光环**: 任何在你附近被击杀的僵尸队友，都会在极短的时间内（**0.5秒**）立即重生。
    - **自身快速重生**: 你自己死亡后，也会以 **0.5秒** 的速度快速重生。
- **属性调整**:
    - **攻击力**: 你的攻击力降低 **20%**。
    - **防御力**: 你的防御力降低 **20%**。
    - **移动速度**: 你的移动速度增加 **50%**。
- **视觉效果**:
    - 你身边会环绕着苍蝇特效，以标识你的光环范围。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_SWARMING_COMBAT` | `-20` | 使用该职业时的攻击力和防御力惩罚百分比。 |
| `ZF_SWARMING_RADIUSSQ` | `(400 * 400)` | 重生光环的半径（平方）。 |
| `ZF_SWARMING_SPEED` | `50` | 使用该职业时获得的速度加成百分比。 |
| `Float:ZF_SWARMING_RESPAWNTIME` | `0.5` | 快速重生所需的时间（秒）。 |

## 事件处理逻辑

- **`updateClientPermStats`**:
    - 在 `else if(validZom(client))` 分支下，为招魂僵尸应用永久的攻防惩罚 (`ZF_SWARMING_COMBAT`) 和速度加成 (`ZF_SWARMING_SPEED`)。

- **`updateClientPermEffects`**:
    - 为招魂僵尸创建并附加苍蝇光环特效 (`ZFPART_AURAFLIES`)。

- **`perk_OnPlayerDeath`**:
    - **对自己**: 当招魂僵尸自己被击杀时，会创建一个 `perk_tSpawnClient` 定时器，在 `ZF_SWARMING_RESPAWNTIME` (0.5秒) 后将自己重生。
    - **对队友**: 当任何其他僵尸被击杀时，代码会遍历所有存活的招魂僵尸。如果被击杀的僵尸在某个招魂僵尸的 `ZF_SWARMING_RADIUSSQ` 范围内，则同样会为该被击杀的僵尸创建一个0.5秒的重生定时器。

- **`perk_tSpawnClient` (定时器回调)**:
    - 实际执行重生 (`spawnClient`) 的逻辑。

## 推荐职业

- **侦察兵**: 侦察兵的高机动性与招魂僵尸的速度加成相结合，可以使其灵活地穿梭于战场，确保其重生光环能够覆盖到尽可能多的队友，尤其是在僵尸集体冲锋时，最大化团队的压制力。# 吐油僵尸 (Tarred)

## 简介

> *古怪的嚎叫声*

**吐油僵尸** 是一名出色的控场型职业，擅长使用黏稠的焦油来限制幸存者的行动。它既可以通过近战攻击直接削弱单个目标，也可以吐出焦油弹在关键位置制造大片减速区域。

## 详细说明

吐油僵尸的核心能力是 **焦油**。

- **主动能力 (吐油)**:
    - **激活方式**: 按下 `医生语音` 键 (默认 `V` 键) 来激活。
    - **激活条件**: 不能处于隐身状态。
    - **能力效果**: 你会快速连续吐出 **5** 颗焦油弹。焦油弹在碰到任何表面后，会形成一滩焦油。
    - **焦油滩效果**: 站在焦油滩上的幸存者，其移动速度会降低 **30%**，攻击速度降低 **20%**。
    - **持续与冷却**: 焦油滩的持续时间为 **30秒**，或者在你死亡后会立即消失。技能的冷却时间也是 **30秒**。

- **被动能力 (近战附着)**:
    - 你的近战攻击会直接对命中的幸存者施加焦油效果，降低其移动速度 **40%** 和攻击速度 **20%**，持续 **10秒**。

- **视觉效果**:
    - 你的模型颜色会变得更深，以作区分。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_TARRED_MAX_ITEMS` | `5` | 一次可以激活的焦油弹/焦油滩的最大数量。 |
| `ZF_TARRED_DURATION_MELEE` | `10` | 近战攻击造成的减速效果的持续时间（秒）。 |
| `ZF_TARRED_DURATION_SLICK` | `30` | 焦油滩造成的减速效果的持续时间（秒）。 |
| `ZF_TARRED_ROF` | `-20` | 焦油效果对幸存者造成的攻击速度惩罚百分比。 |
| `ZF_TARRED_SPEED_MELEE` | `-40` | 近战攻击对幸存者造成的移动速度惩罚百分比。 |
| `ZF_TARRED_SPEED_SLICK` | `-30` | 焦油滩对幸存者造成的移动速度惩罚百分比。 |
| `ZF_TARRED_SPEED_LIMIT` | `-100` | 速度惩罚的上限阈值。 |
| `ZF_TARRED_TIMER` | `30` | 技能的冷却时间（秒）。 |
| `ZF_TARRED_RADIUS` | `75` | 焦油滩造成减速效果的半径范围。 |

## 事件处理逻辑

- **`perk_OnCallForMedic`**:
    - 检查玩家是否为吐油僵尸、技能是否冷却完毕以及是否未隐身。
    - 若满足条件，则通过创建多个 `perk_tTarredSpit` 定时器，连续发射焦油弹。

- **`perk_OnGameFrame`**:
    - 处理飞行中的焦油弹，当其碰撞后生成静态的焦油滩实体。

- **`updateCondStats`**:
    - 减少技能冷却计时器 `zf_perkTimer`。
    - 对站在焦油滩范围内的幸存者触发一次极低伤害的范围攻击，此攻击的主要目的是为了在 `perk_OnTakeDamage` 中施加减益效果。
    - 将吐油僵尸的玩家模型颜色变暗。

- **`perk_OnTakeDamage`**:
    - **近战命中时**: 当吐油僵尸近战击中幸存者时，对幸存者施加 `ZF_TARRED_SPEED_MELEE` 和 `ZF_TARRED_ROF` 的减益。
    - **焦油滩触发时**: 当幸存者受到来自焦油滩的伤害时，对其施加 `ZF_TARRED_SPEED_SLICK` 和 `ZF_TARRED_ROF` 的减益。

- **`perk_OnPlayerSpawn` / `perk_OnPlayerDeath`**:
    - 重置技能计时器并移除所有焦油滩，确保状态正确。

## 推荐职业

- **侦察兵**: 侦察兵的机动性可以帮助吐油僵尸快速接近幸存者进行近战减速，或者找到合适的角度发射焦油弹，有效地分割战场。# 潜行僵尸 (Thieving)

## 简介

> *“这可比拳头好使多了!”*

**潜行僵尸** 是一个极具骚扰性和破坏性的职业。它以大幅削弱自身直接伤害为代价，换来了通过近战攻击偷取幸存者宝贵资源的能力。更致命的是，它有机会直接偷走幸存者的主武器并据为己有。

## 详细说明

潜行僵尸的核心机制是 **偷窃**。

- **被动能力 (资源偷窃)**:
    - 你的近战攻击会根据幸存者当前手持的武器类型，偷取不同的资源：
        - **主武器 (Slot 0)**: 偷取目标 **30%** 的备用弹药。
        - **副武器 (Slot 1)**: 偷取目标 **50%** 的Ubercharge能量和 **30%** 的备用弹药。
        - **近战武器 (Slot 2)**: 偷取目标 **100** 金属。
- **被动能力 (武器偷窃)**:
    - 当你通过攻击将一个幸存者的主武器备用弹药偷光时，你会直接 **偷走** 他们的主武器。
    - 你会获得一把与你当前职业相对应的主武器，并带有少量弹药。
    - 你偷来的武器弹药会随时间快速消耗，弹药耗尽后武器会自动消失。
- **属性调整**:
    - **攻击力**: 你的近战攻击力大幅降低 **66%**。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_THIEVING_ATTACK` | `-66` | 使用该职业时的近战攻击力惩罚百分比。 |
| `Float:ZF_THIEVING_AMMOPCT` | `0.30` | 每次近战攻击偷取的备用弹药百分比。 |
| `ZF_THIEVING_METAL` | `100` | 每次近战攻击偷取的金属量。 |
| `Float:ZF_THIEVING_UBERPCT` | `0.50` | 每次近战攻击偷取的Ubercharge百分比。 |

## 事件处理逻辑

- **`updateClientPermStats`**:
    - 为潜行僵尸应用永久的攻击力惩罚 (`ZF_THIEVING_ATTACK`)。

- **`perk_OnTakeDamage`**:
    - 当潜行僵尸近战攻击幸存者时触发。
    - 根据幸存者当前武器槽位，调用 `subResAmmoPct`, `subUber`, `subMetal` 来减少相应资源。
    - 如果主武器的备用弹药被偷光 (`getResAmmoPct == 0.0`)，则调用 `doThievingSteal`。

- **`doThievingSteal(attacker, victim, slot)`**:
    - **受害者**: 移除其主武器 (`stripWeaponSlot`) 并强制切换到近战武器。
    - **攻击者**: 如果攻击者手上没有偷来的武器 (`zf_perkState == 0`)，则为其生成一把对应职业的主武器，并设置少量初始弹药 (`zf_perkState`)。

- **`perk_OnGameFrame`**:
    - 如果潜行僵尸拥有偷来的武器 (`zf_perkState > 0`)，则调用 `doThievingLimit`。

- **`doThievingLimit(client)`**:
    - 持续减少偷来武器的弹药。
    - 当弹药耗尽时 (`zf_perkState == 0`)，移除该武器。

- **`perk_OnPlayerSpawn`**:
    - 重置 `zf_perkState`，确保重生后没有偷来的武器。

## 推荐职业

- **任何职业**: 潜行僵尸的能力不依赖于特定的基础职业。它的核心玩法在于骚扰和削弱幸存者的战斗力，任何职业都可以有效地执行这一任务。# 剧毒僵尸 (Toxic)

## 简介

> *“记住这个职业:Toxic.”*

**剧毒僵尸** 是一个通过多种方式施加持续性伤害的职业。它的直接攻击力几乎可以忽略不计，但无论是主动攻击、被动反伤还是静止不动时散发的毒气，都能让幸存者陷入中毒状态，持续流失生命。

## 详细说明

剧毒僵尸的核心机制是 **中毒**。

- **被动能力 (接触中毒)**:
    - 当你用 **近战武器** 击中幸存者时，目标会中毒，持续 **10秒**。
    - 当你被幸存者的 **近战武器** 攻击时，攻击你的幸存者也会中毒，持续 **10秒**。
- **被动能力 (剧毒光环)**:
    - 当你 **静止不动** 且 **未隐身** 时，你会向周围半径 **400** 单位内的幸存者持续施加中毒伤害。
- **属性调整**:
    - **攻击力**: 你的近战攻击力大幅降低 **90%**。
- **视觉效果**:
    - 你的模型颜色会变为绿色，以作区分。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_TOXIC_ATTACK` | `-90` | 使用该职业时的近战攻击力惩罚百分比。 |
| `ZF_TOXIC_DURATION_POISON` | `10` | 中毒效果的持续时间（秒）。 |
| `ZF_TOXIC_DAMAGE_PASSIVE` | `5` | 剧毒光环每秒对范围内的幸存者造成的伤害。 |
| `ZF_TOXIC_RADIUSSQ` | `(400 * 400)` | 剧毒光环的半径（平方）。 |
| `ZF_POISON_DAMAGE` | `7` | 中毒状态下，幸存者每秒受到的伤害。 |

## 事件处理逻辑

- **`updateClientPermStats`**:
    - 为剧毒僵尸应用永久的攻击力惩罚 (`ZF_TOXIC_ATTACK`)。

- **`updateCondStats`**:
    - **剧毒光环**: 检查剧毒僵尸是否静止不动且未隐身。如果是，则对光环范围内的所有幸存者施加 `ZF_TOXIC_DAMAGE_PASSIVE` 的中毒伤害 (`SDKHooks_TakeDamage` with `ZF_DMGTYPE_POISON`)。
    - **视觉效果**: 将剧毒僵尸的玩家模型颜色染成绿色 (`fxSetClientColor`)。

- **`perk_OnTakeDamage`**:
    - **主动攻击**: 当剧毒僵尸用近战攻击幸存者时，为幸存者添加 `ZFCondPoisoned` 状态，持续 `ZF_TOXIC_DURATION_POISON` 秒。
    - **被动反伤**: 当幸存者用近战攻击剧毒僵尸时，同样为该幸存者添加中毒状态。

- **`perk_OnPeriodic` (每秒执行)**:
    - 检查所有玩家的中毒状态 (`ZFCondPoisoned`)。如果玩家中毒，则每秒对其造成 `ZF_POISON_DAMAGE` 的伤害。

## 推荐职业

- **侦察兵、间谍**: 这两个职业的高机动性或隐身能力，可以帮助剧毒僵尸更容易地接近幸存者并施加中毒效果，或者安全地找到一个位置静止不动，利用剧毒光环进行区域伤害。# 吸血僵尸 (Vampiric)

## 简介

> *“不靠近你,怎么把你给揍扁呢.”*

**吸血僵尸** 是一个非常强大的前线近战职业。它拥有出色的自我恢复能力，不仅能通过被动效果持续回血，还能在近战攻击命中时将造成的伤害转化为自己的生命值，使其在缠斗中极难被杀死。

## 详细说明

吸血僵尸的核心机制是 **生命偷取** 和 **生命恢复**。

- **被动能力 (吸血)**:
    - 当你用 **近战武器** 击中幸存者时，你会恢复等同于你所造成伤害的生命值。这意味着你造成的伤害越高，恢复的生命就越多。
- **被动能力 (生命恢复)**:
    - 你会获得一个持续的生命恢复效果，每秒恢复 **15** 点生命值。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `Float:ZF_VAMPIRIC_HEALTHPCT` | `1.00` | 近战攻击命中时，将造成伤害的100%转化为生命值。 |
| `ZF_VAMPIRIC_REGEN` | `15` | 每秒被动恢复的生命值。 |

## 事件处理逻辑

- **`updateCondStats` (每秒执行)**:
    - 在 `else if(validZom(client))` 分支下，为吸血僵尸应用 `addHealth` 效果，每秒恢复 `ZF_VAMPIRIC_REGEN` 点生命值。

- **`perk_OnTakeDamagePost` (伤害事件后处理)**:
    - 检查攻击者是否为吸血僵尸，受害者是否为幸存者，并且攻击方式是否为近战。
    - 如果条件满足，则根据实际造成的伤害 (`damage`)，按 `ZF_VAMPIRIC_HEALTHPCT` 的比例为吸血僵尸恢复生命值。
    - 同时播放血液飞溅的视觉特效 (`fxBloodBurst`)。

## 推荐职业

- **侦察兵、机枪手**:
    - **侦察兵**: 拥有高攻速，可以快速进行多次近战攻击，从而频繁触发吸血效果，保持高血量。
    - **机枪手**: 拥有高基础伤害，虽然攻速较慢，但每一次命中都能恢复大量生命值，在与幸存者正面对决时具有极强的生存能力。# 复仇僵尸 (Vindictive)

## 简介

> *“我想这是死去的智者.”*

**复仇僵尸** 是一个典型的“滚雪球”型成长职业。它的初始能力并不出众，但每当它参与击杀或获得助攻时，它的攻击力和防御力都会获得永久性的提升，使其在战局后期变得越来越难以对付。

## 详细说明

复仇僵尸的核心机制是 **属性成长**。

- **被动能力**:
    - **击杀奖励**: 每当你 **击杀** 一名幸存者时，你的攻击力会永久增加 **20%**，防御力永久增加 **10%**。
    - **助攻奖励**: 每当你 **助攻** 击杀一名幸存者时，你的攻击力会永久增加 **10%**，防御力永久增加 **5%**。

这些加成是永久性的，并且会持续累积直到回合结束。

## 参数

| 参数名 | 值 | 描述 |
| :--- | :--- | :--- |
| `ZF_VINDICTIVE_ATTACK` | `20` | 每次击杀获得的永久攻击力加成百分比。 |
| `ZF_VINDICTIVE_ATTACK_ASSIST` | `10` | 每次助攻获得的永久攻击力加成百分比。 |
| `ZF_VINDICTIVE_DEFEND` | `10` | 每次击杀获得的永久防御力加成百分比。 |
| `ZF_VINDICTIVE_DEFEND_ASSIST` | `5` | 每次助攻获得的永久防御力加成百分比。 |

## 事件处理逻辑

- **`perk_OnPlayerDeath`**:
    - 当一个幸存者被击杀时，检查 `killer` (击杀者) 和 `assist` (助攻者)。
    - **如果击杀者是复仇僵尸**: 调用 `addStat` 为其增加 `ZF_VINDICTIVE_ATTACK` 和 `ZF_VINDICTIVE_DEFEND` 的永久属性 (`ZFStatTypePerm`)。
    - **如果助攻者是复仇僵尸**: 调用 `addStat` 为其增加 `ZF_VINDICTIVE_ATTACK_ASSIST` 和 `ZF_VINDICTIVE_DEFEND_ASSIST` 的永久属性。

- **`updateClientPermStats`**:
    - 这个职业没有在 `updateClientPermStats` 中设置初始的永久属性，所有的属性加成都来自于 `perk_OnPlayerDeath` 事件。

## 推荐职业

- **任何职业**: 复仇僵尸的能力不依赖于特定的基础职业。它的成长性使其在任何职业手中都能发挥作用。关键在于积极参与战斗，尽可能多地获得击杀和助攻，以快速累积属性优势。
这些职业中，有哪些事件处理函数和函数，列出来给我看看