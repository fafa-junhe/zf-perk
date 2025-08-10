/* put the line below after all of the includes!
#pragma newdecls required
*/

////////////////////////////////////////////////////////////////////////////////
//
//  Z O M B I E - F O R T R E S S - [TF2]
//
//  This is a rewrite of the original ZF mod.
//
//  Author: dirtyminuth
//
//  Credits: Sirot, original author of ZF.
//
////////////////////////////////////////////////////////////////////////////////

// FEATURES
// Survivors / zombies not changing teams on round restart should not get classes switched.
// + On joinClass, set sur/zom class
// + On roundStart, spawn with sur/zom class
// Improve menus (organize, add screen for current state (enabled / disabled / team roles / author / version)

// BUGS
// pl_goldrush, second stage, last survivor (BLU) dies, crash. Why?
// + Not in event_playerDeath, about 4-6 after
// + Not in handle_winCondition, crash occurs soon (within ~1s) of death
// + Nothing in timer_main. Maybe it's the spectator issue?

// WORK
// (Round Start Respawn)    [ZF] event_PlayerSpawn 1 6
//                          [ZF] Grace period begun. Survivors can change classes.
// (Round Start Spawn)      [ZF] spawnClient 1 6
// (Spawn from spawnClient) [ZF] event_PlayerSpawn 1 5
// (Postspawn from spawn 1) [ZF] timer_postSpawn 1 5
// (Postspawn from spawn 2) [ZF] timer_postSpawn 1 5
#pragma newdecls required

#pragma semicolon 1

//
// Debug Logging
//
#define ZF_DEBUG

//
// Includes
//
#include <sdkhooks>

#include <sdktools>

#include <sourcemod>

#include <tf2_stocks>

#include <tf2>

#include "zf_util_base.inc"

#include "zf_util_pref.inc"

#include <tf2items>

#include <tf2attributes>

int         g_iRoundKills[MAXPLAYERS + 1];
int         g_iSurvivorKills[MAXPLAYERS + 1];
int         g_iZombieKills[MAXPLAYERS + 1];
TFClassType g_eLastSurvivorClass[MAXPLAYERS + 1];
int         g_iLastSurvivorPerk[MAXPLAYERS + 1];
TFClassType g_eLastZombieClass[MAXPLAYERS + 1];
int         g_iLastZombiePerk[MAXPLAYERS + 1];
int         g_iLastAimTarget[MAXPLAYERS + 1];
int         g_iDebugTarget[MAXPLAYERS + 1];
int         g_iDebugStat[MAXPLAYERS + 1];
int         g_iWeaponOwners[4096] = { 0, ... }; // Global array to track weapon ownership
int         g_iLastSurvivorDied;

// 用于CSV日志记录的会话统计
int g_iSessionKills[MAXPLAYERS + 1];
int g_iSessionAssists[MAXPLAYERS + 1];
int g_iSessionDeaths[MAXPLAYERS + 1];

#include "zf_perk.inc"

//
// Plugin Information
//
#define PLUGIN_VERSION "4.5.2fix4"

public Plugin myinfo =
{
    name        = "Zombie Fortress",
    author      = "dirtyminuth (Recode), Sirot (Original)",
    description = "Pits a team of survivors aganist an endless onslaught of zombies.",
    version     = PLUGIN_VERSION,
    url         = "http://forums.alliedmods.net/showthread.php?p=1227078"


}

//
// Defines
//
#define ZF_SPAWNSTATE_REST             0
#define ZF_SPAWNSTATE_HUNGER           1
#define ZF_SPAWNSTATE_FRENZY           2

#define PLAYERBUILTOBJECT_ID_DISPENSER 0
#define PLAYERBUILTOBJECT_ID_TELENT    1
#define PLAYERBUILTOBJECT_ID_TELEXIT   2
#define PLAYERBUILTOBJECT_ID_SENTRY    3

//
// State
//

// Global State
int    zf_bEnabled;
int    zf_bNewRound;
int    zf_spawnState;
int    zf_spawnRestCounter;
int    zf_spawnSurvivorsKilledCounter;
int    zf_spawnZombiesKilledCounter;
// Global Timer Handles
Handle zf_tMain;
Handle zf_tMainSlow;
Handle g_hPlayerAimTimer[MAXPLAYERS + 1];

// Cvar Handles
Handle zf_cvForceOn;
Handle zf_cvRatio;
Handle zf_cvAllowTeamPref;
Handle zf_cvSwapOnPayload;
Handle zf_cvSwapOnAttdef;

////////////////////////////////////////////////////////////
//
// Sourcemod Callbacks
//
////////////////////////////////////////////////////////////
public void OnPluginStart()
{
    LoadTranslations("common.phrases.txt");
    LoadTranslations("zombie_fortress.phrases.txt");
    // TODO Doesn't register as true at this point. Where else can it be called?
    //   // Check for necessary extensions
    //   if(LibraryExists("sdkhooks"))
    //     SetFailState("SDK Hooks is not loaded.");

    // Add server tag.
    AddServerTag("zf");

    // Initialize global state
    zf_bEnabled  = false;
    zf_bNewRound = true;
    setRoundState(RoundInit1);

    // Initialize timer handles
    zf_tMain     = INVALID_HANDLE;
    zf_tMainSlow = INVALID_HANDLE;

    // Initialize other packages
    utilBaseInit();
    utilPrefInit();
    utilFxInit();
    perkInit();
    InitZombieVisuals();

    // Initialize session stats
    for (int i = 0; i <= MAXPLAYERS; i++)
    {
        g_iSessionKills[i] = 0;
        g_iSessionAssists[i] = 0;
        g_iSessionDeaths[i] = 0;
    }

    // Register cvars
    CreateConVar("sm_zf_version", PLUGIN_VERSION, "Current Zombie Fortress Version", FCVAR_SPONLY | FCVAR_REPLICATED | FCVAR_NOTIFY);
    zf_cvForceOn       = CreateConVar("sm_zf_force_on", "1", "<0/1> Activate ZF for non-ZF maps.", FCVAR_REPLICATED | FCVAR_NOTIFY, true, 0.0, true, 1.0);
    zf_cvRatio         = CreateConVar("sm_zf_ratio", "0.65", "<0.01-1.00> Percentage of players that start as survivors.", FCVAR_REPLICATED | FCVAR_NOTIFY, true, 0.01, true, 1.0);
    zf_cvAllowTeamPref = CreateConVar("sm_zf_allowteampref", "1", "<0/1> Allow use of team preference criteria.", FCVAR_REPLICATED | FCVAR_NOTIFY, true, 0.0, true, 1.0);
    zf_cvSwapOnPayload = CreateConVar("sm_zf_swaponpayload", "1", "<0/1> Swap teams on non-ZF payload maps.", FCVAR_REPLICATED | FCVAR_NOTIFY, true, 0.0, true, 1.0);
    zf_cvSwapOnAttdef  = CreateConVar("sm_zf_swaponattdef", "1", "<0/1> Swap teams on non-ZF attack/defend maps.", FCVAR_REPLICATED | FCVAR_NOTIFY, true, 0.0, true, 1.0);
    AutoExecConfig(true, "zombie_fortress_perk");

    // Hook events
    HookEvent("teamplay_round_start", event_RoundStart);
    HookEvent("teamplay_setup_finished", event_SetupEnd);
    HookEvent("teamplay_round_win", event_RoundEnd);
    HookEvent("player_spawn", event_PlayerSpawn);
    HookEvent("player_death", event_PlayerDeath);
    HookEvent("player_builtobject", event_PlayerBuiltObject);
    HookEvent("teamplay_waiting_begins", event_WaitingBegins);
    HookEvent("post_inventory_application", event_post_inventory_application);

    // DEBUG
    //   HookEvent("player_death",            event_PlayerDeathPre, EventHookMode_Pre);

    // Hook entity outputs
    HookEntityOutput("item_healthkit_small", "OnPlayerTouch", event_MedpackPickup);
    HookEntityOutput("item_healthkit_medium", "OnPlayerTouch", event_MedpackPickup);
    HookEntityOutput("item_healthkit_full", "OnPlayerTouch", event_MedpackPickup);
    HookEntityOutput("item_ammopack_small", "OnPlayerTouch", event_AmmopackPickup);
    HookEntityOutput("item_ammopack_medium", "OnPlayerTouch", event_AmmopackPickup);
    HookEntityOutput("item_ammopack_full", "OnPlayerTouch", event_AmmopackPickup);

    // Register Admin Commands
    RegAdminCmd("sm_zf_enable", command_zfEnable, ADMFLAG_GENERIC, "Activates the Zombie Fortress plugin.");
    RegAdminCmd("sm_zf_disable", command_zfDisable, ADMFLAG_GENERIC, "Deactivates the Zombie Fortress plugin.");
    RegAdminCmd("sm_zf_swapteams", command_zfSwapTeams, ADMFLAG_GENERIC, "Swaps current team roles.");

    // Hook Client Commands
    AddCommandListener(hook_JoinTeam, "jointeam");
    AddCommandListener(hook_JoinClass, "joinclass");
    AddCommandListener(hook_VoiceMenu, "voicemenu");
    // Hook Client Console Commands
    AddCommandListener(hook_zfTeamPref, "zf_teampref");
    // Hook Client Chat / Console Commands
    RegConsoleCmd("zf", cmd_zfMenu);
    RegConsoleCmd("zf_menu", cmd_zfMenu);
    RegConsoleCmd("zf_perk", cmd_zfMenu);
    RegConsoleCmd("zfdebug", cmd_zfDebugMenu);

    WriteCsvHeader();
}

