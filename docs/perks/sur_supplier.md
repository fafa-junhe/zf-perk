# 幸存者职业：供应商 (Supplier)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 供应商 (Supplier)

**简短描述**:
> /BUG/供应商(Supplier)——放置弹药补给箱

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你的弹药会周期性的补充,并且拥有双倍备弹量.
> 发医生语音来放置一个补给箱,其他幸存者可以从中补给弹药.
> 冷却时间10秒.

## 参数

| 参数名                           | 默认值 | 备注                                                               |
| -------------------------------- | ------ | ------------------------------------------------------------------ |
| `ZF_SUPPLIER_MAX_ITEMS`          | 2      | 最多可以同时存在的补给箱数量                                       |
| `ZF_SUPPLIER_TIMER`              | 10     | 放置补给箱的冷却时间（秒）                                         |
| `ZF_SUPPLIER_RADIUSSQ`           | 5625   | 拾取补给的半径（75\*75）                                           |
| `ZF_SUPPLIER_UPDATERATE`         | 10     | 自身弹药自动补充的周期（秒）                                       |
| `ZF_SUPPLIER_SELF_DEFEND`        | 25     | 永久防御力加成                                                     |
| `ZF_SUPPLIER_AMMOPCT_RESLIMIT`   | 2.0    | 主武器和副武器的备弹量上限倍率（双倍备弹）                         |
| `ZF_SUPPLIER_AMMOPCT_SELF`       | 0.10   | 每次自我补充时获得的弹药百分比                                     |
| `ZF_SUPPLIER_AMMOPCT_OTHER`      | 0.25   | 其他幸存者从补给箱中获得的弹药百分比                               |
| `ZF_SUPPLIER_RESUPPLY_COUNT`     | 4      | 每个补给箱可被使用的次数                                           |
| `ZF_SUPPLIER_ATTACK`             | 25     | 当补给被拾取时，供应商获得的临时攻击力加成                         |
| `ZF_SUPPLIER_DURATION`           | 10     | 供应商获得临时攻击力加成的持续时间（秒）                           |

## 逻辑处理

### `updateClientPermStats`

-   在玩家重生时，如果选择了此职业，将应用以下永久属性变更：
    -   防御力 (`ZFStatDef`) 增加 `ZF_SUPPLIER_SELF_DEFEND` (25)。

### `updateCondStats`

-   **自我补给**:
    -   每隔 `ZF_SUPPLIER_UPDATERATE` (10) 秒，会自动为供应商补充 `ZF_SUPPLIER_AMMOPCT_SELF` (10%) 的弹药和金属，上限为 `ZF_SUPPLIER_AMMOPCT_RESLIMIT` (2.0) 倍。
-   **补给箱逻辑**:
    -   检查其他幸存者是否在补给箱的 `ZF_SUPPLIER_RADIUSSQ` 范围内。
    -   如果幸存者弹药未满，他们会自动拾取补给，回复 `ZF_SUPPLIER_AMMOPCT_OTHER` (25%) 的弹药和金属。
    -   每次有幸存者拾取补给，供应商会获得一个持续 `ZF_SUPPLIER_DURATION` (10) 秒的 `ZF_SUPPLIER_ATTACK` (25) 攻击力加成。
    -   每个补给箱有 `ZF_SUPPLIER_RESUPPLY_COUNT` (4) 次使用次数，用完后消失。
-   **计时器与HUD**:
    -   管理放置补给箱的冷却时间，并在HUD上显示是否准备就绪。

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   检查冷却时间、是否在地面、是否蹲伏以及是否有可用的物品栏位。
    -   如果满足条件，则在玩家位置放置一个补给箱模型 (`ZFMDL_SUPPLYCRATE`)。
    -   设置 `ZF_SUPPLIER_TIMER` (10) 秒的冷却时间。