#pragma newdecls required

#if defined _ZF_PERK_INC
  #endinput
#endif
#define _ZF_PERK_INC

#include "zf_util_base.inc"
#include "zf_util_fx.inc"
#include "zf_util_pref.inc"

#include "perks/perk_structs.inc"
#include "perks/base_perk.inc"
#include "perks/sur_athletic.inc"
#include "perks/zom_alpha.inc"

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

new String:ZF_SURPERK_NAME[TOTAL_SUR_PERKS][] = 
{ "未选择",
  "运动员", "木工", "慈善家", "懦夫", "伙计", "英雄", "牧师",
  "主宰者", "领袖", "忍者", "治安官", "计划通", "利他主义者",
  "仓鼠", "狂热者", "供应商", "暴脾气", "陷阱大师", "肉盾",
  "智者", "禅师" };

new String:ZF_SURPERK_SHORTDESC[TOTAL_SUR_PERKS][] =
{  
  "未选择",
  "运动员(Athletic)——高移动力",
  "木工(Carpenter)——建造障碍物",
  "慈善家(Charitable)——击杀换取礼物",
  "懦夫(Cowardly)——随时准备逃跑",
  "伙计(Friend)——和伙伴获得加成",
  "英雄(Heroic)——活到最后获得暴击",
  "牧师(Holy)——蹲着治疗幸存者",
  "主宰者(Juggernaut)——攻击力高,速度慢",
  "领袖(Leader)——放置增益旗帜",
  "忍者(Ninja)——放置撤退用诱饵",
  "治安官(Nonlethal)——低威力子弹,击退僵尸",
  "/BUG/计划通(Resourceful)——和补给品做朋友",
  "利他主义者(Selfless)——拉僵尸垫背",
  "/BUG/仓鼠(Stash)——放置藏身处",
  "狂热者(Stir-Crazy)——速度决定攻击力",
  "/BUG/供应商(Supplier)——放置弹药补给箱",
  "暴脾气(Tantrum)——短时间内获得暴击",
  "陷阱大师(Trapper)——放置地雷",
  "肉盾(Turtle)——防御力高,速度慢",
  "智者(Wise)——击杀提高属性",
  "禅师(Zenlike)——蹲着增加暴击率"
  };
//"0xxxxxxxxxxxxxxx1xxxxxxxxxxxxxxx2xxxxxxxxxxxxxxx\n3xxxxxxxxxxxxxxx4xxxxxxxxxxxxxxx5xxxxxxxxxxxxxxx\n6xxxxxxxxxxxxxxx7xxxxxxxxxxxxxxx8xxxxxxxxxxxxxxx\n9xxxxxxxxxxxxxxxAxxxxxxxxxxxxxxxBxxxxxxxxxxxxxxx\nCxxxxxxxxxxxxxxxDxxxxxxxxxxxxxxxExxxxxxxxxxxxxxx\nFxxxxxxxxxxxxxxxGxxxxxxxxxxxxxxxHxxxxxxxxxxxxxxx\nIxxxxxxxxxxxxxxxJxxxxxxxxxxxxxxxKxxxxxxxxxxxxxxx\nLxxxxxxxxxxxxxxxMxxxxxxxxxxxxxxxNxxxxxxxxxxxxxxx\n",
new String:ZF_SURPERK_LONGDESC[TOTAL_SUR_PERKS][] = 
{
  "",
  "你的移动力和攻击速度大幅增加. \n但是你的攻击力降低,而且无法造成随机暴击. \n \n“嘿!你们根本追不上我!” \n推荐职业: 火焰兵、爆破骑士、医生",
  "你有防御力加成,但是你的攻击力减弱. \n发医生语音来建造一个500点生命值的路障,敌我均可破坏. \n冷却时间25秒.同时最多拥有4个路障. \n \n“此路不通。” \n推荐职业: 火焰兵、工程师、医生",
  "你的每个击杀和助攻都会增加礼物点数. \n发医生语音来消耗礼物点数,放出礼物,可被其他的幸存者捡起. \n礼物能给予生命值回复和短暂的攻击力加成. \n \n“人人都有礼物拿!” \n推荐职业: 士兵、爆破手、狙击手",  
  "当你被攻击时,自动激活你的被动能力“恐慌”.  \n恐慌会给你防御力和速度加成. \n恐慌能力持续时间5秒,冷却时间30秒. \n \n“别守着了,快逃命吧!” \n推荐职业: 工程师、医生、狙击手",  
  "在准备时间内,对着目标发医生语音,可将目标设为伙伴.(未选择则由系统产生) \n靠近同伴时,你获得攻击力和生命回复加成. \n当你和同伴击杀或助攻时,你的暴击时间就会增加. \n当同伴死亡后,你就会获得对应时间的暴击.\n \n“两人搭配,干活不累!” \n推荐职业: 任何",
  "你有攻击力和防御力加成. \n你的每个击杀和助攻都会增加你的暴击时间. \n当你是最后一个幸存者时,你就会获得对应时间的暴击. \n \n“晚安,好运.” \n推荐职业: 士兵、火焰兵、爆破手", 
  "你的攻击力降低. \n当蹲下不动时,你可以治疗自己和周围的幸存者. \n \n“圣光赐予我胜利!” \n推荐职业: 缺少医生的队伍",
  "你的攻击力大幅提高,但你的防御力和移动速度降低. \n你免疫掉落伤害. \n如果你落在僵尸附近,僵尸会受到轻微的伤害,并产生击退和击晕效果. \n \n“(译者不会玩MOAB游戏)” \n推荐职业: 士兵、爆破手、狙击手",
  "你有暴击加成,也会给附近的幸存者提供攻击力和防御力加成. \n发医生语音来放置一个旗帜,靠近你的旗帜的幸存者都会获得攻击力和防御力加成. \n旗帜持续90秒,冷却时间150秒. \n \n“我需要重新集结队伍.” \n推荐职业: 士兵、爆破手、工程师",
  "你的移动力大幅增加,但是你的攻击力降低. \n发医生语言来放置一个撤退点.当你受到攻击时, 你会被传送到撤退点. \n撤退点持续15秒,撤退点冷却时间30秒. \n \n“虽然没有飞镖,但是我会这个!” \n推荐职业: 爆破骑士、工程师、医生", 
  "使用子弹类武器时,你的攻击力降低,但能造成击退效果. \n \n“离我远点!” \n推荐职业: 工程师、狙击手",
  "/该职业有漏洞,请勿游玩/ \n你的每个击杀都会为你补充子弹、生命值和金属. \n弹药包会给你临时的攻击力加成,医疗包则是防御力加成.",
  "你死后会爆炸,造成成吨的伤害. \n \n“幸存者们,这是我最后的波纹了,收下吧!” \n推荐职业: 任何",
  "/该职业有漏洞,请勿游玩/ \n你可以通过蹲下来发医生语音来放置一个藏身处. \n藏身处需要远离其他幸存者或藏身处.\n藏身处会为你补充弹药、生命,并提供临时加成. \n藏身处持续时间30秒,冷却时间30秒.",
  "你的移动速度会给你提供攻击力加成,速度越快,攻击力就越高. \n \n“这个职业曾经能获得200%的攻击力加成.” \n推荐职业: 火焰兵、爆破骑士",
  "/该职业有漏洞,请勿游玩/ \n你的弹药会周期性的补充,并且拥有双倍备弹量. \n发医生语音来放置一个补给箱,其他幸存者可以从中补给弹药. \n冷却时间10秒.",
  "发医生语音来激活愤怒. \n愤怒给予你15秒的暴击,结束后进入30秒的疲惫状态. \n疲惫状态下,移动速度大幅降低. \n \n“我想这个职业的确很解压.” \n推荐职业: 士兵、火焰兵、爆破手",
  "发医生语音来放置地雷,僵尸碰到后会自动爆炸,同时最多拥有5个地雷. \n冷却时间20秒. \n \n“EDD Mounted, let them come.” \n推荐职业: 任何", 
  "你的防御力大幅增加,但你的攻击力降低,而且无法造成随机暴击. \nSpy无法背刺你. \n \n“你们僵尸就这点能耐?” \n推荐职业: 士兵、火焰兵、爆破骑士",
  "你的每个击杀和助攻都能使你的攻击力永久增加. \n每当你被近战武器攻击后,你的防御力就会永久增加. \n \n“从战斗中学习.” \n推荐职业: 任何",
  "当蹲下不动时,你的生命值会慢慢恢复,并不断增加你的暴击率. \n每攻击一次,暴击几率都会降低25%. \n(注意:Engi用扳手敲打建筑也会降低暴击率.) \n \n“出来吧,百分百暴击火箭!” \n推荐职业: 士兵"
};

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

new String:ZF_ZOMPERK_NAME[TOTAL_ZOM_PERKS][] =
{ "未选择",
  "零号僵尸", "自爆僵尸", "惊吓僵尸", "猎手僵尸", "飞跃僵尸", "磁化僵尸", "标记僵尸",
  "狂怒僵尸", "咆哮僵尸", "火焰僵尸", "吐酸僵尸", "招魂僵尸", "吐油僵尸", "潜行僵尸",
  "剧毒僵尸", "吸血僵尸", "复仇僵尸"};

new String:ZF_ZOMPERK_SHORTDESC[TOTAL_ZOM_PERKS][] =
{
  "未选择",
  "零号僵尸(Alpha)——召唤僵尸随从",
  "自爆僵尸(Combustible)——尸如其名",
  "惊吓僵尸(Horrifying)——攻击削弱人类",
  "猎手僵尸(Hunter)——手动放置重生点",
  "飞跃僵尸(Leap)——大跳飞向空中",
  "磁化僵尸(Magnetic)——瘫痪附近建筑",
  "标记僵尸(Marked)——瞄准特定目标",
  "狂怒僵尸(Rage)——短时间加强属性",
  "咆哮僵尸(Roar)——咆哮击退幸存者",
  "火焰僵尸(Scorching)——烧死他们!",
  "吐酸僵尸(Sick)——吐出有害的酸液",
  "招魂僵尸(Swarming)——快速复活僵尸",
  "吐油僵尸(Tarred)——吐出减速的焦油",
  "潜行僵尸(Thieving)——偷走人类的弹药和武器",
  "剧毒僵尸(Toxic)——攻击附带剧毒",
  "吸血僵尸(Vampiric)——攻击大量回血",
  "复仇僵尸(Vindictive)——击杀提高属性"
};
//"0xxxxxxxxxxxxxxx1xxxxxxxxxxxxxxx2xxxxxxxxxxxxxxx\n3xxxxxxxxxxxxxxx4xxxxxxxxxxxxxxx5xxxxxxxxxxxxxxx\n6xxxxxxxxxxxxxxx7xxxxxxxxxxxxxxx8xxxxxxxxxxxxxxx\n9xxxxxxxxxxxxxxxAxxxxxxxxxxxxxxxBxxxxxxxxxxxxxxx\nCxxxxxxxxxxxxxxxDxxxxxxxxxxxxxxxExxxxxxxxxxxxxxx\nFxxxxxxxxxxxxxxxGxxxxxxxxxxxxxxxHxxxxxxxxxxxxxxx\nIxxxxxxxxxxxxxxxJxxxxxxxxxxxxxxxKxxxxxxxxxxxxxxx\nLxxxxxxxxxxxxxxxMxxxxxxxxxxxxxxxNxxxxxxxxxxxxxxx\n",
new String:ZF_ZOMPERK_LONGDESC[TOTAL_ZOM_PERKS][] = 
{
  "",  
  "你能通过击杀幸存者或为随从助攻,使得死去的人类成为自己的随从. \n附近的每个僵尸和随从都能让你获得生命恢复和攻击加成. \n发医生语音可以召唤最多5个随从到身边,冷却时间15秒. \n \n“来自黑暗寒冬的仆人们、士兵们!听从我的召唤!” \n推荐职业: 侦察兵、机枪手",
  "你的防御力大幅降低. \n被远程武器击杀后,你会爆炸并造成伤害. \n你不可以使用隐身手表或者原子能饮料. \n(远程武器包括一切非近战武器) \n \n*将Boomer先推开再攻击* \n推荐职业: 侦察兵、机枪手",
  "你的攻击力降低,但你的攻击能降低幸存者的攻击力、防御力和攻击速度. \n减益效果持续15秒.你死亡后,这个效果也随即消失. \n \n“敲骨吸髓.” \n推荐职业: 机枪手、间谍",
  "发医生语音来放置你的重生点. \n从自己的重生点重生时,你的重生时间较短,并获得临时的攻击加成. \n每次重生后,你只能放置一次重生点. \n注意!幸存者可以摧毁你的重生点. \n \n“你将成为我的猎物!” \n推荐职业: 任何",
  "你的攻击力与防御力降低,但不受坠落伤害. \n发医生语音来施展大跳,冷却时间4秒. \n \n“起飞!” \n推荐职业: 侦察兵、间谍",
  "你能使附近的步哨和地雷失效. \n \n“I will murder your toys as well.” \n推荐职业: 任何",
  "系统会随机选择一名幸存者作为你的目标.\n你对目标能造成极高伤害,但是对其他人造成较低伤害. \n当前目标死亡后,若剩余的幸存者超过1个,10秒后将自动选择一个新目标. \n \n“目标已经标记出来了!” \n推荐职业: 侦察兵、机枪手",
  "发医生语音来激活愤怒. \n愤怒使你获得150%的生命和速度加成. \n生命低于80%后,已有的愤怒会消失,也无法激活愤怒. \n冷却时间20秒. \n \n“Taaaaaaaaaank!” \n推荐职业: 机枪手",
  "发医生语音来激活咆哮. \n咆哮造成击退效果并暂时降低幸存者防御力,冷却时间15秒. \n \n“哈!” \n推荐职业: 任何",
  "你的攻击力降低,但获得速度加成,并免疫火焰伤害. \n幸存者撞到你或被你近战击中时会着火. \n你不可以使用原子能饮料. \n \n“孙哥我火了!” \n推荐职业: 侦察兵",
  "你的防御力大幅降低,但可以发医生语音来吐出酸液. \n酸液会持续35秒或直到你死亡. \n酸液造成的伤害与你和酸液之间的距离成正比. \n \n*古怪的嚎叫声* \n推荐职业: 侦察兵",
  "你有移速加成,但你的攻击和防御力降低. \n你能使自己与身边的队友快速重生. \n \n“嘿!速生不是投票关掉了吗?” \n推荐职业: 侦察兵",
  "发医生语音来吐出焦油. \n焦油能降低幸存者的移动速度与攻击速度,持续30秒或直到你死亡. \n你的近战攻击附带焦油效果. \n \n*古怪的嚎叫声* \n推荐职业: 侦察兵" ,
  "你的攻击力降低,但你的近战攻击可以偷取幸存者的弹药、金属与uber. \n如果对方的主武器没有后备弹药,你就可以偷走并使用自己的有部分弹药的主武器. \n(请确保自己装备的主武器在白名单内,不然无法获得.) \n \n“这可比拳头好使多了!” \n推荐职业: 任何",
  "直接攻击时,你的攻击力大幅降低. \n但当你近战击中幸存者或被近战攻击时,目标将会中毒,受到剧毒伤害,持续12秒. \n当你保持不动时, 能对附近的幸存者持续造成伤害. \n \n“记住这个职业:Toxic.” \n推荐职业: 侦察兵、间谍",
  "你有生命值回复加成. \n你的攻击附带吸血效果. \n \n“不靠近你,怎么把你给揍扁呢.” \n推荐职业: 侦察兵、机枪手",
  "你的击杀与助攻能使你获得永久性的攻击力和防御力加成. \n \n“我想这是死去的智者.” \n推荐职业: 任何"
};
  
//
// Perk Bonus Objects
//

//
// Perk Gameplay Objects //建议属性调整 by Spectator 2023.7.2
//我的看法：buff类职业要有明确的使用场景，数值要有强有弱，避免op职业；功能类职业要能产生明显的影响，并且不妨碍正常游玩。
//如果你按这个想法来修改，那就改完了再发给我，我来修改文本(zf_perk.inc和zombie_fortress_perk.sp)。
//另外，可以考虑打造人类方与僵尸方的“英雄职业”，双方只能有一个，但是数值可以OP一点。
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
// Perk Vars
//

// State
new zf_frameCounter;
new zf_surPerksEnabled;
new zf_zomPerksEnabled;
new zf_surPerksLimit[TOTAL_SUR_PERKS];
new zf_zomPerksLimit[TOTAL_ZOM_PERKS];
new zf_perkMode;
new zf_perkPendingMode;
new zf_perkRandSurPerk;
new zf_perkRandZomPerk;
new zf_perkTeamSurPerk;
new zf_perkTeamZomPerk;
new zf_menuPerk[MAXPLAYERS+1];

// Logic
new zf_lastAttack[MAXPLAYERS+1];
new zf_lastButtons[MAXPLAYERS+1];
new zf_lastHealth[MAXPLAYERS+1];
new zf_lastKiller[MAXPLAYERS+1];
new zf_lastPoison[MAXPLAYERS+1];
new zf_lastTeam[MAXPLAYERS+1];

new zf_perkTimer[MAXPLAYERS+1];           // Timer shared by many perks
new zf_perkState[MAXPLAYERS+1];           // State shared by many perks
new Float:zf_perkPos[MAXPLAYERS+1][5][3]; // Position array shared by many perks
new String:zf_perkStr[MAXPLAYERS+1][32];  // String shared by many perks

new zf_perkAlphaMaster[MAXPLAYERS+1];     // Perk-specific var. Didn't want to use it, but had no choice.
new BasePerk:g_hPerks[MAXPLAYERS+1];

// Stats
#define TOTAL_ZFSTATS 5
#define TOTAL_ZFSTAT_TYPES 4
#define TOTAL_ZFCONDS 4
enum ZFStat { 
  ZFStatAtt = 0,    // TODO Desc
  ZFStatCrit,       // TODO Desc
  ZFStatDef,        // TODO Desc
  ZFStatRof,        // TODO Desc
  ZFStatSpeed       // TODO Desc
}
enum ZFStatType { 
  ZFStatTypePerm = 0,        // TODO Desc
  ZFStatTypeCond,            // TODO Desc
  ZFStatTypeTemp,            // TODO Desc
  ZFStatTypeTempDuration     // TODO Desc
}
enum ZFCond { 
  ZFCondCrippled = 0, // TODO Desc
  ZFCondIntimidated,  // TODO Desc
  ZFCondPoisoned,     // TODO Desc
  ZFCondTracked       // TODO Desc
}
new zf_stat[MAXPLAYERS+1][ZFStat][ZFStatType];
new zf_cond[MAXPLAYERS+1][ZFCond];

// FX.Entities
new zf_aura[MAXPLAYERS+1];

#define ICON_SPR 0
#define ICON_ANC 1
new zf_icon[MAXPLAYERS+1][2];

#define MAX_ITEMS 5
new zf_item[MAXPLAYERS+1][MAX_ITEMS];

// FX.HUD
new Handle:zf_hudLine0;
new Handle:zf_hudLine1;
new Handle:zf_hudLine2;

// Menus
new Handle:zf_menuSurPerkList;
new Handle:zf_menuZomPerkList;

// CVARS
new Handle:zf_cvCripple;

////////////////////////////////////////////////////////////
//
// Perk Init
//
////////////////////////////////////////////////////////////
public perkInit()
{  
  // Initialize game state
  zf_frameCounter = 0;
  zf_surPerksEnabled = 0xFFFF_FFFF;
  zf_zomPerksEnabled = 0xFFFF_FFFF;
  for(new i = 0; i < TOTAL_SUR_PERKS; i++)
    zf_surPerksLimit[i] = -1;
  for(new i = 0; i < TOTAL_ZOM_PERKS; i++)
    zf_zomPerksLimit[i] = -1;
  zf_perkMode = 0;
  zf_perkPendingMode = 0;
  zf_perkRandSurPerk = ZF_PERK_NONE;
  zf_perkRandZomPerk = ZF_PERK_NONE;  
  zf_perkTeamSurPerk = ZF_PERK_NONE;
  zf_perkTeamZomPerk = ZF_PERK_NONE;

  // Initialize client perk state
  resetAllClients();
  
  // Initialize HUD synchronizers
  zf_hudLine0 = CreateHudSynchronizer();
  zf_hudLine1 = CreateHudSynchronizer();
  zf_hudLine2 = CreateHudSynchronizer();
  
  // Initialize menu handles
  zf_menuSurPerkList = perk_buildSurPerkListMenu();
  zf_menuZomPerkList = perk_buildZomPerkListMenu();
    
  // Admin Commands
  // [0|normal|1|randplayer|2|randteam|3|cvarteam]
  RegAdminCmd("sm_zf_perk_setmode", command_zfPerkSetMode, ADMFLAG_GENERIC, "Sets ZF perk mode. 0 = Normal, 1 = Random per player, 2 = Random per team, 3 = CVAR per team.");
  // [<surperk>]
  RegAdminCmd("sm_zf_perk_setteamsurperk", command_zfPerkSetTeamSurPerk, ADMFLAG_GENERIC, "Sets survivor perk for CVAR per team mode.");
  // [<zomperk>]
  RegAdminCmd("sm_zf_perk_setteamzomperk", command_zfPerkSetTeamZomPerk, ADMFLAG_GENERIC, "Sets zombie perk for CVAR per team mode.");
  // [all|allsur|allzom]
  RegAdminCmd("sm_zf_perk_list", command_zfPerkList, ADMFLAG_GENERIC, "Lists current perks and status.");
  // [all|allsur|allzom|<perk>]
  RegAdminCmd("sm_zf_perk_enable", command_zfPerkEnable, ADMFLAG_GENERIC, "Enables specified perk. Changes apply on new round.");
  // [all|allsur|allzom|<perk>]
  RegAdminCmd("sm_zf_perk_disable", command_zfPerkDisable, ADMFLAG_GENERIC, "Disables specified perk. Changes apply on new round.");
  // [all|allsur|allzom|<perk>] [<limit>]
  RegAdminCmd("sm_zf_perk_limit", command_zfPerkLimit, ADMFLAG_GENERIC, "Sets limit for specified perk. -1 = Unlimited, 0 = None, >0 = Limit. Changes fully apply on new round.");
  
  // Client Commands
  // [<perk>]
  AddCommandListener(hook_zfSelectPerk, "zf_perk_select");
  
  // Register CVARS
  zf_cvCripple = CreateConVar("sm_zf_cripple", "0", "0 = Crippling backstab disabled, 1 = Crippling backstab enabled.", FCVAR_REPLICATED|FCVAR_NOTIFY, true, 0.0, true, 1.0); 
}

