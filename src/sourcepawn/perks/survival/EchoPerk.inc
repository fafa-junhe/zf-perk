#if defined __EchoPerk_included
    #endinput
#endif
#define __EchoPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"

#define ECHO_KNOCKBACK_BASE_FORCE   200.0
#define ECHO_KNOCKBACK_ENERGY_SCALE 50.0
#define ECHO_AURA_RADIUS            300.0
#define ECHO_AURA_RADIUS_SQ         (ECHO_AURA_RADIUS * ECHO_AURA_RADIUS)

methodmap EchoPerk < SurvivorBasePerk
{

public     EchoPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        EchoPerk         sm      = view_as<EchoPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, EchoPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, EchoPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, EchoPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, EchoPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, EchoPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, EchoPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, EchoPerkFonPlayerSpawn);

        return sm;
    }
}

stock SurvivorBasePerk EchoPerkFnew(int client)
{
    return new EchoPerk(client);
}

FUNCTION(EchoPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Echo");
}

FUNCTION(EchoPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "EchoPerk_shortdesc");
}

FUNCTION(EchoPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "EchoPerk_desc");
}

FUNCTION(EchoPerk, onPlayerSpawn))
{
    zf_perkState[_inst.client] = 0;    // Echo energy
}

FUNCTION_FLOAT(EchoPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    int client = _inst.client;
    if (victim != client)
    {
        return damage;
    }

    // Knockback immunity (by negating knockback-related damage types)
    if (damagetype & (DMG_BLAST | DMG_AIRBOAT))
    {
        // This is a simple way to reduce knockback from explosions/airblast
        // A more robust solution would involve velocity checks in OnPlayerRunCmd
        return 0.0;
    }

    // Store energy from zombie melee attacks
    if (validClient(attacker) && isZom(attacker) && (damagetype & DMG_CLUB))
    {
        zf_perkState[client]++;
        PrintHintText(client, "%t", "EchoPerk_Hint_EnergyStored", zf_perkState[client]);
    }

    return damage;
}

FUNCTION(EchoPerk, onCallForMedic))
{
    int client = _inst.client;
    int energy = zf_perkState[client];

    if (energy <= 0)
    {
        PrintHintText(client, "%t", "EchoPerk_Hint_NoEnergy");
        return;
    }

    float client_pos[3];
    GetClientAbsOrigin(client, client_pos);

    float knockback_force = ECHO_KNOCKBACK_BASE_FORCE + (energy * ECHO_KNOCKBACK_ENERGY_SCALE);

    // Create visual effect
    fxCreateParticle("particles/explosion_shockwave.pcf", client, AttachBase, 2.0);
    fxCreateSoundToAll(ZFSND_PUSHBACK, client)

    for (int i = 1; i <= MaxClients; i++)
    {
        if (validClient(i) && isZom(i) && IsPlayerAlive(i))
        {
            float zom_pos[3];
            GetClientAbsOrigin(i, zom_pos);

            if (GetVectorDistance(client_pos, zom_pos, true) < ECHO_AURA_RADIUS_SQ)
            {
                float direction[3];
                SubtractVectors(zom_pos, client_pos, direction);
                // Add some upward force
                direction[2] += 50.0;
                NormalizeVector(direction, direction);

                fxApplyForce(i, direction, knockback_force);
                fxCreateSoundToClient(ZFSND_THUD, i);
            }
        }
    }

    PrintHintText(client, "%t", "EchoPerk_Hint_EnergyReleased", energy);
    zf_perkState[client] = 0;    // Reset energy
}

FUNCTION(EchoPerk, updateCondStats), char[] buffer, int maxlen)
{
    int energy = zf_perkState[_inst.client];
    if (energy > 0)
    {
        Format(buffer, maxlen, "%t", "EchoPerk_Status_Energy", energy);
    }
    else {
        Format(buffer, maxlen, "%t", "EchoPerk_Status_Ready");
    }
}