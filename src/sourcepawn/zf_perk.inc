/* put the line below after all of the includes!
#pragma newdecls required
*/

#pragma newdecls required

#if defined _ZF_PERK_INC
  #endinput
#endif
#define _ZF_PERK_INC

#include <tf2_stocks>
#include <sourcemod>
#include <sdkhooks>
#include <sdktools>
#include <tf2>
#include <tf2_stocks>

#include "zf_util_base.inc"
#include "zf_util_fx.inc"
#include "zf_util_pref.inc"
#include "zf_zombie_visuals.inc"
#include "perks/zombie/MarkedPerk.inc"
#include "perks/zombie/SwarmingPerk.inc"
ArrayList g_SurPerkTypes;
StringMap g_SurPerkRegistry;
ArrayList g_ZomPerkTypes;
StringMap g_ZomPerkRegistry;

typedef PerkCreator = function BasePerk (int client);
ArrayList g_SurPerkCreators;
ArrayList g_ZomPerkCreators;


#include "perk_structs.inc"

// 全局变量声明
int zf_perkAlphaMaster[MAXPLAYERS+1]; // 记录每个僵尸的主控者（零号僵尸）
//
// Perk Objects
//
#define ZF_PERK_NONE            0
  

BasePerk g_hPerks[MAXPLAYERS+1];

// State
int zf_frameCounter;
int zf_surPerksEnabled;
int zf_zomPerksEnabled;
int zf_surPerksLimit[64];
int zf_zomPerksLimit[64];
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

#include "perks/BasePerk.inc"
#include "perks/Registraion.inc"

// Survivor perks
stock int GetTotalSurPerks() {
    return g_SurPerkTypes.Length;
}

// Zombie perks
stock int GetTotalZomPerks() {
    return g_ZomPerkTypes.Length;
}

// FX.HUD
Handle zf_hudLine0;
Handle zf_hudLine1;
Handle zf_hudLine2;

// Menus
Handle zf_menuSurPerkList;
Handle zf_menuZomPerkList;

// CVARS
Handle hCvarCripple;

////////////////////////////////////////////////////////////
//
// Perk Registration
//
////////////////////////////////////////////////////////////

stock void ZF_LogDebug(const char[] format, any...)
{
#if defined ZF_DEBUG
    char buffer[256];
    VFormat(buffer, sizeof(buffer), format, 2);
    LogMessage("[ZF DEBUG] %s", buffer);
#endif
}

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
stock void GetSurPerkDisplayName(int client, int index, char[] buffer, int maxlen) {
    char name[64];
    GetSurPerkName(index, name, sizeof(name));
    Format(buffer, maxlen, "%T", name, client);
}

stock void GetZomPerkName(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "name", buffer, maxLen); }
stock void GetZomPerkShortDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "shortDesc", buffer, maxLen); }
stock void GetZomPerkLongDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkTypes, index, "longDesc", buffer, maxLen); }
stock void GetZomPerkDisplayName(int client, int index, char[] buffer, int maxlen) {
    char name[64];
    GetZomPerkName(index, name, sizeof(name));
    Format(buffer, maxlen, "%T", name, client);
}





