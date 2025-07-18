# 幸存者职业：肉盾 (Turtle)

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
    -   触发火花视觉效果 (`fxSpark`)。