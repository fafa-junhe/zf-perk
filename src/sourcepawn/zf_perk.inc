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
BasePerk g_hPerks[MAXPLAYERS+1];   // 存储每个玩家的Perk对象

ArrayList g_SurPerkTypes;
StringMap g_SurPerkRegistry;
ArrayList g_ZomPerkTypes;
StringMap g_ZomPerkRegistry;
//
// Perk Objects
//
#define ZF_PERK_NONE            0
  
// Survivor perks
stock int GetTotalSurPerks() {
    return g_SurPerkTypes.Length;
}
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
stock int GetTotalZomPerks() {
    return g_ZomPerkTypes.Length;
}
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
stock void GetPerkInfoString(StringMap registry, ArrayList typeList, int index, const char[] key, char[] buffer, int maxLen)
{
    if (index < 0 || index >= typeList.Length)
    {
        strcopy(buffer, maxLen, "");
        return;
    }

    char class[64];
    typeList.GetString(index, class, sizeof(class));

    StringMap perkInfo;
    if (registry.GetValue(class, view_as<int>(perkInfo)))
    {
        perkInfo.GetString(key, buffer, maxLen);
    }
    else
    {
        strcopy(buffer, maxLen, "");
    }
}

stock void GetSurPerkName(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkRegistry, g_SurPerkTypes, index, "name", buffer, maxLen); }
stock void GetSurPerkShortDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkRegistry, g_SurPerkTypes, index, "shortDesc", buffer, maxLen); }
stock void GetSurPerkLongDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_SurPerkRegistry, g_SurPerkTypes, index, "longDesc", buffer, maxLen); }

stock void GetZomPerkName(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkRegistry, g_ZomPerkTypes, index, "name", buffer, maxLen); }
stock void GetZomPerkShortDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkRegistry, g_ZomPerkTypes, index, "shortDesc", buffer, maxLen); }
stock void GetZomPerkLongDesc(int index, char[] buffer, int maxLen) { GetPerkInfoString(g_ZomPerkRegistry, g_ZomPerkTypes, index, "longDesc", buffer, maxLen); }


stock int RegisterSurvivorPerk(BasePerk bp)
{
    char classname[64];
    bp.getName(classname, sizeof(classname));
    // Prevent duplicate registration
    if (g_SurPerkRegistry.ContainsKey(classname))
    {
        LogError("[ZF] Attempted to register survivor perk with duplicate classname: %s", classname);
        return -1;
    }

    // Create the info map for this perk
    StringMap perkInfo = new StringMap();

    char shortDesc[128];
    bp.getShortdesc(shortDesc, sizeof(shortDesc));
    char longDesc[1024];
    bp.getDesc(longDesc, sizeof(longDesc));

    perkInfo.SetString("name",     classname);
    perkInfo.SetString("shortDesc", shortDesc);
    perkInfo.SetString("longDesc", longDesc);

    // Add the info to the main registry and the type list
    g_SurPerkRegistry.SetValue(classname, perkInfo);
    g_SurPerkTypes.PushString(classname);
    
    // Return the index this perk was registered at
    return g_SurPerkTypes.Length - 1;
}

/**
 * Registers a new zombie perk.
 * This should be called during perkInit().
 *
 * @param bp    The BasePerk object to register.
 * @return      The registered index of the perk, or -1 on failure.
 */
stock int RegisterZombiePerk(BasePerk bp)
{
    char classname[64];
    bp.getName(classname, sizeof(classname));

    // Prevent duplicate registration
    if (g_ZomPerkRegistry.ContainsKey(classname))
    {
        LogError("[ZF] Attempted to register zombie perk with duplicate classname: %s", classname);
        return -1;
    }

    // Create the info map for this perk
    StringMap perkInfo = new StringMap();
    
    char shortDesc[128];
    bp.getShortdesc(shortDesc, sizeof(shortDesc));
    char longDesc[1024];
    bp.getDesc(longDesc, sizeof(longDesc));

    perkInfo.SetString("name",     classname);
    perkInfo.SetString("shortDesc", shortDesc);
    perkInfo.SetString("longDesc", longDesc);

    // Add the info to the main registry and the type list
    g_ZomPerkRegistry.SetValue(classname, perkInfo);
    g_ZomPerkTypes.PushString(classname);

    // Return the index this perk was registered at
    return g_ZomPerkTypes.Length - 1;
}




