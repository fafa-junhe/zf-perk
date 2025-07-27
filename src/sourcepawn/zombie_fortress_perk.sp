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

stock void ZF_LogDebug(const char[] format, any...)
{
#if defined ZF_DEBUG
    char buffer[256];
    VFormat(buffer, sizeof(buffer), format, 2);
    LogMessage("[ZF DEBUG] %s", buffer);
#endif
}

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

#include "zf_perk.inc"

//
// Plugin Information
//
#define PLUGIN_VERSION "4.2.0.17"

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
    ZF_LogDebug("OnPluginStart");
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
}

public void OnConfigsExecuted()
{
    ZF_LogDebug("OnConfigsExecuted");
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
    ZF_LogDebug("OnMapEnd");
    // Close timer handles
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

    perk_OnMapEnd();
}

public void OnClientPostAdminCheck(int client)
{
    ZF_LogDebug("OnClientPostAdminCheck: client=%d", client);
    if (!zf_bEnabled) return;

    CreateTimer(10.0, timer_initialHelp, client, TIMER_FLAG_NO_MAPCHANGE);

    SDKHook(client, SDKHook_Touch, OnTouch);
    SDKHook(client, SDKHook_PreThinkPost, OnPreThinkPost);
    SDKHook(client, SDKHook_OnTakeDamage, OnTakeDamage);
    SDKHook(client, SDKHook_OnTakeDamagePost, OnTakeDamagePost);

    pref_OnClientConnect(client);
    perk_OnClientConnect(client);

    g_hPlayerAimTimer[client] = CreateTimer(0.2, timer_CheckPlayerAim, client, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

public void OnClientDisconnect(int client)
{
    ZF_LogDebug("OnClientDisconnect: client=%d", client);
    if (!zf_bEnabled) return;

    pref_OnClientDisconnect(client);
    perk_OnClientDisconnect(client);

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
    // ZF_LogDebug("OnEntityCreated: entity=%d, classname=%s", entity, classname);
    if (!zf_bEnabled) return;

    perk_OnEntityCreated(entity, classname);
}

////////////////////////////////////////////////////////////
//
// SDKHooks Callbacks
//
////////////////////////////////////////////////////////////
public Action OnGetGameDescription(char gameDesc[64])
{
    ZF_LogDebug("OnGetGameDescription");
    if (!zf_bEnabled) return Plugin_Continue;
    Format(gameDesc, sizeof(gameDesc), "%t", "ZF_GameDescription", PLUGIN_VERSION);
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
    // ZF_LogDebug("OnTakeDamagePost: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d", victim, attacker, inflictor, damage, damagetype);
    if (!zf_bEnabled) return;

    perk_OnTakeDamagePost(victim, attacker, inflictor, damage, damagetype);
    perk_OnDealDamagePost(victim, attacker, inflictor, damage, damagetype);
}

////////////////////////////////////////////////////////////
//
// Admin Console Command Handlers
//
////////////////////////////////////////////////////////////
public Action command_zfEnable(int client, int args)
{
    ZF_LogDebug("command_zfEnable: client=%d", client);
    if (zf_bEnabled) return Plugin_Continue;

    zfEnable();
    ServerCommand("mp_restartgame 10");
    PrintToChatAll("%t", "ZF_Enable");

    return Plugin_Continue;
}

public Action command_zfDisable(int client, int args)
{
    ZF_LogDebug("command_zfDisable: client=%d", client);
    if (!zf_bEnabled) return Plugin_Continue;

    zfDisable();
    ServerCommand("mp_restartgame 10");
    PrintToChatAll("%t", "ZF_Disable");

    return Plugin_Continue;
}

public Action command_zfSwapTeams(int client, int args)
{
    ZF_LogDebug("command_zfSwapTeams: client=%d", client);
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
    ZF_LogDebug("hook_JoinTeam: client=%d, command=%s, argc=%d", client, command, argc);
    char cmd1[32];
    char sSurTeam[16];
    char sZomTeam[16];
    char sZomVgui[16];

    if (!zf_bEnabled) return Plugin_Continue;
    if (argc < 1) return Plugin_Handled;

    GetCmdArg(1, cmd1, sizeof(cmd1));

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
    ZF_LogDebug("hook_JoinClass: client=%d, command=%s, argc=%d", client, command, argc);
    char cmd1[32];

    if (!zf_bEnabled) return Plugin_Continue;
    if (argc < 1) return Plugin_Handled;

    GetCmdArg(1, cmd1, sizeof(cmd1));

    ZF_LogDebug("hook_JoinClass client %d selected class %s", client, cmd1);

    if (IsFakeClient(client) && (StrEqual(cmd1, "spy", false) || StrEqual(cmd1, "engineer", false)))
    {
        return Plugin_Handled;
    }

    if (isZom(client))
    {
        // If an invalid zombie class is selected, print a message and
        // accept joinclass command. ZF spawn logic will correct this
        // issue when the player spawns.
        if (!(StrEqual(cmd1, "scout", false) || StrEqual(cmd1, "spy", false) || StrEqual(cmd1, "heavyweapons", false)))
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
        else if (!(StrEqual(cmd1, "soldier", false) || StrEqual(cmd1, "pyro", false) || StrEqual(cmd1, "demoman", false) || StrEqual(cmd1, "engineer", false) || StrEqual(cmd1, "medic", false) || StrEqual(cmd1, "sniper", false))) {
            PrintToChat(client, "%t", "ZF_SurvivorClasses");
        }
    }

    return Plugin_Continue;
}

public Action hook_VoiceMenu(int client,
                      const char[] command, int argc)
{
    ZF_LogDebug("hook_VoiceMenu: client=%d, command=%s, argc=%d", client, command, argc);
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
    ZF_LogDebug("hook_zfTeamPref: client=%d, command=%s, argc=%d", client, command, argc);
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
    ZF_LogDebug("cmd_zfMenu: client=%d, args=%d", client, args);
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
void remove_entity_all(char[] item, bool ammopack)
{
    int ent = -1;
    while ((ent = FindEntityByClassname(ent, item)) != -1)
    {
        PrintToServer("delete entity (%s) %i", item, ent);
        float position[3];
        GetEntPropVector(ent, Prop_Send, "m_vecOrigin", position);
        if (ammopack)
        {
            SpawnEntity("item_ammopack_small", position);
        }
        else {
            SpawnEntity("item_healthkit_small", position);
        }
        AcceptEntityInput(ent, "Kill");
    }
}
void removeEntitiesByClassname(const char[] classname)
{
    int ent = -1;
    while ((ent = FindEntityByClassname(ent, classname)) != -1)
    {
        AcceptEntityInput(ent, "Kill");
    }
}

void SpawnEntity(char[] entity, float origin[3], float rotation[3] = { 0.0, 0.0, 90.0 })
{
    int ent = CreateEntityByName(entity);
    if (!IsValidEntity(ent))
    {
        ThrowError("Invalid Entity.");
    }

    if (!DispatchSpawn(ent))
    {
        ThrowError("Invalid entity index, or no mod support.");
    }

    TeleportEntity(ent, origin, rotation, NULL_VECTOR);
}

//
// Waiting for Players Begins Event
//
public Action event_WaitingBegins(Handle event, const char[] name, bool dontBroadcast)
{
    ZF_LogDebug("event_WaitingBegins");
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
    ZF_LogDebug("event_RoundStart");
    int players[MAXPLAYERS];
    int playerCount;
    int surCount;

    if (!zf_bEnabled) return Plugin_Continue;
    //
    // Handle round state.
    // + "teamplay_round_start" event is fired twice on new map loads.
    //
    remove_entity_all("item_ammopack_full", true);
    remove_entity_all("item_ammopack_medium", true);
    remove_entity_all("item_healthkit_full", false);
    remove_entity_all("item_healthkit_medium", false);
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

        // Sort players using team preference criteria
        if (GetConVarBool(zf_cvAllowTeamPref))
        {
            SortCustom1D(players, playerCount, view_as<SortFunc1D>(Sort_Preference));
        }

        // Calculate team counts. At least one survivor must exist.
        surCount = RoundToFloor(playerCount * GetConVarFloat(zf_cvRatio));
        if ((surCount == 0) && (playerCount > 0))
        {
            surCount = 1;
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
    ZF_LogDebug("event_SetupEnd");
    if (!zf_bEnabled) return Plugin_Continue;

    if (roundState() != RoundActive)
    {
        setRoundState(RoundActive);
        PrintToChatAll("%t", "ZF_GracePeriodEnd");

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
    ZF_LogDebug("event_RoundEnd");
    if (!zf_bEnabled) return Plugin_Continue;

    //
    // Prepare for a completely new round, if
    // + Round was a full round (full_round flag is set), OR
    // + Zombies are the winning team.
    //
    zf_bNewRound = GetEventBool(event, "full_round") || (GetEventInt(event, "team") == zomTeam());
    setRoundState(RoundPost);

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
    ZF_LogDebug("event_PlayerSpawn: client=%d, team=%d", client, GetClientTeam(client));
    TFClassType clientClass = TF2_GetPlayerClass(client);

    ZF_LogDebug("event_PlayerSpawn client %d, class %d", client, view_as<int>(clientClass));

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

    int victim     = GetClientOfUserId(GetEventInt(event, "userid"));
    int killer     = GetClientOfUserId(GetEventInt(event, "attacker"));
    ZF_LogDebug("event_PlayerDeath: victim=%d, killer=%d", victim, killer);
    int assist     = GetClientOfUserId(GetEventInt(event, "assister"));
    int inflictor  = GetEventInt(event, "inflictor_entindex");
    int damagetype = GetEventInt(event, "damagebits");

    // Handle zombie death logic, all round states.
    if (validZom(victim))
    {
        // Remove dropped ammopacks from zombies.
        int index = -1;
        while ((index = FindEntityByClassname(index, "tf_ammo_pack")) != -1)
        {
            if (GetEntPropEnt(index, Prop_Send, "m_hOwnerEntity") == victim)
                AcceptEntityInput(index, "Kill");
        }
    }

    perk_OnPlayerDeath(victim, killer, assist, inflictor, damagetype);

    if (roundState() != RoundActive) return Plugin_Continue;

    // Handle survivor death logic, active round only.
    if (validSur(victim))
    {
        if (validZom(killer)) zf_spawnSurvivorsKilledCounter--;

        // Transfer player to zombie team.
        CreateTimer(6.0, timer_zombify, victim, TIMER_FLAG_NO_MAPCHANGE);
    }

    // Handle zombie death logic, active round only.
    else if (validZom(victim)) {
        if (validSur(killer)) zf_spawnZombiesKilledCounter--;
    }

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
    int builder = GetClientOfUserId(GetEventInt(event, "userid"));
    ZF_LogDebug("event_PlayerBuiltObject: builder=%d, object_type=%d, entity_index=%d", builder, object_, index);

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

public void event_AmmopackPickup(const char[] output, int caller, int activator, float delay)
{
    ZF_LogDebug("event_AmmopackPickup: activator=%d, caller(entity)=%d", activator, caller);
    if (!zf_bEnabled) return;
    perk_OnAmmoPickup(activator, caller);
}

public void event_MedpackPickup(const char[] output, int caller, int activator, float delay)
{
    ZF_LogDebug("event_MedpackPickup: activator=%d, caller(entity)=%d", activator, caller);
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
    ZF_LogDebug("timer_mainSlow");
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
    ZF_LogDebug("timer_graceStartPost");
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
    ZF_LogDebug("timer_graceEnd");
    if (roundState() != RoundActive)
    {
        setRoundState(RoundActive);
        PrintToChatAll("%t", "ZF_GracePeriodEnd");

        perk_OnGraceEnd();
    }

    return Plugin_Continue;
}

public Action timer_initialHelp(Handle timer, any client)
{
    ZF_LogDebug("timer_initialHelp: client=%d", client);
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
    if (IsClientInGame(client)) ZF_LogDebug("timer_postSpawn: client=%d class=%d", client, view_as<int>(TF2_GetPlayerClass(client)));

    if (IsClientInGame(client) && IsPlayerAlive(client))
    {

        perk_OnPlayerSpawn(client);
    }

    return Plugin_Continue;
}

public Action timer_zombify(Handle timer, any client)
{
    ZF_LogDebug("timer_zombify: client=%d", client);
    if (validClient(client))
    {
        PrintToChat(client, "%t", "ZF_Infected");
        spawnClient(client, zomTeam());
    }

    return Plugin_Continue;
}

public Action timer_CheckPlayerAim(Handle timer, any client)
{
    // ZF_LogDebug("timer_CheckPlayerAim: client=%d", client); // Called frequently
    if (!IsClientInGame(client) || !IsPlayerAlive(client))
    {
        return Plugin_Continue;
    }

    int target = GetClientAimTarget(client, false);

    if (target > 0 && target <= MaxClients && IsClientInGame(target) && IsPlayerAlive(target) && target != client)
    {
        // Check if they are teammates
        if (GetClientTeam(target) == GetClientTeam(client))
        {
            if (g_hPerks[target] != null)
            {
                char perkName[64];
                g_hPerks[target].getName(perkName, sizeof(perkName));

                char targetName[MAX_NAME_LENGTH];
                GetClientName(target, targetName, sizeof(targetName));

                if (perkName[0] != '\0' && !StrEqual(perkName, "None", false))
                {
                    PrintCenterText(client, "%s's Perk:\n%s", targetName, perkName);
                }
            }
        }
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
                case TFClass_Sniper:
                {
                    if (isEquipped(i, ZFWEAP_SMG))
                    {
                        clipAmmo = getClipAmmo(i, 1);
                        resAmmo  = getResAmmo(i, 1);
                        ammoAdj  = min((25 - clipAmmo), resAmmo);
                        if (ammoAdj > 0)
                        {
                            setClipAmmo(i, 1, (clipAmmo + ammoAdj));
                            setResAmmo(i, 1, (resAmmo - ammoAdj));
                        }
                    }
                }

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
    ZF_LogDebug("zfEnable");
    zf_bEnabled  = true;
    zf_bNewRound = true;
    setRoundState(RoundInit2);

    zfSetTeams();

    // Adjust gameplay CVars.
    SetConVarInt(FindConVar("mp_autoteambalance"), 0);
    SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 0);
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
            SDKHook(i, SDKHook_Touch, OnTouch);
            SDKHook(i, SDKHook_PreThinkPost, OnPreThinkPost);
            SDKHook(i, SDKHook_OnTakeDamage, OnTakeDamage);
            SDKHook(i, SDKHook_OnTakeDamagePost, OnTakeDamagePost);
        }
    }
}

void zfDisable()
{
    ZF_LogDebug("zfDisable");
    zf_bEnabled  = false;
    zf_bNewRound = true;
    setRoundState(RoundInit2);

    // Adjust gameplay CVars.
    SetConVarInt(FindConVar("mp_autoteambalance"), 1);
    SetConVarInt(FindConVar("mp_teams_unbalance_limit"), 1);
    // Engineer
    SetConVarInt(FindConVar("tf_obj_upgrade_per_hit"), 25);
    SetConVarInt(FindConVar("tf_sentrygun_metal_per_shell"), 1);
    // Medic
    SetConVarInt(FindConVar("weapon_medigun_charge_rate"), 40);
    SetConVarInt(FindConVar("weapon_medigun_chargerelease_rate"), 8);
    SetConVarFloat(FindConVar("tf_max_health_boost"), 1.5);
    SetConVarInt(FindConVar("tf_boost_drain_time"), 15);
    // Spy
    SetConVarFloat(FindConVar("tf_spy_invis_time"), 1.0);
    SetConVarFloat(FindConVar("tf_spy_invis_unstealth_time"), 2.0);
    SetConVarFloat(FindConVar("tf_spy_cloak_no_attack_time"), 2.0);

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
        }
    }
}

void zfSetTeams()
{
    ZF_LogDebug("zfSetTeams");
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
    ZF_LogDebug("zfSwapTeams");
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
    ZF_LogDebug("panel_PrintMain: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Title");
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_SelectSurPerk");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_SelectZomPerk");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_ChangeTeamPref");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Help");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_PerkHelp");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleMain, 30);
    CloseHandle(panel);
    ZF_LogDebug("panel_PrintMain: Main menu panel sent to client %d", client);
}