public void OnConfigsExecuted()
{
    // Determine whether to enable ZF.
    // + Enable ZF for "zf_" maps or if sm_zf_force_on is set.
    // + Disable ZF otherwise.
    if (mapIsZF() || GetConVarBool(zf_cvForceOn))
    {
        zfEnable();
    }
    else {
        zfDisable();
    }

    setRoundState(RoundInit1);

    perk_OnMapStart();

    //   // DEBUG
    //   decl String:name[128];
    //   new Handle:cvar, bool:isCommand, flags;
    //
    //   cvar = FindFirstConCommand(name, sizeof(name), isCommand, flags);
    //   if(cvar != INVALID_HANDLE)
    //   {
    //     do
    //     {
    //       if (isCommand || !(flags & 0x2) || !StrContains(name, "bot"))
    //       {
    //         continue;
    //       }
    //
    //       LogMessage("Locked ConVar: %s", name);
    //     } while (FindNextConCommand(cvar, name, sizeof(name), isCommand, flags));
    //   }
    //
    //   CloseHandle(cvar);
    //   // DEBUG
}

public void OnMapEnd()
{
    // Reset weapon owners array
    for (int i = 0; i < 4096; i++)
    {
        g_iWeaponOwners[i] = 0;

    }    // Close timer handles
    if (zf_tMain != INVALID_HANDLE)
    {
        CloseHandle(zf_tMain);
        zf_tMain = INVALID_HANDLE;
    }
    if (zf_tMainSlow != INVALID_HANDLE)
    {
        CloseHandle(zf_tMainSlow);
        zf_tMainSlow = INVALID_HANDLE;
    }

    for (int i = 0; i <= MAXPLAYERS; i++)
    {
        if (g_hPlayerAimTimer[i] != INVALID_HANDLE)
        {
            KillTimer(g_hPlayerAimTimer[i]);
            g_hPlayerAimTimer[i] = INVALID_HANDLE;
        }
    }

    setRoundState(RoundPost);
    zfDisable();
    perk_OnMapEnd();
}

public void OnPluginEnd()
{
    // Only perform cleanup if the plugin was active.
    zfDisable();
}

public void OnClientPostAdminCheck(int client)
{
    if (!zf_bEnabled) return;

    CreateTimer(10.0, timer_initialHelp, client, TIMER_FLAG_NO_MAPCHANGE);

    SDKHook(client, SDKHook_Touch, OnTouch);
    SDKHook(client, SDKHook_PreThinkPost, OnPreThinkPost);
    SDKHook(client, SDKHook_OnTakeDamage, OnTakeDamage);
    SDKHook(client, SDKHook_OnTakeDamagePost, OnTakeDamagePost);

    pref_OnClientConnect(client);
    perk_OnClientConnect(client);
    g_iLastAimTarget[client] = 0;

    if (g_hPlayerAimTimer[client] != INVALID_HANDLE)
    {
        KillTimer(g_hPlayerAimTimer[client]);
        g_hPlayerAimTimer[client] = INVALID_HANDLE;
    }
    g_hPlayerAimTimer[client] = CreateTimer(0.2, timer_CheckPlayerAim, client, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

public void OnClientDisconnect(int client)
{
    if (!zf_bEnabled) return;

    LogPlayerSessionData(client); // 记录断开连接玩家的最终数据
    pref_OnClientDisconnect(client);
    perk_OnClientDisconnect(client);
    g_bIsCosmeticZombie[client] = false;
    g_iLastAimTarget[client]    = 0;


    if (g_hPlayerAimTimer[client] != INVALID_HANDLE)
    {
        KillTimer(g_hPlayerAimTimer[client]);
        g_hPlayerAimTimer[client] = INVALID_HANDLE;
    }
}


public void OnGameFrame()
{
    // ZF_LogDebug("OnGameFrame"); // Called every frame, too spammy
    if (!zf_bEnabled) return;

    handle_gameFrameLogic();
    perk_OnGameFrame();
}

public void OnEntityCreated(int entity,
                     const char[] classname)
{
    if (!zf_bEnabled) return;

    if (StrEqual(classname, "obj_sentrygun") || StrEqual(classname, "obj_dispenser") || StrEqual(classname, "obj_teleporter"))
    {
        SDKHook(entity, SDKHook_OnTakeDamage, Hook_BuildingOnTakeDamage);
    }
    perk_OnEntityCreated(entity, classname);
}

////////////////////////////////////////////////////////////
//
// SDKHooks Callbacks
//
////////////////////////////////////////////////////////////
public Action OnGetGameDescription(char gameDesc[64])
{
    if (!zf_bEnabled) return Plugin_Continue;
    Format(gameDesc, sizeof(gameDesc), "%T", "ZF_GameDescription", LANG_SERVER, PLUGIN_VERSION);
    return Plugin_Changed;
}

public void OnTouch(int entity, int other)
{
    // ZF_LogDebug("OnTouch: entity=%d, other=%d", entity, other); // 日志过多，暂时禁用
    if (!zf_bEnabled) return;

    perk_OnTouch(entity, other);
}

public void OnPreThinkPost(int client)
{
    // ZF_LogDebug("OnPreThinkPost: client=%d", client); // Called very frequently
    if (!zf_bEnabled) return;
    //
    // Handle speed bonuses.
    //
    if (validLivingClient(client) && !isSlowed(client) && !isDazed(client) && !isCharging(client))
    {
        float speed = clientBaseSpeed(client) + clientBonusSpeed(client) + getStat(client, ZFStatSpeed);
        setClientSpeed(client, speed);
    }
}

public Action OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
    // ZF_LogDebug("OnTakeDamage: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d", victim, attacker, inflictor, damage, damagetype);
    if (!zf_bEnabled) return Plugin_Continue;

    return perk_OnTakeDamage(victim, attacker, inflictor, damage, damagetype);
}

public void OnTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype)
{
    if (!zf_bEnabled) return;

    ZF_LogDebug("OnTakeDamagePost: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d", victim, attacker, inflictor, damage, damagetype);

    perk_OnTakeDamagePost(victim, attacker, inflictor, damage, damagetype);
    perk_OnDealDamagePost(attacker, victim, inflictor, damage, damagetype);
}

public Action Hook_BuildingOnTakeDamage(int iBuilding, int &iAttacker, int &iInflictor, float &flDamage, int &iDamagetype, int &iWeapon, float flDamageForce[3], float vecDamagePosition[3])
{
    if (validClient(iAttacker) && g_hPerks[iAttacker] != null)
    {
        g_hPerks[iAttacker].onBuildingTakeDamage(iBuilding, iAttacker, iInflictor, flDamage, iDamagetype, iWeapon, flDamageForce, vecDamagePosition);
    }
    return Plugin_Continue;
}

////////////////////////////////////////////////////////////
//
// Admin Console Command Handlers
//
////////////////////////////////////////////////////////////
public Action command_zfEnable(int client, int args)
{
    if (zf_bEnabled) return Plugin_Continue;

    zfEnable();
    ServerCommand("mp_restartgame 10");
    PrintToChatAll("%t", "ZF_Enable");

    return Plugin_Continue;
}

public Action command_zfDisable(int client, int args)
{
    if (!zf_bEnabled) return Plugin_Continue;

    zfDisable();
    ServerCommand("mp_restartgame 10");
    PrintToChatAll("%t", "ZF_Disable");

    return Plugin_Continue;
}

public Action command_zfSwapTeams(int client, int args)
{
    if (!zf_bEnabled) return Plugin_Continue;

    zfSwapTeams();
    ServerCommand("mp_restartgame 10");
    PrintToChatAll("%t", "ZF_SwapTeams");

    zf_bNewRound = true;
    setRoundState(RoundInit2);

    return Plugin_Continue;
}

////////////////////////////////////////////////////////////
//
// Client Console / Chat Command Handlers
//
////////////////////////////////////////////////////////////
public Action hook_JoinTeam(int client,
                     const char[] command, int argc)
{
    if (GetConVarBool(zf_cvDebug))
        return Plugin_Continue;

    char cmd1[32];
    char sSurTeam[16];
    char sZomTeam[16];
    char sZomVgui[16];

    if (!zf_bEnabled) return Plugin_Continue;
    if (argc < 1) return Plugin_Handled;

    GetCmdArg(1, cmd1, sizeof(cmd1));

    // Log session data if team is changing
    int currentTeam = GetClientTeam(client);
    int requestedTeam = -1;
    if (StrEqual(cmd1, "red", false)) requestedTeam = view_as<int>(TFTeam_Red);
    else if (StrEqual(cmd1, "blue", false)) requestedTeam = view_as<int>(TFTeam_Blue);
    else if (StrEqual(cmd1, "spectate", false)) requestedTeam = view_as<int>(TFTeam_Spectator);

    if (requestedTeam != -1 && requestedTeam != currentTeam)
    {
        LogAndResetSession(client);
    }

    if (roundState() >= RoundGrace)
    {
        // Assign team-specific strings
        if (zomTeam() == view_as<int>(TFTeam_Blue))
        {
            sSurTeam = "red";
            sZomTeam = "blue";
            sZomVgui = "class_blue";
        }
        else {
            sSurTeam = "blue";
            sZomTeam = "red";
            sZomVgui = "class_red";
        }

        // If client tries to join the survivor team or a random team
        // during grace period or active round, place them on the zombie
        // team and present them with the zombie class select screen.
        if (StrEqual(cmd1, sSurTeam, false) || StrEqual(cmd1, "auto", false))
        {
            ChangeClientTeam(client, zomTeam());
            ShowVGUIPanel(client, sZomVgui);
            return Plugin_Handled;
        }
        // If client tries to join the zombie team or spectator
        // during grace period or active round, let them do so.
        else if (StrEqual(cmd1, sZomTeam, false) || StrEqual(cmd1, "spectate", false)) {
            return Plugin_Continue;
        }
        // Prevent joining any other team.
        else {
            return Plugin_Handled;
        }
    }

    return Plugin_Continue;
}

