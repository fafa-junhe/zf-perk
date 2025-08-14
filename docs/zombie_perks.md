# 僵尸职业 (Zombie Perks)

本章节详细解析了所有僵尸职业。

### 1. Alpha - [`src/sourcepawn/perks/zombie/AlphaPerk.inc`](src/sourcepawn/perks/zombie/AlphaPerk.inc)

*   **职业名称**: `Alpha`
*   **职业类型**: 僵尸
*   **介绍**: `Alpha`（头狼）是一个领导型僵尸，通过击杀或助攻将幸存者转化为自己的随从。它能从附近的僵尸（尤其是随从）身上获得光环增益，并能主动召唤随从到自己身边。
*   **详细介绍**: 该职业的核心机制是建立自己的僵尸小队。当 `Alpha` 参与击杀一名幸存者后，该幸存者重生为僵尸时会成为 `Alpha` 的随从（通过 `zf_perkAlphaMaster` 数组记录）。`Alpha` 会从周围的普通僵尸和自己的随从获得攻击力和生命恢复光环，其中随从提供的增益远高于普通僵尸。此外，`Alpha` 可以按 `E` 键发动一个有冷却的主动技能，将最多5名随从瞬间传送到自己身边，用于集结部队或发起突袭。
*   **参数 (`Defines`)**:
    *   `ZF_ALPHA_RADIUSSQ`: `(500.0 * 500.0)` - 光环的作用半径（平方）。
    *   `ZF_ALPHA_ATTACK`: `10` - 从每个普通僵尸获得的光环攻击力。
    *   `ZF_ALPHA_PERM_ATTACK`: `15` - 永久攻击力加成。
    *   `ZF_ALPHA_ATTACK_MINION`: `10` - 从每个随从获得的光环攻击力。
    *   `ZF_ALPHA_REGEN`: `4` - 从每个普通僵尸获得的光环生命恢复。
    *   `ZF_ALPHA_REGEN_MINION`: `12` - 从每个随从获得的光环生命恢复。
    *   `ZF_ALPHA_SUMMON_LIMIT`: `5` - 一次最多召唤的随从数量。
    *   `ZF_ALPHA_TIMER_MINION`: `15` - 召唤技能的冷却时间（秒）。
*   **逻辑处理**:
    1.  **随从转化**: [`onKill`](src/sourcepawn/perks/zombie/AlphaPerk.inc:86) 和 [`onAssistKill`](src/sourcepawn/perks/zombie/AlphaPerk.inc:98) 事件在 `Alpha` 参与击杀幸存者后，将被击杀者的 `zf_perkAlphaMaster` 设置为 `Alpha` 的 `client id`。
    2.  **光环增益**: [`updateCondStats`](src/sourcepawn/perks/zombie/AlphaPerk.inc:187) 每帧遍历所有僵尸，根据其是否为 `Alpha` 的随从以及是否在光环范围内，累加攻击力和生命恢复增益。
    3.  **主动召唤**: [`onCallForMedic`](src/sourcepawn/perks/zombie/AlphaPerk.inc:117) (按`E`键) 触发召唤技能。它会检查冷却时间，然后调用 [`doAlphaSummon`](src/sourcepawn/perks/zombie/AlphaPerk.inc:146) 函数，该函数会随机选择最多5名随从，将他们传送到 `Alpha` 身边。
    4.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/AlphaPerk.inc:228) 每秒减少召唤技能的冷却计时器。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久攻击力加成。
    *   `VTABLE_ON_KILL` / `VTABLE_ON_ASSIST_KILL`: 将被击杀的幸存者转化为随从。
    *   `VTABLE_UPDATE_COND_STATS`: 应用光环效果并更新HUD。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发召唤随从的技能。
    *   `VTABLE_ON_PERIODIC`: 管理召唤技能的冷却。

### 2. Charger - [`src/sourcepawn/perks/zombie/ChargerPerk.inc`](src/sourcepawn/perks/zombie/ChargerPerk.inc)

*   **职业名称**: `Charger`
*   **职业类型**: 僵尸
*   **介绍**: `Charger`（冲锋者）是一个坦克型僵尸，以牺牲部分攻击和速度为代价换取高额防御力。其核心技能是发动一次势不可挡的冲锋，抓住一名幸存者并将其带走，直到撞到墙上对双方造成巨大伤害。
*   **详细介绍**: 该职业拥有永久的防御加成和攻击/移速减益。通过按 `E` 键，`Charger` 会在短暂准备后向前猛冲。冲锋期间，它会抓住第一个接触到的幸存者，并带着他/她继续前行。冲锋有最大持续时间，但如果提前撞到墙壁，冲锋会立即结束。撞墙时，`Charger` 和被抓住的幸存者都会受到大量伤害并被眩晕。这是一个高风险、高回报的强力开团技能。
*   **参数 (`Defines`)**:
    *   `CHARGER_DEF_BONUS`: `25` - 永久防御力加成。
    *   `CHARGER_ATT_PENALTY`: `-20` - 永久攻击力惩罚。
    *   `CHARGER_SPEED_PENALTY`: `-20` - 永久移动速度惩罚。
    *   `CHARGER_CHARGE_FORCE`: `1000.0` - 地面冲锋的力量。
    *   `CHARGER_CHARGE_COOLDOWN`: `30` - 冲锋技能的冷却时间（秒）。
    *   `CHARGER_WALL_DAMAGE`: `80.0` - 撞墙时对双方造成的伤害。
    *   `CHARGER_STUN_DURATION`: `2.0` - 撞墙后的眩晕时间。
    *   `CHARGER_MAX_DURATION`: `5.0` - 冲锋的最大持续时间。
*   **逻辑处理**:
    1.  **状态机**: 该职业使用一个私有数据 `perk_state` 来管理其复杂的状态：`0` (空闲), `1` (正在冲锋), `2` (抓住幸存者冲锋), `3` (准备冲锋)。
    2.  **技能发动**: [`onCallForMedic`](src/sourcepawn/perks/zombie/ChargerPerk.inc:233) (按`E`键) 将状态切换为 `3` (准备中)，并在短暂延迟后由 [`onPeriodic`](src/sourcepawn/perks/zombie/ChargerPerk.inc:361) 调用 [`ChargerPerkFdoChargeStart`](src/sourcepawn/perks/zombie/ChargerPerk.inc:198) 正式开始冲锋。
    3.  **抓住幸存者**: 冲锋期间，[`onTouch`](src/sourcepawn/perks/zombie/ChargerPerk.inc:254) 事件检测与幸存者的碰撞。一旦碰到，状态切换为 `2`，并将幸存者“携带”在身前。
    4.  **持续移动与携带**: [`onPlayerRunCmd`](src/sourcepawn/perks/zombie/ChargerPerk.inc:287) 在冲锋状态下接管玩家移动，持续施加向前的力。如果携带了幸存者，也会强制更新幸存者的位置。
    5.  **撞墙检测**: 同样在 [`onPlayerRunCmd`](src/sourcepawn/perks/zombie/ChargerPerk.inc:287) 中，使用 `TR_TraceHullFilterEx` 向前进行体积追踪，以检测是否即将撞上墙壁等障碍物。
    6.  **冲锋结束**: 无论是撞墙、超时、死亡还是更换职业，最终都会调用 [`doChargeEnd`](src/sourcepawn/perks/zombie/ChargerPerk.inc:427) 方法。此方法负责清理所有状态（移除TF2 Condition，重置速度等），并根据是否撞墙来结算伤害和眩晕效果。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 开始冲锋准备。
    *   `VTABLE_ON_TOUCH`: 在冲锋时抓住幸存者。
    *   `VTABLE_ON_PLAYER_RUN_CMD`: 处理冲锋时的持续移动和撞墙检测。
    *   `VTABLE_ON_PERIODIC`: 管理冷却计时器、冲锋准备计时和冲锋超时。
    *   `VTABLE_DO_CHARGE_END` (自定义VTable): 统一处理冲锋结束时的所有逻辑。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 确保在任何情况下都能安全地结束冲锋。


### 3. Combustible - [`src/sourcepawn/perks/zombie/CombustiblePerk.inc`](src/sourcepawn/perks/zombie/CombustiblePerk.inc)

