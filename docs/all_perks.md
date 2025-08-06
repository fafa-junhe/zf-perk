

# Zombie Fortress 职业技能文档

本文档详细解析了 `zf_perk.inc` 文件中定义的所有幸存者和僵尸职业。

## 幸存者职业 (Survivor Perks)

### 1. Alchemist - [`src/sourcepawn/perks/survival/AlchemistPerk.inc`](src/sourcepawn/perks/survival/AlchemistPerk.inc)

*   **职业名称**: `Alchemist`
*   **职业类型**: 幸存者
*   **介绍**: `Alchemist`可以周期性地获得可投掷的药水。药水落地后会形成一个持续一段时间的魔法水坑，对范围内的盟友提供增益，同时对僵尸造成伤害和减速。
*   **详细介绍**:
    这是一个区域控制和团队辅助型职业。玩家通过按 `E` 键投掷药水，在地面上制造一个持续性的效果区域。这个区域对盟友是安全的，并提供移动速度加成；对踏入的僵尸则会造成持续的微量伤害和显著的减速效果。该职业的核心玩法在于策略性地投掷药水，以分割战场、保护关键位置或在撤退时掩护队友。
*   **参数 (`Defines`)**:
    *   `ALCHEMIST_POTION_GEN_TIME`: `25` 秒 - 生成一瓶药水所需的时间。
    *   `ALCHEMIST_MAX_POTIONS`: `3` - 可持有的最大药水数量。
    *   `ALCHEMIST_AOE_DURATION`: `7.0` 秒 - 药水坑的持续时间。
    *   `ALCHEMIST_AOE_RADIUS`: `180.0` - 药水坑的效果半径。
    *   `ALCHEMIST_ALLY_SPEED_BONUS`: `20` - 对盟友的速度加成。
    *   `ALCHEMIST_ZOMBIE_SLOW`: `-30` - 对僵尸的减速效果。
    *   `ALCHEMIST_ZOMBIE_DAMAGE`: `10.0` - （定义未使用，实际伤害为 `1.0`）
    *   `ALCHEMIST_POTION_MODEL`: `"models/props_halloween/hwn_flask_vial.mdl"` - 药水投射物和水坑锚点的模型。
*   **逻辑处理**:
    1.  **药水生成**: [`onPeriodic`](src/sourcepawn/perks/survival/AlchemistPerk.inc:105) 事件每秒检查一次，如果药水未满，则减少计时器，计时器归零时增加一瓶药水。
    2.  **投掷**: [`onCallForMedic`](src/sourcepawn/perks/survival/AlchemistPerk.inc:166) (按`E`键) 触发投掷逻辑。如果没有药水或地面上的水坑已达上限，则会提示玩家。成功投掷后，会创建一个使用 `fxCreateModelThrown` 的投射物实体。
    3.  **投射物追踪**: [`onGameFrame`](src/sourcepawn/perks/survival/AlchemistPerk.inc:203) 事件逐帧追踪投射物。它使用 `doItemCollide` 函数检测投射物是否与墙壁或地面发生碰撞。
    4.  **水坑生成**: 碰撞后，调用 [`Alchemist_CreatePuddle`](src/sourcepawn/perks/survival/AlchemistPerk.inc:303) 函数。该函数会在碰撞点创建一个隐形的实体作为“水坑”的锚点，并附着一个粒子特效 (`ZFPART_AURAPOTIONPUDDLE`) 和一个破碎声效。同时，启动一个计时器，在 `ALCHEMIST_AOE_DURATION` 秒后移除水坑。
    5.  **光环效果**: [`onPeriodic`](src/sourcepawn/perks/survival/AlchemistPerk.inc:105) 事件会遍历所有有效的水坑，并检测所有玩家是否在水坑半径内，然后根据阵营施加对应的增益（幸存者加速）或减益（僵尸减速和伤害）。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置玩家的药水数量和生成计时器。
    *   `VTABLE_ON_PERIODIC`: 处理药水的自动生成和水坑的光环效果。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 投掷药水。
    *   `VTABLE_ON_GAME_FRAME`: 负责投射物的碰撞检测。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示当前药水数量和下一瓶的生成进度。
    *   `VTABLE_ON_REMOVE` / `VTABLE_ON_DEATH`: 玩家死亡或更换职业时，清理其创建的所有水坑和飞行中的投射物，防止实体泄露。

### 2. Athletic - [`src/sourcepawn/perks/survival/AthleticPerk.inc`](src/sourcepawn/perks/survival/AthleticPerk.inc)

