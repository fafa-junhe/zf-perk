#if defined _zf_perk_scavenger_inc
    #endinput
#endif
#define _zf_perk_scavenger_inc

#include "SurvivorBasePerk.inc"

// --- Constants
#define ZF_PERK_SCAVENGER_ID    9
#define SCAVENGER_CRIT_CHANCE   50
#define SCAVENGER_METAL_CHANCE  50
#define SCAVENGER_CRIT_DURATION 10
#define SCAVENGER_METAL_AMOUNT  50

// --- Methodmap
methodmap ScavengerPerk < SurvivorBasePerk
{

public     ScavengerPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        ScavengerPerk    sm      = view_as<ScavengerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ScavengerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ScavengerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ScavengerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_AMMO_PICKUP, ScavengerPerkFonAmmoPickup);

        return sm;
    }
}

stock SurvivorBasePerk ScavengerPerkFnew(int client)
{
    return new ScavengerPerk(client);
}

FUNCTION(ScavengerPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Scavenger");
}

FUNCTION(ScavengerPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ScavengerPerk_shortdesc");
}

FUNCTION(ScavengerPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ScavengerPerk_desc");
}

// --- Vtable Functions
FUNCTION(ScavengerPerk, onAmmoPickup), int pickup)
{
    int client = _inst.client;
    if (!validLivingSur(client))
        return;

    if (GetRandomInt(1, 100) <= SCAVENGER_CRIT_CHANCE)
    {
        addCondKritz(client, float(SCAVENGER_CRIT_DURATION));
        fxKritzStart(client);
        PrintHintText(client, "%t", "ScavengerPerk_Hint_Crits");
    }
    else if (GetRandomInt(1, 100) <= SCAVENGER_METAL_CHANCE)
    {
        int currentMetal = getMetal(client);
        setMetal(client, currentMetal + SCAVENGER_METAL_AMOUNT);
        PrintHintText(client, "%t", "ScavengerPerk_Hint_Metal");
    }
}