public Action hook_JoinClass(int client,
                      const char[] command, int argc)
{
    char cmd1[32];

    if (!zf_bEnabled) return Plugin_Continue;
    if (argc < 1) return Plugin_Handled;

    GetCmdArg(1, cmd1, sizeof(cmd1));

    // Log session data if class is changing
    TFClassType currentClass = TF2_GetPlayerClass(client);
    TFClassType requestedClass = TF2_GetClass(cmd1);
    if (requestedClass != TFClass_Unknown && requestedClass != currentClass)
    {
        LogAndResetSession(client);
    }

    if (IsFakeClient(client) && (StrEqual(cmd1, "spy", false) || StrEqual(cmd1, "engineer", false)))
    {
        return Plugin_Handled;
    }

    if (isZom(client))
    {
        // If an invalid zombie class is selected, print a message and
        // accept joinclass command. ZF spawn logic will correct this
        // issue when the player spawns.
        if (!(StrEqual(cmd1, "scout", false) || StrEqual(cmd1, "spy", false) || StrEqual(cmd1, "heavyweapons", false) || StrEqual(cmd1, "sniper", false)))
        {
            PrintToChat(client, "%t", "ZF_ZombieClasses");
        }
    }
    else if (isSur(client)) {
        // Prevent survivors from switching classes during the round.
        if (roundState() == RoundActive)
        {
            PrintToChat(client, "%t", "ZF_SurvivorClassChangeDisabled");
            return Plugin_Handled;
        }
        // If an invalid survivor class is selected, print a message
        // and accept the joincalss command. ZF spawn logic will
        // correct this issue when the player spawns.
        else if (!(StrEqual(cmd1, "soldier", false) || StrEqual(cmd1, "pyro", false) || StrEqual(cmd1, "demoman", false) || StrEqual(cmd1, "engineer", false) || StrEqual(cmd1, "medic", false))) {
            PrintToChat(client, "%t", "ZF_SurvivorClasses");
        }
    }

    return Plugin_Continue;
}

public Action hook_VoiceMenu(int client,
                      const char[] command, int argc)
{
    char cmd1[32], cmd2[32];

    if (!zf_bEnabled) return Plugin_Continue;
    if (argc < 2) return Plugin_Handled;

    GetCmdArg(1, cmd1, sizeof(cmd1));
    GetCmdArg(2, cmd2, sizeof(cmd2));

    // Capture call for medic commands (represented by "voicemenu 0 0").
    if (StrEqual(cmd1, "0") && StrEqual(cmd2, "0"))
    {
        return perk_OnCallForMedic(client);
    }

    return Plugin_Continue;
}

public Action hook_zfTeamPref(int client,
                       const char[] command, int argc)
{
    char cmd[32];

    if (!zf_bEnabled) return Plugin_Continue;

    // Get team preference
    if (argc == 0)
    {
        if (prefGet(client, TeamPref) == ZF_TEAMPREF_SUR)
            ReplyToCommand(client, "%t", "ZF_Menu_Pref_Survivor");
        else if (prefGet(client, TeamPref) == ZF_TEAMPREF_ZOM)
            ReplyToCommand(client, "%t", "ZF_Menu_Pref_Zombie");
        else if (prefGet(client, TeamPref) == ZF_TEAMPREF_NONE)
            ReplyToCommand(client, "%t", "ZF_Menu_Pref_Random");
        return Plugin_Handled;
    }

    GetCmdArg(1, cmd, sizeof(cmd));

    // Set team preference
    if (StrEqual(cmd, "sur", false))
        prefSet(client, TeamPref, ZF_TEAMPREF_SUR);
    else if (StrEqual(cmd, "zom", false))
        prefSet(client, TeamPref, ZF_TEAMPREF_ZOM);
    else if (StrEqual(cmd, "none", false))
        prefSet(client, TeamPref, ZF_TEAMPREF_NONE);
    else {
        // Error in command format, display usage
        GetCmdArg(0, cmd, sizeof(cmd));
        ReplyToCommand(client, "%t", "ZF_TeamPrefUsage", cmd);
    }

    return Plugin_Handled;
}

public Action cmd_zfMenu(int client, int args)
{
    if (!zf_bEnabled) return Plugin_Continue;
    panel_PrintMain(client);

    return Plugin_Handled;
}

////////////////////////////////////////////////////////////
//
// TF2 Gameplay Event Handlers
//
////////////////////////////////////////////////////////////
public Action TF2_CalcIsAttackCritical(int client, int weapon, char[] weaponname, bool &result)
{
    // ZF_LogDebug("TF2_CalcIsAttackCritical: client=%d, weapon=%d, weaponname=%s", client, weapon, weaponname); // Can be spammy
    if (!zf_bEnabled) return Plugin_Continue;

    perk_OnCalcIsAttackCritical(client);

    // Handle special cases.
    // + Being kritzed overrides other crit calculations.
    if (isKritzed(client))
        return Plugin_Continue;

    // Handle crit penalty.
    // + Always prevent crits with negative crit bonuses.
    if (getStat(client, ZFStatCrit) < 0)
    {
        result = false;
        return Plugin_Changed;
    }

    // Handle crit bonuses.
    // + Survivors: Crit result is combination of perk and standard crit calulations.
    // + Zombies: Crit result is based solely on perk calculation.
    if (isSur(client))
    {
        if (getStat(client, ZFStatCrit) > GetRandomInt(0, 99))
        {
            result = true;
            return Plugin_Changed;
        }
    }
    else {
        result = (getStat(client, ZFStatCrit) > GetRandomInt(0, 99));
        return Plugin_Changed;
    }

    return Plugin_Continue;
}
// void remove_entity_all(char[] item, bool ammopack)
// {
//     int ent = -1;
//     while ((ent = FindEntityByClassname(ent, item)) != -1)
//     {
//         PrintToServer("delete entity (%s) %i", item, ent);
//         float position[3];
//         float angles[3];
//         GetEntPropVector(ent, Prop_Send, "m_vecOrigin", position);
//         GetEntPropVector(ent, Prop_Send, "m_angRotation", angles);
//         if (ammopack)
//         {
//             SpawnEntity("item_ammopack_small", position, angles);
//         }
//         else {
//             SpawnEntity("item_healthkit_small", position, angles);
//         }
//         AcceptEntityInput(ent, "Kill");
//     }
// }
void removeEntitiesByClassname(const char[] classname)
{
    int ent = -1;
    while ((ent = FindEntityByClassname(ent, classname)) != -1)
    {
        AcceptEntityInput(ent, "Kill");
    }
}

// void SpawnEntity(char[] entity, float origin[3], float rotation[3] = { 0.0, 0.0, 0.0 })
// {
//     int ent = CreateEntityByName(entity);
//     if (!IsValidEntity(ent))
//     {
//         ThrowError("Invalid Entity.");
//     }

//     if (!DispatchSpawn(ent))
//     {
//         ThrowError("Invalid entity index, or no mod support.");
//     }

//     TeleportEntity(ent, origin, rotation, NULL_VECTOR);
// }

//
// Waiting for Players Begins Event
//
public Action event_WaitingBegins(Handle event, const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    removeEntitiesByClassname("prop_door_rotating");
    removeEntitiesByClassname("func_door");
    removeEntitiesByClassname("func_door_rotating");

    return Plugin_Continue;
}

//
// Round Start Event
//
public Action event_RoundStart(Handle event,
                        const char[] name, bool dontBroadcast)
{
    for (int i = 0; i <= MaxClients; i++)
    {
        g_iRoundKills[i]        = 0;
        g_iSurvivorKills[i]     = 0;
        g_iZombieKills[i]       = 0;
        g_eLastSurvivorClass[i] = TFClass_Unknown;
        g_iLastSurvivorPerk[i]  = 0;
        g_eLastZombieClass[i]   = TFClass_Unknown;
        g_iLastZombiePerk[i]    = 0;
    }
    g_iLastSurvivorDied = 0;

    // Reset session stats for all players
    for (int i = 0; i <= MaxClients; i++)
    {
        g_iSessionKills[i] = 0;
        g_iSessionAssists[i] = 0;
        g_iSessionDeaths[i] = 0;
    }

    int players[MAXPLAYERS];
    int playerCount;
    int surCount;

    if (!zf_bEnabled) return Plugin_Continue;
    //
    // Handle round state.
    // + "teamplay_round_start" event is fired twice on new map loads.
    // //
    // remove_entity_all("item_ammopack_full", true);
    // remove_entity_all("item_ammopack_medium", true);
    // remove_entity_all("item_healthkit_full", false);
    // remove_entity_all("item_healthkit_medium", false);
    if (roundState() == RoundInit1)
    {
        setRoundState(RoundInit2);
        return Plugin_Continue;
    }
    else {
        setRoundState(RoundGrace);
        PrintToChatAll("%t", "ZF_GracePeriodStart");
    }

    //
    // Assign players to zombie and survivor teams.
    //
    if (zf_bNewRound)
    {
        // Find all active players.
        playerCount = 0;
        for (int i = 1; i <= MaxClients; i++)
        {
            if (IsClientInGame(i) && (GetClientTeam(i) > 1))
            {
                players[playerCount++] = i;
            }
        }

        // Randomize, sort players
        SortIntegers(players, playerCount, Sort_Random);
        // NOTE: As of SM 1.3.1, SortIntegers w/ Sort_Random doesn't
        //       sort the first element of the array. Temp fix below.
        int idx      = GetRandomInt(0, playerCount - 1);
        int temp     = players[idx];
        players[idx] = players[0];
        players[0]   = temp;

        // Prioritize the last survivor to die to become a zombie next round
        if (g_iLastSurvivorDied != 0)
        {
            for (int i = 0; i < playerCount; i++)
            {
                if (players[i] == g_iLastSurvivorDied)
                {
                    // Move this player to the end of the array
                    int lastPlayer = players[playerCount - 1];
                    players[playerCount - 1] = players[i];
                    players[i] = lastPlayer;
                    break;
                }
            }
        }

        // Sort players using team preference criteria
        if (GetConVarBool(zf_cvAllowTeamPref))
        {
            SortCustom1D(players, playerCount, view_as<SortFunc1D>(Sort_Preference));
        }

        // Calculate team counts. At least one survivor must exist.
        surCount = RoundToFloor(playerCount * GetConVarFloat(zf_cvRatio));
        if (playerCount > 0)
        {
            // Ensure at least one survivor
            if (surCount < 1)
            {
                surCount = 1;
            }
            // Ensure at least one zombie if there are multiple players
            if (surCount >= playerCount && playerCount > 1)
            {
                surCount = playerCount - 1;
            }
        }

        // Assign active players to survivor and zombie teams.
        for (int i = 0; i < surCount; i++)
            spawnClient(players[i], surTeam());
        for (int i = surCount; i < playerCount; i++)
            spawnClient(players[i], zomTeam());
    }

    // Handle zombie spawn state.
    zf_spawnState                  = ZF_SPAWNSTATE_HUNGER;
    zf_spawnSurvivorsKilledCounter = 1;
    setTeamRespawnTime(zomTeam(), 8.0);

    // Handle grace period timers.
    CreateTimer(0.5, timer_graceStartPost, TIMER_FLAG_NO_MAPCHANGE);
    CreateTimer(45.0, timer_graceEnd, TIMER_FLAG_NO_MAPCHANGE);

    perk_OnRoundStart();

    return Plugin_Continue;
}