*   **职业名称**: `Combustible`
*   **职业类型**: 僵尸
*   **介绍**: `Combustible`（自爆者）是一个极其脆弱但具有巨大威胁的职业。它会大幅降低自身防御力，但在被幸存者的非近战攻击杀死时，会引发一次威力巨大的爆炸，并快速重生。
*   **详细介绍**: 该职业拥有全游戏最低的防御力，使其非常容易被杀死。然而，它的核心能力在于死亡惩罚：如果被幸存者的枪械、爆炸物等远程攻击杀死，它会在死亡瞬间以自身为中心制造一次大范围、高伤害的爆炸。为了平衡这种强大的能力，该效果每条命只能触发一次，并且对近战攻击无效。此外，作为间谍时，该职业会禁用隐形手表。
*   **参数 (`Defines`)**:
    *   `ZF_COMBUSTIBLE_DAMAGE`: `120` - 爆炸伤害。
    *   `ZF_COMBUSTIBLE_DAMAGE_HEAVY`: `200` - 作为机枪手时的爆炸伤害。
    *   `ZF_COMBUSTIBLE_DEFEND`: `-200` - 永久防御力惩罚。
    *   `ZF_COMBUSTIBLE_RADIUS`: `300` - 爆炸半径。
    *   `ZF_COMBUSTIBLE_RESPAWNTIME`: `4.5` - 爆炸后的重生时间（秒）。
*   **逻辑处理**:
    1.  **永久减防**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/CombustiblePerk.inc:75) 在出生时永久性地大幅降低玩家防御力。
    2.  **死亡爆炸**: [`onDeath`](src/sourcepawn/perks/zombie/CombustiblePerk.inc:79) 是核心逻辑。当玩家死亡时，它会检查几个条件：
        *   本条命是否已经爆炸过（通过 `hasExploded` 私有数据判断）。
        *   击杀者是否为幸存者。
        *   伤害类型是否不包含近战（`DMG_CLUB`）。
    3.  **触发效果**: 如果所有条件满足，则标记为已爆炸，然后使用 `applyDamageRadial` 制造范围伤害，并创建一个 `ZF_COMBUSTIBLE_RESPAWNTIME` 秒的计时器来让玩家提前重生。
    4.  **状态重置与武器禁用**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/CombustiblePerk.inc:114) 在玩家每次出生时，将 `hasExploded` 标记重置为 `false`，并检查如果玩家是间谍，则移除其手表。
    5.  **视觉提示**: [`updateCondStats`](src/sourcepawn/perks/zombie/CombustiblePerk.inc:124) 持续在玩家身上创建一个微小的爆炸粒子效果，以提示其易爆的特性。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 永久降低防御力。
    *   `VTABLE_ON_DEATH`: 检测死亡条件并触发自爆。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置爆炸状态并禁用间谍手表。
    *   `VTABLE_UPDATE_COND_STATS`: 提供视觉提示和HUD状态。


### 4. Corruptor - [`src/sourcepawn/perks/zombie/CorruptorPerk.inc`](src/sourcepawn/perks/zombie/CorruptorPerk.inc)

*   **职业名称**: `Corruptor`
*   **职业类型**: 僵尸
*   **介绍**: `Corruptor`（腐化者）是一个反建筑单位，移动速度稍慢，但对工程师的建筑能造成额外伤害，并能在连续攻击后将其“腐化”，使其暂时为僵尸团队效力。
*   **详细介绍**: 该职业的主要目标是摧毁或转化敌方的防御工事。它被动地对所有建筑造成额外伤害。其核心技能是“腐化”：当 `Corruptor` 连续攻击同一个步哨枪达到指定次数后，它会摧毁原来的步哨枪，并在原地生成一个属于腐化者自己的、同等级的步哨枪。这个被腐化的步哨枪会自动攻击幸存者，但只存在一小段时间便会自毁。
*   **参数 (`Defines`)**:
    *   `CORRUPTOR_SPEED`: `-10` - 永久移动速度惩罚。
    *   `CORRUPTOR_BUILDING_DAMAGE_BONUS`: `50` - 对建筑的额外伤害百分比。
    *   `CORRUPTOR_HIT_COUNT`: `1` - 腐化一个步哨枪所需的连续攻击次数。
    *   `CORRUPTOR_CORRUPTED_LIFETIME`: `15.0` - 被腐化的步哨枪的存在时间（秒）。
*   **逻辑处理**:
    1.  **属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:116) 在出生时应用永久的速度惩罚。
    2.  **伤害与腐化计数**: [`onBuildingTakeDamage`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:228) 是核心。当 `Corruptor` 攻击一个建筑时，此事件触发。
        *   首先，它会施加额外的伤害。
        *   然后，它会检查被攻击的是否为同一个步哨枪。如果是，则增加 `hit_count`；如果不是，则重置计数器并切换目标。
    3.  **触发腐化**: 当 `hit_count` 达到 `CORRUPTOR_HIT_COUNT` 后，调用 [`doCorruptBuilding`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:139)。
    4.  **建筑转化**: [`doCorruptBuilding`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:139) 和它的辅助函数 [`CreateSentry`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:167) 负责转化逻辑。它们会记录原步哨枪的位置和等级，将其销毁，然后在原位创建一个新的、属于 `Corruptor` 的步哨枪，并启动一个自毁计时器。
    5.  **状态重置**: [`updateCondStats`](src/sourcepawn/perks/zombie/CorruptorPerk.inc:265) 会检查距离上次攻击的时间，如果超过1.5秒，则重置腐化计数和目标，防止玩家通过间断攻击无限维持腐化进度。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久速度惩罚。
    *   `VTABLE_ON_BUILDING_TAKE_DAMAGE`: 对建筑造成额外伤害，并处理腐化逻辑。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示腐化进度，并处理腐化计数的超时重置。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置所有腐化相关的状态。


### 5. Geomancer - [`src/sourcepawn/perks/zombie/GeomancerPerk.inc`](src/sourcepawn/perks/zombie/GeomancerPerk.inc)

*   **职业名称**: `Geomancer`
*   **职业类型**: 僵尸
*   **介绍**: `Geomancer`（地卜师）是一个区域控制型僵尸，以牺牲部分移动速度为代价，获得了在远处地面升起石柱的能力，可以阻挡幸存者或将其击飞。
*   **详细介绍**: 该职业的核心技能是按 `E` 键（需要蹲下）在准星指向的地面上升起一根巨大的石柱。石柱在升起过程中会对附近的幸存者造成强大的向上击飞效果。石柱升起后会作为障碍物在场上停留一小段时间，然后再次下沉消失。该技能有冷却时间，并且同一时间只能存在一根石柱。
*   **参数 (`Defines`)**:
    *   `GEOMANCER_SPEED`: `-20` - 永久移动速度惩罚。
    *   `GEOMANCER_COOLDOWN`: `15.0` - 技能冷却时间（秒）。
    *   `GEOMANCER_ROCK_DISTANCE`: `200.0` - 技能施放的最大距离。
    *   `GEOMANCER_ROCK_LIFETIME`: `2.0` - 石柱在升起状态的停留时间。
    *   `GEOMANCER_ROCK_UPWARD_FORCE`: `800.0` - 对幸存者的击飞力量。
