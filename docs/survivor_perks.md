# 幸存者职业 (Survivor Perks)

本章节详细解析了所有幸存者职业。

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

### 25. StirCrazy - [`src/sourcepawn/perks/survival/StirCrazyPerk.inc`](src/sourcepawn/perks/survival/StirCrazyPerk.inc)

*   **职业名称**: `StirCrazy`
*   **职业类型**: 幸存者
*   **介绍**: `StirCrazy`（幽居病）是一个通过不断移动来获得攻击力加成的职业，但会降低自身防御力。击杀僵尸能永久提升移动速度。
*   **详细介绍**: 该职业鼓励玩家保持高机动性。它会永久降低玩家的防御力。其核心机制是根据玩家在过去几秒内的移动距离来动态计算攻击力加成。移动范围越大，获得的攻击力就越高。此外，每次击杀僵尸都会带来微量的永久速度提升，滚雪球效应明显。
*   **参数 (`Defines`)**:
    *   `ZF_STIRCRAZY_MAX_POINTS`: `5` - 用于计算平均位置的位置样本数量。
    *   `ZF_STIRCRAZY_DIST_MIN`: `150.0` - 开始获得攻击力加成的最小移动距离。
    *   `ZF_STIRCRAZY_DIST_MAX`: `750.0` - 获得最大攻击力加成的移动距离。
    *   `ZF_STIRCRAZY_ATTACK`: `30` - 移动时可获得的最大攻击力加成。
    *   `ZF_STIRCRAZY_SPEED_ON_KILL`: `1` - 每次击杀获得的永久速度加成。
    *   `ZF_STIRCRAZY_DEFEND`: `-15` - 永久防御力惩罚。
*   **逻辑处理**:
    1.  **永久属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/survival/StirCrazyPerk.inc:98) 在出生时应用永久的防御力惩罚。
    2.  **位置记录**: [`updateCondStats`](src/sourcepawn/perks/survival/StirCrazyPerk.inc:66) 每帧记录玩家当前的位置到一个循环数组中。
    3.  **动态攻击加成**: 它会计算当前位置与过去一段时间内平均位置的距离。距离越大，提供的条件性攻击力加成（`ZFStatTypeCond`）就越高。
    4.  **击杀奖励**: [`onKill`](src/sourcepawn/perks/survival/StirCrazyPerk.inc:92) 在玩家击杀僵尸时，为其永久增加移动速度。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久防御惩罚。
    *   `VTABLE_ON_PLAYER_SPAWN`: 重置位置记录。
    *   `VTABLE_UPDATE_COND_STATS`: 根据移动距离动态应用攻击力加成。
    *   `VTABLE_ON_KILL`: 提供永久速度加成。

### 26. Tantrum - [`src/sourcepawn/perks/survival/TantrumPerk.inc`](src/sourcepawn/perks/survival/TantrumPerk.inc)

*   **职业名称**: `Tantrum`
*   **职业类型**: 幸存者
*   **介绍**: `Tantrum`（暴怒）可以主动激活一个短暂的暴击状态，但之后会进入一个移动速度大幅降低的虚弱期。
*   **详细介绍**: 这是一个典型的“爆发-虚弱”循环职业。玩家可以按 `E` 键进入“暴怒”状态，获得持续15秒的100%暴击率。暴怒结束后，玩家会立即进入一个持续30秒的“精疲力竭”状态，期间移动速度会受到严重惩罚。
*   **参数 (`Defines`)**:
    *   `ZF_TANTRUM_ACTIVE`: `15` 秒 - 暴怒（暴击）状态的持续时间。
    *   `ZF_TANTRUM_COOLDOWN`: `30` 秒 - 精疲力竭（减速）状态的持续时间。
    *   `ZF_TANTRUM_SPEED`: `-100` - 精疲力竭状态下的速度惩罚。
