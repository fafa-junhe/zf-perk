# 幸存者职业：领袖 (Leader)

## 介绍

**名称**: 领袖 (Leader)

**简短描述**:
> 领袖(Leader)——放置增益旗帜

**详细描述**:
> 你有暴击加成, 也会给附近的幸存者提供攻击力和防御力加成.
> 发医生语音来放置一个旗帜, 靠近你的旗帜的幸存者都会获得攻击力和防御力加成.
> 旗帜持续90秒, 冷却时间150秒.
>
> “我需要重新集结队伍.”
> 推荐职业: 士兵、爆破手、工程师

## 参数

| 参数名                            | 默认值     | 备注                                                       |
| --------------------------------- | ---------- | ---------------------------------------------------------- |
| `ZF_LEADER_SELF_CRIT`             | 15         | 领袖自身的暴击加成                                         |
| `ZF_LEADER_OTHERS_ATTACK`         | 15         | 对附近幸存者的被动攻击力光环加成                           |
| `ZF_LEADER_OTHERS_RADIUSSQ`       | 122500     | 被动光环的半径（350\*350）                                 |
| `ZF_LEADER_RALLY_SELF_ATTACK`     | 5          | 每个在旗帜范围内的幸存者为领袖提供的攻击力加成             |
| `ZF_LEADER_RALLY_SELF_DEFEND`     | 5          | 每个在旗帜范围内的幸存者为领袖提供的防御力加成             |
| `ZF_LEADER_RALLY_OTHERS_ATTACK`   | 15         | 旗帜为范围内的其他幸存者提供的攻击力加成                   |
| `ZF_LEADER_RALLY_OTHERS_DEFEND`   | 15         | 旗帜为范围内的其他幸存者提供的防御力加成                   |
| `ZF_LEADER_RALLY_DURATION`        | 90         | 旗帜的持续时间（秒）                                       |
| `ZF_LEADER_RALLY_COOLDOWN`        | 150        | 放置旗帜的冷却时间（秒）                                   |
| `ZF_LEADER_RALLY_RADIUSSQ`        | 160000     | 旗帜效果的半径（400\*400）                                 |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   暴击率 (`ZFStatCrit`) 增加 `ZF_LEADER_SELF_CRIT` (15)。

### `updateClientPermEffects`

-   为领袖玩家创建一个红色的光环效果 (`ZFPART_AURAINRED`)。

### `updateCondStats`

-   **被动光环**:
    -   周期性检查，为半径 `ZF_LEADER_OTHERS_RADIUSSQ` 内的幸存者（不包括领袖自己）提供 `ZF_LEADER_OTHERS_ATTACK` (15) 的攻击力加成。
-   **旗帜光环**:
    -   如果领袖放置了旗帜 (`zf_item[thisSur][0]` 有效)，则：
        -   为旗帜半径 `ZF_LEADER_RALLY_RADIUSSQ` 内的其他幸存者提供 `ZF_LEADER_RALLY_OTHERS_ATTACK` (15) 的攻击力加成和 `ZF_LEADER_RALLY_OTHERS_DEFEND` (15) 的防御力加成。
        -   根据旗帜范围内的幸存者数量，为领袖自己提供额外的攻击和防御加成（每个幸存者 `ZF_LEADER_RALLY_SELF_ATTACK` 和 `ZF_LEADER_RALLY_SELF_DEFEND`）。
-   **计时器**:
    -   管理旗帜的持续时间和冷却时间。
    -   当旗帜消失或冷却结束时，会向玩家发送提示信息。
    -   HUD会显示旗帜是否准备就绪。

### `perk_OnCallForMedic`

-   当领袖玩家按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上并且处于蹲伏状态。
    -   如果满足条件，则在玩家位置放置一个旗帜模型 (`ZFMDL_FLAG`)。
    -   设置 `ZF_LEADER_RALLY_COOLDOWN` (150) 秒的冷却时间。
    -   旗帜在 `ZF_LEADER_RALLY_DURATION` (90) 秒后消失。