*   **逻辑处理**:
    1.  **技能施放**: [`onCallForMedic`](src/sourcepawn/perks/zombie/GeomancerPerk.inc:94) (按`E`键) 触发技能。它会检查冷却、是否蹲下、以及目标点是否有效（在地面上且不太近）。
    2.  **石柱创建**: 如果条件满足，它会在目标地面下方创建一个石柱模型实体，并将其信息（实体索引、目标位置等）存入一个 `ArrayList` 中。
    3.  **动画计时器**: 技能成功后，会启动一个复杂的动画计时器 [`Geomancer_AnimationTimer`](src/sourcepawn/perks/zombie/GeomancerPerk.inc:265)。这个计时器分两个阶段（上升和下沉），通过递归调用 `CreateTimer` 来逐帧更新石柱的位置，从而模拟出平滑的升降动画。
    4.  **击飞效果**: 在上升动画阶段，计时器会检测石柱附近的幸存者，并对他们施加一个强大的向上速度，实现击飞。
    5.  **实体切换与清理**: 在上升动画结束后，为了让石柱能阻挡玩家，会调用 [`Geomancer_SwapToSolidWalls`](src/sourcepawn/perks/zombie/GeomancerPerk.inc:186) 将动画用的非固体模型替换成一个固体的模型。在下沉动画结束后，[`Geomancer_ClearWalls`](src/sourcepawn/perks/zombie/GeomancerPerk.inc:388) 会负责清理所有相关的实体和数据，防止泄露。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久速度惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发石柱技能。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能状态（准备就绪、冷却中、激活中）。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE` / `VTABLE_ON_PLAYER_SPAWN`: 确保在任何情况下都能清理掉已创建的石柱。


### 6. GravityWarper - [`src/sourcepawn/perks/zombie/GravityWarperPerk.inc`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc)

*   **职业名称**: `GravityWarper`
*   **职业类型**: 僵尸
*   **介绍**: `GravityWarper`（重力扭曲者）是一个被动光环型职业，拥有少量防御加成，并能持续扭曲周围幸存者的重力，使其跳跃能力大幅下降。
*   **详细介绍**: 该职业的核心能力是一个永久存在的、以自身为中心的重力扭曲光环。任何进入光环范围的幸存者，其重力都会被设为一个更高的值，这会导致他们的跳跃高度显著降低，难以到达高处或进行“兔跳”。当幸存者离开光环范围后，其重力会恢复正常。这是一个纯粹的区域控制和骚扰型职业。
*   **参数 (`Defines`)**:
    *   `GRAVITY_WARPER_RADIUS`: `600.0` - 重力光环的作用半径。
    *   `GRAVITY_WARPER_MULTIPLIER`: `1.75` - 施加给幸存者的重力倍率。
    *   `GRAVITY_WARPER_DEFEND`: `10` - 永久防御力加成。
*   **逻辑处理**:
    1.  **永久加成与光环特效**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc:129) 应用永久防御加成。[`onPlayerSpawn`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc:81) 在玩家出生时创建一个附着在身上的粒子特效，以可视化光环的存在。
    2.  **重力施加与恢复**: [`onPeriodic`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc:89) 是核心。它每秒执行一次：
        *   遍历所有幸存者，检查他们是否在光环范围内。
        *   对于新进入光环的幸存者，使用 `SetEntityGravity` 增加其重力。
        *   它会维护一个 `ArrayList` (`affected_survivors`) 来记录当前在光环内的幸存者。通过与上一秒的列表对比，找出刚刚离开光环的幸存者，并将其重力恢复为正常值 `1.0`。
    3.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc:149) 和 [`onRemove`](src/sourcepawn/perks/zombie/GravityWarperPerk.inc:133) 确保在玩家死亡或更换职业时，所有曾被光环影响的幸存者的重力都能被正确重置，并且清理光环粒子特效。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御加成。
    *   `VTABLE_ON_PLAYER_SPAWN`: 创建光环的视觉特效。
    *   `VTABLE_ON_PERIODIC`: 核心逻辑，检测范围内的幸存者并修改/重置他们的重力。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理所有效果，确保没有幸存者永久处于高重力状态。


### 7. Horrifying - [`src/sourcepawn/perks/zombie/HorrifyingPerk.inc`](src/sourcepawn/perks/zombie/HorrifyingPerk.inc)

*   **职业名称**: `Horrifying`
*   **职业类型**: 僵尸
*   **介绍**: `Horrifying`（惊骇者）是一个弱化敌人的职业，自身攻击力和攻速较低，但每次近战命中幸存者时，都会对其施加一个持续性的、可叠加的属性削弱debuff。
*   **详细介绍**: 该职业以永久降低自身攻击力和攻击速度为代价，换取了强大的骚扰能力。当它用近战攻击命中一名幸存者时，会触发一个“惊骇”效果，在短时间内大幅降低该幸存者的攻击力、防御力、移动速度和暴击率。这个效果可以叠加，意味着被连续攻击的幸存者会变得越来越脆弱。作为机枪手时，debuff的持续时间更长。
*   **参数 (`Defines`)**:
    *   `HORRIFYING_ATTACK`: `-8` - 永久攻击力惩罚，同时也是施加给幸存者的debuff数值。
    *   `HORRIFYING_DEFEND`: `-15` - 施加给幸存者的防御、速度、暴击debuff数值。
    *   `HORRIFYING_DURATION`: `10` - Debuff的基础持续时间（秒）。
    *   `HORRIFYING_DURATION_HEAVY`: `20` - 作为机枪手时debuff的持续时间。
    *   `HORRIFYING_ROF`: `-25` - 永久攻击速度惩罚。
*   **逻辑处理**:
    1.  **永久惩罚**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/HorrifyingPerk.inc:60) 在出生时应用永久的攻击力和攻速惩罚，并创建一个光环特效。
    2.  **施加Debuff**: [`onTakeDamagePost`](src/sourcepawn/perks/zombie/HorrifyingPerk.inc:72) 是核心。当该职业对幸存者造成近战伤害后，此事件触发。它会使用 `addStatTempStack` 函数为受害者施加一个临时的、可叠加的属性降低效果，涵盖攻击、防御、速度和暴击率。
    3.  **视觉与听觉效果**: 除了属性降低，它还会调用 `TF2_StunPlayer` 并使用 `TF_STUNFLAGS_GHOSTSCARE` 标志，这会在受害者屏幕上触发一个短暂的惊吓效果。
    4.  **状态清理**: [`onRemove`](src/sourcepawn/perks/zombie/HorrifyingPerk.inc:66) 确保在更换职业时移除光环特效。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性惩罚。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 在近战命中后对幸存者施加debuff。
    *   `VTABLE_ON_REMOVE`: 清理光环特效。


### 8. Hunter - [`src/sourcepawn/perks/zombie/HunterPerk.inc`](src/sourcepawn/perks/zombie/HunterPerk.inc)

*   **职业名称**: `Hunter`
*   **职业类型**: 僵尸
*   **介绍**: `Hunter`（猎手）可以放置一个隐藏的重生信标。只要信标存在，`Hunter` 死亡后就能在信标位置快速重生，并获得短暂的攻击力爆发。
*   **详细介绍**: 这是一个策略性很强的职业。玩家可以按 `E` 键在当前位置放置一个几乎看不见的重生信标。这个信标可以被幸存者的攻击或靠近所摧毁。只要信标未被摧毁，当 `Hunter` 死亡时，它不会在默认重生点重生，而是在信标处快速复活，并且在复活后的短时间内获得巨额的攻击力加成，非常适合进行出其不意的伏击。
*   **参数 (`Defines`)**:
    *   `ZF_HUNTER_ATTACK`: `50` - 重生后获得的临时攻击力加成。
    *   `ZF_HUNTER_DURATION`: `10` - 攻击力加成的持续时间（秒）。
    *   `ZF_HUNTER_RADIUSSQ`: `(85 * 85)` - 幸存者靠近并摧毁信标的半径（平方）。
    *   `ZF_HUNTER_RESPAWNTIME`: `5.5` - 在信标处的重生时间（秒）。
*   **逻辑处理**:
    1.  **放置信标**: [`onCallForMedic`](src/sourcepawn/perks/zombie/HunterPerk.inc:152) (按`E`键) 触发。它会在玩家脚下创建一个隐形的、可被伤害的实体作为信标，并用一个粒子效果标记其位置（仅对`Hunter`自己可见）。同时，将 `placed_spawn` 标记为 `true`。
    2.  **死亡与重生**: [`onDeath`](src/sourcepawn/perks/zombie/HunterPerk.inc:137) 事件检查信标是否存在。如果存在，则创建一个计时器，在 `ZF_HUNTER_RESPAWNTIME` 秒后调用 `TF2_RespawnPlayer` 使玩家重生。
    3.  **重生后效果**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/HunterPerk.inc:106) 在玩家重生时触发。它会检查信标是否存在（以此判断是否为信标重生），如果存在，则将玩家传送到信标位置，并使用 `addStatTempStack` 给予临时的攻击力加成。
    4.  **信标被毁**:
        *   [`onPeriodic`](src/sourcepawn/perks/zombie/HunterPerk.inc:188) 每秒检测是否有幸存者进入信标的警戒范围，如果有则摧毁信标。
        *   信标实体被挂钩了 `SDKHook_OnTakeDamage` 到 [`onSpawnPointTakeDamage`](src/sourcepawn/perks/zombie/HunterPerk.inc:241) 函数，使其在受到幸存者伤害时也会被摧毁。
    5.  **状态清理**: [`destroySpawnPoint`](src/sourcepawn/perks/zombie/HunterPerk.inc:209) 是一个统一的清理函数，负责移除信标实体、粒子效果和相关的SDK钩子。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 放置重生信标。
    *   `VTABLE_ON_DEATH`: 如果信标存在，则触发快速重生。
    *   `VTABLE_ON_PLAYER_SPAWN`: 处理在信标处的重生，并给予攻击力加成。
    *   `VTABLE_ON_PERIODIC`: 检测幸存者是否靠近信标。
    *   `VTABLE_ON_REMOVE`: 清理信标。


### 9. Leap - [`src/sourcepawn/perks/zombie/LeapPerk.inc`](src/sourcepawn/perks/zombie/LeapPerk.inc)

*   **职业名称**: `Leap`
*   **职业类型**: 僵尸
*   **介绍**: `Leap`（跳跃者）是一个高机动性职业，永久降低自身攻防，但免疫坠落伤害，并能通过按 `E` 键发动一次强大的前跳。
*   **详细介绍**: 该职业的核心是其主动技能“跳跃”。玩家在地面上按 `E` 键，会向其准星方向以一个约45度的角度猛力跃出，可以跨越很长的距离或跳上高台。该技能有较短的冷却时间。作为侦察兵时，跳跃的力量会更大。此外，该职业被动地完全免疫因高处坠落造成的伤害。
*   **参数 (`Defines`)**:
    *   `ZF_LEAP_COMBAT`: `-20` - 永久攻击力和防御力惩罚。
    *   `ZF_LEAP_COOLDOWN`: `4` - 跳跃技能的冷却时间（秒）。
    *   `ZF_LEAP_FORCE`: `1300.0` - 基础跳跃力量。
    *   `ZF_LEAP_FORCE_SCOUT`: `1900.0` - 作为侦察兵时的跳跃力量。
*   **逻辑处理**:
    1.  **永久惩罚与免疫**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/LeapPerk.inc:93) 在出生时应用永久的攻防惩罚。[`onTakeDamagePost`](src/sourcepawn/perks/zombie/LeapPerk.inc:99) 检测坠落伤害 (`DMG_FALL`) 并将其完全抵消。
    2.  **发动跳跃**: [`onCallForMedic`](src/sourcepawn/perks/zombie/LeapPerk.inc:107) (按`E`键) 触发跳跃。它会检查冷却时间和玩家是否在地面上。
    3.  **力学计算**: 它会获取玩家的视角方向，然后将跳跃力分解为水平和垂直两个分量，最后通过 `TeleportEntity` 将这个计算出的速度向量直接应用到玩家身上，实现跳跃效果。
    4.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/LeapPerk.inc:148) 每秒减少冷却计时器。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久攻防惩罚。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 免疫坠落伤害。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 发动跳跃技能。
    *   `VTABLE_ON_PERIODIC`: 管理跳跃技能的冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能冷却状态。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置技能冷却。


### 10. Magnetic - [`src/sourcepawn/perks/zombie/MagneticPerk.inc`](src/sourcepawn/perks/zombie/MagneticPerk.inc)

*   **职业名称**: `Magnetic`
*   **职业类型**: 僵尸
*   **介绍**: `Magnetic`（磁力者）是一个被动的反建筑职业，它会持续地禁用自身周围大范围内的所有敌方工程师建筑。
*   **详细介绍**: 该职业的核心能力是一个永久存在的、以自身为中心的“电磁脉冲”光环。任何进入光环范围的敌方步哨枪、补给器或传送装置都会被立即禁用（无法运作），并会冒出火花。一旦建筑离开光环范围，或 `Magnetic` 玩家死亡/更换职业，被禁用的建筑会恢复正常功能。这是一个强大的推进和辅助型职业，能有效地瘫痪敌人的防御阵线。
*   **参数 (`Defines`)**:
    *   `ZF_MAGNETIC_RADIUSSQ`: `(800.0 * 800.0)` - 禁用光环的作用半径（平方）。
*   **逻辑处理**:
    1.  **建筑禁用**: [`onPeriodic`](src/sourcepawn/perks/zombie/MagneticPerk.inc:57) 是唯一的核心逻辑。它每秒遍历服务器上的所有实体，检查它们是否为敌方的工程师建筑。
        *   如果建筑在光环范围内，则通过 `SetEntProp` 将其 `m_bDisabled` 属性设为 `1`，使其失效。
        *   如果建筑在光环范围外，则将其 `m_bDisabled` 属性设为 `0`，使其恢复功能。
    2.  **状态清理**: [`onRemove`](src/sourcepawn/perks/zombie/MagneticPerk.inc:95) 确保在玩家更换职业时，所有曾被其禁用的建筑都能恢复正常。它通过遍历所有建筑并调用 `AcceptEntityInput(i, "SetEnabled")` 来实现。
*   **事件处理**:
    *   `VTABLE_ON_PERIODIC`: 核心逻辑，持续检测并禁用/解禁范围内的敌方建筑。
    *   `VTABLE_ON_REMOVE`: 清理所有禁用效果。


### 11. Marked - [`src/sourcepawn/perks/zombie/MarkedPerk.inc`](src/sourcepawn/perks/zombie/MarkedPerk.inc)

*   **职业名称**: `Marked`
*   **职业类型**: 僵尸
*   **介绍**: `Marked`（标记）是一个刺客型僵尸，它会随机标记一名幸存者作为目标。它对被标记的目标能造成巨额额外伤害，但对其他幸存者的伤害会略微降低。
*   **详细介绍**: 该职业的核心机制是“死亡标记”。在回合开始或上一个目标死亡后，系统会自动随机选择一名幸存者进行标记。`Marked` 僵尸对这名被标记的幸存者拥有极高的伤害加成，使其成为一个高优先级的威胁。然而，为了平衡这一点，它在攻击其他非标记幸存者时会受到轻微的伤害惩罚。当被标记的幸存者死亡后，会有一个短暂的冷却时间，然后系统会重新标记一个新的目标。
*   **参数 (`Defines`)**:
    *   `ZF_MARKED_ATTACK_ON_MARK`: `300` - 对被标记目标的攻击力加成300%。
    *   `ZF_MARKED_ATTACK_OFF_MARK`: `-10` - 对非标记目标的攻击力惩罚-10%。
    *   `ZF_MARKED_TIMER`: `10` - 标记新目标的冷却时间（秒）。
    *   `ZF_MARKED_MIN_SURVIVORS`: `0` - 重新选择目标所需的最少幸存者数量。
*   **逻辑处理**:
    1.  **目标选择**: [`doMarkedSelect`](src/sourcepawn/perks/zombie/MarkedPerk.inc:176) 函数负责随机选择一名存活的幸存者作为目标。这个过程在准备阶段结束时 ([`onGraceEnd`](src/sourcepawn/perks/zombie/MarkedPerk.inc:98)) 或上一个目标死亡并经过冷却后触发。
    2.  **伤害修正**: [`onTakeDamage`](src/sourcepawn/perks/zombie/MarkedPerk.inc:102) 事件在僵尸造成伤害时触发。它会判断受害者是否为被标记的目标，如果是，则大幅增加伤害；如果不是，则略微降低伤害。
    3.  **目标死亡处理**: 当被标记的目标死亡时，[`onDeath`](src/sourcepawn/perks/zombie/MarkedPerk.inc:198) 会调用 [`handleTargetDeath`](src/sourcepawn/perks/zombie/MarkedPerk.inc:204)。该函数会启动一个 `ZF_MARKED_TIMER` 秒的计时器，计时结束后会选择一个新的目标。
    4.  **视觉与HUD**: [`updateCondStats`](src/sourcepawn/perks/zombie/MarkedPerk.inc:147) 负责在HUD上显示当前目标或选择新目标的倒计时。[`OnGameFrame`](src/sourcepawn/perks/zombie/MarkedPerk.inc:123) 则负责在被标记的幸存者头顶上绘制一个红色的准星图标。
    5.  **出生逻辑**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/MarkedPerk.inc:114) 会在玩家出生时，如果当前没有目标，则启动一个15秒的计时器来选择第一个目标，以确保所有玩家都已进入游戏。
*   **事件处理**:
    *   `VTABLE_ON_GRACE_END`: 准备阶段结束，选择第一个目标。
    *   `VTABLE_ON_TAKE_DAMAGE`: 根据目标是否被标记来修正造成的伤害。
    *   `VTABLE_ON_PLAYER_SPAWN`: 在没有目标时，延迟选择第一个目标。
    *   `VTABLE_ON_DEATH`: 在被标记的目标死亡时，启动重新选择目标的流程。
    *   `VTABLE_ON_PERIODIC`: 管理选择新目标的冷却计时器。
    *   `VTABLE_UPDATE_COND_STATS`: 更新HUD状态信息。
    *   `VTABLE_ON_GAME_FRAME`: 为被标记的幸存者绘制头顶图标。

### 12. Overlord - [`src/sourcepawn/perks/zombie/OverlordPerk.inc`](src/sourcepawn/perks/zombie/OverlordPerk.inc)

*   **职业名称**: `Overlord`
*   **职业类型**: 僵尸
*   **介绍**: `Overlord`（领主）是一个极其缓慢但防御力极高的单位，它能通过放置“菌毯”来建立一个对僵尸有利的领域。菌毯会自动繁殖，并为范围内的僵尸提供速度和生命恢复光环。
*   **详细介绍**: 该职业以巨大的速度惩罚换取了极高的防御力。其核心能力是按 `E` 键在地面上放置一个“菌毯”实体。菌毯有自己的生命值，可以被幸存者摧毁。只要菌毯存在，它就会为附近的僵尸提供增益。更重要的是，菌毯会周期性地在附近合适的地面上“繁殖”，生成新的菌毯，最多可达8个。这使得 `Overlord` 能够逐渐将一个区域转化为僵尸的据点。
*   **参数 (`Defines`)**:
    *   `OVERLORD_DEFEND`: `25` - 永久防御力加成。
    *   `OVERLORD_SPEED`: `-50` - 永久移动速度惩罚。
    *   `OVERLORD_COOLDOWN`: `30.0` - 放置菌毯的冷却时间。
    *   `OVERLORD_MAX_CREEPS`: `8` - 最大菌毯数量。
    *   `OVERLORD_CREEP_HEALTH`: `400` - 单个菌毯的生命值。
    *   `OVERLORD_CREEP_RADIUS`: `250.0` - 菌毯光环半径。
    *   `OVERLORD_CREEP_LIFETIME`: `60.0` - 菌毯最大存在时间。
    *   `OVERLORD_CREEP_REPRODUCE_TIME`: `5.0` - 菌毯繁殖间隔。
    *   `OVERLORD_BUFF_SPEED`: `20` - 菌毯提供的速度加成。
    *   `OVERLORD_BUFF_REGEN`: `5` - 菌毯提供的生命恢复。
*   **逻辑处理**:
    1.  **放置菌毯**: [`onCallForMedic`](src/sourcepawn/perks/zombie/OverlordPerk.inc:107) (按`E`键) 触发。它会检查冷却和菌毯数量限制，然后在玩家脚下的地面上调用 [`createCreepAt`](src/sourcepawn/perks/zombie/OverlordPerk.inc:147) 创建一个菌毯实体。
    2.  **菌毯繁殖与光环**: [`updateCondStats`](src/sourcepawn/perks/zombie/OverlordPerk.inc:209) 是核心。它每帧执行以下操作：
        *   检查并清理过期的或被摧毁的菌毯。
        *   对于存活的菌毯，如果达到繁殖时间且总数未满，它会尝试在附近寻找合适的地面生成新的菌毯。
        *   遍历所有僵尸，如果僵尸在任何一个菌毯的光环范围内，则为其提供速度和生命恢复增益。
    3.  **菌毯受伤与摧毁**: 菌毯实体被挂钩了 `SDKHook_OnTakeDamage` 到 [`onCreepTakeDamage`](src/sourcepawn/perks/zombie/OverlordPerk.inc:170) 函数。当菌毯受到伤害时，其生命值会减少，归零时则被移除。
    4.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/OverlordPerk.inc:201) 和 [`onRemove`](src/sourcepawn/perks/zombie/OverlordPerk.inc:205) 会调用 [`cleanupCreeps`](src/sourcepawn/perks/zombie/OverlordPerk.inc:183) 函数，确保在玩家死亡或更换职业时，所有由他放置的菌毯都会被正确移除。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的防御加成和速度惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 放置菌毯。
    *   `VTABLE_UPDATE_COND_STATS`: 处理菌毯的繁殖、光环效果和HUD更新。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理所有菌毯。


### 13. Phantasm - [`src/sourcepawn/perks/zombie/PhantasmPerk.inc`](src/sourcepawn/perks/zombie/PhantasmPerk.inc)

*   **职业名称**: `Phantasm`
*   **职业类型**: 僵尸
*   **介绍**: `Phantasm`（幻象）是一个高风险的渗透型职业，以永久降低自身攻防为代价，获得了短暂进入“虚空行走”状态的能力，可以穿墙移动。
*   **详细介绍**: 该职业拥有永久的生命和攻击惩罚。其核心技能是按 `E` 键激活“虚空行走”。在激活后的1秒内，玩家会变得半透明且可以无视碰撞（Noclip），自由穿过墙壁和障碍物。在虚空行走期间，攻击力会获得加成。然而，这个技能风险极高：如果在1秒结束时，玩家停留在墙体或无效空间内，会立即死亡。如果在有效空间内结束，则会受到短暂的眩晕。
*   **参数 (`Defines`)**:
    *   `ZF_PHANTASM_HEALTH_PENALTY`: `-25` - 永久生命值惩罚。
    *   `ZF_PHANTASM_ATTACK_PENALTY`: `-25` - 永久攻击力惩罚。
    *   `ZF_PHANTASM_ATTACK_BONUS`: `25` - 虚空行走期间的攻击力加成。
    *   `ZF_PHANTASM_WALK_DURATION`: `1.0` - 虚空行走持续时间。
    *   `ZF_PHANTASM_STUN_DURATION`: `0.5` - 成功穿墙后的眩晕时间。
    *   `ZF_PHANTASM_COOLDOWN`: `17.0` - 技能冷却时间。
    *   `ZF_PHANTASM_STUCK_DAMAGE`: `10` - 在墙内时受到的持续伤害。
*   **逻辑处理**:
    1.  **技能激活**: [`onCallForMedic`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:101) (按`E`键) 触发。它会检查冷却，然后将玩家的移动类型设为 `MOVETYPE_NOCLIP`，并使其半透明，同时启动一个 `ZF_PHANTASM_WALK_DURATION` 秒的计时器。
    2.  **状态监控**: [`onPeriodic`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:138) 每秒执行：
        *   减少技能冷却。
        *   如果玩家处于虚空行走状态 (`walk_timer > 0.0`)，则调用 [`IsPlayerStuck`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:123) 检查玩家是否在实体内部。如果在，则造成伤害。
        *   当虚空行走计时器结束时，再次检查玩家是否被卡住。如果卡住，则强制自杀；如果没卡住，则恢复正常移动模式并施加眩晕。
    3.  **属性与HUD**: [`updateCondStats`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:170) 在虚空行走期间提供攻击力加成，并根据技能状态（激活、冷却、就绪）更新HUD。
    4.  **状态重置**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:87) 确保玩家在重生时恢复固体状态，并重置所有计时器。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的生命和攻击惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活虚空行走。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却、持续时间和卡墙惩罚。
    *   `VTABLE_UPDATE_COND_STATS`: 应用条件性攻击加成并更新HUD。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置技能状态。


### 14. Rage - [`src/sourcepawn/perks/zombie/RagePerk.inc`](src/sourcepawn/perks/zombie/RagePerk.inc)

*   **职业名称**: `Rage`
*   **职业类型**: 僵尸
*   **介绍**: `Rage`（狂怒）是一个高风险高回报的职业，能在生命值高于80%时激活技能，获得临时的巨额速度加成和过量治疗，但当生命值低于80%时会失去所有效果。
*   **详细介绍**: 该职业的核心技能是按 `E` 键激活“狂怒”状态。激活条件是当前生命值必须高于最大生命值的80%。成功激活后，玩家会立即获得基于当前生命值的过量治疗，并获得一个持续5秒的巨额速度加成。狂怒状态会一直持续，直到玩家的生命值百分比低于80%，届时所有增益效果会立即消失，并且技能进入冷却。
*   **参数 (`Defines`)**:
    *   `ZF_RAGE_COOLDOWN`: `20` - 技能冷却时间（秒）。
    *   `ZF_RAGE_SPEED`: `150` - 激活后的速度加成。
    *   `ZF_RAGE_SPEED_DURATION`: `5` - 速度加成的持续时间。
    *   `ZF_RAGE_HEALTHPCT_TOUSE`: `0.80` - 激活技能所需的最低生命值百分比。
    *   `ZF_RAGE_HEALTHPCT_ONUSE`: `1.1` - 激活时获得的额外生命值倍率（造成过量治疗）。
*   **逻辑处理**:
    1.  **技能激活**: [`onCallForMedic`](src/sourcepawn/perks/zombie/RagePerk.inc:103) (按`E`键) 触发。它检查冷却和生命值条件。如果满足，则将 `perk_state` 设为 `1` (激活)，施加过量治疗和临时的速度加成，并显示红色光环。
    2.  **状态监控**: [`updateCondStats`](src/sourcepawn/perks/zombie/RagePerk.inc:132) 在狂怒状态下持续检查玩家的生命值百分比。一旦低于 `ZF_RAGE_HEALTHPCT_TOUSE`，它会立即将 `perk_state` 设回 `0`，移除光环并清除所有条件性属性，使狂怒效果结束。
    3.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/RagePerk.inc:126) 在技能未激活且在冷却时，每秒减少冷却计时器 `perk_timer`。
    4.  **状态清理**: [`onRemove`](src/sourcepawn/perks/zombie/RagePerk.inc:95) 和 [`onPlayerSpawn`](src/sourcepawn/perks/zombie/RagePerk.inc:155) 确保在更换职业或死亡时，狂怒效果能被正确清除，并重置冷却。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活狂怒状态。
    *   `VTABLE_UPDATE_COND_STATS`: 监控生命值以维持或结束狂怒状态，并更新HUD。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_ON_REMOVE` / `VTABLE_ON_PLAYER_SPAWN`: 清理效果并重置状态。


