
#pragma newdecls required

#if defined _ZF_DEFINE_INC
  #endinput
#endif
#define _ZF_DEFINE_INC

#define RegisterPerk(%1,%2,%3,%4) \
        { \
            char classname[64]; \
            %1_getName(classname, sizeof(classname)); \
            if (%2.ContainsKey(classname)){ \
                LogError("[ZF] Attempted to register %s perk with duplicate classname: '%s'", %4, classname); \
            } \
            else { \
                StringMap perkInfo = new StringMap(); \
                char shortDesc[128]; \
                %1_getShortdesc(shortDesc, sizeof(shortDesc)); \
                char longDesc[1024]; \
                %1_getDesc(longDesc, sizeof(longDesc)); \
                perkInfo.SetString("name", classname); \
                perkInfo.SetString("shortDesc", shortDesc); \
                perkInfo.SetString("longDesc", longDesc); \
                %2.SetValue(classname, perkInfo); \
                %3.Push(perkInfo); \
            } \
        } LogMessage("[ZF] register successed.")

#define RegisterSurvivorPerk(%1) RegisterPerk(%1, g_SurPerkRegistry, g_SurPerkTypes, "survivor")
#define RegisterZombiePerk(%1) RegisterPerk(%1, g_ZomPerkRegistry, g_ZomPerkTypes, "zombie")


/* ... 其他 includes ... */


// ### START: AUTOMATED DISPATCH & CREATION SYSTEM (FINAL CORRECT VERSION) ###

// 宏1: 自动生成事件分发逻辑
// %1: event name (e.g., onPlayerSpawn)
// %2: self (the StringMap perk object)
// %3: perkName (the classname string)
// ...: other arguments
#define DISPATCH_PERK_EVENT(%1,%2,%3) \
{ \
    /* 内层宏参数必须是 %%1 */ \
    #define PERK_ENTRY(%1) else if (StrEqual(%3, %1_getName(), false)) { return %1_%1(%2, __VA_ARGS__); } \
    if (false) {} \
    else { return BasePerk_%1(%2, ##__VA_ARGS__); } \
    #undef PERK_ENTRY \
}

// 宏2: 自动生成Perk对象创建逻辑
// %1: client index
// %2: perkName (the classname string)
#define CREATE_PERK_OBJECT(%1,%2) \
{ \
    /* 内层宏参数必须是 %%1 */ \
    #define PERK_ENTRY(%1) else if (StrEqual(%2, %1_getName(), false)) { g_hPerks[%1] = %1_new(%1); } \
    if (false) { \
    } \
    else { g_hPerks[%1] = BasePerk_new(%1); } \
    #undef PERK_ENTRY \
}
