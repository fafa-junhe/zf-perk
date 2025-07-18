#pragma newdecls required

#if defined _ZF_PERK_STRUCTS_INC
  #endinput
#endif
#define _ZF_PERK_STRUCTS_INC

//
// Perk Objects
//
#define ZF_PERK_NONE            0
  
// Survivor perks
#define TOTAL_SUR_PERKS 22
#define ZF_PERK_ATHLETIC        1
#define ZF_PERK_CARPENTER       2
#define ZF_PERK_CHARITABLE      3 
#define ZF_PERK_COWARDLY        4 
#define ZF_PERK_FRIEND          5 
#define ZF_PERK_HEROIC          6 
#define ZF_PERK_HOLY            7
#define ZF_PERK_JUGGERNAUT      8
#define ZF_PERK_LEADER          9 
#define ZF_PERK_NINJA           10
#define ZF_PERK_NONLETHAL       11
#define ZF_PERK_RESOURCEFUL     12
#define ZF_PERK_SELFLESS        13
#define ZF_PERK_STASH           14
#define ZF_PERK_STIRCRAZY       15
#define ZF_PERK_SUPPLIER        16
#define ZF_PERK_TANTRUM         17
#define ZF_PERK_TRAPPER         18
#define ZF_PERK_TURTLE          19
#define ZF_PERK_WISE            20
#define ZF_PERK_ZENLIKE         21

// Zombie perks
#define TOTAL_ZOM_PERKS 18
#define ZF_PERK_ALPHA           1 
#define ZF_PERK_COMBUSTIBLE     2 
#define ZF_PERK_HORRIFYING      3
#define ZF_PERK_HUNTER          4 
#define ZF_PERK_LEAP            5 
#define ZF_PERK_MAGNETIC        6
#define ZF_PERK_MARKED          7
#define ZF_PERK_RAGE            8
#define ZF_PERK_ROAR            9 
#define ZF_PERK_SCORCHING       10
#define ZF_PERK_SICK            11
#define ZF_PERK_SWARMING        12
#define ZF_PERK_TARRED          13
#define ZF_PERK_THIEVING        14
#define ZF_PERK_TOXIC           15
#define ZF_PERK_VAMPIRIC        16
#define ZF_PERK_VINDICTIVE      17

//
// Perk Gameplay Objects
//
const ZF_CRIPPLE_DURATION = 45;             // Amount of time cripple condition lasts.
const ZF_DAZE_DEFEND = -40;                 // Defense penalty when dazed.
const ZF_POISON_DAMAGE = 7;                 // Amount of poison damage (per sec).   //改为 7 ，稍微提高该职业威力
const ZF_POISON_HEAL_MEDIC = 5;             // Amount of poison healed (per sec) by medic healing.    //改为 5 ，稍微提高该职业威力
const ZF_POISON_HEAL_MEDPACK = 7;           // Amount of poison healed by medpack pickup.   //改为 7 ，稍微提高该职业威力
const ZF_POISON_HEAL_UBER = 100;            // Amount of poison healed when ubered.

