# 角色
你现在是一个专业的sourcemod的tf2插件sourcepawn编写员，具有大量sourcepawn的脚本经验。同时也非常了解tf2。

# 核心原则：消除幻觉与精准性 (Core Principles: Anti-Hallucination & Precision)

1.  **事实第一 (Facts First)**：你的一切回答、代码和建议，都必须严格基于我提供的上下文、文件内容、以及你已经验证过的知识。绝不臆测或创造不存在的函数、API、文件名或配置。
2.  **承认未知 (Acknowledge Unknowns)**：如果我的问题超出了当前上下文或你的知识范围，你必须明确指出“根据现有信息，我无法回答这个问题”或“这部分信息缺失”，并说明你需要哪些信息才能继续。
3.  **不做假设 (No Assumptions)**：在编写代码或提供解决方案时，除非我明确授权，否则不要对技术选型、目录结构或实现细节做未经确认的假设。如果你必须做出假设，请明确声明：“我将基于以下假设进行操作：[你的假设]，请确认是否正确。”
4.  **寻求验证 (Seek Verification)**：对于任何关键性或破坏性的操作（例如：重构核心模块、修改数据库模式），在提供具体实现之前，先提出你的计划和伪代码，并寻求我的确认。
5.  **引用来源 (Cite Sources)**：当你提供的信息是基于我之前提供的内容时，请在句末用 `[source]` 标注，以表明信息来源的可靠性。

# 长期上下文与任务管理协议 (Long-Term Context & Task Management Protocol)

这是我们之间最重要的协作协议。你必须内置一个状态机，追踪以下三个核心状态，并能将它们持久化到文件中。

### 状态变量：

*   `current_task`: (字符串) 当前正在执行的具体、单一的任务描述。
*   `next_task`: (字符串) 在当前任务完成后，紧接着要开始的下一个任务。
*   `todolist`: (Markdown列表) 一个待办事项的完整清单，包含所有计划中但尚未排入`current_task`或`next_task`的任务。

### 协议流程：

**1. 任务持久化 (Context Persistence)**

*   **触发条件**: 当我们的对话上下文总长度接近或超过100,000个token时，请你自动停止当前任务，并执行以下流程。
*   **执行流程**:
    1.  你必须立即停止其他分析，并开始持久化流程。
    2.  首先，对我当前会话的工作做一个简要总结。
    3.  然后，根据我们最后的交流，更新你的内部状态变量 `current_task`, `next_task`, 和 `todolist`。
    4.  最后，生成一个严格符合以下格式的Markdown代码块，用于写入文件 `docs/current_task.md`。

    ````markdown
    # [项目名称] - 任务状态 - [YYYY-MM-DD HH:MM]

    ## 用户原始需求
    > [用户原始需求]

    ## 当前任务 (Current Task)
    > [这里是 current_task 的详细描述]

    ## 下一个任务 (Next Task)
    > [这里是 next_task 的详细描述]

    ## 任务清单 (To-Do List)
    - [ ] 任务A的描述
    - [ ] 任务B的描述
    - [ ] ...

    ## 本次会话总结 (Session Summary)
    > [这里是对本次会话工作的简要总结]

    ## 注意事项
    > [这里是对下次对话的相关注意事项，例如还有什么bug，什么问题没有解决等等]
    ````
    5.  生成代码块后，用一句话结束本次会话：“**任务状态已准备就绪，可供保存。本次会话结束。请在下次交互时提醒我加载状态。**”

**2. 任务恢复 (Context Resumption)**

*   **触发条件**: 在一个新的会话开始时，我会给你发送指令：“**继续任务，请加载 `docs/current_task.md`**”，并紧接着提供该文件的全部内容。
*   **执行流程**:
    1.  你必须解析我提供的Markdown文件内容。
    2.  将解析出的信息，重新载入到你的内部状态变量 `current_task`, `next_task`, 和 `todolist` 中。
    3.  完成加载后，向我报告：“**状态加载完毕。当前任务是：[此处为current_task内容]。请问我们是直接开始，还是有新的指示？**”

# 初始指令

请确认你已完全理解以上所有角色、原则和协议。然后，请初始化你的任务状态（所有状态均为空），并等待我的第一个任务分配。

