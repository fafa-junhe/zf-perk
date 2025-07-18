# 幸存者职业：仓鼠 (Stash)

**注意：该职业有漏洞，不建议使用。**

## 介绍

**名称**: 仓鼠 (Stash)

**简短描述**:
> /BUG/仓鼠(Stash)——放置藏身处

**详细描述**:
> /该职业有漏洞,请勿游玩/
> 你可以通过蹲下来发医生语音来放置一个藏身处.
> 藏身处需要远离其他幸存者或藏身处.
> 藏身处会为你补充弹药、生命,并提供临时加成.
> 藏身处持续时间30秒,冷却时间30秒.

## 参数

| 参数名                       | 默认值   | 备注                                                       |
| ---------------------------- | -------- | ---------------------------------------------------------- |
| `STASH_GRAB_ATTACK_DURATION` | 45       | 拾取藏身处后临时攻击加成的持续时间（秒）                   |
| `STASH_GRAB_ATTACK_PERM`     | 10       | 拾取藏身处后的永久攻击力加成                               |
| `STASH_GRAB_ATTACK_TEMP`     | 100      | 拾取藏身处后的临时攻击力加成                               |
| `STASH_GRAB_HEALTH`          | 200      | 拾取藏身处获得的生命值                                     |
| `STASH_COOLDOWN`             | 30       | 藏身处准备就绪后，可以放置新藏身处前的冷却时间（秒）       |
| `STASH_WARMUP`               | 40       | 放置藏身处后，其变为可用状态的准备时间（秒）               |
| `STASH_GRAB_RADSQ`           | 2500     | 拾取藏身处的半径（50\*50）                                 |
| `STASH_DROP_RADSQ_STASH`     | 40000    | 放置新藏身处时，附近不能有其他藏身处的半径（200\*200）     |
| `STASH_DROP_RADSQ_CLIENT`    | 640000   | 放置新藏身处时，附近不能有其他幸存者的半径（800\*800）     |

## 逻辑处理

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   检查冷却时间 (`zf_perkTimer`) 是否为0。
    -   检查玩家是否在地面上、处于蹲伏状态。
    -   检查附近 `STASH_DROP_RADSQ_CLIENT` 范围内是否有其他幸存者。
    -   检查附近 `STASH_DROP_RADSQ_STASH` 范围内是否有其他藏身处。
    -   如果所有条件满足，则在玩家位置放置一个补给箱模型 (`ZFMDL_SUPPLYCRATE`) 作为藏身处。
    -   设置一个总共 `STASH_WARMUP + STASH_COOLDOWN` (70) 秒的计时器。

### `updateCondStats`

-   **计时器管理**:
    -   计时器启动后，经过 `STASH_WARMUP` (40) 秒，藏身处变为“准备就绪”状态，并向玩家发送提示。
    -   计时器走完后，玩家可以放置新的藏身处。
-   **拾取逻辑**:
    -   当藏身处“准备就绪”后 (`zf_perkTimer[thisSur] <= STASH_COOLDOWN`)，如果玩家进入其 `STASH_GRAB_RADSQ` 范围内：
        -   玩家获得 `STASH_GRAB_HEALTH` (200) 点生命（可过量治疗）。
        -   弹药和金属回满。
        -   获得一个永久的 `STASH_GRAB_ATTACK_PERM` (10) 攻击力加成。
        -   获得一个持续 `STASH_GRAB_ATTACK_DURATION` (45) 秒的 `STASH_GRAB_ATTACK_TEMP` (100) 临时攻击力加成。
        -   藏身处被消耗并移除。