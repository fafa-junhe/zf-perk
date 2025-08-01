

# Zombie Fortress 职业技能文档

本文档详细解析了 `zf_perk.inc` 文件中定义的所有幸存者和僵尸职业。

## 幸存者职业 (Survivor Perks)

共有 `TOTAL_SUR_PERKS` (22) 个幸存者职业。

---

### 1. 运动员 (Athletic)

*   **ID:** `ZF_PERK_ATHLETIC` (1)
*   **介绍:** 运动员(Athletic)——高移动力
*   **详细描述:** 你的移动力和攻击速度大幅增加。但是你的攻击力降低,而且无法造成随机暴击。“嘿!你们根本追不上我!” 推荐职业: 火焰兵、爆破骑士、医生。
*   **参数 (Defines):**
    *   `ZF_ATHLETIC_ATTACK = -40;` // Innate attack penalty.
    *   `ZF_ATHLETIC_CRIT = -100;` // Innate crit penalty.
    *   `ZF_ATHLETIC_ROF = 100;` // Innate rate of fire bonus.
    *   `ZF_ATHLETIC_SPEED = 100;` // Innate speed bonus.
*   **逻辑处理:**
    *   **永久属性:** 该职业的逻辑完全通过永久属性加成实现。
    *   在 `updateClientPermStats` 函数中，如果玩家是存活的幸存者且选择了此职业 (`usingSurPerk(client, ZF_PERK_ATHLETIC)`), 会通过 `addStat` 函数应用以下永久状态：
        *   攻击力 (`ZFStatAtt`) 减少 `ZF_ATHLETIC_ATTACK` (-40%)。
        *   暴击率 (`ZFStatCrit`) 减少 `ZF_ATHLETIC_CRIT` (-100%)，即无法暴击。
        *   攻击速度 (`ZFStatRof`) 增加 `ZF_ATHLETIC_ROF` (100%)。
        *   移动速度 (`ZFStatSpeed`) 增加 `ZF_ATHLETIC_SPEED` (100%)。
*   **事件处理:**
    *   `perk_OnPlayerSpawn`: 在玩家重生时，通过 `updateClientPermStats` 应用上述属性。

---

### 2. 木工 (Carpenter)

*   **ID:** `ZF_PERK_CARPENTER` (2)
*   **介绍:** 木工(Carpenter)——建造障碍物
*   **详细描述:** 你有防御力加成,但是你的攻击力减弱。发医生语音来建造一个500点生命值的路障,敌我均可破坏。冷却时间25秒。同时最多拥有4个路障。“此路不通。” 推荐职业: 火焰兵、工程师、医生。
*   **参数 (Defines):**
    *   `CARPENTER_ATTACK = -40;` // Innate attack penalty.
    *   `CARPENTER_DEFEND = 25;` // Innate defense bonus.
    *   `CARPENTER_BARRICADE_HEALTH = 500;` // Health of each barricade.
    *   `CARPENTER_COOLDOWN = 25;` // Duration after barricade is placed before a new one can be placed.
    *   `CARPENTER_MAX_ITEMS = 4;` // Maximum number of barricades that can be active at one time.
    *   `CARPENTER_DROP_RADSQ_BARRICADE = (250 * 250);` // Radius (squared) inwhich no barricades must be to place barricade.
    *   `CARPENTER_DROP_RADSQ_CLIENT = (150 * 150);` // Radius (squared) inwhich no other players must be to place barricade.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `CARPENTER_ATTACK` (-40%) 攻击惩罚和 `CARPENTER_DEFEND` (25%) 防御加成。
    *   **条件逻辑 (`updateCondStats`):**
        *   使用 `zf_perkTimer[client]` 作为技能冷却计时器。每秒减1。
        *   当冷却结束时 (`zf_perkTimer[client] == 0`)，会提示玩家 "你可以放置一个新的障碍物了."。
        *   HUD文本 (`zf_perkStr`) 会根据冷却状态和路障数量 (`getFreeItemIndex`) 显示 "(障碍物准备就绪)" 或 "(障碍物数量已满)"。
    *   **核心功能 (`doCarpenterBuild`):**
        *   计算玩家面前的位置和角度。
        *   使用 `fxCreateParticle` 和 `fxCreateSoundToAll` 创建建造特效和音效。
        *   调用 `fxCreateModelStatic` 创建一个静态的栅栏模型 (`ZFMDL_FENCE`)。这个模型是实体，可以被破坏。
        *   返回创建的实体ID。
    *   **路障伤害处理 (`perk_OnFenceTakeDamage`):**
        *   当路障实体受到伤害时触发。
        *   通过 `getItemMetadata` 获取并更新路障的生命值。
        *   如果生命值小于等于0，路障被摧毁，播放音效并调用 `removeItem`。
        *   否则，通过 `SetEntityRenderColor` 更新路障颜色以显示其损坏程度。
*   **事件处理:**
    *   `perk_OnCallForMedic` (呼叫医生):
        *   这是放置路障的触发器。
        *   检查冷却时间 (`zf_perkTimer[client] == 0`)、玩家是否在地面上 (`isGrounded`) 和是否蹲下 (`isCrouching`)。
        *   检查附近是否有其他玩家 (`CARPENTER_DROP_RADSQ_CLIENT`) 或其他路障 (`CARPENTER_DROP_RADSQ_BARRICADE`)。
        *   所有条件满足后，调用 `doCarpenterBuild` 创建路障，并将其ID存入 `zf_item[client][]` 数组。
        *   使用 `setItemMetadata` 将路障的初始生命值 `CARPENTER_BARRICADE_HEALTH` 存入其元数据。
        *   使用 `SDKHook` 将 `perk_OnFenceTakeDamage` 挂接到路障的 `SDKHook_OnTakeDamage` 事件上。

---

### 3. 慈善家 (Charitable)

*   **ID:** `ZF_PERK_CHARITABLE` (3)
*   **介绍:** 慈善家(Charitable)——击杀换取礼物
*   **详细描述:** 你的每个击杀和助攻都会增加礼物点数。发医生语音来消耗礼物点数,放出礼物,可被其他的幸存者捡起。礼物能给予生命值回复和短暂的攻击力加成。“人人都有礼物拿!” 推荐职业: 士兵、爆破手、狙击手。
*   **参数 (Defines):**
    *   `ZF_CHARITABLE_MAX_ITEMS = 5;` // Maximum number of gifts that can be active at one time.
    *   `ZF_CHARITABLE_POINTS_ASSIST = 2;` // Points earned per assist.
    *   `ZF_CHARITABLE_POINTS_KILL = 2;` // Points earned per kill.
    *   `ZF_CHARITABLE_POINTS_GIFT = 4;` // Points needed to toss gift.
    *   `ZF_CHARITABLE_GIFT_BONUS_HEALTH = 75;` // Health gained by gift owner when gift is picked up.
    *   `ZF_CHARITABLE_GIFT_BONUS_MIN = 10;` // Minimum gift bonus strength.
    *   `ZF_CHARITABLE_GIFT_BONUS_MAX = 30;` // Maximum gift bonus strength.
    *   `ZF_CHARITABLE_GIFT_DURATION = 20;` // Duration of gift bonuses.
*   **逻辑处理:**
    *   **点数系统:** 使用 `zf_perkState[client]` 存储礼物点数。
    *   **条件逻辑 (`updateCondStats`):** 更新HUD文本 (`zf_perkStr`) 显示可用的礼物数量 `(zf_perkState[thisSur] / ZF_CHARITABLE_POINTS_GIFT)`。
    *   **礼物拾取 (`perk_OnCharitableGiftTouched`):**
        *   当礼物实体被触碰时触发。
        *   礼物所有者不能拾取。
        *   其他幸存者拾取后，随机选择一个属性 (`ZFStat`)，并给予一个 `ZF_CHARITABLE_GIFT_BONUS_MIN` 到 `ZF_CHARITABLE_GIFT_BONUS_MAX` 之间的随机加成，持续 `ZF_CHARITABLE_GIFT_DURATION` 秒。这是通过 `addStatTempStack` 实现的。
        *   礼物所有者会回复 `ZF_CHARITABLE_GIFT_BONUS_HEALTH` 的生命值。
        *   播放特效 (`fxExplosionParty`) 并移除礼物 (`removeItem`)。
*   **事件处理:**
    *   `perk_OnPlayerDeath`:
        *   当僵尸被该玩家击杀时，`zf_perkState[killer]` 增加 `ZF_CHARITABLE_POINTS_KILL`。
        *   当僵尸被其他玩家击杀，该玩家助攻时，`zf_perkState[assist]` 增加 `ZF_CHARITABLE_POINTS_ASSIST`。
    *   `perk_OnCallForMedic`:
        *   检查玩家是否有足够的点数 (`zf_perkState[client] >= ZF_CHARITABLE_POINTS_GIFT`)。
        *   如果可以，消耗点数，并调用 `doItemThrow` 扔出一个礼物模型 (`ZFMDL_PRESENT`)。
        *   使用 `SDKHook` 将 `perk_OnCharitableGiftTouched` 挂接到礼物的 `SDKHook_Touch` 事件上。

---

### 4. 懦夫 (Cowardly)

*   **ID:** `ZF_PERK_COWARDLY` (4)
*   **介绍:** 懦夫(Cowardly)——随时准备逃跑
*   **详细描述:** 当你被攻击时,自动激活你的被动能力“恐慌”。恐慌会给你防御力和速度加成。恐慌能力持续时间5秒,冷却时间30秒。“别守着了,快逃命吧!” 推荐职业: 工程师、医生、狙击手。
*   **参数 (Defines):**
    *   `ZF_COWARDLY_DEFEND = 50;` // Defense bonus when scared.
    *   `ZF_COWARDLY_SPEED = 200;` // Speed bonus when scared.
    *   `ZF_COWARDLY_DURATION_SCARED = 5;` // Duration of scared state after being hit.
    *   `ZF_COWARDLY_DURATION_COOLDOWN = 30;` // Duration after scared state ends before scared state is again possible.
