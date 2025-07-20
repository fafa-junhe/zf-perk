// zf_perk.inc

/* put the line below after all of the includes!
#pragma newdecls required
*/

#pragma newdecls required

#if defined _ZF_PERK_INC
  #endinput
#endif
#define _ZF_PERK_INC

#include "zf_util_base.inc"
#include "zf_util_fx.inc"
#include "zf_util_pref.inc"

ArrayList g_SurPerkTypes;
StringMap g_SurPerkRegistry;
ArrayList g_ZomPerkTypes;
StringMap g_ZomPerkRegistry;

#define RegisterPerk(%1,%2,%3,%4) \
        { \
            StringMap perk = %1_new(-1); \
            char classname[64]; \
            %1_getName(perk, classname, sizeof(classname)); \
            LogMessage("[ZF DEBUG] Registering %s perk. Initial classname: '%s'", %4, classname); \
            if (%2.ContainsKey(classname)){ \
                LogError("[ZF] Attempted to register %s perk with duplicate classname: '%s'", %4, classname); \
            } \
            else { \
                StringMap perkInfo = new StringMap(); \
                char shortDesc[128]; \
                %1_getShortdesc(perk, shortDesc, sizeof(shortDesc)); \
                char longDesc[1024]; \
                %1_getDesc(perk, longDesc, sizeof(longDesc)); \
                \
                perkInfo.SetString("name", classname); \
                perkInfo.SetString("shortDesc", shortDesc); \
                perkInfo.SetString("longDesc", longDesc); \
                \
                %2.SetValue(classname, perkInfo); \
                %3.Push(perkInfo); \
                LogMessage("[ZF DEBUG] Registered %s perk: '%s'", %4, classname); \
            } \
            delete perk; \
        } LogMessage("[ZF] Success Register")

#define RegisterSurvivorPerk(%1) RegisterPerk(%1, g_SurPerkRegistry, g_SurPerkTypes, "survivor")
#define RegisterZombiePerk(%1) RegisterPerk(%1, g_ZomPerkRegistry, g_ZomPerkTypes, "zombie")

#include "perks/Registraion.inc"
#include "perks/perk_structs.inc"
#include "perks/BasePerk.inc"
#include <sdkhooks>

#include <sdktools>

#include <sourcemod>

#include <tf2_stocks>

#include <tf2>
// 全局变量声明
int zf_perkAlphaMaster[MAXPLAYERS+1]; // 记录每个僵尸的主控者（零号僵尸）
//
// Perk Objects
//
#define ZF_PERK_NONE            0
  


// int g_hPerks[MAXPLAYERS+1];   // 存储每个玩家的Perk对象



// Survivor perks
stock int GetTotalSurPerks() {
    return g_SurPerkTypes.Length;
}

// Zombie perks
stock int GetTotalZomPerks() {
    return g_ZomPerkTypes.Length;
}

// State
int zf_frameCounter;
int zf_surPerksEnabled;
int zf_zomPerksEnabled;
int zf_surPerksLimit[22];
int zf_zomPerksLimit[18];
int zf_perkMode;
int zf_perkPendingMode;
int zf_perkRandSurPerk;
int zf_perkRandZomPerk;
int zf_perkTeamSurPerk;
int zf_perkTeamZomPerk;
int zf_menuPerk[MAXPLAYERS+1];

// Logic
int zf_lastAttack[MAXPLAYERS+1];
int zf_lastButtons[MAXPLAYERS+1];
int zf_lastHealth[MAXPLAYERS+1];
int zf_lastKiller[MAXPLAYERS+1];
int zf_lastPoison[MAXPLAYERS+1];
int zf_lastTeam[MAXPLAYERS+1];

int zf_perkTimer[MAXPLAYERS+1];           // Timer shared by many perks
int zf_perkState[MAXPLAYERS+1];           // State shared by many perks
float zf_perkPos[MAXPLAYERS+1][5][3]; // Position array shared by many perks
char zf_perkStr[MAXPLAYERS+1][32];  // String shared by many perks

int zf_stat[MAXPLAYERS+1][ZFStat][ZFStatType];
int zf_cond[MAXPLAYERS+1][ZFCond];

// FX.Entities
int zf_aura[MAXPLAYERS+1];

#define ICON_SPR 0
#define ICON_ANC 1
int zf_icon[MAXPLAYERS+1][2];

int zf_item[MAXPLAYERS+1][MAX_ITEMS];

// FX.HUD
Handle zf_hudLine0;
Handle zf_hudLine1;
Handle zf_hudLine2;

// Menus
Handle zf_menuSurPerkList;
Handle zf_menuZomPerkList;

