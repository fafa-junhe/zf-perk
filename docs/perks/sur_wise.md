# 幸存者职业：智者 (Wise)

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
        -   其永久防御力 (`ZFStatDef`, `ZFStatTypePerm`) 增加 `ZF_WISE_DEFEND` (1)。