////////////////////////////////////////////////////////////
//
// Admin Command Handlers
//
///////////////////////////////////////////////////////////
public Action:command_zfPerkSetMode(client, args)
{
  decl String:cmd[32];
  
  if(args == 0)
  {
    //
    // Display current mode.
    //
    ReplyToCommand(client, "Current game mode: %d", zf_perkMode);
    return Plugin_Handled;
  }  
  else if(args == 1)
  {
    //
    // Set game mode for next round.
    //
    GetCmdArg(1, cmd, sizeof(cmd));
    if(StrEqual(cmd, "0", false) || StrEqual(cmd, "normal", false))
    {
      zf_perkPendingMode = 0;
      ReplyToCommand(client, "Game mode set to 0 (Normal). Changes apply on new round.");
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "1", false) || StrEqual(cmd, "randplayer", false))
    {
      zf_perkPendingMode = 1;
      ReplyToCommand(client, "Game mode set to 1 (Random perks per player). Changes apply on new round.");
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "2", false) || StrEqual(cmd, "randteam", false))
    {
      zf_perkPendingMode = 2;
      ReplyToCommand(client, "Game mode set to 2 (Random perks per team). Changes apply on new round.");
      return Plugin_Handled;
    } 
    if(StrEqual(cmd, "3", false) || StrEqual(cmd, "cvarteam", false))
    {
      zf_perkPendingMode = 3;
      ReplyToCommand(client, "Game mode set to 3 (CVAR perks per team). Changes apply on new round.");
      return Plugin_Handled;
    }      
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [0|normal|1|randplayer|2|randteam|3|cvarteam]", cmd);    
  return Plugin_Handled;
}

public Action:command_zfPerkSetTeamSurPerk(client, args)
{
  decl String:cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(new i = 0; i < TOTAL_SUR_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_SURPERK_NAME[i], false))
      {
        zf_perkTeamSurPerk = i;
        ReplyToCommand(client, "Selected %s as survivor perk for CVAR per team mode.", ZF_SURPERK_NAME[i]);
        return Plugin_Handled;
      }
    }  
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [<surperk>]", cmd);    
  return Plugin_Handled;  
}

public Action:command_zfPerkSetTeamZomPerk(client, args)
{
  decl String:cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(new i = 0; i < TOTAL_ZOM_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_ZOMPERK_NAME[i], false))
      {
        zf_perkTeamZomPerk = i;
        ReplyToCommand(client, "Selected %s as zombie perk for CVAR per team mode.", ZF_ZOMPERK_NAME[i]);
        return Plugin_Handled;
      }
    }  
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [<zomperk>]", cmd);    
  return Plugin_Handled;  
}

public Action:command_zfPerkList(client, args)
{
  decl String:cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    new bool:listSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    new bool:listZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    if(listSur)
    {
      ReplyToCommand(client, "Survivor Perks (enabled unless specified):");
      for(new i = 1; i < TOTAL_SUR_PERKS; i++)
        if(surPerkEnabled(i))
          ReplyToCommand(client, " > %s (Limit: %d)", ZF_SURPERK_NAME[i], zf_surPerksLimit[i]);
        else
          ReplyToCommand(client, " [Disabled] > %s (Limit: %d)", ZF_SURPERK_NAME[i], zf_surPerksLimit[i]);
    } 
    if(listZom)
    {
      ReplyToCommand(client, "Zombie Perks (enabled unless specified):");
      for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
        if(zomPerkEnabled(i))
          ReplyToCommand(client, " > %s (Limit: %d)", ZF_ZOMPERK_NAME[i], zf_zomPerksLimit[i]);
        else
          ReplyToCommand(client, " [Disabled] > %s (Limit: %d)", ZF_ZOMPERK_NAME[i], zf_zomPerksLimit[i]);
    }
    
    if(listSur | listZom)
      return Plugin_Handled;
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [all|allsur|allzom]", cmd);    
  return Plugin_Handled;
}

public Action:command_zfPerkEnable(client, args)
{
  command_perkUpdate(client, args, true);
  return Plugin_Handled;
}

public Action:command_zfPerkDisable(client, args)
{
  command_perkUpdate(client, args, false);
  return Plugin_Handled;
}

public Action:command_perkUpdate(client, args, bool:doEnable)
{
  decl String:cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));

    new bool:setSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    new bool:setZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    decl String:strState[16];
    strState = doEnable ? "Enabled" : "Disabled";
    
    //
    // Enable/Disable groups of perks.
    //
    if(setSur)
    {
      zf_surPerksEnabled = doEnable ? 0xFFFF_FFFF : 0x0000_0001;
      ReplyToCommand(client, "%s all survivor perks. Changes apply on new round.", strState);      
    }    
    if(setZom)
    {
      zf_zomPerksEnabled = doEnable ? 0xFFFF_FFFF : 0x0000_0001;
      ReplyToCommand(client, "%s all zombie perks. Changes apply on new round.", strState);        
    }
    if(setSur || setZom)
    {
      return Plugin_Handled;
    }

    //
    // Enable/Disable single perk.
    //
    for(new i = 1; i < TOTAL_SUR_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_SURPERK_NAME[i], false))
      {
        zf_surPerksEnabled = doEnable ? (zf_surPerksEnabled | (1 << i)) : (zf_surPerksEnabled & ~(1 << i));
        ReplyToCommand(client, "%s %s. Changes apply on new round.", strState, ZF_SURPERK_NAME[i]);
        return Plugin_Handled;
      }
    }
    for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_ZOMPERK_NAME[i], false))
      {
        zf_zomPerksEnabled = doEnable ? (zf_zomPerksEnabled | (1 << i)) : (zf_zomPerksEnabled & ~(1 << i));
        ReplyToCommand(client, "%s %s. Changes apply on new round.", strState, ZF_ZOMPERK_NAME[i]);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "\"%s\" is not a valid perk!", cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [all|allsur|allzom|<perk>]", cmd);    
  return Plugin_Handled;  
}

public Action:command_zfPerkLimit(client, args)
{
  decl String:cmd[32];
  decl String:cmd2[32];
  
  if(args == 2)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    GetCmdArg(2, cmd2, sizeof(cmd2));

    new bool:setSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    new bool:setZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    new limit = StringToInt(cmd2);
        
    //
    // Limit groups of perks.
    //
    if(setSur)
    {
      for(new i = 1; i < TOTAL_SUR_PERKS; i++)
        zf_surPerksLimit[i] = limit;
      ReplyToCommand(client, "Survivor perk limits set to %d. Changes fully apply on new round.", limit);      
    }    
    if(setZom)
    {
      for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
        zf_zomPerksLimit[i] = limit;
      ReplyToCommand(client, "Zombie perk limits set to %d. Changes fully apply on new round.", limit);      
    }
    if(setSur || setZom)
    {
      return Plugin_Handled;
    }

    //
    // Limit single perk.
    //
    for(new i = 1; i < TOTAL_SUR_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_SURPERK_NAME[i], false))
      {
        zf_surPerksLimit[i] = limit;
        ReplyToCommand(client, "%s perk limit set to %d. Changes fully apply on new round.", ZF_SURPERK_NAME[i], limit);
        return Plugin_Handled;
      }
    }
    for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_ZOMPERK_NAME[i], false))
      {
        zf_zomPerksLimit[i] = limit;
        ReplyToCommand(client, "%s perk limit set to %d. Changes fully apply on new round.", ZF_ZOMPERK_NAME[i], limit);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "\"%s\" is not a valid perk!", cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [all|allsur|allzom|<perk>] [<limit>]", cmd);    
  return Plugin_Handled;    
}

////////////////////////////////////////////////////////////
//
// Client Command Handlers
//
///////////////////////////////////////////////////////////
public Action:hook_zfSelectPerk(client, const String:command[], argc)
{
  decl String:cmd[32];

  //
  // Select a single survivor/zombie perk.
  //
  if(argc == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(new i = 1; i < TOTAL_SUR_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_SURPERK_NAME[i], false))
      {
        selectSurPerk(client, i);       
        return Plugin_Handled;
      }
    }
    
    for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
    {
      if(StrEqual(cmd, ZF_ZOMPERK_NAME[i], false))
      {
        selectZomPerk(client, i);       
        return Plugin_Handled;      
      }
    }
    
    if(StrEqual(cmd, ZF_SURPERK_NAME[0], false) || StrEqual(cmd, ZF_ZOMPERK_NAME[0], false))
    {
      selectSurPerk(client, 0);
      selectZomPerk(client, 0);
      return Plugin_Handled;
    }
  }
  
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "Usage: %s [<perk>]", cmd);    
  return Plugin_Handled;
}

////////////////////////////////////////////////////////////
//
// Perk Stat Routines
//
////////////////////////////////////////////////////////////
stock getStat(client, ZFStat:stat)
{ return zf_stat[client][stat][ZFStatTypePerm] + zf_stat[client][stat][ZFStatTypeCond] + zf_stat[client][stat][ZFStatTypeTemp]; }

stock getStatType(client, ZFStat:stat, ZFStatType:type)
{ return zf_stat[client][stat][type]; }

stock addStat(client, ZFStat:stat, ZFStatType:type, val)
{ zf_stat[client][stat][type] += val; }

// Temporary setters
stock addStatTempStack(client, ZFStat:stat, newStr, newDur)
{ 
  // Note on result:
  // + Strength is averaged across maximum duration (stacks).
  // Note on use:
  // + newStr can be negative or positive.
  // + newDur must be positive.
  if((newStr != 0) && (newDur > 0))
  {
    new oldStr = zf_stat[client][stat][ZFStatTypeTemp];
    new oldDur = zf_stat[client][stat][ZFStatTypeTempDuration]; 
    new total = (oldStr * oldDur) + (newStr * newDur);
    new finalDur = max(oldDur, newDur);
    new finalStr = RoundToCeil(total / float(finalDur));
    
    // Set new temp stats
    zf_stat[client][stat][ZFStatTypeTemp] = finalStr;
    zf_stat[client][stat][ZFStatTypeTempDuration] = finalDur;
  }
  else
  {
    LogError("[ZF] - addStatTempStack() - Invalid newStr (%d) or newDur (%d)", newStr, newDur);
  }
}
stock addStatTempExtend(client, ZFStat:stat, newStr, newDur)
{
  // Note on result:
  // + Strength is averaged across sum of durations (extends).  
  // Note on use:
  // + newStr must be positive.
  // + newDur must be positive.
  if((newStr > 0) && (newDur > 0))
  {
    new oldStr = zf_stat[client][stat][ZFStatTypeTemp];
    new oldDur = zf_stat[client][stat][ZFStatTypeTempDuration]; 
    new total = (oldStr * oldDur) + (newStr * newDur);    
    new finalDur = oldDur + newDur;
    new finalStr = RoundToCeil(total / float(finalDur));
    
    // Set new temp stats
    zf_stat[client][stat][ZFStatTypeTemp] = finalStr;
    zf_stat[client][stat][ZFStatTypeTempDuration] = finalDur;    
  }
  else
  {
    LogError("[ZF] - addStatTempExtend() - Invalid newStr (%d) or newDur (%d)", newStr, newDur);  
  }
}

stock scaleStatTempPct(client, ZFStat:stat, Float:strPct, Float:durPct = 1.0)
{
  zf_stat[client][stat][ZFStatTypeTemp] = RoundToCeil(float(zf_stat[client][stat][ZFStatTypeTemp]) * strPct);
  zf_stat[client][stat][ZFStatTypeTempDuration] = RoundToCeil(float(zf_stat[client][stat][ZFStatTypeTempDuration]) * durPct);
}

////////////////////////////////////////////////////////////
//
// Perk Cond Routines
//
////////////////////////////////////////////////////////////
stock bool:getCond(client, ZFCond:cond)
{ return zf_cond[client][cond] > 0; }

stock addCond(client, ZFCond:cond, val)
{ zf_cond[client][cond] += val; }

stock subCond(client, ZFCond:cond, val)
{ zf_cond[client][cond] = max(0, zf_cond[client][cond] - val); }

////////////////////////////////////////////////////////////
//
// Perk Reset Logic
// + Used to clear variables with no regard to game state.
//
////////////////////////////////////////////////////////////
stock resetAllClients()
{
  for(new i = 0; i <= MAXPLAYERS; i++)
    resetClient(i);
}

stock resetClient(client)
{  
  // State
  zf_lastAttack[client] = 0;
  zf_lastButtons[client] = 0;
  zf_lastHealth[client] = 0;
  zf_lastKiller[client] = 0;
  zf_lastPoison[client] = 0;
  zf_lastTeam[client] = 0;
  zf_perkTimer[client] = 0;
  zf_perkState[client] = 0;
  for(new i = 0; i < 5; i++)
    for(new j = 0; j < 3; j++)
      zf_perkPos[client][i][j] = 0.0;
  zf_perkStr[client] = "";
  zf_perkAlphaMaster[client] = 0;
  
  // Bonuses
  resetClientStats(client);
  resetClientConds(client);
  
  // FX
  removeAura(client);
  removeIcon(client);
  removeItems(client);

  // New Perk Object
  if (g_hPerks[client] != null)
  {
      g_hPerks[client].onRemove();
      delete g_hPerks[client];
      g_hPerks[client] = null;
  }
}

stock resetClientStats(client)
{
  for(new stat = 0; stat < TOTAL_ZFSTATS; stat++)
    for(new type = 0; type < TOTAL_ZFSTAT_TYPES; type++)
      zf_stat[client][stat][type] = 0;
}

stock resetStatType(ZFStatType:type)
{
  for(new i = 0; i <= MAXPLAYERS; i++)
    resetClientStatType(i, type);
}

stock resetClientStatType(client, ZFStatType:type)
{
  for(new stat = 0; stat < TOTAL_ZFSTATS; stat++)
    zf_stat[client][stat][type] = 0;
}

stock resetClientConds(client)
{  
  for(new cond = 0; cond < TOTAL_ZFCONDS; cond++)
    zf_cond[client][cond] = 0;
}

////////////////////////////////////////////////////////////
//
// Perk Selection Utilities
//
////////////////////////////////////////////////////////////
stock bool:surPerkEnabled(perk)
{ return (zf_surPerksEnabled & (1 << perk)) != 0; }
stock bool:zomPerkEnabled(perk)
{ return (zf_zomPerksEnabled & (1 << perk)) != 0; }
stock bool:usingSurPerk(client, perk)
{ return (prefGet(client, SurPerk) == perk); }
stock bool:usingZomPerk(client, perk)
{ return (prefGet(client, ZomPerk) == perk); } 

//
// Called by client when he selects a survivor perk (either from menu or by command).
//
stock selectSurPerk(client, perk)
{
  // Do not select invalid perk.
  if((perk < 0) || (perk >= TOTAL_SUR_PERKS))
    PrintToChat(client, "\x05[ZF]\x01 Can't select perk: Invalid perk.");  
  // Do not select disabled perk.
  else if(!surPerkEnabled(perk))
    PrintToChat(client, "\x05[ZF]\x01 Can't select %s as survivor perk: Disabled by server.", ZF_SURPERK_NAME[perk]);
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
    PrintToChat(client, "\x05[ZF]\x01 Can't select %s as survivor perk: Non-standard game mode active.", ZF_SURPERK_NAME[perk]);
  // Do not select perk if selecting it violate perk limit.
  else if(surPerkAtLimit(client, perk))
    PrintToChat(client, "\x05[ZF]\x01 Can't select %s as survivor perk: Perk limit reached (%d).", ZF_SURPERK_NAME[perk], zf_surPerksLimit[perk]);
  // Select perk.
  else
  {
    prefSet(client, SurPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    // Defer perk selection for non-survivors.
    if(!isSur(client))
      PrintToChat(client, "\x05[ZF]\x01 你选择 %s 作为幸存者职业,将会在复活后切换.", ZF_SURPERK_NAME[perk]);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "\x05[ZF]\x01 你选择 %s 作为幸存者职业,将会在复活后切换.", ZF_SURPERK_NAME[perk]);
    // Respawn client. This will trigger new perk selection.
    else
      TF2_RespawnPlayer(client);
  }
}

//
// Called by client when he selects a zombie perk (either from menu or by command).
//
stock selectZomPerk(client, perk)
{
  // Do not select invalid perk.
  if((perk < 0) || (perk >= TOTAL_ZOM_PERKS))
    PrintToChat(client, "\x05[ZF]\x01 你不能选择 %s 职业: 该职业无效.");  
  // Do not select disabled perk.
  else if(!zomPerkEnabled(perk))
    PrintToChat(client, "\x05[ZF]\x01 你不能选择 %s 职业: 该职业被服务器禁用.", ZF_ZOMPERK_NAME[perk]);
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
    PrintToChat(client, "\x05[ZF]\x01 你不能选择 %s 职业: 现在是非常规模式.", ZF_ZOMPERK_NAME[perk]);    
  // Do not select perk if selecting it violate perk limit.    
  else if(zomPerkAtLimit(client, perk))
    PrintToChat(client, "\x05[ZF]\x01 你不能选择 %s 职业: 该职业达到限制.(%d).", ZF_ZOMPERK_NAME[perk], zf_zomPerksLimit[perk]);    
  // Select perk.
  else
  {
    prefSet(client, ZomPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    // Defer perk selection for non-zombies.
    if(!isZom(client))
      PrintToChat(client, "\x05[ZF]\x01 你选择 %s 作为僵尸职业,将会在复活后切换.", ZF_ZOMPERK_NAME[perk]);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "\x05[ZF]\x01 你选择 %s 作为僵尸职业,将会在复活后切换.", ZF_ZOMPERK_NAME[perk]);
    // Respawn client. This will trigger new perk selection.
    else
      TF2_RespawnPlayer(client);
  }
}

////////////////////////////////////////////////////////////
//
// Perk Limit Utilities
//
////////////////////////////////////////////////////////////
stock bool:surPerkAtLimit(client, perk)
{
  // Perk limit of -1 means no limit.
  if(zf_surPerksLimit[perk] == -1)
  {
    return false;
  }
  // Perk limit of 0 means a limit of 0.
  else if(zf_surPerksLimit[perk] == 0)
  {
    return true;
  }
  // Tally up perk use for all survivors (alive or not).
  // Do not include current client in tally.
  else
  {
    new total = 0;
    for(new i = 1; i <= MaxClients; i++)
      if((i != client) && IsClientInGame(i) && isSur(i) && usingSurPerk(i, perk))
          total++;
  
    return (total >= zf_surPerksLimit[perk]);
  }
}

stock bool:zomPerkAtLimit(client, perk)
{
  // Perk limit of -1 means no limit.
  if(zf_zomPerksLimit[perk] == -1)
  {
    return false;
  }
  // Perk limit of 0 means a limit of 0.
  else if(zf_zomPerksLimit[perk] == 0)
  {
    return true;
  }
  // Tally up perk use for all zombies (alive or not).
  // Do not include current client in tally.
  else
  {
    new total = 0;
    for(new i = 1; i <= MaxClients; i++)
      if((i != client) && IsClientInGame(i) && isZom(i) && usingZomPerk(i, perk))
          total++;
  
    return (total >= zf_zomPerksLimit[perk]);
  }
}

