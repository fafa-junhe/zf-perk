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
        -   施加 `ZF_JUGGERNAUT_FORCE` (500.0) 的击退。