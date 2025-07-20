#pragma newdecls required

#if defined _ZF_PERK_STRUCTS_INC
  #endinput
#endif
#define _ZF_PERK_STRUCTS_INC

//
// Perk Stats
//

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