// CVARS
Handle zf_cvCripple;

////////////////////////////////////////////////////////////
//
// Perk Registration
//
////////////////////////////////////////////////////////////
stock void GetPerkInfoString(ArrayList typeList, int index, const char[] key, char[] buffer, int maxLen)
{
    if (index < 0 || index >= typeList.Length)
    {
        strcopy(buffer, maxLen, "");
        return;
    }

    StringMap perkInfo = view_as<StringMap>(typeList.Get(index));
    if (perkInfo != null)
    {
        perkInfo.GetString(key, buffer, maxLen);
    }
    else
    {
        strcopy(buffer, maxLen, "");
    }
}

stock void GetSurPerkName(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkTypes, index, "name", buffer, maxLen); }
stock void GetSurPerkShortDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkTypes, index, "shortDesc", buffer, maxLen); }
stock void GetSurPerkLongDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkTypes, index, "longDesc", buffer, maxLen); }

stock void GetZomPerkName(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "name", buffer, maxLen); }
stock void GetZomPerkShortDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "shortDesc", buffer, maxLen); }
stock void GetZomPerkLongDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "longDesc", buffer, maxLen); }





//
// Perk Init
//
////////////////////////////////////////////////////////////
public void perkInit()
{
  LogMessage("[ZF DEBUG] perkInit: Starting perk system initialization.");
  g_SurPerkTypes = new ArrayList();
  g_SurPerkRegistry = new StringMap();
  g_ZomPerkTypes = new ArrayList();
  g_ZomPerkRegistry = new StringMap();
  registerSurvivorPerks();
  registerZombiePerks();
  
  // Initialize game state
  zf_frameCounter = 0;
  zf_surPerksEnabled = 0xFFFF_FFFF;
  zf_zomPerksEnabled = 0xFFFF_FFFF;
  for(int i = 0; i < GetTotalSurPerks(); i++)
    zf_surPerksLimit[i] = -1;
  for(int i = 0; i < GetTotalZomPerks(); i++)
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
  RegAdminCmd("sm_zf_perk_enable", command_zfPerkEnable, ADMFLAG_GENERIC, "Enables specified perk. Changes apply on int round.");
  // [all|allsur|allzom|<perk>]
  RegAdminCmd("sm_zf_perk_disable", command_zfPerkDisable, ADMFLAG_GENERIC, "Disables specified perk. Changes apply on int round.");
  // [all|allsur|allzom|<perk>] [<limit>]
  RegAdminCmd("sm_zf_perk_limit", command_zfPerkLimit, ADMFLAG_GENERIC, "Sets limit for specified perk. -1 = Unlimited, 0 = None, >0 = Limit. Changes fully apply on int round.");
  
  // Client Commands
  // [<perk>]
  AddCommandListener(hook_zfSelectPerk, "zf_perk_select");
  
  // Register CVARS
  zf_cvCripple = CreateConVar("sm_zf_cripple", "0", "0 = Crippling backstab disabled, 1 = Crippling backstab enabled.", FCVAR_REPLICATED|FCVAR_NOTIFY, true, 0.0, true, 1.0);
  LogMessage("[ZF DEBUG] perkInit: Perk system initialization finished.");
}

////////////////////////////////////////////////////////////
//
// Admin Command Handlers
//
///////////////////////////////////////////////////////////
public Action command_zfPerkSetMode(int client, int args)
{
  char cmd[32];
  
  if(args == 0)
  {
    //
    // Display current mode.
    //
    ReplyToCommand(client, "%T", "ZF_Perk_Mode_Current", LANG_SERVER, zf_perkMode);
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
      ReplyToCommand(client, "%T", "ZF_Perk_Mode_Set_Normal", LANG_SERVER);
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "1", false) || StrEqual(cmd, "randplayer", false))
    {
      zf_perkPendingMode = 1;
      ReplyToCommand(client, "%T", "ZF_Perk_Mode_Set_RandPlayer", LANG_SERVER);
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "2", false) || StrEqual(cmd, "randteam", false))
    {
      zf_perkPendingMode = 2;
      ReplyToCommand(client, "%T", "ZF_Perk_Mode_Set_RandTeam", LANG_SERVER);
      return Plugin_Handled;
    } 
    if(StrEqual(cmd, "3", false) || StrEqual(cmd, "cvarteam", false))
    {
      zf_perkPendingMode = 3;
      ReplyToCommand(client, "%T", "ZF_Perk_Mode_Set_CvarTeam", LANG_SERVER);
      return Plugin_Handled;
    }      
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Mode_Usage", LANG_SERVER, cmd);
  return Plugin_Handled;
}