////////////////////////////////////////////////////////////
//
// Perk HUD Logic
//
////////////////////////////////////////////////////////////
updateHud(client)
{
  decl String:strHudPerk[48];
  decl String:strHudBonuses[32];
  
  if(IsClientInGame(client) && !(zf_lastButtons[client] & IN_SCORE))
  {
    if(isSur(client) || isZom(client))
    {
      if(IsPlayerAlive(client))
      {  
        // HUD Element: Perk Selection
        if(isSur(client))
          Format(strHudPerk, sizeof(strHudPerk), "职业: %s", ZF_SURPERK_NAME[prefGet(client, SurPerk)]);
        else if(isZom(client))
          Format(strHudPerk, sizeof(strHudPerk), "职业: %s", ZF_ZOMPERK_NAME[prefGet(client, ZomPerk)]);      
        else
          strHudPerk = "";
        SetHudTextParams(0.15, 0.90, 2.1, 200, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s %s", strHudPerk, zf_perkStr[client]);
        
        // HUD Element: Bonuses 1 (Attack, Defense)
        Format(strHudBonuses, sizeof(strHudBonuses), "攻击力: %d%s, 防御力: %d%s", getStat(client, ZFStatAtt), "\%", getStat(client, ZFStatDef), "\%"); 
        SetHudTextParams(0.15, 0.93, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine1, "%s", strHudBonuses);
        
        // HUD Element: Bonuses 2 (Crit, Speed)
        Format(strHudBonuses, sizeof(strHudBonuses), "暴击率: %d%s, 速度: %d", getStat(client, ZFStatCrit), "\%", getStat(client, ZFStatSpeed)); 
        SetHudTextParams(0.15, 0.96, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine2, "%s", strHudBonuses);      
      }
      else
      {
        // HUD Element: Killer's perk
        new killer = zf_lastKiller[client];
        if(validClient(killer) && (killer != client))
        {
          if(isSur(killer))
            Format(strHudPerk, sizeof(strHudPerk), "你被 %s 职业杀死了......", ZF_SURPERK_NAME[prefGet(killer, SurPerk)]);
          else if(isZom(killer))
            Format(strHudPerk, sizeof(strHudPerk), "你被 %s 职业杀死了......", ZF_ZOMPERK_NAME[prefGet(killer, ZomPerk)]);
          else
            strHudPerk = "";
          SetHudTextParams(0.15, 0.90, 2.1, 250, 200, 200, 150);
          ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);            
        }
      }
    }
    else
    {
      // HUD Element: Specator target's perk
      new spectate = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
      if(validClient(spectate))
      {
        if(isSur(spectate))
          Format(strHudPerk, sizeof(strHudPerk), "观察目标职业: %s ", ZF_SURPERK_NAME[prefGet(spectate, SurPerk)]);
        else if(isZom(spectate))
          Format(strHudPerk, sizeof(strHudPerk), "观察目标职业: %s ", ZF_ZOMPERK_NAME[prefGet(spectate, ZomPerk)]);
        else
          strHudPerk = "";
        SetHudTextParams(0.15, 0.90, 2.1, 250, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);            
      }
    }
  }     
}

////////////////////////////////////////////////////////////
//
// Aura Effect Routines
//
////////////////////////////////////////////////////////////
stock createAura(client, const String:strPart[], ZFParticleAttachStyle:attachStyle, const Float:offset[3] = {0.0, 0.0, 0.0})
{
  removeAura(client);
  zf_aura[client] = fxCreateParticle(strPart, client, attachStyle, 0.0, offset);
}

stock removeAura(client)
{
  fxDeleteParticle(zf_aura[client]);
  zf_aura[client] = -1;
}

stock bool:validAura(client)
{ return fxIsParticleValid(zf_aura[client]); }

stock showAura(client)
{ fxStartParticle(zf_aura[client]); }

stock hideAura(client)
{ fxStopParticle(zf_aura[client]); }

////////////////////////////////////////////////////////////
//
// Icons Effect Routines
//
////////////////////////////////////////////////////////////
stock createIcon(ownerClient, targetClient, const String:strSprite[])
{
  removeIcon(ownerClient);
  fxCreateSprite(strSprite, targetClient, zf_icon[ownerClient][ICON_SPR], zf_icon[ownerClient][ICON_ANC]);
}

stock removeIcon(client)
{
  fxDeleteSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]);
  zf_icon[client][ICON_SPR] = -1;
  zf_icon[client][ICON_ANC] = -1;
}

stock bool:validIcon(client)
{ return fxIsSpriteValid(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

stock showIcon(client)
{ fxShowSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

stock hideIcon(client)
{ fxHideSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

////////////////////////////////////////////////////////////
//
// Items Effect Routines
//
////////////////////////////////////////////////////////////
stock removeItem(client, item)
{
  if(validItem(client, item))
    fxPuffSmall(zf_item[client][item]);
  fxDeleteModel(zf_item[client][item]);
  zf_item[client][item] = -1;
}

stock removeItems(client)
{
  for(new i = 0; i < MAX_ITEMS; i++)
    removeItem(client, i);
}

stock bool:validItem(client, itemIndex)
{ return fxIsModelValid(zf_item[client][itemIndex]); }

stock getItemMetadata(item)
{
  // Utilizes an entity prop that is, in theory, not used.
  return GetEntProp(item, Prop_Send, "m_hEffectEntity");
}

stock setItemMetadata(item, value)
{
  // Utilizes an entity prop that is, in theory, not used.
  SetEntProp(item, Prop_Send, "m_hEffectEntity", value);
}

stock getFreeItemIndex(client, maxItems)
{
  // Return first item index in range [0, maxItems-1] that is free (i.e. not valid).
  // Return -1 if no item in range is free.
  for(new i = 0; i < maxItems; i++)
  {
    if(!validItem(client, i))
      return i;
  }
  return -1;
}

////////////////////////////////////////////////////////////
//
// Perk Utility Helpers
//
////////////////////////////////////////////////////////////
stock addHealth(client, health, bool:doOverheal = false)
{
  // Add health up to max (or overheal max [500]).
  new cur = GetClientHealth(client);
  new lim = doOverheal ? 500 : clientMaxHealth(client);
  if(cur < lim)
  {
    SetEntityHealth(client, min((cur+health),lim));
    if(!isCloaked(client)) 
      fxHealthGained(client);
  }
}

////////////////////////////////////////////////////////////
//
// Perk Logic Helpers
//
////////////////////////////////////////////////////////////
stock bool:doItemCollide(
  ent, 
  const Float:prevPos[3], 
  Float:hitPos[3], 
  Float:hitVec[3])
{
  new bool:didHit = false;
  decl Float:thisPos[3];
  decl Float:diffPos[3];
  decl Float:nextPos[3];
  
  // Use current position(thisPos) and previous position(prevPos)
  // to calculate a difference (diffPos). Normalize and scale
  // difference to compute predicted next position (nextPos).
  getEntityPos(ent, thisPos);
  SubtractVectors(thisPos, prevPos, diffPos);      
  NormalizeVector(diffPos, diffPos);
  ScaleVector(diffPos, 35.0);              
  AddVectors(thisPos, diffPos, nextPos);

  // Trace from current to next position, checking for impacts.
  new Handle:TraceEx = TR_TraceRayFilterEx(thisPos, nextPos, MASK_SOLID, RayType_EndPoint, TraceFilter);
  if(TR_DidHit(TraceEx))
  {
    TR_GetEndPosition(hitPos, TraceEx);
    TR_GetPlaneNormal(TraceEx, hitVec);
    if(!TR_PointOutsideWorld(hitPos))
    {
      didHit = true;
    }
  }
  CloseHandle(TraceEx);

  return didHit;     
}

public bool:TraceFilter(ent, contentMask)
{
  return false;
}

stock doItemImpact(
  client, 
  const Float:hitPos[3], 
  const Float:hitVec[3], 
  const color[3])
{
  decl Float:hitAng[3];   
  GetVectorAngles(hitVec, hitAng);
  hitAng[0] += 90.0; // Adjust for ZFMDL_HAYBALE
    
  // Place item at hitPos, oriented based on hitVec.
  return fxCreateModelStatic(ZFMDL_HAYBALE, client, true, false, hitPos, hitAng, color); 
}

stock doItemPlace(client, const String:strModel[])
{
  decl Float:pos[3];
  decl Float:ang[3]; 
  
  GetClientAbsOrigin(client, pos); 
  GetClientAbsAngles(client, ang);    

  // Place item at client's feet.
  return fxCreateModelStatic(strModel, client, true, false, pos, ang);
}

stock doItemThrow(
  client, 
  const String:strModel[], 
  Float:force, 
  const color[3] = {255, ...})
{
  decl Float:pos[3];
  decl Float:ang[3];
    
  GetClientEyePosition(client, pos);  
  GetClientEyeAngles(client, ang);
  ang[0] += GetRandomFloat(-20.0, -10.0); // Pitch (-90.0 is up, 90.0 is down)
  ang[1] += GetRandomFloat(-6.0, 6.0);    // Yaw

  // Throw item from client's eye position and along look vector.
  return fxCreateModelThrown(strModel, client, pos, ang, force, color);  
}

////////////////////////////////////////////////////////////
//
// Perk Logic Helpers
//
////////////////////////////////////////////////////////////
stock perk_DoAlphaSummon(client)
{
  decl summonCount;
  decl validMinionCount;
  decl validMinions[MAXPLAYERS+1];
  decl Float:clientPos[3];
  decl Float:clientAng[3];
  decl Float:summonVel[3];
  
  GetClientAbsOrigin(client, clientPos);
  GetClientAbsAngles(client, clientAng);
  
  // Determine which clients to summon.
  validMinionCount = 0;
  for(new i = 1; i <= MaxClients; i++)
  {
    if(i == client) continue;
    if(IsClientInGame(i) && IsPlayerAlive(i) && isZom(i))
      if((zf_perkAlphaMaster[i] == client) && (isScout(i) || isHeavy(i)))
        validMinions[validMinionCount++] = i;
  }
   
  // Summon valid minions.
  summonCount = 0;
  SortIntegers(validMinions, validMinionCount, Sort_Random);
  for(new i = 0; i < validMinionCount; i++)
  {
    if(i == ZF_ALPHA_SUMMON_LIMIT) break;
    
    summonVel[0] = GetRandomFloat(-100.0, 100.0);
    summonVel[1] = GetRandomFloat(-100.0, 100.0);
    summonVel[2] = GetRandomFloat(300.0, 400.0);
    TeleportEntity(validMinions[i], clientPos, clientAng, summonVel);
    ClientCommand(validMinions[i], "voicemenu 1 0");
    PrintHintText(validMinions[i], "你的主人召唤了你!");
    fxTeleportTrail(validMinions[i], 5.0);
    ++summonCount;
  }
  
  if(summonCount == 1)
    PrintHintText(client, "你召唤了 %d 个随从!", summonCount);
  else if(summonCount > 1)
    PrintHintText(client, "你召唤了 %d 个随从!", summonCount);
    
  return summonCount;
}

stock doCarpenterBuild(client, Float:physPos[3])
{
  decl Float:pos[3];
  decl Float:ang[3]; 
  decl Float:vecFwd[3];
  decl Float:vecRgt[3];
  GetClientAbsOrigin(client, pos); 
  GetClientAbsAngles(client, ang);   
     
  // Move item in front of client.
  GetAngleVectors(ang, vecFwd, vecRgt, NULL_VECTOR); 
  ScaleVector(vecFwd, 45.0);
  ScaleVector(vecRgt, -30.0);           // Adjust for ZFMDL_FENCE
  AddVectors(pos, vecFwd, pos);         // Adjust for ZFMDL_FENCE
  AddVectors(pos, vecRgt, pos);         // Adjust for ZFMDL_FENCE
  pos[2] += 120.0; 
  ang[0] += GetRandomFloat(-4.0, 4.0);  // Adjust for ZFMDL_FENCE
  ang[1] += GetRandomFloat(85.0, 95.0); // Adjust for ZFMDL_FENCE

  // Calculate physical position (used for distance calculations)
  physPos[0] = pos[0];
  physPos[1] = pos[1];
  physPos[2] = pos[2] - 120.0;
  SubtractVectors(physPos, vecRgt, physPos);
    
  // Barricade build effects
  new Float:off[3] = {0.0,...};
  AddVectors(off, vecFwd, off);
  fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);    
   
  ScaleVector(vecRgt, 1.3);
  AddVectors(vecFwd, vecRgt, off);
  fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);  
  SubtractVectors(vecFwd, vecRgt, off);
  fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);  

  ScaleVector(vecRgt, 1.3);
  AddVectors(vecFwd, vecRgt, off);
  fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);  
  SubtractVectors(vecFwd, vecRgt, off);
  fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);  
  
  fxCreateSoundToAll(ZFSND_WOOD_HIT[GetRandomInt(0,1)], client);
    
  // Build solid item in front of client.
  return fxCreateModelStatic(ZFMDL_FENCE, client, false, true, pos, ang);  
}

stock doFriendSelect(client, desiredFriend = 0)
{
  decl validSurCount;
  decl validSurs[MAXPLAYERS+1];
  new selectedFriend = desiredFriend;
  
  // Find random friend (if no friend is desired already).
  if(selectedFriend == 0)
  {
    validSurCount = 0;
    for(new i = 1; i <= MaxClients; i++)
    {
      if((client != i) && validLivingSur(i)) 
        validSurs[validSurCount++] = i;
    }
   
    if(validSurCount > 0)
    {
      selectedFriend = validSurs[GetRandomInt(0, validSurCount - 1)];
    }
  }
  
  // Select friend.
  if(selectedFriend == 0)
  {
    zf_perkState[client] = 0;
    zf_perkTimer[client] = 0;
    PrintHintText(client, "你还没找到伙伴,快去找一个吧!");    
  }  
  else
  {
    zf_perkState[client] = selectedFriend;
    zf_perkTimer[client] = ZF_FRIEND_CRITTIME_INIT;        
    PrintHintText(client, "你的伙伴是: %N . 多和ta呆在一起!", zf_perkState[client]);
    createIcon(client, zf_perkState[client], ZFSPR_DOMINATED);    
  }
}

stock doMarkedSelect(client)
{
  decl validSurCount;
  decl validSurs[MAXPLAYERS+1];
 
  // Build common state information.
  validSurCount = 0;
  for(new i = 1; i <= MaxClients; i++)
  {
    if(validLivingSur(i)) 
      validSurs[validSurCount++] = i;
  }

  // Handle mark select logic.
  if(validSurCount < ZF_MARKED_MIN_SURVIVORS)
  {
    zf_perkState[client] = -1;

    PrintHintText(client, "没有足够的幸存者可以标记.");
  }
  else
  {
    zf_perkState[client] = validSurs[GetRandomInt(0,(validSurCount-1))];

    createIcon(client, zf_perkState[client], ZFSPR_EXCLAMATION);
    PrintHintText(client, "你标记了一个新的目标, ta是 %N !", zf_perkState[client]);
    PrintHintText(zf_perkState[client], "你被某个僵尸标记了! 小心行事!");
  }
}

stock doNinjaDecoyPlace(client)
{  
  // Create decoy at client.
  fxPuffSmall(client);
  zf_item[client][0] = doItemPlace(client, ZFMDL_CUTOUT[_:TF2_GetPlayerClass(client)]);
    
  // Move client to retreat position.
  decl Float:retreatPos[3];
  getEntityPos(zf_aura[client], retreatPos);
  TeleportEntity(client, retreatPos, NULL_VECTOR, NULL_VECTOR);
  fxPuffSmall(client);  
  
  // Remove retreat point.
  removeAura(client);
}

stock doNinjaDecoyPoof(client)
{
  // Remove decoy, if it exists.
  if(validItem(client, 0))
  {
    fxPuffBig(zf_item[client][0]);
    removeItem(client, 0);
  }
}

stock doThievingLimit(client)
{
  // Enforce ammo limit.
  new ammo;
  switch(TF2_GetPlayerClass(client))
  {
    case TFClass_Scout: 
    {
      ammo = min(zf_perkState[client], getClipAmmo(client, 0));
      setClipAmmo(client, 0, ammo);
      setResAmmo(client, 0, 0);
      zf_perkState[client] = ammo;
    }
    case TFClass_Heavy:
    {
      ammo = min(zf_perkState[client], getResAmmo(client, 0));
      setResAmmo(client, 0, ammo);
      zf_perkState[client] = ammo;
    }
    case TFClass_Spy:
    {
      ammo = min(zf_perkState[client], getClipAmmo(client, 0));
      setClipAmmo(client, 0, ammo);
      setResAmmo(client, 1, 0); // Slot 0 weapon uses slot 1 ammo reserve.
      zf_perkState[client] = ammo;
    }
  }
  
  // Strip weapon when ammo reaches zero.
  if(zf_perkState[client] == 0)
  { 
    stripWeaponSlot(client, 0);  
    switchToSlot(client, 2);
  }
}

stock doThievingSteal(attacker, victim, slot)
{
  //
  // Handle victim logic. Remove weapon in given slot, switch to melee.
  //
  stripWeaponSlot(victim, slot);
  switchToSlot(victim, 2);
  PrintHintText(victim, "你的武器被潜行僵尸偷走了!");
  
  //
  // Handle attacker logic. Add ammo, give weapon, switch to weapon.
  //
  if(zf_perkState[attacker] == 0)
  {
    new health = GetClientHealth(attacker);
    TF2_RegeneratePlayer(attacker);
    SetEntityHealth(attacker, health);
    switch(TF2_GetPlayerClass(attacker))
    {
      case TFClass_Scout:
      {
        new ammo = 0;
        if(isEquipped(attacker, ZFWEAP_SCATTERGUN))
          ammo = 3;
        if(isEquipped(attacker, ZFWEAP_FORCEANATURE))
          ammo = 2;
        if(isEquipped(attacker, ZFWEAP_SHORTSTOP))
          ammo = 4;
        setClipAmmo(attacker, 0, ammo);
        setResAmmo(attacker, 0, 0);
        zf_perkState[attacker] = ammo;
      }
      case TFClass_Heavy:
      {
        setResAmmo(attacker, 0, 20);
        zf_perkState[attacker] = 20;
      }
      case TFClass_Spy:
      {
        setClipAmmo(attacker, 0, 4);
        setResAmmo(attacker, 0, 0);
        zf_perkState[attacker] = 4;
      }
    }  
    stripWeapons(attacker, true);
    switchToSlot(attacker, slot);
    PrintHintText(attacker, "你偷走了幸存者的一把武器! 现在你也有武器了!");
  }
}

////////////////////////////////////////////////////////////
//
// Perk Bonus Update Logic
//
////////////////////////////////////////////////////////////
stock updateClientPermStats(client)
{  
  resetClientStatType(client, ZFStatTypePerm);

  //
  // Apply permanent bonuses for survivors.
  // (Survivors must be alive)
  //
  if(validLivingSur(client))
  {        
    // Handle Perk (S): ATHLETIC
    if(usingSurPerk(client, ZF_PERK_ATHLETIC))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
      addStat(client, ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
      addStat(client, ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
    }
    // Handle Perk (S): CARPENTER
    else if(usingSurPerk(client, ZF_PERK_CARPENTER))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, CARPENTER_ATTACK);
      addStat(client, ZFStatDef, ZFStatTypePerm, CARPENTER_DEFEND);
    }
    // Handle Perk (S): HEROIC
    else if(usingSurPerk(client, ZF_PERK_HEROIC))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, HEROIC_COMBAT);
      addStat(client, ZFStatDef, ZFStatTypePerm, HEROIC_COMBAT);
      zf_perkTimer[client] = HEROIC_CRITTIME_INIT;
    } 
    // Handle Perk (S): JUGGERNAUT
    else if(usingSurPerk(client, ZF_PERK_JUGGERNAUT))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_JUGGERNAUT_ATTACK);
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_JUGGERNAUT_DEFEND);
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_JUGGERNAUT_SPEED);
    }
    // Handle Perk (S): LEADER
    else if(usingSurPerk(client, ZF_PERK_LEADER))
    {
      addStat(client, ZFStatCrit, ZFStatTypePerm, ZF_LEADER_SELF_CRIT);
    }
    // Handle Perk (S): NINJA
    else if(usingSurPerk(client, ZF_PERK_NINJA))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_NINJA_ATTACK);        
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_NINJA_SPEED);      
    }
    // Handle Perk (S): TURTLE
    else if(usingSurPerk(client, ZF_PERK_TURTLE))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_TURTLE_ATTACK);
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_TURTLE_DEFEND);
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_TURTLE_SPEED);
    }
    // Handle Perk (S): SUPPLIER
    else if(usingSurPerk(client, ZF_PERK_SUPPLIER)){	
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_SUPPLIER_SELF_DEFEND);

    }
  }
    
  //
  // Apply permanent bonuses for zombies.
  // (Zombies can be dead or alive)
  //
  else if(validZom(client))
  {
    // Handle Perk (Z): COMBUSTIBLE
    if(usingZomPerk(client, ZF_PERK_COMBUSTIBLE))
    {
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_COMBUSTIBLE_DEFEND);
    }
    // Handle Perk (Z): LEAP
    else if(usingZomPerk(client, ZF_PERK_LEAP))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_LEAP_COMBAT);
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_LEAP_COMBAT);
    }
    // Handle Perk (Z): SICK
    else if(usingZomPerk(client, ZF_PERK_SICK))
    {
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_SICK_DEFEND);          
    }
    // Handle Perk (Z): SCORCHING
    else if(usingZomPerk(client, ZF_PERK_SCORCHING))
    {
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_SCORCHING_SPEED);      
    }
    // Handle Perk (Z): SWARMING
    else if(usingZomPerk(client, ZF_PERK_SWARMING))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_SWARMING_COMBAT);
      addStat(client, ZFStatDef, ZFStatTypePerm, ZF_SWARMING_COMBAT);  
      addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_SWARMING_SPEED);   
    }
    // Handle Perk (Z): THIEVING
    else if(usingZomPerk(client, ZF_PERK_THIEVING))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_THIEVING_ATTACK);
    }
    // Handle Perk (Z): TOXIC
    else if(usingZomPerk(client, ZF_PERK_TOXIC))
    {
      addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_TOXIC_ATTACK);
    }  
  }       
}

