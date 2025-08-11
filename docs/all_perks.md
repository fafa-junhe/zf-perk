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

### 4. BountyHunter - [`src/sourcepawn/perks/survival/BountyHunterPerk.inc`](src/sourcepawn/perks/survival/BountyHunterPerk.inc)

*   **职业名称**: `BountyHunter`
*   **职业类型**: 幸存者
*   **介绍**: `BountyHunter`（赏金猎人）可以主动扫描周围的光环型僵尸，并在击杀它们后获得永久属性奖励。
*   **详细介绍**: 这是一个针对特定高威胁僵尸的刺客型职业。玩家可以按 `E` 键发动扫描技能，在短时间内高亮显示周围大范围内的所有光环型僵尸（`Toxic`, `StaticField`, `GravityWarper`, `Swarming`）。成功击杀这些被标记的僵尸后，赏金猎人会获得永久的攻击力和暴击率加成，并回复生命值。该技能有冷却时间。
*   **参数 (`Defines`)**:
    *   `BOUNTY_SCAN_RADIUS`: `2000.0` - 扫描技能的作用半径。
    *   `BOUNTY_SCAN_DURATION`: `5.0` 秒 - 扫描效果的持续时间。
    *   `BOUNTY_SCAN_COOLDOWN`: `20` 秒 - 扫描技能的冷却时间。
    *   `BOUNTY_KILL_DAMAGE_BONUS`: `5` - 每次击杀目标获得的永久攻击力加成。
    *   `BOUNTY_KILL_CRIT_BONUS`: `5` - 每次击杀目标获得的永久暴击率加成。
    *   `BOUNTY_KILL_HEALTH_BONUS`: `25` - 每次击杀目标获得的生命恢复。
*   **逻辑处理**:
    1.  **扫描**: [`onCallForMedic`](src/sourcepawn/perks/survival/BountyHunterPerk.inc:102) (按`E`键) 触发扫描。如果技能不在冷却中，它会检查周围是否有符合条件的光环僵尸。
    2.  **高亮显示**: 如果找到目标，[`updateCondStats`](src/sourcepawn/perks/survival/BountyHunterPerk.inc:154) 会在扫描持续时间内，为所有范围内的目标僵尸绘制一条指向玩家的光束，并根据僵尸的具体职业显示不同颜色。
    3.  **击杀奖励**: [`onKill`](src/sourcepawn/perks/survival/BountyHunterPerk.inc:144) 事件在玩家完成击杀时触发。如果受害者是光环型僵尸，则为玩家提供永久的属性加成和生命恢复。
    4.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/survival/BountyHunterPerk.inc:138) 每秒减少扫描技能的冷却计时器。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发扫描技能。
    *   `VTABLE_ON_KILL`: 在击杀特定僵尸后获得奖励。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在扫描期间高亮显示目标，并更新HUD状态。

### 5. Carpenter - [`src/sourcepawn/perks/survival/CarpenterPerk.inc`](src/sourcepawn/perks/survival/CarpenterPerk.inc)

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

### 6. Charitable - [`src/sourcepawn/perks/survival/CharitablePerk.inc`](src/sourcepawn/perks/survival/CharitablePerk.inc)

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

### 7. Cowardly - [`src/sourcepawn/perks/survival/CowardlyPerk.inc`](src/sourcepawn/perks/survival/CowardlyPerk.inc)

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

### 8. Echo - [`src/sourcepawn/perks/survival/EchoPerk.inc`](src/sourcepawn/perks/survival/EchoPerk.inc)

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


### 9. Friend - [`src/sourcepawn/perks/survival/FriendPerk.inc`](src/sourcepawn/perks/survival/FriendPerk.inc)

*   **职业名称**: `Friend`
*   **职业类型**: 幸存者
*   **介绍**: `Friend`（伙伴）可以选择一名盟友作为伙伴。当与伙伴并肩作战时，双方都会获得增益；当伙伴阵亡时，自己会获得强大的复仇暴击。
*   **详细介绍**: 这是一个强调团队协作的职业。在回合开始时，玩家可以瞄准一名队友按 `E` 来指定其为伙伴，否则系统会自动随机分配。只要伙伴存活且在附近，双方都会获得持续的生命恢复和攻击力加成。如果伙伴不幸阵亡，该职业会将其转化为力量，将积累的“暴击时间”转化为持续的暴击效果，持续时间取决于之前的合作表现（击杀/助攻）。
*   **参数 (`Defines`)**:
    *   `ZF_FRIEND_ATTACK`: `25` - 与伙伴在附近时的攻击力加成。
    *   `ZF_FRIEND_REGEN`: `10` - 与伙伴在附近时的生命恢复量。
    *   `ZF_FRIEND_CRITTIME_INIT`: `5` - 初始暴击时间（秒）。
    *   `ZF_FRIEND_CRITTIME_KILL`: `4` - 每次击杀（自己或伙伴）增加的暴击时间。
    *   `ZF_FRIEND_CRITTIME_ASSIST`: `2` - 每次助攻（自己或伙伴）增加的暴击时间。
    *   `ZF_FRIEND_RADIUSSQ`: `(300 * 300)` - 伙伴光环的有效距离（平方）。
*   **逻辑处理**:
    1.  **伙伴选择**: [`onGraceEnd`](src/sourcepawn/perks/survival/FriendPerk.inc:127) 事件在准备阶段结束时自动选择一个伙伴（如果玩家没手动选）。[`onCallForMedic`](src/sourcepawn/perks/survival/FriendPerk.inc:136) 允许在准备阶段手动选择。
    2.  **积累暴击时间**: [`onKill`](src/sourcepawn/perks/survival/FriendPerk.inc:148) 和 [`onAssistKill`](src/sourcepawn/perks/survival/FriendPerk.inc:163) 事件在玩家或其伙伴完成击杀/助攻时，增加存储在Perk私有数据中的 `critTime`。
    3.  **光环与复仇**: [`updateCondStats`](src/sourcepawn/perks/survival/FriendPerk.inc:185) 是核心。它每帧检查伙伴的状态：
        *   如果伙伴存活且在范围内，则为双方施加攻击和回血光环，并用光束连接。
        *   如果伙伴阵亡，则将伙伴ID标记为-1，并根据剩余的 `critTime` 为玩家激活暴击效果。
    4.  **暴击时间衰减**: [`onPeriodic`](src/sourcepawn/perks/survival/FriendPerk.inc:176) 在伙伴阵亡后，每秒减少剩余的 `critTime`。
*   **事件处理**:
    *   `VTABLE_ON_GRACE_END`: 自动选择伙伴。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 手动选择伙伴。
    *   `VTABLE_ON_KILL` / `VTABLE_ON_ASSIST_KILL`: 积累暴击时间。
    *   `VTABLE_UPDATE_COND_STATS`: 应用光环增益或在伙伴死后激活复仇暴击，并更新HUD。
    *   `VTABLE_ON_PERIODIC`: 在伙伴死后使暴击时间衰减。


### 10. Gambler - [`src/sourcepawn/perks/survival/GamblerPerk.inc`](src/sourcepawn/perks/survival/GamblerPerk.inc)

*   **职业名称**: `Gambler`
*   **职业类型**: 幸存者
*   **介绍**: `Gambler`（赌徒）无法自然暴击，但通过击杀和助攻积累“运气”。每次攻击都有几率消耗运气触发一个随机的正面或负面效果。
*   **详细介绍**: 这是一个高随机性的职业。玩家的暴击率被设为0，但可以通过战斗获得“运气”点数。每次攻击时，系统会根据当前的运气值进行一次检定。如果成功，会消耗少量运气并触发以下四种效果之一：造成3倍暴击伤害、立即装填弹药、获得短暂的速度爆发，或者误伤自己。运气越高，触发几率越大。
*   **参数 (`Defines`)**:
    *   `GAMBLER_CRIT_REDUCTION`: `-100` - 永久暴击率惩罚。
    *   `GAMBLER_MAX_LUCK`: `30` - 可积累的最大运气值。
    *   `GAMBLER_KILL_LUCK_GAIN`: `2` - 每次击杀获得的运气。
    *   `GAMBLER_ASSIST_LUCK_GAIN`: `1` - 每次助攻获得的运气。
    *   `GAMBLER_HIT_LUCK_COST`: `1` - 每次成功触发效果消耗的运气。
    *   `GAMBLER_SELF_DAMAGE`: `10.0` - “误伤”效果造成的自我伤害。
    *   `GAMBLER_SPEED_BOOST_DURATION`: `5` 秒 - 速度爆发的持续时间。
    *   `GAMBLER_SPEED_BOOST_AMOUNT`: `50` - 速度爆发的加成量。
*   **逻辑处理**:
    1.  **运气积累**: [`onKill`](src/sourcepawn/perks/survival/GamblerPerk.inc:123) 和 [`onAssistKill`](src/sourcepawn/perks/survival/GamblerPerk.inc:132) 在玩家完成击杀或助攻时增加其运气值。
    2.  **触发检定**: [`onCalcIsAttackCritical`](src/sourcepawn/perks/survival/GamblerPerk.inc:74) 在每次攻击的暴击计算阶段触发。它会根据运气值进行随机检定。
    3.  **随机效果**: 如果检定成功，则消耗运气并随机选择一个效果应用给玩家，包括临时增加攻击和暴击、装填弹药、增加速度或自我伤害。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 永久移除暴击能力。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置运气值。
    *   `VTABLE_ON_KILL` / `VTABLE_ON_ASSIST_KILL`: 积累运气。
    *   `VTABLE_ON_CALC_IS_ATTACK_CRITICAL`: 进行运气检定并触发随机效果。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示当前的运气值。

### 11. Guardian - [`src/sourcepawn/perks/survival/GuardianPerk.inc`](src/sourcepawn/perks/survival/GuardianPerk.inc)

