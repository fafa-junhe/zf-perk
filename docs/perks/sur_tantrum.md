# 幸存者职业：暴脾气 (Tantrum)

## 介绍

**名称**: 暴脾气 (Tantrum)

**简短描述**:
> 暴脾气(Tantrum)——短时间内获得暴击

**详细描述**:
> 发医生语音来激活愤怒.
> 愤怒给予你15秒的暴击,结束后进入30秒的疲惫状态.
> 疲惫状态下,移动速度大幅降低.
>
> “我想这个职业的确很解压.”
> 推荐职业: 士兵、火焰兵、爆破手

## 参数

| 参数名                | 默认值 | 备注                               |
| --------------------- | ------ | ---------------------------------- |
| `ZF_TANTRUM_ACTIVE`   | 15     | 愤怒（100%暴击）状态的持续时间（秒） |
| `ZF_TANTRUM_COOLDOWN` | 30     | 疲惫（速度惩罚）状态的持续时间（秒） |
| `ZF_TANTRUM_SPEED`    | -100   | 疲惫状态下的移动速度惩罚           |

## 逻辑处理

### `perk_OnCallForMedic`

-   当拥有此职业的幸存者按下“医生”语音键时：
    -   如果技能冷却完毕 (`zf_perkTimer[client] == 0`)，则激活技能。
    -   设置一个总共 `ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN` (45) 秒的计时器。
    -   在 `ZF_TANTRUM_ACTIVE` (15) 秒内，玩家获得100%暴击 (`addCondKritz`)。
    -   触发暴击视觉效果 (`fxKritzStart`)。

### `updateCondStats`

-   **计时器管理**:
    -   当计时器在 `(0, ZF_TANTRUM_COOLDOWN]` 区间时，玩家处于疲惫状态，移动速度受到 `ZF_TANTRUM_SPEED` (-100) 的惩罚。
    -   当计时器在 `(ZF_TANTRUM_COOLDOWN, ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN]` 区间时，玩家处于愤怒状态，获得暴击。
    -   当愤怒状态结束时，暴击效果停止 (`fxKritzStop`)，并提示玩家进入疲惫状态。
    -   当疲惫状态结束时，提示玩家技能准备就绪。
-   **HUD**:
    -   HUD会显示技能是否准备就绪。