*   **职业名称**: `Athletic`
*   **职业类型**: 幸存者
*   **介绍**: `Athletic`是一个纯粹的属性增强职业，以牺牲伤害和暴击为代价，大幅提升移动速度和攻击速度。
*   **详细介绍**: 这是一个简单直接的被动职业，适合需要高机动性的玩家。它没有任何主动技能或复杂机制，只是在玩家出生时永久性地修改其基础属性。选择此职业意味着玩家将更难被僵尸追上，并且能更快地攻击，但每次攻击的伤害会降低，且完全失去暴击能力。
*   **参数 (`Defines`)**:
    *   `ZF_ATHLETIC_ATTACK`: `-40` - 攻击力减少40%。
    *   `ZF_ATHLETIC_CRIT`: `-100` - 暴击率减少100%（即无法暴击）。
    *   `ZF_ATHLETIC_ROF`: `100` - 攻击速度提升100%。
    *   `ZF_ATHLETIC_SPEED`: `100` - 移动速度提升100%。
*   **逻辑处理**:
    1.  **属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/survival/AthleticPerk.inc:49) 是唯一的核心逻辑。当玩家出生或职业确定时，该函数被调用，通过 `addStat` 函数将定义的四个属性值作为永久（`ZFStatTypePerm`）属性应用给玩家。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久性的属性增减。

### 3. Berserker - [`src/sourcepawn/perks/survival/BerserkerPerk.inc`](src/sourcepawn/perks/survival/BerserkerPerk.inc)

*   **职业名称**: `Berserker`
*   **职业类型**: 幸存者
*   **介绍**: `Berserker`（狂战士）是一个高风险高回报的职业，生命值越低，获得的攻击速度和近战吸血效果越强，但会永久降低自身防御力。
*   **详细介绍**: 该职业鼓励玩家在低生命值状态下战斗。它会永久性地降低玩家的防御力，使其更容易受到伤害。然而，随着生命值的降低，玩家会获得线性的攻击速度加成和近战攻击吸血能力，在生命值低于25%时达到最大值。这种机制使得狂战士在濒死时具有强大的爆发和生存能力，但同时也非常脆弱。
*   **参数 (`Defines`)**:
    *   `BERSERKER_DEF_REDUCTION`: `-25` - 永久防御力降低25。
    *   `BERSERKER_MAX_ROF_BONUS`: `150` - 最大攻击速度加成150%。
    *   `BERSERKER_MAX_LIFESTEAL_BONUS`: `0.50` - 最大近战吸血比例50%。
    *   `BERSERKER_HP_THRESHOLD`: `0.25` - 触发最大加成的生命值阈值（25%）。
*   **逻辑处理**:
    1.  **永久减防**: [`updateClientPermStats`](src/sourcepawn/perks/survival/BerserkerPerk.inc:65) 在玩家出生时永久降低其防御力。
    2.  **动态加成计算**: [`updateCondStats`](src/sourcepawn/perks/survival/BerserkerPerk.inc:69) 每帧计算玩家当前的生命值百分比。根据生命值百分比，线性地计算出应得的攻击速度和吸血率加成。当生命值低于25%时，获得最大加成；高于25%时，加成随生命值升高而降低。计算出的攻击速度作为条件性属性（`ZFStatTypeCond`）应用，吸血率则存入Perk私有数据。
    3.  **近战吸血**: [`onDealDamagePost`](src/sourcepawn/perks/survival/BerserkerPerk.inc:102) 在玩家造成伤害后触发。如果伤害是近战伤害（`DMG_CLUB`），则根据当前存储的吸血率，将造成伤害的一部分转化为治疗量返还给玩家。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 永久降低防御力。
    *   `VTABLE_UPDATE_COND_STATS`: 根据当前生命值动态计算并应用攻击速度加成，更新吸血率，并显示在HUD上。
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 实现近战攻击的吸血效果。

### 4. Carpenter - [`src/sourcepawn/perks/survival/CarpenterPerk.inc`](src/sourcepawn/perks/survival/CarpenterPerk.inc)

*   **职业名称**: `Carpenter`
*   **职业类型**: 幸存者
*   **介绍**: `Carpenter`（木匠）是一个防御型职业，以降低攻击力为代价换取更高的防御力，并能够建造坚固的木制路障来阻挡僵尸。
*   **详细介绍**: 木匠拥有更高的基础防御力，但攻击力较弱。其核心技能是通过蹲下并按 `E` 键来建造一个拥有较高生命值的路障。建造有冷却时间，并且对建造位置有一定要求（不能离其他玩家或路障太近）。路障可以有效地封锁狭窄的通道，为团队创造安全的防御空间。
*   **参数 (`Defines`)**:
    *   `CARPENTER_ATTACK`: `-40` - 永久攻击力降低40。
    *   `CARPENTER_DEFEND`: `25` - 永久防御力提升25。
    *   `CARPENTER_BARRICADE_HEALTH`: `500` - 路障的生命值。
    *   `CARPENTER_COOLDOWN`: `25` - 建造路障的冷却时间（秒）。
    *   `CARPENTER_MAX_ITEMS`: `4` - 最多可同时存在的路障数量。
    *   `CARPENTER_DROP_RADSQ_BARRICADE`: `(250 * 250)` - 建造时离其他路障的最小距离（平方）。
    *   `CARPENTER_DROP_RADSQ_CLIENT`: `(150 * 150)` - 建造时离其他玩家的最小距离（平方）。