*   **职业名称**: `Guardian`
*   **职业类型**: 幸存者
*   **介绍**: `Guardian`（守护者）在原地站立不动一段时间后，会激活一个防御光环，为自己和范围内的盟友提供防御加成，并持续推开靠近的僵尸。
*   **详细介绍**: 这是一个鼓励玩家坚守阵地的防御型职业。当玩家保持静止（速度低于一个很小的值）达到3秒后，一个可见的防御光环会以玩家为中心展开。在此光环内，守护者自身和所有盟友都会获得显著的防御力提升。同时，任何试图进入光环的僵尸都会被一股力量持续推开，使其难以近身。一旦玩家移动，光环就会立即消失，需要重新静止来再次激活。
*   **参数 (`Defines`)**:
    *   `GUARDIAN_STILL_TIME`: `3.0` 秒 - 激活光环所需的静止时间。
    *   `GUARDIAN_AURA_RADIUS`: `250.0` - 光环的作用半径。
    *   `GUARDIAN_DEF_BONUS`: `15` - 光环提供的防御力加成。
    *   `GUARDIAN_ZOMBIE_PUSH`: `350.0` - 对僵尸的推开力度。
*   **逻辑处理**:
    1.  **静止检测**: [`onPeriodic`](src/sourcepawn/perks/survival/GuardianPerk.inc:126) 事件每秒检查一次玩家的速度。如果玩家正在移动，则重置静止计时器并移除光环。如果玩家静止，则增加计时器。
    2.  **光环激活**: 当静止计时器达到 `GUARDIAN_STILL_TIME` 时，`field_active` 状态被设为 `true`，并调用 [`GuardianPerkF_CreateAura`](src/sourcepawn/perks/survival/GuardianPerk.inc:222) 创建一个附着在玩家身上的粒子特效作为光环的可视化效果。
    3.  **光环效果应用**: [`updateCondStats`](src/sourcepawn/perks/survival/GuardianPerk.inc:157) 在光环激活时执行。它会遍历所有玩家：
        *   为范围内的幸存者（包括自己）应用条件性的防御加成。
        *   为范围内的僵尸施加一个远离守护者的力，实现击退效果。
    4.  **状态清理**: [`onPlayerSpawn`](src/sourcepawn/perks/survival/GuardianPerk.inc:119), [`onDeath`](src/sourcepawn/perks/survival/GuardianPerk.inc:214), 和 [`onRemove`](src/sourcepawn/perks/survival/GuardianPerk.inc:209) 事件确保在玩家重生、死亡或更换职业时，光环特效被正确移除，防止实体泄露。
    5.  **HUD显示**: [`updateCondStats`](src/sourcepawn/perks/survival/GuardianPerk.inc:157) 根据当前状态（移动中、充能中、已激活）在HUD上显示不同的提示信息。
*   **事件处理**:
    *   `VTABLE_ON_PERIODIC`: 检测玩家是否静止，并管理光环的激活计时。
    *   `VTABLE_UPDATE_COND_STATS`: 在光环激活时应用防御加成和击退效果，并更新HUD。
    *   `VTABLE_ON_PLAYER_SPAWN` / `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理光环效果，重置状态。


### 12. Heroic - [`src/sourcepawn/perks/survival/HeroicPerk.inc`](src/sourcepawn/perks/survival/HeroicPerk.inc)

*   **职业名称**: `Heroic`
*   **职业类型**: 幸存者
*   **介绍**: `Heroic`（英雄）在平时拥有均衡的属性加成。当成为战场上最后一名幸存者时，会激活一个强大的暴击光环，持续时间取决于之前积累的“英雄点数”。
*   **详细介绍**: 该职业为玩家提供了稳定的攻击和防御加成。其核心机制在于“最后的希望”：玩家通过击杀和助攻来不断积累暴击时间（存储在全局 `zf_perkTimer` 中）。在平时，这些积累的时间没有作用。然而，一旦场上只剩下该玩家一名幸存者，就会立即消耗所有积累的时间，触发一个持续性的暴击效果，让玩家有机会力挽狂澜。
*   **参数 (`Defines`)**:
    *   `HEROIC_ATTACK_BONUS`: `10` - 永久攻击力加成。
    *   `HEROIC_DEFENSE_BONUS`: `10` - 永久防御力加成。
    *   `HEROIC_CRITTIME_INIT`: `30` - 初始暴击时间。
    *   `HEROIC_CRITTIME_KILL`: `3` - 每次击杀增加的暴击时间。
    *   `HEROIC_CRITTIME_ASSIST`: `1` - 每次助攻增加的暴击时间。
    *   `HEROIC_CRITTIME_KILL_ACTIVE`: `0` - 暴击激活后击杀不再增加时间。
    *   `HEROIC_CRITTIME_ASSIST_ACTIVE`: `0` - 暴击激活后助攻不再增加时间。
*   **逻辑处理**:
    1.  **永久加成**: [`updateClientPermStats`](src/sourcepawn/perks/survival/HeroicPerk.inc:64) 在玩家出生时应用永久的攻击和防御加成，并初始化暴击时间。
    2.  **积累时间**: [`onKill`](src/sourcepawn/perks/survival/HeroicPerk.inc:74) 和 [`onAssistKill`](src/sourcepawn/perks/survival/HeroicPerk.inc:81) 在玩家完成击杀或助攻时，为其增加暴击时间。
    3.  **暴击激活**: [`updateCondStats`](src/sourcepawn/perks/survival/HeroicPerk.inc:88) 每帧检查幸存者数量。当发现幸存者只剩一人（即玩家自己）且暴击状态未激活时，它会激活暴击效果，持续时间等于当前积累的 `zf_perkTimer`。
    4.  **暴击持续与衰减**: 暴击激活后，[`updateCondStats`](src/sourcepawn/perks/survival/HeroicPerk.inc:88) 会每秒减少 `zf_perkTimer`，直到归零，然后移除暴击效果。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性加成并初始化暴击时间。
    *   `VTABLE_ON_KILL` / `VTABLE_ON_ASSIST_KILL`: 积累暴击时间。
    *   `VTABLE_UPDATE_COND_STATS`: 检测是否为最后一名幸存者，以激活和管理暴击效果，并更新HUD。


### 13. Holy - [`src/sourcepawn/perks/survival/HolyPerk.inc`](src/sourcepawn/perks/survival/HolyPerk.inc)

*   **职业名称**: `Holy`
*   **职业类型**: 幸存者
*   **介绍**: `Holy`（圣徒）拥有永久的防御加成。当蹲下、静止不动时，会创造一个治疗光环，为自己和附近的盟友持续恢复生命值。
*   **详细介绍**: 这是一个防御和辅助型职业。玩家永久获得防御力提升。其核心技能是一个需要满足特定条件才能激活的治疗光环：玩家必须在地面上、处于蹲伏姿态且完全静止。满足条件后，一个治疗光环会展开，为范围内的所有幸存者（包括自己）提供持续的生命恢复。
*   **参数 (`Defines`)**:
    *   `ZF_HOLY_DEFENCE`: `10` - 永久防御力加成。
    *   `ZF_HOLY_RADIUSSQ`: `(400 * 400)` - 治疗光环的作用半径（平方）。
    *   `ZF_HOLY_REGEN`: `20` - 每秒的生命恢复量。
*   **逻辑处理**:
    1.  **永久加成**: [`updateClientPermStats`](src/sourcepawn/perks/survival/HolyPerk.inc:59) 在出生时应用永久防御加成，并创建一个视觉光环。
    2.  **条件检测与治疗**: [`updateCondStats`](src/sourcepawn/perks/survival/HolyPerk.inc:65) 每帧检查玩家是否满足“蹲下、静止、在地上”的条件。如果满足，则遍历所有幸存者，为在光环范围内的盟友恢复生命值。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御加成和视觉光环。
    *   `VTABLE_UPDATE_COND_STATS`: 检测条件并施加治疗光环效果，更新HUD。

### 14. Juggernaut - [`src/sourcepawn/perks/survival/JuggernautPerk.inc`](src/sourcepawn/perks/survival/JuggernautPerk.inc)

*   **职业名称**: `Juggernaut`
*   **职业类型**: 幸存者
*   **介绍**: `Juggernaut`（主宰）是一个重型近战职业，拥有高攻击、低防御和低移速。它免疫坠落和自我伤害，并且所有近战攻击和从高处坠落都能击退并眩晕附近的僵尸。
*   **详细介绍**: 这是一个牺牲机动性和防御换取强大控制和近战能力的职业。它被动地免疫所有坠落伤害和自我伤害（如火箭跳）。其核心能力是击退：无论是用近战武器命中僵尸，还是从高处坠落到地面，都会以玩家为中心产生一个冲击波，对范围内的僵尸造成少量伤害、强力击退和短暂的减速效果。
*   **参数 (`Defines`)**:
    *   `ZF_JUGGERNAUT_ATTACK`: `50` - 永久攻击力加成。
    *   `ZF_JUGGERNAUT_DEFEND`: `-50` - 永久防御力惩罚。
    *   `ZF_JUGGERNAUT_SPEED`: `-100` - 永久移动速度惩罚。
    *   `ZF_JUGGERNAUT_FORCE`: `500.0` - 击退的力量。
    *   `ZF_JUGGERNAUT_RADIUS`: `150.0` - 坠落冲击波的半径。
    *   `ZF_JUGGERNAUT_STUN_DURATION`: `1.0` 秒 - 坠落冲击波造成的减速持续时间。
    *   `ZF_JUGGERNAUT_STUN_SLOWDOWN`: `-100` - 坠落冲击波造成的减速效果。
*   **逻辑处理**:
    1.  **属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/survival/JuggernautPerk.inc:66) 应用永久的属性修改。
    2.  **伤害免疫**: [`onTakeDamage`](src/sourcepawn/perks/survival/JuggernautPerk.inc:72) 事件在伤害计算前触发，将坠落和自我伤害归零。
    3.  **击退效果**: [`onTakeDamagePost`](src/sourcepawn/perks/survival/JuggernautPerk.inc:90) 在伤害计算后触发。
        *   如果事件是玩家的坠落伤害，则对周围僵尸施加范围伤害、减速和击退。
        *   如果事件是玩家对僵尸的近战攻击，则对被击中的僵尸施加击退。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_TAKE_DAMAGE`: 免疫坠落和自我伤害。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 实现坠落冲击波和近战击退。

### 15. Leader - [`src/sourcepawn/perks/survival/LeaderPerk.inc`](src/sourcepawn/perks/survival/LeaderPerk.inc)