### 15. Roar - [`src/sourcepawn/perks/zombie/RoarPerk.inc`](src/sourcepawn/perks/zombie/RoarPerk.inc)

*   **职业名称**: `Roar`
*   **职业类型**: 僵尸
*   **介绍**: `Roar`（咆哮）是一个区域控制职业，能通过发动一次强大的咆哮，击退周围的幸存者并暂时削弱他们的防御力。
*   **详细介绍**: 该职业的核心技能是按 `E` 键（必须在地面上）发动“咆哮”。发动时，会以自身为中心，对一个大范围内的所有幸存者造成一次强力的击退，并将他们推向远离自己的方向。同时，被击中的幸存者会受到一个持续性的防御力降低debuff。作为机枪手时，击退力量和debuff持续时间都会大幅增加。
*   **参数 (`Defines`)**:
    *   `ZF_ROAR_COOLDOWN`: `15` - 技能冷却时间。
    *   `ZF_ROAR_DURATION`: `20` - Debuff的基础持续时间。
    *   `ZF_ROAR_DURATION_HEAVY`: `60` - 作为机枪手时的Debuff持续时间。
    *   `ZF_ROAR_FORCE`: `1200.0` - 基础击退力量。
    *   `ZF_ROAR_FORCE_HEAVY`: `3000.0` - 作为机枪手时的击退力量。
    *   `ZF_ROAR_RADIUS`: `450.0` - 咆哮的作用半径。
    *   `ZF_ROAR_DEFEND`: `-25` - 施加给幸存者的防御力debuff。
