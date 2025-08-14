#if defined _zf_perk_heroic_inc
    #endinput
#endif
#define _zf_perk_heroic_inc

#include "SurvivorBasePerk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_perk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#pragma newdecls required

// --- Constants
#define HEROIC_ATTACK_BONUS           10
#define HEROIC_DEFENSE_BONUS          10
#define HEROIC_CRITTIME_INIT          30
#define HEROIC_CRITTIME_KILL          3
#define HEROIC_CRITTIME_KILL_ACTIVE   0
#define HEROIC_CRITTIME_ASSIST        1
#define HEROIC_CRITTIME_ASSIST_ACTIVE 0

// --- Methodmap
methodmap HeroicPerk < SurvivorBasePerk
{

public     HeroicPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        HeroicPerk       sm      = view_as<HeroicPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, HeroicPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, HeroicPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, HeroicPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, HeroicPerkFonAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, HeroicPerkFupdateCondStats);

        return sm;
    }
}

stock SurvivorBasePerk HeroicPerkFnew(int client)
{
    return new HeroicPerk(client);
}

FUNCTION(HeroicPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Heroic");
}

FUNCTION(HeroicPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "HeroicPerk_shortdesc");
}

FUNCTION(HeroicPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "HeroicPerk_desc");
}

FUNCTION_INT(HeroicPerk, getCategory))
{
    return 1;
}

FUNCTION(HeroicPerk, updateClientPermStats))
{
    int client = _inst.client;

    addStat(client, ZFStatAtt, ZFStatTypePerm, HEROIC_ATTACK_BONUS);
    addStat(client, ZFStatDef, ZFStatTypePerm, HEROIC_DEFENSE_BONUS);
    zf_perkTimer[client] = HEROIC_CRITTIME_INIT;
    zf_perkState[client] = 0;
}

FUNCTION(HeroicPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int client = _inst.client;
    int time   = zf_perkState[client] == 1 ? HEROIC_CRITTIME_KILL_ACTIVE : HEROIC_CRITTIME_KILL;
    zf_perkTimer[client] += time;
}

FUNCTION(HeroicPerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int client = _inst.client;
    int time   = zf_perkState[client] == 1 ? HEROIC_CRITTIME_ASSIST_ACTIVE : HEROIC_CRITTIME_ASSIST;
    zf_perkTimer[client] += time;
}

FUNCTION(HeroicPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;
    if (!validLivingSur(client))
        return;

    int survivorCount = 0;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (validLivingSur(i))
        {
            survivorCount++;
        }
    }
    int state = zf_perkState[client];
    int timer = zf_perkTimer[client];

    if (survivorCount == 1 && state == 0)
    {
        zf_perkState[client] = 1;
        addCondKritz(client, float(timer));
        fxKritzStart(client);
        createAura(client, ZFPART_AURAINRED, AttachBase);
    }

    if (state == 1)
    {
        if (timer > 0)
        {
            zf_perkTimer[client]--;
        }
        else
        {
            removeAura(client);
            TF2_RemoveCondition(client, TFCond_Kritzkrieged);
            fxKritzStop(client);
            zf_perkState[client] = 2;    // 0: normal, 1: active, 2: used
        }
    }

    Format(buffer, maxlen, "%t", "HeroicPerk_Status_CritTime", timer);
}