*   **职业名称**: `Leader`
*   **职业类型**: 幸存者
*   **介绍**: `Leader`（领袖）拥有一个被动光环，能为附近的盟友增加攻击力。它还能放置一个战旗，为战旗周围的盟友提供更强的攻防增益。
*   **详细介绍**: 这是一个强大的团队辅助职业。它被动地为周围的幸存者提供攻击力光环。其核心技能是按 `E` 键（需蹲下）放置一个战旗。战旗会创造一个更大的增益区域，为范围内的所有盟友提供攻击力和防御力加成。领袖自身也会根据战旗范围内盟友的数量获得额外的攻防加成。战旗有持续时间和冷却时间。
*   **参数 (`Defines`)**:
    *   `ZF_LEADER_SELF_CRIT`: `15` - 永久暴击率加成。
    *   `ZF_LEADER_OTHERS_ATTACK`: `15` - 被动光环提供的攻击力加成。
    *   `ZF_LEADER_OTHERS_RADIUSSQ`: `(350 * 350)` - 被动光环的作用半径（平方）。
    *   `ZF_LEADER_RALLY_SELF_ATTACK`: `5` - 战旗范围内每有一个盟友，自己获得的攻击力加成。
    *   `ZF_LEADER_RALLY_SELF_DEFEND`: `5` - 战旗范围内每有一个盟友，自己获得的防御力加成。
    *   `ZF_LEADER_RALLY_OTHERS_ATTACK`: `15` - 战旗光环提供的攻击力加成。
    *   `ZF_LEADER_RALLY_OTHERS_DEFEND`: `15` - 战旗光环提供的防御力加成。
    *   `ZF_LEADER_RALLY_DURATION`: `90` 秒 - 战旗的持续时间。
    *   `ZF_LEADER_RALLY_COOLDOWN`: `150` 秒 - 战旗的冷却时间。
    *   `ZF_LEADER_RALLY_RADIUSSQ`: `(400 * 400)` - 战旗光环的作用半径（平方）。
*   **逻辑处理**:
    1.  **被动光环**: [`updateCondStats`](src/sourcepawn/perks/survival/LeaderPerk.inc:78) 每帧检测周围的幸存者并为他们施加被动的攻击力加成。
    2.  **放置战旗**: [`onCallForMedic`](src/sourcepawn/perks/survival/LeaderPerk.inc:127) (按`E`键) 触发，在玩家脚下创建一个战旗实体，并启动冷却计时器。
    3.  **战旗光环**: [`updateCondStats`](src/sourcepawn/perks/survival/LeaderPerk.inc:78) 同时检测战旗是否存在。如果存在，则为战旗范围内的盟友施加更强的攻防增益，并根据盟友数量为领袖自己提供额外加成。
    4.  **清理**: [`onDeath`](src/sourcepawn/perks/survival/LeaderPerk.inc:149) 确保在玩家死亡时移除其放置的战旗。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久暴击加成和视觉光环。
    *   `VTABLE_UPDATE_COND_STATS`: 应用被动光环和战旗光环。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 放置战旗。
    *   `VTABLE_ON_DEATH`: 清理战旗。
    *   `VTABLE_ON_PERIODIC`: 管理战旗冷却。

### 16. Mountaineer - [`src/sourcepawn/perks/survival/MountaineerPerk.inc`](src/sourcepawn/perks/survival/MountaineerPerk.inc)

*   **职业名称**: `Mountaineer`
*   **职业类型**: 幸存者
*   **介绍**: `Mountaineer`（登山家）免疫坠落伤害，能够攀爬墙壁，并能使用抓钩快速移动到目标位置。
*   **详细介绍**: 这是一个高机动性职业。它被动地免疫所有坠落伤害。按住跳跃键面向墙壁时，可以向上攀爬。其核心技能是按 `E` 键发射一个抓钩，将自己快速拉向准星瞄准的位置。抓钩技能有冷却时间。
*   **参数 (`Defines`)**:
    *   `MOUNTAINEER_CLIMB_SPEED`: `200.0` - 攀爬墙壁时的向上速度。
    *   `MOUNTAINEER_GRAPPLE_COOLDOWN`: `12` 秒 - 抓钩技能的冷却时间。
    *   `MOUNTAINEER_GRAPPLE_FORCE`: `1000.0` - 抓钩的拉力。
*   **逻辑处理**:
    1.  **坠落免疫**: [`onTakeDamagePost`](src/sourcepawn/perks/survival/MountaineerPerk.inc:100) 将受到的坠落伤害返还为等量治疗。
    2.  **攀爬**: [`onPlayerRunCmd`](src/sourcepawn/perks/survival/MountaineerPerk.inc:108) 在玩家按住跳跃键时，检测前方近距离是否有墙壁。如果有，则持续施加一个向上的速度，实现攀爬。
    3.  **抓钩**: [`onCallForMedic`](src/sourcepawn/perks/survival/MountaineerPerk.inc:155) (按`E`键) 向准星方向进行射线检测。如果击中目标，则计算方向并施加一个强大的力，将玩家拉过去，并进入冷却。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 免疫坠落伤害。
    *   `VTABLE_ON_PLAYER_RUN_CMD`: 处理攀爬逻辑。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 发射抓钩。
    *   `VTABLE_ON_PERIODIC`: 管理抓钩冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示攀爬状态或技能冷却。

### 17. Ninja - [`src/sourcepawn/perks/survival/NinjaPerk.inc`](src/sourcepawn/perks/survival/NinjaPerk.inc)

*   **职业名称**: `Ninja`
*   **职业类型**: 幸存者
*   **介绍**: `Ninja`（忍者）拥有二段跳能力和坠落伤害减免。它能放置一个替身，在受到僵尸近战攻击时瞬移到替身位置，并使替身消失。
*   **详细介绍**: 这是一个高机动性和生存能力的职业。它永久提升移动速度但降低攻击力，并拥有二段跳能力。其核心技能是按 `E` 键在原地设置一个“替身”标记点，并进入一个持续25秒的激活状态。在此状态下，如果受到僵尸的近战攻击，忍者会免疫该次伤害并瞬间传送到之前设置的替身位置，同时在原地留下一个木桩迷惑敌人。
*   **参数 (`Defines`)**:
    *   `ZF_NINJA_ATTACK`: `-40` - 永久攻击力惩罚。
    *   `ZF_NINJA_SPEED`: `50` - 永久移动速度加成。
    *   `ZF_NINJA_DURATION_DECOY_ACTIVE`: `25` 秒 - 替身激活状态的持续时间。
    *   `ZF_NINJA_DURATION_COOLDOWN`: `30` 秒 - 技能冷却时间。
    *   `ZF_NINJA_FALLDMG_RESIST`: `50` - 坠落伤害减免50%。
    *   `ZF_NINJA_FORCE`: `600.0` - 二段跳的力量。
*   **逻辑处理**:
    1.  **二段跳与属性**: [`onPlayerRunCmd`](src/sourcepawn/perks/survival/NinjaPerk.inc:223) 在玩家在空中按跳跃键时施加一个向上的力。[`updateClientPermStats`](src/sourcepawn/perks/survival/NinjaPerk.inc:217) 应用永久属性修改。
    2.  **设置替身**: [`onCallForMedic`](src/sourcepawn/perks/survival/NinjaPerk.inc:273) (按`E`键) 记录当前位置为替身点，并进入激活状态。
    3.  **触发传送**: [`onTakeDamage`](src/sourcepawn/perks/survival/NinjaPerk.inc:253) 在受到伤害时检查。如果处于激活状态且受到僵尸近战攻击，则免疫伤害，调用 [`doNinjaDecoyPlace`](src/sourcepawn/perks/survival/NinjaPerk.inc:324) 执行传送和放置木桩的逻辑，并让木桩在短暂延迟后消失。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_RUN_CMD`: 实现二段跳。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 设置替身。
    *   `VTABLE_ON_TAKE_DAMAGE`: 减免坠落伤害，并在满足条件时触发传送。
    *   `VTABLE_ON_PERIODIC`: 管理技能的激活和冷却计时。
    *   `VTABLE_ON_REMOVE`: 确保玩家更换职业时清理诱饵实体。

### 18. Nonlethal - [`src/sourcepawn/perks/survival/NonlethalPerk.inc`](src/sourcepawn/perks/survival/NonlethalPerk.inc)

*   **职业名称**: `Nonlethal`
*   **职业类型**: 幸存者
*   **介绍**: `Nonlethal`（非致命）的子弹伤害极低，但会根据造成的伤害对僵尸产生强大的击退效果。
*   **详细介绍**: 这是一个纯粹的控制型职业。它的枪械子弹伤害被大幅削减，几乎不具备击杀能力。然而，每一发命中僵尸的子弹都会产生击退效果，造成的伤害越高，击退力度越强。这使得它能有效地控制单个僵尸的位置，阻止其前进。
*   **参数 (`Defines`)**:
    *   `ZF_NONLETHAL_ATTACK_BULLET`: `-90` - 子弹伤害降低90%。
    *   `ZF_NONLETHAL_FORCE`: `50.0` - 伤害到击退力的转化系数。
*   **逻辑处理**:
    1.  **伤害削减**: [`onTakeDamage`](src/sourcepawn/perks/survival/NonlethalPerk.inc:55) 在伤害计算前触发，如果是玩家造成的子弹伤害，则将其大幅降低。
    2.  **击退施加**: [`onTakeDamagePost`](src/sourcepawn/perks/survival/NonlethalPerk.inc:72) 在伤害计算后触发。如果是玩家造成的子弹伤害，则根据最终的伤害值计算击退力度，并施加给僵尸。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE`: 削减子弹伤害。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 施加击退效果。

### 19. Resourceful - [`src/sourcepawn/perks/survival/ResourcefulPerk.inc`](src/sourcepawn/perks/survival/ResourcefulPerk.inc)

*   **职业名称**: `Resourceful`
*   **职业类型**: 幸存者
*   **介绍**: `Resourceful`（足智多谋）在击杀僵尸时能获得补给，拾取弹药包或医疗包时能获得临时的属性增益。
*   **详细介绍**: 这是一个通过多种方式获得奖励的职业。每次击杀僵尸，玩家都会立即回复生命值、弹药和金属。当玩家拾取地图上的弹药包时，会获得一个持续10秒的攻击力加成。当拾取医疗包时，则会获得一个持续10秒的防御力加成。
*   **参数 (`Defines`)**:
    *   `ZF_RESOURCEFUL_AMMOPCT`: `0.20` - 击杀时恢复20%的备用弹药。
    *   `ZF_RESOURCEFUL_ATTACK`: `25` - 拾取弹药包获得的临时攻击力加成。
    *   `ZF_RESOURCEFUL_DEFEND`: `25` - 拾取医疗包获得的临时防御力加成。
    *   `ZF_RESOURCEFUL_HEALTH`: `25` - 击杀时恢复的生命值。
    *   `ZF_RESOURCEFUL_HEALTH_OVERHEAL`: `15` - 击杀时可获得的过量治疗。
    *   `ZF_RESOURCEFUL_METAL`: `25` - 击杀时获得的金属。
    *   `ZF_RESOURCEFUL_PICKUP_DURATION`: `10` 秒 - 拾取物品后增益的持续时间。