*   **逻辑处理**:
    1.  **技能发动**: [`onCallForMedic`](src/sourcepawn/perks/zombie/RoarPerk.inc:82) (按`E`键) 触发。它检查冷却和是否在地面。
    2.  **效果施加**: 技能发动后，它会遍历范围内的所有幸存者。对于每一个幸存者：
        *   使用 `addStatTempStack` 施加一个临时的防御力降低debuff。
        *   计算从咆哮者指向幸存者的方向向量，并施加一个强大的速度来击退他们。代码特别处理了向上的分量，以防止玩家被推入地下。
    3.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/RoarPerk.inc:132) 每秒减少冷却计时器 `perk_timer`。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 发动咆哮，击退并削弱范围内的幸存者。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能冷却状态。


### 16. Scorching - [`src/sourcepawn/perks/zombie/ScorchingPerk.inc`](src/sourcepawn/perks/zombie/ScorchingPerk.inc)

*   **职业名称**: `Scorching`
*   **职业类型**: 僵尸
*   **介绍**: `Scorching`（灼烧）是一个高速移动的骚扰型职业，自身永久燃烧，免疫火焰伤害，并且能通过近战攻击和身体接触点燃幸存者。
*   **详细介绍**: 该职业以永久降低攻击力为代价，换取了极高的移动速度。它自身会持续燃烧（除非在水中），并且完全免疫任何来源的火焰伤害（包括敌方火焰兵的攻击）。它的核心能力是点燃敌人：无论是通过近战攻击命中，还是直接触碰到幸存者，都能使对方燃烧起来。此外，它还能禁用侦察兵的“原子能饮料”，并在使用猎人短弓时自动点燃弓箭。
*   **参数 (`Defines`)**:
    *   `ZF_SCORCHING_ATTACK`: `-30` - 永久攻击力惩罚。
    *   `ZF_SCORCHING_SPEED`: `50` - 永久移动速度加成。
    *   `ZF_SCORCH_DURATION`: `10.0` - 点燃效果的持续时间。
