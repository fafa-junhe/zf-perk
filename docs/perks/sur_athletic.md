# 幸存者职业: 运动员 (Athletic)

## 描述

**简短描述:**
> 运动员(Athletic)——高移动力

**详细描述:**
> 你的移动力和攻击速度大幅增加. 
> 但是你的攻击力降低,而且无法造成随机暴击. 
> 
> “嘿!你们根本追不上我!” 
> 推荐职业: 火焰兵、爆破骑士、医生

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_ATHLETIC` | 1 | Perk的ID |
| `ZF_ATHLETIC_ATTACK` | -40 | 攻击力惩罚 (-40%) |
| `ZF_ATHLETIC_CRIT` | -100 | 暴击率惩罚 (-100%) |
| `ZF_ATHLETIC_ROF` | 100 | 攻击速度加成 (+100%) |
| `ZF_ATHLETIC_SPEED` | 100 | 移动速度加成 (+100%) |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了运动员职业 (`usingSurPerk(client, ZF_PERK_ATHLETIC)`), 则会应用以下永久性的属性修改：
        *   攻击力 (`ZFStatAtt`) 减少 `ZF_ATHLETIC_ATTACK` (-40)。
        *   暴击率 (`ZFStatCrit`) 减少 `ZF_ATHLETIC_CRIT` (-100)，即无法暴击。
        *   攻击速度 (`ZFStatRof`) 增加 `ZF_ATHLETIC_ROF` (100)。
        *   移动速度 (`ZFStatSpeed`) 增加 `ZF_ATHLETIC_SPEED` (100)。

## 事件处理

该职业没有特定的事件处理逻辑，其功能完全通过永久性的属性修改实现。