*   **逻辑处理**:
    1.  **击杀奖励**: [`onKill`](src/sourcepawn/perks/survival/ResourcefulPerk.inc:59) 在玩家击杀僵尸时，为其回复生命、弹药和金属。
    2.  **拾取弹药**: [`onAmmoPickup`](src/sourcepawn/perks/survival/ResourcefulPerk.inc:68) 在玩家拾取弹药包时，为其提供临时的攻击力加成。
    3.  **拾取医疗**: [`onMedPickup`](src/sourcepawn/perks/survival/ResourcefulPerk.inc:72) 在玩家拾取医疗包时，为其提供临时的防御力加成。
*   **事件处理**:
    *   `VTABLE_ON_KILL`: 提供击杀奖励。
    *   `VTABLE_ON_AMMO_PICKUP`: 提供拾取弹药奖励。
    *   `VTABLE_ON_MED_PICKUP`: 提供拾取医疗奖励。

### 20. RicochetSpecialist - [`src/sourcepawn/perks/survival/RicochetSpecialistPerk.inc`](src/sourcepawn/perks/survival/RicochetSpecialistPerk.inc)

*   **职业名称**: `RicochetSpecialist`
*   **职业类型**: 幸存者
*   **介绍**: `RicochetSpecialist`（反弹专家）的子弹有几率在击中僵尸后反弹到附近的其他僵尸身上，最多反弹两次。
*   **详细介绍**: 这是一个具有群体伤害潜力的职业。玩家的每次子弹攻击在命中僵尸时，都有50%的几率触发反弹。如果触发，子弹会寻找最近的另一个僵尸，对其造成上一次伤害50%的伤害。这个过程可以再次发生，最多连续反弹两次，对三个目标造成伤害。
*   **参数 (`Defines`)**:
    *   `RICOCHET_CHANCE`: `50` - 触发反弹的几率（%）。
    *   `RICOCHET_BOUNCE_DAMAGE_MULTIPLIER`: `0.5` - 每次反弹的伤害衰减系数。
    *   `RICOCHET_MAX_BOUNCES`: `2` - 最大反弹次数。
    *   `RICOCHET_SEARCH_RADIUS`: `400.0` - 寻找下一个反弹目标的半径。
*   **逻辑处理**:
    1.  **触发反弹**: [`onTakeDamagePost`](src/sourcepawn/perks/survival/RicochetSpecialistPerk.inc:72) 在玩家用子弹对僵尸造成伤害后触发。它会进行一次随机检定。
    2.  **反弹链**: 如果检定成功，它会调用 [`Ricochet_Chain`](src/sourcepawn/perks/survival/RicochetSpecialistPerk.inc:113) 计时器函数。该函数负责对当前目标造成伤害，绘制视觉光束，然后寻找下一个目标并创建新的计时器，直到达到最大反弹次数或找不到新目标为止。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 触发初始的反弹效果。

### 21. Scavenger - [`src/sourcepawn/perks/survival/ScavengerPerk.inc`](src/sourcepawn/perks/survival/ScavengerPerk.inc)

*   **职业名称**: `Scavenger`
*   **职业类型**: 幸存者
*   **介绍**: `Scavenger`（拾荒者）在拾取弹药包时有几率获得临时的无限暴击或额外的金属。
*   **详细介绍**: 这是一个依赖地图资源的随机增益职业。每次玩家拾取地上的弹药包时，都会进行一次随机检定。有25%的几率获得持续5秒的100%暴击，或者有50%的几率（在前一个检定失败后）获得50点金属。
*   **参数 (`Defines`)**:
    *   `SCAVENGER_CRIT_CHANCE`: `25` - 获得暴击的几率（%）。
    *   `SCAVENGER_METAL_CHANCE`: `50` - 获得金属的几率（%）。
    *   `SCAVENGER_CRIT_DURATION`: `5` 秒 - 暴击效果的持续时间。
    *   `SCAVENGER_METAL_AMOUNT`: `50` - 获得的金属数量。
*   **逻辑处理**:
    1.  **拾取触发**: [`onAmmoPickup`](src/sourcepawn/perks/survival/ScavengerPerk.inc:54) 是唯一的核心逻辑。当玩家拾取弹药包时，它会进行两次随机检定，并根据结果给予暴击或金属奖励。
*   **事件处理**:
    *   `VTABLE_ON_AMMO_PICKUP`: 触发随机奖励。

### 22. Selfless - [`src/sourcepawn/perks/survival/SelflessPerk.inc`](src/sourcepawn/perks/survival/SelflessPerk.inc)

*   **职业名称**: `Selfless`
*   **职业类型**: 幸存者
*   **介绍**: `Selfless`（无私者）在被僵尸的致命一击杀死时，会触发一次巨大的范围爆炸。
*   **详细介绍**: 这是一个终极牺牲型职业。它的核心能力在于死亡瞬间的复仇。当玩家即将被僵尸的一次攻击杀死时（即伤害大于等于当前生命值），`onTakeDamage` 事件会在玩家死亡前触发一次威力巨大的范围爆炸，对周围的僵尸造成毁灭性打击。这个效果每条命只能触发一次。
*   **参数 (`Defines`)**:
    *   `ZF_SELFLESS_DAMAGE`: `10000` - 爆炸伤害。
    *   `ZF_SELFLESS_RADIUS`: `5000` - 爆炸半径。
*   **逻辑处理**:
    1.  **致命一击检测**: [`onTakeDamage`](src/sourcepawn/perks/survival/SelflessPerk.inc:66) 在玩家受到伤害时触发。它会检查攻击者是否为僵尸，以及该次伤害是否足以杀死玩家。
    2.  **触发爆炸**: 如果满足条件且本条命尚未爆炸过，它会在伤害应用前，以玩家为中心制造一次范围巨大的爆炸，然后才允许玩家正常死亡。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置“已爆炸”状态。
    *   `VTABLE_ON_TAKE_DAMAGE`: 检测致命一击并触发爆炸。

### 23. Specter - [`src/sourcepawn/perks/survival/SpecterPerk.inc`](src/sourcepawn/perks/survival/SpecterPerk.inc)

*   **职业名称**: `Specter`
*   **职业类型**: 幸存者
*   **介绍**: `Specter`（幽灵）是一个玻璃大炮职业，拥有极高的属性加成但生命值上限极低。它能通过按 `E` 键短暂进入无敌和无法攻击的相位状态。
*   **详细介绍**: 该职业为玩家提供了巨额的攻击、暴击和速度加成，但将其最大生命值锁定在60，且无法被治疗。其核心生存技能是按 `E` 键激活“相位移动”，在短时间内变得完全无敌，但同时也无法进行任何攻击。该技能有较长的冷却时间。
*   **参数 (`Defines`)**:
    *   `SPECTER_ATT_BONUS`: `50` - 永久攻击力加成。
    *   `SPECTER_CRIT_BONUS`: `25` - 永久暴击率加成。
    *   `SPECTER_SPEED_BONUS`: `25` - 永久移动速度加成。
    *   `SPECTER_HEALTH`: `60` - 锁定的最大生命值。
    *   `SPECTER_PHASE_DURATION`: `4.0` 秒 - 相位状态的持续时间。
    *   `SPECTER_PHASE_COOLDOWN`: `30` 秒 - 技能冷却时间。
*   **逻辑处理**:
    1.  **属性与生命值锁定**: [`updateClientPermStats`](src/sourcepawn/perks/survival/SpecterPerk.inc:82) 应用永久属性加成。[`onPlayerSpawn`](src/sourcepawn/perks/survival/SpecterPerk.inc:90) 和 [`onPeriodic`](src/sourcepawn/perks/survival/SpecterPerk.inc:97) 持续将玩家的生命值设为上限60。
    2.  **相位移动**: [`onCallForMedic`](src/sourcepawn/perks/survival/SpecterPerk.inc:118) (按`E`键) 触发技能，为玩家施加无敌（`TFCond_Ubercharged`）和无法攻击（`TFCond_Bonked`）的状态，并进入冷却。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性加成。
    *   `VTABLE_ON_PLAYER_SPAWN`: 设置初始生命值并重置冷却。
    *   `VTABLE_ON_PERIODIC`: 锁定生命值上限并管理冷却。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活相位移动。
    *   `VTABLE_UPDATE_COND_STATS`: 更新HUD状态。

### 24. Stash - [`src/sourcepawn/perks/survival/StashPerk.inc`](src/sourcepawn/perks/survival/StashPerk.inc)