*   **逻辑处理**:
    1.  **永久燃烧与免疫**: [`onPeriodic`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:68) 每秒重新点燃自己。[`onTakeDamage`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:75) 事件检测并免疫所有火焰伤害 (`DMG_BURN`) 和火焰兵的非近战伤害。
    2.  **点燃敌人**:
        *   [`onTakeDamagePost`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:92): 在近战攻击命中幸存者后，点燃对方。
        *   [`onTouch`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:98): 在身体接触到幸存者时，点燃对方。
    3.  **禁用饮料**: [`onPlayerRunCmd`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:113) 持续检测玩家是否在使用原子能饮料 (`tf_weapon_bonk`)，如果是，则阻止其使用（通过移除 `IN_ATTACK2` 按键）。
    4.  **点燃弓箭**: [`onCalcIsAttackCritical`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:129) 在攻击时检查是否手持猎人短弓，如果是，则设置 `m_bArrowAlight` 属性，使射出的箭矢带火。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的速度加成和攻击惩罚。
    *   `VTABLE_ON_PERIODIC`: 使自身保持燃烧。
    *   `VTABLE_ON_TAKE_DAMAGE`: 免疫火焰伤害。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 通过近战攻击点燃敌人。
    *   `VTABLE_ON_TOUCH`: 通过身体接触点燃敌人。
    *   `VTABLE_ON_PLAYER_RUN_CMD`: 禁用原子能饮料。
    *   `VTABLE_ON_CALC_IS_ATTACK_CRITICAL`: 点燃猎人短弓的箭矢。

### 17. Sick - [`src/sourcepawn/perks/zombie/SickPerk.inc`](src/sourcepawn/perks/zombie/SickPerk.inc)

*   **职业名称**: `Sick`
*   **职业类型**: 僵尸
*   **介绍**: `Sick`（病菌）是一个区域控制型职业，自身防御力较低，但能通过按 `E` 键连续喷射出多个酸性投射物，在撞击点形成持续伤害的酸液池。
*   **详细介绍**: 该职业以降低自身防御为代价，换来了强大的区域封锁能力。按 `E` 键后，`Sick` 会在短时间内连续喷射出5个酸性投射物，这些投射物会以抛物线轨迹飞行。当投射物撞击到地面或墙壁时，会在撞击点生成一个持续15秒的酸液池。任何进入酸液池的幸存者都会受到持续的毒性伤害。这是一个非常适合封锁狭窄通道或压制固定位置敌人的职业。
*   **参数 (`Defines`)**:
    *   `ZF_SICK_DEFEND`: `-75` - 永久防御力惩罚。
    *   `ZF_SICK_DAMAGE`: `3.0` - 酸液池每秒造成的伤害。
    *   `ZF_SICK_DAMAGE_RADIUS`: `150.0` - 酸液池的作用半径。
    *   `ZF_SICK_COOLDOWN`: `15.0` - 技能冷却时间。
    *   `ZF_SICK_LIFETIME`: `15.0` - 酸液池的存在时间。
    *   `ZF_SICK_SPIT_COUNT`: `5` - 一次技能喷射的投射物数量。
    *   `ZF_SICK_SPIT_INTERVAL`: `0.2` - 每次喷射的间隔时间。
    *   `ZF_SICK_PROJECTILE_SPEED`: `1600.0` - 投射物的飞行速度。