public Action command_zfPerkSetTeamSurPerk(int client, int args)
{
  char cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(int i = 0; i < GetTotalSurPerks(); i++)
    {
      char perkName[64];
      GetSurPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_perkTeamSurPerk = i;
        ReplyToCommand(client, "%T", "ZF_Perk_Team_Set_Sur", LANG_SERVER, perkName);
        return Plugin_Handled;
      }
    }
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Team_Usage_Sur", LANG_SERVER, cmd);
  return Plugin_Handled;  
}

public Action command_zfPerkSetTeamZomPerk(int client, int args)
{
  char cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(int i = 0; i < GetTotalZomPerks(); i++)
    {
      char perkName[64];
      GetZomPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_perkTeamZomPerk = i;
        ReplyToCommand(client, "%T", "ZF_Perk_Team_Set_Zom", LANG_SERVER, perkName);
        return Plugin_Handled;
      }
    }
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Team_Usage_Zom", LANG_SERVER, cmd);
  return Plugin_Handled;  
}

public Action command_zfPerkList(int client, int args)
{
  char cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    bool listSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    bool listZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    if(listSur)
    {
      ReplyToCommand(client, "%T", "ZF_Perk_List_Sur_Header", LANG_SERVER);
      for(int i = 1; i < GetTotalSurPerks(); i++)
      {
        char perkName[64];
        GetSurPerkName(i, perkName, sizeof(perkName));
        if(surPerkEnabled(i))
          ReplyToCommand(client, "%T", "ZF_Perk_List_Item", LANG_SERVER, perkName, zf_surPerksLimit[i]);
        else
          ReplyToCommand(client, "%T", "ZF_Perk_List_Item_Disabled", LANG_SERVER, perkName, zf_surPerksLimit[i]);
      }
    } 
    if(listZom)
    {
      ReplyToCommand(client, "%T", "ZF_Perk_List_Zom_Header", LANG_SERVER);
      for(int i = 1; i < GetTotalZomPerks(); i++)
      {
        char perkName[64];
        GetZomPerkName(i, perkName, sizeof(perkName));
        if(zomPerkEnabled(i))
          ReplyToCommand(client, "%T", "ZF_Perk_List_Item", LANG_SERVER, perkName, zf_zomPerksLimit[i]);
        else
          ReplyToCommand(client, "%T", "ZF_Perk_List_Item_Disabled", LANG_SERVER, perkName, zf_zomPerksLimit[i]);
      }
    }
    
    if(listSur | listZom)
      return Plugin_Handled;
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_List_Usage", LANG_SERVER, cmd);
  return Plugin_Handled;
}

public Action command_zfPerkEnable(int client, int args)
{
  command_perkUpdate(client, args, true);
  return Plugin_Handled;
}

public Action command_zfPerkDisable(int client, int args)
{
  command_perkUpdate(client, args, false);
  return Plugin_Handled;
}

public Action command_perkUpdate(int client, int args, bool doEnable)
{
  char cmd[32];
  
  if(args == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));

    bool setSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    bool setZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    char strState[16];
    strState = doEnable ? "Enabled" : "Disabled";
    
    //
    // Enable/Disable groups of perks.
    //
    if(setSur)
    {
      zf_surPerksEnabled = doEnable ? 0xFFFF_FFFF : 0x0000_0001;
      ReplyToCommand(client, "%T", "ZF_Perk_Enable_All_Sur", LANG_SERVER, strState);
    }    
    if(setZom)
    {
      zf_zomPerksEnabled = doEnable ? 0xFFFF_FFFF : 0x0000_0001;
      ReplyToCommand(client, "%T", "ZF_Perk_Enable_All_Zom", LANG_SERVER, strState);
    }
    if(setSur || setZom)
    {
      return Plugin_Handled;
    }

    //
    // Enable/Disable single perk.
    //
    for(int i = 1; i < GetTotalSurPerks(); i++)
    {
      char perkName[64];
      GetSurPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_surPerksEnabled = doEnable ? (zf_surPerksEnabled | (1 << i)) : (zf_surPerksEnabled & ~(1 << i));
        ReplyToCommand(client, "%T", "ZF_Perk_Enable_Single", LANG_SERVER, strState, perkName);
        return Plugin_Handled;
      }
    }
    for(int i = 1; i < GetTotalZomPerks(); i++)
    {
      char perkName[64];
      GetZomPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_zomPerksEnabled = doEnable ? (zf_zomPerksEnabled | (1 << i)) : (zf_zomPerksEnabled & ~(1 << i));
        ReplyToCommand(client, "%T", "ZF_Perk_Enable_Single", LANG_SERVER, strState, perkName);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "%T", "ZF_Perk_Invalid", LANG_SERVER, cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Enable_Usage", LANG_SERVER, cmd);
  return Plugin_Handled;  
}

