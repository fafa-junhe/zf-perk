# 幸存者职业：忍者 (Ninja)

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
-   HUD会显示技能是否准备就绪。