*   **逻辑处理:**
    *   **状态机:** 使用 `zf_perkTimer[client]` 管理状态。
        *   `timer > COOLDOWN`: 恐慌激活状态。
        *   `0 < timer <= COOLDOWN`: 冷却状态。
        *   `timer == 0`: 准备就绪状态。
    *   **条件逻辑 (`updateCondStats`):**
        *   如果 `zf_perkTimer` 大于 `ZF_COWARDLY_DURATION_COOLDOWN`，则应用 `ZF_COWARDLY_DEFEND` 防御加成和 `ZF_COWARDLY_SPEED` 速度加成。
        *   每秒减少 `zf_perkTimer`。
        *   在状态切换时（如 `timer == COOLDOWN` 或 `timer == 0`）向玩家显示提示信息。
        *   HUD文本 (`zf_perkStr`) 在准备就绪时显示 "(随时准备跑路)"。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   当玩家被僵尸近战攻击时触发。
        *   如果技能准备就绪 (`zf_perkTimer[victim] == 0`)，则激活恐慌状态。
        *   设置 `zf_perkTimer[victim]` 为 `SCARED + COOLDOWN` 的总时长。
        *   立即应用防御和速度加成。
        *   播放音效 (`fxYikes`) 并使本次伤害无效 (`damage = 0.0`)。

---

### 5. 伙计 (Friend)

*   **ID:** `ZF_PERK_FRIEND` (5)
*   **介绍:** 伙计(Friend)——和伙伴获得加成
*   **详细描述:** 在准备时间内,对着目标发医生语音,可将目标设为伙伴。(未选择则由系统产生) 靠近同伴时,你获得攻击力和生命回复加成。当你和同伴击杀或助攻时,你的暴击时间就会增加。当同伴死亡后,你就会获得对应时间的暴击。“两人搭配,干活不累!” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_FRIEND_ATTACK = 25;` // Attack bonus when near friend.
    *   `ZF_FRIEND_REGEN = 10;` // Regen bonus when near friend.
    *   `ZF_FRIEND_CRITTIME_INIT = 0;` // Starting crit time.
    *   `ZF_FRIEND_CRITTIME_KILL = 4;` // Crit time added per kill.
    *   `ZF_FRIEND_CRITTIME_ASSIST = 2;` // Crit time added per assist.
    *   `ZF_FRIEND_RADIUSSQ = (300 * 300);` // Radius (squared) inwhich passive bonuses apply.
*   **逻辑处理:**
    *   **状态变量:**
        *   `zf_perkState[client]`: 存储伙伴的客户端ID。
        *   `zf_perkTimer[client]`: 存储累积的暴击秒数。
    *   **伙伴选择 (`doFriendSelect`):**
        *   如果指定了伙伴，则直接设置。
        *   否则，在存活的幸存者中随机选择一个。
        *   设置 `zf_perkState` 和 `zf_perkTimer`，并调用 `createIcon` 在伙伴头顶创建图标。
    *   **条件逻辑 (`updateCondStats`):**
        *   **伙伴存活时:**
            *   检查玩家与伙伴的距离 (`GetVectorDistance`) 是否在 `ZF_FRIEND_RADIUSSQ` 内。
            *   如果在范围内，双方都通过 `addHealth` 获得 `ZF_FRIEND_REGEN` 生命回复，并通过 `addStat` 获得 `ZF_FRIEND_ATTACK` 攻击加成。
        *   **伙伴死亡时:**
            *   如果伙伴死亡 (`validLivingSur(zf_perkState[thisSur])` 为假)，并且 `zf_perkState` 尚未清零。
            *   将 `zf_perkState` 设为0，表示伙伴已死。
            *   调用 `addCondKritz` 给予玩家 `zf_perkTimer` 秒的暴击。
            *   播放特效 (`fxKritzStart`, `fxDeathScream`)，创建光环 (`createAura`)，并移除图标 (`removeIcon`)。
        *   **暴击时间衰减:** 当伙伴死亡后，`zf_perkTimer` 每秒递减，直到为0，届时暴击效果结束。
    *   **HUD文本 (`zf_perkStr`)** 显示当前的暴击时间。
*   **事件处理:**
    *   `perk_OnGraceEnd`: 如果玩家在准备阶段结束时还未选择伙伴，系统会自动调用 `doFriendSelect` 为其随机选择一个。
    *   `perk_OnCallForMedic`: 在准备阶段，玩家可以对着另一幸存者按 "呼叫医生" 来手动选择伙伴。
    *   `perk_OnPlayerDeath`: 当玩家和伙伴共同击杀或助攻时，`zf_perkTimer` 会增加 `ZF_FRIEND_CRITTIME_KILL` 或 `ZF_FRIEND_CRITTIME_ASSIST`。

---

### 6. 英雄 (Heroic)

*   **ID:** `ZF_PERK_HEROIC` (6)
*   **介绍:** 英雄(Heroic)——活到最后获得暴击
*   **详细描述:** 你有攻击力和防御力加成。你的每个击杀和助攻都会增加你的暴击时间。当你是最后一个幸存者时,你就会获得对应时间的暴击。“晚安,好运。” 推荐职业: 士兵、火焰兵、爆破手。
*   **参数 (Defines):**
    *   `HEROIC_COMBAT = 15;` // Combat bonus when using perk.
    *   `HEROIC_CRITTIME_INIT = 30;` // Starting crit time.
    *   `HEROIC_CRITTIME_KILL = 3;` // Crit time added per kill.
    *   `HEROIC_CRITTIME_KILL_ACTIVE = 0;` // Crit time added per kill when crittime is active.
    *   `HEROIC_CRITTIME_ASSIST = 1;` // Crit time added per assist.
    *   `HEROIC_CRITTIME_ASSIST_ACTIVE = 0;` // Crit time added per assist when crittime is active.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `HEROIC_COMBAT` (15%) 的攻击和防御加成，并初始化暴击时间 `zf_perkTimer[client]` 为 `HEROIC_CRITTIME_INIT`。
    *   **状态变量:**
        *   `zf_perkTimer[client]`: 存储累积的暴击秒数。
        *   `zf_perkState[client]`: 标记英雄模式是否已激活 (0=未激活, 1=已激活)。
    *   **条件逻辑 (`updateCondStats`):**
        *   检查当前是否只剩一个幸存者 (`validSurCount == 1`)。
        *   **激活:** 如果是，且 `zf_perkState` 为0，则激活英雄模式，将 `zf_perkState` 设为1，并给予 `zf_perkTimer` 秒的暴击 (`addCondKritz`) 和特效。
        *   **持续:** 如果已激活，`zf_perkTimer` 每秒递减，直到为0，暴击效果结束。
    *   **HUD文本 (`zf_perkStr`)** 显示当前的暴击时间。
*   **事件处理:**
    *   `perk_OnPlayerDeath`:
        *   当玩家击杀僵尸时，根据英雄模式是否激活，`zf_perkTimer` 增加 `HEROIC_CRITTIME_KILL` 或 `HEROIC_CRITTIME_KILL_ACTIVE`。
        *   助攻同理，增加 `HEROIC_CRITTIME_ASSIST` 或 `HEROIC_CRITTIME_ASSIST_ACTIVE`。

---

### 7. 牧师 (Holy)

*   **ID:** `ZF_PERK_HOLY` (7)
*   **介绍:** 牧师(Holy)——蹲着治疗幸存者
*   **详细描述:** 你的攻击力降低。当蹲下不动时,你可以治疗自己和周围的幸存者。“圣光赐予我胜利!” 推荐职业: 缺少医生的队伍。
*   **参数 (Defines):**
    *   `ZF_HOLY_ATTACK = -25;` // Attack penalty when using perk.
    *   `ZF_HOLY_RADIUSSQ = (400 * 400);` // Radius (squared) inwhich regen bonuses apply.
    *   `ZF_HOLY_REGEN = 10;` // Regen bonus for nearby survivors.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_HOLY_ATTACK` (-25%) 的攻击惩罚。
    *   **永久特效:** 在 `updateClientPermEffects` 中，为玩家创建一个光环特效 (`ZFPART_AURAGLOWBEAMS`)。
    *   **条件逻辑 (`updateCondStats`):**
        *   检查玩家是否在地面 (`isGrounded`)、蹲下 (`isCrouching`) 且没有移动 (`isNotMoving`)。
        *   如果满足条件，遍历所有存活的幸存者。
        *   对于在 `ZF_HOLY_RADIUSSQ` 范围内的幸存者（包括自己），通过 `addHealth` 给予 `ZF_HOLY_REGEN` 的生命回复。
        *   满足条件时显示光环 (`showAura`)，否则隐藏 (`hideAura`)。
*   **事件处理:**
    *   `perk_OnPlayerSpawn`: 应用永久属性和特效。

---

### 8. 主宰者 (Juggernaut)