//
// Setup End Event
//
public Action event_SetupEnd(Handle event,
                      const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    if (roundState() != RoundActive)
    {
        setRoundState(RoundActive);
        PrintToChatAll("%t", "ZF_GracePeriodEnd");

        for (int i = 1; i <= MaxClients; i++)
        {
            if (IsClientInGame(i) && !IsPlayerAlive(i) && isSur(i))
            {
                TF2_RespawnPlayer(i);
            }
        }

        perk_OnGraceEnd();
    }

    return Plugin_Continue;
}

//
// Round End Event
//
public Action event_RoundEnd(Handle event,
                      const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    //
    // Prepare for a completely new round, if
    // + Round was a full round (full_round flag is set), OR
    // + Zombies are the winning team.
    //
    zf_bNewRound = GetEventBool(event, "full_round") || (GetEventInt(event, "team") == zomTeam());
    setRoundState(RoundPost);

    // Log final session data for all connected players
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            LogPlayerSessionData(i);
        }
    }

    perk_OnRoundEnd();

    return Plugin_Continue;
}

//
// Player Spawn Event
//
public Action event_PlayerSpawn(Handle event,
                         const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    int         client      = GetClientOfUserId(GetEventInt(event, "userid"));

    // Unhook weapon drop to prevent issues during spawn

    TFClassType clientClass = TF2_GetPlayerClass(client);

    // 1. Prevent players spawning on survivors if round has started.
    //    Prevent players spawning on survivors as an invalid class.
    //    Prevent players spawning on zombies as an invalid class.
    if (isSur(client))
    {
        if (roundState() == RoundActive)
        {
            spawnClient(client, zomTeam());
            // return Plugin_Continue;
            return Plugin_Handled;
        }
        if (!validSurvivor(clientClass))
        {
            spawnClient(client, surTeam());
            // return Plugin_Continue;
            return Plugin_Handled;
        }
    }
    else if (isZom(client)) {
        if (!validZombie(clientClass))
        {
            spawnClient(client, zomTeam());
            // return Plugin_Continue;
            return Plugin_Handled;
        }
    }

    // 2. Handle valid, post spawn logic
    CreateTimer(0.1, timer_postSpawn, client, TIMER_FLAG_NO_MAPCHANGE);

    

    return Plugin_Continue;
}

// //
// // Player Death Pre Event
// // TODO : Use to change kill icons.
// //
// public Action:event_PlayerDeathPre(Handle:event, const String:name[], bool:dontBroadcast)
// {
//   // DEBUG
//   //SetEventString(event, "weapon_logclassname", "goomba");
//   //SetEventString(event, "weapon", "taunt_scout");
//   //SetEventInt(event, "customkill", 0);
//
//   // SELFLESS explode on death     "[ZF DEBUG] Vic 9, Klr 1, Ast 0, Inf 1, DTp 40"   // "world"
//   // COMBUSTIBLE explode on death  "[ZF DEBUG] Vic 9, Klr 1, Ast 0, Inf 1, DTp 40"   // "world"
//   // SCORCHING fire damage         "[ZF DEBUG] Vic 10, Klr 1, Ast 0, Inf 1, DTp 808" // "flamethrower"
//   // SICK acid patch               "[ZF DEBUG] Vic 11, Klr 1, Ast 0, Inf 1, DTp 40"  // currently held weapon
//   // TARRED oil patch              "[ZF DEBUG] Vic 11, Klr 1, Ast 0, Inf 1, DTp 40"  // currently held weapon
//   // TOXIC active poison           "[ZF DEBUG] Vic 13, Klr 1, Ast 0, Inf 293, DTp 80000000" // "point_hurt"
//   // TOXIC passive poison          "[ZF DEBUG] Vic 6, Klr 1, Ast 0, Inf 475, DTp 80000000"  // "point_hurt"
//
//   return Plugin_Continue;
// }

//
// Player Death Event
//
public Action event_PlayerDeath(Handle event,
                         const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    int victim = GetClientOfUserId(GetEventInt(event, "userid"));
    int killer = GetClientOfUserId(GetEventInt(event, "attacker"));

    if (killer > 0 && killer != victim)
    {
        g_iRoundKills[killer]++;
        g_iSessionKills[killer]++; // 累加会话击杀
        if (isSur(killer) && isZom(victim))
        {
            g_iSurvivorKills[killer]++;
        }
        else if (isZom(killer) && isSur(victim)) {
            g_iZombieKills[killer]++;
        }
    }
    g_iSessionDeaths[victim]++; // 累加会话死亡

    int assist     = GetClientOfUserId(GetEventInt(event, "assister"));
    if (assist > 0 && assist != victim)
    {
        g_iSessionAssists[assist]++; // 累加会话助攻
    }
    int inflictor  = GetEventInt(event, "inflictor_entindex");
    int damagetype = GetEventInt(event, "damagebits");

    // Handle zombie death logic, all round states.
    if (validZom(victim))
    {
        // Store last class and perk for zombies
        g_eLastZombieClass[victim] = TF2_GetPlayerClass(victim);
        g_iLastZombiePerk[victim]  = prefGet(victim, ZomPerk);

        // Remove dropped ammopacks from zombies.
        int index = -1;
        while ((index = FindEntityByClassname(index, "tf_ammo_pack")) != -1)
        {
            if (GetEntPropEnt(index, Prop_Send, "m_hOwnerEntity") == victim)
                AcceptEntityInput(index, "Kill");
        }
    }
 
    perk_OnPlayerDeath(victim, killer, assist, inflictor, damagetype);
 
    // 当幸存者死亡时，检查他是否在任何重力光环的影响下
    if (isSur(victim))
    {
        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingZom(i) && isClientPerkNameEquals(i, "GravityWarper"))
            {
                GravityWarperPerk perk = view_as<GravityWarperPerk>(g_hPerks[i]);
                if (perk != null)
                {
                    int index = perk.affected_survivors.FindValue(victim);
                    if (index != -1)
                    {
                        SetEntityGravity(victim, 1.0);
                        perk.affected_survivors.Erase(index);
                    }
                }
            }
        }
    }

    // If a survivor dies during the grace period, respawn them immediately.
    if (roundState() == RoundGrace)
    {
        if (validSur(victim))
        {
            CreateTimer(0.1, timer_instantRespawn, victim, TIMER_FLAG_NO_MAPCHANGE);
        }
        return Plugin_Continue;
    }

    if (roundState() != RoundActive) return Plugin_Continue;

    // Handle survivor death logic, active round only.
    if (validSur(victim))
    {
        // Check if this is the last survivor
        int survivorCount = 0;
        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingSur(i))
            {
                survivorCount++;
            }
        }
        if (survivorCount == 1)
        {
            g_iLastSurvivorDied = victim;
        }

        if (GetConVarBool(zf_cvDebug))
        {
            CreateTimer(0.1, timer_instantRespawn, victim, TIMER_FLAG_NO_MAPCHANGE);
        }
        else
        {
            if (validZom(killer)) zf_spawnSurvivorsKilledCounter--;

            g_eLastSurvivorClass[victim] = TF2_GetPlayerClass(victim);
            g_iLastSurvivorPerk[victim]  = prefGet(victim, SurPerk);

            // Transfer player to zombie team.
            CreateTimer(6.0, timer_zombify, victim, TIMER_FLAG_NO_MAPCHANGE);
        }
    }

    // Handle zombie death logic, active round only.
    else if (validZom(victim)) {
        if (validSur(killer)) zf_spawnZombiesKilledCounter--;
    }

    RemoveZombieVisuals(victim);

    return Plugin_Continue;
}