public Action command_zfPerkLimit(int client, int args)
{
  char cmd[32];
  char cmd2[32];
  
  if(args == 2)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    GetCmdArg(2, cmd2, sizeof(cmd2));

    bool setSur = StrEqual(cmd, "all", false) || StrEqual(cmd, "allsur", false);
    bool setZom = StrEqual(cmd, "all", false) || StrEqual(cmd, "allzom", false);
    int limit = StringToInt(cmd2);
        
    //
    // Limit groups of perks.
    //
    if(setSur)
    {
      for(int i = 1; i < GetTotalSurPerks(); i++)
        zf_surPerksLimit[i] = limit;
      ReplyToCommand(client, "%T", "ZF_Perk_Limit_All_Sur", LANG_SERVER, limit);
    }    
    if(setZom)
    {
      for(int i = 1; i < GetTotalZomPerks(); i++)
        zf_zomPerksLimit[i] = limit;
      ReplyToCommand(client, "%T", "ZF_Perk_Limit_All_Zom", LANG_SERVER, limit);
    }
    if(setSur || setZom)
    {
      return Plugin_Handled;
    }

    //
    // Limit single perk.
    //
    for(int i = 1; i < GetTotalSurPerks(); i++)
    {
      char perkName[64];
      GetSurPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_surPerksLimit[i] = limit;
        ReplyToCommand(client, "%T", "ZF_Perk_Limit_Single", LANG_SERVER, perkName, limit);
        return Plugin_Handled;
      }
    }
    for(int i = 1; i < GetTotalZomPerks(); i++)
    {
      char perkName[64];
      GetZomPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        zf_zomPerksLimit[i] = limit;
        ReplyToCommand(client, "%T", "ZF_Perk_Limit_Single", LANG_SERVER, perkName, limit);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "%T", "ZF_Perk_Invalid", LANG_SERVER, cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Limit_Usage", LANG_SERVER, cmd);
  return Plugin_Handled;    
}

////////////////////////////////////////////////////////////
//
// Client Command Handlers
//
///////////////////////////////////////////////////////////
public Action hook_zfSelectPerk(int client, const char[] command, int argc)
{
  char cmd[32];

  //
  // Select a single survivor/zombie perk.
  //
  if(argc == 1)
  {
    GetCmdArg(1, cmd, sizeof(cmd));
    
    for(int i = 1; i < GetTotalSurPerks(); i++)
    {
      char perkName[64];
      GetSurPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        selectSurPerk(client, i);
        return Plugin_Handled;
      }
    }
    
    for(int i = 1; i < GetTotalZomPerks(); i++)
    {
      char perkName[64];
      GetZomPerkName(i, perkName, sizeof(perkName));
      if(StrEqual(cmd, perkName, false))
      {
        selectZomPerk(client, i);
        return Plugin_Handled;
      }
    }
    
    char unselectedName[64];
    GetSurPerkName(0, unselectedName, sizeof(unselectedName));
    if(StrEqual(cmd, unselectedName, false))
    {
      selectSurPerk(client, 0);
      selectZomPerk(client, 0);
      return Plugin_Handled;
    }
  }
  
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%T", "ZF_Perk_Select_Usage", LANG_SERVER, cmd);
  return Plugin_Handled;
}

////////////////////////////////////////////////////////////
//
// Perk Stat Routines
//
////////////////////////////////////////////////////////////
stock int getStat(int client, ZFStat stat)
{ return zf_stat[client][stat][ZFStatTypePerm] + zf_stat[client][stat][ZFStatTypeCond] + zf_stat[client][stat][ZFStatTypeTemp]; }

stock int getStatType(int client, ZFStat stat, ZFStatType type)
{ return zf_stat[client][stat][type]; }

stock void addStat(int client, ZFStat stat, ZFStatType type, int val)
{ zf_stat[client][stat][type] += val; }