*   **ID:** `ZF_PERK_JUGGERNAUT` (8)
*   **介绍:** 主宰者(Juggernaut)——攻击力高,速度慢
*   **详细描述:** 你的攻击力大幅提高,但你的防御力和移动速度降低。你免疫掉落伤害。如果你落在僵尸附近,僵尸会受到轻微的伤害,并产生击退和击晕效果。“(译者不会玩MOAB游戏)” 推荐职业: 士兵、爆破手、狙击手。
*   **参数 (Defines):**
    *   `ZF_JUGGERNAUT_ATTACK = 50;` // Attack bonus when using perk.
    *   `ZF_JUGGERNAUT_DEFEND = -50;` // Defense bonus when using perk.
    *   `Float:ZF_JUGGERNAUT_FORCE = 500.0;` // Knockback force.
    *   `ZF_JUGGERNAUT_RADIUS = 150;` // Radius inwhich fall damage causes stun.
    *   `ZF_JUGGERNAUT_SPEED = -100;` // Speed penalty when using perk.
    *   `Float:ZF_JUGGERNAUT_STUN_DURATION = 1.0;` // Duration of stun.
    *   `Float:ZF_JUGGERNAUT_STUN_SLOWDOWN = 1.0;` // Slowdown of stun.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_JUGGERNAUT_ATTACK` 攻击加成、`ZF_JUGGERNAUT_DEFEND` 防御惩罚和 `ZF_JUGGERNAUT_SPEED` 速度惩罚。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   **坠落伤害:** 如果是坠落伤害 (`attackWasSelfFall`)，则伤害无效 (`damage = 0.0`)，并在玩家周围 `ZF_JUGGERNAUT_RADIUS` 范围内造成范围伤害 (`applyDamageRadialAtClient`) 和特效 (`fxPuffBig`)。
        *   **坠落冲击:** 当僵尸受到上述范围伤害时，会触发 `perk_OnTakeDamage` 的另一分支，对僵尸造成击晕 (`TF2_StunPlayer`) 和击退 (`fxKnockback`)。
        *   **近战攻击:** 当玩家用近战攻击僵尸时，也会触发击退 (`fxKnockback`)。

---

### 9. 领袖 (Leader)

*   **ID:** `ZF_PERK_LEADER` (9)
*   **介绍:** 领袖(Leader)——放置增益旗帜
*   **详细描述:** 你有暴击加成,也会给附近的幸存者提供攻击力和防御力加成。发医生语音来放置一个旗帜,靠近你的旗帜的幸存者都会获得攻击力和防御力加成。旗帜持续90秒,冷却时间150秒。“我需要重新集结队伍。” 推荐职业: 士兵、爆破手、工程师。
*   **参数 (Defines):**
    *   `ZF_LEADER_SELF_CRIT = 15;` // Crit bonus when using perk.
    *   `ZF_LEADER_OTHERS_ATTACK = 15;` // Attack bonus for survivors near perk user.
    *   `ZF_LEADER_OTHERS_RADIUSSQ = (350 * 350);` // Radius (squared) inwhich passive bonuses are applied.
    *   `ZF_LEADER_RALLY_SELF_ATTACK = 5;` // Attack bonus for user per survivor near rally point.
    *   `ZF_LEADER_RALLY_SELF_DEFEND = 5;` // Defense bonus for user per survivor near rally point.
    *   `ZF_LEADER_RALLY_OTHERS_ATTACK = 15;` // Attack bonus for survivors near rally point.
    *   `ZF_LEADER_RALLY_OTHERS_DEFEND = 15;` // Defense bonus for user per survivor near rally point.
    *   `ZF_LEADER_RALLY_DURATION = 90;` // Duration after rally is placed before it expires.
    *   `ZF_LEADER_RALLY_COOLDOWN = 150;` // Duration after rally is placed before it can be replaced.
    *   `ZF_LEADER_RALLY_RADIUSSQ = (400 * 400);` // Radius (squared) inwhich rally bonuses apply.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_LEADER_SELF_CRIT` 暴击加成。
    *   **永久特效:** 在 `updateClientPermEffects` 中，创建被动光环 (`ZFPART_AURAINRED`)。
    *   **条件逻辑 (`updateCondStats`):**
        *   **被动光环:** 遍历所有幸存者，对 `ZF_LEADER_OTHERS_RADIUSSQ` 范围内的队友施加 `ZF_LEADER_OTHERS_ATTACK` 攻击加成。
        *   **旗帜光环:** 如果旗帜 (`zf_item[thisSur][0]`) 存在，遍历所有幸存者。
            *   对 `ZF_LEADER_RALLY_RADIUSSQ` 范围内的队友施加 `ZF_LEADER_RALLY_OTHERS_ATTACK` 和 `ZF_LEADER_RALLY_OTHERS_DEFEND` 加成。
            *   领袖自己则根据旗帜旁的队友数量，获得叠加的 `ZF_LEADER_RALLY_SELF_ATTACK` 和 `ZF_LEADER_RALLY_SELF_DEFEND` 加成。
        *   **计时器:** `zf_perkTimer[client]` 用于冷却。当计时器到 `ZF_LEADER_RALLY_DURATION` 时，旗帜消失 (`removeItem`)。当计时器到0时，技能准备就绪。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面和蹲下。
        *   满足条件后，设置冷却时间为 `ZF_LEADER_RALLY_COOLDOWN`，并调用 `doItemPlace` 放置旗帜模型 (`ZFMDL_FLAG`)。

---

### 10. 忍者 (Ninja)

*   **ID:** `ZF_PERK_NINJA` (10)
*   **介绍:** 忍者(Ninja)——放置撤退用诱饵
*   **详细描述:** 你的移动力大幅增加,但是你的攻击力降低。发医生语言来放置一个撤退点。当你受到攻击时, 你会被传送到撤退点。撤退点持续15秒,撤退点冷却时间30秒。“虽然没有飞镖,但是我会这个!” 推荐职业: 爆破骑士、工程师、医生。
*   **参数 (Defines):**
    *   `ZF_NINJA_ATTACK = -40;` // Attack penalty when using perk.
    *   `ZF_NINJA_SPEED = 50;` // Speed bonus using perk.
    *   `ZF_NINJA_DURATION_DECOY_ACTIVE = 15;` // Duration of decoy retreat point lifetime.
    *   `ZF_NINJA_DURATION_DECOY_DECAY = 5;` // Duration of decoy before it poofs.
    *   `ZF_NINJA_DURATION_COOLDOWN = 30;` // Duration after decoy before new decoy can be used.
    *   `ZF_NINJA_FALLDMG_RESIST = 50;` // Percentage of fall damage resistance.
    *   `Float:ZF_NINJA_FORCE = 600.0;` // Jump force.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_NINJA_ATTACK` 攻击惩罚和 `ZF_NINJA_SPEED` 速度加成。
    *   **状态变量:**
        *   `zf_perkState[client]`: 标记撤退点是否激活 (1=激活)。
        *   `zf_perkTimer[client]`: 管理撤退点激活和冷却时间。
    *   **核心功能 (`doNinjaDecoyPlace`, `doNinjaDecoyPoof`):**
        *   `doNinjaDecoyPlace`: 在玩家当前位置创建诱饵模型 (`doItemPlace`)，然后将玩家传送到 `zf_aura[client]` (撤退点) 的位置 (`TeleportEntity`)，最后移除撤退点光环。
        *   `doNinjaDecoyPoof`: 移除诱饵模型 (`removeItem`) 并播放特效。
    *   **条件逻辑 (`updateCondStats`):**
        *   `zf_perkTimer` 每秒递减。当计时器到 `ZF_NINJA_DURATION_COOLDOWN` 时，如果撤退点还未被触发，则移除光环，表示撤退点失效。
*   **事件处理:**
    *   `OnPlayerRunCmd`: 玩家按跳跃键时，如果满足条件，会触发二段跳 (`fxJump`)。
    *   `perk_OnTakeDamage`:
        *   **坠落伤害:** 减免 `ZF_NINJA_FALLDMG_RESIST` 的坠落伤害。
        *   **被近战攻击:** 如果撤退点已激活 (`zf_perkState[victim] == 1`)，则调用 `doNinjaDecoyPlace` 触发传送和诱饵，并创建一个定时器 `perk_tNinjaDecoyPoof` 在 `ZF_NINJA_DURATION_DECOY_DECAY` 秒后移除诱饵。本次伤害无效。
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面等条件。
        *   满足后，设置 `zf_perkState` 为1，设置 `zf_perkTimer` 为总时长，并调用 `createAura` 在当前位置创建一个光环作为撤退点标记。

---

### 11. 治安官 (Nonlethal)

*   **ID:** `ZF_PERK_NONLETHAL` (11)
*   **介绍:** 治安官(Nonlethal)——低威力子弹,击退僵尸
*   **详细描述:** 使用子弹类武器时,你的攻击力降低,但能造成击退效果。“离我远点!” 推荐职业: 工程师、狙击手。
*   **参数 (Defines):**
    *   `ZF_NONLETHAL_ATTACK_BULLET = -90;` // Attack penalty when using perk and bullet-based weapon.
    *   `Float:ZF_NONLETHAL_FORCE = 75.0;` // Base force applied per bullet-based attack.
*   **逻辑处理:**
    *   该职业没有永久属性或条件逻辑，完全由事件驱动。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   当僵尸被该玩家的子弹武器 (`attackWasBullet`) 攻击时，在伤害计算前，通过 `localAttAdjust` 施加 `ZF_NONLETHAL_ATTACK_BULLET` 的攻击惩罚。
    *   `perk_OnTakeDamagePost`:
        *   在伤害计算后，如果攻击是子弹武器，则对僵尸施加一个与伤害值成正比的击退效果 (`fxKnockback(victim, attacker, (ZF_NONLETHAL_FORCE * damage))`)。

---

### 12. 计划通 (Resourceful)