//
// Object Built Event
//
public Action event_PlayerBuiltObject(Handle event,
                               const char[] name, bool dontBroadcast)
{
    if (!zf_bEnabled) return Plugin_Continue;

    int index   = GetEventInt(event, "index");
    int object_ = GetEventInt(event, "object");

    // 1. Handle dispenser rules.
    //    Disable dispensers when they begin construction.
    //    Increase max health to 250 (default level 1 is 150).
    if (object_ == PLAYERBUILTOBJECT_ID_DISPENSER)
    {
        SetEntProp(index, Prop_Send, "m_bDisabled", 1);
        SetEntProp(index, Prop_Send, "m_iMaxHealth", 250);
    }

    return Plugin_Continue;
}

public void event_post_inventory_application(Handle event, const char[] name, bool dontBroadcast)
{
    int userid = GetEventInt(event, "userid");
    int client = GetClientOfUserId(userid);
    ZombieVisuals_OnPlayerInventory(client);
}

public void TF2_OnConditionAdded(int client, TFCond cond)
{
    
	if(cond == TFCond_Jarated)
	{
        TF2_RemoveCondition(client, cond);
		addStatTempStack(client, ZFStatSpeed, -150, 10);
        SetEntityRenderColor(client, 255, 242, 89, 255);
        CreateTimer(10.0, timer_reset_color, client, TIMER_FLAG_NO_MAPCHANGE);
	}
    ZombieVisuals_OnConditionAdded(client, cond);
}

public Action timer_reset_color(Handle timer, int victim){
    SetEntityRenderColor(victim, 255, 255, 255, 255);
    return Plugin_Continue;
}

public void TF2_OnConditionRemoved(int client, TFCond cond)
{
    ZombieVisuals_OnConditionRemoved(client, cond);
}

public void event_AmmopackPickup(const char[] output, int caller, int activator, float delay)
{
    if (!zf_bEnabled) return;
    perk_OnAmmoPickup(activator, caller);
}

public void event_MedpackPickup(const char[] output, int caller, int activator, float delay)
{
    if (!zf_bEnabled) return;
    perk_OnMedPickup(activator, caller);
}

////////////////////////////////////////////////////////////
//
// Periodic Timer Callbacks
//
////////////////////////////////////////////////////////////
public Action timer_main(Handle timer)    // 1Hz
{
    // ZF_LogDebug("timer_main"); // 1Hz, maybe spammy
    if (!zf_bEnabled) return Plugin_Continue;

    handle_survivorAbilities();
    handle_zombieAbilities();
    perk_OnPeriodic();

    if (roundState() == RoundActive)
    {
        handle_winCondition();
        handle_spawnState();
    }

    return Plugin_Continue;
}

public Action timer_mainSlow(Handle timer)    // 4 min
{
    if (!zf_bEnabled) return Plugin_Continue;
    help_printZFInfoChat(0);

    return Plugin_Continue;
}

////////////////////////////////////////////////////////////
//
// Aperiodic Timer Callbacks
//
////////////////////////////////////////////////////////////
public Action timer_graceStartPost(Handle timer)
{
    // Disable all resupply cabinets.
    int index = -1;
    while ((index = FindEntityByClassname(index, "func_regenerate")) != -1)
        AcceptEntityInput(index, "Disable");

    // Remove all dropped ammopacks.
    index = -1;
    while ((index = FindEntityByClassname(index, "tf_ammo_pack")) != -1)
        AcceptEntityInput(index, "Kill");

    // Remove all ragdolls.
    index = -1;
    while ((index = FindEntityByClassname(index, "tf_ragdoll")) != -1)
        AcceptEntityInput(index, "Kill");

    // Disable all payload cart dispensers.
    index = -1;
    while ((index = FindEntityByClassname(index, "mapobj_cart_dispenser")) != -1)
        SetEntProp(index, Prop_Send, "m_bDisabled", 1);

    // Disable all respawn room visualizers (non-ZF maps only)
    if (!mapIsZF())
    {
        index = -1;
        while ((index = FindEntityByClassname(index, "func_respawnroomvisualizer")) != -1)
            AcceptEntityInput(index, "Disable");
    }

    return Plugin_Continue;
}

public Action timer_graceEnd(Handle timer)
{
    if (roundState() != RoundActive)
    {
        setRoundState(RoundActive);
        PrintToChatAll("%t", "ZF_GracePeriodEnd");

        for (int i = 1; i <= MaxClients; i++)
        {
            if (IsClientInGame(i) && !IsPlayerAlive(i) && isSur(i))
            {
                TF2_RespawnPlayer(i);
            }
        }

        perk_OnGraceEnd();
    }

    return Plugin_Continue;
}

public Action timer_initialHelp(Handle timer, any client)
{
    // Wait until client is in game before printing initial help text.
    if (IsClientInGame(client))
    {
        help_printZFInfoChat(client);
    }
    else {
        CreateTimer(10.0, timer_initialHelp, client, TIMER_FLAG_NO_MAPCHANGE);
    }

    return Plugin_Continue;
}

public Action timer_postSpawn(Handle timer, any client)
{
    ZF_LogDebug("timer_postSpawn: client=%d", client);
    if (IsClientInGame(client) && IsPlayerAlive(client))
    {
        perk_OnPlayerSpawn(client);
    }
    if (isZom(client))
    {
        ApplyZombieVisuals(client);
        
    }
    else
    {
        RemoveZombieVisuals(client);
    }
    return Plugin_Continue;
}


public Action timer_zombify(Handle timer, any client)
{
    if (validClient(client))
    {
        PrintToChat(client, "%t", "ZF_Infected");
        LogAndResetSession(client); // Log before zombifying
        spawnClient(client, zomTeam());
    }

    return Plugin_Continue;
}

public Action timer_instantRespawn(Handle timer, any client)
{
    if (validClient(client) && !IsPlayerAlive(client))
    {
        TF2_RespawnPlayer(client);
    }
    return Plugin_Continue;
}

public Action timer_CheckPlayerAim(Handle timer, any client)
{
    // ZF_LogDebug("timer_CheckPlayerAim: client=%d", client); // Called frequently
    if (!IsClientInGame(client) || !IsPlayerAlive(client))
    {
        if (g_iLastAimTarget[client] != 0)
        {
            updateTeammateHud(client, 0);
            g_iLastAimTarget[client] = 0;
        }
        return Plugin_Continue;
    }

    int target    = GetClientAimTarget(client, false);
    int newTarget = 0;

    if (target > 0 && target <= MaxClients && IsClientInGame(target) && IsPlayerAlive(target) && target != client)
    {
        // Check if they are teammates or if debug is on
        if (GetClientTeam(target) == GetClientTeam(client) || GetConVarBool(zf_cvDebug))
        {
            newTarget = target;
        }
    }

    if (newTarget != g_iLastAimTarget[client])
    {
        updateTeammateHud(client, newTarget);
        g_iLastAimTarget[client] = newTarget;
    }

    return Plugin_Continue;
}

////////////////////////////////////////////////////////////
//
// Handling Functionality
//
////////////////////////////////////////////////////////////
void handle_gameFrameLogic()
{
    // 1. Limit spy cloak to 80% of max.
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && IsPlayerAlive(i) && isZom(i))
        {
            if (getCloak(i) > 80.0)
                setCloak(i, 80.0);
        }
    }
}

void handle_winCondition()
{
    // 1. Check for any survivors that are still alive.
    bool anySurvivorAlive = false;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && IsPlayerAlive(i) && isSur(i))
        {
            anySurvivorAlive = true;
            break;
        }
    }

    // 2. If no survivors are alive and at least 1 zombie is playing,
    //    end round with zombie win.
    if (!anySurvivorAlive && (GetTeamClientCount(zomTeam()) > 0))
    {
        endRound(zomTeam());
    }
}

void handle_spawnState()
{
    // 1. Handle zombie spawn times. Zombie spawn times can have one of three
    //    states: Rest (long spawn times), Hunger (medium spawn times), and
    //    Frenzy (short spawn times).
    switch (zf_spawnState)
    {
            // 1a. Rest state (long spawn times). Transition to Hunger
            //     state after rest timer reaches zero.
        case ZF_SPAWNSTATE_REST:
        {
            zf_spawnRestCounter--;
            if (zf_spawnRestCounter <= 0)
            {
                zf_spawnState                  = ZF_SPAWNSTATE_HUNGER;
                zf_spawnSurvivorsKilledCounter = 1;
                PrintToChatAll("%t", "ZF_ZombiesHungry");
                setTeamRespawnTime(zomTeam(), 8.0);
            }
        }

        // 1b. Hunger state (medium spawn times). Transition to Frenzy
        //     state after one survivor is killed.
        case ZF_SPAWNSTATE_HUNGER:
        {
            if (zf_spawnSurvivorsKilledCounter <= 0)
            {
                zf_spawnState                = ZF_SPAWNSTATE_FRENZY;
                zf_spawnZombiesKilledCounter = (2 * GetTeamClientCount(zomTeam()));
                PrintToChatAll("%t", "ZF_TheyAreComing");
                setTeamRespawnTime(zomTeam(), 0.0);
            }
        }

        // 1c. Frenzy state (short spawn times). Transition to Rest
        //     state after a given number of zombies are killed.
        case ZF_SPAWNSTATE_FRENZY:
        {
            if (zf_spawnZombiesKilledCounter <= 0)
            {
                zf_spawnState       = ZF_SPAWNSTATE_REST;
                zf_spawnRestCounter = min(45, (3 * GetTeamClientCount(zomTeam())));
                PrintToChatAll("%t", "ZF_WeAreSafe");
                setTeamRespawnTime(zomTeam(), 16.0);
            }
        }
    }
}