stock updateClientPermEffects(client)
{
  new Float:headOffset[3] = {0.0, 0.0, 15.0};
  
  // Handle survivor effects.
  if(validLivingSur(client))
  {            
    if(usingSurPerk(client, ZF_PERK_HOLY))
      createAura(client, ZFPART_AURAGLOWBEAMS, AttachHead, headOffset);
    else if(usingSurPerk(client, ZF_PERK_LEADER))
      createAura(client, ZFPART_AURAINRED, AttachBase);
  }
  
  // Handle zombie effects.
  else if(validZom(client))
  {
    if(usingZomPerk(client, ZF_PERK_ALPHA))
      createAura(client, ZFPART_AURAINBLU, AttachBase);
    else if(usingZomPerk(client, ZF_PERK_HORRIFYING))
      createAura(client, ZFPART_AURAOUTBLU, AttachBase);
    else if(usingZomPerk(client, ZF_PERK_SWARMING))
      createAura(client, ZFPART_AURAFLIES, AttachHead, headOffset);      
  }         
}

stock updateCondStats()
{  
  decl i,j,k;
  decl validSurCount;
  decl validZomCount;
  decl validSurs[MAXPLAYERS+1];
  decl validZoms[MAXPLAYERS+1];  
  decl Float:validSursPos[MAXPLAYERS+1][3];
  decl Float:validZomsPos[MAXPLAYERS+1][3];
  
  // Clear conditional bonuses
  resetStatType(ZFStatTypeCond);
  
  // Build common state information. 
  // + Survivors must be in game and alive.
  // + Zombies must be in game and alive.
  validSurCount = 0;
  validZomCount = 0;  
  for(i = 1; i <= MaxClients; i++)
  {
    if(IsClientInGame(i) && IsPlayerAlive(i))
    {
      if(isSur(i))
      {
        validSurs[validSurCount] = i;
        GetClientAbsOrigin(i, validSursPos[validSurCount]);
        validSurCount++;
      }
      else if(isZom(i))
      {
        validZoms[validZomCount] = i;
        GetClientAbsOrigin(i, validZomsPos[validZomCount]);
        validZomCount++;
      }
    }
  }
  
  // 3. Apply conditional bonuses for survivors.
  for(i = 0; i < validSurCount; i++)
  {   
    new thisSur = validSurs[i];
    
    // DEBUG
    // PrintToChat(thisSur, "PrCl %d, PrRs %d, SeCl %d, SeRs %d", getClipAmmo(thisSur, 0), getResAmmo(thisSur, 0), getClipAmmo(thisSur, 1), getResAmmo(thisSur, 1));
       
    // Handle Perk (S): CARPENTER
    // + Update cooldown timer.
    // + Update HUD info (ability ready).
    if(usingSurPerk(thisSur, ZF_PERK_CARPENTER))
    {
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你可以放置一个新的障碍物了.");
        }        
      }
      
      if(getFreeItemIndex(thisSur, CARPENTER_MAX_ITEMS) >= 0)
        Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(障碍物准备就绪)" : ""));
      else
        Format(zf_perkStr[thisSur], 32, "(障碍物数量已满)");
    }    
    // Handle Perk (S): CHARITABLE
    // + Update HUD info (gifts ready).
    else if(usingSurPerk(thisSur, ZF_PERK_CHARITABLE))
    {
      if(getFreeItemIndex(thisSur, ZF_CHARITABLE_MAX_ITEMS) >= 0)
        Format(zf_perkStr[thisSur], 32, "(礼物点数: %d)", (zf_perkState[thisSur] / ZF_CHARITABLE_POINTS_GIFT));
      else
        Format(zf_perkStr[thisSur], 32, "(礼物点数已满)");
    }    
    // Handle Perk (S): COWARDLY
    // + Apply scared bonuses.
    // + Update scared timer.
    // + Update HUD info (ability ready).
    else if(usingSurPerk(thisSur, ZF_PERK_COWARDLY))
    {      
      if(zf_perkTimer[thisSur] > ZF_COWARDLY_DURATION_COOLDOWN)
      {
        addStat(thisSur, ZFStatDef, ZFStatTypeCond, ZF_COWARDLY_DEFEND);
        addStat(thisSur, ZFStatSpeed, ZFStatTypeCond, ZF_COWARDLY_SPEED);
      }
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == ZF_COWARDLY_DURATION_COOLDOWN)
        {
          PrintHintText(thisSur, "你的恐慌状态结束了.");
        }
        else if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你开始感到害怕.");
        }
      }
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(随时准备跑路)" : ""));
    }

    // Handle Perk (S): FRIEND
    else if(usingSurPerk(thisSur, ZF_PERK_FRIEND))
    {
      if(validLivingSur(zf_perkState[thisSur]))
      {
        // Handle passive friend bonus.
        decl Float:friendPos[3];
        GetClientAbsOrigin(zf_perkState[thisSur], friendPos);
        if(GetVectorDistance(validSursPos[i], friendPos, true) <= ZF_FRIEND_RADIUSSQ)
        {          
          addHealth(thisSur, ZF_FRIEND_REGEN);
          addHealth(zf_perkState[thisSur], ZF_FRIEND_REGEN);
          addStat(thisSur, ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);
          addStat(zf_perkState[thisSur], ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);    
        }
      }
      else
      {
        // Handle friend death crit time bonus.
        if(zf_perkState[thisSur] > 0)
        {
          zf_perkState[thisSur] = 0;
          PrintHintText(thisSur, "你的伙伴死了!现在杀了这些该死的僵尸!");
    
          addCondKritz(thisSur, float(zf_perkTimer[thisSur]));
          fxKritzStart(thisSur);
          fxDeathScream(thisSur);
          createAura(thisSur, ZFPART_AURAOUTRED, AttachBase);
          removeIcon(thisSur);
        }
        else
        {
          if(zf_perkTimer[thisSur] > 0)
          {
            zf_perkTimer[thisSur]--;
            if(zf_perkTimer[thisSur] == 0)
            {
              removeAura(thisSur);
              fxKritzStop(thisSur);
            }
            else
            {
              addCondKritz(thisSur, float(zf_perkTimer[thisSur]));
            }
          }
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, "(暴击时间: %d)", zf_perkTimer[thisSur]);
    }    
        
    // Handle Perk (S): HEROIC
    else if(usingSurPerk(thisSur, ZF_PERK_HEROIC))
    {
      if(validSurCount == 1)
      {
        // Activate bonus
        if(zf_perkState[thisSur] == 0)
        {
          zf_perkState[thisSur] = 1;
          PrintHintText(thisSur, "你是最后一位幸存者了!坚持活下去吧!");

          addCondKritz(thisSur, float(zf_perkTimer[thisSur]));
          fxKritzStart(thisSur);
          createAura(thisSur, ZFPART_AURAOUTRED, AttachBase);
        }
        // Continue bonus
        else
        {
          if(zf_perkTimer[thisSur] > 0)
          {            
            zf_perkTimer[thisSur]--;
            if(zf_perkTimer[thisSur] == 0)
            {
              remCondKritz(thisSur);
              fxKritzStop(thisSur);
            }
            else
              addCondKritz(thisSur, float(zf_perkTimer[thisSur]));
          }
        }        
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, "(暴击时间: %d)", zf_perkTimer[thisSur]);  
    }
    
    // Handle Perk (S): HOLY
    else if(usingSurPerk(thisSur, ZF_PERK_HOLY))
    {
      if(isGrounded(thisSur) && isCrouching(thisSur) && isNotMoving(thisSur))
      {
        for(j = 0; j < validSurCount; j++)
        {
          if(GetVectorDistance(validSursPos[i], validSursPos[j], true) <= ZF_HOLY_RADIUSSQ)
          {
            addHealth(validSurs[j], ZF_HOLY_REGEN);
          }
        }
        showAura(thisSur);
      }      
      else
      {
        hideAura(thisSur);
      }
    }
    
    // Handle Perk (S): LEADER
    else if(usingSurPerk(thisSur, ZF_PERK_LEADER))
    {
      // Handle passive bonuses.
      for(j = 0; j < validSurCount; j++)
      {
        if(i == j) continue;
        if(GetVectorDistance(validSursPos[i], validSursPos[j], true) <= ZF_LEADER_OTHERS_RADIUSSQ)
        {          
          addStat(validSurs[j], ZFStatAtt, ZFStatTypeCond, ZF_LEADER_OTHERS_ATTACK);
        }
      }
      
      // Handle rally bonuses.
      new rallyCount = 0;
      decl Float:rallyPos[3];            
      if(validItem(thisSur, 0))
      {
        getEntityPos(zf_item[thisSur][0], rallyPos);
        for(j = 0; j < validSurCount; j++)
        {
          if(i == j) continue;
          if(GetVectorDistance(rallyPos, validSursPos[j], true) <= ZF_LEADER_RALLY_RADIUSSQ)
          {
            ++rallyCount;
            addStat(validSurs[j], ZFStatAtt, ZFStatTypeCond, ZF_LEADER_RALLY_OTHERS_ATTACK);
            addStat(validSurs[j], ZFStatDef, ZFStatTypeCond, ZF_LEADER_RALLY_OTHERS_DEFEND);
          }
        }
        
        addStat(validSurs[i], ZFStatAtt, ZFStatTypeCond, (rallyCount * ZF_LEADER_RALLY_SELF_ATTACK));
        addStat(validSurs[i], ZFStatDef, ZFStatTypeCond, (rallyCount * ZF_LEADER_RALLY_SELF_DEFEND));
      }
      
      // Handle timer logic.
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == ZF_LEADER_RALLY_DURATION)
        {
          PrintHintText(thisSur, "你的旗帜消失了.");
          removeItem(thisSur, 0);
        }
        else if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你可以放置一面新的旗帜了.");
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(旗帜准备就绪)" : ""));        
    }
     
    // Handle Perk (S): NINJA
    else if(usingSurPerk(thisSur, ZF_PERK_NINJA))
    {
      // Handle timer logic.
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == ZF_NINJA_DURATION_COOLDOWN)
        {
          if(zf_perkState[thisSur] == 1)
          {
            zf_perkState[thisSur] = 0;
            removeAura(thisSur);
            PrintHintText(thisSur, "你的诱饵被识破了.");
          }
        }
        else if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你现在可以放置诱饵.");
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(诱饵准备就绪)" : ""));         
    }
    
    // Handle Perk (S): STASH
    else if(usingSurPerk(thisSur, ZF_PERK_STASH))
    {
      // Handle pickup logic.
      if(validItem(thisSur, 0))
      {
        if(zf_perkTimer[thisSur] <= STASH_COOLDOWN)
        {
          decl Float:stashPos[3];          
          getEntityPos(zf_item[thisSur][0], stashPos);
          
          if(GetVectorDistance(validSursPos[i], stashPos, true) <= STASH_GRAB_RADSQ)
          {
            addResAmmoPct(thisSur, 0, 1.0);
            addResAmmoPct(thisSur, 1, 1.0);
            addMetal(thisSur, 200);
            addHealth(thisSur, STASH_GRAB_HEALTH, true);            
            addStatTempStack(thisSur, ZFStatAtt, STASH_GRAB_ATTACK_TEMP, STASH_GRAB_ATTACK_DURATION);
            addStat(thisSur, ZFStatAtt, ZFStatTypePerm, STASH_GRAB_ATTACK_PERM);
            
            fxEvilLaughToAll(thisSur);
            fxCreateSoundToClient(ZFSND_PACKPICKUP, thisSur);            
            removeItem(thisSur, 0);
          }
        }
      }   
      
      // Handle timer logic.
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == STASH_COOLDOWN)
        {
          PrintHintText(thisSur, "你的藏身处准备好了.");
        }
        else if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你可以放置新的藏身处了.别离大家太近了!");
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(藏身处准备就绪)" : ""));        
    }
            
    // Handle Perk (S): STIR-CRAZY
    else if(usingSurPerk(thisSur, ZF_PERK_STIRCRAZY))
    {
      // Update rotating position buffer.
      GetClientAbsOrigin(thisSur, zf_perkPos[thisSur][zf_frameCounter % ZF_STIRCRAZY_MAX_POINTS]);
      
      // Calculate average position of previous player positions.
      new Float:avgPos[3] = {0.0, 0.0, 0.0};
      for(j = 0; j < ZF_STIRCRAZY_MAX_POINTS; j++)        
        AddVectors(avgPos, zf_perkPos[thisSur][j], avgPos);
      ScaleVector(avgPos, (1 / float(ZF_STIRCRAZY_MAX_POINTS)));
   
      // Calculate distance between current position and average position.
      // Note: Max distance (when running in a straight line is ~600).
      new dist = RoundToCeil(GetVectorDistance(avgPos, zf_perkPos[thisSur][zf_frameCounter % ZF_STIRCRAZY_MAX_POINTS], false));
      if(dist >= ZF_STIRCRAZY_DIST_MIN)
      {
        new Float:factor = fMin(1.0, dist / float(ZF_STIRCRAZY_DIST_MAX));
        addStat(thisSur, ZFStatAtt, ZFStatTypeCond, RoundToCeil(ZF_STIRCRAZY_ATTACK * factor));
      }
    }
    
    // Handle Perk (S): SUPPLIER
    else if(usingSurPerk(thisSur, ZF_PERK_SUPPLIER))
    {
      // Handle timer logic.
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你可以放置新的补给箱了.");
        }
      }
      
      // Handle self-resupply logic.
      if((zf_frameCounter % ZF_SUPPLIER_UPDATERATE) == 0)
      {
        addResAmmoPct(thisSur, 0, ZF_SUPPLIER_AMMOPCT_SELF, ZF_SUPPLIER_AMMOPCT_RESLIMIT);
        addResAmmoPct(thisSur, 1, ZF_SUPPLIER_AMMOPCT_SELF, ZF_SUPPLIER_AMMOPCT_RESLIMIT);
        addMetalPct(thisSur, ZF_SUPPLIER_AMMOPCT_SELF, ZF_SUPPLIER_AMMOPCT_RESLIMIT);
      }
      
      // Handle resupply logic.
      decl resupplyCount;
      decl Float:supplyPos[3];
      for(j = 0; j < ZF_SUPPLIER_MAX_ITEMS; j++)
      {        
        if(validItem(thisSur, j))
        {
          // Get resupply count.
          resupplyCount = getItemMetadata(zf_item[thisSur][j]);
          
          getEntityPos(zf_item[thisSur][j], supplyPos);
          for(k = 0; k < validSurCount; k++)
          {
            if(i == k) continue;
            if(GetVectorDistance(supplyPos, validSursPos[k], true) <= ZF_SUPPLIER_RADIUSSQ)
            {
              if((getResAmmoPct(validSurs[k], 0) < 1.0) ||
                 (getResAmmoPct(validSurs[k], 1) < 1.0))
              {
                addResAmmoPct(validSurs[k], 0, ZF_SUPPLIER_AMMOPCT_OTHER);
                addResAmmoPct(validSurs[k], 1, ZF_SUPPLIER_AMMOPCT_OTHER);
                addMetalPct(validSurs[k], ZF_SUPPLIER_AMMOPCT_OTHER);
                addStatTempStack(thisSur, ZFStatAtt, ZF_SUPPLIER_ATTACK, ZF_SUPPLIER_DURATION);
                fxCreateSoundToClient(ZFSND_PACKPICKUP, validSurs[k]);
                
                resupplyCount--;
                if(resupplyCount == 0) break;
              }
            }
          }
          
          // Set resupply count.
          setItemMetadata(zf_item[thisSur][j], resupplyCount);
                      
          if(resupplyCount == 0)
            removeItem(thisSur, j);
        }
      }
      
      // Handle HUD string.
      if(getFreeItemIndex(thisSur, ZF_SUPPLIER_MAX_ITEMS) >= 0)
        Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(补给箱准备就绪)" : ""));
      else
        Format(zf_perkStr[thisSur], 32, "(补给箱已达上限)");
    }
    
    // Handle Perk (S): TANTRUM
    else if(usingSurPerk(thisSur, ZF_PERK_TANTRUM))
    {
      // Handle timer logic.      
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;  
        if(zf_perkTimer[thisSur] == 0)
        {
          PrintHintText(thisSur, "你现在一点就炸!");
        }
        else if(zf_perkTimer[thisSur] == ZF_TANTRUM_COOLDOWN)
        {      
          fxKritzStop(thisSur);
          PrintHintText(thisSur, "你进入疲惫状态了!最好是躲着点僵尸!");
        }
      }
      
      // Handle state logic.
      if(zf_perkTimer[thisSur] > ZF_TANTRUM_COOLDOWN) // Tantrum active
      {
         addCondKritz(thisSur, float(zf_perkTimer[thisSur] - ZF_TANTRUM_COOLDOWN));
      }
      else if(zf_perkTimer[thisSur] > 0) // Tantrum cooldown
      {
        addStat(thisSur, ZFStatSpeed, ZFStatTypeCond, ZF_TANTRUM_SPEED);
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(愤怒状态准备就绪)" : ""));
    }
            
    // Handle Perk (S): TRAPPER
    else if(usingSurPerk(thisSur, ZF_PERK_TRAPPER))
    {
      // Handle mine detection and detonation.
      for(j = 0; j < ZF_TRAPPER_MAX_ITEMS; j++)
      {
        if(validItem(thisSur, j))
        {        
          decl bool:mineDisabled;
          decl bool:mineTriggered;
          decl mineTarget;
          decl Float:minePos[3]; 
          
          // Determine whether mine has detonated or is disabled.
          mineDisabled = false;
          mineTriggered = false;
          getEntityPos(zf_item[thisSur][j], minePos);
          for(k = 0; k < validZomCount; k++)
          {        
            if(GetVectorDistance(minePos, validZomsPos[k], true) <= ZF_TRAPPER_RADIUSSQ)
            {
              if(usingZomPerk(validZoms[k], ZF_PERK_MAGNETIC))
              {
                mineDisabled = true;
              }
              else
              {
                mineTriggered = true;
                mineTarget = validZoms[k];
              }
              break;
            }   
          }
          
          // Detonate or disable mine.
          if(mineTriggered && !mineDisabled)
          {
            TF2_IgnitePlayer(mineTarget, thisSur);
            minePos[2] += 8.0;
            applyDamageRadial(thisSur, ZF_TRAPPER_DAMAGE, minePos, ZF_TRAPPER_RADIUS, true);  
            fxExplosionBig(zf_item[thisSur][j]);
            removeItem(thisSur, j);          
          }
          else if(mineDisabled)
          {
            fxSpark(zf_item[thisSur][j]);
          }
          else
          {
            fxCreateSoundToAll(ZFSND_TICK, zf_item[thisSur][j]);
          }
        }
      }
      
      // Handle timer
      if(zf_perkTimer[thisSur] > 0)
      {
        zf_perkTimer[thisSur]--;
        if(zf_perkTimer[thisSur] == 0)
          PrintHintText(thisSur, "你可以放置另一个地雷了.");
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisSur], 32, (zf_perkTimer[thisSur] == 0 ? "(地雷准备就绪)" : ""));        
    }
    
    // Handle Perk (S): ZENLIKE
    else if(usingSurPerk(thisSur, ZF_PERK_ZENLIKE))
    { 
      if(isGrounded(thisSur) && isCrouching(thisSur) && isNotMoving(thisSur))
      {
        zf_perkState[thisSur] += ZF_ZENLIKE_CRIT_INC;
        if(zf_perkState[thisSur] > 100) 
          zf_perkState[thisSur] = 100;
          
        addHealth(thisSur, ZF_ZENLIKE_REGEN);
        subCond(thisSur, ZFCondPoisoned, ZF_ZENLIKE_HEAL);        
      }
      addStat(thisSur, ZFStatCrit, ZFStatTypeCond, zf_perkState[thisSur]);
    }
  }
   
  // 4. Apply conditional bonuses for zombies.
  for(i = 0; i < validZomCount; i++)
  {
    new thisZom = validZoms[i];
         
   // Handle Perk (Z): COMBUSTIBLE
    else if(usingZomPerk(thisZom, ZF_PERK_COMBUSTIBLE))
    {
      fxExplosionTiny(thisZom);
    }

    // Handle Perk (Z): HUNTER
    else if(usingZomPerk(thisZom, ZF_PERK_HUNTER))
    {
      Format(zf_perkStr[thisZom], 32, (validAura(thisZom) ? "(重生点已激活)" : "(重生点未激活)"));              
    }
    
    // Handle Perk (Z): LEAP
    else if(usingZomPerk(thisZom, ZF_PERK_LEAP))
    {
      // Handle timer.
      if(zf_perkTimer[thisZom] > 0)
      {
        zf_perkTimer[thisZom]--;
        if(zf_perkTimer[thisZom] == 0)
          PrintHintText(thisZom, "飞跃准备就绪!");
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisZom], 32, (zf_perkTimer[thisZom] == 0 ? "(飞跃准备就绪)" : ""));         
    }
    
    // Handle Perk (Z): MARKED
    else if(usingZomPerk(thisZom, ZF_PERK_MARKED))
    {
      //
      // No mark exists.
      // Do not attempt to select a mark.
      //
      if(zf_perkState[thisZom] == -1)
      {
        // Do nothing.
      }
      //
      // No mark exists. 
      // Wait until timer reaches zero before selecting new mark.
      //
      else if(zf_perkState[thisZom] == 0)
      {
        if(zf_perkTimer[thisZom] > 0)
        {
          zf_perkTimer[thisZom]--;
        }
        else
        {
          doMarkedSelect(thisZom);
        }
      }
      //
      // Mark exists. 
      // Monitor mark until he dies, then initiate timer.
      //
      else
      {
        if(!validLivingSur(zf_perkState[thisZom]))
        {            
          zf_perkState[thisZom] = 0;
          zf_perkTimer[thisZom] = ZF_MARKED_TIMER;
          removeIcon(thisZom);
          PrintHintText(thisZom, "标记新的幸存者中...");
        }
      }
    }
  
    // Handle Perk (Z): RAGE
    else if(usingZomPerk(thisZom, ZF_PERK_RAGE))
    {
      // Handle cooldown when rage is not active.
      if(zf_perkState[thisZom] == 0)
      {
        if(zf_perkTimer[thisZom] > 0)
        {
          zf_perkTimer[thisZom]--;
          if(zf_perkTimer[thisZom] == 0)
            PrintHintText(thisZom, "你现在很想把人类大卸八块!");
        }
      }
      // Handle logic when rage is active.
      else
      {
        if(getHealthPct(thisZom) >= ZF_RAGE_HEALTHPCT_TOUSE)
        {
          addStat(thisZom, ZFStatSpeed, ZFStatTypeCond, ZF_RAGE_SPEED);
        }
        else
        {
          zf_perkState[thisZom] = 0;
          removeAura(thisZom);
          PrintHintText(thisZom, "你不再渴望鲜血......暂时.");
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisZom], 32, (zf_perkTimer[thisZom] == 0 ? "(愤怒准备就绪)" : ""));      
    }
    
    // Handle Perk (Z): ROAR
    else if(usingZomPerk(thisZom, ZF_PERK_ROAR))
    {
      if(zf_perkTimer[thisZom] > 0)
      {
        zf_perkTimer[thisZom]--;
        if(zf_perkTimer[thisZom] == 0)
          PrintHintText(thisZom, "咆哮准备就绪!");
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisZom], 32, (zf_perkTimer[thisZom] == 0 ? "(咆哮准备就绪)" : ""));   
    }    
    
    // Handle Perk (Z): SCORCHING
    else if(usingZomPerk(thisZom, ZF_PERK_SCORCHING))
    {    
      if(!isInWater(thisZom))
        TF2_IgnitePlayer(thisZom, thisZom);
    }
    
    // Handle Perk (Z): SICK
    else if(usingZomPerk(thisZom, ZF_PERK_SICK))
    {
      // Handle timer events
      if(zf_perkTimer[thisZom] > 0)
      {
        zf_perkTimer[thisZom]--;
        if(zf_perkTimer[thisZom] == 0)
        {
          removeItems(thisZom);
          PrintHintText(thisZom, "你现在可以吐出酸液了.");
        }
      }
            
      // Handle bonus logic
      for(j = 0; j < ZF_SICK_MAX_ITEMS; j++)
      {
        if(validItem(thisZom, j))
        {
          if(getItemMetadata(zf_item[thisZom][j]) == 1)
          {
            applyDamageRadial(thisZom, ZF_SICK_DAMAGE, zf_perkPos[thisZom][j], ZF_SICK_DAMAGE_RADIUS);
          }
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisZom], 32, (zf_perkTimer[thisZom] == 0 ? "(酸液准备就绪)" : ""));      
    }

    // Handle Perk (Z): TARRED
    else if(usingZomPerk(thisZom, ZF_PERK_TARRED))
    {
      // Handle timer events
      if(zf_perkTimer[thisZom] > 0)
      {
        zf_perkTimer[thisZom]--;
        if(zf_perkTimer[thisZom] == 0)
        {
          removeItems(thisZom);
          PrintHintText(thisZom, "焦油准备就绪!");
        }
      }
            
      // Handle bonus logic
      for(j = 0; j < ZF_TARRED_MAX_ITEMS; j++)
      {
        if(validItem(thisZom, j))
        {
          if(getItemMetadata(zf_item[thisZom][j]) == 1)
          {
            applyDamageRadial(thisZom, 1, zf_perkPos[thisZom][j], ZF_TARRED_RADIUS);
          }
        }
      }
      
      // Handle HUD string.
      Format(zf_perkStr[thisZom], 32, (zf_perkTimer[thisZom] == 0 ? "(焦油准备就绪)" : ""));  
      
      // Render color.
      fxSetClientColor(thisZom, 25, 25, 25, 255);
    }
     
    // Handle Perk (Z): TOXIC
    else if(usingZomPerk(thisZom, ZF_PERK_TOXIC))
    {
      // Apply poison to nearby survivors.
      if(isNotMoving(thisZom) && !isCloaked(thisZom))
      {
        for(j = 0; j < validSurCount; j++)
        {
          if(isUbered(validSurs[j])) continue;
          if(GetVectorDistance(validZomsPos[i], validSursPos[j], true) <= ZF_TOXIC_RADIUSSQ)
          {
            SDKHooks_TakeDamage(validSurs[j], thisZom, thisZom, float(ZF_TOXIC_DAMAGE_PASSIVE), ZF_DMGTYPE_POISON); 
            zf_lastPoison[validSurs[j]] = thisZom;
          }
        }        
      }
      
      // Render color.
      fxSetClientColor(thisZom, 50, 200, 50, 255);
    }
    
    // Handle Perk (Z): VAMPIRIC
    else if(usingZomPerk(thisZom, ZF_PERK_VAMPIRIC))
    {
      addHealth(thisZom, ZF_VAMPIRIC_REGEN);
    }       
    
    //
    // Handle general zombie logic.
    //
    
    // Hide auras on cloaked zombies.
    isCloaked(thisZom) ? hideAura(thisZom) : showAura(thisZom);
  }

  // Handle Perk (Z): HUNTER
  // * Destroy spawn marks near survivors
  for(i = 1; i <= MaxClients; i++)
  {
    if(validZom(i) && usingZomPerk(i, ZF_PERK_HUNTER) && validAura(i))
    {
      decl Float:spawnPos[3];
      getEntityPos(zf_aura[i], spawnPos);
        
      for(j = 0; j < validSurCount; j++)
      {
        if(GetVectorDistance(spawnPos, validSursPos[j], true) <= ZF_HUNTER_RADIUSSQ)
        {
          PrintHintText(i, "你的重生点被破坏了!");
          PrintHintText(validSurs[j], "你破坏了猎手僵尸的重生点!");
          removeAura(i);
          break;
        }
      }
    }
  }
      
  // Handle Perk (Z): MAGNETIC
  // * Disable all sentries that are near a zombie using the MAGNETIC perk.
  new index = -1;
  while ((index = FindEntityByClassname(index, "obj_sentrygun")) != -1)
  {  
    decl Float:sentPos[3];
    getEntityPos(index, sentPos);
    
    // Determine if sentry is disabled.
    new bool:sentryDisabled = false;
    for(i = 0; i < validZomCount; i++)
    {
      if(usingZomPerk(validZoms[i], ZF_PERK_MAGNETIC)) 
      {
        if(GetVectorDistance(sentPos, validZomsPos[i], true) <= ZF_MAGNETIC_RADIUSSQ)
        {
          sentryDisabled = true;
          break;
        }
      }
    }

    // Handle sentry disable logic.
    if(sentryDisabled)
    {
      SetEntProp(index, Prop_Send, "m_bDisabled", 1);
      fxSpark(index);
    }
    else
    {
      SetEntProp(index, Prop_Send, "m_bDisabled", 0);
    }
  }  
}