*   **ID:** `ZF_PERK_RESOURCEFUL` (12)
*   **介绍:** /BUG/计划通(Resourceful)——和补给品做朋友
*   **详细描述:** /该职业有漏洞,请勿游玩/ 你的每个击杀都会为你补充子弹、生命值和金属。弹药包会给你临时的攻击力加成,医疗包则是防御力加成。
*   **参数 (Defines):**
    *   `Float:ZF_RESOURCEFUL_AMMOPCT = 0.20;` // Percent of ammo received per kill.
    *   `ZF_RESOURCEFUL_ATTACK = 25;` // Attack bonus (temporary) when grabbing an ammopack.
    *   `ZF_RESOURCEFUL_DEFEND = 25;` // Defense bonus (temporary) when grabbing a medpack.
    *   `ZF_RESOURCEFUL_HEALTH = 25;` // Health gained per kill (up to max).
    *   `ZF_RESOURCEFUL_HEALTH_OVERHEAL = 15;` // Additional health gained per kill (overheal possible).
    *   `ZF_RESOURCEFUL_METAL = 25;` // Amount of metal received per kill.
    *   `ZF_RESOURCEFUL_PICKUP_DURATION = 10;` // Duration of temporary bonuses from pickups.
*   **逻辑处理:**
    *   该职业主要由事件驱动。
*   **事件处理:**
    *   `perk_OnPlayerDeath`: 当玩家击杀僵尸时，会获得 `ZF_RESOURCEFUL_HEALTH` 生命值、`ZF_RESOURCEFUL_HEALTH_OVERHEAL` 过量治疗、`ZF_RESOURCEFUL_AMMOPCT` 百分比的弹药 (`addResAmmoPct`) 和 `ZF_RESOURCEFUL_METAL` 的金属。
    *   `perk_OnAmmoPickup`: 拾取弹药包时，会补满弹药和金属，并获得 `ZF_RESOURCEFUL_ATTACK` 的临时攻击加成，持续 `ZF_RESOURCEFUL_PICKUP_DURATION` 秒 (`addStatTempStack`)。
    *   `perk_OnMedPickup`: 拾取医疗包时，会补满生命，并获得 `ZF_RESOURCEFUL_DEFEND` 的临时防御加成，持续 `ZF_RESOURCEFUL_PICKUP_DURATION` 秒 (`addStatTempStack`)。

---

---

### 13. 拾荒者 (Scavenger)

*   **ID:** `ZF_PERK_SCAVENGER` (9)
*   **介绍:** 拾荒者(Scavenger)——拾取弹药时有几率获得额外奖励。
*   **详细描述:** 当你拾取弹药包时，你有25%的几率获得5秒的暴击，或者50%的几率获得50金属。“一个人的垃圾是另一个人的宝藏。” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `SCAVENGER_CRIT_CHANCE = 25;` // Chance to gain crits on ammo pickup.
    *   `SCAVENGER_METAL_CHANCE = 50;` // Chance to gain metal on ammo pickup.
    *   `SCAVENGER_CRIT_DURATION = 5;` // Duration of crits.
    *   `SCAVENGER_METAL_AMOUNT = 50;` // Amount of metal gained.
*   **逻辑处理:**
    *   该职业完全由事件驱动。
*   **事件处理:**
    *   `perk_OnAmmoPickup`:
        *   当玩家拾取弹药包时触发。
        *   有 `SCAVENGER_CRIT_CHANCE` (25%) 的几率，通过 `addCondKritz` 获得 `SCAVENGER_CRIT_DURATION` (5) 秒的暴击，并播放特效。
        *   如果未触发暴击，则有 `SCAVENGER_METAL_CHANCE` (50%) 的几率获得 `SCAVENGER_METAL_AMOUNT` (50) 的金属。


### 13. 利他主义者 (Selfless)

*   **ID:** `ZF_PERK_SELFLESS` (13)
*   **介绍:** 利他主义者(Selfless)——拉僵尸垫背
*   **详细描述:** 你死后会爆炸,造成成吨的伤害。“幸存者们,这是我最后的波纹了,收下吧!” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_SELFLESS_DAMAGE = 10000;` // Explosion damage applied to zombies on death.
    *   `ZF_SELFLESS_RADIUS = 5000;` // Radius of explosion.
*   **逻辑处理:**
    *   该职业完全由事件驱动。
*   **事件处理:**
    *   `perk_OnPlayerDeath`: 当玩家被僵尸击杀时，会以玩家为中心，在 `ZF_SELFLESS_RADIUS` 范围内造成 `ZF_SELFLESS_DAMAGE` 的范围伤害 (`applyDamageRadialAtClient`)，并播放大爆炸特效 (`fxExplosionBig`)。

---

### 14. 仓鼠 (Stash)

*   **ID:** `ZF_PERK_STASH` (14)
*   **介绍:** /BUG/仓鼠(Stash)——放置藏身处
*   **详细描述:** /该职业有漏洞,请勿游玩/ 你可以通过蹲下来发医生语音来放置一个藏身处。藏身处需要远离其他幸存者或藏身处。藏身处会为你补充弹药、生命,并提供临时加成。藏身处持续时间30秒,冷却时间30秒。
*   **参数 (Defines):**
    *   `STASH_GRAB_ATTACK_DURATION = 45;`
    *   `STASH_GRAB_ATTACK_PERM = 10;`
    *   `STASH_GRAB_ATTACK_TEMP = 100;`
    *   `STASH_GRAB_HEALTH = 200;`
    *   `STASH_COOLDOWN = 30;`
    *   `STASH_WARMUP = 40;`
    *   `STASH_GRAB_RADSQ = (50 * 50);`
    *   `STASH_DROP_RADSQ_STASH = (200 * 200);`
    *   `STASH_DROP_RADSQ_CLIENT = (800 * 800);`
*   **逻辑处理:**
    *   **状态变量:** `zf_perkTimer[client]` 管理冷却和准备时间。
    *   **条件逻辑 (`updateCondStats`):**
        *   **拾取逻辑:** 如果藏身处 (`zf_item[thisSur][0]`) 存在且已准备好 (`zf_perkTimer <= STASH_COOLDOWN`)，玩家靠近 (`STASH_GRAB_RADSQ`) 即可拾取。
        *   拾取后，补满弹药金属，回复 `STASH_GRAB_HEALTH` 生命，获得 `STASH_GRAB_ATTACK_TEMP` 临时攻击加成和 `STASH_GRAB_ATTACK_PERM` 永久攻击加成。
        *   **计时器:** `zf_perkTimer` 每秒递减，在不同时间点提示玩家 "藏身处准备好了" 或 "可以放置新的藏身处了"。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面和蹲下。
        *   检查附近是否有其他幸存者 (`STASH_DROP_RADSQ_CLIENT`) 或藏身处 (`STASH_DROP_RADSQ_STASH`)。
        *   满足条件后，设置计时器为 `WARMUP + COOLDOWN`，并放置藏身处模型 (`ZFMDL_SUPPLYCRATE`)。

---

### 15. 狂热者 (Stir-Crazy)

*   **ID:** `ZF_PERK_STIRCRAZY` (15)
*   **介绍:** 狂热者(Stir-Crazy)——速度决定攻击力
*   **详细描述:** 你的移动速度会给你提供攻击力加成,速度越快,攻击力就越高。“这个职业曾经能获得200%的攻击力加成。” 推荐职业: 火焰兵、爆破骑士。
*   **参数 (Defines):**
    *   `ZF_STIRCRAZY_MAX_POINTS = 5;` // Number of previous positions used to calculate average position.
    *   `ZF_STIRCRAZY_DIST_MIN = 150;` // Minimum distance from average of last X positions overwhich bonuses apply.
    *   `ZF_STIRCRAZY_DIST_MAX = 750;` // Maximum distance from average of last X positions overwhich no further bonuses apply.
    *   `ZF_STIRCRAZY_ATTACK = 30;` // Attack bonus when ZF_STIRCRAZY_DIST_MAX is reached.
*   **逻辑处理:**
    *   **位置缓存:** 使用 `zf_perkPos[client][][]` 数组循环存储玩家过去 `ZF_STIRCRAZY_MAX_POINTS` 帧的位置。
    *   **条件逻辑 (`updateCondStats`):**
        *   每帧更新位置缓存。
        *   计算过去几帧的平均位置 (`avgPos`)。
        *   计算当前位置与平均位置的距离 (`dist`)。
        *   如果距离大于 `ZF_STIRCRAZY_DIST_MIN`，则根据距离与 `ZF_STIRCRAZY_DIST_MAX` 的比例，计算一个攻击力加成因子，并应用 `ZF_STIRCRAZY_ATTACK` 的加成。
*   **事件处理:**
    *   `perk_OnPlayerSpawn`: 初始化位置缓存数组，将所有缓存点设为玩家当前位置。

---

### 16. 供应商 (Supplier)

*   **ID:** `ZF_PERK_SUPPLIER` (16)
*   **介绍:** /BUG/供应商(Supplier)——放置弹药补给箱
*   **详细描述:** /该职业有漏洞,请勿游玩/ 你的弹药会周期性的补充,并且拥有双倍备弹量。发医生语音来放置一个补给箱,其他幸存者可以从中补给弹药。冷却时间10秒。
*   **参数 (Defines):**
    *   `ZF_SUPPLIER_MAX_ITEMS = 2;`
    *   `ZF_SUPPLIER_TIMER = 10;`
    *   `ZF_SUPPLIER_RADIUSSQ = (75 * 75);`
    *   `ZF_SUPPLIER_UPDATERATE = 10;`
    *   `ZF_SUPPLIER_SELF_DEFEND = 25;`
    *   `Float:ZF_SUPPLIER_AMMOPCT_RESLIMIT = 2.0;`
    *   `Float:ZF_SUPPLIER_AMMOPCT_SELF = 0.10;`
    *   `Float:ZF_SUPPLIER_AMMOPCT_OTHER = 0.25;`
    *   `ZF_SUPPLIER_RESUPPLY_COUNT = 4;`
    *   `ZF_SUPPLIER_ATTACK = 25;`
    *   `ZF_SUPPLIER_DURATION = 10;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_SUPPLIER_SELF_DEFEND` 防御加成。
    *   **条件逻辑 (`updateCondStats`):**
        *   **自我补给:** 每 `ZF_SUPPLIER_UPDATERATE` 秒，玩家会自动获得 `ZF_SUPPLIER_AMMOPCT_SELF` 的弹药和金属。
        *   **补给箱逻辑:**
            *   遍历玩家放置的补给箱 (`zf_item`)。
            *   `getItemMetadata` 用于存储补给箱剩余使用次数。
            *   其他幸存者靠近 (`ZF_SUPPLIER_RADIUSSQ`) 补给箱时，会获得 `ZF_SUPPLIER_AMMOPCT_OTHER` 的补给，并消耗次数。
            *   供应商本人会因此获得临时的 `ZF_SUPPLIER_ATTACK` 攻击加成。
            *   次数用完后，补给箱消失。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面和蹲下。
        *   满足条件后，放置补给箱模型 (`ZFMDL_SUPPLYCRATE`)，并用 `setItemMetadata` 设置其初始使用次数为 `ZF_SUPPLIER_RESUPPLY_COUNT`。

---

### 17. 暴脾气 (Tantrum)

*   **ID:** `ZF_PERK_TANTRUM` (17)
*   **介绍:** 暴脾气(Tantrum)——短时间内获得暴击
*   **详细描述:** 发医生语音来激活愤怒。愤怒给予你15秒的暴击,结束后进入30秒的疲惫状态。疲惫状态下,移动速度大幅降低。“我想这个职业的确很解压。” 推荐职业: 士兵、火焰兵、爆破手。
*   **参数 (Defines):**
    *   `ZF_TANTRUM_ACTIVE = 15;` // Duration of anger (100% crit) state.
    *   `ZF_TANTRUM_COOLDOWN = 30;` // Duration of tired (speed penalty) state.
    *   `ZF_TANTRUM_SPEED = -100;` // Speed penalty given after perk use.
*   **逻辑处理:**
    *   **状态机:** 使用 `zf_perkTimer[client]` 管理状态。
        *   `timer > COOLDOWN`: 愤怒激活状态。
        *   `0 < timer <= COOLDOWN`: 疲惫冷却状态。
        *   `timer == 0`: 准备就绪状态。
    *   **条件逻辑 (`updateCondStats`):**
        *   **愤怒状态:** 如果 `zf_perkTimer` 大于 `ZF_TANTRUM_COOLDOWN`，则给予玩家暴击 (`addCondKritz`)。
        *   **疲惫状态:** 如果 `zf_perkTimer` 在 0 和 `ZF_TANTRUM_COOLDOWN` 之间，则施加 `ZF_TANTRUM_SPEED` 的速度惩罚。
        *   在状态切换时（如 `timer == COOLDOWN`）向玩家显示提示信息。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   如果技能准备就绪 (`zf_perkTimer[client] == 0`)，则激活技能。
        *   设置 `zf_perkTimer` 为 `ACTIVE + COOLDOWN` 的总时长。
        *   立即给予 `ZF_TANTRUM_ACTIVE` 秒的暴击 (`addCondKritz`) 和特效 (`fxKritzStart`)。

---

### 18. 陷阱大师 (Trapper)

*   **ID:** `ZF_PERK_TRAPPER` (18)
*   **介绍:** 陷阱大师(Trapper)——放置地雷
*   **详细描述:** 发医生语音来放置地雷,僵尸碰到后会自动爆炸,同时最多拥有5个地雷。冷却时间20秒。“EDD Mounted, let them come.” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_TRAPPER_MAX_ITEMS = 5;` // Maximum number of mines that can be active at once.
    *   `ZF_TRAPPER_DAMAGE = 200;` // Damage done by trapper mines.
    *   `ZF_TRAPPER_RADIUS = 150;` // Radius inwhich mines trigger and cause damage.
    *   `ZF_TRAPPER_RADIUSSQ = (200 * 200);` // Radius (squared) inwhich mines trigger and cause damage.
    *   `ZF_TRAPPER_TIMER = 20;` // Time between use of perk.
