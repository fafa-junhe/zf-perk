#if defined _zf_util_team_included
#endinput
#endif
#define _zf_util_team_included

#include "zf_perk.inc"

/**
 * Custom sort function to sort players by their zombie priority score in descending order.
 *
 * @param elem1         The first client index.
 * @param elem2         The second client index.
 * @param array         The array being sorted.
 * @param hndl          Optional data handle.
 * @return              -1 if elem1 > elem2, 1 if elem1 < elem2, 0 if equal.
 */
public int SortByZombiePriority(int elem1, int elem2, const int[] array, Handle hndl)
{
    int priority1 = g_iZombiePriority[elem1];
    int priority2 = g_iZombiePriority[elem2];

    if (priority1 > priority2)
    {
        return -1;    // elem1 comes first (higher priority)
    }
    if (priority1 < priority2)
    {
        return 1;    // elem2 comes first
    }
    return GetRandomInt(0, 1) ? -1 : 1;
}

/**
 * Assigns players to survivor and zombie teams at the start of a new round.
 */
stock void AssignTeams()
{
    int players[MAXPLAYERS];
    int playerCount = 0;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && (GetClientTeam(i) > 1))
        {
            players[playerCount++] = i;
        }
    }

    // Randomize the initial list of players
    SortIntegers(players, playerCount, Sort_Random);
    // NOTE: As of SM 1.3.1, SortIntegers w/ Sort_Random doesn't
    //       sort the first element of the array. Temp fix below.
    if (playerCount > 1)
    {
        int idx      = GetRandomInt(0, playerCount - 1);
        int temp     = players[idx];
        players[idx] = players[0];
        players[0]   = temp;
    }

    // Sort the player list based on the zombie priority score in descending order.
    // Players with a higher score (more consecutive rounds as a survivor) will be at the front.
    SortCustom1D(players, playerCount, SortByZombiePriority);

    // Calculate team counts based on the ratio.
    int surCount = RoundToFloor(playerCount * GetConVarFloat(zf_cvRatio));

    // --- Team Balance Rules ---
    // 1. If there are players but the calculation resulted in zero survivors, force at least one.
    if (playerCount > 0 && surCount == 0)
    {
        surCount = 1;
    }
    // 2. If there are multiple players but the calculation resulted in zero zombies, force at least one.
    if (playerCount > 1 && surCount == playerCount)
    {
        surCount = playerCount - 1;
    }

    // Assign active players to survivor and zombie teams.
    for (int i = 0; i < surCount; i++)
        spawnClient(players[i], zomTeam());
    for (int i = surCount; i < playerCount; i++)
        spawnClient(players[i], surTeam());
}