*   **职业名称**: `Stash`
*   **职业类型**: 幸存者
*   **介绍**: `Stash`（贮藏）可以放置一个补给箱。在一段时间的准备后，玩家可以与补给箱互动，获得大量补给和强大的临时及永久攻击力加成。
*   **详细介绍**: 这是一个延迟回报型职业。玩家可以按 `E` 键（需蹲下）放置一个补给箱。补给箱在放置后需要经过40秒的“准备期”才能使用。准备完成后，玩家可以走近补给箱将其拾取，立即获得大量生命、全额弹药和金属，同时获得一个永久的攻击力加成和一个持续45秒的巨额临时攻击力加成。放置和拾取补给箱都有很长的冷却时间。
*   **参数 (`Defines`)**:
    *   `STASH_GRAB_ATTACK_DURATION`: `45` 秒 - 拾取后临时攻击力加成的持续时间。
    *   `STASH_GRAB_ATTACK_PERM`: `10` - 拾取后获得的永久攻击力加成。
    *   `STASH_GRAB_ATTACK_TEMP

## 僵尸职业 (Zombie Perks)

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
    1.  **目标选择**: [`doMarkedSelect`](src/sourcepawn/perks/zombie/MarkedPerk.inc:170) 函数负责随机选择一名存活的幸存者作为目标。这个过程在准备阶段结束时 ([`onGraceEnd`](src/sourcepawn/perks/zombie/MarkedPerk.inc:97)) 或上一个目标死亡并经过冷却后触发。
    2.  **伤害修正**: [`onTakeDamage`](src/sourcepawn/perks/zombie/MarkedPerk.inc:101) 事件在僵尸造成伤害时触发。它会判断受害者是否为被标记的目标，如果是，则大幅增加伤害；如果不是，则略微降低伤害。
    3.  **目标死亡处理**: 当被标记的目标死亡时，[`onDeath`](src/sourcepawn/perks/zombie/MarkedPerk.inc:192) 和 [`onPeriodic`](src/sourcepawn/perks/zombie/MarkedPerk.inc:122) 会调用 [`handleTargetDeath`](src/sourcepawn/perks/zombie/MarkedPerk.inc:198)。该函数会启动一个 `ZF_MARKED_TIMER` 秒的计时器，计时结束后会选择一个新的目标。
    4.  **视觉与HUD**: [`updateCondStats`](src/sourcepawn/perks/zombie/MarkedPerk.inc:138) 负责在 `Marked` 僵尸和被标记的幸存者之间绘制一条红色的光束，并在HUD上显示当前目标或选择新目标的倒计时。
    5.  **出生逻辑**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/MarkedPerk.inc:113) 会在玩家出生时启动一个15秒的计时器来选择第一个目标，以确保所有玩家都已进入游戏。
*   **事件处理**:
    *   `VTABLE_ON_GRACE_END`: 选择初始目标。
    *   `VTABLE_ON_TAKE_DAMAGE`: 根据目标修正造成的伤害。
    *   `VTABLE_ON_DEATH`: 处理被标记目标的死亡事件。
    *   `VTABLE_ON_PERIODIC`: 管理目标死亡后的冷却计时器，并检查目标的存活状态。
    *   `VTABLE_ON_PLAYER_SPAWN`: 初始化首次选择目标的计时器。
    *   `VTABLE_UPDATE_COND_STATS`: 更新HUD信息和玩家之间的光束。


### 12. Overlord - [`src/sourcepawn/perks/zombie/OverlordPerk.inc`](src/sourcepawn/perks/zombie/OverlordPerk.inc)

*   **职业名称**: `Overlord`
*   **职业类型**: 僵尸
*   **介绍**: `Overlord`（领主）是一个极其缓慢但非常坚韧的战略型僵尸。它能够放置可以自我繁殖的“菌毯”，为附近的僵尸提供速度和恢复增益。
*   **详细介绍**: 该职业以极低的移动速度换取了强大的防御能力和独特的区域控制手段。通过按 `E` 键，`Overlord` 可以在脚下放置一个菌毯肿瘤。菌毯本身有生命值，可以被幸存者摧毁。只要菌毯存在，它就会为光环范围内的所有僵尸提供移动速度和生命恢复加成。更重要的是，每个菌毯都会在一定时间后尝试在附近“繁殖”，生成一个新的菌毯，直到达到数量上限。这使得 `Overlord` 能够逐步将战场转化为对僵尸有利的地形。
*   **参数 (`Defines`)**:
    *   `OVERLORD_DEFEND`: `25` - 永久防御力加成。
    *   `OVERLORD_SPEED`: `-50` - 永久移动速度惩罚。
    *   `OVERLORD_COOLDOWN`: `30.0` - 放置菌毯的冷却时间。
    *   `OVERLORD_MAX_CREEPS`: `8` - 最大菌毯数量。
    *   `OVERLORD_CREEP_HEALTH`: `400` - 每个菌毯的生命值。
    *   `OVERLORD_CREEP_RADIUS`: `250.0` - 菌毯光环的作用半径。
    *   `OVERLORD_CREEP_LIFETIME`: `60.0` - 菌毯的存在时间。
    *   `OVERLORD_CREEP_REPRODUCE_TIME`: `15.0` - 菌毯的繁殖间隔。
    *   `OVERLORD_BUFF_SPEED`: `20` - 菌毯提供的速度加成。
    *   `OVERLORD_BUFF_REGEN`: `5` - 菌毯提供的生命恢复。
*   **逻辑处理**:
    1.  **放置菌毯**: [`onCallForMedic`](src/sourcepawn/perks/zombie/OverlordPerk.inc:107) (按`E`键) 触发。在检查冷却和数量限制后，会在玩家脚下的地面上调用 [`createCreepAt`](src/sourcepawn/perks/zombie/OverlordPerk.inc:147) 创建一个菌毯实体。
    2.  **菌毯管理**: 该职业使用一个 `DataPack` (`creeps`) 来存储所有菌毯实体及其相关数据（创建时间、上次繁殖时间）。
    3.  **繁殖与光环**: [`updateCondStats`](src/sourcepawn/perks/zombie/OverlordPerk.inc:209) 是最核心的函数，每帧执行以下操作：
        *   遍历所有菌毯，检查其是否超时或被摧毁，并进行清理。
        *   如果菌毯存活且未达到最大数量，则在 `OVERLORD_CREEP_REPRODUCE_TIME` 秒后，尝试在附近寻找一个有效的地面位置生成新的菌毯。
        *   对所有存活的菌毯，检测其光环范围内的僵尸，并为他们施加条件性的速度和生命恢复增益。
    4.  **菌毯受伤**: 菌毯实体被挂钩了 `SDKHook_OnTakeDamage` 到 [`onCreepTakeDamage`](src/sourcepawn/perks/zombie/OverlordPerk.inc:170) 函数，用于处理其受到的伤害和被摧毁的逻辑。
    5.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/OverlordPerk.inc:201) 和 [`onRemove`](src/sourcepawn/perks/zombie/OverlordPerk.inc:205) 会调用 [`cleanupCreeps`](src/sourcepawn/perks/zombie/OverlordPerk.inc:183)，确保在玩家死亡或更换职业时，其创建的所有菌毯都被正确移除。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的防御加成和速度惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 放置菌毯。
    *   `VTABLE_UPDATE_COND_STATS`: 处理菌毯的生命周期、繁殖、光环效果，并更新HUD。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理所有菌毯。


### 13. Phantasm - [`src/sourcepawn/perks/zombie/PhantasmPerk.inc`](src/sourcepawn/perks/zombie/PhantasmPerk.inc)

*   **职业名称**: `Phantasm`
*   **职业类型**: 僵尸
*   **介绍**: `Phantasm`（幻象）是一个脆弱的渗透型僵尸，它能短暂地进入“虚空行走”状态，变得半透明并能穿越墙壁和玩家，但期间无法攻击。
*   **详细介绍**: 该职业以永久降低自身生命值和攻击力为代价，换取了强大的机动和渗透能力。通过按 `E` 键，`Phantasm` 可以激活“虚空行走”，持续一小段时间。在此期间，它会进入 `NOCLIP` 模式，可以无视地形和单位的碰撞，直接穿行。为了平衡，虚空行走期间玩家无法攻击。技能结束后，玩家会短暂眩晕。如果在技能结束时，玩家卡在了实体（如墙壁）内部，会立即死亡。这是一个高风险、高回报的技能，适合用于绕后、侦察或逃生。
*   **参数 (`Defines`)**:
    *   `ZF_PHANTASM_HEALTH_PENALTY`: `-25` - 永久生命值惩罚。
    *   `ZF_PHANTASM_ATTACK_PENALTY`: `-25` - 永久攻击力惩罚。
    *   `ZF_PHANTASM_WALK_DURATION`: `1.0` - 虚空行走的持续时间（秒）。
    *   `ZF_PHANTASM_STUN_DURATION`: `0.5` - 技能结束后的眩晕时间。
    *   `ZF_PHANTASM_COOLDOWN`: `17.0` - 技能的冷却时间。
    *   `ZF_PHANTASM_STUCK_DAMAGE`: `10` - 在虚空行走期间卡住时每秒受到的伤害。
*   **逻辑处理**:
    1.  **技能激活**: [`onCallForMedic`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:100) (按`E`键) 触发。在检查冷却后，它会将玩家的移动类型设为 `MOVETYPE_NOCLIP`，渲染模式设为半透明，并冻结其攻击能力。
    2.  **虚空行走期间**: [`onPeriodic`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:140) 函数管理技能的持续时间。
        *   它会持续检查玩家是否卡在实体内部 ([`IsPlayerStuck`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:125))，如果是，则持续造成伤害。
        *   当 `walk_timer` 计时结束时，它会检查玩家是否仍然卡住。如果卡住，则强制自杀；如果安全，则恢复玩家的正常状态（`MOVETYPE_WALK`，实体渲染），并施加短暂的眩晕。
    3.  **状态重置**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/PhantasmPerk.inc:86) 确保玩家在出生时是正常的实体状态，防止因意外情况导致的状态残留。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的生命值和攻击力惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活虚空行走技能。
    *   `VTABLE_ON_PERIODIC`: 管理虚空行走的状态、计时、卡墙检测和技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能的当前状态（激活中、冷却中、准备就绪）。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置技能状态，确保玩家正常生成。


### 14. Rage - [`src/sourcepawn/perks/zombie/RagePerk.inc`](src/sourcepawn/perks/zombie/RagePerk.inc)

*   **职业名称**: `Rage`
*   **职业类型**: 僵尸
*   **介绍**: `Rage`（狂怒）是一个高风险高回报的职业，它能消耗当前大部分生命值来获得临时的巨额生命和速度加成，但效果会随着生命值降低而消失。
*   **详细介绍**: 该职业的核心技能是“狂怒”。通过按 `E` 键，玩家可以激活此技能，前提是当前生命值百分比高于一个阈值（80%）。激活时，玩家会立即获得基于当前生命值的额外生命值（超过最大生命值上限），并获得一个持续5秒的巨额速度加成。然而，这个“狂怒”状态是不稳定的：一旦玩家的生命值百分比因受到伤害而降回阈值以下，所有增益效果会立即消失。技能使用后会进入冷却。
*   **参数 (`Defines`)**:
    *   `ZF_RAGE_COOLDOWN`: `20` - 技能的冷却时间（秒）。
    *   `ZF_RAGE_SPEED`: `150` - 激活时获得的速度加成。
    *   `ZF_RAGE_SPEED_DURATION`: `5` - 速度加成的持续时间。
    *   `ZF_RAGE_HEALTHPCT_TOUSE`: `0.80` - 使用技能所需的最低生命值百分比（80%）。
    *   `ZF_RAGE_HEALTHPCT_ONUSE`: `1.1` - 激活时获得的额外生命值倍率（当前生命的110%）。
*   **逻辑处理**:
    1.  **技能激活**: [`onCallForMedic`](src/sourcepawn/perks/zombie/RagePerk.inc:103) (按`E`键) 触发。它会检查冷却时间和当前生命值百分比。如果满足条件，则将 `perk_state` 设为 `1`（激活状态），增加玩家生命值，并使用 `addStatTempStack` 施加一个临时的速度加成。
    2.  **状态监控**: [`updateCondStats`](src/sourcepawn/perks/zombie/RagePerk.inc:132) 在狂怒状态下持续监控玩家的生命值。一旦生命值百分比低于 `ZF_RAGE_HEALTHPCT_TOUSE`，它会立即重置 `perk_state`，移除光环和所有条件性属性，提前结束狂怒状态。
    3.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/RagePerk.inc:126) 在技能未激活时每秒减少冷却计时器 `perk_timer`。
    4.  **状态清理**: [`onRemove`](src/sourcepawn/perks/zombie/RagePerk.inc:95) 和 [`onPlayerSpawn`](src/sourcepawn/perks/zombie/RagePerk.inc:155) 确保在玩家更换职业、死亡或重生时，所有狂怒效果都能被正确清理，防止属性残留。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活狂怒技能。
    *   `VTABLE_UPDATE_COND_STATS`: 监控狂怒状态的持续条件，并在HUD上显示状态。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_ON_REMOVE` / `VTABLE_ON_PLAYER_SPAWN`: 清理狂怒效果并重置状态。