*   **逻辑处理**:
    1.  **技能发动**: [`onCallForMedic`](src/sourcepawn/perks/zombie/SickPerk.inc:233) (按`E`键) 触发。它会设置一个重复计时器 [`timer_SpitProjectile`](src/sourcepawn/perks/zombie/SickPerk.inc:253)，该计时器会以 `ZF_SICK_SPIT_INTERVAL` 的间隔触发 `ZF_SICK_SPIT_COUNT` 次。
    2.  **投射物创建**: 每次计时器触发时，都会在玩家眼前创建一个投射物实体，并将其加入 `projectiles` 数据包进行追踪。
    3.  **碰撞检测**: [`onGameFrame`](src/sourcepawn/perks/zombie/SickPerk.inc:438) 每帧检查所有追踪中的投射物。它使用 `doItemCollide` 函数来判断投射物是否与墙壁或地面发生碰撞。
    4.  **酸液池生成**: 一旦检测到碰撞，就会调用 [`createAcidPool`](src/sourcepawn/perks/zombie/SickPerk.inc:341) 在碰撞点创建一个酸液池模型，并将其加入 `pools` 数据包进行管理。
    5.  **酸液池伤害与清理**: [`onPeriodic`](src/sourcepawn/perks/zombie/SickPerk.inc:383) 每秒遍历所有激活的酸液池，对范围内的幸存者造成伤害，并清理过期的酸液池。
    6.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/SickPerk.inc:188) 和 [`onPlayerSpawn`](src/sourcepawn/perks/zombie/SickPerk.inc:167) 确保在玩家死亡或重生时，所有相关的投射物、酸液池和计时器都会被正确清理。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 开始喷射投射物。
    *   `VTABLE_ON_GAME_FRAME`: 检测投射物碰撞。
    *   `VTABLE_ON_PERIODIC`: 管理酸液池的伤害和生命周期，并处理技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能冷却状态。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_PLAYER_SPAWN`: 清理所有技能效果。


### 18. StaticField - [`src/sourcepawn/perks/zombie/StaticFieldPerk.inc`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc)

*   **职业名称**: `StaticField`
*   **职业类型**: 僵尸
*   **介绍**: `StaticField`（静电场）是一个被动光环型职业，它会持续对周围移动的幸存者造成伤害，幸存者移动速度越快，受到的伤害越高。
*   **详细介绍**: 该职业的核心能力是一个永久存在的、以自身为中心的“静电场”光环。任何在光环范围内的幸存者，只要其移动速度超过一个阈值，就会持续受到电击伤害。伤害量与幸存者的当前速度成正比，这意味着高速移动的侦察兵或正在冲刺的幸存者会受到远高于正常行走幸存者的伤害。这是一个强大的反机动性和区域骚扰职业。
*   **参数 (`Defines`)**:
    *   `STATIC_FIELD_RADIUS`: `350.0` - 静电场光环的作用半径。
    *   `STATIC_FIELD_DAMAGE_FACTOR`: `0.02` - 伤害系数，每秒伤害 = 速度 * 此系数。
*   **逻辑处理**:
    1.  **光环伤害**: [`onPeriodic`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:88) 是唯一的核心逻辑。它每秒执行一次：
        *   遍历所有幸存者，检查他们是否在光环范围内。
        *   获取幸存者的当前速度向量 (`m_vecVelocity`) 并计算其长度（即速率）。
        *   如果速率大于50.0，则根据 `STATIC_FIELD_DAMAGE_FACTOR` 计算伤害，并使用 `SDKHooks_TakeDamage` 对幸存者造成伤害。
    2.  **视觉效果与清理**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:67) 会创建一个附着在玩家身上的粒子效果来可视化光环。[`onDeath`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:74) 和 [`onRemove`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:81) 确保在玩家死亡或更换职业时，粒子效果会被正确移除。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_SPAWN`: 创建光环的视觉特效。
    *   `VTABLE_ON_PERIODIC`: 核心逻辑，对范围内的移动幸存者造成伤害。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理光环特效。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示一个静态的“激活”状态。


### 19. Swarming - [`src/sourcepawn/perks/zombie/SwarmingPerk.inc`](src/sourcepawn/perks/zombie/SwarmingPerk.inc)

*   **职业名称**: `Swarming`
*   **职业类型**: 僵尸
*   **介绍**: `Swarming`（蜂群）是一个弱化自身但重生极快的职业，它在靠近其他僵尸时会获得快速重生能力。
*   **详细介绍**: 该职业以永久降低自身攻防为代价，换取了极高的移动速度和独特的重生机制。它的核心能力是“蜂拥重生”：当 `Swarming` 僵尸死亡时，如果其死亡地点附近（400单位半径内）有至少一名其他存活的僵尸，它就能在短短3秒后原地重生。这是一个典型的人海战术职业，鼓励玩家与大部队一同行动，通过不断地快速重生来消耗幸存者的资源。
*   **参数 (`Defines`)**:
    *   `ZF_SWARMING_COMBAT`: `-20` - 永久攻击力和防御力惩罚。
    *   `ZF_SWARMING_RADIUSSQ`: `(400 * 400)` - 触发快速重生的所需队友半径（平方）。
    *   `ZF_SWARMING_SPEED`: `50` - 永久移动速度加成。
    *   `ZF_SWARMING_RESPAWNTIME`: `3.0` - 快速重生时间。