// Temporary setters
stock void addStatTempStack(int client, ZFStat stat, int newStr, int newDur)
{ 
  // Note on result:
  // + Strength is averaged across maximum duration (stacks).
  // Note on use:
  // + newStr can be negative or positive.
  // + newDur must be positive.
  if((newStr != 0) && (newDur > 0))
  {
    int oldStr = zf_stat[client][stat][ZFStatTypeTemp];
    int oldDur = zf_stat[client][stat][ZFStatTypeTempDuration]; 
    int total = (oldStr * oldDur) + (newStr * newDur);
    int finalDur = max(oldDur, newDur);
    int finalStr = RoundToCeil(total / float(finalDur));
    
    // Set new temp stats
    zf_stat[client][stat][ZFStatTypeTemp] = finalStr;
    zf_stat[client][stat][ZFStatTypeTempDuration] = finalDur;
  }
  else
  {
    LogError("[ZF] - addStatTempStack() - Invalid newStr (%d) or newDur (%d)", newStr, newDur);
  }
}
stock void addStatTempExtend(int client, ZFStat stat, int newStr, int newDur)
{
  // Note on result:
  // + Strength is averaged across sum of durations (extends).  
  // Note on use:
  // + newStr must be positive.
  // + newDur must be positive.
  if((newStr > 0) && (newDur > 0))
  {
    int oldStr = zf_stat[client][stat][ZFStatTypeTemp];
    int oldDur = zf_stat[client][stat][ZFStatTypeTempDuration]; 
    int total = (oldStr * oldDur) + (newStr * newDur);    
    int finalDur = oldDur + newDur;
    int finalStr = RoundToCeil(total / float(finalDur));
    
    // Set new temp stats
    zf_stat[client][stat][ZFStatTypeTemp] = finalStr;
    zf_stat[client][stat][ZFStatTypeTempDuration] = finalDur;    
  }
  else
  {
    LogError("[ZF] - addStatTempExtend() - Invalid newStr (%d) or newDur (%d)", newStr, newDur);  
  }
}

stock void scaleStatTempPct(int client, ZFStat stat, float strPct, float durPct = 1.0)
{
  zf_stat[client][stat][ZFStatTypeTemp] = RoundToCeil(float(zf_stat[client][stat][ZFStatTypeTemp]) * strPct);
  zf_stat[client][stat][ZFStatTypeTempDuration] = RoundToCeil(float(zf_stat[client][stat][ZFStatTypeTempDuration]) * durPct);
}

////////////////////////////////////////////////////////////
//
// Perk Cond Routines
//
////////////////////////////////////////////////////////////
stock bool getCond(int client, ZFCond cond)
{ return zf_cond[client][cond] > 0; }

stock void addCond(int client, ZFCond cond, int val)
{ zf_cond[client][cond] += val; }

stock void subCond(int client, ZFCond cond, int val)
{ zf_cond[client][cond] = max(0, zf_cond[client][cond] - val); }

////////////////////////////////////////////////////////////
//
// Perk Reset Logic
// + Used to clear variables with no regard to game state.
//
////////////////////////////////////////////////////////////
stock void resetAllClients()
{
  for(int i = 0; i <= MAXPLAYERS; i++)
    resetClient(i);
}

stock void resetClient(int client)
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
  for(int i = 0; i < 5; i++)
    for(int j = 0; j < 3; j++)
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
  // if (g_hPerks[client] != null)
  // {
  //     g_hPerks[client].onRemove();
  //     delete g_hPerks[client];
  //     g_hPerks[client] = null;
  // }
}

stock void resetClientStats(int client)
{
  for(int stat = 0; stat < TOTAL_ZFSTATS; stat++)
    for(int type = 0; type < TOTAL_ZFSTAT_TYPES; type++)
      zf_stat[client][stat][type] = 0;
}

stock void resetStatType(ZFStatType type)
{
  for(int i = 0; i <= MAXPLAYERS; i++)
    resetClientStatType(i, type);
}

stock void resetClientStatType(int client, ZFStatType type)
{
  for(int stat = 0; stat < TOTAL_ZFSTATS; stat++)
    zf_stat[client][stat][type] = 0;
}

stock void resetClientConds(int client)
{  
  for(int cond = 0; cond < TOTAL_ZFCONDS; cond++)
    zf_cond[client][cond] = 0;
}

////////////////////////////////////////////////////////////
//
// Perk Selection Utilities
//
////////////////////////////////////////////////////////////
stock bool surPerkEnabled(int perk)
{ return (zf_surPerksEnabled & (1 << perk)) != 0; }
stock bool zomPerkEnabled(int perk)
{ return (zf_zomPerksEnabled & (1 << perk)) != 0; }
stock bool usingSurPerk(int client, int perk)
{ return (prefGet(client, SurPerk) == perk); }
stock bool usingZomPerk(int client, int perk)
{ return (prefGet(client, ZomPerk) == perk); } 