public void panel_HandleMain(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleMain: action=%d, client=%d, item=%d", action, param1, param2);
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                ZF_LogDebug("panel_HandleMain: Client %d selected item 1 (Survivor Perks). Menu handle: %x", param1, zf_menuSurPerkList);
                DisplayMenu(zf_menuSurPerkList, param1, MENU_TIME_FOREVER);
                return;
            }
            case 2:
            {
                ZF_LogDebug("panel_HandleMain: Client %d selected item 2 (Zombie Perks). Menu handle: %x", param1, zf_menuZomPerkList);
                DisplayMenu(zf_menuZomPerkList, param1, MENU_TIME_FOREVER);
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
    ZF_LogDebug("panel_PrintPrefTeam: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_ChangeTeamPref");
    SetPanelTitle(panel, buffer);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Pref_Random");
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_NONE)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Pref_Survivor");
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_SUR)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Pref_Zombie");
    if (prefGet(client, TeamPref) == ZF_TEAMPREF_ZOM)
        DrawPanelItem(panel, buffer, ITEMDRAW_DISABLED);
    else
        DrawPanelItem(panel, buffer);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer);
    SendPanelToClient(panel, client, panel_HandlePrefTeam, 30);
    CloseHandle(panel);
}

public void panel_HandlePrefTeam(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandlePrefTeam: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintHelp: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Help");
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Overview");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_SurvivorOverview");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_ZombieOverview");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_ClassSurvivor");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_ClassZombie");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelp, 30);
    CloseHandle(panel);
}