*   **逻辑处理:**
    *   **条件逻辑 (`updateCondStats`):**
        *   遍历玩家放置的所有地雷 (`zf_item`)。
        *   遍历所有存活的僵尸，检查距离 (`GetVectorDistance`) 是否在 `ZF_TRAPPER_RADIUSSQ` 内。
        *   **触发:** 如果有僵尸进入范围，且该僵尸不是磁化僵尸，则地雷引爆。
        *   引爆时，点燃目标 (`TF2_IgnitePlayer`)，造成 `ZF_TRAPPER_DAMAGE` 的范围伤害 (`applyDamageRadial`)，播放爆炸特效 (`fxExplosionBig`)，并移除地雷。
        *   **失效:** 如果进入范围的僵尸是磁化僵尸 (`ZF_PERK_MAGNETIC`)，地雷会失效并播放火花特效 (`fxSpark`)。
        *   **待机:** 如果没有僵尸靠近，地雷会播放心跳声 (`ZFSND_TICK`)。
        *   `zf_perkTimer` 用于管理冷却。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面和蹲下。
        *   满足条件后，设置冷却时间，并调用 `doItemPlace` 放置地雷模型 (`ZFMDL_MINE`)。

---

### 19. 肉盾 (Turtle)

*   **ID:** `ZF_PERK_TURTLE` (19)
*   **介绍:** 肉盾(Turtle)——防御力高,速度慢
*   **详细描述:** 你的防御力大幅增加,但你的攻击力降低,而且无法造成随机暴击。Spy无法背刺你。“你们僵尸就这点能耐?” 推荐职业: 士兵、火焰兵、爆破骑士。
*   **参数 (Defines):**
    *   `ZF_TURTLE_ATTACK = -50;` // Attack penalty when using this perk.
    *   `ZF_TURTLE_DEFEND = 75;` // Defense bonus when using this perk.
    *   `ZF_TURTLE_SPEED = -100;` // Speed penalty when using perk.
    *   `Float:ZF_TURTLE_STUN_DURATION = 1.0;` // Stun time when zombie backstabs perk user.
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_TURTLE_ATTACK` 攻击惩罚、`ZF_TURTLE_DEFEND` 防御加成和 `ZF_TURTLE_SPEED` 速度惩罚。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   当玩家被间谍僵尸背刺时 (`attackWasBackstab`)，伤害无效 (`damage = 0.0`)。
        *   攻击者（间谍僵尸）会被击晕 `ZF_TURTLE_STUN_DURATION` 秒 (`TF2_StunPlayer`)。
        *   播放格挡特效 (`fxSpark`)。

---

### 20. 智者 (Wise)

*   **ID:** `ZF_PERK_WISE` (20)
*   **介绍:** 智者(Wise)——击杀提高属性
*   **详细描述:** 你的每个击杀和助攻都能使你的攻击力永久增加。每当你被近战武器攻击后,你的防御力就会永久增加。“从战斗中学习。” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_WISE_ATTACK_KILL = 1;` // Attack bonus when killing a zombie.
    *   `ZF_WISE_ATTACK_ASSIST = 0;` // Attack bonus when assisting in killing a zombie.
    *   `ZF_WISE_DEFEND = 1;` // Defense bonus when hit by a zombie.
    *   `ZF_WISE_DEFEND_LIMIT = 20;` // Defense bonus threshold above which no defense bonus is granted.
*   **逻辑处理:**
    *   该职业完全由事件驱动，通过修改永久属性 (`ZFStatTypePerm`) 实现成长。
*   **事件处理:**
    *   `perk_OnTakeDamage`: 当玩家被僵尸近战攻击时，如果其永久防御加成低于 `ZF_WISE_DEFEND_LIMIT`，则通过 `addStat` 增加 `ZF_WISE_DEFEND` 的永久防御。
    *   `perk_OnPlayerDeath`:
        *   当玩家击杀僵尸时，通过 `addStat` 增加 `ZF_WISE_ATTACK_KILL` 的永久攻击力。
        *   当玩家助攻时，增加 `ZF_WISE_ATTACK_ASSIST` 的永久攻击力。

---

### 21. 禅师 (Zenlike)

*   **ID:** `ZF_PERK_ZENLIKE` (21)
*   **介绍:** 禅师(Zenlike)——蹲着增加暴击率
*   **详细描述:** 当蹲下不动时,你的生命值会慢慢恢复,并不断增加你的暴击率。每攻击一次,暴击几率都会降低25%。(注意:Engi用扳手敲打建筑也会降低暴击率.)“出来吧,百分百暴击火箭!” 推荐职业: 士兵。
*   **参数 (Defines):**
    *   `ZF_ZENLIKE_CRIT_INC = 3;` // Crit bonus gained when crouched and not moving.
    *   `ZF_ZENLIKE_CRIT_DEC = 25;` // Crit bonus lost per shot.
    *   `ZF_ZENLIKE_HEAL = 1;` // Poison healed when crouched and not moving.
    *   `ZF_ZENLIKE_REGEN = 1;` // Health regen bonus when crouched and not moving.
*   **逻辑处理:**
    *   **状态变量:** `zf_perkState[client]` 存储当前累积的暴击率。
    *   **条件逻辑 (`updateCondStats`):**
        *   检查玩家是否在地面 (`isGrounded`)、蹲下 (`isCrouching`) 且没有移动 (`isNotMoving`)。
        *   如果满足条件，`zf_perkState` 每秒增加 `ZF_ZENLIKE_CRIT_INC`，上限为100。
        *   同时，玩家会回复 `ZF_ZENLIKE_REGEN` 的生命，并治疗 `ZF_ZENLIKE_HEAL` 的中毒效果。
        *   将 `zf_perkState` 的值作为条件暴击率 (`ZFStatTypeCond`) 应用于玩家。