stock updateTempStats()
{  
  for(new i = 0; i < MAXPLAYERS; i++)
  {
    for(new j = 0; j < TOTAL_ZFSTATS; j++)
    {
      // Clear temp bonus if duration reaches zero.
      zf_stat[i][j][ZFStatTypeTempDuration]--;
      if(zf_stat[i][j][ZFStatTypeTempDuration] <= 0)
      {
        zf_stat[i][j][ZFStatTypeTemp] = 0;
        zf_stat[i][j][ZFStatTypeTempDuration] = 0;
      }
    }
  }
}

stock updateConds()
{
  for(new i = 0; i < MAXPLAYERS; i++)
  {
    for(new j = 0; j < TOTAL_ZFCONDS; j++)
    {
      if(zf_cond[i][j] > 0)
        zf_cond[i][j]--;
    }
  }
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Timing / Round State Events
//
////////////////////////////////////////////////////////////
public perk_OnPeriodic() // 1Hz
{
  zf_frameCounter++;

  //
  // Build common state information for periodic updates.
  //
  decl validSurCount, validZomCount;
  decl validSurs[MAXPLAYERS+1], validZoms[MAXPLAYERS+1];
  decl Float:validSursPos[MAXPLAYERS+1][3], Float:validZomsPos[MAXPLAYERS+1][3];
  
  validSurCount = 0;
  validZomCount = 0;
  for(new i = 1; i <= MaxClients; i++)
  {
    if(IsClientInGame(i) && IsPlayerAlive(i))
    {
      if(isSur(i))
      {
        validSurs[validSurCount] = i;
        GetClientAbsOrigin(i, validSursPos[validSurCount]);
        validSurCount++;
      }
      else if(isZom(i))
      {
        validZoms[validZomCount] = i;
        GetClientAbsOrigin(i, validZomsPos[validZomCount]);
        validZomCount++;
      }
    }
  }

  //
  // Call new object-oriented periodic updates
  //
  for(new i = 1; i <= MaxClients; i++)
  {
    if(g_hPerks[i] != null && IsClientInGame(i) && IsPlayerAlive(i))
    {
      g_hPerks[i].onPeriodicUpdate(validSurs, validSurCount, validSursPos, validZoms, validZomCount, validZomsPos, zf_perkAlphaMaster);
    }
  }

  //
  // Legacy updates for non-refactored perks
  //
  updateConds();
  updateTempStats();
  updateCondStats(); // This now only handles legacy perks
    
  //
  // Legacy condition application
  //
  for(new i = 1; i <= MaxClients; i++)
  {
    if(IsClientInGame(i) && IsPlayerAlive(i))
    {
      if(getCond(i, ZFCondCrippled))
      {
        if(GetRandomInt(0,3) == 1) fxBloodBurst(i);
        if(GetRandomInt(0,3) == 1) fxBloodSpray(i);
        if(GetRandomInt(0,3) == 1) fxPain(i);
      }
      if(getCond(i, ZFCondIntimidated))
      {
        addStat(i, ZFStatDef, ZFStatTypeCond, ZF_DAZE_DEFEND);
      }
      if(getCond(i, ZFCondPoisoned))
      {
        if(isBeingHealed(i)) subCond(i, ZFCondPoisoned, ZF_POISON_HEAL_MEDIC);
        if(isUbered(i)) subCond(i, ZFCondPoisoned, ZF_POISON_HEAL_UBER);
        SDKHooks_TakeDamage(i, zf_lastPoison[i], zf_lastPoison[i], float(ZF_POISON_DAMAGE), ZF_DMGTYPE_POISON);
        fxHealthLost(i);
      }
      setGlow(i, getCond(i, ZFCondTracked));
    }
  }
  
  //
  // Handle HUD updates.
  //
  if(roundState() != RoundPost)
  {
    for(new i = 1; i <= MaxClients; i++)
    {
      updateHud(i);
    }
  }
}

public perk_OnGameFrame()
{
  decl Float:collidePos[3];
  decl Float:collideVec[3];
  
  for(new i = 1; i <= MaxClients; i++)
  {
    if(IsClientInGame(i) && IsPlayerAlive(i))
    {
      //
      // Handle Condition: Crippled
      // + Prevent health gain.
      //
      if(getCond(i, ZFCondCrippled))
      {
        zf_lastHealth[i] = min(GetClientHealth(i), zf_lastHealth[i]);
        zf_lastHealth[i] = max(1, zf_lastHealth[i]);
        SetEntityHealth(i, zf_lastHealth[i]);
      }
      
      //
      // Handle rate of fire calculations.
      // 
      if(zf_lastAttack[i] == 1)
      {
        zf_lastAttack[i] = 0;
        if((getStat(i, ZFStatRof) != 0) && !isWieldingAuto(i))
        {
          setWeaponRof(activeWeapon(i), getStat(i, ZFStatRof));
        }        
      } 
    
      //
      // Handle zombie logic.
      //
      if(isZom(i))
      {
        // Handle Perk (Z): SICK
        if(usingZomPerk(i, ZF_PERK_SICK))
        {
          for(new j = 0; j < ZF_SICK_MAX_ITEMS; j++)
          {
            if(validItem(i, j))
            {
              // Check for collision for projectiles only.
              if(getItemMetadata(zf_item[i][j]) == 0)
              {
                if(doItemCollide(zf_item[i][j], zf_perkPos[i][j], collidePos, collideVec))
                {
                  removeItem(i, j);
                  zf_item[i][j] = doItemImpact(i, collidePos, collideVec, {100, 200, 100});
                  setItemMetadata(zf_item[i][j], 1);
                  fxBits(zf_item[i][j]);
                }

                getEntityPos(zf_item[i][j], zf_perkPos[i][j]);
              }
            }
          }
        }

        // Handle Perk (Z): TARRED
        else if(usingZomPerk(i, ZF_PERK_TARRED))
        {
          for(new j = 0; j < ZF_TARRED_MAX_ITEMS; j++)
          {
            if(validItem(i, j))
            {
              // Check for collision for projectiles only.
              if(getItemMetadata(zf_item[i][j]) == 0)
              {
                if(doItemCollide(zf_item[i][j], zf_perkPos[i][j], collidePos, collideVec))
                {
                  removeItem(i, j);
                  zf_item[i][j] = doItemImpact(i, collidePos, collideVec, {25, 25, 25});
                  setItemMetadata(zf_item[i][j], 1);
                  fxBits(zf_item[i][j]);
                }
                
                getEntityPos(zf_item[i][j], zf_perkPos[i][j]);
              }
            }
          }
        }
        
        // Handle Perk (Z): THIEVING
        else if(usingZomPerk(i, ZF_PERK_THIEVING))
        {
          if(zf_perkState[i] > 0)
          {
            doThievingLimit(i);
          }
        }
                
      } // isZom(i)
    } // Client in game and alive.
  } // for i
}

public perk_OnMapStart()
{ 
  utilFxPrecache(); 
  resetAllClients();
}

public perk_OnMapEnd()
{
  resetAllClients();
}

public perk_OnClientConnect(client)
{
  resetClient(client);
  if(prefGet(client, PerkSelectMode) > 1)
    prefSet(client, PerkSelectMode, 0);
}

public perk_OnClientDisconnect(client)
{
  resetClient(client);
}

public perk_OnRoundStart()
{
  resetAllClients();
 
  //
  // Handle ZF Mode Selection
  //
  zf_perkMode = zf_perkPendingMode;

  //
  // Reset perk select mode.
  //
  for(new i = 1; i <= MaxClients; i++)
    prefSet(i, PerkSelectMode, 0);
        
  //
  // Select random team perks.
  //
  if(zf_perkMode == 2)
  {
    new validSurPerkCount = 0;
    new validZomPerkCount = 0;   
    decl validSurPerks[TOTAL_SUR_PERKS];
    decl validZomPerks[TOTAL_ZOM_PERKS];
       
    for(new i = 1; i < TOTAL_SUR_PERKS; i++)
      if(surPerkEnabled(i))
        validSurPerks[validSurPerkCount++] = i;
    zf_perkRandSurPerk = (validSurPerkCount == 0) ? ZF_PERK_NONE : validSurPerks[GetRandomInt(0, validSurPerkCount - 1)];

    for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
      if(zomPerkEnabled(i))
        validZomPerks[validZomPerkCount++] = i;
    zf_perkRandZomPerk = (validZomPerkCount == 0) ? ZF_PERK_NONE : validZomPerks[GetRandomInt(0, validZomPerkCount - 1)];
  }
}

public perk_OnGraceEnd()
{
  for(new i = 1; i <= MaxClients; i++)
  {
    //
    // Handle survivor logic. 
    // Survivors must be alive, otherwise they will respawn as a zombie.
    //
    if(validLivingSur(i))
    {
      // Handle Perk (S): FRIEND
      // + Select random friend unless friend al准备好了 selected.
      if(usingSurPerk(i, ZF_PERK_FRIEND))
      {
        if(zf_perkState[i] == 0)
          doFriendSelect(i);
      }
    }
    
    //
    // Handle zombie logic.
    //
    else if(validZom(i))
    {
      // Handle Perk (Z): MARKED
      if(usingZomPerk(i, ZF_PERK_MARKED))
        doMarkedSelect(i);
    }
  }
}

public perk_OnRoundEnd()
{    
  resetAllClients();
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Entity Events
//
////////////////////////////////////////////////////////////
public perk_OnEntityCreated(entity, const String:classname[])
{
  if(StrEqual(classname, "env_sprite"))
    SDKHook(entity, SDKHook_Spawn, perk_OnEntitySpawn);
}

public perk_OnEntitySpawn(entity)
{
  // TODO consider use of m_hEffectEntity for sprites to further filter?
  SDKHook(entity, SDKHook_SetTransmit, perk_OnSetTransmit);
}

public Action:perk_OnSetTransmit(entity, client)
{
  if(isSur(client))
  {
    // Handle Perk (S): FRIEND
    // Show entity to client iff this is client's friend icon.
    // (This icon appears over client's friend)
    if(usingSurPerk(client, ZF_PERK_FRIEND))
    {
      if(zf_icon[client][ICON_SPR] == entity)
        return Plugin_Continue;
    }
  }
  else if(isZom(client))
  {
    // Handle Perk (Z): MARKED
    // Show entity to client iff this is client's marked icon.
    // (This icon appears over client's mark)    
    if(usingZomPerk(client, ZF_PERK_MARKED))
    {
      if(zf_icon[client][ICON_SPR] == entity)
        return Plugin_Continue;
    }
  }
  return Plugin_Handled;
}

public perk_OnCharitableGiftTouched(entity, other)
{
  static String:zf_statStr[ZFStat][16] = { "Attack", "Crit", "Defense", "Rate of Fire", "Speed" };
  
  //
  // Determine gift owner.
  //
  new giftOwner = -1;  
  new giftIndex = -1;
  for(new i = 1; i <= MaxClients; i++)
  {
    for(new j = 0; j < ZF_CHARITABLE_MAX_ITEMS; j++)
      if(zf_item[i][j] == entity)
      {
        giftOwner = i;
        giftIndex = j;
      }
  }
   
  // 
  // Handle gift bonus generation and pickup.
  // + Gift owner can't pick up gift.
  // + Other survivors receive stat bonus.
  //
  if(validLivingClient(other))
  {
    if(other == giftOwner) return;
      
    // Calculate gift bonus.
    new randStat = GetRandomInt(0, (TOTAL_ZFSTATS - 1));
    new randBonus = GetRandomInt(ZF_CHARITABLE_GIFT_BONUS_MIN, ZF_CHARITABLE_GIFT_BONUS_MAX);
    
    // Apply gift bonus.
    if(isSur(other))
    {
      PrintHintText(other, "你获得了 %s 的加成!", zf_statStr[ZFStat:randStat], randBonus);    
      addStatTempStack(other, ZFStat:randStat, randBonus, ZF_CHARITABLE_GIFT_DURATION);    
      addHealth(giftOwner, ZF_CHARITABLE_GIFT_BONUS_HEALTH, true);
    }
    
    fxExplosionParty(entity);     
    removeItem(giftOwner, giftIndex);
  }
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Player Gameplay Events
//
////////////////////////////////////////////////////////////
public Action:OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon) 
{ 
  new prevButtons = buttons;
  
  if(isSur(client))
  {
    // Handle Perk (S): NINJA
    if(usingSurPerk(client, ZF_PERK_NINJA))
    {
      if((buttons & IN_JUMP) && !(zf_lastButtons[client] & IN_JUMP))
      {
        if(isGrounded(client) && !isDazed(client))
        {
          fxJump(client, ZF_NINJA_FORCE, true); 
          fxPuffSmall(client);                  
        }
      }
    }
  }
  else if(isZom(client))
  {
    // Handle Perk (Z): COMBUSTIBLE
    if(usingZomPerk(client, ZF_PERK_COMBUSTIBLE))
    {
      if(isScout(client) && (buttons & IN_ATTACK) && isWielding(client, ZFWEAP_BONK))
      {
        buttons &= ~IN_ATTACK;
        if(!(zf_lastButtons[client] & IN_ATTACK))
          PrintHintText(client, "当扮演自爆僵尸时,不可以喝原子能饮料! .");       
      }
      
      if(isSpy(client) && (buttons & IN_ATTACK2))
      {
        buttons &= ~IN_ATTACK2;
        if(!(zf_lastButtons[client] & IN_ATTACK2))
          PrintHintText(client, "当扮演自爆僵尸时,不可以伪装.");
      }
    }
    
    // Handle Perk (Z): SCORCHING
    else if(usingZomPerk(client, ZF_PERK_SCORCHING))
    {
      if(isScout(client) && (buttons & IN_ATTACK) && isWielding(client, ZFWEAP_BONK))
      {
        buttons &= ~IN_ATTACK;
        if(!(zf_lastButtons[client] & IN_ATTACK))
          PrintHintText(client, "当扮演火焰僵尸时,不可以喝原子能饮料."); 
      }
    }    
  }
  
  // Save buttons for next call.
  zf_lastButtons[client] = prevButtons;
  return Plugin_Continue;
}

public perk_OnCalcIsAttackCritical(client)
{
  // DEBUG
  //PrintToChatAll("[ZF] CalcCrit (%d, slot %d)", client, activeWeaponSlot(client));
  
  if(validLivingSur(client))
  {
    // Handle Perk (S): ZENLIKE
    if(usingSurPerk(client, ZF_PERK_ZENLIKE))
    {
      CreateTimer(0.1, perk_tZenlikeAttack, client, TIMER_FLAG_NO_MAPCHANGE);
    }
  }
  
  // Handle general RoF logic.
  zf_lastAttack[client] = 1;
}

public Action:perk_OnFenceTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
//    // DEBUG
//    decl String:inflictorClass[32];
//    GetEdictClassname(inflictor, inflictorClass, sizeof(inflictorClass));
//    PrintToChatAll("\x05[ZF DEBUG]\x01 Vic %d, Att %d, Inf %d (%s), Dmg %f, DTp %x", victim, attacker, inflictor, inflictorClass, damage, damagetype);

  new fenceOwner = -1;
  new fenceIndex = -1;
  new fenceHP;
  new fenceFx;
  new Float:off[3];
  
  //
  // Determine fence owner and index.
  //
  for(new i = 1; i <= MaxClients; i++)
  {
    for(new j = 0; j < CARPENTER_MAX_ITEMS; j++)
    {
      if(victim == zf_item[i][j])
      {
        fenceOwner = i;
        fenceIndex = j;
        fenceHP = getItemMetadata(zf_item[i][j]);
        fenceFx = (fenceHP * 255) / CARPENTER_BARRICADE_HEALTH;
      }
    }
  }
  
  //
  // Adjust fence health.
  //
  if(fenceOwner != -1)
  {
    fenceHP -= RoundToCeil(damage);
    if(fenceHP <= 0)
    {
      // Barricade destruction effects
      off[2] -= 120.0; // Adjust for ZFMDL_FENCE
      fxCreateParticle(ZFPART_PUFFBIG, victim, AttachNone, 4.0, off); 
      fxCreateSoundToAll(ZFSND_WOOD_HIT[GetRandomInt(2,3)], victim);
        
      removeItem(fenceOwner, fenceIndex);
    }
    else
    {     
      setItemMetadata(zf_item[fenceOwner][fenceIndex], fenceHP);
      SetEntityRenderMode(zf_item[fenceOwner][fenceIndex], RENDER_TRANSCOLOR);
      SetEntityRenderColor(zf_item[fenceOwner][fenceIndex], 255, fenceFx, fenceFx, 255); 
    }
  }
  else
  {
    // Remove fence.
    LogError("[ZF] - perk_OnFenceTakeDamage() - Invalid owner of fence (%d)", victim);
    AcceptEntityInput(victim, "Kill");
  }
}

public Action:perk_OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{ 
//   // DEBUG
//   decl String:inflictorClass[32];
//   GetEdictClassname(inflictor, inflictorClass, sizeof(inflictorClass));
//   PrintToChatAll("\x05[ZF DEBUG]\x01 Vic %d, Att %d, Inf %d (%s), Dmg %f, DTp %x", victim, attacker, inflictor, inflictorClass, damage, damagetype);

  new localAttAdjust = 0;
  new localDefAdjust = 0;
  
  //
  // Ignore perk damage events for ubered victims.
  // 
  if(validClient(victim) && isUbered(victim))
    return Plugin_Continue;

  //
  // Attribute poison damage.
  //
  if(damagetype & ZF_DMGTYPE_POISON)
  {
    if(validClient(victim) && (validSur(zf_lastPoison[victim]) || validZom(zf_lastPoison[victim])))
    {
      attacker = zf_lastPoison[victim];      
    }
    return Plugin_Changed;
  }
   
  //
  // Calculate attack (from attacker) and defense (from victim) damage adjustments.
  //
  if(validLivingClient(attacker) && (victim != attacker))
    localAttAdjust = getStat(attacker, ZFStatAtt);
  if(validLivingClient(victim) && (victim != attacker))
    localDefAdjust = getStat(victim, ZFStatDef);
  
  //
  // Reduce sentry attack bonuses by half.
  //
  if(entIsSentry(inflictor))
    localAttAdjust /= 2;
    
  //
  // Survivor is taking damage from zombie.
  //
  if(validSur(victim) && validZom(attacker))
  {
    //
    // Melee attacks.    
    //
    if(attackWasMelee(attacker, inflictor, damagetype))
    {
      // Handle Perk (S): COWARDLY
      // + Scare victim on melee hit.
      if(usingSurPerk(victim, ZF_PERK_COWARDLY))
      {
        if(zf_perkTimer[victim] == 0)
        {
          zf_perkTimer[victim] = ZF_COWARDLY_DURATION_SCARED + ZF_COWARDLY_DURATION_COOLDOWN;
  
          addStat(victim, ZFStatDef, ZFStatTypeCond, ZF_COWARDLY_DEFEND);
          addStat(victim, ZFStatSpeed, ZFStatTypeCond, ZF_COWARDLY_SPEED);
  
          PrintHintText(victim, "你被惊吓僵尸吓住了!你的属性暂时下降!");
          fxYikes(victim);
          
          damage = 0.0;
        }          
      }
      // Handle Perk (S): NINJA
      // + Activate decoy on melee hit.
      else if(usingSurPerk(victim, ZF_PERK_NINJA))
      {
        if(zf_perkState[victim] == 1)
        {
          zf_perkState[victim] = 0;
          CreateTimer(float(ZF_NINJA_DURATION_DECOY_DECAY), perk_tNinjaDecoyPoof, victim, TIMER_FLAG_NO_MAPCHANGE);
  
          doNinjaDecoyPlace(victim);        
          fxCreateSoundToClient(ZFSND_BOING, victim);
          fxCreateSoundToClient(ZFSND_BOING, attacker);          
          
          PrintHintText(victim, "僵尸们攻击的是你的诱饵,你脱身了.");
          PrintHintText(attacker, "你在打的是一块木板,笨蛋!");
          
          damage = 0.0;      
        }
      }
      // Handle Perk (S): TURTLE
      // + Block backstabs.
      else if(usingSurPerk(victim, ZF_PERK_TURTLE))
      {
        if(attackWasBackstab(attacker, inflictor, damagetype))
        {
          TF2_StunPlayer(attacker, ZF_TURTLE_STUN_DURATION, 0.0, TF_STUNFLAGS_NORMALBONK, victim);        
          PrintHintText(attacker, "你的背刺被防住了!");
          
          fxSpark(victim);   
          PrintHintText(victim, "你防住了一次背刺!");
     
          damage = 0.0;
        } 
      }
      // Handle Perk (S): WISE
      // + Increase defense with each melee hit.
      else if(usingSurPerk(victim, ZF_PERK_WISE))
      {
        if(getStatType(victim, ZFStatDef, ZFStatTypePerm) < ZF_WISE_DEFEND_LIMIT)
          addStat(victim, ZFStatDef, ZFStatTypePerm, ZF_WISE_DEFEND);
      }

      // Handle Perk (Z): HORRIFYING
      // + Apply penalties with each melee hit.
      if(usingZomPerk(attacker, ZF_PERK_HORRIFYING))
      {
        if(isHeavy(attacker))
        {
          addStatTempStack(victim, ZFStatAtt, HORRIFYING_ATTACK_HEAVY, HORRIFYING_DURATION_HEAVY);
          addStatTempStack(victim, ZFStatDef, HORRIFYING_DEFEND_HEAVY, HORRIFYING_DURATION_HEAVY);
          addStatTempStack(victim, ZFStatRof, HORRIFYING_ROF_HEAVY, HORRIFYING_DURATION_HEAVY);
        }
        else
        {
          addStatTempStack(victim, ZFStatAtt, HORRIFYING_ATTACK, HORRIFYING_DURATION);
          addStatTempStack(victim, ZFStatDef, HORRIFYING_DEFEND, HORRIFYING_DURATION);       
        }
      }
      // Handle Perk (Z): MARKED
      // + Adjust damage on/off mark with each melee hit.
      else if(usingZomPerk(attacker, ZF_PERK_MARKED))
      {
        if(zf_perkState[attacker] == victim)
          localAttAdjust += ZF_MARKED_ATTACK_ON_MARK;
        else
          localAttAdjust += ZF_MARKED_ATTACK_OFF_MARK;
      }
      // Handle Perk (Z): SCORCHING
      // + Ignite victim on melee hit.
      // + Adjust damage for melee attacks.
      else if(usingZomPerk(attacker, ZF_PERK_SCORCHING))
      {
        if(!isPyro(victim))
          TF2_IgnitePlayer(victim, attacker);
        localAttAdjust += ZF_SCORCHING_ATTACK;        
      }
      // Handle Perk (Z): TARRED
      // + Slow victim on melee hit.
      else if(usingZomPerk(attacker, ZF_PERK_TARRED))
      {
        if(getStat(victim, ZFStatSpeed) > ZF_TARRED_SPEED_LIMIT)      
          addStatTempStack(victim, ZFStatSpeed, ZF_TARRED_SPEED_MELEE, ZF_TARRED_DURATION_MELEE);
      }
      // Handle Perk (Z): THIEVING
      // + Steal from victim with each melee hit.
      else if(usingZomPerk(attacker, ZF_PERK_THIEVING))
      {
        new slot = activeWeaponSlot(victim);
        if(slot == 0) // Primary weapon
        {
          subResAmmoPct(victim, slot, ZF_THIEVING_AMMOPCT);
          if(getResAmmoPct(victim, slot) == 0.0)
            doThievingSteal(attacker, victim, slot);
        }
        else if(slot == 1) // Secondary weapon
        {
          subUber(victim, ZF_THIEVING_UBERPCT);
          subResAmmoPct(victim, slot, ZF_THIEVING_AMMOPCT);
        }
        else if(slot == 2) // Melee
        {
          subMetal(victim, ZF_THIEVING_METAL);
        }
  
        // Effects
        fxSpark(victim);       
      }
      // Handle Perk (Z): TOXIC
      // + Poison victim on melee hit.
      else if(usingZomPerk(attacker, ZF_PERK_TOXIC))
      {
        addCond(victim, ZFCondPoisoned, ZF_TOXIC_DURATION_POISON);
        zf_lastPoison[victim] = attacker;
      }
    }
    //
    // Non-melee attacks.
    //      
    else
    {
      // Handle Perk (Z): ROAR
      // + Handle env_explosion damage (i.e. roar).
      if(usingZomPerk(attacker, ZF_PERK_ROAR))
      {
        if(attackWasEnvExplosion(damagetype))
        {     
          decl Float:attPos[3];
          decl Float:vicPos[3];
          GetClientAbsOrigin(attacker, attPos);
          GetClientAbsOrigin(victim, vicPos);
   
          new Float:factor = fMax(0.0, 1.0 - GetVectorDistance(attPos, vicPos, false) / float(ZF_ROAR_RADIUS));        
          new Float:force = factor * (isHeavy(attacker) ? ZF_ROAR_FORCE_HEAVY : ZF_ROAR_FORCE);
          new duration = RoundToCeil(factor * (isHeavy(attacker) ? ZF_ROAR_DURATION_HEAVY : ZF_ROAR_DURATION));
             
          fxKnockback(victim, attacker, force);
          if(!getCond(victim, ZFCondIntimidated))
          {
            addCond(victim, ZFCondIntimidated, duration);
          }
        }
      }
      // Handle Perk (Z): SCORCHING
      // + Negate innate attack penalty on scorching afterburn.
      else if(usingZomPerk(attacker, ZF_PERK_SCORCHING))
      {
        if(!attackWasFire(damagetype))
          localAttAdjust += ZF_SCORCHING_ATTACK; 
      }
      // Handle Perk (Z): TARRED
      // + Handle env_explosion damage (i.e. oil slick proximity).
      else if(usingZomPerk(attacker, ZF_PERK_TARRED))
      {
        if(attackWasEnvExplosion(damagetype))
        {        
          if(getStat(victim, ZFStatSpeed) > ZF_TARRED_SPEED_LIMIT)
            addStatTempStack(victim, ZFStatSpeed, ZF_TARRED_SPEED_SLICK, ZF_TARRED_DURATION_SLICK);
          damage = 0.0;
        }
      }
      // Handle Perk (Z): THIEVING
      // + Negate innate attack penalty on non-melee attacks.
      else if(usingZomPerk(attacker, ZF_PERK_THIEVING))
      {
        localAttAdjust -= ZF_THIEVING_ATTACK;          
      }        
    }
  }

  //
  // Survivor is taking damage from non-zombie.
  //
  else if(validSur(victim) && !validZom(attacker))
  {
    // Handle Perk (S): JUGGERNAUT 
    // + No self damage, no fall damage, handle fall damage impact.
    if(usingSurPerk(victim, ZF_PERK_JUGGERNAUT))
    {
      if(victim == attacker)
        damage = 0.0;
      if(attackWasSelfFall(inflictor, damagetype))
      {
        applyDamageRadialAtClient(victim, 1, ZF_JUGGERNAUT_RADIUS); 
        fxPuffBig(victim);
        damage = 0.0;
      }
    }
    
    // Handle Perk (S): NINJA
    // + Reduced fall damage.
    else if(usingSurPerk(victim, ZF_PERK_NINJA))
    {
      if(attackWasSelfFall(inflictor, damagetype))
      {
        localDefAdjust += ZF_NINJA_FALLDMG_RESIST;
      }        
    }
  }
  
  //
  // Zombie is taking damage from survivor.
  //
  else if(validZom(victim) && validSur(attacker))
  {
    //
    // Melee attacks.
    //
    if(attackWasMelee(attacker, inflictor, damagetype))
    {
      // Handle Perk (S): JUGGERNAUT
      // + Knockback victim with melee hit.
      if(usingSurPerk(attacker, ZF_PERK_JUGGERNAUT))
      {
        fxKnockback(victim, attacker, ZF_JUGGERNAUT_FORCE);       
      }
      // Handle Perk (Z): TARRED
      // + Apply penalties with each melee hit.
      if(usingZomPerk(victim, ZF_PERK_TARRED))
      {
        if(!isUbered(attacker))
        {
          if(getStat(attacker, ZFStatSpeed) > ZF_TARRED_SPEED_LIMIT)
          {
            addStatTempStack(attacker, ZFStatRof, ZF_TARRED_ROF, ZF_TARRED_DURATION_MELEE);
            addStatTempStack(attacker, ZFStatSpeed, ZF_TARRED_SPEED_MELEE, ZF_TARRED_DURATION_MELEE);
          }
        }
      }
      // Handle Perk (Z): TOXIC
      // + Poison attacker who is attacking with melee.
      else if(usingZomPerk(victim, ZF_PERK_TOXIC))
      {
        if(!isUbered(attacker))
        {
          addCond(attacker, ZFCondPoisoned, ZF_TOXIC_DURATION_POISON);
          zf_lastPoison[victim] = attacker;
        }
      }
    }
    //
    // Non-melee attacks.
    //
    else
    {
      // Handle Perk (S): JUGGERNAUT
      // + Handle env_explosion damage (i.e. fall damage impact).
      if(usingSurPerk(attacker, ZF_PERK_JUGGERNAUT))
      {
        if(attackWasEnvExplosion(damagetype))
        {
          TF2_StunPlayer(victim, ZF_JUGGERNAUT_STUN_DURATION, ZF_JUGGERNAUT_STUN_SLOWDOWN, TF_STUNFLAGS_SMALLBONK, attacker);      
          fxKnockback(victim, attacker, ZF_JUGGERNAUT_FORCE);
        }
      }
      // Handle Perk (S): NONLETHAL
      // + Apply innate bullet damage attack penalty.
      else if(usingSurPerk(attacker, ZF_PERK_NONLETHAL))
      {
        if(attackWasBullet(attacker, inflictor))
        {
          localAttAdjust += ZF_NONLETHAL_ATTACK_BULLET;
        }
      }
      
      // Handle Perk (Z): SCORCHING
      // + No fire damage.
      if(usingZomPerk(victim, ZF_PERK_SCORCHING))
      {
        if(!(validSur(attacker) && isMedic(attacker)) && attackWasFire(damagetype))
          damage = 0.0;
      }        
    }
  }
    
  //
  // Zombie is taking damage from non-survivor.
  //
  else if(validZom(victim) && !validSur(attacker))
  {
    // Handle Perk (Z): LEAP
    // + No fall damage.
    if(usingZomPerk(victim, ZF_PERK_LEAP))
    {
      if(attackWasSelfFall(inflictor, damagetype))
        damage = 0.0;
    }
    // Handle Perk (Z): SCORCHING
    // + No fire damage.
    else if(usingZomPerk(victim, ZF_PERK_SCORCHING))
    {
      if(attackWasFire(damagetype))
        damage = 0.0;
    }
  }
  
  // 
  // Adjust damage based on attacker attack bonus and vicitm defense bonus.
  //
  if(damage > 0.0)
  {
    new Float:dmgFactor = ((100 + localAttAdjust) * (100 - localDefAdjust)) / 10000.0;        
    damage *= fMax(dmgFactor, 0.0);
    
    // Handle special effects (crit, minicrit) for sufficient dmgFactor.
    if(dmgFactor >= 3.0)
    {
      if(validClient(attacker)) fxCreateSoundToClient(ZFSND_CRITXMIT, attacker);
      if(validClient(victim)) fxCreateSoundToClient(ZFSND_CRITRECV, victim);
    }
    else if(dmgFactor >= 1.35)
    {
      if(validClient(attacker)) fxCreateSoundToClient(ZFSND_CRITMINI, attacker);
      if(validClient(victim)) fxCreateSoundToClient(ZFSND_CRITMINI, victim);
    }
  }
  
  //
  // Cripple on backstab.
  //
  if(GetConVarBool(zf_cvCripple))
  {
    if((damage > 0.0) && validSur(victim) && validZom(attacker))
    {
      if(attackWasBackstab(attacker, inflictor, damagetype))
      {
        if(!getCond(victim, ZFCondCrippled))
        {
          zf_lastHealth[victim] = GetClientHealth(victim);
          
          PrintHintText(attacker, "你大幅削弱了 %N !", victim);
          PrintHintText(victim, "%N 大幅削弱了你!", attacker);          
          addCond(victim, ZFCondCrippled, ZF_CRIPPLE_DURATION);
          addCond(victim, ZFCondTracked, ZF_CRIPPLE_DURATION);          
          fxDeathScream(victim);
          fxBloodBurst(victim);
          fxBloodSpray(victim);
          setGlow(victim, true);
        }
        damage = 0.0;
      }
    }
  }
     
  return Plugin_Changed;  
}

public perk_OnTakeDamagePost(victim, attacker, inflictor, Float:damage, damagetype)
{
//   // DEBUG
//   decl String:inflictorClass[32];
//   GetEdictClassname(inflictor, inflictorClass, sizeof(inflictorClass));
//   PrintToChatAll("\x05[ZF DEBUG]\x01 Vic %d, Att %d, Inf %d (%s), Dmg %f, DTp %x", victim, attacker, inflictor, inflictorClass, damage, damagetype);
     
  //
  // Survivor is taking damage from zombie.
  //
  if(validSur(victim) && validZom(attacker))
  {
    // Handle Perk (Z): VAMPIRIC
    // + Leech health with each melee hit.
    if(usingZomPerk(attacker, ZF_PERK_VAMPIRIC))
    {
      if(attackWasMelee(attacker, inflictor, damagetype))
      {
        addHealth(attacker, RoundToCeil(damage * ZF_VAMPIRIC_HEALTHPCT));
        fxBloodBurst(victim);
      }
    }
  }
  //
  // Zombie is taking damage from survivor.
  //
  else if(validZom(victim) && validSur(attacker))
  {
    // Handle Perk (S): NONLETHAL
    // + Knockback victim on bullet-based hit.
    if(usingSurPerk(attacker, ZF_PERK_NONLETHAL))
    {
      if(attackWasBullet(attacker, inflictor))
      {
        fxKnockback(victim, attacker, (ZF_NONLETHAL_FORCE * damage));
      }
    }
  }
}

public perk_OnTouch(toucher, touchee)
{ 
  if(validLivingZom(toucher) && validLivingSur(touchee))
  {
    // Handle Perk (Z): SCORCHING
    // + Ignite touchee on touch.
    if(usingZomPerk(toucher, ZF_PERK_SCORCHING))
    {
      if(isOnFire(toucher) && !isPyro(touchee))
        TF2_IgnitePlayer(touchee, toucher);
    }
  }
}

public perk_OnPlayerSpawn(client)
{
  if(validClient(client))
  { 
    //
    // Handle spawn menu presentation.
    //
    if(zf_perkMode == 0)
    {
      new js = prefGet(client, JoinState);
      if(validSur(client))
      {
        if((js & ZF_JOINSTATE_SUR) == 0)
        {
          DisplayMenu(zf_menuSurPerkList, client, MENU_TIME_FOREVER);
          prefSet(client, JoinState, (js | ZF_JOINSTATE_SUR));
        }
      }
      else if(validZom(client))
      {
        if((js & ZF_JOINSTATE_ZOM) == 0)
        {   
          DisplayMenu(zf_menuZomPerkList, client, MENU_TIME_FOREVER);
          prefSet(client, JoinState, (js | ZF_JOINSTATE_ZOM));         
        }
      }
    }

    //
    // Change perks if they were not selected during current game mode.
    //      
    if(prefGet(client, PerkSelectMode) != zf_perkMode)
    {      
      //
      // Clear perks.
      //
      if(zf_perkMode == 0)
      {
        prefSet(client, SurPendPerk, ZF_PERK_NONE);
        prefSet(client, ZomPendPerk, ZF_PERK_NONE);
        PrintToChat(client, "\x05[ZF]\x01 Perks have been cleared. You must reselect your perks.");
      }
      //
      // Select random perks.
      //
      else if(zf_perkMode == 1)
      {
        new validSurPerkCount = 0;
        new validZomPerkCount = 0;   
        decl validSurPerks[TOTAL_SUR_PERKS];
        decl validZomPerks[TOTAL_ZOM_PERKS];
           
        for(new i = 1; i < TOTAL_SUR_PERKS; i++)
          if(surPerkEnabled(i))
            validSurPerks[validSurPerkCount++] = i;
        if(validSurPerkCount == 0)
          validSurPerks[validSurPerkCount++] = 0;
    
        for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
          if(zomPerkEnabled(i))
            validZomPerks[validZomPerkCount++] = i;
        if(validZomPerkCount == 0)
          validZomPerks[validZomPerkCount++] = 0;

        prefSet(client, SurPendPerk, validSurPerks[GetRandomInt(0, validSurPerkCount - 1)]);
        prefSet(client, ZomPendPerk, validZomPerks[GetRandomInt(0, validZomPerkCount - 1)]);
        PrintToChat(client, "\x05[ZF]\x01 Random perks: %s (Survivor) / %s (Zombie)", ZF_SURPERK_NAME[prefGet(client, SurPendPerk)], ZF_ZOMPERK_NAME[prefGet(client, ZomPendPerk)]);      
      }
      //
      // Use random team perks.
      //   
      else if(zf_perkMode == 2)
      {
        prefSet(client, SurPendPerk, zf_perkRandSurPerk);
        prefSet(client, ZomPendPerk, zf_perkRandZomPerk);
        PrintToChat(client, "\x05[ZF]\x01 Random team perks: %s (Survivors) vs. %s (Zombies)", ZF_SURPERK_NAME[zf_perkRandSurPerk], ZF_ZOMPERK_NAME[zf_perkRandZomPerk]); 
      }
      //
      // Use CVAR team perks.
      //
      else if(zf_perkMode == 3)
      {
        prefSet(client, SurPendPerk, zf_perkTeamSurPerk);
        prefSet(client, ZomPendPerk, zf_perkTeamZomPerk);
        PrintToChat(client, "\x05[ZF]\x01 CVAR team perks: %s (Survivors) vs. %s (Zombies)", ZF_SURPERK_NAME[zf_perkTeamSurPerk], ZF_ZOMPERK_NAME[zf_perkTeamZomPerk]);
      }
      prefSet(client, SurPerk, ZF_PERK_NONE);
      prefSet(client, ZomPerk, ZF_PERK_NONE);
      prefSet(client, PerkSelectMode, zf_perkMode);
    }
    
    //
    // Apply new perk if client changed teams or if desired perk
    // (accounting for those disabled) differs from current perk.
    // Account for perk limits (only during normal game mode).
    //
    new nextPerk = ZF_PERK_NONE;
    new bool:perkChange = false;
    new bool:teamChange = (GetClientTeam(client) != zf_lastTeam[client]);     
    if(isSur(client))
    {
      if(surPerkEnabled(prefGet(client, SurPendPerk)))
        nextPerk = prefGet(client, SurPendPerk);      
      else if(surPerkEnabled(prefGet(client, SurPerk)))
        nextPerk = prefGet(client, SurPerk);
      
      if((zf_perkMode == 0) && surPerkAtLimit(client, nextPerk))
      {
        PrintToChat(client, "\x05[ZF]\x01 Desired perk (%s) al准备好了 at limit (%d).", ZF_SURPERK_NAME[nextPerk], zf_surPerksLimit[nextPerk]);
        nextPerk = ZF_PERK_NONE;
      }
        
      if(teamChange || (nextPerk != prefGet(client, SurPerk)))
      {
        perkChange = true;  
        prefSet(client, SurPerk, nextPerk);
      }
      prefSet(client, SurPendPerk, nextPerk);
    }
    else if(isZom(client))
    {      
      if(zomPerkEnabled(prefGet(client, ZomPendPerk)))
        nextPerk = prefGet(client, ZomPendPerk);      
      else if(zomPerkEnabled(prefGet(client, ZomPerk)))
        nextPerk = prefGet(client, ZomPerk);

      if((zf_perkMode == 0) && zomPerkAtLimit(client, nextPerk))
      {
        PrintToChat(client, "\x05[ZF]\x01 Desired perk (%s) al准备好了 at limit (%d).", ZF_ZOMPERK_NAME[nextPerk], zf_zomPerksLimit[nextPerk]);
        nextPerk = ZF_PERK_NONE;
      }
                        
      if(teamChange || (nextPerk != prefGet(client, ZomPerk)))
      {
        perkChange = true;         
        prefSet(client, ZomPerk, nextPerk);
      }  
      prefSet(client, ZomPendPerk, nextPerk);
    }

    //
    // Cleanup client state based on whether perk changed.
    //
    if(perkChange)
    {
        // Delete old perk object if it exists
        if (g_hPerks[client] != null)
        {
            g_hPerks[client].onRemove();
            delete g_hPerks[client];
            g_hPerks[client] = null;
        }

        // Create new perk object based on team and selected perk
        if (isSur(client))
        {
            switch (nextPerk)
            {
                case ZF_PERK_ATHLETIC:
                {
                    g_hPerks[client] = new AthleticPerk(client);
                }
                // ... other survivor perks will be added here
                default:
                {
                    g_hPerks[client] = new base_perk(client);
                }
            }
        }
        else if (isZom(client))
        {
            switch (nextPerk)
            {
                case ZF_PERK_ALPHA:
                {
                    g_hPerks[client] = new AlphaPerk(client);
                }
                // ... other zombie perks will be added here
                default:
                {
                    g_hPerks[client] = new base_perk(client);
                }
            }
        }
    }

    //
    // Call the onSpawn method for the current perk object
    //
    if (g_hPerks[client] != null)
    {
        g_hPerks[client].onSpawn();
    }
    
    // Reset legacy state vars that are no longer managed by individual perks
    zf_lastAttack[client] = 0;
    zf_lastButtons[client] = 0;
    zf_lastHealth[client] = 0;
    zf_lastKiller[client] = 0;
    zf_lastPoison[client] = 0;
    zf_lastTeam[client] = GetClientTeam(client);

    // HUD
    updateHud(client);
  }
}

public perk_OnPlayerDeath(victim, killer, assist, inflictor, damagetype)
{
//   // DEBUG
//   LogMessage("[ZF DEBUG] Vic %d, Klr %d, Ast %d, Inf %d, DTp %x", victim, killer, assist, inflictor, damagetype);
  
  if(validClient(victim))
  {  
    //
    // Set last killer.
    //
    zf_lastKiller[victim] = killer;
             
    //
    // Clear conditions.
    //
    resetClientConds(victim);
    
    //
    // Handle effects.
    //
    hideAura(victim);
    hideIcon(victim);
    removeItems(victim);      
    setGlow(victim, false);
  }

  //
  // Survivor killed.
  //
  if(validSur(victim))
  {
    //
    // Survivor killed by zombie.
    //        
    if(validZom(killer))
    {
      // Handle Perk (S): SELFLESS
      // + Explode on death.
      if(usingSurPerk(victim, ZF_PERK_SELFLESS))
      {
        applyDamageRadialAtClient(victim, ZF_SELFLESS_DAMAGE, ZF_SELFLESS_RADIUS, true);
        fxExplosionBig(victim);  
      }
          
      // Handle Perk (Z): ALPHA
      // + Add minion.
      if(usingZomPerk(killer, ZF_PERK_ALPHA))
      {      
        zf_perkAlphaMaster[victim] = killer;      
        PrintHintText(killer, "%N 现在是你的随从了.", victim);
      }
      // Handle Perk (Z): VINDICTIVE
      else if(usingZomPerk(killer, ZF_PERK_VINDICTIVE))
      {
        addStat(killer, ZFStatAtt, ZFStatTypePerm, ZF_VINDICTIVE_ATTACK);
        addStat(killer, ZFStatDef, ZFStatTypePerm, ZF_VINDICTIVE_DEFEND);
      }
    }
    
    //
    // Survivor killed, assisted by zombie.
    //
    if(validZom(assist))
    {
      // Handle Perk (Z): ALPHA
      // + Add minion.
      if(usingZomPerk(assist, ZF_PERK_ALPHA))
      {
        if(!(validZom(killer) && usingZomPerk(killer, ZF_PERK_ALPHA)))
        {
          zf_perkAlphaMaster[victim] = assist;
          PrintHintText(assist, "%N 已经是你的随从了.", victim);
        }
      }
      // Handle Perk (Z): VINDICTIVE
      else if(usingZomPerk(assist, ZF_PERK_VINDICTIVE))
      {
        addStat(assist, ZFStatAtt, ZFStatTypePerm, ZF_VINDICTIVE_ATTACK_ASSIST);
        addStat(assist, ZFStatDef, ZFStatTypePerm, ZF_VINDICTIVE_DEFEND_ASSIST);
      }
    }
  }
  //
  // Zombie killed.
  // 
  else if(validZom(victim))
  {  
    // Handle Perk (Z): HUNTER
    if(usingZomPerk(victim, ZF_PERK_HUNTER))
    {
      showAura(victim);
    }
    // Handle Perk (Z): RAGE
    else if(usingZomPerk(victim, ZF_PERK_RAGE))
    {
      removeAura(victim);
    }
          
    //
    // Zombie killed by survivor.
    //     
    if(validSur(killer))
    {    
      // Handle Perk (S): CHARITABLE
      if(usingSurPerk(killer, ZF_PERK_CHARITABLE))
      {
        zf_perkState[killer] += ZF_CHARITABLE_POINTS_KILL;
      }
      // Handle Perk (S): FRIEND
      else if(usingSurPerk(killer, ZF_PERK_FRIEND))
      {
        if(validSur(assist) && (zf_perkState[killer] == assist))
          zf_perkTimer[killer] += ZF_FRIEND_CRITTIME_KILL;
      }
      // Handle Perk (S): HEROIC
      else if(usingSurPerk(killer, ZF_PERK_HEROIC))
      {
        if(zf_perkTimer[killer] > 0)
        {
          if(zf_perkState[killer] == 0)
            zf_perkTimer[killer] += HEROIC_CRITTIME_KILL;
          else
            zf_perkTimer[killer] += HEROIC_CRITTIME_KILL_ACTIVE;
        }
      }
      // Handle Perk (S): RESOURCEFUL
      else if(usingSurPerk(killer, ZF_PERK_RESOURCEFUL))
      {
        addHealth(killer, ZF_RESOURCEFUL_HEALTH);
        addHealth(killer, ZF_RESOURCEFUL_HEALTH_OVERHEAL, true);
              
        addResAmmoPct(killer, 0, ZF_RESOURCEFUL_AMMOPCT);
        addResAmmoPct(killer, 1, ZF_RESOURCEFUL_AMMOPCT);
        addMetal(killer, ZF_RESOURCEFUL_METAL);
        fxCreateSoundToClient(ZFSND_PICKUPAMMO, killer);        
      }      
      // Handle Perk (S): WISE
      else if(usingSurPerk(killer, ZF_PERK_WISE))
      {
        addStat(killer, ZFStatAtt, ZFStatTypePerm, ZF_WISE_ATTACK_KILL);
      }

      // Handle Perk (Z): COMBUSTIBLE 
      if(usingZomPerk(victim, ZF_PERK_COMBUSTIBLE))
      {
        if(!attackWasMelee(killer, inflictor, damagetype))
        {
          new damage = isHeavy(victim) ? ZF_COMBUSTIBLE_DAMAGE_HEAVY : ZF_COMBUSTIBLE_DAMAGE;
          applyDamageRadialAtClient(victim, damage, ZF_COMBUSTIBLE_RADIUS, true);             
          fxExplosionBig(victim);
          CreateTimer(ZF_COMBUSTIBLE_RESPAWNTIME, perk_tSpawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
        }
      }
      // Handle Perk (Z): HORRIFYING
      // + Reduce temporary stat penalties.
      else if(usingZomPerk(victim, ZF_PERK_HORRIFYING))
      {
        if(getStatType(killer, ZFStatAtt, ZFStatTypeTemp) < 0)
          scaleStatTempPct(killer, ZFStatAtt, (1.0 - HORRIFYING_PENALTYPCT_KILL));
        if(getStatType(killer, ZFStatDef, ZFStatTypeTemp) < 0)
          scaleStatTempPct(killer, ZFStatDef, (1.0 - HORRIFYING_PENALTYPCT_KILL));
        if(getStatType(killer, ZFStatRof, ZFStatTypeTemp) < 0)
          scaleStatTempPct(killer, ZFStatRof, (1.0 - HORRIFYING_PENALTYPCT_KILL));
      }      
      // Handle Perk (Z): HUNTER
      else if(usingZomPerk(victim, ZF_PERK_HUNTER))
      {
        CreateTimer(ZF_HUNTER_RESPAWNTIME, perk_tSpawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
      }   
      // Handle Perk (Z): SWARMING
      else if(usingZomPerk(victim, ZF_PERK_SWARMING))
      {
        CreateTimer(ZF_SWARMING_RESPAWNTIME, perk_tSpawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
      }
      // Handle Perk (Z): SWARMING
      // + Instant respawn for those killed near a SWARMING zombie.      
      else
      {
        decl Float:iPos[3];
        decl Float:victimPos[3];        
        GetClientAbsOrigin(victim, victimPos);
        for(new i = 1; i <= MaxClients; i++)
        {
          if(validLivingZom(i) && usingZomPerk(i, ZF_PERK_SWARMING))
          {         
            GetClientAbsOrigin(i, iPos);
            if(GetVectorDistance(iPos, victimPos, true) <= ZF_SWARMING_RADIUSSQ)
            {
              CreateTimer(ZF_SWARMING_RESPAWNTIME, perk_tSpawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
              break;
            }
          }
        }
      }
    }
    
    //
    // Zombie killed, assisted by survivor.
    //
    if(validSur(assist))
    {
      // Handle Perk (S): CHARITABLE
      if(usingSurPerk(assist, ZF_PERK_CHARITABLE))
      {
        zf_perkState[assist] += ZF_CHARITABLE_POINTS_ASSIST;
      }
      // Handle Perk (S): FRIEND
      else if(usingSurPerk(assist, ZF_PERK_FRIEND))
      {
        if(validSur(killer) && (zf_perkState[assist] == killer))
          zf_perkTimer[assist] += ZF_FRIEND_CRITTIME_ASSIST;
      }
      // Handle Perk (S): HEROIC
      else if(usingSurPerk(assist, ZF_PERK_HEROIC))
      {
        if(zf_perkTimer[assist] > 0)
        {
          if(zf_perkState[assist] == 0)
            zf_perkTimer[assist] += HEROIC_CRITTIME_ASSIST;
          else
            zf_perkTimer[assist] += HEROIC_CRITTIME_ASSIST_ACTIVE;        
        }
      }
      // Handle Perk (S): WISE
      else if(usingSurPerk(assist, ZF_PERK_WISE))
      {
        addStat(assist, ZFStatAtt, ZFStatTypePerm, ZF_WISE_ATTACK_ASSIST);
      }
      
      // Handle Perk (Z): HORRIFYING
      // + Reduce temporary stat penalties.
      if(usingZomPerk(victim, ZF_PERK_HORRIFYING))
      {
        if(getStatType(assist, ZFStatAtt, ZFStatTypeTemp) < 0)
          scaleStatTempPct(assist, ZFStatAtt, (1.0 - HORRIFYING_PENALTYPCT_ASSIST));
        if(getStatType(assist, ZFStatDef, ZFStatTypeTemp) < 0)
          scaleStatTempPct(assist, ZFStatDef, (1.0 - HORRIFYING_PENALTYPCT_ASSIST));
        if(getStatType(assist, ZFStatRof, ZFStatTypeTemp) < 0)
          scaleStatTempPct(assist, ZFStatRof, (1.0 - HORRIFYING_PENALTYPCT_ASSIST));
      }      
    }
  }
}

public Action:perk_OnCallForMedic(client)
{  
  if(validLivingSur(client))
  {
    // Handle Perk (S): CARPENTER
    // + Place barricade.
    if(usingSurPerk(client, ZF_PERK_CARPENTER))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能放置障碍物.");
        }
        else if(!isCrouching(client))
        {
          PrintHintText(client, "你必须一直蹲在地上才能放置障碍物.");
        }
        else
        {          
          new bool:clientsNearby = false;
          new bool:barricadesNearby = false;
          decl Float:clientPos[3];
          decl Float:otherPos[3];
                              
          GetClientAbsOrigin(client, clientPos);
          for(new i = 1; i <= MaxClients; i++)
          {
            if(IsClientInGame(i) && IsPlayerAlive(i))
            {
              // Check for nearby barricades.
              if(isSur(i) && usingSurPerk(i, ZF_PERK_CARPENTER))
              {
                for(new j = 0; j < CARPENTER_MAX_ITEMS; j++)
                {
                  if(validItem(i, j))
                  {
                    if(GetVectorDistance(clientPos, zf_perkPos[i][j], true) <= CARPENTER_DROP_RADSQ_BARRICADE)
                    {
                      barricadesNearby = true;
                      break;
                    }
                  }
                }
                if(barricadesNearby)
                  break;
              }
              
              // Check for other nearby clients.
              if(i == client) continue;
              GetClientAbsOrigin(i, otherPos);
              if(GetVectorDistance(clientPos, otherPos, true) <= CARPENTER_DROP_RADSQ_CLIENT)
              {
                clientsNearby = true;
                break;
              }
            }
          }
          
          // Place barricade.
          if(clientsNearby)
          {
            PrintHintText(client, "无法设置障碍物 - 附近有其他玩家.");
          }
          else if(barricadesNearby)
          {
            PrintHintText(client, "无法设置障碍物 - 附近有其他玩家的障碍物.");
          }
          else          
          {
            new itemIndex = getFreeItemIndex(client, CARPENTER_MAX_ITEMS);
            if(itemIndex >= 0)
            {
              zf_perkTimer[client] = CARPENTER_COOLDOWN;
              
              removeItem(client, itemIndex);
              zf_item[client][itemIndex] = doCarpenterBuild(client, zf_perkPos[client][itemIndex]);
              setItemMetadata(zf_item[client][itemIndex], CARPENTER_BARRICADE_HEALTH);   
              SDKHook(zf_item[client][itemIndex], SDKHook_OnTakeDamage, perk_OnFenceTakeDamage);
            }
          }
          return Plugin_Handled; 
        }
      }      
    }
    
    // Handle Perk (S): CHARITABLE
    else if(usingSurPerk(client, ZF_PERK_CHARITABLE))
    {
      if((zf_perkState[client] / ZF_CHARITABLE_POINTS_GIFT) > 0)
      {
        // Toss present.
        new itemIndex = getFreeItemIndex(client, ZF_CHARITABLE_MAX_ITEMS);
        if(itemIndex >= 0)
        {
          zf_perkState[client] -= ZF_CHARITABLE_POINTS_GIFT;
          zf_item[client][itemIndex] = doItemThrow(client, ZFMDL_PRESENT[GetRandomInt(0,2)], 600.0);
          SDKHook(zf_item[client][itemIndex], SDKHook_Touch, perk_OnCharitableGiftTouched);
        }
        else
        {
          PrintHintText(client, "你没法扔掉那个礼物.");
        }
        return Plugin_Handled;        
      }
    }
      
    // Handle Perk (S): FRIEND
    // + Select friend (grace period or earlier only).
    else if(usingSurPerk(client, ZF_PERK_FRIEND))
    {
      if(roundState() <= RoundGrace)
      {
        new desiredFriend = GetClientAimTarget(client);
        if(validLivingSur(desiredFriend))
        {
          doFriendSelect(client, desiredFriend);
          return Plugin_Handled;
        }
      }
    }
    
    // Handle Perk (S): LEADER
    else if(usingSurPerk(client, ZF_PERK_LEADER))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能放置旗帜.");
        }
        else if(!isCrouching(client))
        {
          PrintHintText(client, "你必须一直蹲在地上才能放置旗帜.");
        }
        else
        {
          zf_perkTimer[client] = ZF_LEADER_RALLY_COOLDOWN;
          removeItem(client, 0);            
          zf_item[client][0] = doItemPlace(client, ZFMDL_FLAG);

          fxCreateSoundToAll(ZFSND_PACKDROP, client);
          return Plugin_Handled;
        }        
      }
    }
    
    // Handle Perk (S): NINJA
    else if(usingSurPerk(client, ZF_PERK_NINJA))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能设置诱饵.");
        }
        else if(isCrouching(client))
        {
          PrintHintText(client, "你不需要蹲着来设置诱饵.");
        }
        else
        {
          // Place retreat point.
          // Retreat point is valid for ZF_NINJA_DURATION_DECOY_ACTIVE seconds.
          // Retreat point poofs after ZF_NINJA_DURATION_DECOY_ACTIVE seconds.
          // New retreat point can be placed after (ZF_NINJA_DURATION_DECOY_ACTIVE + ZF_NINJA_DURATION_COOLDOWN) seconds.
          zf_perkState[client] = 1;
          zf_perkTimer[client] = (ZF_NINJA_DURATION_DECOY_ACTIVE + ZF_NINJA_DURATION_COOLDOWN);
          
          createAura(client, ZFPART_AURAHALO, AttachNone);
          return Plugin_Handled;
        }        
      }
    }
     
    // Handle Perk (S): STASH
    else if(usingSurPerk(client, ZF_PERK_STASH))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你需要在地上放置藏身处.");
        }
        else if(!isCrouching(client))
        {
          PrintHintText(client, "你必须蹲在地上才能布置藏身处.");
        }
        else
        {
          new bool:stashNearby = false;
          new bool:survivorsNearby = false;
          decl Float:clientPos[3];
          decl Float:otherPos[3];
          
          GetClientAbsOrigin(client, clientPos);
          for(new i = 1; i <= MaxClients; i++)
          {
            if(i == client) continue;
            if(IsClientInGame(i) && IsPlayerAlive(i) && isSur(i))
            {
              // Check for other nearby survivors.
              GetClientAbsOrigin(i, otherPos);
              if(GetVectorDistance(clientPos, otherPos, true) <= STASH_DROP_RADSQ_CLIENT)
              {
                survivorsNearby = true;
                break;
              }
              
              // Check for nearby stashes from other survivors.
              if(usingSurPerk(i, ZF_PERK_STASH) && validItem(i, 0))
              {      
                getEntityPos(zf_item[i][0], otherPos);
                if(GetVectorDistance(clientPos, otherPos, true) <= STASH_DROP_RADSQ_STASH)
                {
                  stashNearby = true;
                  break;
                }
              }
            }            
          }        
     
          // Place stash.
          if(survivorsNearby)
          {
            PrintHintText(client, "无法布置藏身处 - 附近有其他幸存者.");
          }
          else if(stashNearby)
          {
            PrintHintText(client, "无法布置藏身处 - 附近有其他藏身处.");
          }
          else
          {
            zf_perkTimer[client] = STASH_WARMUP + STASH_COOLDOWN;
            removeItem(client, 0);            
            zf_item[client][0] = doItemPlace(client, ZFMDL_SUPPLYCRATE);
          
            fxCreateSoundToAll(ZFSND_PACKDROP, client);
          }
          return Plugin_Handled; 
        }      
      }
    }
    
    // Handle Perk (S): SUPPLIER
    else if(usingSurPerk(client, ZF_PERK_SUPPLIER))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能放置补给箱.");
        }
        else if(!isCrouching(client))
        {
          PrintHintText(client, "你必须一直蹲在地上才能放置补给箱.");
        }
        else
        {
          // Handle supplies placement.
          new itemIndex = getFreeItemIndex(client, ZF_SUPPLIER_MAX_ITEMS);
          if(itemIndex >= 0)
          {
            zf_perkTimer[client] = ZF_SUPPLIER_TIMER;
            
            removeItem(client, itemIndex);
            zf_item[client][itemIndex] = doItemPlace(client, ZFMDL_SUPPLYCRATE);
            setItemMetadata(zf_item[client][itemIndex], ZF_SUPPLIER_RESUPPLY_COUNT);
            
            fxCreateSoundToAll(ZFSND_PACKDROP, client);               
          }
          else
          {
            PrintHintText(client, "你无法再放置补给箱.");
          }
          return Plugin_Handled;          
        }        
      }
    }
    
    // Handle Perk (S): TRAPPER
    else if(usingSurPerk(client, ZF_PERK_TRAPPER))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能布置地雷.");
        }
        else if(!isCrouching(client))
        {
          PrintHintText(client, "你必须一直蹲在地上才能布置地雷.");
        }
        else
        {
          // Handle mine placement.
          new itemIndex = getFreeItemIndex(client, ZF_TRAPPER_MAX_ITEMS);
          if(itemIndex >= 0)
          {
            zf_perkTimer[client] = ZF_TRAPPER_TIMER;
            removeItem(client, itemIndex);
            zf_item[client][itemIndex] = doItemPlace(client, ZFMDL_MINE);

            fxCreateSoundToAll(ZFSND_PACKDROP, client);            
          }
          else
          {
            PrintHintText(client, "你无法再放置地雷.");
          }
          return Plugin_Handled;
        }        
      }
    }
    
    // Handle Perk (S): TANTRUM
    else if(usingSurPerk(client, ZF_PERK_TANTRUM))
    {
      if(zf_perkTimer[client] == 0)
      {
        zf_perkTimer[client] = ZF_TANTRUM_ACTIVE + ZF_TANTRUM_COOLDOWN;
        addCondKritz(client, float(ZF_TANTRUM_ACTIVE));
        fxKritzStart(client);
        
        return Plugin_Handled;
      }      
    }
    
  }
  else if(validLivingZom(client))
  {        
    // Handle Perk (Z): ALPHA
    if(usingZomPerk(client, ZF_PERK_ALPHA))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能召唤你的随从们.");
          ClientCommand(client, "voicemenu 2 5");
        }
        else if(isCrouching(client))
        {
          PrintHintText(client, "你不需要蹲着来召唤随从们.");
          ClientCommand(client, "voicemenu 2 5");
        }
        else if(isCloaked(client))
        {
          PrintHintText(client, "你必须要解除隐身才能召唤随从.");
          ClientCommand(client, "voicemenu 2 5");
        }
        else
        {
          new summonCount = doAlphaSummon(client);
          if(summonCount > 0)
          {
            zf_perkTimer[client] = ZF_ALPHA_TIMER_MINION * summonCount;
            ClientCommand(client, "voicemenu 2 1");
          }
          else
          {
            PrintHintText(client, "你没有随从可以召唤!");
            ClientCommand(client, "voicemenu 2 5");            
          }          
        }
      }
      else
      {
        PrintHintText(client, "召唤技能没准备好.");
        ClientCommand(client, "voicemenu 2 5");
      }      
    }
    
    // Handle Perk (Z): HUNTER
    else if(usingZomPerk(client, ZF_PERK_HUNTER))
    {
      if(zf_perkState[client] == 1)
      {
        PrintHintText(client, "你已经放过了你的重生点.");
        ClientCommand(client, "voicemenu 2 5");
      }
      else if(!isGrounded(client))
      {
        PrintHintText(client, "你必须站在地上才能布置重生点.");
        ClientCommand(client, "voicemenu 2 5");        
      }
      else if(isCrouching(client))
      {
        PrintHintText(client, "你不需要蹲着来布置重生点.");
        ClientCommand(client, "voicemenu 2 5");
      }  
      else
      {
        zf_perkState[client] = 1;
        createAura(client, ZFPART_AURAVORTEXBLU, AttachNone);
        GetClientEyeAngles(client, zf_perkPos[client][0]);
        ClientCommand(client, "voicemenu 0 6");
      }
    }

    // Handle Perk (Z): LEAP 
    else if(usingZomPerk(client, ZF_PERK_LEAP))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能飞跃.");
          ClientCommand(client, "voicemenu 2 5");
        }
        else if(isCloaked(client))
        {
          PrintHintText(client, "你必须解除隐身才能飞跃.");
          ClientCommand(client, "voicemenu 2 5");
        }
        else
        {
          zf_perkTimer[client] = ZF_LEAP_COOLDOWN;
          fxJump(client, (isScout(client) ? ZF_LEAP_FORCE_SCOUT : ZF_LEAP_FORCE));
          fxPuffBig(client); 
        }
      }
      else
      {
        PrintHintText(client, "飞跃还没准备好.");
        ClientCommand(client, "voicemenu 2 5");      
      }        
    }    
    
    // Handle Perk (Z): RAGE  
    else if(usingZomPerk(client, ZF_PERK_RAGE))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(getHealthPct(client) >= ZF_RAGE_HEALTHPCT_TOUSE)
        {
          zf_perkState[client] = 1;
          zf_perkTimer[client] = ZF_RAGE_COOLDOWN;
                                 
          addStat(client, ZFStatSpeed, ZFStatTypeCond, ZF_RAGE_SPEED);    
          setHealthPct(client, (1.0 + ZF_RAGE_HEALTHPCT_ONUSE)); 

          fxPowerup(client);
          createAura(client, ZFPART_AURABURNINGORANGE, AttachHead);
        }
        else
        {
          PrintHintText(client, "生命值至少要在80%以上才能进入愤怒状态.");
          ClientCommand(client, "voicemenu 2 5"); 
        }
      }
      else
      {
        PrintHintText(client, "愤怒状态还没准备好.");
        ClientCommand(client, "voicemenu 2 5");      
      }  
    }
       
    // Handle Perk (Z): ROAR
    else if(usingZomPerk(client, ZF_PERK_ROAR))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(!isGrounded(client))
        {
          PrintHintText(client, "你必须站在地上才能咆哮.");
          ClientCommand(client, "voicemenu 2 5");           
        }
        else if(isCloaked(client))
        {
          PrintHintText(client, "你必须解除隐身才能咆哮.");
          ClientCommand(client, "voicemenu 2 5");           
        }
        else
        {
          zf_perkTimer[client] = ZF_ROAR_COOLDOWN;

          // Apply radial damage. Use damage hook to detect roar "explosion."
          applyDamageRadialAtClient(client, 1, ZF_ROAR_RADIUS);
          fxPuffBig(client);
          ClientCommand(client, "voicemenu 2 1");           
        }
      }
      else
      {
        PrintHintText(client, "咆哮还没准备好.");
        ClientCommand(client, "voicemenu 2 5");          
      }
    }
       
    // Handle Perk (Z): SICK  
    else if(usingZomPerk(client, ZF_PERK_SICK))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(isCloaked(client))
        {
          PrintHintText(client, "你必须解除隐身才能吐酸液.");
          ClientCommand(client, "voicemenu 2 5"); 
        }  
        else
        {
          zf_perkTimer[client] = ZF_SICK_TIMER;

          new Float:time = 0.0;
          for(new entIdx = 0; entIdx < ZF_SICK_MAX_ITEMS; entIdx++)
          {
            removeItem(client, entIdx);
            new Handle:dataPack = CreateDataPack();
            WritePackCell(dataPack, client);
            WritePackCell(dataPack, entIdx);
            CreateTimer(time, perk_tSickSpit, dataPack, TIMER_FLAG_NO_MAPCHANGE);
            time += 0.15;
          }
        }
      }
      else
      {
        PrintHintText(client, "酸液还没有准备好.");
        ClientCommand(client, "voicemenu 2 5");      
      }  
    }    
    
    // Handle Perk (Z): TARRED
    else if(usingZomPerk(client, ZF_PERK_TARRED))
    {
      if(zf_perkTimer[client] == 0)
      {
        if(isCloaked(client))
        {
          PrintHintText(client, "你必须解除隐身才能吐焦油);");
          ClientCommand(client, "voicemenu 2 5"); 
        }  
        else
        {
          zf_perkTimer[client] = ZF_TARRED_TIMER;

          new Float:time = 0.0;
          for(new entIdx = 0; entIdx < ZF_TARRED_MAX_ITEMS; entIdx++)
          {
            removeItem(client, entIdx);
            new Handle:dataPack = CreateDataPack();
            WritePackCell(dataPack, client);
            WritePackCell(dataPack, entIdx);
            CreateTimer(time, perk_tTarredSpit, dataPack, TIMER_FLAG_NO_MAPCHANGE);
            time += 0.15;
          }
        }
      }
      else
      {
        PrintHintText(client, "焦油还没有准备好.");
        ClientCommand(client, "voicemenu 2 5");
      }
    }

    return Plugin_Handled;
  }
    
  return Plugin_Continue;
}

