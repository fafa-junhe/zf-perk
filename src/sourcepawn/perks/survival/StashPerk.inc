#if defined __StashPerk_included
    #endinput
#endif
#define __StashPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define STASH_GRAB_ATTACK_DURATION 45
#define STASH_GRAB_ATTACK_PERM     10
#define STASH_GRAB_ATTACK_TEMP     100
#define STASH_GRAB_HEALTH          200
#define STASH_COOLDOWN             30
#define STASH_WARMUP               40
#define STASH_GRAB_RADSQ           (50 * 50)
#define STASH_DROP_RADSQ_STASH     (200 * 200)
#define STASH_DROP_RADSQ_CLIENT    (800 * 800)

methodmap StashPerk < SurvivorBasePerk
{

public     StashPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        StashPerk        sm      = view_as<StashPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, StashPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, StashPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, StashPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, StashPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, StashPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, StashPerkFonDeath);

        return sm;
    }
}

stock SurvivorBasePerk StashPerkFnew(int client)
{
    return new StashPerk(client);
}

FUNCTION(StashPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Stash");
}

FUNCTION(StashPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "StashPerk_shortdesc");
}

FUNCTION(StashPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "StashPerk_desc");
}

FUNCTION(StashPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;
    int timer  = zf_perkTimer[client];
    int stash  = zf_item[client][0];

    if (timer > 0)
    {
        zf_perkTimer[client]--;
        if (timer == STASH_COOLDOWN && validItem(client, 0))
        {
            PrintHintText(client, "%t", "StashPerk_Hint_Ready");
        }
        else if (timer == 1) {
            PrintHintText(client, "%t", "StashPerk_Hint_CanPlaceNew");
            if (validItem(client, 0))
            {
                removeItem(client, 0);
            }
        }
    }

    if (validItem(client, 0) && timer <= STASH_COOLDOWN)
    {
        float stashPos[3];
        GetEntPropVector(stash, Prop_Send, "m_vecOrigin", stashPos);
        float clientPos[3];
        GetClientAbsOrigin(client, clientPos);

        if (GetVectorDistance(stashPos, clientPos, true) <= STASH_GRAB_RADSQ)
        {
            addHealth(client, STASH_GRAB_HEALTH, STASH_GRAB_HEALTH);
            addResAmmoPct(client, 0, 1.0);
            addResAmmoPct(client, 1, 1.0);
            addMetal(client, 200);
            addStat(client, ZFStatAtt, ZFStatTypePerm, STASH_GRAB_ATTACK_PERM);
            addStatTempStack(client, ZFStatAtt, STASH_GRAB_ATTACK_TEMP, STASH_GRAB_ATTACK_DURATION);
            removeItem(client, 0);
            PrintHintText(client, "%t", "StashPerk_Hint_PickedUp");
        }
    }

    if (timer > STASH_COOLDOWN)
    {
        Format(buffer, maxlen, "%t", "StashPerk_Status_Warmup", timer - STASH_COOLDOWN);
    }
    else if (timer > 0) {
        Format(buffer, maxlen, "Cooldown: %ds", timer);
    }
    else {
        Format(buffer, maxlen, "%t", "StashPerk_Status_Ready");
    }
}

FUNCTION(StashPerk, onCallForMedic))
{
    int client = _inst.client;

    if (zf_perkTimer[client] > 0) return;
    if (!isGrounded(client) || !isCrouching(client)) return;

    float pos[3];
    GetClientAbsOrigin(client, pos);

    for (int i = 1; i <= MaxClients; i++)
    {
        if (i == client) continue;
        if (validLivingSur(i))
        {
            float otherPos[3];
            GetClientAbsOrigin(i, otherPos);
            if (GetVectorDistance(pos, otherPos, true) < STASH_DROP_RADSQ_CLIENT)
            {
                PrintHintText(client, "%t", "StashPerk_Hint_TooCloseToSurvivor");
                return;
            }
        }
        if (validItem(i, 0))
        {
            float itemPos[3];
            GetEntPropVector(zf_item[i][0], Prop_Send, "m_vecOrigin", itemPos);
            if (GetVectorDistance(pos, itemPos, true) < STASH_DROP_RADSQ_STASH)
            {
                PrintHintText(client, "%t", "StashPerk_Hint_TooCloseToStash");
                return;
            }
        }
    }

    int entity = fxCreateModelStatic(ZFMDL_SUPPLYCRATE, client, true, true, pos);
    if (entity != -1)
    {
        zf_item[client][0]   = entity;
        zf_perkTimer[client] = STASH_WARMUP + STASH_COOLDOWN;
        PrintHintText(client, "%t", "StashPerk_Hint_Placed");
    }
}

FUNCTION(StashPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    removeItem(_inst.client, 0);
}