void handle_survivorAbilities()
{
    int clipAmmo;
    int resAmmo;
    int ammoAdj;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && IsPlayerAlive(i) && isSur(i))
        {
            // 1. Handle survivor weapon rules.
            //    SMG doesn't have to reload.
            //    Syringe gun / blutsauger don't have to reload.
            //    Flamethrower / backburner ammo limited to 125.
            switch (TF2_GetPlayerClass(i))
            {
                case TFClass_Medic:
                {
                    if (isEquipped(i, ZFWEAP_SYRINGEGUN) || isEquipped(i, ZFWEAP_BLUTSAUGER))
                    {
                        clipAmmo = getClipAmmo(i, 0);
                        resAmmo  = getResAmmo(i, 0);
                        ammoAdj  = min((40 - clipAmmo), resAmmo);
                        if (ammoAdj > 0)
                        {
                            setClipAmmo(i, 0, (clipAmmo + ammoAdj));
                            setResAmmo(i, 0, (resAmmo - ammoAdj));
                        }
                    }
                }

                case TFClass_Pyro:
                {
                    resAmmo = getResAmmo(i, 0);
                    if (resAmmo > 125)
                    {
                        ammoAdj = max((resAmmo - 10), 125);
                        setResAmmo(i, 0, ammoAdj);
                    }
                }
            }    // switch
        }        // if
    }            // for

    // 3. Handle sentry rules.
    //    + Norm sentry starts with 60 ammo and decays to 10.
    //    + Mini sentry starts with 60 ammo and decays to 0, then self destructs.
    //    + No sentry can be upgraded.
    int index = -1;
    while ((index = FindEntityByClassname(index, "obj_sentrygun")) != -1)
    {
        bool sentBuilding = GetEntProp(index, Prop_Send, "m_bBuilding") == 1;
        bool sentPlacing  = GetEntProp(index, Prop_Send, "m_bPlacing") == 1;
        bool sentCarried  = GetEntProp(index, Prop_Send, "m_bCarried") == 1;
        bool sentIsMini   = GetEntProp(index, Prop_Send, "m_bMiniBuilding") == 1;
        if (!sentBuilding && !sentPlacing && !sentCarried)
        {
            int sentAmmo = GetEntProp(index, Prop_Send, "m_iAmmoShells");
            if (sentAmmo > 0)
            {
                if (sentIsMini || (sentAmmo > 10))
                {
                    sentAmmo = min(60, (sentAmmo - 1));
                    SetEntProp(index, Prop_Send, "m_iAmmoShells", sentAmmo);
                }
            }
            else {
                SetVariantInt(GetEntProp(index, Prop_Send, "m_iMaxHealth"));
                AcceptEntityInput(index, "RemoveHealth");
            }
        }

        int sentLevel = GetEntProp(index, Prop_Send, "m_iHighestUpgradeLevel");
        if (sentLevel > 1)
        {
            SetVariantInt(GetEntProp(index, Prop_Send, "m_iMaxHealth"));
            AcceptEntityInput(index, "RemoveHealth");
        }
    }
}

void handle_zombieAbilities()
{
    TFClassType clientClass;
    int         curH;
    int         maxH;
    int         bonus;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && IsPlayerAlive(i) && isZom(i))
        {
            clientClass = TF2_GetPlayerClass(i);
            curH        = GetClientHealth(i);
            maxH        = GetEntProp(i, Prop_Data, "m_iMaxHealth");

            // 1. Handle zombie regeneration.
            //    Zombies regenerate health based on class.
            //    Zombies decay health when overhealed.
            bonus       = 0;
            if (curH < maxH)
            {
                switch (clientClass)
                {
                    case TFClass_Scout:
                        bonus = 2;
                    case TFClass_Heavy:
                        bonus = 4;
                    case TFClass_Spy:
                        bonus = 2;
                }
                curH += bonus;
                curH = min(curH, maxH);
                SetEntityHealth(i, curH);
            }
            else if (curH > maxH) {
                switch (clientClass)
                {
                    case TFClass_Scout:
                        bonus = -3;
                    case TFClass_Heavy:
                        bonus = -7;
                    case TFClass_Spy:
                        bonus = -3;
                }
                curH += bonus;
                curH = max(curH, maxH);
                SetEntityHealth(i, curH);
            }
        }    // if
    }        // for
}

////////////////////////////////////////////////////////////
//
// ZF Logic Functionality
//
////////////////////////////////////////////////////////////
void zfEnable()
{
    zf_bEnabled  = true;
    zf_bNewRound = true;
    setRoundState(RoundInit2);

    zfSetTeams();

    // Adjust gameplay CVars.
    SetConVarInt(FindConVar("mp_autoteambalance"), 0);
    SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
    SetConVarInt(FindConVar("tf_forced_holiday"), 2);    // for zombie skin
    SetConVarInt(FindConVar("sv_noclipspeed"), 2);       // for phantasm perk

    // Engineer
    SetConVarInt(FindConVar("tf_obj_upgrade_per_hit"), 0);
    SetConVarInt(FindConVar("tf_sentrygun_metal_per_shell"), 201);
    // Medic
    SetConVarInt(FindConVar("weapon_medigun_charge_rate"), 30);          // Time (in 2s) of non-damaged healing for 100% uber.
    SetConVarInt(FindConVar("weapon_medigun_chargerelease_rate"), 6);    // Duration (in s) of uber.
    SetConVarFloat(FindConVar("tf_max_health_boost"), 1.25);             // Percentage of full HP that is max overheal HP.
    SetConVarInt(FindConVar("tf_boost_drain_time"), 3600);               // Time (in s) of max overheal HP decay to normal health.
    // Spy
    SetConVarFloat(FindConVar("tf_spy_invis_time"), 0.5);               // Time (in s) between cloak command actual cloak.
    SetConVarFloat(FindConVar("tf_spy_invis_unstealth_time"), 0.75);    // Time (in s) between decloak command and actual decloak.
    SetConVarFloat(FindConVar("tf_spy_cloak_no_attack_time"), 1.0);     // Time (in s) between decloak and first attack.
    // [Re]Enable periodic timers.
    if (zf_tMain != INVALID_HANDLE)
        CloseHandle(zf_tMain);
    zf_tMain = CreateTimer(1.0, timer_main, _, TIMER_REPEAT);

    if (zf_tMainSlow != INVALID_HANDLE)
        CloseHandle(zf_tMainSlow);
    zf_tMainSlow = CreateTimer(240.0, timer_mainSlow, _, TIMER_REPEAT);

    // Hook all connected clients
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            OnClientPostAdminCheck(i);
        }
    }
}

void zfDisable()
{
    zf_bEnabled  = false;
    zf_bNewRound = true;
    setRoundState(RoundInit2);

    // Adjust gameplay CVars.
    ResetConVar(FindConVar("mp_autoteambalance"));
    ResetConVar(FindConVar("mp_teams_unbalance_limit"));
    // Engineer
    ResetConVar(FindConVar("tf_obj_upgrade_per_hit"));
    ResetConVar(FindConVar("tf_sentrygun_metal_per_shell"));
    // Medic
    ResetConVar(FindConVar("weapon_medigun_charge_rate"));
    ResetConVar(FindConVar("weapon_medigun_chargerelease_rate"));
    ResetConVar(FindConVar("tf_max_health_boost"));
    ResetConVar(FindConVar("tf_boost_drain_time"));
    // Spy
    ResetConVar(FindConVar("tf_spy_invis_time"));
    ResetConVar(FindConVar("tf_spy_invis_unstealth_time"));
    ResetConVar(FindConVar("tf_spy_cloak_no_attack_time"));

    // Disable periodic timers.
    if (zf_tMain != INVALID_HANDLE)
    {
        CloseHandle(zf_tMain);
        zf_tMain = INVALID_HANDLE;
    }
    if (zf_tMainSlow != INVALID_HANDLE)
    {
        CloseHandle(zf_tMainSlow);
        zf_tMainSlow = INVALID_HANDLE;
    }

    // Enable resupply lockers.
    int index = -1;
    while ((index = FindEntityByClassname(index, "func_regenerate")) != -1)
        AcceptEntityInput(index, "Enable");

    // Unhook all connected clients
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            SDKUnhook(i, SDKHook_Touch, OnTouch);
            SDKUnhook(i, SDKHook_PreThinkPost, OnPreThinkPost);
            SDKUnhook(i, SDKHook_OnTakeDamage, OnTakeDamage);
            SDKUnhook(i, SDKHook_OnTakeDamagePost, OnTakeDamagePost);

            if (g_hPlayerAimTimer[i] != INVALID_HANDLE)
            {
                KillTimer(g_hPlayerAimTimer[i]);
                g_hPlayerAimTimer[i] = INVALID_HANDLE;
            }
        }
    }
}

void zfSetTeams()
{
    //
    // Determine team roles.
    // + By default, survivors are RED and zombies are BLU.
    //
    int survivorTeam = view_as<int>(TFTeam_Red);
    int zombieTeam   = view_as<int>(TFTeam_Blue);

    //
    // Determine whether to swap teams on payload maps.
    // + For "pl_" prefixed maps, swap teams if sm_zf_swaponpayload is set.
    //
    if (mapIsPL())
    {
        if (GetConVarBool(zf_cvSwapOnPayload))
        {
            survivorTeam = view_as<int>(TFTeam_Blue);
            zombieTeam   = view_as<int>(TFTeam_Red);
        }
    }

    //
    // Determine whether to swap teams on attack / defend maps.
    // + For "cp_" prefixed maps with all RED control points, swap teams if sm_zf_swaponattdef is set.
    //
    else if (mapIsCP()) {
        if (GetConVarBool(zf_cvSwapOnAttdef))
        {
            bool isAttdef = true;
            int  index    = -1;
            while ((index = FindEntityByClassname(index, "team_control_point")) != -1)
            {
                if (GetEntProp(index, Prop_Send, "m_iTeamNum") != view_as<int>(TFTeam_Red))
                {
                    isAttdef = false;
                    break;
                }
            }

            if (isAttdef)
            {
                survivorTeam = view_as<int>(TFTeam_Blue);
                zombieTeam   = view_as<int>(TFTeam_Red);
            }
        }
    }

    // Set team roles.
    setSurTeam(survivorTeam);
    setZomTeam(zombieTeam);
}