public void panel_HandleHelp(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleHelp: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintHelpOverview: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_Overview");
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    char fullText[512];
    Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Overview_Full");
    char lines[4][128];
    int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
    for (int i = 0; i < numLines; i++)
    {
        DrawPanelText(panel, lines[i]);
    }

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Back");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpOverview, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpOverview(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleHelpOverview: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintHelpTeam: client=%d, team=%d", client, team);
    Handle panel = CreatePanel();
    char   buffer[256];

    if (team == surTeam())
    {
        Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_SurvivorTeam");
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        Format(buffer, sizeof(buffer), "%t", "ZF_Help_Text_SurvivorClasses_Full");
        DrawPanelText(panel, buffer);

        Format(buffer, sizeof(buffer), "%t", "ZF_Help_Text_SurvivorWeapons");
        DrawPanelText(panel, buffer);

        DrawPanelText(panel, "----------------------------------------");
    }
    else if (team == zomTeam()) {
        Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_ZombieTeam");
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        char fullText[512];
        Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_ZombieTeam_Full");
        char lines[4][128];
        int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
        for (int i = 0; i < numLines; i++)
        {
            DrawPanelText(panel, lines[i]);
        }

        DrawPanelText(panel, "----------------------------------------");
    }
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Back");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpTeam, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpTeam(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleHelpTeam: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintHelpSurClass: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_SurvivorClasses");
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Soldier");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Sniper");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Medic");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Demoman");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Pyro");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Engineer");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpSurClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpSurClass(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleHelpSurClass: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintHelpZomClass: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_ZombieClasses");
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Scout");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Heavy");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Class_Spy");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpZomClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpZomClass(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleHelpZomClass: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintClass: client=%d, class=%d", client, class);
    Handle panel = CreatePanel();
    char   buffer[256];
    switch (class)
    {
        case TFClass_Soldier:
        {
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Soldier");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Soldier_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Pyro");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Pyro_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Demoman");
            SetPanelTitle(panel, buffer);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Demoman_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Engineer");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Engineer_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Medic");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Medic_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Sniper");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Sniper_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Scout");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Scout_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Heavy");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Heavy_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Help_Class_Spy");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Spy_Full");
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
            Format(buffer, sizeof(buffer), "%t", "ZF_Class_Spectator");
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%t", "ZF_Help_Text_Spectator_Full");
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
    }
    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Back");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleClass, 8);
    CloseHandle(panel);
}

public void panel_HandleClass(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandleClass: action=%d, client=%d, item=%d", action, param1, param2);
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
    ZF_LogDebug("panel_PrintPerkHelp: client=%d", client);
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Title_PerkHelp");
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Text_PerkHelp1");
    DrawPanelText(panel, buffer);

    Format(buffer, sizeof(buffer), "%t", "ZF_Help_Text_PerkHelp2");
    DrawPanelText(panel, buffer);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Back_Brackets");
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%t", "ZF_Menu_Close_Brackets");
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandlePerkHelp, 30);
    CloseHandle(panel);
}

public void panel_HandlePerkHelp(Handle menu, MenuAction action, int param1, int param2)
{
    ZF_LogDebug("panel_HandlePerkHelp: action=%d, client=%d, item=%d", action, param1, param2);
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
