# 僵尸职业: 猎手僵尸 (Hunter)

## 描述

**简短描述:**
> 猎手僵尸(Hunter)——手动放置重生点

**详细描述:**
> 发医生语音来放置你的重生点. 
> 从自己的重生点重生时,你的重生时间较短,并获得临时的攻击加成. 
> 每次重生后,你只能放置一次重生点. 
> 注意!幸存者可以摧毁你的重生点. 
> 
> “你将成为我的猎物!” 
> 推荐职业: 任何

## 参数

| 参数名 | 值 | 描述 |
| --- | --- | --- |
| `ZF_PERK_HUNTER` | 4 | Perk的ID |
| `ZF_HUNTER_ATTACK` | 50 | 在重生点重生时获得的临时攻击加成 |
| `ZF_HUNTER_DURATION` | 10 | 临时攻击加成的持续时间（秒） |
| `ZF_HUNTER_RADIUSSQ` | (85 * 85) | 幸存者摧毁重生点的半径（平方） |
| `Float:ZF_HUNTER_RESPAWNTIME` | 5.5 | 死亡后的快速重生时间（秒） |

## 核心逻辑

1.  **放置重生点**:
    *   在 `perk_OnCallForMedic` 事件中，如果玩家是猎手僵尸且本回合尚未放置过重生点 (`zf_perkState[client] == 0`)，则可以在当前位置创建一个重生点。
    *   重生点通过一个视觉光环 (`ZFPART_AURAVORTEXBLU`) 表示，并记录下当前的观察角度 (`zf_perkPos[client][0]`) 用于重生。
    *   放置后，状态 `zf_perkState[client]` 会被设为1，防止重复放置。

2.  **在重生点重生**:
    *   在 `perk_OnPlayerSpawn` 事件中，如果猎手僵尸拥有一个有效的重生点 (`validAura(client)`)，他将被传送到该点。
    *   传送后，玩家会获得 `ZF_HUNTER_ATTACK` 的临时攻击力加成，持续 `ZF_HUNTER_DURATION` 秒。
    *   同时，为了兼容性，会移除玩家的重生保护。

3.  **重生点被摧毁**:
    *   在 `updateCondStats` 的周期性检查中，如果一个幸存者进入了某个重生点 `ZF_HUNTER_RADIUSSQ` 范围，该重生点将被移除 (`removeAura`)，并向双方玩家显示提示信息。

## 事件处理

*   **`perk_OnCallForMedic` (玩家按“E”键)**:
    *   触发放置重生点的逻辑。

*   **`perk_OnPlayerSpawn` (玩家重生)**:
    *   触发在重生点重生的逻辑。

*   **`perk_OnPlayerDeath` (玩家死亡)**:
    *   如果猎手僵尸死亡，会通过 `showAura` 重新显示重生点光环（如果存在）。
    *   同时，会创建一个计时器，在 `ZF_HUNTER_RESPAWNTIME` 秒后尝试重生玩家（此逻辑与其他快速重生职业共享）。

*   **`updateCondStats` (周期性更新)**:
    *   触发重生点被摧毁的逻辑检查。