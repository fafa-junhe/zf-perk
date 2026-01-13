#if defined __RicochetSpecialistPerk_included
    #endinput
#endif
#define __RicochetSpecialistPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdktools_tempents>

#define RICOCHET_CHANCE                   50
#define RICOCHET_DAMAGE_MULTIPLIER_BULLET 0.7    // The "passive" 30% damage reduction for bullets
#define RICOCHET_BOUNCE_DAMAGE_MULTIPLIER 0.5    // Each bounce deals half the damage of the previous hit
#define RICOCHET_MAX_BOUNCES              4
#define RICOCHET_SEARCH_RADIUS            400.0
#define RICOCHET_SEARCH_RADIUS_SQ         (RICOCHET_SEARCH_RADIUS * RICOCHET_SEARCH_RADIUS)

methodmap RicochetSpecialistPerk < SurvivorBasePerk
{

public     RicochetSpecialistPerk(int client)
    {
        SurvivorBasePerk       sm_base = new SurvivorBasePerk(client);
        RicochetSpecialistPerk sm      = view_as<RicochetSpecialistPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, RicochetSpecialistPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, RicochetSpecialistPerkFonDealDamagePost);

        static bool s_bInitialized = false;
        if (!s_bInitialized)
        {
            PrecacheModel("sprites/laserbeam.vmt", true);
            s_bInitialized = true;
        }

        return sm;
    }
}

stock SurvivorBasePerk RicochetSpecialistPerkFnew(int client)
{
    return new RicochetSpecialistPerk(client);
}

FUNCTION(RicochetSpecialistPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "RicochetSpecialist");
}

FUNCTION(RicochetSpecialistPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "RicochetSpecialistPerk_shortdesc");
}

FUNCTION(RicochetSpecialistPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "RicochetSpecialistPerk_desc");
}

FUNCTION_INT(RicochetSpecialistPerk, getCategory))
{
    return 1;
}

// Use an unused damage type to mark our bounced damage and prevent recursive triggers.
#define DMG_RICOCHET DMG_PREVENT_PHYSICS_FORCE

/**
 * This is the entry point for the ricochet effect.
 * It only triggers on the player's direct bullet hits, not from the bounced damage itself.
 */
FUNCTION(RicochetSpecialistPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    // This perk only applies to the owner, hitting a zombie, with a bullet, and not already a ricochet.
    if (attacker != _inst.client || !isZom(victim) || (damagetype & DMG_RICOCHET))
    {
        return;
    }

    // Check for the ricochet chance
    if (GetRandomInt(1, 100) <= RICOCHET_CHANCE)
    {
        float victim_pos[3];
        GetClientEyePosition(victim, victim_pos);

        // Find the closest valid zombie to bounce to
        int next_target = findNextRicochetTarget(victim, victim_pos);

        if (next_target != -1)
        {
            // Start the bounce chain. The chain is handled entirely by the timer function.
            float    bounce_damage = damage * RICOCHET_BOUNCE_DAMAGE_MULTIPLIER;

            DataPack hPack         = new DataPack();
            hPack.WriteCell(next_target);
            hPack.WriteCell(attacker);
            hPack.WriteCell(inflictor);
            hPack.WriteFloat(bounce_damage);
            hPack.WriteCell(damagetype | DMG_RICOCHET);    // Add our custom flag
            hPack.WriteCell(1);                            // Initial bounce count
            hPack.WriteFloat(victim_pos[0]);               // Origin position X for the beam effect
            hPack.WriteFloat(victim_pos[1]);               // Origin position Y
            hPack.WriteFloat(victim_pos[2]);               // Origin position Z
            CreateTimer(0.01, Ricochet_Chain, hPack);
        }
    }
}

/**
 * Handles a single link in the ricochet chain and creates the next one if applicable.
 * This approach avoids race conditions by making the chain state self-contained in the DataPack.
 */
public Action Ricochet_Chain(Handle timer, DataPack hPack)
{
    hPack.Position     = DP(0);
    int   victim       = hPack.ReadCell();
    int   attacker     = hPack.ReadCell();
    int   inflictor    = hPack.ReadCell();
    float damage       = hPack.ReadFloat();
    int   damagetype   = hPack.ReadCell();
    int   bounce_count = hPack.ReadCell();
    float last_victim_pos[3];
    last_victim_pos[0] = hPack.ReadFloat();
    last_victim_pos[1] = hPack.ReadFloat();
    last_victim_pos[2] = hPack.ReadFloat();

    delete hPack;

    // Use IsClientInGame for a more robust check to ensure players are still connected.
    if (!IsClientInGame(victim) || !IsPlayerAlive(victim))
    {
        return Plugin_Stop;    // Target is no longer valid, stop the chain.
    }
    if (!IsClientInGame(attacker))
    {
        return Plugin_Stop;    // Attacker is no longer valid, stop the chain.
    }

    // Also validate the weapon entity, as it might be removed if the attacker disconnects or switches weapons.
    if (!IsValidEntity(inflictor))
    {
        return Plugin_Stop;
    }

    // --- Visuals & Sound ---
    float victim_pos[3];
    GetClientEyePosition(victim, victim_pos);

    int beamSprite = PrecacheModel("sprites/physbeam.vmt");
    victim_pos[2] -= 30;
    last_victim_pos[2] -= 30;
    TE_SetupBeamPoints(last_victim_pos, victim_pos, beamSprite, 0, 0, 10, 0.3, 5.0, 5.0, 10, 0.0, { 0, 255, 255, 255 }, 10);
    TE_SendToAll();
    PrintHintText(attacker, "%t", "RicochetSpecialist_Hint_Bounced", bounce_count);
    // --- Deal Damage ---
    SDKHooks_TakeDamage(victim, attacker, inflictor, damage, damagetype);

    // --- Check for Next Bounce ---
    if (bounce_count >= RICOCHET_MAX_BOUNCES)
    {
        return Plugin_Stop;    // Max bounces reached.
    }
    victim_pos[2] += 30;
    int next_target = findNextRicochetTarget(victim, victim_pos);

    if (next_target != -1)
    {
        // Create the next link in the chain
        float    next_bounce_damage = damage * RICOCHET_BOUNCE_DAMAGE_MULTIPLIER;

        DataPack next_hPack         = new DataPack();
        next_hPack.WriteCell(next_target);
        next_hPack.WriteCell(attacker);
        next_hPack.WriteCell(inflictor);
        next_hPack.WriteFloat(next_bounce_damage);
        next_hPack.WriteCell(damagetype);
        next_hPack.WriteCell(bounce_count + 1);
        next_hPack.WriteFloat(victim_pos[0]);    // The new origin X for the next beam
        next_hPack.WriteFloat(victim_pos[1]);    // The new origin Y
        next_hPack.WriteFloat(victim_pos[2]);    // The new origin Z
        CreateTimer(0.01, Ricochet_Chain, next_hPack);
    }

    return Plugin_Stop;
}

stock int findNextRicochetTarget(int current_victim, float origin_pos[3])
{
    int   next_target = -1;
    float min_dist_sq = RICOCHET_SEARCH_RADIUS_SQ;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (i == current_victim || !IsClientInGame(i) || !isZom(i) || !IsPlayerAlive(i))
        {
            continue;
        }

        float other_pos[3];
        GetClientEyePosition(i, other_pos);
        float dist_sq = GetVectorDistance(origin_pos, other_pos, true);

        if (dist_sq < min_dist_sq)
        {
            min_dist_sq = dist_sq;
            next_target = i;
        }
    }
    return next_target;
}