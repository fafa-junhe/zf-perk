#if defined _zf_util_szf_included
#endinput
#endif
#define _zf_util_szf_included

#pragma newdecls required
#include "zf_perk.inc"
ConVar g_hCvarSzfSpecialPerkChance;
ArrayList g_hSzfRandomSurvivorPerks;
ArrayList g_hSzfRandomZombiePerks;
ArrayList g_hSzfPerksToDisable;

/**
 * Initializes the SZF compatibility module.
 * Called once on plugin start.
 */
stock void SZF_Init()
{
    g_hCvarSzfSpecialPerkChance = CreateConVar("zf_szf_special_perk_chance", "10", "The chance (out of 100) for a player to be assigned a special random perk in SZF mode.", 0, true, 0.0, true, 100.0);

    g_hSzfRandomSurvivorPerks = new ArrayList();
    g_hSzfRandomZombiePerks = new ArrayList();
    g_hSzfPerksToDisable = new ArrayList(ByteCountToCells(32));

    // --- USER CONFIGURATION ---
    // The user can configure the SZF behavior here.

    // Queue perks to be disabled in SZF mode
    g_hSzfPerksToDisable.PushString("Alpha");
    g_hSzfPerksToDisable.PushString("Vindictive");
    g_hSzfPerksToDisable.PushString("Phantasm");
    g_hSzfPerksToDisable.PushString("Static Field");
    g_hSzfPerksToDisable.PushString("Gravity Warper");
    g_hSzfPerksToDisable.PushString("Toxic");
    g_hSzfPerksToDisable.PushString("Roar");

    // Example: Make perks unselectable for random assignment
    g_hSzfPerksToDisable.PushString("Combustible");
    g_hSzfPerksToDisable.PushString("Anchor");
    g_hSzfPerksToDisable.PushString("Charger");
    g_hSzfPerksToDisable.PushString("Rage");
    SZF_RegisterRandomOverridePerk("Combustible", true); // false for survivor
    SZF_RegisterRandomOverridePerk("Anchor", true); // false for survivor
    SZF_RegisterRandomOverridePerk("Charger", true); // false for survivor
    SZF_RegisterRandomOverridePerk("Rage", true); // false for survivor
    

}

/**
 * Updates the availability of perks based on the SZF compatibility mode.
 */
stock void SZF_UpdatePerkAvailability()
{
    // Reset all perks to enabled state first.
    zf_surPerksEnabled = 0xFFFF_FFFF;
    zf_zomPerksEnabled = 0xFFFF_FFFF;

    // If SZF mode is enabled, disable the specified perks.
    if (g_bSzfCompatible)
    {
        char perkName[32];
        for (int i = 0; i < g_hSzfPerksToDisable.Length; i++)
        {
            g_hSzfPerksToDisable.GetString(i, perkName, sizeof(perkName));
            SZF_DisablePerk(perkName);
        }
        LogMessage("[SZF] Compatibility mode enabled. Restricted perks have been disabled.");
    }
    else
    {
        LogMessage("[SZF] Compatibility mode disabled. All perks are available.");
    }
}

/**
 * Main entry point for SZF compatibility logic on player spawn.
 *
 * @param client The client index.
 */
stock void SZF_OnPlayerSpawn(int client)
{
    if (!g_bSzfCompatible)
        return;

    SZF_OverridePerkSelection(client);
}

/**
 * Disables a perk by its name.
 *
 * @param perkName The name of the perk to disable (e.g., "Roar").
 */
stock void SZF_DisablePerk(const char[] perkNameToDisable)
{
    // --- Survivor Perks ---
    for (int i = 0; i < g_SurPerkTypes.Length; i++)
    {
        char currentPerkName[64];
        GetPerkName(i, false, currentPerkName, sizeof(currentPerkName));
        if (StrEqual(currentPerkName, perkNameToDisable, false))
        {
            zf_surPerksEnabled &= ~(1 << i);
            ZF_LogDebug("[SZF] Disabled Survivor Perk: %s (ID: %d)", perkNameToDisable, i);
            return; // Perk found and disabled, no need to check zombie list.
        }
    }

    // --- Zombie Perks ---
    for (int i = 0; i < g_ZomPerkTypes.Length; i++)
    {
        char currentPerkName[64];
        GetPerkName(i, true, currentPerkName, sizeof(currentPerkName));
        if (StrEqual(currentPerkName, perkNameToDisable, false))
        {
            zf_zomPerksEnabled &= ~(1 << i);
            ZF_LogDebug("[SZF] Disabled Zombie Perk: %s (ID: %d)", perkNameToDisable, i);
            return; // Perk found and disabled.
        }
    }
}

