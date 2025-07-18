# 幸存者职业：计划通 (Resourceful)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 计划通 (Resourceful)

**简短描述**:
> /BUG/计划通(Resourceful)——和补给品做朋友

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你的每个击杀都会为你补充子弹、生命值和金属.
> 弹药包会给你临时的攻击力加成,医疗包则是防御力加成.

## 参数

| 参数名                          | 默认值 | 备注                                         |
| ------------------------------- | ------ | -------------------------------------------- |
| `ZF_RESOURCEFUL_AMMOPCT`        | 0.20   | 每次击杀获得的弹药百分比                     |
| `ZF_RESOURCEFUL_ATTACK`         | 25     | 拾取弹药包时的临时攻击力加成                 |
| `ZF_RESOURCEFUL_DEFEND`         | 25     | 拾取医疗包时的临时防御力加成                 |
| `ZF_RESOURCEFUL_HEALTH`         | 25     | 每次击杀获得的生命值（最高至上限）           |
| `ZF_RESOURCEFUL_HEALTH_OVERHEAL`| 15     | 每次击杀获得的额外生命值（可过量治疗）       |
| `ZF_RESOURCEFUL_METAL`          | 25     | 每次击杀获得的金属量                         |
| `ZF_RESOURCEFUL_PICKUP_DURATION`| 10     | 拾取物品后临时加成的持续时间（秒）           |

## 逻辑处理

### `perk_OnPlayerDeath`

-   当拥有此职业的幸存者击杀一个僵尸时：
    -   回复 `ZF_RESOURCEFUL_HEALTH` (25) 点生命值，并可额外获得 `ZF_RESOURCEFUL_HEALTH_OVERHEAL` (15) 点过量治疗。
    -   主武器和副武器弹药按比例 (`ZF_RESOURCEFUL_AMMOPCT`, 20%) 回复。
    -   回复 `ZF_RESOURCEFUL_METAL` (25) 点金属。

### `perk_OnAmmoPickup`

-   当拥有此职业的幸存者拾取弹药包时：
    -   主武器和副武器弹药回满。
    -   金属回满。
    -   获得一个持续 `ZF_RESOURCEFUL_PICKUP_DURATION` (10) 秒的 `ZF_RESOURCEFUL_ATTACK` (25) 攻击力加成。

### `perk_OnMedPickup`

-   当拥有此职业的幸存者拾取医疗包时：
    -   生命值回满。
    -   获得一个持续 `ZF_RESOURCEFUL_PICKUP_DURATION` (10) 秒的 `ZF_RESOURCEFUL_DEFEND` (25) 防御力加成。