*   **逻辑处理**:
    1.  **激活技能**: [`onCallForMedic`](src/sourcepawn/perks/survival/TantrumPerk.inc:79) (按`E`键) 触发技能，设置一个总时长为45秒的计时器到Perk私有数据中，并播放暴击声效。
    2.  **状态管理**: [`onPeriodic`](src/sourcepawn/perks/survival/TantrumPerk.inc:89) 每秒减少计时器。当计时器从 `31` 减到 `30` 时，暴击效果结束，虚弱期开始。计时器归零时，技能恢复。
    3.  **应用效果**: [`updateCondStats`](src/sourcepawn/perks/survival/TantrumPerk.inc:108) 根据计时器的值应用不同的效果：
        *   如果计时器 > 30 (暴怒期)，应用100%暴击。
        *   如果 0 < 计时器 <= 30 (虚弱期)，应用减速惩罚。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 激活暴怒状态。
    *   `VTABLE_ON_PERIODIC`: 管理技能的两个阶段计时。
    *   `VTABLE_UPDATE_COND_STATS`: 根据当前阶段应用暴击或减速，并更新HUD。

### 27. Trapper - [`src/sourcepawn/perks/survival/TrapperPerk.inc`](src/sourcepawn/perks/survival/TrapperPerk.inc)

*   **职业名称**: `Trapper`
*   **职业类型**: 幸存者
*   **介绍**: `Trapper`（陷阱猎人）可以放置爆炸地雷，对触发的僵尸造成巨大伤害。
*   **详细介绍**: 这是一个区域封锁和陷阱职业。玩家可以蹲下按 `E` 键在面前的地面上放置一个地雷。地雷在僵尸靠近时会自动引爆，造成范围伤害。放置地雷有冷却时间，并且同时存在的地雷数量有上限。特别地，如果“磁力”僵尸靠近，地雷会被无效化而不是爆炸。
*   **参数 (`Defines`)**:
    *   `ZF_TRAPPER_MAX_ITEMS`: `5` - 最多可同时存在的地雷数量。
    *   `ZF_TRAPPER_DAMAGE`: `200` - 地雷的爆炸伤害。
    *   `ZF_TRAPPER_RADIUS`: `150` - 地雷的爆炸半径。
    *   `ZF_TRAPPER_RADIUSSQ`: `(200 * 200)` - 地雷的触发半径（平方）。
    *   `ZF_TRAPPER_TIMER`: `20` 秒 - 放置地雷的冷却时间。
*   **逻辑处理**:
    1.  **放置地雷**: [`onCallForMedic`](src/sourcepawn/perks/survival/TrapperPerk.inc:156) (按`E`键，需蹲下) 触发。在检查冷却和放置条件后，它会向玩家前方进行射线检测以找到合适的地面位置，然后创建一个地雷实体。
    2.  **触发检测**: [`updateCondStats`](src/sourcepawn/perks/survival/TrapperPerk.inc:64) 每帧遍历所有地雷，并检测所有僵尸与地雷的距离。
    3.  **爆炸/失效**: 如果有僵尸进入触发范围，地雷会爆炸，对范围内的所有僵尸造成伤害。但如果触发者是“磁力”僵尸，地雷只会产生火花并消失，不会造成伤害。
    4.  **清理**: [`onDeath`](src/sourcepawn/perks/survival/TrapperPerk.inc:232) 确保在玩家死亡时，其放置的所有地雷都会自爆，防止实体泄露。
*   **事件处理**:
    *   `VTABLE_ON_CALL_FOR_MEDIC`: 放置地雷。
    *   `VTABLE_UPDATE_COND_STATS`: 检测地雷的触发条件并执行爆炸/失效逻辑，更新HUD。
    *   `VTABLE_ON_PERIODIC`: 管理放置冷却。
    *   `VTABLE_ON_DEATH`: 清理所有地雷。

### 28. Turtle - [`src/sourcepawn/perks/survival/TurtlePerk.inc`](src/sourcepawn/perks/survival/TurtlePerk.inc)