//
// Perk Init
//
////////////////////////////////////////////////////////////
public void perkInit()
{
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
    ReplyToCommand(client, "%t", "ZF_Perk_Mode_Current", zf_perkMode);
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
      ReplyToCommand(client, "%t", "ZF_Perk_Mode_Set_Normal");
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "1", false) || StrEqual(cmd, "randplayer", false))
    {
      zf_perkPendingMode = 1;
      ReplyToCommand(client, "%t", "ZF_Perk_Mode_Set_RandPlayer");
      return Plugin_Handled;
    }
    if(StrEqual(cmd, "2", false) || StrEqual(cmd, "randteam", false))
    {
      zf_perkPendingMode = 2;
      ReplyToCommand(client, "%t", "ZF_Perk_Mode_Set_RandTeam");
      return Plugin_Handled;
    } 
    if(StrEqual(cmd, "3", false) || StrEqual(cmd, "cvarteam", false))
    {
      zf_perkPendingMode = 3;
      ReplyToCommand(client, "%t", "ZF_Perk_Mode_Set_CvarTeam");
      return Plugin_Handled;
    }      
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_Mode_Usage", cmd);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Team_Set_Sur", perkName);
        return Plugin_Handled;
      }
    }
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_Team_Usage_Sur", cmd);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Team_Set_Zom", perkName);
        return Plugin_Handled;
      }
    }
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_Team_Usage_Zom", cmd);
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
      ReplyToCommand(client, "%t", "ZF_Perk_List_Sur_Header");
      for(int i = 1; i < GetTotalSurPerks(); i++)
      {
        char perkName[64];
        GetSurPerkName(i, perkName, sizeof(perkName));
        if(surPerkEnabled(i))
          ReplyToCommand(client, "%t", "ZF_Perk_List_Item", perkName, zf_surPerksLimit[i]);
        else
          ReplyToCommand(client, "%t", "ZF_Perk_List_Item_Disabled", perkName, zf_surPerksLimit[i]);
      }
    } 
    if(listZom)
    {
      ReplyToCommand(client, "%t", "ZF_Perk_List_Zom_Header");
      for(int i = 1; i < GetTotalZomPerks(); i++)
      {
        char perkName[64];
        GetZomPerkName(i, perkName, sizeof(perkName));
        if(zomPerkEnabled(i))
          ReplyToCommand(client, "%t", "ZF_Perk_List_Item", perkName, zf_zomPerksLimit[i]);
        else
          ReplyToCommand(client, "%t", "ZF_Perk_List_Item_Disabled", perkName, zf_zomPerksLimit[i]);
      }
    }
    
    if(listSur | listZom)
      return Plugin_Handled;
  }
  
  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_List_Usage", cmd);
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
      ReplyToCommand(client, "%t", "ZF_Perk_Enable_All_Sur", strState);
    }    
    if(setZom)
    {
      zf_zomPerksEnabled = doEnable ? 0xFFFF_FFFF : 0x0000_0001;
      ReplyToCommand(client, "%t", "ZF_Perk_Enable_All_Zom", strState);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Enable_Single", strState, perkName);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Enable_Single", strState, perkName);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "%t", "ZF_Perk_Invalid", cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_Enable_Usage", cmd);
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
      ReplyToCommand(client, "%t", "ZF_Perk_Limit_All_Sur", limit);
    }    
    if(setZom)
    {
      for(int i = 1; i < GetTotalZomPerks(); i++)
        zf_zomPerksLimit[i] = limit;
      ReplyToCommand(client, "%t", "ZF_Perk_Limit_All_Zom", limit);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Limit_Single", perkName, limit);
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
        ReplyToCommand(client, "%t", "ZF_Perk_Limit_Single", perkName, limit);
        return Plugin_Handled;
      }
    }
    
    ReplyToCommand(client, "%t", "ZF_Perk_Invalid", cmd);
    return Plugin_Handled;
  }

  //
  // Error in command format, display usage.
  //
  GetCmdArg(0, cmd, sizeof(cmd));
  ReplyToCommand(client, "%t", "ZF_Perk_Limit_Usage", cmd);
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
  ReplyToCommand(client, "%t", "ZF_Perk_Select_Usage", cmd);
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
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Invalid");
  // Do not select disabled perk.
  else if(!surPerkEnabled(perk))
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Disabled_Sur", perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Gamemode_Sur", perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(surPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetSurPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Limit_Sur", perkName, zf_surPerksLimit[perk]);
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
      PrintToChat(client, "%t", "ZF_Perk_Select_Success_Sur", perkName);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "%t", "ZF_Perk_Select_Success_Sur", perkName);
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
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Invalid");
  // Do not select disabled perk.
  else if(!zomPerkEnabled(perk))
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Disabled_Zom", perkName);
  }
  // Do not select perk during non-standard game modes.
  else if(zf_perkMode > 0)
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Gamemode_Zom", perkName);
  }
  // Do not select perk if selecting it violate perk limit.
  else if(zomPerkAtLimit(client, perk))
  {
    char perkName[64];
    GetZomPerkName(perk, perkName, sizeof(perkName));
    PrintToChat(client, "%t", "ZF_Perk_Select_Error_Limit_Zom", perkName, zf_zomPerksLimit[perk]);
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
      PrintToChat(client, "%t", "ZF_Perk_Select_Success_Zom", perkName);
    // Defer perk selection during active rounds.
    else if(roundState() > RoundGrace)
      PrintToChat(client, "%t", "ZF_Perk_Select_Success_Zom", perkName);
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
          Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_Perk", perkName);
        }
        else if(isZom(client))
        {
          char perkName[48];
          GetZomPerkName(prefGet(client, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_Perk", perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.90, 2.1, 200, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s %s", strHudPerk, zf_perkStr[client]);
        
        // HUD Element: Bonuses 1 (Attack, Defense)
        Format(strHudBonuses, sizeof(strHudBonuses), "%t", "ZF_HUD_Bonuses1", getStat(client, ZFStatAtt), getStat(client, ZFStatDef));
        SetHudTextParams(0.15, 0.93, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine1, "%s", strHudBonuses);
        
        // HUD Element: Bonuses 2 (Crit, Speed)
        Format(strHudBonuses, sizeof(strHudBonuses), "%t", "ZF_HUD_Bonuses2", getStat(client, ZFStatCrit), getStat(client, ZFStatSpeed));
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
            Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_KilledBy", perkName);
          }
          else if(isZom(killer))
          {
            char perkName[48];
            GetZomPerkName(prefGet(killer, ZomPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_KilledBy", perkName);
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
          Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_Spectating", perkName);
        }
        else if(isZom(spectate))
        {
          char perkName[48];
          GetZomPerkName(prefGet(spectate, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%t", "ZF_HUD_Spectating", perkName);
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
  Handle menu = CreateMenu(perk_menuSurPerkList);  
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%t", "ZF_Menu_SelectSurPerk_Title");
    SetMenuTitle(menu, title);
    
    // Perks
    for(int i = 1; i < GetTotalSurPerks(); i++)
    {
      char name[64], shortDesc[128];
      GetSurPerkName(i, name, sizeof(name));
      GetSurPerkShortDesc(i, shortDesc, sizeof(shortDesc));
      AddMenuItem(menu, name, shortDesc);
    }
  }
  return menu; 
}

public void perk_menuSurPerkList(Handle menu, MenuAction action, int param1, int param2)
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
Handle perk_buildZomPerkListMenu()
{
  Handle menu = CreateMenu(perk_menuZomPerkList);  
  if(menu != INVALID_HANDLE)
  {
    // Title
    char title[128];
    Format(title, sizeof(title), "%t", "ZF_Menu_SelectZomPerk_Title");
    SetMenuTitle(menu, title);
    
    // Perks
    for(int i = 1; i < GetTotalZomPerks(); i++)
    {
      char name[64], shortDesc[128];
      GetZomPerkName(i, name, sizeof(name));
      GetZomPerkShortDesc(i, shortDesc, sizeof(shortDesc));
      AddMenuItem(menu, name, shortDesc);
    }

  }
  return menu; 
}

public void perk_menuZomPerkList(Handle menu, MenuAction action, int param1, int param2)
{
  if(action == MenuAction_Select)
  {
    panel_PrintZomPerkSelect(param1, param2 + 1);
  }
}

// 
// Survivor Perk Select Menu
//
public void panel_PrintSurPerkSelect(int client, int perk)
{
  Handle panel = CreatePanel();
  
  char title[128];
  Format(title, sizeof(title), "%t", "ZF_Menu_PerkSelect_Title");
  SetPanelTitle(panel, title);
  char name[64], longDesc[1024];
  GetSurPerkName(perk, name, sizeof(name));
  GetSurPerkLongDesc(perk, longDesc, sizeof(longDesc));
  DrawPanelText(panel, name);
  DrawPanelText(panel, longDesc);
  if(surPerkEnabled(perk))
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_PerkSelect_Select");
    DrawPanelItem(panel, buffer, ITEMDRAW_DEFAULT);
  }
  else
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_PerkSelect_Disabled");
    DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
  }
  char backBuffer[128];
  Format(backBuffer, sizeof(backBuffer), "%t", "ZF_Menu_PerkSelect_Back");
  DrawPanelItem(panel, backBuffer);
  char closeBuffer[128];
  Format(closeBuffer, sizeof(closeBuffer), "%t", "ZF_Menu_PerkSelect_Close");
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
  Format(title, sizeof(title), "%t", "ZF_Menu_PerkSelect_Title");
  SetPanelTitle(panel, title);
  char name[64], longDesc[1024];
  GetZomPerkName(perk, name, sizeof(name));
  GetZomPerkLongDesc(perk, longDesc, sizeof(longDesc));
  DrawPanelText(panel, name);
  DrawPanelText(panel, longDesc);
  if(zomPerkEnabled(perk))
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_PerkSelect_Select");
    DrawPanelItem(panel, buffer, ITEMDRAW_DEFAULT);
  }
  else
  {
    char buffer[128];
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_PerkSelect_Disabled");
    DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
  }
  char backBuffer[128];
  Format(backBuffer, sizeof(backBuffer), "%t", "ZF_Menu_PerkSelect_Back");
  DrawPanelItem(panel, backBuffer);
  char closeBuffer[128];
  Format(closeBuffer, sizeof(closeBuffer), "%t", "ZF_Menu_PerkSelect_Close");
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
  removeIcon(ownerClient);
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
{ fxShowSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

stock void hideIcon(int client)
{ fxHideSprite(zf_icon[client][ICON_SPR], zf_icon[client][ICON_ANC]); }

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


////////////////////////////////////////////////////////////
//
// Perk Bonus Update Logic
//
////////////////////////////////////////////////////////////
stock void updateClientPermStats(int client)
{
  resetClientStatType(client, ZFStatTypePerm);

  if (g_hPerks[client] != null) {
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

stock void updateClientPermEffects(int client)
{
  float headOffset[3] = {0.0, 0.0, 15.0};
  
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
      if(usingZomPerk(client, ZF_PERK_HORRIFYING))
          createAura(client, ZFPART_AURAOUTBLU, AttachBase);
      else if(usingZomPerk(client, ZF_PERK_SWARMING))
          createAura(client, ZFPART_AURAFLIES, AttachHead, headOffset);
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
        g_hPerks[thisSur].updateCondStats();
    }
    
  }
   
  // 4. Apply conditional bonuses for zombies.
  for(i = 0; i < validZomCount; i++)
  {
    int thisZom = validZoms[i];
    if (g_hPerks[thisZom] != null) {
        g_hPerks[thisZom].updateCondStats();
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
        // addStat(i, ZFStatDef, ZFStatTypeCond, ZF_DAZE_DEFEND);
      }
      if(getCond(i, ZFCondPoisoned))
      {
        // if(isBeingHealed(i)) subCond(i, ZFCondPoisoned, ZF_POISON_HEAL_MEDIC);
        // if(isUbered(i)) subCond(i, ZFCondPoisoned, ZF_POISON_HEAL_UBER);
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
  // 修复: 移除了未使用的变量
  // float collidePos[3];
  // float collideVec[3];

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
    int validSurPerks[22];
    int validZomPerks[18];
       
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
  if(StrEqual(classname, "env_sprite"))
    SDKHook(entity, SDKHook_Spawn, perk_OnEntitySpawn);
}

public void perk_OnEntitySpawn(int entity)
{
  // TODO consider use of m_hEffectEntity for sprites to further filter?
  SDKHook(entity, SDKHook_SetTransmit, perk_OnSetTransmit);
}

public Action perk_OnSetTransmit(int entity, int client)
{
  if (g_hPerks[client] != null) {
    g_hPerks[client].onSetTransmit(entity, client);
  }
  if(isSur(client))
  {
    
  }
  else if(isZom(client))
  {
    
  }
  return Plugin_Handled;
}

public void perk_OnCharitableGiftTouched(int entity, int other)
{
  char zf_statStr[ZFStat][] = { "Attack", "Crit", "Defense", "Rate of Fire", "Speed", "" };
  
  //
  // Determine gift owner.
  //
  int giftOwner = -1;  
  int giftIndex = -1;
  for(int i = 1; i <= MaxClients; i++)
  {
    // for(int j = 0; j < ZF_CHARITABLE_MAX_ITEMS; j++)
    //   if(zf_item[i][j] == entity)
    //   {
    //     giftOwner = i;
    //     giftIndex = j;
    //   }
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
    int randStat = GetRandomInt(0, (TOTAL_ZFSTATS - 1));
    // int randBonus = GetRandomInt(ZF_CHARITABLE_GIFT_BONUS_MIN, ZF_CHARITABLE_GIFT_BONUS_MAX);
    
    // Apply gift bonus.
    if(isSur(other))
    {
      // PrintHintText(other, "你获得了 %s 的加成!", zf_statStr[randStat], randBonus);    
      // addStatTempStack(other, view_as<ZFStat>(randStat), randBonus, ZF_CHARITABLE_GIFT_DURATION);    
      // addHealth(giftOwner, ZF_CHARITABLE_GIFT_BONUS_HEALTH, true);
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
public Action OnPlayerRunCmd(int client, int &buttons, int &impulse, float vel[3], float angles[3], int &weapon)
{
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
//    // DEBUG
//    char inflictorClass[32];
//    GetEdictClassname(inflictor, inflictorClass, sizeof(inflictorClass));
//    PrintToChatAll("\x05[ZF DEBUG]\x01 Vic %d, Att %d, Inf %d (%s), Dmg %f, DTp %x", victim, attacker, inflictor, inflictorClass, damage, damagetype);

  int fenceOwner = -1;
  int fenceIndex = -1;
  int fenceHP;
  int fenceFx;
  float off[3];
  
  //
  // Determine fence owner and index.
  //
  for(int i = 1; i <= MaxClients; i++)
  {
    // for(int j = 0; j < CARPENTER_MAX_ITEMS; j++)
    // {
    //   if(victim == zf_item[i][j])
    //   {
    //     fenceOwner = i;
    //     fenceIndex = j;
    //     fenceHP = getItemMetadata(zf_item[i][j]);
    //     fenceFx = (fenceHP * 255) / CARPENTER_BARRICADE_HEALTH;
    //   }
    // }
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
  return Plugin_Continue;
}

public Action perk_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
  if (g_hPerks[victim] != null) {
    g_hPerks[victim].onTakeDamage(victim, attacker, inflictor, damage, damagetype);
  }
  // 修复: 移除了未使用的变量
  // int localAttAdjust = 0;
  // int localDefAdjust = 0;
  
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
    /* localAttAdjust = */ getStat(attacker, ZFStatAtt); // 变量未使用，但保留函数调用（如果它有副作用）
  if(validLivingClient(victim) && (victim != attacker))
    /* localDefAdjust = */ getStat(victim, ZFStatDef); // 变量未使用，但保留函数调用
  
  //
  // Reduce sentry attack bonuses by half.
  //
  if(entIsSentry(inflictor))
    // localAttAdjust /= 2; // 变量未使用
    {}
    

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

public void perk_OnTouch(int toucher, int touchee)
{
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
    if(prefGet(client, PerkSelectMode) != zf_perkMode)
    {
      //
      // Clear perks.
      //
      if(zf_perkMode == 0)
      {
        prefSet(client, SurPendPerk, ZF_PERK_NONE);
        prefSet(client, ZomPendPerk, ZF_PERK_NONE);
        PrintToChat(client, "%t", "ZF_Perk_Cleared");
      }
      //
      // Select random perks.
      //
      else if(zf_perkMode == 1)
      {
        int validSurPerkCount = 0;
        int validZomPerkCount = 0;
        int validSurPerks[22];
        int validZomPerks[18];
           
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
        char surPerkName[64], zomPerkName[64];
        GetSurPerkName(prefGet(client, SurPendPerk), surPerkName, sizeof(surPerkName));
        GetZomPerkName(prefGet(client, ZomPendPerk), zomPerkName, sizeof(zomPerkName));
        PrintToChat(client, "%t", "ZF_Perk_Random_Player", surPerkName, zomPerkName);
      }
      //
      // Use random team perks.
      //
      else if(zf_perkMode == 2)
      {
        prefSet(client, SurPendPerk, zf_perkRandSurPerk);
        prefSet(client, ZomPendPerk, zf_perkRandZomPerk);
        char surPerkName[64], zomPerkName[64];
        GetSurPerkName(zf_perkRandSurPerk, surPerkName, sizeof(surPerkName));
        GetZomPerkName(zf_perkRandZomPerk, zomPerkName, sizeof(zomPerkName));
        PrintToChat(client, "%t", "ZF_Perk_Random_Team", surPerkName, zomPerkName);
      }
      //
      // Use CVAR team perks.
      //
      else if(zf_perkMode == 3)
      {
        prefSet(client, SurPendPerk, zf_perkTeamSurPerk);
        prefSet(client, ZomPendPerk, zf_perkTeamZomPerk);
        char surPerkName[64], zomPerkName[64];
        GetSurPerkName(zf_perkTeamSurPerk, surPerkName, sizeof(surPerkName));
        GetZomPerkName(zf_perkTeamZomPerk, zomPerkName, sizeof(zomPerkName));
        PrintToChat(client, "%t", "ZF_Perk_Cvar_Team", surPerkName, zomPerkName);
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
        GetSurPerkName(nextPerk, perkName, sizeof(perkName));
        PrintToChat(client, "%t", "ZF_Perk_Limit_Reached_Sur", perkName, zf_surPerksLimit[nextPerk]);
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
        GetZomPerkName(nextPerk, perkName, sizeof(perkName));
        PrintToChat(client, "%t", "ZF_Perk_Limit_Reached_Zom", perkName, zf_zomPerksLimit[nextPerk]);
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
    // Create or update Perk object
    //
    if (perkChange)
    {
        // Delete old perk object if exists
        if (g_hPerks[client] != null)
        {
            g_hPerks[client].onRemove();
            delete g_hPerks[client];
            g_hPerks[client] = null;
        }

        // Create new perk object
        if (isSur(client))
        {
            
        }
        else if (isZom(client))
        {
            
        }
    }

    // Call perk's onSpawn method
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
        g_hPerks[victim].onPlayerDeath(victim, killer, assist, inflictor, damagetype);
    }
    // Destroy perk object
    if (g_hPerks[victim] != null)
    {
        g_hPerks[victim].onRemove();
        delete g_hPerks[victim];
        g_hPerks[victim] = null;
    }
    
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
      
    }
    
    //
    // Survivor killed, assisted by zombie.
    //
    if(validZom(assist))
    {
      
    }
  }
  //
  // Zombie killed.
  //
  else if(validZom(victim))
  {
   
    //
    // Zombie killed by survivor.
    //
    if(validSur(killer))
    {
      
    }
    
    //
    // Zombie killed, assisted by survivor.
    //
    if(validSur(assist))
    {
      
    }
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
    // doNinjaDecoyPoof(client);
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
    // zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 1200.0, {75,255,75});
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
    // zf_item[client][entIdx] = doItemThrow(client, ZFMDL_PRESENT[2], 900.0, {25,25,25});
    getEntityPos(zf_item[client][entIdx], zf_perkPos[client][entIdx]);
    fxCreateSoundToAll(ZFSND_SPIT[GetRandomInt(0,2)], client);
  }  
}

public void perk_tZenlikeAttack(Handle timer, any client)
{
  if(validLivingSur(client))
  {
    // zf_perkState[client] -= ZF_ZENLIKE_CRIT_DEC;
    if(zf_perkState[client] < 0) 
      zf_perkState[client] = 0;
  }
}