*   **事件处理:**
    *   `perk_OnCalcIsAttackCritical`: 当玩家攻击时，会创建一个0.1秒的定时器 `perk_tZenlikeAttack`。
    *   `perk_tZenlikeAttack` (定时器回调): 玩家的 `zf_perkState` 减少 `ZF_ZENLIKE_CRIT_DEC`，下限为0。

---
## 僵尸职业 (Zombie Perks)

共有 `TOTAL_ZOM_PERKS` (18) 个僵尸职业。

---

### 1. 零号僵尸 (Alpha)

*   **ID:** `ZF_PERK_ALPHA` (1)
*   **介绍:** 零号僵尸(Alpha)——召唤僵尸随从
*   **详细描述:** 你能通过击杀幸存者或为随从助攻,使得死去的人类成为自己的随从。附近的每个僵尸和随从都能让你获得生命恢复和攻击加成。发医生语音可以召唤最多5个随从到身边,冷却时间15秒。“来自黑暗寒冬的仆人们、士兵们!听从我的召唤!” 推荐职业: 侦察兵、机枪手。
*   **参数 (Defines):**
    *   `ZF_ALPHA_RADIUSSQ = (500 * 500);`
    *   `ZF_ALPHA_ATTACK = 5;`
    *   `ZF_ALPHA_ATTACK_MINION = 10;`
    *   `ZF_ALPHA_REGEN = 4;`
    *   `ZF_ALPHA_REGEN_MINION = 12;`
    *   `ZF_ALPHA_SUMMON_LIMIT = 5;`
    *   `ZF_ALPHA_TIMER_MINION = 15;`
*   **逻辑处理:**
    *   **状态变量:** `zf_perkAlphaMaster[client]` 存储每个僵尸的“主人”ID。如果一个僵尸的 `zf_perkAlphaMaster` 指向一个零号僵尸，它就是随从。
    *   **核心功能 (`doAlphaSummon`):**
        *   遍历所有玩家，找出所有主人是自己的随从 (`zf_perkAlphaMaster[i] == client`)。
        *   随机选择最多 `ZF_ALPHA_SUMMON_LIMIT` 个随从，将他们传送到自己身边 (`TeleportEntity`)。
    *   **条件逻辑 (`updateCondStats`):**
        *   遍历所有僵尸，根据与自己的距离 (`ZF_ALPHA_RADIUSSQ`) 计算加成。
        *   每个普通僵尸提供 `ZF_ALPHA_ATTACK` 攻击和 `ZF_ALPHA_REGEN` 回复。
        *   每个随从提供 `ZF_ALPHA_ATTACK_MINION` 攻击和 `ZF_ALPHA_REGEN_MINION` 回复。
        *   `zf_perkTimer` 用于管理召唤技能的冷却。
*   **事件处理:**
    *   `perk_OnPlayerDeath`: 当零号僵尸击杀或助攻击杀幸存者时，该幸存者（复活为僵尸后）的 `zf_perkAlphaMaster` 会被设为该零号僵尸的ID。
    *   `perk_OnCallForMedic`: 检查冷却等条件后，调用 `doAlphaSummon` 召唤随从，并根据召唤数量设置冷却时间。

---

### 2. 自爆僵尸 (Combustible)

*   **ID:** `ZF_PERK_COMBUSTIBLE` (2)
*   **介绍:** 自爆僵尸(Combustible)——尸如其名
*   **详细描述:** 你的防御力大幅降低。被远程武器击杀后,你会爆炸并造成伤害。你不可以使用隐身手表或者原子能饮料。(远程武器包括一切非近战武器)“*将Boomer先推开再攻击*” 推荐职业: 侦察兵、机枪手。
*   **参数 (Defines):**
    *   `ZF_COMBUSTIBLE_DAMAGE = 120;`
    *   `ZF_COMBUSTIBLE_DAMAGE_HEAVY = 200;`
    *   `ZF_COMBUSTIBLE_DEFEND = -200;`
    *   `ZF_COMBUSTIBLE_RADIUS = 300;`
    *   `Float:ZF_COMBUSTIBLE_RESPAWNTIME = 4.5;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_COMBUSTIBLE_DEFEND` 的防御惩罚。
    *   **条件逻辑 (`updateCondStats`):** 播放微小的爆炸特效 (`fxExplosionTiny`) 来提示该职业。
*   **事件处理:**
    *   `OnPlayerRunCmd`: 禁用原子能饮料 (`ZFWEAP_BONK`) 和隐形手表。
    *   `perk_OnPlayerDeath`:
        *   如果被幸存者以非近战方式 (`!attackWasMelee`) 击杀，则会爆炸。
        *   根据职业（是否为机枪手）造成 `ZF_COMBUSTIBLE_DAMAGE` 或 `ZF_COMBUSTIBLE_DAMAGE_HEAVY` 的范围伤害。
        *   创建一个定时器 `perk_tSpawnClient`，在 `ZF_COMBUSTIBLE_RESPAWNTIME` 秒后强制重生。

---

### 3. 惊吓僵尸 (Horrifying)

*   **ID:** `ZF_PERK_HORRIFYING` (3)
*   **介绍:** 惊吓僵尸(Horrifying)——攻击削弱人类
*   **详细描述:** 你的攻击力降低,但你的攻击能降低幸存者的攻击力、防御力和攻击速度。减益效果持续15秒。你死亡后,这个效果也随即消失。“敲骨吸髓。” 推荐职业: 机枪手、间谍。
*   **参数 (Defines):**
    *   `HORRIFYING_ATTACK = -20;`
    *   `HORRIFYING_ATTACK_HEAVY = -30;`
    *   `HORRIFYING_DEFEND = 0;`
    *   `HORRIFYING_DEFEND_HEAVY = 0;`
    *   `HORRIFYING_ROF_HEAVY = -10;`
    *   `Float:HORRIFYING_PENALTYPCT_KILL = 0.75;`
    *   `Float:HORRIFYING_PENALTYPCT_ASSIST = 0.25;`
    *   `HORRIFYING_DURATION = 15;`
    *   `HORRIFYING_DURATION_HEAVY = 30;`
*   **逻辑处理:**
    *   **永久特效:** 在 `updateClientPermEffects` 中，创建蓝色外光环 (`ZFPART_AURAOUTBLU`)。
*   **事件处理:**
    *   `perk_OnTakeDamage`: 当该僵尸用近战攻击幸存者时，会通过 `addStatTempStack` 给幸存者施加一个临时的负面状态（降低攻击、防御、攻速），持续 `HORRIFYING_DURATION` 或 `HORRIFYING_DURATION_HEAVY` 秒。
    *   `perk_OnPlayerDeath`: 当该僵尸被幸存者击杀或助攻击杀时，幸存者身上的负面状态会按 `HORRIFYING_PENALTYPCT_KILL` 或 `HORRIFYING_PENALTYPCT_ASSIST` 的比例减轻 (`scaleStatTempPct`)。

---

### 4. 猎手僵尸 (Hunter)

*   **ID:** `ZF_PERK_HUNTER` (4)
*   **介绍:** 猎手僵尸(Hunter)——手动放置重生点
*   **详细描述:** 发医生语音来放置你的重生点。从自己的重生点重生时,你的重生时间较短,并获得临时的攻击加成。每次重生后,你只能放置一次重生点。注意!幸存者可以摧毁你的重生点。“你将成为我的猎物!” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_HUNTER_ATTACK = 50;`
    *   `ZF_HUNTER_DURATION = 10;`
    *   `ZF_HUNTER_RADIUSSQ = (85 * 85);`
    *   `Float:ZF_HUNTER_RESPAWNTIME = 5.5;`
*   **逻辑处理:**
    *   **状态变量:**
        *   `zf_perkState[client]`: 标记本轮是否已放置重生点 (1=已放置)。
        *   `zf_aura[client]`: 存储重生点光环实体的ID。
    *   **条件逻辑 (`updateCondStats`):**
        *   检查是否有幸存者靠近了重生点 (`ZF_HUNTER_RADIUSSQ`)。
        *   如果有，则移除光环 (`removeAura`)，重生点被破坏。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查是否已放置过重生点。
        *   如果没有，则设置 `zf_perkState` 为1，并调用 `createAura` 在当前位置创建一个光环作为重生点。
    *   `perk_OnPlayerSpawn`:
        *   重置 `zf_perkState` 为0。
        *   如果重生点 (`zf_aura`) 存在，则将玩家传送到重生点位置，并给予 `ZF_HUNTER_ATTACK` 的临时攻击加成，持续 `ZF_HUNTER_DURATION` 秒。
    *   `perk_OnPlayerDeath`:
        *   显示重生点光环 (`showAura`)。
        *   创建一个定时器，在 `ZF_HUNTER_RESPAWNTIME` 秒后强制重生。

---

### 5. 飞跃僵尸 (Leap)

*   **ID:** `ZF_PERK_LEAP` (5)
*   **介绍:** 飞跃僵尸(Leap)——大跳飞向空中
*   **详细描述:** 你的攻击力与防御力降低,但不受坠落伤害。发医生语音来施展大跳,冷却时间4秒。“起飞!” 推荐职业: 侦察兵、间谍。
*   **参数 (Defines):**
    *   `ZF_LEAP_COMBAT = -20;`
    *   `ZF_LEAP_COOLDOWN = 4;`
    *   `Float:ZF_LEAP_FORCE = 900.0;`
    *   `Float:ZF_LEAP_FORCE_SCOUT = 1500.0;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_LEAP_COMBAT` 的攻击和防御惩罚。
    *   **条件逻辑 (`updateCondStats`):** `zf_perkTimer` 用于管理飞跃技能的冷却。
*   **事件处理:**
    *   `perk_OnTakeDamage`: 免疫坠落伤害 (`attackWasSelfFall`)。
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面等条件。
        *   满足后，设置冷却时间，并调用 `fxJump` 让玩家跳跃。跳跃力度根据是否为侦察兵而不同。

---

### 6. 磁化僵尸 (Magnetic)

*   **ID:** `ZF_PERK_MAGNETIC` (6)
*   **介绍:** 磁化僵尸(Magnetic)——瘫痪附近建筑
*   **详细描述:** 你能使附近的步哨和地雷失效。“I will murder your toys as well.” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_MAGNETIC_RADIUSSQ = (500 * 500);`
*   **逻辑处理:**
    *   **条件逻辑 (`updateCondStats`):**
        *   遍历地图上所有的步哨枪 (`obj_sentrygun`)。
        *   检查是否有磁化僵尸在步哨的 `ZF_MAGNETIC_RADIUSSQ` 范围内。
        *   如果有，则通过 `SetEntProp` 设置步哨的 `m_bDisabled` 属性为1，使其失效，并播放火花特效。
        *   如果没有，则设为0，使其恢复正常。
