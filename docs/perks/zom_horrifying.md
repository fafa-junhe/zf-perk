# 僵尸职业: 惊吓僵尸 (Horrifying)

## 描述

**简短描述:**
> 惊吓僵尸(Horrifying)——攻击削弱人类

**详细描述:**
> 你的攻击力降低,但你的攻击能降低幸存者的攻击力、防御力和攻击速度. 
> 减益效果持续15秒.你死亡后,这个效果也随即消失. 
> 
> “敲骨吸髓.” 
> 推荐职业: 机枪手、间谍

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HORRIFYING` | 3 | Perk的ID |
| `HORRIFYING_ATTACK` | -20 | 对非机枪手幸存者施加的攻击力惩罚 |
| `HORRIFYING_ATTACK_HEAVY` | -30 | 对机枪手幸存者施加的攻击力惩罚 |
| `HORRIFYING_DEFEND` | 0 | 对非机枪手幸存者施加的防御力惩罚 |
| `HORRIFYING_DEFEND_HEAVY` | 0 | 对机枪手幸存者施加的防御力惩罚 |
| `HORRIFYING_ROF_HEAVY` | -10 | 对机枪手幸存者施加的攻击速度惩罚 |
| `HORRIFYING_DURATION` | 15 | 惩罚持续时间（秒） |
| `HORRIFYING_DURATION_HEAVY` | 30 | 对机枪手幸存者的惩罚持续时间（秒） |
| `Float:HORRIFYING_PENALTYPCT_KILL` | 0.75 | 击杀惊吓僵尸后，惩罚效果减少的百分比 |
| `Float:HORRIFYING_PENALTYPCT_ASSIST` | 0.25 | 助攻击杀惊吓僵尸后，惩罚效果减少的百分比 |

## 核心逻辑

1.  **光环效果**:
    *   在 `updateClientPermEffects` 函数中，如果玩家是惊吓僵尸，会创建一个蓝色的外层光环 (`ZFPART_AURAOUTBLU`)。

## 事件处理

*   **`perk_OnTakeDamage` (当玩家造成伤害时)**:
    *   当惊吓僵尸通过**近战攻击**伤害一名幸存者时，会触发削弱效果。
    *   该效果通过 `addStatTempStack` 给幸存者施加一个临时的、可叠加的属性惩罚。
    *   惩罚的数值和持续时间取决于惊吓僵尸是否为机枪手 (`isHeavy`)，分别对应 `_HEAVY` 后缀的参数。
    *   惩罚的属性包括：攻击力 (`ZFStatAtt`)、防御力 (`ZFStatDef`) 和攻击速度 (`ZFStatRof`)。

*   **`perk_OnPlayerDeath` (当玩家死亡时)**:
    *   当惊吓僵尸被一名幸存者击杀或助攻击杀时，会减少该幸存者身上由惊吓僵尸施加的临时属性惩罚。
    *   通过 `scaleStatTempPct` 函数，将幸存者身上的负面状态（攻击、防御、攻速）按 `HORRIFYING_PENALTYPCT_KILL` 或 `HORRIFYING_PENALTYPCT_ASSIST` 的百分比进行缩减，从而减轻debuff效果。