### 15. Roar - [`src/sourcepawn/perks/zombie/RoarPerk.inc`](src/sourcepawn/perks/zombie/RoarPerk.inc)

*   **职业名称**: `Roar`
*   **职业类型**: 僵尸
*   **介绍**: `Roar`（咆哮）是一个控场型僵尸，能够通过咆哮击退周围的幸存者，并暂时削弱他们的防御力。
*   **详细介绍**: 该职业的核心技能是按 `E` 键（必须在地面上）发动一次范围巨大的“毁灭咆哮”。咆哮会立即对周围大范围内的所有幸存者产生两个效果：首先，将他们强力地击退，并附加一个向上的力以防止他们被推入地下；其次，对他们施加一个持续性的防御力降低debuff。作为机枪手时，击退的力度和debuff的持续时间都会大幅增加。这是一个强大的团战发起或阵地破坏技能。
*   **参数 (`Defines`)**:
    *   `ZF_ROAR_COOLDOWN`: `15` - 技能的冷却时间（秒）。
    *   `ZF_ROAR_DURATION`: `20` - Debuff的基础持续时间。
    *   `ZF_ROAR_DURATION_HEAVY`: `60` - 作为机枪手时的Debuff持续时间。
    *   `ZF_ROAR_FORCE`: `1200.0` - 基础击退力量。
    *   `ZF_ROAR_FORCE_HEAVY`: `3000.0` - 作为机枪手时的击退力量。
    *   `ZF_ROAR_RADIUS`: `450.0` - 咆哮的作用半径。
    *   `ZF_ROAR_DEFEND`: `-25` - 施加给幸存者的防御力减益。
*   **逻辑处理**:
    1.  **技能激活**: [`onCallForMedic`](src/sourcepawn/perks/zombie/RoarPerk.inc:82) (按`E`键) 触发。在检查冷却和是否在地面上之后，它会遍历所有幸存者。
    2.  **效果施加**: 对于在 `ZF_ROAR_RADIUS` 范围内的每个幸存者：
        *   使用 `addStatTempStack` 施加一个临时的、可叠加的防御力降低debuff。
        *   计算从咆哮者指向幸存者的方向向量，并施加一个巨大的速度（通过 `TeleportEntity`）来实现击退。击退力被赋予了一个向上的分量，以获得更好的效果。
    3.  **冷却管理**: [`onPeriodic`](src/sourcepawn/perks/zombie/RoarPerk.inc:132) 每秒减少冷却计时器 `perk_timer`。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发咆哮技能，对范围内的幸存者施加击退和debuff。
    *   `VTABLE_ON_PERIODIC`: 管理技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能的冷却状态。


### 16. Scorching - [`src/sourcepawn/perks/zombie/ScorchingPerk.inc`](src/sourcepawn/perks/zombie/ScorchingPerk.inc)

*   **职业名称**: `Scorching`
*   **职业类型**: 僵尸
*   **介绍**: `Scorching`（灼热）是一个浑身燃烧的僵尸，移动速度较快但攻击力较低。它免疫火焰伤害，并能通过接触或近战攻击点燃幸存者。
*   **详细介绍**: 这是一个持续燃烧的职业，以永久降低攻击力为代价换取了较高的移动速度。它的核心特性是对火的亲和力：
    *   **自我燃烧**: 除非在水中，否则会持续点燃自己。
    *   **火焰免疫**: 完全免疫来自任何来源的火焰伤害（`DMG_BURN`），并且免疫火焰兵的非近战武器伤害。
    *   **点燃敌人**: 通过近战攻击或直接触碰幸存者，都能将他们点燃。
    *   **特殊交互**: 能够点燃猎人短弓的箭矢，并能通过触碰移除侦察兵的“原子能饮料”效果。同时，它无法使用原子能饮料。
*   **参数 (`Defines`)**:
    *   `ZF_SCORCHING_ATTACK`: `-30` - 永久攻击力惩罚。
    *   `ZF_SCORCHING_SPEED`: `50` - 永久移动速度加成。
    *   `ZF_SCORCH_DURATION`: `10.0` - 点燃幸存者的持续时间。
*   **逻辑处理**:
    1.  **永久属性**: [`updatePermStats`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:63) 在出生时应用永久的速度加成和攻击力惩罚。
    2.  **自我点燃**: [`onPeriodic`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:68) 每秒检查一次，如果玩家不在水里，就使用 `TF2_IgnitePlayer` 点燃自己。
    3.  **火焰免疫**: [`onTakeDamage`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:75) 事件检查受到的伤害。如果是火焰伤害或来自火焰兵的非近战伤害，则将伤害归零。
    4.  **点燃他人**:
        *   [`onTakeDamagePost`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:92) 在造成近战伤害后，点燃受害者。
        *   [`onTouch`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:98) 在触碰到幸存者时，点燃他们并移除其“原子能饮料”效果。
    5.  **武器交互**:
        *   [`onPlayerRunCmd`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:113) 阻止玩家使用“原子能饮料”。
        *   [`onCalcIsAttackCritical`](src/sourcepawn/perks/zombie/ScorchingPerk.inc:129) 在暴击计算时检查玩家是否手持猎人短弓，如果是，则点燃箭矢。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_PERIODIC`: 维持自身的燃烧状态。
    *   `VTABLE_ON_TAKE_DAMAGE`: 实现火焰免疫。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 通过近战攻击点燃敌人。
    *   `VTABLE_ON_TOUCH`: 通过接触点燃敌人。
    *   `VTABLE_ON_PLAYER_RUN_CMD`: 禁用特定武器。
    *   `VTABLE_ON_CALC_IS_ATTACK_CRITICAL`: 实现武器的特殊效果（点燃箭矢）。


### 17. Sick - [`src/sourcepawn/perks/zombie/SickPerk.inc`](src/sourcepawn/perks/zombie/SickPerk.inc)

*   **职业名称**: `Sick`
*   **职业类型**: 僵尸
*   **介绍**: `Sick`（病患）是一个极其脆弱的远程攻击型僵尸，它能像求生之路里的 Spitter 一样，吐出一连串的酸性投射物，在地面形成造成持续伤害的酸液池。
*   **详细介绍**: 该职业以巨额的防御力惩罚为代价，换取了强大的远程区域封锁能力。通过按 `E` 键，`Sick` 会在短时间内连续吐出多发酸液球。这些酸液球会沿着抛物线飞行，在撞击到地面或墙壁时破裂，形成一滩持续15秒的酸液池。任何进入酸液池的幸存者都会受到持续的毒性伤害。这是一个典型的玻璃大炮职业，自身非常脆弱，但能有效地分割战场和压制阵地。
*   **参数 (`Defines`)**:
    *   `ZF_SICK_DEFEND`: `-75` - 永久防御力惩罚。
    *   `ZF_SICK_DAMAGE`: `3.0` - 酸液池每秒造成的伤害。
    *   `ZF_SICK_DAMAGE_RADIUS`: `150.0` - 酸液池的作用半径。
    *   `ZF_SICK_MAX_DIST`: `2000.0` - 投射物的最大飞行距离。
    *   `ZF_SICK_COOLDOWN`: `15.0` - 技能冷却时间。
    *   `ZF_SICK_LIFETIME`: `15.0` - 酸液池的持续时间。
    *   `ZF_SICK_SPIT_COUNT`: `5` - 一次技能吐出的投射物数量。
    *   `ZF_SICK_SPIT_INTERVAL`: `0.2` - 每次吐痰的间隔时间。
    *   `ZF_SICK_PROJECTILE_SPEED`: `1600.0` - 投射物的飞行速度。
*   **逻辑处理**:
    1.  **吐痰序列**: [`onCallForMedic`](src/sourcepawn/perks/zombie/SickPerk.inc:233) (按`E`键) 触发技能。它不会直接创建投射物，而是启动一个重复计时器 [`timer_SpitProjectile`](src/sourcepawn/perks/zombie/SickPerk.inc:253)。该计时器会以 `ZF_SICK_SPIT_INTERVAL` 的间隔重复触发 `ZF_SICK_SPIT_COUNT` 次。
    2.  **投射物创建**: 每次计时器触发时，会调用 `fxCreateModelThrown` 创建一个带有随机角度偏差的投射物，并将其添加到 `projectiles` DataPack 中进行追踪。
    3.  **投射物追踪**: [`onGameFrame`](src/sourcepawn/perks/zombie/SickPerk.inc:438) 逐帧检查所有飞行中的投射物。它使用 `doItemCollide` 函数检测碰撞。
    4.  **酸液池生成**: 一旦检测到碰撞，会调用 [`createAcidPool`](src/sourcepawn/perks/zombie/SickPerk.inc:341) 在碰撞点创建一个作为“酸液池”的实体，并附上粒子特效。同时，将该实体添加到 `pools` DataPack 中。
    5.  **区域伤害**: [`onPeriodic`](src/sourcepawn/perks/zombie/SickPerk.inc:383) 每秒遍历所有有效的酸液池，检测范围内的幸存者并对他们造成伤害。同时，它也负责清理超时的酸液池。
    6.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/SickPerk.inc:188) 和 [`onPlayerSpawn`](src/sourcepawn/perks/zombie/SickPerk.inc:167) 确保在玩家死亡或重生时，所有飞行中的投射物、酸液池和吐痰计时器都被正确清理，防止实体和计时器泄露。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久的防御力惩罚。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 启动吐痰序列计时器。
    *   `VTABLE_ON_GAME_FRAME`: 负责投射物的碰撞检测。
    *   `VTABLE_ON_PERIODIC`: 管理酸液池的生命周期和伤害效果，并处理技能冷却。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_PLAYER_SPAWN`: 清理所有相关实体和计时器。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示技能冷却状态。