*   **事件处理:**
    *   `perk_OnPlayerSpawn`: 为玩家添加 `TF_COND_NO_TARGET` 状态，使步哨不会主动攻击自己。
    *   `updateCondStats` (在陷阱大师的逻辑中): 磁化僵尸可以使陷阱大师的地雷失效。

---

### 7. 标记僵尸 (Marked)

*   **ID:** `ZF_PERK_MARKED` (7)
*   **介绍:** 标记僵尸(Marked)——瞄准特定目标
*   **详细描述:** 系统会随机选择一名幸存者作为你的目标。你对目标能造成极高伤害,但是对其他人造成较低伤害。当前目标死亡后,若剩余的幸存者超过1个,10秒后将自动选择一个新目标。“目标已经标记出来了!” 推荐职业: 侦察兵、机枪手。
*   **参数 (Defines):**
    *   `ZF_MARKED_ATTACK_ON_MARK = 200;`
    *   `ZF_MARKED_ATTACK_OFF_MARK = -10;`
    *   `ZF_MARKED_MIN_SURVIVORS = 1;`
    *   `ZF_MARKED_TIMER = 10;`
*   **逻辑处理:**
    *   **状态变量:**
        *   `zf_perkState[client]`: 存储被标记的幸存者ID。0表示正在选择，-1表示无法选择。
        *   `zf_perkTimer[client]`: 重新选择目标的冷却计时器。
    *   **核心功能 (`doMarkedSelect`):**
        *   在存活的幸存者中随机选择一个作为目标。
        *   设置 `zf_perkState`，并在目标头顶创建图标 (`createIcon`)。
    *   **条件逻辑 (`updateCondStats`):**
        *   如果目标死亡，则将 `zf_perkState` 设为0，并启动 `ZF_MARKED_TIMER` 秒的冷却。
        *   冷却结束后，调用 `doMarkedSelect` 选择新目标。
*   **事件处理:**
    *   `perk_OnGraceEnd`: 准备阶段结束后，调用 `doMarkedSelect` 选择第一个目标。
    *   `perk_OnTakeDamage`:
        *   当该僵尸攻击幸存者时，检查受害者是否为标记目标 (`zf_perkState[attacker] == victim`)。
        *   如果是，则获得 `ZF_MARKED_ATTACK_ON_MARK` 的巨额攻击加成。
        *   如果不是，则受到 `ZF_MARKED_ATTACK_OFF_MARK` 的攻击惩罚。

---

### 8. 狂怒僵尸 (Rage)

*   **ID:** `ZF_PERK_RAGE` (8)
*   **介绍:** 狂怒僵尸(Rage)——短时间加强属性
*   **详细描述:** 发医生语音来激活愤怒。愤怒使你获得150%的生命和速度加成。生命低于80%后,已有的愤怒会消失,也无法激活愤怒。冷却时间20秒。“Taaaaaaaaaank!” 推荐职业: 机枪手。
*   **参数 (Defines):**
    *   `ZF_RAGE_COOLDOWN = 20;`
    *   `ZF_RAGE_SPEED = 100;`
    *   `Float:ZF_RAGE_HEALTHPCT_TOUSE = 0.80;`
    *   `Float:ZF_RAGE_HEALTHPCT_ONUSE = 0.50;`
*   **逻辑处理:**
    *   **状态变量:**
        *   `zf_perkState[client]`: 标记愤怒状态是否激活 (1=激活)。
        *   `zf_perkTimer[client]`: 管理技能冷却。
    *   **条件逻辑 (`updateCondStats`):**
        *   如果愤怒已激活，检查生命值百分比是否低于 `ZF_RAGE_HEALTHPCT_TOUSE`。
        *   如果低于，则愤怒状态结束，移除光环。
        *   如果未激活，`zf_perkTimer` 每秒递减。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却和生命值百分比是否高于 `ZF_RAGE_HEALTHPCT_TOUSE`。
        *   满足后，激活愤怒，设置冷却，增加 `ZF_RAGE_HEALTHPCT_ONUSE` 的生命值，并获得 `ZF_RAGE_SPEED` 速度加成和特效。

---

### 9. 咆哮僵尸 (Roar)