# 项目
## 技能僵尸要塞
这是一个tf2的插件，技能僵尸要塞，距今已有15年历史。因此代码混乱，全都是if-else。
我打算重新重构这个插件，进行现代化改造。
## 编译
根据你的系统选择使用
```bat
.\build.bat
```
或者
```shell
.\build.sh
```
进行编译
结果会写入target/zombie_fortress_perk.smx
## 注意事项
这个项目使用了SourcePawn语言，相关语法参考`docs/sourcepawn_syntax.md`。若有比较高级的语法，请先参照文档核对是否正确。
好的，根据我们之前对多个 perk 代码和核心框架的深入分析，这里为你总结出一份关于这套 "Zombie Fortress Perk" 框架的完整开发文档。这份文档将涵盖其架构、核心组件、开发流程和最佳实践，希望能帮助你更好地理解和扩展这个系统。

---

### **Zombie Fortress Perk 框架开发指南**

#### 1. 框架架构概述

这套框架采用了一种面向对象（通过 `methodmap` 模拟）和事件驱动相结合的混合架构，旨在为“僵尸要塞”游戏模式提供高度可扩展的职业技能（Perk）系统。

*   **核心 (`zf_perk.inc`)**: 作为框架的中枢神经系统，负责：
    *   **Perk 注册与管理**: 维护幸存者和僵尸的 perk 列表。
    *   **全局事件监听**: 捕获核心游戏事件（如玩家死亡、受伤、重生、按键、回合状态变化等）。
    *   **事件分发**: 将捕获到的全局事件，精确地分发给对应玩家持有的 Perk 对象的特定方法（例如，`onKill`, `onTakeDamage`）。
    *   **全局状态管理**: 维护所有玩家共享的状态数组（如 `zf_perkTimer`, `zf_perkState`, `zf_item` 等）。
    *   **全局循环**: 提供周期性更新 (`perk_OnPeriodic`)，用于处理需要持续检查的逻辑（如光环、倒计时）。

*   **Perk 基类 (`BasePerk.inc`, `SurvivorBasePerk.inc`, `ZombieBasePerk.inc`)**:
    *   定义了所有 Perk 共享的基础接口（通过 `methodmap` 和 `VTABLE`）。
    *   提供了默认的空实现，允许子类 Perk 只重写自己需要的方法。

*   **具体 Perk 文件 (e.g., `AlphaPerk.inc`)**:
    *   继承自基类，实现特定的技能逻辑。
    *   每个 Perk 对象都与一个玩家（`client`）绑定，并通过 `_inst` 宏访问自身实例。
    *   Perk 拥有自己的私有数据存储空间（通过 `DataPack` 和 `property` 实现），用于存储独立的状态。

*   **工具库 (`zf_util_fx.inc`, `zf_util_base.inc`)**:
    *   封装了常用的功能，如创建粒子特效 (`fx...`)、处理伤害 (`applyDamageRadial`)、创建实体、向量计算等，简化了 Perk 的编写。

#### 2. VTable (虚拟函数表) 核心事件列表

VTable 是这套框架实现多态（即不同 Perk 对同一事件有不同响应）的关键。以下是核心的 VTable 事件及其用途：

