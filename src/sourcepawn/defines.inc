
#pragma newdecls required

#if defined _ZF_DEFINE_INC
  #endinput
#endif
#define _ZF_DEFINE_INC

#define RegisterPerk(%1,%2,%3,%4,%5) \
        { \
            %1 perk = new %1(-1); \
            char classname[64]; \
            perk.getName(classname, sizeof(classname)); \
            LogMessage("[ZF] Register %s perk with classname %s", %5, classname); \
            if (%2.ContainsKey(classname)){ \
                LogError("[ZF] Attempted to register %s perk with duplicate classname: '%s'", %5, classname); \
            } \
            else { \
                StringMap perkInfo = new StringMap(); \
                char shortDesc[128]; \
                perk.getShortdesc(shortDesc, sizeof(shortDesc)); \
                char longDesc[1024]; \
                perk.getDesc(longDesc, sizeof(longDesc)); \
                perkInfo.SetString("name", classname); \
                perkInfo.SetString("shortDesc", shortDesc); \
                perkInfo.SetString("longDesc", longDesc); \
                %2.SetValue(classname, perkInfo); \
                %3.Push(perkInfo); \
                %4.Push(view_as<Function>(%1_new)); \
                LogMessage("[ZF] %s perk: classname %s shortDesc %s longDesc", %5, classname, shortDesc, longDesc); \
            } \
        } LogMessage("[ZF] register successed.")

#define RegisterSurvivorPerk(%1) RegisterPerk(%1, g_SurPerkRegistry, g_SurPerkTypes, g_SurPerkCreators, "survivor")
#define RegisterZombiePerk(%1) RegisterPerk(%1, g_ZomPerkRegistry, g_ZomPerkTypes, g_ZomPerkCreators, "zombie")

