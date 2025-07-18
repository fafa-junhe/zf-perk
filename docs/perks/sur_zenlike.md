# 幸存者职业：禅师 (Zenlike)

## 介绍

**名称**: 禅师 (Zenlike)

**简短描述**:
> 禅师(Zenlike)——蹲着增加暴击率

**详细描述**:
> 当蹲下不动时,你的生命值会慢慢恢复,并不断增加你的暴击率.
> 每攻击一次,暴击几率都会降低25%.
> (注意:Engi用扳手敲打建筑也会降低暴击率.)
>
> “出来吧,百分百暴击火箭!”
> 推荐职业: 士兵

## 参数

| 参数名                | 默认值 | 备注                                     |
| --------------------- | ------ | ---------------------------------------- |
| `ZF_ZENLIKE_CRIT_INC` | 3      | 蹲下不动时每秒增加的暴击率               |
| `ZF_ZENLIKE_CRIT_DEC` | 25     | 每次攻击后降低的暴击率                   |
| `ZF_ZENLIKE_HEAL`     | 1      | 蹲下不动时每秒恢复的中毒效果量           |
| `ZF_ZENLIKE_REGEN`    | 1      | 蹲下不动时每秒恢复的生命值               |

## 逻辑处理

### `updateCondStats`

-   当拥有此职业的幸存者在地面上、蹲下且没有移动时：
    -   暴击率 (`zf_perkState`) 每秒增加 `ZF_ZENLIKE_CRIT_INC` (3)，上限为100。
    -   每秒回复 `ZF_ZENLIKE_REGEN` (1) 点生命值。
    -   每秒移除 `ZF_ZENLIKE_HEAL` (1) 点中毒效果。
-   增加的暴击率会作为条件性暴击加成 (`ZFStatCrit`, `ZFStatTypeCond`) 应用于玩家。

### `perk_OnCalcIsAttackCritical`

-   当玩家进行一次暴击计算的攻击时（通常是每次开火），会触发一个0.1秒后的计时器 `perk_tZenlikeAttack`。

### `perk_tZenlikeAttack` (Timer Callback)

-   该计时器执行时，会从玩家当前的暴击率 (`zf_perkState`) 中减去 `ZF_ZENLIKE_CRIT_DEC` (25)，下限为0。