public perk_OnAmmoPickup(client, pickup)
{
  if(validLivingSur(client))
  {
    // Handle Perk (S): RESOURCEFUL    
    if(usingSurPerk(client, ZF_PERK_RESOURCEFUL))
    {
      addResAmmoPct(client, 0, 1.0);
      addResAmmoPct(client, 1, 1.0);
      addMetal(client, 200);
      addStatTempStack(client, ZFStatAtt, ZF_RESOURCEFUL_ATTACK, ZF_RESOURCEFUL_PICKUP_DURATION);
    }
  }
}

public perk_OnMedPickup(client, pickup)
{
  if(validLivingSur(client))
  {
    // Handle general survivor logic
    subCond(client, ZFCondPoisoned, ZF_POISON_HEAL_MEDPACK);    
    
    // Handle Perk (S): RESOURCEFUL    
    if(usingSurPerk(client, ZF_PERK_RESOURCEFUL))
    {
      setHealthPct(client, 1.0);
      addStatTempStack(client, ZFStatDef, ZF_RESOURCEFUL_DEFEND, ZF_RESOURCEFUL_PICKUP_DURATION);
    }
  }
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Timer Callbacks
//
////////////////////////////////////////////////////////////
public Action:perk_tSpawnClient(Handle:timer, any:client)
{
  if(validClient(client) && !IsPlayerAlive(client))
  {
    spawnClient(client, zomTeam());
  }
}

public Action:perk_tNinjaDecoyPoof(Handle:Timer, any:client)
{
  if(validClient(client))
  {
    doNinjaDecoyPoof(client);
  }
}
   
public Action:perk_tSickSpit(Handle:timer, Handle:dataPack)
{        
  ResetPack(dataPack);
  new client = ReadPackCell(dataPack);
  new entIdx = ReadPackCell(dataPack); 
  CloseHandle(dataPack);
    
  if(validLivingZom(client))
  {
    zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 1200.0, {75,255,75});
    getEntityPos(zf_item[client][entIdx], zf_perkPos[client][entIdx]);
    fxCreateSoundToAll(ZFSND_SPIT[GetRandomInt(0,2)], client);   
  }  
}