// Survivor perks
const ZF_ATHLETIC_ATTACK = -40;             // Innate attack penalty.   //改为 -40 ，降低该职业的输出能力
const ZF_ATHLETIC_CRIT = -100;              // Innate crit penalty.
const ZF_ATHLETIC_ROF = 100;                // Innate rate of fire bonus.
const ZF_ATHLETIC_SPEED = 100;              // Innate speed bonus.
const CARPENTER_ATTACK = -40;               // Innate attack penalty.   //改为 -40 ，提高该职业的输出能力
const CARPENTER_DEFEND = 25;                // Innate defense bonus.
const CARPENTER_BARRICADE_HEALTH = 500;     // Health of each barricade.    //改为 500 ，提高路障存在感
const CARPENTER_COOLDOWN = 25;              // Duration after barricade is placed before a new one can be placed.
const CARPENTER_MAX_ITEMS = 4;              // Maximum number of barricades that can be active at one time.   //改为 4 ，提高木工这个职业的存在感
const CARPENTER_DROP_RADSQ_BARRICADE = (250 * 250); // Radius (squared) inwhich no barricades must be to place barricade.
const CARPENTER_DROP_RADSQ_CLIENT = (150 * 150);    // Radius (squared) inwhich no other players must be to place barricade.
const ZF_CHARITABLE_MAX_ITEMS = 5;          // Maximum number of gifts that can be active at one time.
const ZF_CHARITABLE_POINTS_ASSIST = 2;      // Points earned per assist.    //改为 2 
const ZF_CHARITABLE_POINTS_KILL = 2;        // Points earned per kill.    //改为 2 
const ZF_CHARITABLE_POINTS_GIFT = 4;        // Points needed to toss gift.    // 改为 4 ，让慈善家能放出更多礼物
const ZF_CHARITABLE_GIFT_BONUS_HEALTH = 75; // Health gained by gift owner when gift is picked up.    //改为 75 ，礼物应该以回血为主，加攻击buff为辅
const ZF_CHARITABLE_GIFT_BONUS_MIN = 10;    // Minimum gift bonus strength.   //改为 10
const ZF_CHARITABLE_GIFT_BONUS_MAX = 30;    // Maximum gift bonus strength.   //改为 30
const ZF_CHARITABLE_GIFT_DURATION = 20;     // Duration of gift bonuses.    //改为 20
const ZF_COWARDLY_DEFEND = 50;              // Defense bonus when scared.   //改为 50 ，目前懦夫这个职业的能力反而让他们能大胆攻击僵尸 如果能添加个降低伤害的效果则更好
const ZF_COWARDLY_SPEED = 200;              // Speed bonus when scared.   //改为 200 ，能快速甩掉僵尸
const ZF_COWARDLY_DURATION_SCARED = 5;     // Duration of scared state after being hit.    //改为 5，以免僵尸根本追不上人类
const ZF_COWARDLY_DURATION_COOLDOWN = 30;   // Duration after scared state ends before scared state is again possible.    //改为 30 ，避免技能多次触发
const ZF_FRIEND_ATTACK = 25;                // Attack bonus when near friend.   //改为 25 ，这个50是认真的?
const ZF_FRIEND_REGEN = 10;                 // Regen bonus when near friend.    
const ZF_FRIEND_CRITTIME_INIT = 0;          // Starting crit time.    //改为 0 ，暴击时间这个效果必须严重削弱
const ZF_FRIEND_CRITTIME_KILL = 4;          // Crit time added per kill.    //改为 4 
const ZF_FRIEND_CRITTIME_ASSIST = 2;        // Crit time added per assist.    //改为 2
const ZF_FRIEND_RADIUSSQ = (300 * 300);     // Radius (squared) inwhich passive bonuses apply.    //改为 300 * 300 ，适当削弱这个纸面实力最强的职业
const HEROIC_COMBAT = 15;                   // Combat bonus when using perk.    //这个combat似乎指的是攻击&防御，建议适当下调；另外好像能给职业数量加限制，这个职业最好是限制在 1 个。
const HEROIC_CRITTIME_INIT = 30;            // Starting crit time.    //改为 30 ，因为后面有削弱
const HEROIC_CRITTIME_KILL = 3;            // Crit time added per kill.    //改为 3
const HEROIC_CRITTIME_KILL_ACTIVE = 0;      // Crit time added per kill when crittime is active.    //改为 0 ，还想着越战越勇?
const HEROIC_CRITTIME_ASSIST = 1;           // Crit time added per assist.    //改为 1
const HEROIC_CRITTIME_ASSIST_ACTIVE = 0;    // Crit time added per assist when crittime is active.    //改为 0，同上
const ZF_HOLY_ATTACK = -25;                 // Attack penalty when using perk.    //改为 -25 ，-50的伤害输出过于离谱
const ZF_HOLY_RADIUSSQ = (400 * 400);       // Radius (squared) inwhich regen bonuses apply.
const ZF_HOLY_REGEN = 10;                    // Regen bonus for nearby survivors.    //改为 10 ，这是1级补给器的回血效率
const ZF_JUGGERNAUT_ATTACK = 50;            // Attack bonus when using perk.    //改为 50 ，让该职业的伤害特别明显
const ZF_JUGGERNAUT_DEFEND = -50;            // Defense bonus when using perk.   //改为 -50 ，让该职业的定位成为玻璃大炮
const Float:ZF_JUGGERNAUT_FORCE = 500.0;    // Knockback force for nearby zombies when fall damage taken / on melee hit.
const ZF_JUGGERNAUT_RADIUS = 150;           // Radius inwhich fall damage causes stun.    //改为 150 ，避免范围过大，在某些地图得以滥用(例如cliff地图中央的梯子)
const ZF_JUGGERNAUT_SPEED = -100;           // Speed penalty when using perk.
const Float:ZF_JUGGERNAUT_STUN_DURATION = 1.0; // Duration of stun on melee hit or when fall damage taken.    //改为 1
const Float:ZF_JUGGERNAUT_STUN_SLOWDOWN = 1.0; // Slowdown of stun on melee hit or when fall damage taken.    //改为 1
const ZF_LEADER_SELF_CRIT = 15;                // Crit bonus when using perk.   //改为 15 ，下调暴击率
const ZF_LEADER_OTHERS_ATTACK = 15;            // Attack bonus for survivors near perk user.    //改为 15 ，下调伤害
const ZF_LEADER_OTHERS_RADIUSSQ = (350 * 350); // Radius (squared) inwhich passive bonuses are applied.
const ZF_LEADER_RALLY_SELF_ATTACK = 5;        // Attack bonus for user per survivor near rally point.    //改为 5 ，此处是每个幸存者给领袖的buff，不宜过强。这个职业最好是限制在 1 个。
const ZF_LEADER_RALLY_SELF_DEFEND = 5;         // Defense bonus for user per survivor near rally point.   
const ZF_LEADER_RALLY_OTHERS_ATTACK = 15;      // Attack bonus for survivors near rally point.    //改为 15
const ZF_LEADER_RALLY_OTHERS_DEFEND = 15;      // Defense bonus for user per survivor near rally point.   //改为 15
const ZF_LEADER_RALLY_DURATION = 90;           // Duration after rally is placed before it expires.   //改为 90
const ZF_LEADER_RALLY_COOLDOWN = 150;          // Duration after rally is placed before it can be replaced.   //改为 150 ，延长空档期
const ZF_LEADER_RALLY_RADIUSSQ = (400 * 400);  // Radius (squared) inwhich rally bonuses apply. 
const ZF_NINJA_ATTACK = -40;                // Attack penalty when using perk.    //改为 -40 ，对标运动员
const ZF_NINJA_SPEED = 50;                  // Speed bonus using perk.
const ZF_NINJA_DURATION_DECOY_ACTIVE = 15;  // Duration of decoy retreat point lifetime.    
const ZF_NINJA_DURATION_DECOY_DECAY = 5;    // Duration of decoy before it poofs.
const ZF_NINJA_DURATION_COOLDOWN = 30;      // Duration after decoy before new decoy can be used.
const ZF_NINJA_FALLDMG_RESIST = 50;         // Percentage of fall damage resistance.
const Float:ZF_NINJA_FORCE = 600.0;         // Jump force.
const ZF_NONLETHAL_ATTACK_BULLET = -90;     // Attack penalty when using perk and bullet-based weapon.    //改为 -90 ，大幅降低子弹本身伤害
const Float:ZF_NONLETHAL_FORCE = 75.0;      // Base force applied per bullet-based attack.    //改为 75 ，大幅提高击退力
const Float:ZF_RESOURCEFUL_AMMOPCT = 0.20;  // Percent of ammo received per kill.   //BUG职业 暂不做调整
const ZF_RESOURCEFUL_ATTACK = 25;           // Attack bonus (temporary) when grabbing an ammopack.
const ZF_RESOURCEFUL_DEFEND = 25;           // Defense bonus (temporary) when grabbing a medpack.
const ZF_RESOURCEFUL_HEALTH = 25;           // Health gained per kill (up to max).
const ZF_RESOURCEFUL_HEALTH_OVERHEAL = 15;  // Additional health gained per kill (overheal possible).
const ZF_RESOURCEFUL_METAL = 25;            // Amount of metal received per kill.
const ZF_RESOURCEFUL_PICKUP_DURATION = 10;  // Duration of temporary bonuses from pickups.
const ZF_SELFLESS_DAMAGE = 10000;            // Explosion damage applied to zombies on death.    //改为 10000
const ZF_SELFLESS_RADIUS = 5000;             // Radius of explosion.   //改为 5000 ，让全场玩家注意到你的牺牲！
const STASH_GRAB_ATTACK_DURATION = 45;      // Duration of temporary bonuses from grabbing stash.   //BUG职业 暂不做调整
const STASH_GRAB_ATTACK_PERM = 10;          // Permanent attack bonus from grabbing stash.
const STASH_GRAB_ATTACK_TEMP = 100;         // Temporary attack bonus from grabbing stash.
const STASH_GRAB_HEALTH = 200;              // Health bonus from grabbing stash.
const STASH_COOLDOWN = 30;                  // Duration after stash becomes ready before a new stash can be placed.
const STASH_WARMUP = 40;                    // Duration after placing stash before it becomes ready.
const STASH_GRAB_RADSQ = (50 * 50);         // Radius (squared) inwhich you must be to grab stash.
const STASH_DROP_RADSQ_STASH = (200 * 200); // Radius (squared) inwhich no other stashes must exist in order to place new stash.
const STASH_DROP_RADSQ_CLIENT = (800 * 800);// Radius (squared) inwhich no other survivors must exist in order to place new stash.
const ZF_STIRCRAZY_MAX_POINTS = 5;          // Number of previous positions used to calculate average position.   //“狂人”输出机制有点复杂，只改最大值吧。建议添加降低生命的效果。
const ZF_STIRCRAZY_DIST_MIN = 150;          // Minimum distance from average of last X positions overwhich bonuses apply.
const ZF_STIRCRAZY_DIST_MAX = 750;          // Maximum distance from average of last X positions overwhich no further bonuses apply.
const ZF_STIRCRAZY_ATTACK = 30;            // Attack bonus when ZF_STIRCRAZY_DIST_MAX is reached.    //改为 30 ，谁给你的权力上200?
const ZF_SUPPLIER_MAX_ITEMS = 2;            // Maximum number of supply boxes that can be active at once.   //BUG职业 暂不做调整
const ZF_SUPPLIER_TIMER = 10;               // Duration after supplies are placed before supplies can be placed again.          
const ZF_SUPPLIER_RADIUSSQ = (75 * 75);     // Radius (squared) inwhich supplies are picked up.
const ZF_SUPPLIER_UPDATERATE = 10;          // Period (in s) of self-resupply.
const ZF_SUPPLIER_SELF_DEFEND = 25;            // Defense bonus when using perk.
const Float:ZF_SUPPLIER_AMMOPCT_RESLIMIT = 2.0; // Primary and secondary ammo reserve limit.
const Float:ZF_SUPPLIER_AMMOPCT_SELF = 0.10;    // Percent of ammo (of standard maximum) received per self-resupply.
const Float:ZF_SUPPLIER_AMMOPCT_OTHER = 0.25;   // Percent of ammo (of standard maximum) received per resupply.
const ZF_SUPPLIER_RESUPPLY_COUNT = 4;       // Number of resupplies from each supply box.
const ZF_SUPPLIER_ATTACK = 25;              // Attack bonus for supplier when supplies picked up.
const ZF_SUPPLIER_DURATION = 10;            // Duration of bonuses for suppliers when supplies picked up.
const ZF_TANTRUM_ACTIVE = 15;               // Duration of anger (100% crit) state.
const ZF_TANTRUM_COOLDOWN = 30;             // Duration of tired (speed penalty) state.
const ZF_TANTRUM_SPEED = -100;              // Speed penalty given after perk use.
const ZF_TRAITOR_CRIT = -100;               // Crit penalty when using this perk.
const ZF_TRAITOR_CRIT_ZOM = 25;             // Crit bonus for nearby zombies.
const ZF_TRAITOR_POISON = 3;                // Poison damage applied per/s to other remaining survivor.
const ZF_TRAITOR_RADIUSSQ = (500 * 500);    // Radius (squared) inwhich zombie crit bonus applies.
const ZF_TRAPPER_MAX_ITEMS = 5;             // Maximum number of mines that can be active at once.    //改为 5 ，建议添加降低生命的效果
const ZF_TRAPPER_DAMAGE = 200;               // Damage done by trapper mines.    //改为 200
const ZF_TRAPPER_RADIUS = 150;              // Radius inwhich mines trigger and cause damage.   //改为 150
const ZF_TRAPPER_RADIUSSQ = (200 * 200);    // Radius (squared) inwhich mines trigger and cause damage.
const ZF_TRAPPER_TIMER = 20;                // Time between use of perk.    //改为 20
const ZF_TURTLE_ATTACK = -50;               // Attack penalty when using this perk.
const ZF_TURTLE_DEFEND = 75;                // Defense bonus when using this perk.    //改为 75 ，肉盾就该硬！基本是“守护神”的反面了
const ZF_TURTLE_SPEED = -100;               // Speed penalty when using perk.
const Float:ZF_TURTLE_STUN_DURATION = 1.0;  // Stun time when zombie backstabs perk user.   //改为 1 ，被背刺后眩晕1秒
const ZF_WISE_ATTACK_KILL = 1;              // Attack bonus when killing a zombie.
const ZF_WISE_ATTACK_ASSIST = 0;            // Attack bonus when assisting in killing a zombie.   //改为 0 ，避免轻松滚雪球 
const ZF_WISE_DEFEND = 1;                   // Defense bonus when hit by a zombie.
const ZF_WISE_DEFEND_LIMIT = 20;            // Defense bonus threshold above which no defense bonus is granted.   //改为 20 ，毕竟有的职业出生就有 20 。
const ZF_ZENLIKE_CRIT_INC = 3;              // Crit bonus gained when crouched and not moving.    //改为 5 ，暴击率流失地还是很快的， 1 实在是太慢了
const ZF_ZENLIKE_CRIT_DEC = 25;             // Crit bonus lost per shot.
const ZF_ZENLIKE_HEAL = 1;                  // Poison healed when crouched and not moving.
const ZF_ZENLIKE_REGEN = 1;                // Health regen bonus when crouched and not moving.    //改为 3