void zfSwapTeams()
{
    int survivorTeam = surTeam();
    int zombieTeam   = zomTeam();

    // Swap team roles.
    setSurTeam(zombieTeam);
    setZomTeam(survivorTeam);
}

////////////////////////////////////////////////////////////
//
// Utility Functionality
//
////////////////////////////////////////////////////////////
public int Sort_Preference(int client1, int          client2,
                    const int[] array, Handle hndl)
{
    // Used during round start to sort using client team preference.
    int prefCli1 = IsFakeClient(client1) ? ZF_TEAMPREF_NONE : prefGet(client1, TeamPref);
    int prefCli2 = IsFakeClient(client2) ? ZF_TEAMPREF_NONE : prefGet(client2, TeamPref);
    return (prefCli1 < prefCli2) ? -1 : (prefCli1 > prefCli2) ? 1
                                                              : 0;
}

////////////////////////////////////////////////////////////
//
// Help Functionality
//
////////////////////////////////////////////////////////////
public void help_printZFInfoChat(int client)
{
    if (client == 0)
    {
        PrintToChatAll("%t", "ZF_MenuHint");
    }
    else {
        PrintToChatAll("%t", "ZF_MenuHint");
    }
}

////////////////////////////////////////////////////////////
//
// Main Menu Functionality
//
////////////////////////////////////////////////////////////

//
// Main //edit:2023.6.29 by spectator
//
public void panel_PrintMain(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Title", client, PLUGIN_VERSION);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_SelectSurPerk", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_SelectZomPerk", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_ChangeTeamPref", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Help", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkHelp", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Credits", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Change_Language", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleMain, 30);
    CloseHandle(panel);
}

public void panel_HandleMain(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintSurPerkList(param1, true);
                return;
            }
            case 2:
            {
                panel_PrintZomPerkList(param1, true);
                return;
            }
            case 3:
            {
                panel_PrintPrefTeam(param1);
                return;
            }
            case 4:
            {
                panel_PrintHelp(param1);
                return;
            }
            case 5:
            {
                panel_PrintPerkHelp(param1);
                return;
            }
            case 6:
            {
                panel_PrintCredits(param1);
                return;
            }
            case 7:
            {
                panel_ChangeLanguage(param1);
                return;
            }
            case 8:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.PrefTeam
//
public void panel_PrintPrefTeam(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_ChangeTeamPref", client);
    SetPanelTitle(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Pref_Random", client);
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_NONE)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Pref_Survivor", client);
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_SUR)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Pref_Zombie", client);
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_ZOM)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer);
    SendPanelToClient(panel, client, panel_HandlePrefTeam, 30);
    CloseHandle(panel);
}

public void panel_HandlePrefTeam(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                prefSet(param1, TeamPref, ZF_TEAMPREF_NONE);
                panel_PrintPrefTeam(param1);
                return;
            }
            case 2:
            {
                prefSet(param1, TeamPref, ZF_TEAMPREF_SUR);
                panel_PrintPrefTeam(param1);
                return;
            }
            case 3:
            {
                prefSet(param1, TeamPref, ZF_TEAMPREF_ZOM);
                panel_PrintPrefTeam(param1);
                return;
            }
            case 4:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help
//
public void panel_PrintHelp(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Help", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Overview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_SurvivorOverview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ZombieOverview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ClassSurvivor", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ClassZombie", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelp, 30);
    CloseHandle(panel);
}

public void panel_HandleHelp(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintHelpOverview(param1);
                return;
            }
            case 2:
            {
                panel_PrintHelpTeam(param1, surTeam());
                return;
            }
            case 3:
            {
                panel_PrintHelpTeam(param1, zomTeam());
                return;
            }
            case 4:
            {
                panel_PrintHelpSurClass(param1);
                return;
            }
            case 5:
            {
                panel_PrintHelpZomClass(param1);
                return;
            }
            case 6:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Overview
//
public void panel_PrintHelpOverview(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_Overview", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    char fullText[512];
    Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Overview_Full", client);
    char lines[4][128];
    int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
    for (int i = 0; i < numLines; i++)
    {
        DrawPanelText(panel, lines[i]);
    }

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpOverview, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpOverview(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Team
//
public void panel_PrintHelpTeam(int client, int team)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    if (team == surTeam())
    {
        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_SurvivorTeam", client);
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_SurvivorClasses_Full", client);
        DrawPanelText(panel, buffer);

        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_SurvivorWeapons", client);
        DrawPanelText(panel, buffer);

        DrawPanelText(panel, "----------------------------------------");
    }
    else if (team == zomTeam()) {
        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_ZombieTeam", client);
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        char fullText[512];
        Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_ZombieTeam_Full", client);
        char lines[4][128];
        int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
        for (int i = 0; i < numLines; i++)
        {
            DrawPanelText(panel, lines[i]);
        }

        DrawPanelText(panel, "----------------------------------------");
    }
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpTeam, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpTeam(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Class
//
public void panel_PrintHelpSurClass(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_SurvivorClasses", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Soldier", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Sniper", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Medic", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Demoman", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Pyro", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Engineer", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpSurClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpSurClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintClass(param1, TFClass_Soldier);
                return;
            }
            case 2:
            {
                panel_PrintClass(param1, TFClass_Sniper);
                return;
            }
            case 3:
            {
                panel_PrintClass(param1, TFClass_Medic);
                return;
            }
            case 4:
            {
                panel_PrintClass(param1, TFClass_DemoMan);
                return;
            }
            case 5:
            {
                panel_PrintClass(param1, TFClass_Pyro);
                return;
            }
            case 6:
            {
                panel_PrintClass(param1, TFClass_Engineer);
                return;
            }
            case 7:    // Close
            {
                return;
            }
        }
    }
}

public void panel_PrintHelpZomClass(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_ZombieClasses", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Scout", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Heavy", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Spy", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpZomClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpZomClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintClass(param1, TFClass_Scout);
                return;
            }
            case 2:
            {
                panel_PrintClass(param1, TFClass_Heavy);
                return;
            }
            case 3:
            {
                panel_PrintClass(param1, TFClass_Spy);
                return;
            }
            case 4:    // Close
            {
                return;
            }
        }
    }
}

public void panel_PrintClass(int client, TFClassType class)
{
    Handle panel = CreatePanel();
    char   buffer[256];
    switch (class)
    {
        case TFClass_Soldier:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Soldier", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Soldier_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Pyro:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Pyro", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Pyro_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_DemoMan:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Demoman", client);
            SetPanelTitle(panel, buffer);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Demoman_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Engineer:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Engineer", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Engineer_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Medic:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Medic", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Medic_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Sniper:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Sniper", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Sniper_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Scout:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Scout", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Scout_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Heavy:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Heavy", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Heavy_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Spy:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Spy", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Spy_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        default:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Class_Spectator", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Spectator_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
    }
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleClass, 8);
    CloseHandle(panel);
}

public void panel_HandleClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.PerkHelp
//
void panel_PrintPerkHelp(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_PerkHelp", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_PerkHelp1", client);
    DrawPanelText(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_PerkHelp2", client);
    DrawPanelText(panel, buffer);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back_Brackets", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close_Brackets", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandlePerkHelp, 30);
    CloseHandle(panel);
}

public void panel_HandlePerkHelp(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

stock void GetClassNameFromEnum(TFClassType classType, char[] buffer, int maxlen)
{
    switch (classType)
    {
        case TFClass_Unknown:
        {
            strcopy(buffer, maxlen, "Unknown");
        }
        case TFClass_Scout:
        {
            strcopy(buffer, maxlen, "Scout");
        }
        case TFClass_Sniper:
        {
            strcopy(buffer, maxlen, "Sniper");
        }
        case TFClass_Soldier:
        {
            strcopy(buffer, maxlen, "Soldier");
        }
        case TFClass_DemoMan:
        {
            strcopy(buffer, maxlen, "Demoman");
        }
        case TFClass_Medic:
        {
            strcopy(buffer, maxlen, "Medic");
        }
        case TFClass_Heavy:
        {
            strcopy(buffer, maxlen, "Heavy");
        }
        case TFClass_Pyro:
        {
            strcopy(buffer, maxlen, "Pyro");
        }
        case TFClass_Spy:
        {
            strcopy(buffer, maxlen, "Spy");
        }
        case TFClass_Engineer:
        {
            strcopy(buffer, maxlen, "Engineer");
        }
        default:
        {
            strcopy(buffer, maxlen, "Spectator");
        }
    }
}

public int Sort_Kills(int index1, int index2, Handle array, Handle hndl)
{
    DataPack pack1 = view_as<DataPack>(GetArrayCell(array, index1));
    DataPack pack2 = view_as<DataPack>(GetArrayCell(array, index2));

    pack1.Reset();
    pack1.ReadCell();    // skip client
    int kills1 = pack1.ReadCell();

    pack2.Reset();
    pack2.ReadCell();    // skip client
    int kills2 = pack2.ReadCell();

    return kills2 - kills1;
}


//
// Main.Credits
//
public void panel_PrintCredits(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Credits", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Recode", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original_Translation", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original_Balancing", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Reremake", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Special_thanks", client);
    DrawPanelText(panel, buffer);
    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleCredits, 30);
    CloseHandle(panel);
}