public Action:perk_tTarredSpit(Handle:timer, Handle:dataPack)
{
  ResetPack(dataPack);
  new client = ReadPackCell(dataPack);
  new entIdx = ReadPackCell(dataPack); 
  CloseHandle(dataPack);
    
  if(validLivingZom(client))
  {
    zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 900.0, {25,25,25});
    getEntityPos(zf_item[client][entIdx], zf_perkPos[client][entIdx]);
    fxCreateSoundToAll(ZFSND_SPIT[GetRandomInt(0,2)], client);
  }  
}

public Action:perk_tZenlikeAttack(Handle:timer, any:client)
{
  if(validLivingSur(client))
  {
    zf_perkState[client] -= ZF_ZENLIKE_CRIT_DEC;
    if(zf_perkState[client] < 0) 
      zf_perkState[client] = 0;
  }
}

////////////////////////////////////////////////////////////
//
// Perk Menu Functionality
//
////////////////////////////////////////////////////////////

//
// Survivor Perk List Menu
// (Menu item N is perk N+1)
//
Handle:perk_buildSurPerkListMenu()
{
  new Handle:menu = CreateMenu(perk_menuSurPerkList);  
  if(menu != INVALID_HANDLE)
  {
    // Title
    SetMenuTitle(menu, "选择幸存者职业");
    
    // Perks
    for(new i = 1; i < TOTAL_SUR_PERKS; i++)
      AddMenuItem(menu, ZF_SURPERK_NAME[i], ZF_SURPERK_SHORTDESC[i]);  
  }
  return menu; 
}