//
// Called by client when he selects a survivor perk (either from menu or by command).
//
stock void selectSurPerk(int client, int perk)
{
  // Do not select invalid perk.
  if((perk < 0) || (perk >= GetTotalSurPerks()))
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Invalid", client);
  // Do not select disabled perk.
  else if(!surPerkEnabled(perk))
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Disabled_Sur", client, perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Gamemode_Sur", client, perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(surPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Limit_Sur", client, perkName, zf_surPerksLimit[perk]);
  }
  // Select perk.
  else
  {
    prefSet(client, SurPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    // Defer perk selection for non-survivors.
    if(!isSur(client))
      PrintToChat(client, "%T", "ZF_Perk_Select_Success_Sur", client, perkName);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "%T", "ZF_Perk_Select_Success_Sur", client, perkName);
    // Respawn client. This will trigger new perk selection.
    else
      TF2_RespawnPlayer(client);
  }
}

//
// Called by client when he selects a zombie perk (either from menu or by command).
//
stock void selectZomPerk(int client, int perk)
{
  // Do not select invalid perk.
  if((perk < 0) || (perk >= GetTotalZomPerks()))
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Invalid", client);
  // Do not select disabled perk.
  else if(!zomPerkEnabled(perk))
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Disabled_Zom", client, perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Gamemode_Zom", client, perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(zomPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Limit_Zom", client, perkName, zf_zomPerksLimit[perk]);
  }
  // Select perk.
  else
  {
    prefSet(client, ZomPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    // Defer perk selection for non-zombies.
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    if(!isZom(client))
      PrintToChat(client, "%T", "ZF_Perk_Select_Success_Zom", client, perkName);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "%T", "ZF_Perk_Select_Success_Zom", client, perkName);
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
stock bool surPerkAtLimit(int client, int perk)
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
    int total = 0;
    for(int i = 1; i <= MaxClients; i++)
      if((i != client) && IsClientInGame(i) && isSur(i) && usingSurPerk(i, perk))
          total++;
  
    return (total >= zf_surPerksLimit[perk]);
  }
}

