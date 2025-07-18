# 僵尸职业: 标记僵尸 (Marked)

## 描述

**简短描述:**
> 标记僵尸(Marked)——瞄准特定目标

**详细描述:**
> 系统会随机选择一名幸存者作为你的目标.
> 你对目标能造成极高伤害,但是对其他人造成较低伤害. 
> 当前目标死亡后,若剩余的幸存者超过1个,10秒后将自动选择一个新目标. 
> 
> “目标已经标记出来了!” 
> 推荐职业: 侦察兵、机枪手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_MARKED` | 7 | Perk的ID |
| `ZF_MARKED_ATTACK_ON_MARK` | 200 | 对标记目标造成的额外攻击加成 |
| `ZF_MARKED_ATTACK_OFF_MARK` | -10 | 对非标记目标造成的攻击惩罚 |
| `ZF_MARKED_MIN_SURVIVORS` | 1 | 触发标记所需的最少幸存者数量 |
| `ZF_MARKED_TIMER` | 10 | 重新选择标记目标的冷却时间（秒） |

## 核心逻辑

1.  **选择目标**:
    *   在 `perk_OnGraceEnd` (准备时间结束) 或 `doMarkedSelect` 函数被调用时，系统会开始选择目标。
    *   如果存活的幸存者数量不少于 `ZF_MARKED_MIN_SURVIVORS`，系统会从存活的幸存者中随机选择一个作为目标，并将其ID存入 `zf_perkState[client]`。
    *   一个感叹号图标 (`ZFSPR_EXCLAMATION`) 会被创建并显示在被标记的幸存者头上，但这个图标只对该标记僵尸可见。
    *   标记僵尸和被标记的幸存者都会收到相应的提示信息。

2.  **伤害调整**:
    *   在 `perk_OnTakeDamage` 事件中，当标记僵尸用**近战攻击**伤害一名幸存者时：
        *   如果受害者是被标记的目标 (`zf_perkState[attacker] == victim`)，则攻击力会增加 `ZF_MARKED_ATTACK_ON_MARK`。
        *   如果受害者不是被标记的目标，则攻击力会减少 `ZF_MARKED_ATTACK_OFF_MARK`。

3.  **重新选择目标**:
    *   在 `updateCondStats` 的周期性检查中，会持续监控被标记目标的状态。
    *   如果被标记的幸存者死亡 (`!validLivingSur(zf_perkState[thisZom])`)，系统会清除标记图标，并将 `zf_perkState[thisZom]` 设为0，然后启动一个 `ZF_MARKED_TIMER` 秒的冷却计时器。
    *   计时器结束后，会自动调用 `doMarkedSelect` 来选择一个新的目标。

## 事件处理

*   **`perk_OnGraceEnd`**: 游戏回合正式开始时，为标记僵尸选择第一个目标。
*   **`perk_OnTakeDamage`**: 处理对标记目标和非标记目标的伤害调整。
*   **`updateCondStats`**: 监控目标存活状态，并在其死亡后启动重新选择目标的流程。
*   **`perk_OnSetTransmit`**: 控制标记图标只对特定的标记僵尸可见。