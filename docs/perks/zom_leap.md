# 僵尸职业: 飞跃僵尸 (Leap)

## 描述

**简短描述:**
> 飞跃僵尸(Leap)——大跳飞向空中

**详细描述:**
> 你的攻击力与防御力降低,但不受坠落伤害. 
> 发医生语音来施展大跳,冷却时间4秒. 
> 
> “起飞!” 
> 推荐职业: 侦察兵、间谍

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_LEAP` | 5 | Perk的ID |
| `ZF_LEAP_COMBAT` | -20 | 攻击力和防御力的永久惩罚 (-20) |
| `ZF_LEAP_COOLDOWN` | 4 | 大跳技能的冷却时间（秒） |
| `Float:ZF_LEAP_FORCE` | 900.0 | 大跳的力度 |
| `Float:ZF_LEAP_FORCE_SCOUT` | 1500.0 | 侦察兵(Scout)的大跳力度 |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了飞跃僵尸，会应用永久性的攻击力和防御力惩罚 (`ZF_LEAP_COMBAT`)。

2.  **免疫坠落伤害**:
    *   在 `perk_OnTakeDamage` 事件中，会检查伤害类型。如果是坠落伤害 (`attackWasSelfFall`)，则将伤害设置为0。

## 事件处理

*   **`perk_OnCallForMedic` (玩家按“E”键)**:
    *   如果技能不在冷却中 (`zf_perkTimer[client] == 0`)，且玩家在地面上 (`isGrounded`) 并且没有隐身 (`!isCloaked`)，则触发大跳。
    *   大跳通过 `fxJump` 函数实现，力度根据玩家是否为侦察兵 (`isScout`) 分别使用 `ZF_LEAP_FORCE_SCOUT` 或 `ZF_LEAP_FORCE`。
    *   触发后，技能进入冷却，时间为 `ZF_LEAP_COOLDOWN` 秒。

*   **`updateCondStats` (周期性更新)**:
    *   处理大跳技能的冷却倒计时。
    *   更新HUD，显示技能是否准备就绪。