//
// Main.Credits
//
public void panel_ChangeLanguage(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Change_Language", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");
    Format(buffer, sizeof(buffer), "%T", "ZF_Change_Language_Guide", client);
    DrawPanelText(panel, buffer);
    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleCredits, 30);
    CloseHandle(panel);
}

public void panel_HandleCredits(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintMain(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

////////////////////////////////////////////////////////////
//
// Debug Menu Functionality
//
////////////////////////////////////////////////////////////
public Action cmd_zfDebugMenu(int client, int args)
{
    if (!GetConVarBool(zf_cvDebug))
    {
        PrintToChat(client, "%T", "ZF_Debug_Not_Enabled", client);
        return Plugin_Handled;
    }

    if (!CheckCommandAccess(client, "zfdebug", ADMFLAG_ROOT))
    {
        PrintToChat(client, "%T", "ZF_Debug_No_Access", client);
        return Plugin_Handled;
    }

    panel_DebugPlayerList(client);
    return Plugin_Handled;
}

public void panel_DebugPlayerList(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugPlayerList);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Menu_Title", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            char info[32];
            IntToString(i, info, sizeof(info));
            char name[MAX_NAME_LENGTH];
            GetClientName(i, name, sizeof(name));
            AddMenuItem(menu, info, name);
        }
    }
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugPlayerList(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int target             = StringToInt(info);
        g_iDebugTarget[client] = target;
        panel_DebugActionMenu(client);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugActionMenu(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugActionMenu);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Action_Title", client, g_iDebugTarget[client]);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Modify_Stats", client);
    AddMenuItem(menu, "stats", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Team", client);
    AddMenuItem(menu, "team", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Class", client);
    AddMenuItem(menu, "class", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Perk", client);
    AddMenuItem(menu, "perk", buffer);

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugActionMenu(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        if (StrEqual(info, "stats"))
        {
            panel_DebugModifyStats(client);
        }
        else if (StrEqual(info, "team"))
        {
            panel_DebugChangeTeam(client);
        }
        else if (StrEqual(info, "class"))
        {
            panel_DebugChangeClass(client);
        }
        else if (StrEqual(info, "perk"))
        {
            // TODO: Implement perk change menu
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugChangeTeam(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugChangeTeam);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Change_Team", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    AddMenuItem(menu, "2", "Survivor");
    AddMenuItem(menu, "3", "Zombie");
    AddMenuItem(menu, "1", "Spectator");

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugChangeTeam(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int team   = StringToInt(info);
        int target = g_iDebugTarget[client];
        if (team == surTeam())
            spawnClient(target, surTeam());
        else if (team == zomTeam())
            spawnClient(target, zomTeam());
        else
            ChangeClientTeam(target, team);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugChangeClass(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugChangeClass);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Change_Class", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    if (isSur(g_iDebugTarget[client]))
    {
        AddMenuItem(menu, "soldier", "Soldier");
        AddMenuItem(menu, "pyro", "Pyro");
        AddMenuItem(menu, "demoman", "Demoman");
        AddMenuItem(menu, "engineer", "Engineer");
        AddMenuItem(menu, "medic", "Medic");
        AddMenuItem(menu, "sniper", "Sniper");
    }
    else if (isZom(g_iDebugTarget[client]))
    {
        AddMenuItem(menu, "scout", "Scout");
        AddMenuItem(menu, "heavyweapons", "Heavy");
        AddMenuItem(menu, "spy", "Spy");
    }

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

// --- Start of Debug Stat Modification Menus ---

// This is an assumption as perk_structs.inc is not available.
// Based on HUD code, there are at least 4 stats.
#if !defined TOTAL_ZFSTATS
    #define TOTAL_ZFSTATS 4
enum ZFStat
{
    ZFStatAtt,
    ZFStatDef,
    ZFStatCrit,
    ZFStatSpeed
};
#endif

stock void GetStatNameFromEnum(ZFStat stat, char[] buffer, int maxlen)
{
    switch (stat)
    {
        case ZFStatAtt:
        {
            strcopy(buffer, maxlen, "Attack");
        }
        case ZFStatDef:
        {
            strcopy(buffer, maxlen, "Defense");
        }
        case ZFStatCrit:
        {
            strcopy(buffer, maxlen, "Critical Hit");
        }
        case ZFStatSpeed:
        {
            strcopy(buffer, maxlen, "Speed");
        }
        default:
        {
            strcopy(buffer, maxlen, "Unknown Stat");
        }
    }
}

public void panel_DebugModifyStats(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugModifyStats);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Modify_Stats", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, false);

    char statName[64];
    char statIndexStr[4];
    for (int i = 0; i < TOTAL_ZFSTATS; i++)
    {
        GetStatNameFromEnum(view_as<ZFStat>(i), statName, sizeof(statName));
        IntToString(i, statIndexStr, sizeof(statIndexStr));
        AddMenuItem(menu, statIndexStr, statName);
    }

    AddMenuItem(menu, "back", "Back");
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugModifyStats(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        if (StrEqual(info, "back"))
        {
            panel_DebugActionMenu(client);
        }
        else
        {
            g_iDebugStat[client] = view_as<ZFStat>(StringToInt(info));
            panel_DebugModifyStatValue(client);
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugModifyStatValue(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugModifyStatValue);

    char title[128];
    char statName[64];
    char targetName[MAX_NAME_LENGTH];
    GetClientName(g_iDebugTarget[client], targetName, sizeof(targetName));
    GetStatNameFromEnum(view_as<ZFStat>(g_iDebugStat[client]), statName, sizeof(statName));

    Format(title, sizeof(title), "Modify %s for %s", statName, targetName);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, false);

    AddMenuItem(menu, "100", "+100");
    AddMenuItem(menu, "10", "+10");
    AddMenuItem(menu, "1", "+1");
    AddMenuItem(menu, "-1", "-1");
    AddMenuItem(menu, "-10", "-10");
    AddMenuItem(menu, "-100", "-100");
    AddMenuItem(menu, "back", "Back");

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugModifyStatValue(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));

        if (StrEqual(info, "back"))
        {
            panel_DebugModifyStats(client);
        }
        else
        {
            int    value  = StringToInt(info);
            int    target = g_iDebugTarget[client];
            ZFStat stat   = view_as<ZFStat>(g_iDebugStat[client]);

            addStat(target, stat, ZFStatTypePerm, value);

            // Re-display the menu to show it was successful and allow more changes
            panel_DebugModifyStatValue(client);
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

// ====================================================================================================
//
// CSV Logging
//
// ====================================================================================================

stock void WriteCsvHeader()
{
    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "logs/zf_stats.csv");

    if (!FileExists(path))
    {
        LogToFile(path, "user,steamid,team,class,perk,kill,assist,death,lastattack,lastspeed,lastcritchance,lastrof,lastdefense");
    }
}

void LogPlayerSessionData(int client)
{
    if (!IsClientInGame(client))
        return;

    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "logs/zf_stats.csv");

    // User
    char name[MAX_NAME_LENGTH];
    GetClientName(client, name, sizeof(name));
    ReplaceString(name, sizeof(name), ",", ";"); // Escape commas in name

    // SteamID
    char steamid[64];
    GetClientAuthId(client, AuthId_Steam2, steamid, sizeof(steamid));

    // Team, Class, Perk
    char teamStr[16];
    char classStr[32];
    char perkName[32];
    int perkId;

    if (isSur(client))
    {
        strcopy(teamStr, sizeof(teamStr), "survivor");
        GetClassNameFromEnum(TF2_GetPlayerClass(client), classStr, sizeof(classStr));
        perkId = prefGet(client, SurPerk);
        GetSurPerkName(perkId, perkName, sizeof(perkName));
    }
    else if (isZom(client))
    {
        strcopy(teamStr, sizeof(teamStr), "zombie");
        GetClassNameFromEnum(TF2_GetPlayerClass(client), classStr, sizeof(classStr));
        perkId = prefGet(client, ZomPerk);
        GetZomPerkName(perkId, perkName, sizeof(perkName));
    }
    else
    {
        strcopy(teamStr, sizeof(teamStr), "spectator");
        strcopy(classStr, sizeof(classStr), "none");
        strcopy(perkName, sizeof(perkName), "none");
    }

    // Session Stats
    int kills = g_iSessionKills[client];
    int assists = g_iSessionAssists[client];
    int deaths = g_iSessionDeaths[client];

    // Last Stats
    int lastAttack = getStat(client, ZFStatAtt);
    int lastSpeed = getStat(client, ZFStatSpeed);
    int lastCritChance = getStat(client, ZFStatCrit);
    int lastRof = getStat(client, ZFStatRof);
    int lastDefense = getStat(client, ZFStatDef);

    LogToFile(path, "%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d",
        name, steamid, teamStr, classStr, perkName,
        kills, assists, deaths,
        lastAttack, lastSpeed, lastCritChance, lastRof, lastDefense);
}

public void LogAndResetSession(int client)
{
    LogPlayerSessionData(client);

    // Reset session stats
    g_iSessionKills[client] = 0;
    g_iSessionAssists[client] = 0;
    g_iSessionDeaths[client] = 0;
}

// --- End of Debug Stat Modification Menus ---
public int panel_HandleDebugChangeClass(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int target = g_iDebugTarget[client];
        TF2_SetPlayerClass(target, TF2_GetClass(info));
        TF2_RespawnPlayer(target);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}