// Zombie perks
const ZF_ALPHA_RADIUSSQ = (500 * 500);      // Radius (squared) inwhich zombies provide attack and regen bonuses.
const ZF_ALPHA_ATTACK = 5;                 // Attack bonus per nearby non-minioned zombie.   //改为 5 。或者考虑将该职业打造成僵尸方的英雄职业。
const ZF_ALPHA_ATTACK_MINION = 10;          // Attack bonus per nearby minioned zombie.   //改为 10 
const ZF_ALPHA_REGEN = 4;                   // Regen bonus per nearby non-minioned zombie.
const ZF_ALPHA_REGEN_MINION = 12;           // Regen bonus per nearby minioned zombie.
const ZF_ALPHA_SUMMON_LIMIT = 5;            // Maximum number of minions that can be summoned at once.    //改为 5 ，提高戏剧性
const ZF_ALPHA_TIMER_MINION = 15;           // Time between use of perk per zombie summoned.
const ZF_COMBUSTIBLE_DAMAGE = 120;          // Explosion damage done on death.    //改为 120 ，稍微提高威力
const ZF_COMBUSTIBLE_DAMAGE_HEAVY = 200;    // Explosion damage done on death for heavies.    //改为 200
const ZF_COMBUSTIBLE_DEFEND = -200;         // Defense penalty for using perk.
const ZF_COMBUSTIBLE_RADIUS = 300;          // Radius of explosion.   //改为 300
const Float:ZF_COMBUSTIBLE_RESPAWNTIME = 4.5; // Respawn time when using perk.
const HORRIFYING_ATTACK = -20;              // Attack penalty per hit (non-heavies).
const HORRIFYING_ATTACK_HEAVY = -30;        // Attack penalty per hit (heavies).
const HORRIFYING_DEFEND = 0;              // Defense penalty per hit (non-heavies).   //改为 0 ，这职业需要存活才能生效，但是目前很难全身而退，因此没人玩。
const HORRIFYING_DEFEND_HEAVY = 0;        // Defense penalty per hit (heavies).   //改为 0
const HORRIFYING_ROF_HEAVY = -10;           // Rate of fire penalty per hit (heavies).
const Float:HORRIFYING_PENALTYPCT_KILL = 0.75;   // Temp stat penalty reduction when killing perk user.
const Float:HORRIFYING_PENALTYPCT_ASSIST = 0.25; // Temp stat penalty reduction when assisting in killing perk user.
const HORRIFYING_DURATION = 15;             // Duration of per-hit penalties.
const HORRIFYING_DURATION_HEAVY = 30;       // Duration of per-hit penalties.
const ZF_HUNTER_ATTACK = 50;                // Attack bonus given when spawning on mark.    //改为 50 ，输出超过 50 就得有些特别离谱的代价
const ZF_HUNTER_DURATION = 10;              // Duration of spawn bonuses.
const ZF_HUNTER_RADIUSSQ = (85 * 85);       // Radius (squared) inwhich survivors will destroy a spawn mark.
const Float:ZF_HUNTER_RESPAWNTIME = 5.5;    // Respawn time when using perk.
const ZF_LEAP_COMBAT = -20;                 // Combat penalty when using perk.    //改为 -20 ，稍微提高威力
const ZF_LEAP_COOLDOWN = 4;                 // Duration after leap before ability is ready.
const Float:ZF_LEAP_FORCE = 900.0;          // Leap force.    //应该削弱，增强，还是保持现状?这个我不好说，反正scout的是能一飞冲天。
const Float:ZF_LEAP_FORCE_SCOUT = 1500.0;   // Leap force for scouts.
const ZF_MAGNETIC_RADIUSSQ = (500 * 500);   // Radius (squared) inwhich sentries are disabled.
const ZF_MARKED_ATTACK_ON_MARK = 200;       // Attack bonus when hitting marked target.   //看起来OP，但就该有这威力，建议保持
const ZF_MARKED_ATTACK_OFF_MARK = -10;      // Attack penalty when hitting non-marked target.   //改为 -10 ，避免该职业过于乏力
const ZF_MARKED_MIN_SURVIVORS = 1;          // The minimum amount of survivors that must be alive to select a mark.   //改为 1 ，避免幸存者过少时该职业无效
const ZF_MARKED_TIMER = 10;                 // Time between old target death and new target selection.    //改为 10
const ZF_RAGE_COOLDOWN = 20;                // Duration after rage ends before ability is ready.    //或者考虑将该职业打造成僵尸方的另一个英雄职业。
const ZF_RAGE_SPEED = 100;                  // Speed bonus when ability is active.
const Float:ZF_RAGE_HEALTHPCT_TOUSE = 0.80; // Percent of health above which ability can be activated.
const Float:ZF_RAGE_HEALTHPCT_ONUSE = 0.50; // Percent of health added when ability is activated.
const ZF_ROAR_COOLDOWN = 15;                // Time between use of perk.
const ZF_ROAR_DURATION = 20;                // Base duration of DAZED condition (non-heavies).
const ZF_ROAR_DURATION_HEAVY = 60;          // Base duration of DAZED condition (heavies).
const Float:ZF_ROAR_FORCE = 1200.0;          // Knockback force for nearby survivors (non-heavies).    //改为 1200 ，提高击退力
const Float:ZF_ROAR_FORCE_HEAVY = 3000.0;   // Knockback force for nearby survivors (heavies).    //改为 2000 ，提高击退力
const ZF_ROAR_RADIUS = 450;                 // Radius inwhich knockback and defense penalties are applied.
const ZF_SCORCHING_ATTACK = -50;            // Attack penalty when using perk.
const ZF_SCORCHING_SPEED = 50;              // Speed bonus when using perk.
const ZF_SICK_MAX_ITEMS = 5;                // Maximum number of acid projectiles / puddles active at once.
const ZF_SICK_DEFEND = -75;                 // Defense penalty when using perk.   //改为 -75 ，定位类似L4D2的Spitter，脆弱但是危险
const ZF_SICK_DAMAGE = 15;                   // Base acid damage done to nearby survivors.   //改为 15
const ZF_SICK_DAMAGE_RADIUS = 150;           // Radius inwhich acid causes damamge.   //改为 150
const ZF_SICK_TIMER = 15;                   // Time between use of perk.
const ZF_SWARMING_COMBAT = -20;             // Combat penalty when using perk.    //改为 -20 ，稍微提高威力
const ZF_SWARMING_RADIUSSQ = (400 * 400);   // Radius (squared) inwhich nearby zombies, when killed, get instant respawn.   //改为 400 * 400 ，稍微提高这个手动速生的威力
const ZF_SWARMING_SPEED = 50;              // Speed bonus when using perk.   //改为 50 ，这个速度buff和职业关联性不大
const Float:ZF_SWARMING_RESPAWNTIME = 0.5;  // Respawn time when using perk.
const ZF_TARRED_MAX_ITEMS = 5;              // Maximum number of oil projectiles / slicks active at once.
const ZF_TARRED_DURATION_MELEE = 10;        // Duration of slowing penalties for melee hits.
const ZF_TARRED_DURATION_SLICK = 30;        // Duration of slowing penalties for oil slick hits.
const ZF_TARRED_ROF = -20;                  // Rate of fire penalty on hit / when hit.
const ZF_TARRED_SPEED_MELEE = -40;          // Speed penalty on hit / when hit.   //改为 20 ，目前该职业的焦油毫无用处，减速全靠输出
const ZF_TARRED_SPEED_SLICK = -30;           // Speed penalty on oil slick hit.    //改为 50
const ZF_TARRED_SPEED_LIMIT = -100;         // Speed penalty threshold below which no further speed penalties are granted.
const ZF_TARRED_TIMER = 30;                 // Time between use of perk.
const ZF_TARRED_RADIUS = 75;                // Radius inwhich oil causes slowing effects.
const Float:ZF_THIEVING_AMMOPCT = 0.30;     // Percent of current ammo reserve stolen per hit.
const ZF_THIEVING_ATTACK = -66;             // Attack penalty when using perk.
const ZF_THIEVING_METAL = 100;              // Metal stolen per hit.
const Float:ZF_THIEVING_UBERPCT = 0.50;     // Percent of ubercharge stolen per hit.
const ZF_TOXIC_ATTACK = -90;               // Attack penalty when using perk.    //改为 -90 ，去掉所有伤害是不是有点极端了?
const ZF_TOXIC_DURATION_POISON = 10;        // Time each regen penalty lasts.   //改为 10 ，在提高总输出的前提下稍微降低时间
const ZF_TOXIC_DAMAGE_PASSIVE = 5;          // Damage per/s for survivors nearby stationary zombie.   //改为 5
const ZF_TOXIC_RADIUSSQ = (400 * 400);      // Radius (squared) inwhich toxic passive poison is applied.
const Float:ZF_VAMPIRIC_HEALTHPCT = 1.00;   // Percent of damage leeched when hit.    //改为 1 ，“所伤即所得”
const ZF_VAMPIRIC_REGEN = 15;               // Regeneration bonus when using perk.    //改为 15 ，让该职业可以快速恢复。或者考虑让攻击回血进一步增强，但是会缓慢掉血?这才叫吸血鬼。
const ZF_VINDICTIVE_ATTACK = 20;            // Attack bonus per kill.   
const ZF_VINDICTIVE_ATTACK_ASSIST = 10;      // Attack bonus per assist.   //改为 10 ，这职业很难滚起雪球
const ZF_VINDICTIVE_DEFEND = 10;             // Defense bonus per kill.    //改为 10
const ZF_VINDICTIVE_DEFEND_ASSIST = 5;      // Defense bonus per assist.    //改为 5

//
// Perk Stats
//
#define TOTAL_ZFSTATS 5
#define TOTAL_ZFSTAT_TYPES 4
#define TOTAL_ZFCONDS 4
enum ZFStat { 
  ZFStatAtt = 0,    // 攻击力
  ZFStatCrit,       // 暴击率
  ZFStatDef,        // 防御力
  ZFStatRof,        // 攻击速度
  ZFStatSpeed,      // 移动速度
  ZFStat_Count      // 总数
}
enum ZFStatType { 
  ZFStatTypePerm = 0,        // 永久
  ZFStatTypeCond,            // 条件
  ZFStatTypeTemp,            // 临时
  ZFStatTypeTempDuration     // 临时持续时间
}
enum ZFCond { 
  ZFCondCrippled = 0, // 残废
  ZFCondIntimidated,  // 恐吓
  ZFCondPoisoned,     // 中毒
  ZFCondTracked       // 追踪
}

#define MAX_ITEMS 5