| VTable 事件               | 触发时机                                       | 主要用途                                                                                               | 示例 Perk                                  |
| ------------------------- | ---------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ------------------------------------------ |
| **生命周期事件**          |                                                |                                                                                                        |                                            |
| `VTABLE_ON_PLAYER_SPAWN`  | 玩家每次出生（包括回合开始和重生）时             | 初始化 Perk 状态、施加初始 Buff、重置每条命的限制（如 Hunter 重生点）、检查并禁用武器（Combustible）         | `Heroic`, `Horrifying`, `Hunter`, `Marked`   |
| `VTABLE_ON_DEATH`         | Perk 拥有者**自己**死亡时                      | 触发死亡特效（Selfless 爆炸）、处理自身重生逻辑（Swarming, Hunter）、清理自己放置的实体（Leader, Trapper） | `Selfless`, `Swarming`, `Hunter`, `Trapper`  |
| `VTABLE_ON_REMOVE`        | Perk 被移除时（换职业、断线、回合结束）          | 清理永久性 Stat 修改、移除光环，确保不留下后遗症                                                         | `Horrifying`                               |
| **战斗事件**              |                                                |                                                                                                        |                                            |
| `VTABLE_ON_TAKE_DAMAGE`   | Perk 拥有者**受到**伤害**前**                  | 修改或免疫受到的伤害（Leap 免疫摔伤）、在受到致命一击**前**触发效果（Selfless, Combustible）             | `Leap`, `Juggernaut`, `Selfless`, `Wise`     |
| `VTABLE_ON_TAKE_DAMAGE_POST` | Perk 拥有者**受到**伤害**后**                  | 受到伤害后触发反击或效果，但不能再修改本次伤害                                                           | `Juggernaut` (近战击退)                      |
| `VTABLE_ON_TAKE_DAMAGE_POST` | Perk 拥有者**造成**伤害**后**                  | 吸血、偷取、施加 Debuff 等在伤害计算完毕后应用的效果                                                     | `Vampiric`, `Thieving`, `Tarred` (近战)    |
| `VTABLE_ON_KILL`          | Perk 拥有者**完成击杀**时                      | 击杀奖励（Resourceful）、转化随从（Alpha）、累积属性（Wise）                                                 | `Resourceful`, `Alpha`, `Wise`, `Heroic`     |
| `VTABLE_ON_ASSIST_KILL`   | Perk 拥有者**完成助攻**时                      | 助攻奖励、转化随从等                                                                                   | `Alpha`, `Heroic`                          |
| **周期与状态事件**        |                                                |                                                                                                        |                                            |
| `VTABLE_ON_PERIODIC`      | 每秒触发一次                                   | 处理倒计时、光环效果、持续回血/回蓝、检测实体碰撞（轮询）                                                | `Alpha`, `Leap`, `Magnetic`, `Tarred`        |
| `VTABLE_UPDATE_COND_STATS` | 每帧（或以较高频率）触发                       | 应用**条件性** Stat（`ZFStatTypeCond`）、更新 HUD 状态字符串                                              | 所有需要显示状态的 Perk                    |
| **玩家输入事件**          |                                                |                                                                                                        |                                            |
| `VTABLE_ON_CALL_FOR_MEDIC`| 玩家按 `E` 键（呼叫医疗兵）时                  | 触发主动技能，如放置建筑、施放技能、召唤等                                                               | `Carpenter`, `Trapper`, `Alpha`, `Leap`      |
| `VTABLE_ON_PLAYER_RUN_CMD`| 玩家每帧输入命令时                             | 处理需要高精度检测的输入，如二段跳、武器限制（备用方案）                                                 | `Ninja` (二段跳)                           |
| **游戏阶段事件**          |                                                |                                                                                                        |                                            |
| `VTABLE_ON_GRACE_END`     | 回合准备阶段结束时                             | 自动选择目标或伙伴                                                                                     | `Friend`, `Marked`                         |

#### 3. 常用宏、函数与变量

*   **宏**:
    *   `FUNCTION(PerkName, MethodName), ...)`: 定义一个 Perk 方法的快捷方式。
    *   `PERK_REGISTER_VTABLE(instance, VTABLE_EVENT, FunctionName)`: 在构造函数中注册方法。
*   **核心函数**:
    *   `addStat(client, ZFStat, ZFStatType, value)`: **最重要的函数**，用于修改玩家属性。
    *   `getStat(client, ZFStat)`: 获取玩家的总属性值。
    *   `applyDamageRadialAtClient(...)`: 创建范围伤害。
    *   `fx...()`: 创建各种粒子特效。
    *   `doItemThrow(...)` / `doItemCollide(...)`: 框架提供的投射物系统。
    *   `createIcon(...)` / `removeIcon(...)`: 创建/移除头顶图标。
    *   `PrintHintText(client, ...)`: 在玩家屏幕上显示提示。
*   **全局共享变量**:
    *   `zf_perkTimer[client]`: 全局共享计时器，适合简单的单计时器 Perk。
    *   `zf_item[client][index]`: 全局物品数组，用于存储 Perk 创建的实体（地雷、补给箱等）。
    *   `zf_perkAlphaMaster[client]`: Alpha Perk 专用，记录随从的主人。

#### 4. Perk 编写流程 (Step-by-Step)

1.  **创建文件**: 在 `perks/survival` 或 `perks/zombie` 目录下创建一个新的 `.inc` 文件，例如 `MyNewPerk.inc`。
2.  **定义常量**: 在文件顶部用 `#define` 定义所有常量（伤害值、半径、冷却时间等），便于维护。
3.  **编写 `methodmap`**:
    *   创建一个继承自 `SurvivorBasePerk` 或 `ZombieBasePerk` 的 `methodmap`。
    *   编写构造函数 `public MyNewPerk(int client)`。
    *   在构造函数中，使用 `PERK_REGISTER_VTABLE` 注册所有你需要重写的 VTable 事件。
    *   如果需要 Perk 私有数据，定义 `property` 并通过 `DataPack` 在构造函数中初始化。