stock bool zomPerkAtLimit(int client, int perk)
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
    int total = 0;
    for(int i = 1; i <= MaxClients; i++)
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
void updateHud(int client)
{
  char strHudPerk[48];
  char strHudBonuses[32];
  
  if(IsClientInGame(client) && !(zf_lastButtons[client] & IN_SCORE))
  {
    if(isSur(client) || isZom(client))
    {
      if(IsPlayerAlive(client))
      {
        // HUD Element: Perk Selection
        if(isSur(client))
        {
          char perkName[48];
          GetSurPerkName(prefGet(client, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else if(isZom(client))
        {
          char perkName[48];
          GetZomPerkName(prefGet(client, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.90, 2.1, 200, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s %s", strHudPerk, zf_perkStr[client]);
        
        // HUD Element: Bonuses 1 (Attack, Defense)
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses1", client, getStat(client, ZFStatAtt), getStat(client, ZFStatDef));
        SetHudTextParams(0.15, 0.93, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine1, "%s", strHudBonuses);
        
        // HUD Element: Bonuses 2 (Crit, Speed)
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses2", client, getStat(client, ZFStatCrit), getStat(client, ZFStatSpeed));
        SetHudTextParams(0.15, 0.96, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine2, "%s", strHudBonuses);
      }
      else
      {
        // HUD Element: Killer's perk
        int killer = zf_lastKiller[client];
        if(validClient(killer) && (killer != client))
        {
          if(isSur(killer))
          {
            char perkName[48];
            GetSurPerkName(prefGet(killer, SurPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else if(isZom(killer))
          {
            char perkName[48];
            GetZomPerkName(prefGet(killer, ZomPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else
          {
            strHudPerk = "";
          }
          SetHudTextParams(0.15, 0.90, 2.1, 250, 200, 200, 150);
          ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);
        }
      }
    }
    else
    {
      // HUD Element: Specator target's perk
      int spectate = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
      if(validClient(spectate))
      {
        if(isSur(spectate))
        {
          char perkName[48];
          GetSurPerkName(prefGet(spectate, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else if(isZom(spectate))
        {
          char perkName[48];
          GetZomPerkName(prefGet(spectate, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.90, 2.1, 250, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);
      }
    }
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
Handle perk_buildSurPerkListMenu()
{
  LogMessage("[ZF DEBUG] perk_buildSurPerkListMenu: Building survivor perk list menu.");
  Handle menu = CreateMenu(perk_menuSurPerkList);
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectSurPerk_Title", LANG_SERVER);
    SetMenuTitle(menu, title);

    // Perks
    int perkCount = GetTotalSurPerks();
    LogMessage("[ZF DEBUG] perk_buildSurPerkListMenu: Found %d survivor perks to add.", perkCount);
    for(int i = 0; i < perkCount; i++)
    {
      char name[64], shortDesc[128];
      GetSurPerkName(i, name, sizeof(name));
      GetSurPerkShortDesc(i, shortDesc, sizeof(shortDesc));
      AddMenuItem(menu, shortDesc, name);
    }
  }
  else
  {
    LogError("[ZF DEBUG] perk_buildSurPerkListMenu: Failed to create menu handle.");
  }
  LogMessage("[ZF DEBUG] perk_buildSurPerkListMenu: Menu handle is %x.", menu);
  return menu;
}

public void perk_menuSurPerkList(Handle menu, MenuAction action, int param1, int param2)
{
  if(action == MenuAction_Select)
  {
    if (param2 == 0) // "None" option
    {
        selectSurPerk(param1, 0);
    }
    else
    {
        panel_PrintSurPerkSelect(param1, param2);
    }
  }
}

//
// Zombie Perk List Menu
// (Menu item N is perk N+1)
//
Handle perk_buildZomPerkListMenu()
{
  LogMessage("[ZF DEBUG] perk_buildZomPerkListMenu: Building zombie perk list menu.");
  Handle menu = CreateMenu(perk_menuZomPerkList);
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectZomPerk_Title", LANG_SERVER);
    SetMenuTitle(menu, title);

    // Perks
    int perkCount = GetTotalZomPerks();
    LogMessage("[ZF DEBUG] perk_buildZomPerkListMenu: Found %d zombie perks to add.", perkCount);
    for(int i = 0; i < perkCount; i++)
    {
      char name[64], shortDesc[128];
      GetZomPerkName(i, name, sizeof(name));
      GetZomPerkShortDesc(i, shortDesc, sizeof(shortDesc));
      AddMenuItem(menu, name, shortDesc);
    }

  }
  else
  {
    LogError("[ZF DEBUG] perk_buildZomPerkListMenu: Failed to create menu handle.");
  }
  LogMessage("[ZF DEBUG] perk_buildZomPerkListMenu: Menu handle is %x.", menu);
  return menu;
}

public void perk_menuZomPerkList(Handle menu, MenuAction action, int param1, int param2)
{
  if(action == MenuAction_Select)
  {
    if (param2 == 0) // "None" option
    {
        selectZomPerk(param1, 0);
    }
    else
    {
        panel_PrintZomPerkSelect(param1, param2);
    }
  }
}

// 
// Survivor Perk Select Menu
//
public void panel_PrintSurPerkSelect(int client, int perk)
{
  Handle panel = CreatePanel();
  
  char title[128];
  Format(title, sizeof(title), "%T", "ZF_Menu_PerkSelect_Title", LANG_SERVER);
  SetPanelTitle(panel, title);
  char name[64], longDesc[1024];
  GetSurPerkName(perk, name, sizeof(name));
  GetSurPerkLongDesc(perk, longDesc, sizeof(longDesc));
  DrawPanelText(panel, name);
  DrawPanelText(panel, longDesc);
  if(surPerkEnabled(perk))
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Select", LANG_SERVER);
    DrawPanelItem(panel, buffer, ITEMDRAW_DEFAULT);
  }
  else
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Disabled", LANG_SERVER);
    DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
  }
  char backBuffer[128];
  Format(backBuffer, sizeof(backBuffer), "%T", "ZF_Menu_PerkSelect_Back", LANG_SERVER);
  DrawPanelItem(panel, backBuffer);
  char closeBuffer[128];
  Format(closeBuffer, sizeof(closeBuffer), "%T", "ZF_Menu_PerkSelect_Close", LANG_SERVER);
  DrawPanelItem(panel, closeBuffer);
  
  zf_menuPerk[client] = perk;
  SendPanelToClient(panel, client, panel_HandleSurPerkSelect, MENU_TIME_FOREVER);
  CloseHandle(panel);
}

public void panel_HandleSurPerkSelect(Handle menu, MenuAction action, int param1, int param2)
{ 
  if(action == MenuAction_Select)
  {
    switch(param2)
    {
      case 1: 
        selectSurPerk(param1, zf_menuPerk[param1]);
      case 2: 
      {
        int firstItem = (((zf_menuPerk[param1] - 1) / 7) * 7);
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
public void panel_PrintZomPerkSelect(int client, int perk)
{
  Handle panel = CreatePanel();
  
  char title[128];
  Format(title, sizeof(title), "%T", "ZF_Menu_PerkSelect_Title", LANG_SERVER);
  SetPanelTitle(panel, title);
  char name[64], longDesc[1024];
  GetZomPerkName(perk, name, sizeof(name));
  GetZomPerkLongDesc(perk, longDesc, sizeof(longDesc));
  DrawPanelText(panel, name);
  DrawPanelText(panel, longDesc);
  if(zomPerkEnabled(perk))
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Select", LANG_SERVER);
    DrawPanelItem(panel, buffer, ITEMDRAW_DEFAULT);
  }
  else
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Disabled", LANG_SERVER);
    DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
  }
  char backBuffer[128];
  Format(backBuffer, sizeof(backBuffer), "%T", "ZF_Menu_PerkSelect_Back", LANG_SERVER);
  DrawPanelItem(panel, backBuffer);
  char closeBuffer[128];
  Format(closeBuffer, sizeof(closeBuffer), "%T", "ZF_Menu_PerkSelect_Close", LANG_SERVER);
  DrawPanelItem(panel, closeBuffer);
  
  zf_menuPerk[client] = perk;
  SendPanelToClient(panel, client, panel_HandleZomPerkSelect, MENU_TIME_FOREVER);
  CloseHandle(panel);
}

public void panel_HandleZomPerkSelect(Handle menu, MenuAction action, int param1, int param2)
{ 
  if(action == MenuAction_Select)
  {
    switch(param2)
    {
      case 1: 
        selectZomPerk(param1, zf_menuPerk[param1]);
      case 2: 
      {
        int firstItem = (((zf_menuPerk[param1] - 1) / 7) * 7);
        DisplayMenuAtItem(zf_menuZomPerkList, param1, firstItem, MENU_TIME_FOREVER); 
      }
      default: 
        return;   
    } 
  }
}


////////////////////////////////////////////////////////////
//
// Perk Bonus Update Logic
//
////////////////////////////////////////////////////////////
stock void updateClientPermStats(int client)
{
  resetClientStatType(client, ZFStatTypePerm);

  // if (g_hPerks[client] != null) {
  //   g_hPerks(client).updateClientPermStats();
  // }
  //
  // Apply permanent bonuses for survivors.
  // (Survivors must be alive)
  //
  if(validLivingSur(client))
  {
  }
    
  //
  // Apply permanent bonuses for zombies.
  // (Zombies can be dead or alive)
  //
  else if(validZom(client))
  {
  }
}

stock void updateClientPermEffects(int client)
{
  float headOffset[3] = {0.0, 0.0, 15.0};
  
  // Handle survivor effects.
  if(validLivingSur(client))
  {            
    
  }
  
  // Handle zombie effects.
  else if(validZom(client))
  {
      
  }
}


// BasePerk.inc
// Generated from D:\workspace\code\tf2\zf\src\python\perks\BasePerk.py

#if defined __BasePerk_included
#endinput
#endif
#define __BasePerk_included

#include "../zf_util_base.inc"
#include <adt_trie>

#define BASE_PERK_NAME "Unselected"
#define BASE_PERK_SHORTDESC "Unselected"
#define BASE_PERK_DESC "Please select one perk to check info"

stock StringMap BasePerk_new(int client) {
    StringMap sm = new StringMap();
    setParam(sm, "owner", client);
    setParamString(sm, "test", "orrrrr");
    return sm;
}

stock void BasePerk_getName(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_NAME);
}

stock void BasePerk_getShortdesc(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_SHORTDESC);
}

stock void BasePerk_getDesc(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_DESC);
}

stock void BasePerk_updateClientPermStats(StringMap self) {
}



// AthleticPerk.inc
// Generated from D:\workspace\code\tf2\zf\src\python\perks\survival\AthleticPerk.py

#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <adt_trie>

#define ZF_ATHLETIC_ATTACK -40
#define ZF_ATHLETIC_CRIT -100
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100
#define ATHLETIC_PERK_NAME "Athletic"
#define ATHLETIC_PERK_SHORTDESC "Faster movement and ROF"
#define ATHLETIC_PERK_DESC "Faster movement and ROF!!"

stock StringMap AthleticPerk_new(int client) {
    StringMap sm = SurvivorBasePerk_new(client);
    setParamString(sm, "test", "okkk");
    return sm;
}

stock void AthleticPerk_getName(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_NAME);
}

stock void AthleticPerk_getShortdesc(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_SHORTDESC);
}

stock void AthleticPerk_getDesc(StringMap self, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_DESC);
}

stock void AthleticPerk_updateClientPermStats(StringMap self) {
    addStat(getParam(self, "owner"), ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(getParam(self, "owner"), ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(getParam(self, "owner"), ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(getParam(self, "owner"), ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}

想办法让zf_perk.inc对接BasePerk.inc及其相关perk，不能修改BasePerk.inc及其相关perk。可以使用宏等方法
