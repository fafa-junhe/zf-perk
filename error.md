```L 07/27/2025 - 01:17:57: [SM] Exception reported: Array index out-of-bounds (index 83, limit 5)
L 07/27/2025 - 01:17:57: [SM] Blaming: zombie_fortress_perk.smx
L 07/27/2025 - 01:17:57: [SM] Call stack trace:
L 07/27/2025 - 01:17:57: [SM]   [1] Line 1325, src/sourcepawn\zf_perk.inc::validItem
L 07/27/2025 - 01:17:57: [SM]   [2] Line 1312, src/sourcepawn\zf_perk.inc::removeItem
L 07/27/2025 - 01:17:57: [SM]   [3] Line 285, src/sourcepawn\perks\survival\NinjaPerk.inc::NinjaPerkFdoNinjaDecoyPoof
L 07/27/2025 - 01:17:57: [SM]   [5] Call_Finish
L 07/27/2025 - 01:17:57: [SM]   [6] Line 156, src/sourcepawn\perks\survival\NinjaPerk.inc::NinjaPerk.doNinjaDecoyPoof
L 07/27/2025 - 01:17:57: [SM]   [7] Line 257, src/sourcepawn\perks\survival\NinjaPerk.inc::NinjaPerkFonPeriodic
L 07/27/2025 - 01:17:57: [SM]   [9] Call_Finish
L 07/27/2025 - 01:17:57: [SM]   [10] Line 101, src/sourcepawn\perks\BasePerk.inc::BasePerk.onPeriodic
L 07/27/2025 - 01:17:57: [SM]   [11] Line 1642, src/sourcepawn\zf_perk.inc::perk_OnPeriodic
L 07/27/2025 - 01:17:57: [SM]   [12] Line 990, src/sourcepawn/zombie_fortress_perk.sp::timer_main
```
```
L 07/27/2025 - 01:12:27: [SM] Exception reported: Property "m_iClass" not found (entity 0/worldspawn)
L 07/27/2025 - 01:12:27: [SM] Blaming: zombie_fortress_perk.smx
L 07/27/2025 - 01:12:27: [SM] Call stack trace:
L 07/27/2025 - 01:12:27: [SM]   [0] GetEntProp
L 07/27/2025 - 01:12:27: [SM]   [1] Line 386, D:\workspace\code\tf2\zf\include\tf2_stocks.inc::TF2_GetPlayerClass
L 07/27/2025 - 01:12:27: [SM]   [2] Line 880, src/sourcepawn\zf_util_base.inc::isSpy
L 07/27/2025 - 01:12:27: [SM]   [3] Line 1092, src/sourcepawn\zf_util_base.inc::attackWasBackstab
L 07/27/2025 - 01:12:27: [SM]   [4] Line 65, src/sourcepawn\perks\survival\TurtlePerk.inc::TurtlePerkFonTakeDamage
L 07/27/2025 - 01:12:27: [SM]   [6] Call_Finish
L 07/27/2025 - 01:12:27: [SM]   [7] Line 121, src/sourcepawn\perks\BasePerk.inc::BasePerk.onTakeDamage
L 07/27/2025 - 01:12:27: [SM]   [8] Line 2030, src/sourcepawn\zf_perk.inc::perk_OnTakeDamage
L 07/27/2025 - 01:12:27: [SM]   [9] Line 364, src/sourcepawn/zombie_fortress_perk.sp::OnTakeDamag
```
```
L 07/27/2025 - 01:37:50: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:37:51: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:37:51: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:39:58: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:39:58: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:40:08: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
L 07/27/2025 - 01:40:08: [zombie_fortress_perk.smx] SDKCall for GetMaxAmmo is invalid!
```