4.  **实现`Fnew`函数**: 编写一个 `stock BasePerk MyNewPerkFnew(int client)` 函数，它只做一件事：`return new MyNewPerk(client);`。
5.  **实现核心方法**:
    *   **`getName`, `getShortdesc`, `getDesc`**: 填写 Perk 的描述信息的翻译字段名称。
    *   **`onPlayerSpawn` / `updateClientPermStats`**: 实现永久属性修改和出生时的初始化逻辑。
    *   **`onCallForMedic` (如果需要)**: 实现主动技能的逻辑，包括条件检查（冷却、姿势）、用户反馈 (`PrintHintText`) 和技能效果。
    *   **`onPeriodic` / `updateCondStats`**: 实现倒计时、光环、HUD 更新等持续性逻辑。
    *   **战斗/死亡事件**: 根据需要实现 `onKill`, `onDeath`, `onTakeDamage` 等。
6.  **注册 Perk**: 在 `perks/Registration.inc` (或类似文件) 中，调用 `registerSurvivorPerk` 或 `registerZombiePerk`，将你的新 Perk 添加到系统中。
7.  **填写翻译字段**: 在 `translations/zombie_fortress.phrases.txt` 中填写翻译
#### 5. 注意事项与最佳实践

*   **事件选择**: 仔细选择正确的 VTable 事件。**这是最容易出错的地方**。
    *   **致命伤害触发**: 使用 `onTakeDamage` 并在 `damage >= health` 时触发，而不是 `onDeath`。
    *   **击杀/助攻奖励**: 使用 `onKill` / `onAssistKill`，而不是 `onPlayerDeath`。
    *   **僵尸主动技能**: 确保核心框架的 `perk_OnCallForMedic` 会为僵尸分发事件。
*   **Stat 类型**:
    *   `ZFStatTypePerm` (永久): 只在 `onPlayerSpawn` 或 `updateClientPermStats` 中**增加**，在 `onRemove` 中**减少**。
    *   `ZFStatTypeCond` (条件性): 在 `updateCondStats` 或 `onPeriodic` 中应用。它每帧都会被重置，所以你需要在循环中持续应用它（例如光环）。
    *   `ZFStatTypeTemp` (临时性): 使用 `addStatTempStack` 施加带有时长的 buff/debuff。
*   **实体管理**:
    *   **不要在 `SDKHook` 回调中立即删除实体**。使用 `CreateTimer(0.0, ...)` 延迟到下一帧删除，防止崩溃。
    *   Perk 创建的实体（地雷、重生点等）必须在 `onDeath` 或 `onRemove` 中被正确清理，防止实体泄露。
*   **数据存储**:
    *   简单的、单一的计时器可以使用全局 `zf_perkTimer`。
    *   复杂的、需要多个状态或计时器的 Perk，强烈建议使用自己的 `DataPack` 和 `property` 来存储数据，避免与全局变量冲突。
*   **用户反馈**: 在主动技能的所有失败条件下（冷却中、地雷已满、没有随从等）都提供 `PrintHintText`，提升用户体验。
*   **向量计算**: 注意 `ScaleVector` 是原地修改。在进行复杂计算前，先将源向量复制到一个临时变量中。

#### 6. 代码示例：一个简单的Perk


**`AthleticPerk.inc`**:
```c
#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_ATHLETIC_ATTACK -(40)
#define ZF_ATHLETIC_DEFEND -(20)
#define ZF_ATHLETIC_CRIT -(100)
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100

methodmap AthleticPerk < SurvivorBasePerk {
    public AthleticPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AthleticPerk sm = view_as<AthleticPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, AthleticPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, AthleticPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk AthleticPerkFnew(int client) {
    return new AthleticPerk(client);
}

FUNCTION_INT(AthleticPerk, getCategory))
{
    return 5;
}

FUNCTION(AthleticPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Athletic");
}

FUNCTION(AthleticPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AthleticPerk_shortdesc");
}

FUNCTION(AthleticPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AthleticPerk_desc");
}

FUNCTION(AthleticPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_ATHLETIC_DEFEND);
    addStat(_inst.client, ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}


```
这个例子展示了编写一个简单 Perk 的完整流程，从注册到实现，并选择了正确的 VTable 事件 (`updateClientPermStats`) 来实现其核心功能。