//
// Perk Init
//
////////////////////////////////////////////////////////////
public void perkInit()
{

  g_SurPerkTypes = new ArrayList();
  g_SurPerkRegistry = new StringMap();
  g_SurPerkCreators = new ArrayList();
  g_ZomPerkTypes = new ArrayList();
  g_ZomPerkRegistry = new StringMap();
  g_ZomPerkCreators = new ArrayList();
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
  hCvarCripple = CreateConVar("sm_zf_cripple", "0", "0 = Crippling backstab disabled, 1 = Crippling backstab enabled.", FCVAR_REPLICATED|FCVAR_NOTIFY, true, 0.0, true, 1.0);

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
      char dispalyName[64];
      GetSurPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
      char dispalyName[64];
      GetZomPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
        GetSurPerkDisplayName(client, i, perkName, sizeof(perkName));
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
        GetZomPerkDisplayName(client, i, perkName, sizeof(perkName));
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
      char dispalyName[64];
      GetSurPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
      char dispalyName[64];
      GetZomPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
      char dispalyName[64];
      GetSurPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
      char dispalyName[64];
      GetZomPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
      char dispalyName[64];
      GetSurPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
      {
        selectSurPerk(client, i);
        return Plugin_Handled;
      }
    }
    
    for(int i = 1; i < GetTotalZomPerks(); i++)
    {
      char perkName[64];
      GetZomPerkName(i, perkName, sizeof(perkName));
      char dispalyName[64];
      GetZomPerkDisplayName(client, i, dispalyName, sizeof(dispalyName));
      if(StrEqual(cmd, perkName, false) || StrEqual(cmd, dispalyName, false))
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
  RemoveZombieVisuals(client);

  // New Perk Object
  if (g_hPerks[client] != null)
  {
      g_hPerks[client].onRemove();
      delete g_hPerks[client];
      g_hPerks[client] = null;
  }

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
    GetSurPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Disabled_Sur", client, perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetSurPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Gamemode_Sur", client, perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(surPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetSurPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Limit_Sur", client, perkName, zf_surPerksLimit[perk]);
  }
  // Select perk.
  else
  {
    prefSet(client, SurPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    char perkName[64];
    GetSurPerkDisplayName(client, perk, perkName, sizeof(perkName));
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
    GetZomPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Disabled_Zom", client, perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetZomPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Gamemode_Zom", client, perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(zomPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetZomPerkDisplayName(client, perk, perkName, sizeof(perkName));
    PrintToChat(client, "%T", "ZF_Perk_Select_Error_Limit_Zom", client, perkName, zf_zomPerksLimit[perk]);
  }
  // Select perk.
  else
  {
    prefSet(client, ZomPendPerk, perk);
    prefSet(client, PerkSelectMode, zf_perkMode);
    
    // Defer perk selection for non-zombies.
    char perkName[64];
    GetZomPerkDisplayName(client, perk, perkName, sizeof(perkName));
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
void clearHud(int client)
{
  if (IsClientInGame(client))
  {
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine0, "");
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine1, "");
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine2, "");
  }
}

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
          GetSurPerkDisplayName(client, prefGet(client, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else if(isZom(client))
        {
          char perkName[48];
          GetZomPerkDisplayName(client, prefGet(client, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.80, 2.1, 200, 200, 200, 150);
        if (strcmp(zf_perkStr[client], "") != 0)
          ShowSyncHudText(client, zf_hudLine0, "%s (%s)", strHudPerk, zf_perkStr[client]);
        else 
          ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        
        // HUD Element: Bonuses 1 (Attack, Defense)
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses1", client, getStat(client, ZFStatAtt), getStat(client, ZFStatDef));
        SetHudTextParams(0.15, 0.83, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine1, "%s", strHudBonuses);
        
        // HUD Element: Bonuses 2 (Crit, Speed)
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses2", client, getStat(client, ZFStatCrit), getStat(client, ZFStatSpeed));
        SetHudTextParams(0.15, 0.86, 2.1, 150, 150, 150, 150);
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
            GetSurPerkDisplayName(client, prefGet(killer, SurPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else if(isZom(killer))
          {
            char perkName[48];
            GetZomPerkDisplayName(client, prefGet(killer, ZomPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else
          {
            strHudPerk = "";
          }
          SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
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
          GetSurPerkDisplayName(client, prefGet(spectate, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else if(isZom(spectate))
        {
          char perkName[48];
          GetZomPerkDisplayName(client, prefGet(spectate, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
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

  Handle menu = CreateMenu(perk_menuSurPerkList);
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectSurPerk_Title", LANG_SERVER);
    SetMenuTitle(menu, title);

    // Perks
    int perkCount = GetTotalSurPerks();
  
    for(int i = 0; i < perkCount; i++)
    {
      char name[128], shortDescKey[128], shortDescTranslated[128];
      GetSurPerkShortDesc(i, shortDescKey, sizeof(shortDescKey));
      Format(shortDescTranslated, sizeof(shortDescTranslated), "%T", shortDescKey, LANG_SERVER);
      AddMenuItem(menu, name, shortDescTranslated);
    }
  }
  else
  {
  
  }

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

  Handle menu = CreateMenu(perk_menuZomPerkList);
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectZomPerk_Title", LANG_SERVER);
    SetMenuTitle(menu, title);

    // Perks
    int perkCount = GetTotalZomPerks();
  
    for(int i = 0; i < perkCount; i++)
    {
      char name[128], shortDescKey[128], shortDescTranslated[128];
      GetZomPerkShortDesc(i, shortDescKey, sizeof(shortDescKey));
      Format(shortDescTranslated, sizeof(shortDescTranslated), "%T", shortDescKey, LANG_SERVER);
      AddMenuItem(menu, name, shortDescTranslated);
    }

  }
  else
  {
  
  }

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
  Format(title, sizeof(title), "%T", "ZF_Menu_PerkSelect_Title", client);
  SetPanelTitle(panel, title);
  char longDescKey[1024], longDescTranslated[1024], displayName[64];
  GetSurPerkDisplayName(client, perk, displayName, sizeof(displayName));
  GetSurPerkLongDesc(perk, longDescKey, sizeof(longDescKey));
  Format(longDescTranslated, sizeof(longDescTranslated), "%T", longDescKey, client);
  DrawPanelText(panel, displayName);
  DrawPanelText(panel, longDescTranslated);
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
  Format(title, sizeof(title), "%T", "ZF_Menu_PerkSelect_Title", client);
  SetPanelTitle(panel, title);
  char longDescKey[1024], longDescTranslated[1024], displayName[64];
  GetZomPerkDisplayName(client, perk, displayName, sizeof(displayName));
  GetZomPerkLongDesc(perk, longDescKey, sizeof(longDescKey));
  Format(longDescTranslated, sizeof(longDescTranslated), "%T", longDescKey, client);
  DrawPanelText(panel, displayName);
  DrawPanelText(panel, longDescTranslated);
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
// Aura Effect Routines
//
////////////////////////////////////////////////////////////
stock void createAura(int client, const char[] strPart, ZFParticleAttachStyle attachStyle, const float offset[3] = {0.0, 0.0, 0.0})
{
  removeAura(client);
  zf_aura[client] = fxCreateParticle(strPart, client, attachStyle, 0.0, offset);
}

stock void removeAura(int client)
{
  fxDeleteParticle(zf_aura[client]);
  zf_aura[client] = -1;
}

stock bool validAura(int client)
{ return fxIsParticleValid(zf_aura[client]); }

stock void showAura(int client)
{ fxStartParticle(zf_aura[client]); }

stock void hideAura(int client)
{ fxStopParticle(zf_aura[client]); }

////////////////////////////////////////////////////////////
//
// Icons Effect Routines
//
////////////////////////////////////////////////////////////
stock void createIcon(int ownerClient, int targetClient, const char[] strSprite)
{
  fxCreateSprite(strSprite, targetClient, zf_icon[ownerClient][ICON_SPR], zf_icon[ownerClient][ICON_ANC]);
}

stock void removeIcon(int client)
{
  fxDeleteSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]);
  zf_icon[client][ICON_SPR] = -1;
  zf_icon[client][ICON_ANC] = -1;
}

stock bool validIcon(int client)
{ return fxIsSpriteValid(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

stock void showIcon(int client)
{ 

  fxShowSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]);
   }

stock void hideIcon(int client)
{ 

  fxHideSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

////////////////////////////////////////////////////////////
//
// Items Effect Routines
//
////////////////////////////////////////////////////////////
stock void removeItem(int client, int item)
{
  if(validItem(client, item))
    fxPuffSmall(zf_item[client][item]);
  fxDeleteModel(zf_item[client][item]);
  zf_item[client][item] = -1;
}

stock void removeItems(int client)
{
  for(int i = 0; i < MAX_ITEMS; i++)
    removeItem(client, i);
}

stock bool validItem(int client, int itemIndex)
{ return fxIsModelValid(zf_item[client][itemIndex]); }

stock int getItemMetadata(int item)
{
  // Utilizes an entity prop that is, in theory, not used.
  return GetEntProp(item, Prop_Send, "m_hEffectEntity");
}

stock void setItemMetadata(int item, int value)
{
  // Utilizes an entity prop that is, in theory, not used.
  SetEntProp(item, Prop_Send, "m_hEffectEntity", value);
}

stock int getFreeItemIndex(int client, int maxItems)
{
  // Return first item index in range [0, maxItems-1] that is free (i.e. not valid).
  // Return -1 if no item in range is free.
  for(int i = 0; i < maxItems; i++)
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
stock void addHealth(int client, int health, bool doOverheal = false)
{
  // Add health up to max (or overheal max [500]).
  int cur = GetClientHealth(client);
  int lim = doOverheal ? 500 : clientMaxHealth(client);
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
stock bool doItemCollide(
  int ent, 
  const float prevPos[3], 
  float hitPos[3], 
  float hitVec[3])
{
  bool didHit = false;
  float thisPos[3];
  float diffPos[3];
  float nextPos[3];
  
  // Use current position(thisPos) and previous position(prevPos)
  // to calculate a difference (diffPos). Normalize and scale
  // difference to compute predicted next position (nextPos).
  getEntityPos(ent, thisPos);
  SubtractVectors(thisPos, prevPos, diffPos);      
  NormalizeVector(diffPos, diffPos);
  ScaleVector(diffPos, 35.0);              
  AddVectors(thisPos, diffPos, nextPos);

  // Trace from current to next position, checking for impacts.
  Handle TraceEx = TR_TraceRayFilterEx(thisPos, nextPos, MASK_SOLID, RayType_EndPoint, TraceFilter);
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

public bool TraceFilter(int ent, int contentMask)
{
  return false;
}


stock int doItemImpact(
  int client, 
  const float hitPos[3], 
  const float hitVec[3], 
  const int color[3])
{
  float hitAng[3];   
  GetVectorAngles(hitVec, hitAng);
  hitAng[0] += 90.0; // Adjust for ZFMDL_HAYBALE
    
  // Place item at hitPos, oriented based on hitVec.
  return fxCreateModelStatic(ZFMDL_HAYBALE, client, true, false, hitPos, hitAng, color); 
}

stock int doItemPlace(int client, const char[] strModel)
{
  float pos[3];
  float ang[3]; 
  
  GetClientAbsOrigin(client, pos); 
  GetClientAbsAngles(client, ang);    

  // Place item at client's feet.
  return fxCreateModelStatic(strModel, client, true, false, pos, ang);
}

stock int doItemThrow(
  int client, 
  const char[] strModel, 
  float force, 
  const int color[3] = {255, ...})
{
  float pos[3];
  float ang[3];
    
  GetClientEyePosition(client, pos);  
  GetClientEyeAngles(client, ang);
  ang[0] += GetRandomFloat(-20.0, -10.0); // Pitch (-90.0 is up, 90.0 is down)
  ang[1] += GetRandomFloat(-6.0, 6.0);    // Yaw

  // Throw item from client's eye position and along look vector.
  return fxCreateModelThrown(strModel, client, pos, ang, force, color);  
}

////////////////////////////////////////////////////////////
//
// Perk Bonus Update Logic
//
////////////////////////////////////////////////////////////
stock void updateClientPermStats(int client)
{
  resetClientStatType(client, ZFStatTypePerm);

  if (g_hPerks[client] != null) {
    char buffer[64];
    g_hPerks[client].getName(buffer, sizeof(buffer));
  
    g_hPerks[client].updateClientPermStats();
  }
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

stock void updateCondStats()
{  
  // 修复: 移除了未使用的变量 j 和 k
  int i;
  int validSurCount;
  int validZomCount;
  int validSurs[MAXPLAYERS+1];
  int validZoms[MAXPLAYERS+1];  
  float validSursPos[MAXPLAYERS+1][3];
  float validZomsPos[MAXPLAYERS+1][3];
  
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
    // 修复: 移除了未使用的变量 thisSur
    int thisSur = validSurs[i];
    if (g_hPerks[thisSur] != null) {
        g_hPerks[thisSur].updateCondStats(zf_perkStr[thisSur], sizeof(zf_perkStr[thisSur]));
    }
  }
   
  // 4. Apply conditional bonuses for zombies.
  for(i = 0; i < validZomCount; i++)
  {
    int thisZom = validZoms[i];
    if (g_hPerks[thisZom] != null) {
        g_hPerks[thisZom].updateCondStats(zf_perkStr[thisZom], sizeof(zf_perkStr[thisZom]));
    }
    
    // Hide auras on cloaked zombies.
    isCloaked(thisZom) ? hideAura(thisZom) : showAura(thisZom);
  }

   
}

stock void updateTempStats()
{  
  for(int i = 0; i < MAXPLAYERS; i++)
  {
    for(int j = 0; j < TOTAL_ZFSTATS; j++)
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

stock void updateConds()
{
  for(int i = 0; i < MAXPLAYERS; i++)
  {
    for(int j = 0; j < TOTAL_ZFCONDS; j++)
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
public void perk_OnPeriodic() // 1Hz
{
  zf_frameCounter++;

  //
  // Build common state information for periodic updates.
  //
  int validSurCount, validZomCount;
  int validSurs[MAXPLAYERS+1], validZoms[MAXPLAYERS+1];
  float validSursPos[MAXPLAYERS+1][3];
  float validZomsPos[MAXPLAYERS+1][3];
  
  validSurCount = 0;
  validZomCount = 0;
  for(int i = 1; i <= MaxClients; i++)
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
  for(int i = 1; i <= MaxClients; i++)
  {
    if(g_hPerks[i] != null && IsClientInGame(i) && IsPlayerAlive(i))
    {
      g_hPerks[i].onPeriodic();
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
  for(int i = 1; i <= MaxClients; i++)
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
        // addStat(i, ZFStatDef, ZFStatTypeCond, ZF_DAZE_DEFEND); // 保留以备将来使用
      }
      if(getCond(i, ZFCondPoisoned))
      {
        // SDKHooks_TakeDamage(i, zf_lastPoison[i], zf_lastPoison[i], float(ZF_POISON_DAMAGE), ZF_DMGTYPE_POISON);
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
    for(int i = 1; i <= MaxClients; i++)
    {
      updateHud(i);
    }
  }
}

public void perk_OnGameFrame()
{
  for(int i = 1; i <= MaxClients; i++)
  {
    if(g_hPerks[i] != null && IsClientInGame(i) && IsPlayerAlive(i))
    {
      g_hPerks[i].onGameFrame();
    }
  }
  
  for(int i = 1; i <= MaxClients; i++)
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
        
                
      } // isZom(i)
    } // Client in game and alive.
  } // for i
}

public void perk_OnMapStart()
{

  utilFxPrecache(); 
  resetAllClients();
}

public void perk_OnMapEnd()
{

  resetAllClients();
}

public void perk_OnClientConnect(int client)
{

  resetClient(client);
  if(prefGet(client, PerkSelectMode) > 1)
    prefSet(client, PerkSelectMode, 0);
}

public void perk_OnClientDisconnect(int client)
{

  // 删除Perk对象
  if (g_hPerks[client] != null)
  {
    g_hPerks[client].onRemove();
    delete g_hPerks[client];
    g_hPerks[client] = null;
  }

  resetClient(client);
}

public void perk_OnRoundStart()
{

  resetAllClients();
 
  //
  // Handle ZF Mode Selection
  //
  zf_perkMode = zf_perkPendingMode;

  //
  // Reset perk select mode.
  //
  for(int i = 1; i <= MaxClients; i++)
    prefSet(i, PerkSelectMode, 0);
        
  //
  // Select random team perks.
  //
  if(zf_perkMode == 2)
  {
    int validSurPerkCount = 0;
    int validZomPerkCount = 0;   
    int validSurPerks[64];
    int validZomPerks[64];
       
    for(int i = 1; i < GetTotalSurPerks(); i++)
      if(surPerkEnabled(i))
        validSurPerks[validSurPerkCount++] = i;
    zf_perkRandSurPerk = (validSurPerkCount == 0) ? ZF_PERK_NONE : validSurPerks[GetRandomInt(0, validSurPerkCount - 1)];

    for(int i = 1; i < GetTotalZomPerks(); i++)
      if(zomPerkEnabled(i))
        validZomPerks[validZomPerkCount++] = i;
    zf_perkRandZomPerk = (validZomPerkCount == 0) ? ZF_PERK_NONE : validZomPerks[GetRandomInt(0, validZomPerkCount - 1)];
  }
}

public void perk_OnGraceEnd()
{

  for(int i = 1; i <= MaxClients; i++)
  {
    if (g_hPerks[i] != null) {
        g_hPerks[i].onGraceEnd();
    }
    //
    // Handle survivor logic.
    // Survivors must be alive, otherwise they will respawn as a zombie.
    //
    if(validLivingSur(i))
    {
      
    }
    
    //
    // Handle zombie logic.
    //
    else if(validZom(i))
    {
      
    }
  }
}

public void perk_OnRoundEnd()
{

  // Destroy all perk objects
  for (int i = 1; i <= MaxClients; i++)
  {
    if (g_hPerks[i] != null)
    {
      g_hPerks[i].onRemove();
      delete g_hPerks[i];
      g_hPerks[i] = null;
    }
  }
  
  resetAllClients();
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Entity Events
//
////////////////////////////////////////////////////////////
public void perk_OnEntityCreated(int entity, const char[] classname)
{
  // ZF_LogDebug("perk_OnEntityCreated: entity=%d, classname=%s", entity, classname);
  if(StrEqual(classname, "env_sprite"))
    SDKHook(entity, SDKHook_Spawn, perk_OnEntitySpawn);
}

public void perk_OnEntitySpawn(int entity)
{
  // ZF_LogDebug("perk_OnEntitySpawn: entity=%d", entity);
  // TODO consider use of m_hEffectEntity for sprites to further filter?
  SDKHook(entity, SDKHook_SetTransmit, perk_OnSetTransmit);
}

public Action perk_OnSetTransmit(int entity, int client)
{
    // ZF_LogDebug("perk_OnSetTransmit: entity=%d, client=%d", entity, client); // Spammy

    // Find the owner of this icon entity
    int owner = -1;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (g_hPerks[i] != null && zf_icon[i][ICON_SPR] == entity)
        {
            owner = i;
            break;
        }
    }

    // If no owner was found, it's not a perk icon, let the game handle it.
    if (owner == -1)
    {
        return Plugin_Continue;
    }

    // Now we have the owner, apply visibility rules.
    char perkName[32];
    g_hPerks[owner].getName(perkName, sizeof(perkName));

    if (StrEqual(perkName, "Friend"))
    {
        // Friend icon is visible to the owner and all survivors
        if (client == owner || isSur(client))
        {
            return Plugin_Continue;
        }
        return Plugin_Handled; // Hide from others (zombies)
    }
    else if (StrEqual(perkName, "Marked"))
    {
        // Marked icon is visible to the owner and all zombies
        if (client == owner || isZom(client))
        {
            return Plugin_Continue;
        }
        return Plugin_Handled; // Hide from others (survivors)
    }

    // Default visibility for all other perk icons: only the owner can see it.
    if (client == owner)
    {
        return Plugin_Continue;
    }
    
    return Plugin_Handled;
}

public Action perk_OnCharitableGiftTouched(int entity, int other)
{
    int owner = -1;
    int itemIndex = -1;

    // Find the owner of the gift
    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            // Charitable perk uses item slots 0-4
            for (int j = 0; j < 5; j++) { // ZF_CHARITABLE_MAX_ITEMS is 5
                if (zf_item[i][j] == entity) {
                    owner = i;
                    itemIndex = j;
                    break;
                }
            }
        }
        if (owner != -1) break;
    }

    // Gift can only be picked up by another living survivor
    if (owner != -1 && validLivingSur(other) && other != owner) {
        // Give the recipient a random stat boost
        int randStat = GetRandomInt(0, TOTAL_ZFSTATS - 1);
        int randBonus = GetRandomInt(10, 30); // ZF_CHARITABLE_GIFT_BONUS_MIN, ZF_CHARITABLE_GIFT_BONUS_MAX
        addStatTempStack(other, view_as<ZFStat>(randStat), randBonus, 20); // ZF_CHARITABLE_GIFT_DURATION
        
        // Give the recipient some health
        addHealth(other, 75, true); // ZF_CHARITABLE_GIFT_BONUS_HEALTH
        
        // TODO: Add a hint message to the recipient about the stat boost.
        // // This requires a global way to get stat names, which is not yet implemented.
        // char statName[32];
        // GetStatName(randStat, statName, sizeof(statName));
        // PrintHintText(other, "You received a %d%% bonus to %s!", randBonus, statName);

        // Clean up
        fxExplosionParty(entity);
        removeItem(owner, itemIndex);
        return Plugin_Handled;
    }

    return Plugin_Continue;
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Player Gameplay Events
//
////////////////////////////////////////////////////////////
public Action OnPlayerRunCmd(int client, int &buttons, int &impulse, float vel[3], float angles[3], int &weapon)
{
  // ZF_LogDebug("OnPlayerRunCmd: client=%d", client); // Spammy
  int prevButtons = buttons;

  
  if (g_hPerks[client] != null) {
    g_hPerks[client].onPlayerRunCmd(buttons, impulse, vel, angles, weapon);
  }
  if(isSur(client))
  {
  }
  else if(isZom(client))
  {
  }
  
  // Save buttons for next call.
  zf_lastButtons[client] = prevButtons;
  return Plugin_Continue;
}

public void perk_OnCalcIsAttackCritical(int client)
{
  // ZF_LogDebug("perk_OnCalcIsAttackCritical: client=%d", client); // Spammy
  // DEBUG
  //PrintToChatAll("[ZF] CalcCrit (%d, slot %d)", client, activeWeaponSlot(client));
  
  if(validLivingSur(client))
  {
    if (g_hPerks[client] != null) {
      g_hPerks[client].onCalcIsAttackCritical();
    }
  }
  
  // Handle general RoF logic.
  zf_lastAttack[client] = 1;
}

public Action perk_OnFenceTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
    int owner = -1;
    int itemIndex = -1;

    // Find the owner of the fence
    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            for (int j = 0; j < 4; j++) { // Assuming CARPENTER_MAX_ITEMS is 4
                if (zf_item[i][j] == victim) {
                    owner = i;
                    itemIndex = j;
                    break;
                }
            }
        }
        if (owner != -1) break;
    }

    if (owner != -1) {
        int fenceHP = getItemMetadata(victim);
        fenceHP -= RoundToCeil(damage);

        if (fenceHP <= 0) {
            fxCreateParticle(ZFPART_PUFFBIG, victim, AttachNone, 4.0, {0.0, 0.0, 4.0});
            fxCreateSoundToAll(ZFSND_WOOD_HIT[GetRandomInt(2, 3)], victim);
            removeItem(owner, itemIndex);
        } else {
            setItemMetadata(victim, fenceHP);
            int fenceFx = (fenceHP * 255) / 500; // Assuming CARPENTER_BARRICADE_HEALTH is 500
            SetEntityRenderMode(victim, RENDER_TRANSCOLOR);
            SetEntityRenderColor(victim, 255, fenceFx, fenceFx, 255);
        }
        // Prevent further damage processing
        damage = 0.0;
        return Plugin_Handled;
    }

    return Plugin_Continue;
}

public Action perk_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
  // [CRASH FIX] Prevent crashes from invalid attackers (e.g., world damage, attacker index 0).
  // Passing an invalid client index to functions like isZom() is a guaranteed crash.
  // This check is essential for stability. Perks that need to handle world damage (like fall damage)
  // should be aware that fall damage has attacker == victim, so it will pass this check.
  // This specifically blocks damage where the attacker is not a valid player.
  if (attacker <= 0 || !IsValidEdict(attacker))
  {
      return Plugin_Continue;
  }

  float originalDamage = damage; // 保存原始伤害

  // 增加更严格的检查：确保受害者是有效、存活的玩家，并且其Perk对象存在
  if (validLivingClient(victim) && g_hPerks[victim] != null)
  {
    char perkName[64];
    g_hPerks[victim].getName(perkName, sizeof(perkName));
    
    damage = g_hPerks[victim].onTakeDamage(victim, attacker, inflictor, damage, damagetype);
    
  }

  

  // 忽略对无敌玩家的伤害事件
  if(validClient(victim) && isUbered(victim))
  {
    return Plugin_Continue;
  }

  // Cripple logic for backstabs
  if (GetConVarBool(hCvarCripple) && validZom(attacker) && attackWasBackstab(attacker, inflictor, damage))
  {
      PrintHintText(attacker, "%t", "ZF_Cripple_Backstab_Success", victim);
      PrintHintText(victim, "%t", "ZF_Cripple_Backstab_Victim", attacker);
      damage = 60.0;
      TF2_StunPlayer(victim, 5.0, _, TF_STUNFLAGS_LOSERSTATE, attacker);
      return Plugin_Changed;
  }
  
  if (damage != originalDamage) {
      // Perk 已经修改了伤害，我们信任 perk 的计算，直接返回
      return Plugin_Changed;
  }

  

  // 处理毒药伤害归属
  if(damagetype & ZF_DMGTYPE_POISON)
  {
    if(validClient(victim) && (validSur(zf_lastPoison[victim]) || validZom(zf_lastPoison[victim])))
    {
      attacker = zf_lastPoison[victim];
    }
    return Plugin_Changed;
  }
   
  // 修复并实现伤害调整逻辑
  float fDamage = damage;
  float att_bonus = 0.0;
  float def_bonus = 0.0;

  if(validLivingClient(attacker) && (victim != attacker))
    att_bonus = float(getStat(attacker, ZFStatAtt));
  if(validLivingClient(victim) && (victim != attacker))
    def_bonus = float(getStat(victim, ZFStatDef));
  

  
    
  // 应用攻击和防御加成
  fDamage = fDamage * (1.0 + att_bonus / 100.0);
  fDamage = fDamage * (1.0 - def_bonus / 100.0);
  // 步哨枪的攻击加成减半
  if(entIsSentry(inflictor))
  {

    fDamage = 4.0;
  }
  // Marked Perk team damage bonus
  if (validLivingZom(attacker) && validLivingSur(victim))
  {
      for (int i = 1; i <= MaxClients; i++)
      {
          if (g_hPerks[i] != null && isZom(i))
          {
              char perkName[64];
              g_hPerks[i].getName(perkName, sizeof(perkName));
              if (StrEqual(perkName, "Marked"))
              {
                  MarkedPerk perk = view_as<MarkedPerk>(g_hPerks[i]);
                  if (perk.target == victim)
                  {
                      // Apply 50% damage bonus from teammates
                      fDamage *= 1.5;
                      break; // Found the marker, no need to check others
                  }
              }
          }
      }
  }

  // 确保伤害不为负
  if (fDamage < 0.0) {
      fDamage = 0.0;
  }

  damage = fDamage;

  return Plugin_Changed;
}

public void perk_OnTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype)
{
  if (g_hPerks[victim] != null) {
    g_hPerks[victim].onTakeDamagePost(victim, attacker, inflictor, damage, damagetype);
  }
//   // DEBUG
//   char inflictorClass[32];
//   GetEdictClassname(inflictor, inflictorClass, sizeof(inflictorClass));
//   PrintToChatAll("\x05[ZF DEBUG]\x01 Vic %d, Att %d, Inf %d (%s), Dmg %f, DTp %x", victim, attacker, inflictor, inflictorClass, damage, damagetype);
     
  //
  // Survivor is taking damage from zombie.
  //
  if(validSur(victim) && validZom(attacker))
  {
  }
  //
  // Zombie is taking damage from survivor.
  //
  else if(validZom(victim) && validSur(attacker))
  {
  }
}

public void perk_OnDealDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype)
{
  if (validClient(attacker) && g_hPerks[attacker] != null)
  {
    g_hPerks[attacker].onDealDamagePost(victim, attacker, inflictor, damage, damagetype);
  }
}

public void perk_OnTouch(int toucher, int touchee)
{
  // ZF_LogDebug("perk_OnTouch: toucher=%d, touchee=%d", toucher, touchee); // 日志过多，暂时禁用
  if(validLivingZom(toucher) && validLivingSur(touchee))
  {
    if (g_hPerks[toucher] != null) {
      g_hPerks[toucher].onTouch(toucher, touchee);
    }
  }
}

public void perk_OnPlayerSpawn(int client)
{

  if(validClient(client))
  {
    //
    // Handle spawn menu presentation.
    //
    if(zf_perkMode == 0)
    {
      int js = prefGet(client, JoinState);
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
    if(IsFakeClient(client) || prefGet(client, PerkSelectMode) != zf_perkMode)
    {
      // If client is a bot, force random perk mode (1). Otherwise, use server's perk mode.
      int mode = IsFakeClient(client) ? 1 : zf_perkMode;

      //
      // Clear perks.
      //
      if(mode == 0)
      {
        prefSet(client, SurPendPerk, ZF_PERK_NONE);
        prefSet(client, ZomPendPerk, ZF_PERK_NONE);
        PrintToChat(client, "%T", "ZF_Perk_Cleared", client);
      }
      //
      // Select random perks.
      //
      else if(mode == 1)
      {
        int validSurPerkCount = 0;
        int validZomPerkCount = 0;
        int validSurPerks[64];
        int validZomPerks[64];
           
        for(int i = 1; i < GetTotalSurPerks(); i++)
          if(surPerkEnabled(i))
            validSurPerks[validSurPerkCount++] = i;
        if(validSurPerkCount == 0)
          validSurPerks[validSurPerkCount++] = 0;
    
        for(int i = 1; i < GetTotalZomPerks(); i++)
          if(zomPerkEnabled(i))
            validZomPerks[validZomPerkCount++] = i;
        if(validZomPerkCount == 0)
          validZomPerks[validZomPerkCount++] = 0;

        prefSet(client, SurPendPerk, validSurPerks[GetRandomInt(0, validSurPerkCount - 1)]);
        prefSet(client, ZomPendPerk, validZomPerks[GetRandomInt(0, validZomPerkCount - 1)]);
        
        if (!IsFakeClient(client))
        {
            char surPerkName[64], zomPerkName[64];
            GetSurPerkDisplayName(client, prefGet(client, SurPendPerk), surPerkName, sizeof(surPerkName));
            GetZomPerkDisplayName(client, prefGet(client, ZomPendPerk), zomPerkName, sizeof(zomPerkName));
            PrintToChat(client, "%T", "ZF_Perk_Random_Player", client, surPerkName, zomPerkName);
        }
      }
      //
      // Use random team perks.
      //
      else if(mode == 2)
      {
        prefSet(client, SurPendPerk, zf_perkRandSurPerk);
        prefSet(client, ZomPendPerk, zf_perkRandZomPerk);
        if (!IsFakeClient(client))
        {
            char surPerkName[64], zomPerkName[64];
            GetSurPerkDisplayName(client, zf_perkRandSurPerk, surPerkName, sizeof(surPerkName));
            GetZomPerkDisplayName(client, zf_perkRandZomPerk, zomPerkName, sizeof(zomPerkName));
            PrintToChat(client, "%T", "ZF_Perk_Random_Team", client, surPerkName, zomPerkName);
        }
      }
      //
      // Use CVAR team perks.
      //
      else if(mode == 3)
      {
        prefSet(client, SurPendPerk, zf_perkTeamSurPerk);
        prefSet(client, ZomPendPerk, zf_perkTeamZomPerk);
        if (!IsFakeClient(client))
        {
            char surPerkName[64], zomPerkName[64];
            GetSurPerkDisplayName(client, zf_perkTeamSurPerk, surPerkName, sizeof(surPerkName));
            GetZomPerkDisplayName(client, zf_perkTeamZomPerk, zomPerkName, sizeof(zomPerkName));
            PrintToChat(client, "%T", "ZF_Perk_Cvar_Team", client, surPerkName, zomPerkName);
        }
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
    int nextPerk = ZF_PERK_NONE;
    bool perkChange = false;
    bool teamChange = (GetClientTeam(client) != zf_lastTeam[client]);
    if(isSur(client))
    {
      if(surPerkEnabled(prefGet(client, SurPendPerk)))
        nextPerk = prefGet(client, SurPendPerk);
      else if(surPerkEnabled(prefGet(client, SurPerk)))
        nextPerk = prefGet(client, SurPerk);
      
      if((zf_perkMode == 0) && surPerkAtLimit(client, nextPerk))
      {
        char perkName[64];
        GetSurPerkDisplayName(client, nextPerk, perkName, sizeof(perkName));
        PrintToChat(client, "%T", "ZF_Perk_Limit_Reached_Sur", client, perkName, zf_surPerksLimit[nextPerk]);
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
        char perkName[64];
        GetZomPerkDisplayName(client, nextPerk, perkName, sizeof(perkName));
        PrintToChat(client, "%T", "ZF_Perk_Limit_Reached_Zom", client, perkName, zf_zomPerksLimit[nextPerk]);
        nextPerk = ZF_PERK_NONE;
      }
                        
      if(teamChange || (nextPerk != prefGet(client, ZomPerk)))
      {
        perkChange = true;
        prefSet(client, ZomPerk, nextPerk);
      }
      prefSet(client, ZomPendPerk, nextPerk);
      stripWeapons(client);
    }

    //
    // Create or update Perk object
    //
    if (perkChange)
    {
        // Destroy the old perk object first
        if (g_hPerks[client] != null)
        {
            g_hPerks[client].onRemove();
            delete g_hPerks[client];
            g_hPerks[client] = null;
        }

        // Reset all player-specific states for a clean slate, this is crucial when changing teams/perks.
        resetClientStats(client);
        resetClientConds(client);
        removeAura(client);
        removeIcon(client);
        removeItems(client);
        setGlow(client, false);
        fxSetClientColor(client, 255, 255, 255, 255);
        zf_perkStr[client] = "";
        clearHud(client);

        // Create the new perk instance if one is selected
        if (nextPerk != ZF_PERK_NONE)
        {
            PerkCreator creator;
            if (isSur(client))
            {
                creator = g_SurPerkCreators.Get(nextPerk);
            }
            else
            {
                creator = g_ZomPerkCreators.Get(nextPerk);
            }

            if (creator != null)
            {
                BasePerk perk;
                Call_StartFunction(INVALID_HANDLE, creator);
                Call_PushCell(client);
                Call_Finish(perk);
                g_hPerks[client] = perk;
                char buffer[64];
                perk.getName(buffer, sizeof(buffer));
              
            }
            else
            {
              
                g_hPerks[client] = new BasePerk(client); // Fallback to BasePerk
            }
        }
        
        // Apply permanent stats from the new perk (or reset to zero if no perk is selected)
        updateClientPermStats(client);
    }

    
    // Call the spawn event on the new/existing perk
    if (g_hPerks[client] != null)
    {
        g_hPerks[client].onPlayerSpawn();
    }
    
    // Reset legacy state
    zf_lastAttack[client] = 0;
    zf_lastButtons[client] = 0;
    zf_lastHealth[client] = 0;
    zf_lastKiller[client] = 0;
    zf_lastPoison[client] = 0;
    zf_lastTeam[client] = GetClientTeam(client);

    // Update HUD
    updateHud(client);
  }
}

public void perk_OnPlayerDeath(int victim, int killer, int assist, int inflictor, int damagetype)
{

//   // DEBUG
//   LogMessage("[ZF DEBUG] Vic %d, Klr %d, Ast %d, Inf %d, DTp %x", victim, killer, assist, inflictor, damagetype);
  
  if(validClient(victim))
  {
    if (g_hPerks[victim] != null)
    {
        g_hPerks[victim].onDeath(victim, killer, assist, inflictor, damagetype);
    }
    // Destroy perk object on death
    
    // 幸存者死亡后会变成僵尸，所以他们的技能对象必须被清除。
    // 僵尸死亡后仍然是僵尸，所以他们的技能对象被保留以便重生。
    if (isSur(victim) && roundState() == RoundActive)
    {
        if (g_hPerks[victim] != null)
        {
            g_hPerks[victim].onRemove();
            delete g_hPerks[victim];
            g_hPerks[victim] = null;
        }
    }
    
    //
    // Set last killer.
    //
    zf_lastKiller[victim] = killer;

    // Swarming Perk Logic: Fast respawn aura
    if (isZom(victim))
    {
        bool respawnTimerCreated = false;

        // 1. Check for self-respawn first
        if (g_hPerks[victim] != null)
        {
            char perkName[32];
            g_hPerks[victim].getName(perkName, sizeof(perkName));
            if (StrEqual(perkName, "Swarming"))
            {
                CreateTimer(ZF_SWARMING_RESPAWNTIME, perk_tSpawnClient, victim);
                respawnTimerCreated = true;
            }
        }

        // 2. If not a Swarmer or self-respawn didn't apply, check for nearby Swarmer teammates
        if (!respawnTimerCreated)
        {
            float victimPos[3];
            GetClientAbsOrigin(victim, victimPos);

            for (int i = 1; i <= MaxClients; i++)
            {
                if (i == victim || !validLivingZom(i) || g_hPerks[i] == null)
                    continue;

                char teammatePerkName[32];
                g_hPerks[i].getName(teammatePerkName, sizeof(teammatePerkName));

                if (StrEqual(teammatePerkName, "Swarming"))
                {
                    float swarmerPos[3];
                    GetClientAbsOrigin(i, swarmerPos);

                    if (GetVectorDistance(victimPos, swarmerPos, true) <= ZF_SWARMING_RADIUSSQ)
                    {
                        CreateTimer(ZF_SWARMING_RESPAWNTIME, perk_tSpawnClient, victim);
                        break; // Found a swarmer, no need to check further
                    }
                }
            }
        }
    }
             
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


  if (validClient(killer) && g_hPerks[killer] != null)
  {
      g_hPerks[killer].onKill(victim, killer, assist, inflictor, damagetype);
  }
  if (validClient(assist) && assist != killer && g_hPerks[assist] != null)
  {
      g_hPerks[assist].onAssistKill(victim, killer, assist, inflictor, damagetype);
  }
}

public Action perk_OnCallForMedic(int client)
{
  if(validLivingSur(client))
  {
    if (g_hPerks[client] != null) {
      g_hPerks[client].onCallForMedic();
    }

    return Plugin_Handled;
  }
  if(validLivingZom(client))
  {
    if (g_hPerks[client] != null) {
      g_hPerks[client].onCallForMedic();
    }

    return Plugin_Handled;
  }
    
  return Plugin_Continue;
}

public void perk_OnAmmoPickup(int client, int pickup)
{

  if(validLivingSur(client))
  {
    if (g_hPerks[client] != null) {
      g_hPerks[client].onAmmoPickup(pickup);
    }
  }
}

public void perk_OnMedPickup(int client, int pickup)
{

  if(validLivingSur(client))
  {
    if (g_hPerks[client] != null) {
      g_hPerks[client].onMedPickup(pickup);
    }
    // Handle general survivor logic
    // subCond(client, ZFCondPoisoned, ZF_POISON_HEAL_MEDPACK);
  }
}

////////////////////////////////////////////////////////////
//
// Perk Event Logic, Timer Callbacks
//
////////////////////////////////////////////////////////////
public void perk_tSpawnClient(Handle timer, any client)
{
  if(validClient(client) && !IsPlayerAlive(client))
  {
    spawnClient(client, zomTeam());
  }
}

public void perk_tNinjaDecoyPoof(Handle Timer, any client)
{
  if(validClient(client))
  {
    // doNinjaDecoyPoof(client); // 保留以备将来使用
  }
}
   
public void perk_tSickSpit(Handle timer, Handle dataPack)
{        
  ResetPack(dataPack);
  int client = ReadPackCell(dataPack);
  int entIdx = ReadPackCell(dataPack); 
  CloseHandle(dataPack);
    
  if(validLivingZom(client))
  {
    // zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 1200.0, {75,255,75}); // 保留以备将来使用
    getEntityPos(zf_item[client][entIdx], zf_perkPos[client][entIdx]);
    fxCreateSoundToAll(ZFSND_SPIT[GetRandomInt(0,2)], client);   
  }  
}

public void perk_tTarredSpit(Handle timer, Handle dataPack)
{
  ResetPack(dataPack);
  int client = ReadPackCell(dataPack);
  int entIdx = ReadPackCell(dataPack); 
  CloseHandle(dataPack);
    
  if(validLivingZom(client))
  {
    // zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 900.0, {25,25,25}); // 保留以备将来使用
    getEntityPos(zf_item[client][entIdx], zf_perkPos[client][entIdx]);
    fxCreateSoundToAll(ZFSND_SPIT[GetRandomInt(0,2)], client);
  }  
}

public void perk_tZenlikeAttack(Handle timer, any client)
{
  if(validLivingSur(client))
  {
    // zf_perkState[client] -= ZF_ZENLIKE_CRIT_DEC; // 保留以备将来使用
    if(zf_perkState[client] < 0) 
      zf_perkState[client] = 0;
  }
}