*   **职业名称**: `Turtle`
*   **职业类型**: 幸存者
*   **介绍**: `Turtle`（乌龟）是一个极端防御型职业，以牺牲所有进攻和机动能力为代价，换取极高的防御力并免疫背刺。
*   **详细介绍**: 这是一个纯粹的“坦克”职业。它会大幅降低玩家的攻击力、移动速度，并完全移除暴击能力。作为交换，它提供了巨额的防御力加成，并且完全免疫僵尸的背刺攻击。当一次背刺被格挡时，攻击者会被短暂眩晕。
*   **参数 (`Defines`)**:
    *   `ZF_TURTLE_ATTACK`: `-40` - 永久攻击力惩罚。
    *   `ZF_TURTLE_DEFEND`: `75` - 永久防御力加成。
    *   `ZF_TURTLE_SPEED`: `-100` - 永久移动速度惩罚。
    *   `ZF_TURTLE_CRIT`: `-100` - 永久暴击率惩罚。
    *   `ZF_TURTLE_STUN_DURATION`: `1.0` 秒 - 格挡背刺后对攻击者的眩晕时间。
*   **逻辑处理**:
    1.  **属性修改**: [`updateClientPermStats`](src/sourcepawn/perks/survival/TurtlePerk.inc:63) 在出生时应用所有永久属性修改。
    2.  **背刺免疫**: [`onTakeDamage`](src/sourcepawn/perks/survival/TurtlePerk.inc:72) 在受到伤害时检查。如果攻击是背刺，则将伤害归零，并眩晕攻击者。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用永久属性修改。
    *   `VTABLE_ON_TAKE_DAMAGE`: 实现背刺免疫和反制眩晕。

### 29. Wise - [`src/sourcepawn/perks/survival/WisePerk.inc`](src/sourcepawn/perks/survival/WisePerk.inc)

*   **职业名称**: `Wise`
*   **职业类型**: 幸存者
*   **介绍**: `Wise`（智者）初始属性较低，但通过战斗（击杀、助攻、被近战攻击）来不断成长，永久提升攻击力和防御力。
*   **详细介绍**: 这是一个成长型职业。初始状态下，玩家的攻击和防御都有惩罚。但通过以下方式可以获得永久属性提升：
    *   每次击杀僵尸，增加攻击力。
    *   每次助攻，少量增加攻击力。
    *   每次被僵尸近战攻击，少量降低防御力惩罚（即提升防御），直到防御惩罚被完全抵消。
*   **参数 (`Defines`)**:
    *   `ZF_WISE_ATTACK_KILL`: `3` - 每次击杀获得的永久攻击力。
    *   `ZF_WISE_ATTACK_ASSIST`: `2` - 每次助攻获得的永久攻击力。
    *   `ZF_WISE_DEFEND`: `-1` - 每次被近战攻击时，防御力惩罚减少的值（实际效果是+1防御）。
    *   `ZF_WISE_DEFEND_LIMIT`: `20` - 防御力可提升的上限。
    *   `ZF_WISE_ATTACK_DEBUFF`: `-10` - 初始攻击力惩罚。
    *   `ZF_WISE_DEFEND_DEBUFF`: `10` - 初始防御力惩罚。
*   **逻辑处理**:
    1.  **初始惩罚**: [`updateClientPermStats`](src/sourcepawn/perks/survival/WisePerk.inc:57) 在出生时应用初始的攻击和防御惩罚。
    2.  **提升攻击**: [`onKill`](src/sourcepawn/perks/survival/WisePerk.inc:72) 和 [`onAssistKill`](src/sourcepawn/perks/survival/WisePerk.inc:79) 在玩家完成击杀或助攻时，为其永久增加攻击力。
    3.  **提升防御**: [`onTakeDamage`](src/sourcepawn/perks/survival/WisePerk.inc:62) 在玩家被僵尸近战攻击时，为其永久增加防御力，直到达到上限。
*   **事件处理**:
    *   `VTABLE_UPDATE_CLIENT_PERM_STATS`: 应用初始属性惩罚。
    *   `VTABLE_ON_KILL`: 击杀时成长攻击力。
    *   `VTABLE_ON_ASSIST_KILL`: 助攻时成长攻击力。
    *   `VTABLE_ON_TAKE_DAMAGE`: 被近战攻击时成长防御力。