*   **逻辑处理**:
    1.  **属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/survival/CarpenterPerk.inc:65) 在出生时应用永久的攻击力减益和防御力增益。
    2.  **建造路障**: [`onCallForMedic`](src/sourcepawn/perks/survival/CarpenterPerk.inc:101) (按`E`键) 触发建造逻辑。函数会检查冷却时间、玩家是否蹲在地上、以及周围是否有其他玩家或路障。
    3.  **实体创建**: 如果所有条件满足，调用 [`doCarpenterBuild`](src/sourcepawn/perks/survival/CarpenterPerk.inc:185) 函数。该函数在玩家面前创建一个静态模型实体（`ZFMDL_FENCE`）作为路障，并为其设置生命值和受伤回调。
    4.  **路障受伤**: 路障的 `SDKHook_OnTakeDamage` 事件被挂钩到全局函数 `perk_OnFenceTakeDamage`，用于处理路障受到的伤害。
    5.  **状态显示**: [`updateCondStats`](src/sourcepawn/perks/survival/CarpenterPerk.inc:71) 在HUD上显示当前状态，如“准备就绪”、“冷却中”或“路障已满”。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发建造路障的技能。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能状态。

### 5. Charitable - [`src/sourcepawn/perks/survival/CharitablePerk.inc`](src/sourcepawn/perks/survival/CharitablePerk.inc)

*   **职业名称**: `Charitable`
*   **职业类型**: 幸存者
*   **介绍**: `Charitable`（慈善家）通过击杀和助攻积累点数，然后消耗点数制造可以被队友拾取的礼物。
*   **详细介绍**: 这是一个纯粹的辅助职业。玩家通过参与战斗（击杀或助攻）来获得“慈善点数”。当点数足够时，可以按 `E` 键消耗点数，投掷一个礼物盒。任何接触到礼物盒的盟友都会获得一个临时的增益效果（如生命恢复或属性加成）。
*   **参数 (`Defines`)**:
    *   `ZF_CHARITABLE_MAX_ITEMS`: `5` - 最多可同时存在的礼物数量。
    *   `ZF_CHARITABLE_POINTS_ASSIST`: `2` - 每次助攻获得的点数。
    *   `ZF_CHARITABLE_POINTS_KILL`: `2` - 每次击杀获得的点数。
    *   `ZF_CHARITABLE_POINTS_GIFT`: `4` - 制造一个礼物需要消耗的点数。
*   **逻辑处理**:
    1.  **积累点数**: [`onKill`](src/sourcepawn/perks/survival/CharitablePerk.inc:59) 和 [`onAssistKill`](src/sourcepawn/perks/survival/CharitablePerk.inc:64) 事件在玩家完成击杀或助攻时，为其增加存储在 `zf_perkState` 中的点数。
    2.  **制造礼物**: [`onCallForMedic`](src/sourcepawn/perks/survival/CharitablePerk.inc:69) (按`E`键) 检查玩家是否有足够的点数。如果有，则消耗点数并调用 `doItemThrow` 投掷一个礼物盒模型。
    3.  **礼物交互**: 投掷出的礼物盒实体被挂钩了 `SDKHook_Touch` 事件到全局函数 `perk_OnCharitableGiftTouched`。当其他玩家接触到礼物时，该函数会触发礼物的效果。
    4.  **状态显示**: [`updateCondStats`](src/sourcepawn/perks/survival/CharitablePerk.inc:54) 在HUD上显示玩家当前拥有的礼物数量（点数/制造所需点数）。
*   **事件处理**:
    *   `VTABLE_ON_KILL`: 击杀时获得点数。
    *   `VTABLE_ON_ASSIST_KILL`: 助攻时获得点数。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 消耗点数制造并投掷礼物。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示可制造的礼物数量。

### 6. Cowardly - [`src/sourcepawn/perks/survival/CowardlyPerk.inc`](src/sourcepawn/perks/survival/CowardlyPerk.inc)