### 18. StaticField - [`src/sourcepawn/perks/zombie/StaticFieldPerk.inc`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc)

*   **职业名称**: `StaticField`
*   **职业类型**: 僵尸
*   **介绍**: `StaticField`（静电场）是一个被动的光环型职业，它会持续对周围快速移动的幸存者造成伤害。
*   **详细介绍**: 该职业的核心能力是一个永久存在的、以自身为中心的“静电场”光环。任何进入光环范围的幸存者，如果其移动速度超过一个阈值，就会持续受到电击伤害。伤害量与幸存者的移动速度成正比，移动得越快，受到的伤害就越高。这是一个纯粹的区域控制和反机动职业，能有效地惩罚那些试图利用高机动性进行“放风筝”的幸存者。
*   **参数 (`Defines`)**:
    *   `STATIC_FIELD_RADIUS`: `350.0` - 静电场光环的作用半径。
    *   `STATIC_FIELD_DAMAGE_FACTOR`: `0.02` - 伤害系数，每秒造成的伤害等于 `速度 * 0.02`。
    *   `STATIC_FIELD_AURA_PARTICLE`: `"utaunt_electric_mist"` - 光环的粒子特效名称。
*   **逻辑处理**:
    1.  **光环创建**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:67) 在玩家出生时，创建一个附着在其身上的粒子特效，以可视化光环的存在。
    2.  **伤害计算**: [`onPeriodic`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:88) 是核心逻辑。它每秒遍历所有幸存者，检查他们是否在光环范围内。
        *   如果幸存者在范围内，则获取其当前的速度（`m_vecVelocity`）。
        *   如果速度大于一个较小的值（以忽略站立时的微小抖动），则根据 `STATIC_FIELD_DAMAGE_FACTOR` 计算伤害，并使用 `SDKHooks_TakeDamage` 对其造成电击伤害。
    3.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:74) 和 [`onRemove`](src/sourcepawn/perks/zombie/StaticFieldPerk.inc:81) 确保在玩家死亡或更换职业时，光环的粒子特效被正确移除。
*   **事件处理**:
    *   `VTABLE_ON_PLAYER_SPAWN`: 创建光环的视觉特效。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 清理光环特效。
    *   `VTABLE_ON_PERIODIC`: 核心逻辑，检测范围内的幸存者并根据其速度造成伤害。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示一个静态的“激活”状态。


### 19. Swarming - [`src/sourcepawn/perks/zombie/SwarmingPerk.inc`](src/sourcepawn/perks/zombie/SwarmingPerk.inc)

*   **职业名称**: `Swarming`
*   **职业类型**: 僵尸
*   **介绍**: `Swarming`（蜂群）是一个纯粹的属性修改职业，以牺牲攻击力和防御力为代价，换取了极高的移动速度和极快的重生时间。
*   **详细介绍**: 这是一个简单直接的被动职业，体现了“以量取胜”的理念。选择此职业的僵尸会变得更脆弱，攻击力也更低，但他们的移动速度会大幅提升，并且死亡后的重生时间会缩短到几乎可以忽略不计。这使得他们能够源源不断地冲击幸存者的防线。玩家出生时会有一个苍蝇光环作为视觉提示。
*   **参数 (`Defines`)**:
    *   `ZF_SWARMING_COMBAT`: `-20` - 永久攻击力和防御力惩罚。
    *   `ZF_SWARMING_SPEED`: `50` - 永久移动速度加成。
    *   `ZF_SWARMING_RESPAWNTIME`: `0.5` - 重生时间（秒）。（注意：此定义存在但未在代码中使用，实际重生时间可能由其他全局设置控制）。
*   **逻辑处理**:
    1.  **属性修改**: [`updatePermStats`](src/sourcepawn/perks/zombie/SwarmingPerk.inc:58) 是唯一的核心逻辑。当玩家出生或职业确定时，该函数被调用，通过 `addStat` 函数将定义的三个属性值作为永久（`ZFStatTypePerm`）属性应用给玩家。
    2.  **视觉效果**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/SwarmingPerk.inc:65) 在玩家出生时，为其创建一个苍蝇环绕的粒子光环。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久性的属性增减。
    *   `VTABLE_ON_PLAYER_SPAWN`: 添加视觉光环。


### 20. Tarred - [`src/sourcepawn/perks/zombie/TarredPerk.inc`](src/sourcepawn/perks/zombie/TarredPerk.inc)

*   **职业名称**: `Tarred`
*   **职业类型**: 僵尸
*   **介绍**: `Tarred`（焦油）是一个双重减速型职业。它能通过近战攻击短暂减速敌人，也能吐出一连串的焦油球，在地面形成大范围的减速和减攻速区域。
*   **详细介绍**: 该职业拥有两种施加减速的手段：
    1.  **近战减速**: 每次近战攻击命中幸存者时，都会对其施加一个持续10秒的强力减速debuff。
    2.  **焦油喷吐**: 通过按 `E` 键，`Tarred` 会在短时间内连续吐出多发焦油球。这些焦油球在撞击地面后会形成一滩持续30秒的焦油池。任何进入焦油池的幸存者，其移动速度和攻击速度都会被大幅降低。
*   **参数 (`Defines`)**:
    *   `ZF_TARRED_DURATION_MELEE`: `10` - 近战减速的持续时间（秒）。
    *   `ZF_TARRED_DURATION_SLICK`: `30.0` - 焦油池的持续时间。
    *   `ZF_TARRED_ROF`: `-20` - 焦油池施加的攻击速度惩罚。
    *   `ZF_TARRED_SPEED_MELEE`: `-60` - 近战攻击施加的减速效果。
    *   `ZF_TARRED_SPEED_SLICK`: `-50` - 焦油池施加的减速效果。
    *   `ZF_TARRED_TIMER`: `30` - 吐痰技能的冷却时间。
    *   `ZF_TARRED_RADIUS`: `75.0` - 焦油池的作用半径。
    *   `ZF_TARRED_PROJECTILE_SPEED`: `1600.0` - 投射物的飞行速度。
    *   `ZF_TARRED_SPIT_COUNT`: `5` - 一次技能吐出的投射物数量。
    *   `ZF_TARRED_SPIT_INTERVAL`: `0.2` - 每次吐痰的间隔时间。
*   **逻辑处理**:
    1.  **近战减速**: [`onDealDamagePost`](src/sourcepawn/perks/zombie/TarredPerk.inc:361) 在造成近战伤害后触发，使用 `addStatTempStack` 对受害者施加临时的速度惩罚。
    2.  **吐痰序列**: [`onCallForMedic`](src/sourcepawn/perks/zombie/TarredPerk.inc:226) (按`E`键) 触发技能，启动一个重复计时器 [`timer_SpitTarProjectile`](src/sourcepawn/perks/zombie/TarredPerk.inc:244)，该计时器会以固定间隔重复创建投射物。
    3.  **投射物追踪与焦油池生成**: 这部分逻辑与 `Sick` 职业几乎完全相同。[`onGameFrame`](src/sourcepawn/perks/zombie/TarredPerk.inc:449) 追踪投射物碰撞，碰撞后调用 [`createTarPool`](src/sourcepawn/perks/zombie/TarredPerk.inc:328) 生成焦油池实体。
    4.  **区域减速**: [`updateCondStats`](src/sourcepawn/perks/zombie/TarredPerk.inc:399) 每帧遍历所有焦油池，对范围内的幸存者施加条件性的速度和攻击速度惩罚。
    5.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/TarredPerk.inc:184) 和 [`onPlayerSpawn`](src/sourcepawn/perks/zombie/TarredPerk.inc:165) 确保在玩家死亡或重生时，所有相关实体和计时器都被正确清理。
*   **事件处理**:
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 通过近战攻击施加减速。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 启动吐痰序列计时器。
    *   `VTABLE_ON_GAME_FRAME`: 负责投射物的碰撞检测。
    *   `VTABLE_ON_PERIODIC`: 管理焦油池的生命周期和技能冷却。
    *   `VTABLE_UPDATE_COND_STATS`: 对焦油池内的幸存者施加光环减益，并更新HUD。
    *   `VTABLE_ON_DEATH` / `VTABLE_ON_PLAYER_SPAWN`: 清理所有相关实体和计时器。



### 21. Toxic - [`src/sourcepawn/perks/zombie/ToxicPerk.inc`](src/sourcepawn/perks/zombie/ToxicPerk.inc)

*   **职业名称**: `Toxic`
*   **职业类型**: 僵尸
*   **介绍**: `Toxic`（剧毒）是一个具有双重中毒能力的僵尸，攻击力较低，但能通过近战攻击或被近战攻击时使敌人中毒。静止不动时还能激活一个致命的毒性光环。
*   **详细介绍**: 该职业以永久降低自身攻击力为代价，换来了强大的区域控制和持续伤害能力。它有两种方式施加中毒效果：主动用近战攻击命中幸存者，或在被幸存者近战攻击时反伤对手。此外，当 `Toxic` 僵尸在原地保持静止3秒后，会激活一个可见的毒性光环。光环会对范围内的所有幸存者造成持续伤害，且距离越近伤害越高。一旦移动，光环就会消失，需要重新静止来充能。
*   **参数 (`Defines`)**:
    *   `ZF_TOXIC_DAMAGE_PENALTY`: `-50` - 永久攻击力惩罚。
    *   `ZF_TOXIC_POISON_DURATION`: `12.0` - （定义未使用）
    *   `ZF_TOXIC_POISON_DAMAGE`: `10` - （定义未使用，实际中毒效果由 `ZFCondPoisoned` 控制）
    *   `ZF_TOXIC_AURA_RADIUS`: `500.0` - 毒性光环的作用半径。
    *   `ZF_TOXIC_AURA_DAMAGE`: `10.0` - 毒性光环在最大伤害时的基础值。
    *   `TOXIC_STILL_TIME`: `3.0` 秒 - 激活光环所需的静止时间。