*   **ID:** `ZF_PERK_ROAR` (9)
*   **介绍:** 咆哮僵尸(Roar)——咆哮击退幸存者
*   **详细描述:** 发医生语音来激活咆哮。咆哮造成击退效果并暂时降低幸存者防御力,冷却时间15秒。“哈!” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_ROAR_COOLDOWN = 15;`
    *   `ZF_ROAR_DURATION = 20;`
    *   `ZF_ROAR_DURATION_HEAVY = 60;`
    *   `Float:ZF_ROAR_FORCE = 1200.0;`
    *   `Float:ZF_ROAR_FORCE_HEAVY = 3000.0;`
    *   `ZF_ROAR_RADIUS = 450;`
*   **逻辑处理:**
    *   **条件逻辑 (`updateCondStats`):** `zf_perkTimer` 用于管理技能冷却。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却、是否在地面等条件。
        *   满足后，设置冷却，并以自己为中心造成一次范围伤害 (`applyDamageRadialAtClient`)。这个伤害本身很小，主要目的是为了触发 `perk_OnTakeDamage`。
    *   `perk_OnTakeDamage`:
        *   当幸存者受到上述范围伤害时 (`attackWasEnvExplosion`)，会根据距离和攻击者职业，受到不同力度的击退 (`fxKnockback`)。
        *   同时，幸存者会获得一个持续 `ZF_ROAR_DURATION` 或 `ZF_ROAR_DURATION_HEAVY` 秒的 `ZFCondIntimidated` 状态，该状态会降低其防御力。

---

### 10. 火焰僵尸 (Scorching)

*   **ID:** `ZF_PERK_SCORCHING` (10)
*   **介绍:** 火焰僵尸(Scorching)——烧死他们!
*   **详细描述:** 你的攻击力降低,但获得速度加成,并免疫火焰伤害。幸存者撞到你或被你近战击中时会着火。你不可以使用原子能饮料。“孙哥我火了!” 推荐职业: 侦察兵。
*   **参数 (Defines):**
    *   `ZF_SCORCHING_ATTACK = -50;`
    *   `ZF_SCORCHING_SPEED = 50;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_SCORCHING_SPEED` 速度加成。
    *   **条件逻辑 (`updateCondStats`):** 如果不在水中，则持续点燃自己 (`TF2_IgnitePlayer`)。
*   **事件处理:**
    *   `OnPlayerRunCmd`: 禁用原子能饮料。
    *   `perk_OnTakeDamage`:
        *   免疫火焰伤害 (`attackWasFire`)。
        *   近战攻击时，点燃目标，并受到 `ZF_SCORCHING_ATTACK` 的攻击惩罚。
    *   `perk_OnTouch`: 当接触到幸存者时，如果自己处于着火状态，则点燃对方。

---

### 11. 吐酸僵尸 (Sick)

*   **ID:** `ZF_PERK_SICK` (11)
*   **介绍:** 吐酸僵尸(Sick)——吐出有害的酸液
*   **详细描述:** 你的防御力大幅降低,但可以发医生语音来吐出酸液。酸液会持续35秒或直到你死亡。酸液造成的伤害与你和酸液之间的距离成正比。“*古怪的嚎叫声*” 推荐职业: 侦察兵。
*   **参数 (Defines):**
    *   `ZF_SICK_MAX_ITEMS = 5;`
    *   `ZF_SICK_DEFEND = -75;`
    *   `ZF_SICK_DAMAGE = 15;`
    *   `ZF_SICK_DAMAGE_RADIUS = 150;`
    *   `ZF_SICK_TIMER = 15;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_SICK_DEFEND` 防御惩罚。
    *   **条件逻辑 (`updateCondStats`):**
        *   `zf_perkTimer` 用于管理酸液持续时间。时间到后，移除所有酸液 (`removeItems`)。
        *   遍历所有酸液池 (`zf_item`)，对 `ZF_SICK_DAMAGE_RADIUS` 范围内的幸存者造成 `ZF_SICK_DAMAGE` 的伤害。
    *   **核心功能 (`perk_tSickSpit`):**
        *   这是一个定时器回调，用于连续吐出酸液。
        *   调用 `doItemThrow` 扔出一个酸液弹。
    *   **弹道逻辑 (`perk_OnGameFrame`):**
        *   检查酸液弹是否与地形碰撞 (`doItemCollide`)。
        *   碰撞后，移除弹体，并在碰撞点生成一个酸液池 (`doItemImpact`)。
*   **事件处理:**
    *   `perk_OnCallForMedic`:
        *   检查冷却等条件。
        *   满足后，设置计时器，并创建多个 `perk_tSickSpit` 定时器来连续吐出酸液弹。

---

### 12. 招魂僵尸 (Swarming)

*   **ID:** `ZF_PERK_SWARMING` (12)
*   **介绍:** 招魂僵尸(Swarming)——快速复活僵尸
*   **详细描述:** 你有移速加成,但你的攻击和防御力降低。你能使自己与身边的队友快速重生。“嘿!速生不是投票关掉了吗?” 推荐职业: 侦察兵。
*   **参数 (Defines):**
    *   `ZF_SWARMING_COMBAT = -20;`
    *   `ZF_SWARMING_RADIUSSQ = (400 * 400);`
    *   `ZF_SWARMING_SPEED = 50;`
    *   `Float:ZF_SWARMING_RESPAWNTIME = 0.5;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_SWARMING_COMBAT` 的攻防惩罚和 `ZF_SWARMING_SPEED` 的速度加成。
    *   **永久特效:** 在 `updateClientPermEffects` 中，创建苍蝇光环 (`ZFPART_AURAFLIES`)。
*   **事件处理:**
    *   `perk_OnPlayerDeath`:
        *   **自己死亡:** 创建一个定时器，在 `ZF_SWARMING_RESPAWNTIME` 秒后强制重生。
        *   **队友死亡:** 如果有队友在该僵尸的 `ZF_SWARMING_RADIUSSQ` 范围内死亡，也会为该队友创建一个快速重生定时器。

---

### 13. 吐油僵尸 (Tarred)

*   **ID:** `ZF_PERK_TARRED` (13)
*   **介绍:** 吐油僵尸(Tarred)——吐出减速的焦油
*   **详细描述:** 发医生语音来吐出焦油。焦油能降低幸存者的移动速度与攻击速度,持续30秒或直到你死亡。你的近战攻击附带焦油效果。“*古怪的嚎叫声*” 推荐职业: 侦察兵。
*   **参数 (Defines):**
    *   `ZF_TARRED_MAX_ITEMS = 5;`
    *   `ZF_TARRED_DURATION_MELEE = 10;`
    *   `ZF_TARRED_DURATION_SLICK = 30;`
    *   `ZF_TARRED_ROF = -20;`
    *   `ZF_TARRED_SPEED_MELEE = -40;`
    *   `ZF_TARRED_SPEED_SLICK = -30;`
    *   `ZF_TARRED_SPEED_LIMIT = -100;`
    *   `ZF_TARRED_TIMER = 30;`
    *   `ZF_TARRED_RADIUS = 75;`
*   **逻辑处理:**
    *   与吐酸僵尸类似，但效果是减速。
    *   **条件逻辑 (`updateCondStats`):**
        *   `zf_perkTimer` 管理焦油持续时间。
        *   焦油池对范围内的幸存者造成极低伤害，以触发 `perk_OnTakeDamage`。
        *   玩家模型会被染黑 (`fxSetClientColor`)。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   **近战攻击:** 当该僵尸近战攻击幸存者时，通过 `addStatTempStack` 施加 `ZF_TARRED_SPEED_MELEE` 的减速效果。
        *   **焦油池:** 当幸存者踩到焦油池时，施加 `ZF_TARRED_SPEED_SLICK` 的减速效果。
    *   `perk_OnCallForMedic`: 类似吐酸僵尸，吐出焦油弹。

---

### 14. 潜行僵尸 (Thieving)

*   **ID:** `ZF_PERK_THIEVING` (14)
*   **介绍:** 潜行僵尸(Thieving)——偷走人类的弹药和武器
*   **详细描述:** 你的攻击力降低,但你的近战攻击可以偷取幸存者的弹药、金属与uber。如果对方的主武器没有后备弹药,你就可以偷走并使用自己的有部分弹药的主武器。(请确保自己装备的主武器在白名单内,不然无法获得.)“这可比拳头好使多了!” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `Float:ZF_THIEVING_AMMOPCT = 0.30;`
    *   `ZF_THIEVING_ATTACK = -66;`
    *   `ZF_THIEVING_METAL = 100;`
    *   `Float:ZF_THIEVING_UBERPCT = 0.50;`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_THIEVING_ATTACK` 攻击惩罚。
    *   **核心功能 (`doThievingSteal`, `doThievingLimit`):**
        *   `doThievingSteal`: 移除受害者的武器 (`stripWeaponSlot`)，并为攻击者重新生成武器并给予少量弹药。
        *   `doThievingLimit`: 限制偷来的武器的弹药量，弹药用完后武器消失 (`stripWeaponSlot`)。
    *   **游戏循环 (`perk_OnGameFrame`):** 持续调用 `doThievingLimit` 检查并限制弹药。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   当该僵尸近战攻击幸存者时，根据幸存者当前手持的武器槽位，偷取不同资源：
            *   主武器槽：偷取 `ZF_THIEVING_AMMOPCT` 的备用弹药。如果备弹为0，则调用 `doThievingSteal` 偷走武器。
            *   副武器槽：偷取 `ZF_THIEVING_UBERPCT` 的Uber和备弹。
            *   近战槽：偷取 `ZF_THIEVING_METAL` 的金属。

---

### 15. 剧毒僵尸 (Toxic)

*   **ID:** `ZF_PERK_TOXIC` (15)
*   **介绍:** 剧毒僵尸(Toxic)——攻击附带剧毒
*   **详细描述:** 直接攻击时,你的攻击力大幅降低。但当你近战击中幸存者或被近战攻击时,目标将会中毒,受到剧毒伤害,持续12秒。当你保持不动时, 能对附近的幸存者持续造成伤害。“记住这个职业:Toxic.” 推荐职业: 侦察兵、间谍。
*   **参数 (Defines):**
    *   `ZF_TOXIC_ATTACK = -90;`
    *   `ZF_TOXIC_DURATION_POISON = 10;`
    *   `ZF_TOXIC_DAMAGE_PASSIVE = 5;`
    *   `ZF_TOXIC_RADIUSSQ = (400 * 400);`
*   **逻辑处理:**
    *   **永久属性:** 在 `updateClientPermStats` 中，应用 `ZF_TOXIC_ATTACK` 攻击惩罚。
    *   **条件逻辑 (`updateCondStats`):**
        *   如果僵尸不动 (`isNotMoving`) 且未隐身，则对 `ZF_TOXIC_RADIUSSQ` 范围内的幸存者造成 `ZF_TOXIC_DAMAGE_PASSIVE` 的毒性伤害 (`SDKHooks_TakeDamage`)。
        *   玩家模型会被染绿 (`fxSetClientColor`)。
*   **事件处理:**
    *   `perk_OnTakeDamage`:
        *   **攻击幸存者:** 当该僵尸近战攻击幸存者时，给予幸存者 `ZF_TOXIC_DURATION_POISON` 秒的 `ZFCondPoisoned` 状态。
        *   **被幸存者攻击:** 当该僵尸被幸存者近战攻击时，同样给予攻击者中毒状态。

---

### 16. 吸血僵尸 (Vampiric)

*   **ID:** `ZF_PERK_VAMPIRIC` (16)
*   **介绍:** 吸血僵尸(Vampiric)——攻击大量回血
*   **详细描述:** 你有生命值回复加成。你的攻击附带吸血效果。“不靠近你,怎么把你给揍扁呢.” 推荐职业: 侦察兵、机枪手。
*   **参数 (Defines):**
    *   `Float:ZF_VAMPIRIC_HEALTHPCT = 1.00;` // Percent of damage leeched when hit.
    *   `ZF_VAMPIRIC_REGEN = 15;` // Regeneration bonus when using perk.
*   **逻辑处理:**
    *   **条件逻辑 (`updateCondStats`):** 每秒回复 `ZF_VAMPIRIC_REGEN` 的生命值。
*   **事件处理:**
    *   `perk_OnTakeDamagePost`: 当该僵尸用近战攻击对幸存者造成伤害后，会回复 `伤害值 * ZF_VAMPIRIC_HEALTHPCT` 的生命值。

---

### 17. 复仇僵尸 (Vindictive)

*   **ID:** `ZF_PERK_VINDICTIVE` (17)
*   **介绍:** 复仇僵尸(Vindictive)——击杀提高属性
*   **详细描述:** 你的击杀与助攻能使你获得永久性的攻击力和防御力加成。“我想这是死去的智者.” 推荐职业: 任何。
*   **参数 (Defines):**
    *   `ZF_VINDICTIVE_ATTACK = 20;`
    *   `ZF_VINDICTIVE_ATTACK_ASSIST = 10;`
    *   `ZF_VINDICTIVE_DEFEND = 10;`
    *   `ZF_VINDICTIVE_DEFEND_ASSIST = 5;`
*   **逻辑处理:**
    *   与幸存者的“智者”职业类似，完全由事件驱动，修改永久属性。
*   **事件处理:**
    *   `perk_OnPlayerDeath`:
        *   当该僵尸击杀幸存者时，通过 `addStat` 增加 `ZF_VINDICTIVE_ATTACK` 的永久攻击和 `ZF_VINDICTIVE_DEFEND` 的永久防御。
        *   助攻同理，增加 `ZF_VINDICTIVE_ATTACK_ASSIST` 和 `ZF_VINDICTIVE_DEFEND_ASSIST`。