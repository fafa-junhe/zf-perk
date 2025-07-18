# 幸存者职业: 懦夫 (Cowardly)

## 描述

**简短描述:**
> 懦夫(Cowardly)——随时准备逃跑

**详细描述:**
> 当你被攻击时,自动激活你的被动能力“恐慌”.  
> 恐慌会给你防御力和速度加成. 
> 恐慌能力持续时间5秒,冷却时间30秒. 
> 
> “别守着了,快逃命吧!” 
> 推荐职业: 工程师、医生、狙击手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_COWARDLY` | 4 | Perk的ID |
| `ZF_COWARDLY_DEFEND` | 50 | 恐慌状态下的防御力加成 (+50%) |
| `ZF_COWARDLY_SPEED` | 200 | 恐慌状态下的速度加成 (+200%) |
| `ZF_COWARDLY_DURATION_SCARED` | 5 | 恐慌状态的持续时间 (秒) |
| `ZF_COWARDLY_DURATION_COOLDOWN` | 30 | 恐慌能力的冷却时间 (秒) |

## 核心逻辑

1.  **触发恐慌**:
    *   在 `perk_OnTakeDamage` 函数中，当懦夫玩家被僵尸的**近战攻击**击中时，会触发恐慌逻辑。
    *   **条件检查**:
        *   玩家的 `zf_perkTimer` 必须为0，表示技能不在冷却中。
    *   **激活**:
        *   如果满足条件，`zf_perkTimer` 会被设置为 `ZF_COWARDLY_DURATION_SCARED + ZF_COWARDLY_DURATION_COOLDOWN` (5 + 30 = 35秒)。
        *   立即给予玩家 `ZF_COWARDLY_DEFEND` 和 `ZF_COWARDLY_SPEED` 的属性加成。
        *   播放 `fxYikes` 特效并提示玩家。
        *   该次攻击的伤害将被完全免疫 (`damage = 0.0`)。

2.  **恐慌状态管理**:
    *   在 `updateCondStats` 函数中，每秒对 `zf_perkTimer` 进行更新。
    *   **恐慌期间** (`zf_perkTimer > ZF_COWARDLY_DURATION_COOLDOWN`): 玩家会持续获得防御和速度加成。
    *   **恐慌结束** (`zf_perkTimer == ZF_COWARDLY_DURATION_COOLDOWN`): 提示玩家恐慌状态结束，此时属性加成消失。
    *   **冷却结束** (`zf_perkTimer == 0`): 提示玩家技能准备就绪。

## 事件处理

*   **`perk_OnTakeDamage`**: 当被僵尸近战攻击时，如果技能未在冷却，则触发“恐慌”状态，免疫当次伤害，并进入持续5秒的增益状态和30秒的冷却期。
*   **`updateCondStats`**: 每秒更新一次。
    *   管理恐慌状态的计时器。
    *   在恐慌期间提供属性加成。
    *   在状态变化时向玩家发送提示信息。
    *   更新HUD，显示技能是否准备就绪。