/**
 * Sets a usage limit for a specific perk.
 *
 * @param perkName The name of the perk.
 * @param limit The maximum number of players that can use this perk.
 */
stock void SZF_LimitPerk(const char[] perkName, int limit)
{
    int perkId;
    if (g_SurPerkRegistry.GetValue(perkName, perkId))
    {
        zf_surPerksLimit[perkId] = limit;
        ZF_LogDebug("[SZF] Limited Survivor Perk '%s' to %d players.", perkName, limit);
    }
    else if (g_ZomPerkRegistry.GetValue(perkName, perkId))
    {
        zf_zomPerksLimit[perkId] = limit;
        ZF_LogDebug("[SZF] Limited Zombie Perk '%s' to %d players.", perkName, limit);
    }
}

/**
 * Registers a perk to be part of the random override pool.
 *
 * @param perkName The name of the perk.
 * @param isZombie True if it's a zombie perk, false for survivor.
 */
stock void SZF_RegisterRandomOverridePerk(const char[] perkNameToRegister, bool isZombie)
{
    if (isZombie)
    {
        for (int i = 0; i < g_ZomPerkTypes.Length; i++)
        {
            char currentPerkName[64];
            GetPerkName(i, true, currentPerkName, sizeof(currentPerkName));
            if (StrEqual(currentPerkName, perkNameToRegister, false))
            {
                g_hSzfRandomZombiePerks.Push(i);
                ZF_LogDebug("[SZF] Registered '%s' (ID: %d) to the random zombie perk pool.", perkNameToRegister, i);
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < g_SurPerkTypes.Length; i++)
        {
            char currentPerkName[64];
            GetPerkName(i, false, currentPerkName, sizeof(currentPerkName));
            if (StrEqual(currentPerkName, perkNameToRegister, false))
            {
                g_hSzfRandomSurvivorPerks.Push(i);
                ZF_LogDebug("[SZF] Registered '%s' (ID: %d) to the random survivor perk pool.", perkNameToRegister, i);
                return;
            }
        }
    }
}

/**
 * Overrides the player's pending perk selection if conditions are met.
 *
 * @param client The client index.
 */
stock void SZF_OverridePerkSelection(int client)
{
    // Example condition: Only run this logic if there are more than 20 players.
    // if (GetClientCount(true) < 20) return;

    // Example condition: 10% chance to get a special random perk.
    if (GetRandomInt(1, 100) > g_hCvarSzfSpecialPerkChance.IntValue) return;

    if (isSur(client) && g_hSzfRandomSurvivorPerks.Length > 0)
    {
        int perkId = 0;
        int attempts = 0;
        while (perkId == 0 && attempts < 10)
        {
            int randomIndex = GetRandomInt(0, g_hSzfRandomSurvivorPerks.Length - 1);
            perkId = view_as<int>(g_hSzfRandomSurvivorPerks.Get(randomIndex));
            attempts++;
        }

        if (perkId != 0)
        {
            prefSet(client, SurPendPerk, perkId);

            char perkName[64];
            GetPerkDisplayName(client, perkId, false, perkName, sizeof(perkName));
            PrintToChat(client, "%t", "SZF_SpecialPerkAssigned", perkName);
        }
    }
    else if (isZom(client) && g_hSzfRandomZombiePerks.Length > 0)
    {
        int perkId = 0;
        int attempts = 0;
        while (perkId == 0 && attempts < 10)
        {
            int randomIndex = GetRandomInt(0, g_hSzfRandomZombiePerks.Length - 1);
            perkId = view_as<int>(g_hSzfRandomZombiePerks.Get(randomIndex));
            attempts++;
        }

        if (perkId != 0)
        {
            prefSet(client, ZomPendPerk, perkId);

            char perkName[64];
            GetPerkDisplayName(client, perkId, true, perkName, sizeof(perkName));
            PrintToChat(client, "%t", "SZF_SpecialPerkAssigned", perkName);
        }

    }
}

/**
 * Called at the start of each round to announce SZF mode if active.
 */
stock void SZF_OnRoundStart()
{
    if (g_bSzfCompatible)
    {
        PrintToChatAll("%t", "SZF_ModeActive_Announce");
    }
}
