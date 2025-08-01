1.幽灵打复仇僵尸会直接死（复仇僵尸重新写了）
2.幽灵自杀的回满血bug
3.山加冷却，减血量，位置改为自己的正前方
4.降低易爆伤害和范围
5.L 08/01/2025 - 00:27:18: [SM] Exception reported: Property "m_vecOrigin" not found (entity 161/instanced_scripted_scene)
L 08/01/2025 - 00:27:18: [SM] Blaming: zombie_fortress_perk.smx
L 08/01/2025 - 00:27:18: [SM] Call stack trace:
L 08/01/2025 - 00:27:18: [SM]   [0] GetEntPropVector
L 08/01/2025 - 00:27:18: [SM]   [1] Line 400, src/sourcepawn\perks\zombie\SickPerk.inc::SickPerkFonGameFrame
L 08/01/2025 - 00:27:18: [SM]   [3] Call_Finish
L 08/01/2025 - 00:27:18: [SM]   [4] Line 93, src/sourcepawn\perks\zombie\..\BasePerk.inc::BasePerk.onGameFrame
L 08/01/2025 - 00:27:18: [SM]   [5] Line 1740, src/sourcepawn\zf_perk.inc::perk_OnGameFrame
L 08/01/2025 - 00:27:18: [SM]   [6] Line 324, src/sourcepawn/zombie_fortress_perk.sp::OnGameFrame
6.可能修复了步哨升级bug

7.修复地卜师
8.修复陷阱大师冷却时间不走
9.修复红队还是有可能有僵尸skin
10.禁用永生的汇报
11.禁用菜刀（可以用包装纸杀手代替）
12.修复静电场特效干扰视野了
13.修复登山家有摔伤
14.幸存者在准备时间内自杀可以直接复活，不用等冷却
15.修复toxic的毒气
16.修复以下职业的平衡: Phatanom幽影, Volatile易爆