#if defined _zf_util_bot_included
#endinput
#endif
#define _zf_util_bot_included

#include "zf_perk.inc"
/**
 * Automatically selects perks for a bot client.
 * Bots are always forced into random perk mode.
 *
 * @param client The client index of the bot.
 */
stock void bot_AutoSelectPerk(int client)
{
    // Force random perk mode for bots
    int validSurPerkCount = 0;
    int validZomPerkCount = 0;
    int validSurPerks[64];
    int validZomPerks[64];

    for (int i = 1; i < GetTotalSurPerks(); i++)
        if (surPerkEnabled(i))
            validSurPerks[validSurPerkCount++] = i;
    if (validSurPerkCount == 0)
        validSurPerks[validSurPerkCount++] = 0;

    for (int i = 1; i < GetTotalZomPerks(); i++)
        if (zomPerkEnabled(i))
            validZomPerks[validZomPerkCount++] = i;
    if (validZomPerkCount == 0)
        validZomPerks[validZomPerkCount++] = 0;

    prefSet(client, SurPendPerk, validSurPerks[GetRandomInt(0, validSurPerkCount - 1)]);
    prefSet(client, ZomPendPerk, validZomPerks[GetRandomInt(0, validZomPerkCount - 1)]);

    // Ensure the perk selection mode is correctly set for the bot's session
    prefSet(client, SurPerk, ZF_PERK_NONE);
    prefSet(client, ZomPerk, ZF_PERK_NONE);
    prefSet(client, PerkSelectMode, zf_perkMode);
}

stock void bot_onPlayerRunCmd(int client, int &buttons, int &impulse, float vel[3], float angles[3], int &weapon)
{
    // 如果是工程师或间谍，则不执行任何操作
    TFClassType class = TF2_GetPlayerClass(client);
    if (class == TFClass_Engineer || class == TFClass_Spy)
    {
        return;
    }
    angles[0] = angles[0];
    // --- 基本变量 ---
    float myPos[3];
    GetClientAbsOrigin(client, myPos);
    int myTeam = GetClientTeam(client);

    // --- 寻找目标 ---
    int closestEnemy = -1;
    float closestEnemyDist = 999999.0;
    float closestEnemyPos[3];
    int closestTeammate = -1;
    float closestTeammateDist = 999999.0;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (i == client || !IsClientInGame(i) || !IsPlayerAlive(i))
            continue;

        float targetPos[3];
        GetClientAbsOrigin(i, targetPos);
        float dist = GetVectorDistance(myPos, targetPos);

        if (GetClientTeam(i) != myTeam)
        {
            if (dist < closestEnemyDist)
            {
                closestEnemyDist = dist;
                closestEnemy = i;
                closestEnemyPos = targetPos;
            }
        }
        else
        {
            if (dist < closestTeammateDist)
            {
                closestTeammateDist = dist;
                closestTeammate = i;
            }
        }
    }


    // --- 根据职业调整行为 ---

    // 近战/光环类: 敌人近身时呼叫
    if (isClientPerkNameEquals(client, "Echo") || isClientPerkNameEquals(client, "Charger") || isClientPerkNameEquals(client, "Geomancer") || isClientPerkNameEquals(client, "Roar") || isClientPerkNameEquals(client, "Phantasm") || isClientPerkNameEquals(client, "Volatile"))
    {
        if (closestEnemy != -1 && closestEnemyDist < 450.0)
        {
            ClientCommand(client, "voicemenu 0 0");
        }
    }
    // 远程类: 敌人在远处时呼叫
    else if (isClientPerkNameEquals(client, "Ninja") || isClientPerkNameEquals(client, "Alpha") || isClientPerkNameEquals(client, "Leap") || isClientPerkNameEquals(client, "Hunter") || isClientPerkNameEquals(client, "Overlord") || isClientPerkNameEquals(client, "Sick"))
    {
        if (closestEnemy != -1 && closestEnemyDist > 500.0)
        {
            ClientCommand(client, "voicemenu 0 0");
        }
    }
    // 辅助类 (普通): 队友在附近时呼叫
    else if (isClientPerkNameEquals(client, "Charitable"))
    {
        if (closestTeammate != -1 && closestTeammateDist < 600.0)
        {
            ClientCommand(client, "voicemenu 0 0");
        }
    }
    // 辅助类 (蹲下): 队友在附近时蹲下并呼叫
    else if (isClientPerkNameEquals(client, "Leader") || isClientPerkNameEquals(client, "Trapper") || isClientPerkNameEquals(client, "Carpenter") || isClientPerkNameEquals(client, "Alchemist"))
    {
        if (closestTeammate != -1 && closestTeammateDist < 600.0)
        {
            buttons |= IN_DUCK;
            ClientCommand(client, "voicemenu 0 0");
        }
    }
    // 持续蹲伏类
    else if (isClientPerkNameEquals(client, "Zenlike") || isClientPerkNameEquals(client, "Holy"))
    {
        buttons |= IN_DUCK;
    }
    // 持续呼叫类
    else if (isClientPerkNameEquals(client, "Specter") || isClientPerkNameEquals(client, "Tantrum"))
    {
        ClientCommand(client, "voicemenu 0 0");
    }
    // 光环不动类: 敌人在近处时停下
    else if (isClientPerkNameEquals(client, "Toxic") || isClientPerkNameEquals(client, "StaticField") || isClientPerkNameEquals(client, "Guardian"))
    {
        if (closestEnemy != -1 && closestEnemyDist < 300.0)
        {
            vel[0] = 0.0;
            vel[1] = 0.0;
        }
    }

    // --- 独立逻辑 (覆盖默认移动) ---

    // 远离僵尸类
    if (isClientPerkNameEquals(client, "Holy") || isClientPerkNameEquals(client, "Cowardly") || isClientPerkNameEquals(client, "Ninja"))
    {
        if (closestEnemy != -1 && myTeam == view_as<int>(TFTeam_Blue)) // 假设幸存者是蓝队
        {
            float runAwayVec[3];
            MakeVectorFromPoints(closestEnemyPos, myPos, runAwayVec);
            NormalizeVector(runAwayVec, vel);
            ScaleVector(vel, 300.0);
        }
    }

    // --- 无限弹药 ---
    int activeSlot = activeWeaponSlot(client);
    if (activeSlot != -1)
    {
        addClipAmmo(client, activeSlot, 999);
        addResAmmo(client, activeSlot, 999);
    }
}