public perk_menuSurPerkList(Handle:menu, MenuAction:action, param1, param2)
{
  if(action == MenuAction_Select)
  { 
    panel_PrintSurPerkSelect(param1, param2 + 1);
  }
}

//
// Zombie Perk List Menu
// (Menu item N is perk N+1)
//
Handle:perk_buildZomPerkListMenu()
{
  new Handle:menu = CreateMenu(perk_menuZomPerkList);  
  if(menu != INVALID_HANDLE)
  {
    // Title
    SetMenuTitle(menu, "选择僵尸职业");
    
    // Perks
    for(new i = 1; i < TOTAL_ZOM_PERKS; i++)
      AddMenuItem(menu, ZF_ZOMPERK_NAME[i], ZF_ZOMPERK_SHORTDESC[i]);  

  }
  return menu; 
}

public perk_menuZomPerkList(Handle:menu, MenuAction:action, param1, param2)
{
  if(action == MenuAction_Select)
  {
    panel_PrintZomPerkSelect(param1, param2 + 1);
  }
}

// 
// Survivor Perk Select Menu
//
public panel_PrintSurPerkSelect(client, perk)
{
  new Handle:panel = CreatePanel();
  
  SetPanelTitle(panel, "技能选择");
  DrawPanelText(panel, ZF_SURPERK_NAME[perk]);
  DrawPanelText(panel, ZF_SURPERK_LONGDESC[perk]);
  if(surPerkEnabled(perk))
    DrawPanelItem(panel, "[选择]", ITEMDRAW_DEFAULT);
  else
    DrawPanelItem(panel, "(被服务器禁用)", ITEMDRAW_DISABLED);
  DrawPanelItem(panel, "[返回]");
  DrawPanelItem(panel, "[关闭]");
  
  zf_menuPerk[client] = perk;
  SendPanelToClient(panel, client, panel_HandleSurPerkSelect, MENU_TIME_FOREVER);
  CloseHandle(panel);
}

public panel_HandleSurPerkSelect(Handle:menu, MenuAction:action, param1, param2)
{ 
  if(action == MenuAction_Select)
  {
    switch(param2)
    {
      case 1: 
        selectSurPerk(param1, zf_menuPerk[param1]);
      case 2: 
      {
        new firstItem = (((zf_menuPerk[param1] - 1) / 7) * 7);
        DisplayMenuAtItem(zf_menuSurPerkList, param1, firstItem, MENU_TIME_FOREVER); 
      }
      default: 
        return;   
    } 
  }
}

// 
// Zombie Perk Select Menu
//
public panel_PrintZomPerkSelect(client, perk)
{
  new Handle:panel = CreatePanel();
  
  SetPanelTitle(panel, "技能选择");
  DrawPanelText(panel, ZF_ZOMPERK_NAME[perk]);
  DrawPanelText(panel, ZF_ZOMPERK_LONGDESC[perk]);
  if(zomPerkEnabled(perk))
    DrawPanelItem(panel, "[选择]", ITEMDRAW_DEFAULT);
  else
    DrawPanelItem(panel, "(被服务器禁用)", ITEMDRAW_DISABLED);
  DrawPanelItem(panel, "[返回]");
  DrawPanelItem(panel, "[关闭]");
  
  zf_menuPerk[client] = perk;
  SendPanelToClient(panel, client, panel_HandleZomPerkSelect, MENU_TIME_FOREVER);
  CloseHandle(panel);
}

public panel_HandleZomPerkSelect(Handle:menu, MenuAction:action, param1, param2)
{ 
  if(action == MenuAction_Select)
  {
    switch(param2)
    {
      case 1: 
        selectZomPerk(param1, zf_menuPerk[param1]);
      case 2: 
      {
        new firstItem = (((zf_menuPerk[param1] - 1) / 7) * 7);
        DisplayMenuAtItem(zf_menuZomPerkList, param1, firstItem, MENU_TIME_FOREVER); 
      }
      default: 
        return;   
    } 
  }
}
