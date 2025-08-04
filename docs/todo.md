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


- [x] 1. 修复Geomancer
- [x] 2. 修复吐痰道具乱抽

- [x] 8. 加charger冲刺速度
- [x] 9. 幻影僵尸能在准备时间飞出来
- [x] 10. 修复Leap力度过小
- [x] 12. 修复Alchemist、Sick、tarred、rage开局/死亡冷却
- [x] 13. 让Scorching能够防御pyro除近战之外的所有伤害

- [x] 16. 修复tarred/sick目前没有任何作用
- [x] 17. 加入能够检视队友属性面板的功能

- [x] 20. 修复Rage的速度只能加成一下
- [x] 21. 加强Alchemist的冷却

# ZF 4.5.1 版本更新
## 修改:
> **狙击手**从人类职业改为僵尸职业
> 自带一把使用速度很慢、伤害很低的弓

## 平衡：
- **charger冲撞僵尸**
> 2000速度-> 1400
> 50撞墙伤害 -> 80
> 3眩晕时间 -> 2
> 15伤害减益 -> 20
- **rage狂怒僵尸**
> 触发时1.5倍生命值 -> 1.3
> 触发时10秒加速 -> 5
- **toxic剧毒僵尸**
> 根据距离远近来决定伤害大小，伤害范围0-8
> 最大范围600 -> 500
> toxic颜色变绿
- **static电场僵尸**
> 范围250 -> 350
- **horrifying惊吓僵尸**
> 近战攻击带有惊吓效果
> 近战攻击可以扣对方8点攻击力、防御力属性值
> 近战攻击可以扣对方15点暴击率、速度属性值
> 自带有-25的开火速度削弱
- **carpenter木工**
> 暂时移除栅栏之间距离放置限制
- **cowardly懦夫**
> 30秒冷却时间 -> 20秒
- **holy牧师**
> -25攻击力 -> +10防御力
- **geomancer唤石师**
> 150唤石距离 -> 200



## 其他
- [x] 22. 修复回合间的卡顿
- [x] 6. 修复僵尸heavy能够捡到幸存者的散弹枪
- [x] 23. 增加一个职业选择记录
- [x] 18. 支持给不同客户端展示不同的描述
- [x] 19. 修复最大子弹的获取
- [x] 7. 修复charger可能会把幸存者卡到墙壁里
- [x] 4. 让最后死的幸存者优先变成下一轮的僵尸以防止游玩过久的僵尸或者幸存者
- [x] 3. 修复Horrifying永久伤害减益
~~- [ ] 11. 加入小偷~~
- [ ] 5. 修复spy能够伪装 [暂时没试出来怎么触发]
 话说/zf - 4 - 4 里面的几个条目都查看不了
