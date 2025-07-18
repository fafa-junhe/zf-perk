# 僵尸职业: 自爆僵尸 (Combustible)

## 描述

**简短描述:**
> 自爆僵尸(Combustible)——尸如其名

**详细描述:**
> 你的防御力大幅降低. 
> 被远程武器击杀后,你会爆炸并造成伤害. 
> 你不可以使用隐身手表或者原子能饮料. 
> (远程武器包括一切非近战武器) 
> 
> *将Boomer先推开再攻击* 
> 推荐职业: 侦察兵、机枪手

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_COMBUSTIBLE` | 2 | Perk的ID |
| `ZF_COMBUSTIBLE_DAMAGE` | 120 | 爆炸伤害 |
| `ZF_COMBUSTIBLE_DAMAGE_HEAVY` | 200 | 作为机枪手(Heavy)时的爆炸伤害 |
| `ZF_COMBUSTIBLE_DEFEND` | -200 | 防御力惩罚 (-200%) |
| `ZF_COMBUSTIBLE_RADIUS` | 300 | 爆炸半径 |
| `Float:ZF_COMBUSTIBLE_RESPAWNTIME` | 4.5 | 爆炸后的重生时间（秒） |

## 核心逻辑

1.  **永久属性修改**:
    *   在 `updateClientPermStats` 函数中，如果玩家选择了自爆僵尸，会应用永久性的防御力惩罚 (`ZF_COMBUSTIBLE_DEFEND`)。

2.  **视觉特效**:
    *   在 `updateCondStats` 函数中，会周期性地在玩家身上产生微小的爆炸特效 (`fxExplosionTiny`)，以提示其职业特性。

## 事件处理

*   **`OnPlayerRunCmd` (玩家输入指令)**:
    *   阻止玩家使用原子能饮料 (`IN_ATTACK` + `ZFWEAP_BONK`) 和隐身手表 (`IN_ATTACK2`)，并给出提示。

*   **`perk_OnPlayerDeath` (玩家死亡)**:
    *   如果自爆僵尸被**非近战武器**击杀 (`!attackWasMelee`)，则会触发爆炸。
    *   爆炸伤害根据职业 (`isHeavy`) 分为 `ZF_COMBUSTIBLE_DAMAGE` 和 `ZF_COMBUSTIBLE_DAMAGE_HEAVY`。
    *   爆炸通过 `applyDamageRadialAtClient` 对 `ZF_COMBUSTIBLE_RADIUS` 范围内的敌人造成伤害。
    *   触发一次大型爆炸视觉特效 (`fxExplosionBig`)。
    *   通过计时器 `perk_tSpawnClient` 在 `ZF_COMBUSTIBLE_RESPAWNTIME` 秒后强制重生该玩家。