*   **逻辑处理**:
    1.  **属性修改**: [`updatePermStats`](src/sourcepawn/perks/zombie/SwarmingPerk.inc:58) 在出生时应用永久的攻防惩罚和速度加成。
    2.  **重生逻辑**: 游戏的核心重生逻辑（未在此文件中，而在 `zf_perk.inc` 的 `perk_OnDeath` 中处理）会检查死亡的僵尸是否为 `Swarming`。如果是，它会遍历所有其他僵尸，检查是否有僵尸在 `ZF_SWARMING_RADIUSSQ` 范围内。如果找到，则会覆盖默认重生逻辑，改为在 `ZF_SWARMING_RESPAWNTIME` 秒后原地重生该玩家。
    3.  **视觉效果**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/SwarmingPerk.inc:65) 在玩家出生时创建一个苍蝇光环特效。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_PLAYER_SPAWN`: 创建视觉光环。
    *   `VTABLE_ON_DEATH`: (由核心逻辑处理) 检查附近是否有队友以触发快速重生。


### 20. Toxic - [`src/sourcepawn/perks/zombie/ToxicPerk.inc`](src/sourcepawn/perks/zombie/ToxicPerk.inc)

*   **职业名称**: `Toxic`
*   **职业类型**: 僵尸
*   **介绍**: `Toxic`（剧毒）是一个骚扰和区域控制型职业，攻击力较低，但能通过近战攻击和反伤使幸存者中毒，并在原地站立不动时产生一个持续伤害的毒性光环。
*   **详细介绍**: 该职业拥有永久的攻击力惩罚。它的能力分为两部分：
    1.  **中毒**: 无论是它用近战攻击命中幸存者，还是幸存者用近战攻击命中它，都会使幸存者中毒，在一段时间内持续受到伤害。
    2.  **毒性领域**: 当 `Toxic` 玩家在原地站立不动超过3秒后，会在自身周围生成一个可见的毒性光环。任何进入光环的幸存者都会受到持续伤害，伤害量离 `Toxic` 玩家越近越高。一旦玩家移动，光环就会立即消失。
*   **参数 (`Defines`)**:
    *   `ZF_TOXIC_DAMAGE_PENALTY`: `-50` - 永久攻击力惩罚。
    *   `ZF_TOXIC_POISON_DURATION`: `12.0` - 中毒效果的持续时间。
    *   `ZF_TOXIC_AURA_RADIUS`: `500.0` - 毒性光环的作用半径。
    *   `ZF_TOXIC_AURA_DAMAGE`: `10.0` - 在光环中心点每秒造成的最大伤害。
    *   `TOXIC_STILL_TIME`: `3.0` - 激活毒性光环所需的站立时间。
*   **逻辑处理**:
    1.  **中毒效果**:
        *   [`onDealDamagePost`](src/sourcepawn/perks/zombie/ToxicPerk.inc:142): 在用近战攻击命中幸存者后，为对方施加中毒状态。
        *   [`onTakeDamagePost`](src/sourcepawn/perks/zombie/ToxicPerk.inc:157): 在被幸存者近战命中后，为对方施加中毒状态。
    2.  **毒性光环**: [`onPeriodic`](src/sourcepawn/perks/zombie/ToxicPerk.inc:166) 是核心。它每秒检查玩家的速度：
        *   如果玩家在移动，则重置站立计时器 `still_timer`，并移除已存在的光环。
        *   如果玩家静止，则增加 `still_timer`。当计时器达到 `TOXIC_STILL_TIME` 时，激活光环 (`field_active = true`) 并创建粒子效果。
        *   如果光环已激活，则遍历所有幸存者，根据其与光环中心的距离计算并施加伤害。
    3.  **状态清理**: [`onRemove`](src/sourcepawn/perks/zombie/ToxicPerk.inc:253) 和 [`onDeath`](src/sourcepawn/perks/zombie/ToxicPerk.inc:261) 确保在更换职业或死亡时，光环效果能被正确移除。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久攻击力惩罚。
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 主动攻击使人中毒。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 被动反伤使人中毒。
    *   `VTABLE_ON_PERIODIC`: 管理静止计时器和毒性光环的激活与伤害。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示光环状态（充能中、已激活、未激活）。
    *   `VTABLE_ON_PLAYER_SPAWN` / `VTABLE_ON_REMOVE` / `VTABLE_ON_DEATH`: 清理光环效果。


### 21. Vampiric - [`src/sourcepawn/perks/zombie/VampiricPerk.inc`](src/sourcepawn/perks/zombie/VampiricPerk.inc)

*   **职业名称**: `Vampiric`
*   **职业类型**: 僵尸
*   **介绍**: `Vampiric`（吸血鬼）是一个续航型职业，拥有少量防御加成和持续的生命恢复，并且能通过造成伤害来吸取生命值。
*   **详细介绍**: 该职业的核心能力是强大的生命回复。它拥有两个回血来源：
    1.  **被动恢复**: 每秒自动恢复少量生命值。
    2.  **吸血**: 对幸存者造成的任何伤害，都会按一定比例转化为自己的生命值。这个吸血效果可以提供过量治疗。
*   **参数 (`Defines`)**:
    *   `ZF_VAMPIRIC_LIFESTEAL_RATIO`: `0.6` - 造成伤害的60%会转化为生命值。
    *   `ZF_VAMPIRIC_REGEN`: `10` - 每秒被动恢复的生命值。
    *   `ZF_VAMPIRIC_DEFEND`: `10` - 永久防御力加成。
*   **逻辑处理**:
    1.  **吸血**: [`onDealDamagePost`](src/sourcepawn/perks/zombie/VampiricPerk.inc:62) 在对幸存者造成伤害后触发。它会计算造成伤害的60%，然后调用 `addHealth` 函数为自己恢复生命，并允许过量治疗。
    2.  **被动恢复**: [`onPeriodic`](src/sourcepawn/perks/zombie/VampiricPerk.inc:75) 每秒为玩家恢复 `ZF_VAMPIRIC_REGEN` 点生命值，但不允许过量治疗。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御加成。
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 核心逻辑，根据造成的伤害吸取生命。
    *   `VTABLE_ON_PERIODIC`: 提供被动的生命恢复。


### 22. Vindictive - [`src/sourcepawn/perks/zombie/VindictivePerk.inc`](src/sourcepawn/perks/zombie/VindictivePerk.inc)

*   **职业名称**: `Vindictive`
*   **职业类型**: 僵尸
*   **介绍**: `Vindictive`（复仇者）是一个成长型职业，通过击杀和助攻来永久性地获得伤害、防御、速度和暴击率加成。
*   **详细介绍**: 该职业的核心机制是“复仇”。每当 `Vindictive` 玩家完成一次击杀或助攻，他都会获得一定数量的“伤害加成”和“通用加成”。伤害加成会同时提升攻击力和防御力，而通用加成则会同时提升移动速度和暴击率。这些加成是永久累积的，但有上限。这是一个典型的“滚雪球”职业，在游戏前期可能较弱，但随着战功的积累会变得越来越强大。
*   **参数 (`Defines`)**:
    *   `ZF_VINDICTIVE_DAMAGE_CAP`: `50` - 伤害加成（攻击/防御）的上限。
    *   `ZF_VINDICTIVE_UTILITY_CAP`: `25` - 通用加成（速度/暴击）的上限。
    *   `ZF_VINDICTIVE_DAMAGE_ON_KILL`: `10` - 每次击杀获得的伤害加成。
    *   `ZF_VINDICTIVE_UTILITY_ON_KILL`: `5` - 每次击杀获得的通用加成。
    *   `ZF_VINDICTIVE_DAMAGE_ON_ASSIST`: `5` - 每次助攻获得的伤害加成。
    *   `ZF_VINDICTIVE_UTILITY_ON_ASSIST`: `2` - 每次助攻获得的通用加成。
*   **逻辑处理**:
    1.  **累积加成**:
        *   [`onKill`](src/sourcepawn/perks/zombie/VindictivePerk.inc:104): 在完成击杀后，增加 `damage_bonus` 和 `utility_bonus`，并确保不超过上限。
        *   [`onAssistKill`](src/sourcepawn/perks/zombie/VindictivePerk.inc:110): 在完成助攻后，增加 `damage_bonus` 和 `utility_bonus`，并确保不超过上限。
    2.  **应用加成**: [`updateCondStats`](src/sourcepawn/perks/zombie/VindictivePerk.inc:116) 每帧将储存的 `damage_bonus` 和 `utility_bonus` 作为条件性属性 (`ZFStatTypeCond`) 应用于玩家的攻击、防御、速度和暴击率上。
*   **事件处理**:
    *   `VTABLE_ON_KILL`: 累积击杀奖励。
    *   `VTABLE_ON_ASSIST_KILL`: 累积助攻奖励。
    *   `VTABLE_UPDATE_COND_STATS`: 应用累积的属性加成。

### 23. Volatile - [`src/sourcepawn/perks/zombie/VolatilePerk.inc`](src/sourcepawn/perks/zombie/VolatilePerk.inc)

*   **职业名称**: `Volatile`
*   **职业类型**: 僵尸
*   **介绍**: `Volatile`（易爆者）是一个移动的炸弹，它能将被动承受的伤害转化为能量，并能通过按 `E` 键主动引爆，对周围造成基于已储存能量的巨大伤害。
*   **详细介绍**: 该职业的核心机制是“能量转换”。每当 `Volatile` 受到任何来源的伤害时，都会将一部分伤害（50%）储存为“能量”。这些能量会随着时间缓慢衰减。玩家可以随时按 `E` 键主动引爆，引爆时会立即自杀，并对周围大范围内的敌人造成正比于当前储存能量的伤害。这是一个高风险、高回报的自杀式攻击职业，适合在吸收大量伤害后与敌人同归于尽。
*   **参数 (`Defines`)**:
    *   `VOLATILE_ENERGY_STORE_RATIO`: `0.5` - 将承受伤害的50%转化为能量。
    *   `VOLATILE_ENERGY_DECAY`: `8.0` - 每秒自然衰减的能量值。
    *   `VOLATILE_MAX_ENERGY`: `150.0` - 可储存的最大能量上限。
    *   `VOLATILE_EXPLOSION_RADIUS`: `500.0` - 爆炸半径。
    *   `VOLATILE_SUICIDE_MULTIPLIER`: `1.0` - 爆炸伤害与储存能量的倍率。
*   **逻辑处理**:
    1.  **能量储存**: [`onTakeDamage`](src/sourcepawn/perks/zombie/VolatilePerk.inc:84) 事件在玩家受到伤害时触发，将伤害值按比例增加到 `energy` 变量中，并确保不超过上限。
    2.  **主动引爆**: [`onCallForMedic`](src/sourcepawn/perks/zombie/VolatilePerk.inc:99) (按`E`键) 触发。如果当前有能量，它会根据储存的能量计算爆炸伤害，然后使用 `applyDamageRadial` 制造范围伤害，并立即强制玩家自杀。
    3.  **能量衰减**: [`onPeriodic`](src/sourcepawn/perks/zombie/VolatilePerk.inc:117) 每秒减少当前储存的能量。
    4.  **状态重置**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/VolatilePerk.inc:79) 确保玩家在重生时能量清零。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE`: 核心逻辑，将被动受到的伤害转化为能量。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 主动引爆，造成范围伤害并自杀。
    *   `VTABLE_ON_PERIODIC`: 处理能量的自然衰减。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示当前储存的能量。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置能量。