*   **职业名称**: `Cowardly`
*   **职业类型**: 幸存者
*   **介绍**: `Cowardly`（懦夫）在受到僵尸近战攻击时会因恐慌而免疫该次伤害，并获得短暂的巨额速度和防御加成，但之后会进入冷却。
*   **详细介绍**: 这是一个生存向职业。当僵尸对玩家进行近战攻击时，`onTakeDamage` 事件会触发。如果技能未处于冷却状态，玩家将完全免疫这次攻击的伤害，并进入“恐慌”状态。在恐慌状态下，玩家获得极高的移动速度和防御力加成，便于逃离危险。恐慌状态结束后，技能会进入一个较长的冷却期，期间无法再次触发。
*   **参数 (`Defines`)**:
    *   `ZF_COWARDLY_DEFEND`: `50` - 恐慌状态下的防御加成。
    *   `ZF_COWARDLY_SPEED`: `200` - 恐慌状态下的速度加成。
    *   `ZF_COWARDLY_DURATION_SCARED`: `5` - 恐慌状态的持续时间（秒）。
    *   `ZF_COWARDLY_DURATION_COOLDOWN`: `20` - 技能的冷却时间（秒）。
*   **逻辑处理**:
    1.  **触发恐慌**: [`onTakeDamage`](src/sourcepawn/perks/survival/CowardlyPerk.inc:82) 检查受到的伤害是否为僵尸的近战攻击 (`DMG_CLUB`) 并且技能可用。如果条件满足，将伤害归零，并设置一个持续 `(5 + 20)` 秒的计时器到Perk私有数据中。
    2.  **计时器管理**: [`onPeriodic`](src/sourcepawn/perks/survival/CowardlyPerk.inc:101) 每秒减少计时器。当计时器从 `21` 减到 `20` 时，提示玩家恐慌效果结束；当计时器归零时，提示技能准备就绪。
    3.  **应用加成**: [`updateCondStats`](src/sourcepawn/perks/survival/CowardlyPerk.inc:119) 检查计时器。如果计时器值大于冷却时间（即处于恐慌的5秒内），则为玩家应用条件性的速度和防御加成。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE`: 检测僵尸近战攻击，免疫伤害并触发技能。
    *   `VTABLE_ON_PERIODIC`: 管理技能的恐慌和冷却计时器。
    *   `VTABLE_UPDATE_COND_STATS`: 在恐慌期间应用属性加成，并更新HUD状态。

### 7. Echo - [`src/sourcepawn/perks/survival/EchoPerk.inc`](src/sourcepawn/perks/survival/EchoPerk.inc)

*   **职业名称**: `Echo`
*   **职业类型**: 幸存者
*   **介绍**: `Echo`（回声）能够吸收僵尸近战攻击的能量，并免疫爆炸和冲击伤害。积攒能量后，可以通过按 `E` 键释放一次强力的冲击波，击退周围的僵尸。
*   **详细介绍**: 该职业有两个主要能力。首先，它被动地免疫所有爆炸和气流冲击造成的位移和伤害。其次，每次受到僵尸的近战攻击时，它会积攒一点“回声能量”。玩家可以随时按 `E` 键释放所有积攒的能量，产生一次以自身为中心的范围冲击波。冲击波的击退力度与消耗的能量成正比，能有效地为自己解围。
*   **参数 (`Defines`)**:
    *   `ECHO_KNOCKBACK_BASE_FORCE`: `200.0` - 冲击波的基础击退力。
    *   `ECHO_KNOCKBACK_ENERGY_SCALE`: `50.0` - 每点能量提供的额外击退力。
    *   `ECHO_AURA_RADIUS`: `300.0` - 冲击波的作用半径。
*   **逻辑处理**:
    1.  **能量积攒与伤害免疫**: [`onTakeDamage`](src/sourcepawn/perks/survival/EchoPerk.inc:63) 检查受到的伤害类型。如果是爆炸或气流伤害 (`DMG_BLAST | DMG_AIRBOAT`)，则将伤害归零。如果是僵尸的近战伤害 (`DMG_CLUB`)，则为玩家增加存储在 `zf_perkState` 中的能量点数。
    2.  **能量重置**: [`onPlayerSpawn`](src/sourcepawn/perks/survival/EchoPerk.inc:58) 在玩家出生时将其能量清零。
    3.  **释放冲击波**: [`onCallForMedic`](src/sourcepawn/perks/survival/EchoPerk.inc:89) (按`E`键) 检查玩家是否有能量。如果有，则计算总击退力，然后遍历所有僵尸，对在范围内的僵尸使用 `fxApplyForce` 施加一个远离玩家的力，并消耗所有能量。
    4.  **状态显示**: [`updateCondStats`](src/sourcepawn/perks/survival/EchoPerk.inc:134) 在HUD上显示当前积攒的能量点数。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置能量。
    *   `VTABLE_ON_TAKE_DAMAGE`: 免疫特定伤害并从僵尸近战攻击中吸收能量。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 释放能量冲击波。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示能量状态。
