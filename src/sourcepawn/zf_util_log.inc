#if defined _zf_util_log_included
#endinput
#endif
#define _zf_util_log_included

// 用于CSV日志记录的会话统计
int g_iSessionKills[MAXPLAYERS + 1];
int g_iSessionAssists[MAXPLAYERS + 1];
int g_iSessionDeaths[MAXPLAYERS + 1];

// ====================================================================================================
//
// CSV Logging
//
// ====================================================================================================

stock void InitSessionStats()
{
    for (int i = 0; i <= MAXPLAYERS; i++)
    {
        g_iSessionKills[i]   = 0;
        g_iSessionAssists[i] = 0;
        g_iSessionDeaths[i]  = 0;
    }
}

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
    ReplaceString(name, sizeof(name), ",", ";");    // Escape commas in name

    // SteamID
    char steamid[64];
    GetClientAuthId(client, AuthId_Steam2, steamid, sizeof(steamid));

    // Team, Class, Perk
    char teamStr[16];
    char classStr[32];
    char perkName[32];
    int  perkId;

    if (isSur(client))
    {
        strcopy(teamStr, sizeof(teamStr), "survivor");
        GetClassNameFromEnum(TF2_GetPlayerClass(client), classStr, sizeof(classStr));
        perkId = prefGet(client, SurPerk);
        GetPerkName(perkId, false, perkName, sizeof(perkName));
    }
    else if (isZom(client))
    {
        strcopy(teamStr, sizeof(teamStr), "zombie");
        GetClassNameFromEnum(TF2_GetPlayerClass(client), classStr, sizeof(classStr));
        perkId = prefGet(client, ZomPerk);
        GetPerkName(perkId, true, perkName, sizeof(perkName));
    }
    else
    {
        strcopy(teamStr, sizeof(teamStr), "spectator");
        strcopy(classStr, sizeof(classStr), "none");
        strcopy(perkName, sizeof(perkName), "none");
    }

    // Session Stats
    int kills          = g_iSessionKills[client];
    int assists        = g_iSessionAssists[client];
    int deaths         = g_iSessionDeaths[client];

    // Last Stats
    int lastAttack     = getStat(client, ZFStatAtt);
    int lastSpeed      = getStat(client, ZFStatSpeed);
    int lastCritChance = getStat(client, ZFStatCrit);
    int lastRof        = getStat(client, ZFStatRof);
    int lastDefense    = getStat(client, ZFStatDef);

    LogToFile(path, "%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d",
              name, steamid, teamStr, classStr, perkName,
              kills, assists, deaths,
              lastAttack, lastSpeed, lastCritChance, lastRof, lastDefense);
}

public void LogAndResetSession(int client)
{
    LogPlayerSessionData(client);

    // Reset session stats
    g_iSessionKills[client]   = 0;
    g_iSessionAssists[client] = 0;
    g_iSessionDeaths[client]  = 0;
}