*   **逻辑处理**:
    1.  **永久惩罚**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/ToxicPerk.inc:128) 在出生时应用永久的攻击力惩罚。
    2.  **中毒施加**:
        *   [`onDealDamagePost`](src/sourcepawn/perks/zombie/ToxicPerk.inc:142): 在用近战攻击对幸存者造成伤害后，对受害者施加中毒状态 (`ZFCondPoisoned`)。
        *   [`onTakeDamagePost`](src/sourcepawn/perks/zombie/ToxicPerk.inc:160): 在被幸存者近战攻击后，对攻击者施加中毒状态。
    3.  **光环激活与伤害**: [`onPeriodic`](src/sourcepawn/perks/zombie/ToxicPerk.inc:169) 是核心。它每秒检查玩家的速度：
        *   如果玩家移动，则重置静止计时器并移除光环。
        *   如果玩家静止，则增加计时器，达到 `TOXIC_STILL_TIME` 时激活光环并创建粒子特效。
        *   光环激活时，遍历所有幸存者，对范围内的敌人造成基于距离的伤害。
    4.  **视觉效果**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/ToxicPerk.inc:248) 会将玩家的皮肤染成绿色，以作区分。
    5.  **状态清理**: [`onDeath`](src/sourcepawn/perks/zombie/ToxicPerk.inc:264) 和 [`onRemove`](src/sourcepawn/perks/zombie/ToxicPerk.inc:256) 确保在玩家死亡或更换职业时，光环特效和皮肤颜色被正确移除。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久攻击力惩罚。
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 通过近战攻击使敌人中毒。
    *   `VTABLE_ON_TAKE_DAMAGE_POST`: 通过被近战攻击使敌人中毒。
    *   `VTABLE_ON_PERIODIC`: 管理静止检测、光环激活和光环伤害。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示光环状态（充能中、已激活）。
    *   `VTABLE_ON_PLAYER_SPAWN` / `VTABLE_ON_DEATH` / `VTABLE_ON_REMOVE`: 重置状态、应用/移除视觉效果。



### 22. Vampiric - [`src/sourcepawn/perks/zombie/VampiricPerk.inc`](src/sourcepawn/perks/zombie/VampiricPerk.inc)

*   **职业名称**: `Vampiric`
*   **职业类型**: 僵尸
*   **介绍**: `Vampiric`（吸血鬼）是一个生存型僵尸，拥有少量防御加成、持续的生命恢复，并且能将对幸存者造成的一部分伤害转化为自己的生命值。
*   **详细介绍**: 这是一个简单而有效的被动恢复职业。它为玩家提供了少量永久防御力。其核心能力有两个：首先，它会持续地被动恢复生命值；其次，在对幸存者造成任何类型的伤害后，它都能将造成伤害的一半转化为治疗量返还给自己，并且这种吸血效果可以提供过量治疗。
*   **参数 (`Defines`)**:
    *   `ZF_VAMPIRIC_LIFESTEAL_RATIO`: `0.5` - 造成伤害的50%会转化为治疗量。
    *   `ZF_VAMPIRIC_REGEN`: `5` - 每秒被动恢复的生命值。
    *   `ZF_VAMPIRIC_DEFEND`: `10` - 永久防御力加成。
*   **逻辑处理**:
    1.  **永久加成**: [`updateClientPermStats`](src/sourcepawn/perks/zombie/VampiricPerk.inc:58) 在出生时应用永久的防御力加成。
    2.  **吸血**: [`onDealDamagePost`](src/sourcepawn/perks/zombie/VampiricPerk.inc:62) 在对幸存者造成伤害后触发。它会计算造成伤害的50%，然后调用 `addHealth` 将这部分生命值返还给玩家，并允许过量治疗。
    3.  **被动回血**: [`onPeriodic`](src/sourcepawn/perks/zombie/VampiricPerk.inc:75) 每秒为玩家恢复 `ZF_VAMPIRIC_REGEN` 点生命值，但不允许过量治疗。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御力加成。
    *   `VTABLE_ON_DEAL_DAMAGE_POST`: 实现伤害吸血。
    *   `VTABLE_ON_PERIODIC`: 实现被动生命恢复。



### 23. Vindictive - [`src/sourcepawn/perks/zombie/VindictivePerk.inc`](src/sourcepawn/perks/zombie/VindictivePerk.inc)

*   **职业名称**: `Vindictive`
*   **职业类型**: 僵尸
*   **介绍**: `Vindictive`（复仇）是一个成长型僵尸，通过获得击杀和助攻来永久性地、无上限地提升自己的各项属性。
*   **详细介绍**: 这是一个纯粹的被动成长型职业。它没有任何主动技能或复杂机制。其核心玩法在于参与战斗并获得击杀或助攻。每次完成击杀，玩家都会获得攻击力、防御力、移动速度和暴击率的永久性加成。每次完成助攻，也会获得数值较低的永久性加成。理论上，只要能持续参与战斗，该职业可以成长为一个势不可挡的怪物。
*   **参数 (`Defines`)**:
    *   `ZF_VINDICTIVE_KILL_DAMAGE_BONUS`: `20` - 每次击杀获得的攻击力加成。
    *   `ZF_VINDICTIVE_KILL_RESIST_BONUS`: `20` - 每次击杀获得的防御力加成。
    *   `ZF_VINDICTIVE_KILL_SPEED_BONUS`: `20` - 每次击杀获得的速度加成。
    *   `ZF_VINDICTIVE_KILL_CRIT_BONUS`: `20` - 每次击杀获得的暴击率加成。
    *   `ZF_VINDICTIVE_ASSIST_DAMAGE_BONUS`: `10` - 每次助攻获得的攻击力加成。
    *   `ZF_VINDICTIVE_ASSIST_RESIST_BONUS`: `10` - 每次助攻获得的防御力加成。
    *   `ZF_VINDICTIVE_ASSIST_SPEED_BONUS`: `10` - 每次助攻获得的速度加成。
    *   `ZF_VINDICTIVE_ASSIST_CRIT_BONUS`: `10` - 每次助攻获得的暴击率加成。
*   **逻辑处理**:
    1.  **击杀奖励**: [`onKill`](src/sourcepawn/perks/zombie/VindictivePerk.inc:63) 在玩家完成击杀时触发，使用 `addStat` 为玩家永久增加攻击、防御、速度和暴击属性。
    2.  **助攻奖励**: [`onAssistKill`](src/sourcepawn/perks/zombie/VindictivePerk.inc:70) 在玩家完成助攻时触发，同样为玩家永久增加各项属性，但数值较低。
*   **事件处理**:
    *   `VTABLE_ON_KILL`: 在击杀时获得永久属性加成。
    *   `VTABLE_ON_ASSIST_KILL`: 在助攻时获得永久属性加成。



### 24. Volatile - [`src/sourcepawn/perks/zombie/VolatilePerk.inc`](src/sourcepawn/perks/zombie/VolatilePerk.inc)

*   **职业名称**: `Volatile`
*   **职业类型**: 僵尸
*   **介绍**: `Volatile`（不稳定）是一个自杀式攻击职业，它能将被动吸收的伤害转化为能量，并通过按 `E` 键主动引爆，对周围造成巨大伤害。
*   **详细介绍**: 这是一个高风险、高回报的职业。它被动地将受到的所有伤害的50%储存为“能量”。储存的能量会随着时间缓慢衰减。玩家可以随时按 `E` 键来引爆自己，引爆的伤害与当前储存的能量值成正比。引爆后玩家会立即死亡。这是一个典型的“一换多”战术职业，适合在被围攻或冲入敌阵时使用，将自己受到的伤害加倍奉还给敌人。
*   **参数 (`Defines`)**:
    *   `VOLATILE_ENERGY_STORE_RATIO`: `0.5` - 将受到伤害的50%储存为能量。
    *   `VOLATILE_ENERGY_DECAY`: `8.0` - 每秒能量衰减值。
    *   `VOLATILE_MAX_ENERGY`: `150.0` - 可储存的最大能量值。
    *   `VOLATILE_EXPLOSION_RADIUS`: `200.0` - 爆炸的作用半径。
    *   `VOLATILE_SUICIDE_MULTIPLIER`: `1.0` - 爆炸伤害与能量的转化率。
*   **逻辑处理**:
    1.  **能量储存**: [`onTakeDamage`](src/sourcepawn/perks/zombie/VolatilePerk.inc:84) 在玩家受到任何伤害时触发，将伤害值的一半累加到私有数据 `energy` 中，有最大值限制。
    2.  **能量衰减**: [`onPeriodic`](src/sourcepawn/perks/zombie/VolatilePerk.inc:117) 每秒减少储存的能量，并播放一个视觉特效提示玩家当前有能量。
    3.  **主动引爆**: [`onCallForMedic`](src/sourcepawn/perks/zombie/VolatilePerk.inc:99) (按`E`键) 触发自爆。如果玩家有能量且本条命未爆炸过，则会以自身为中心制造一次范围伤害，伤害量等于当前能量值，然后强制玩家自杀。
    4.  **状态重置**: [`onPlayerSpawn`](src/sourcepawn/perks/zombie/VolatilePerk.inc:79) 在玩家每次出生时，将能量和“已爆炸”标记清零。
*   **事件处理**:
    *   `VTABLE_ON_TAKE_DAMAGE`: 吸收受到的伤害并储存为能量。
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 触发主动自爆。
    *   `VTABLE_ON_PERIODIC`: 处理能量的自然衰减。
    *   `VTABLE_UPDATE_COND_STATS`: 在HUD上显示当前储存的能量值。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置能量和爆炸状态。
