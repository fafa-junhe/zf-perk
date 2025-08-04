#if defined __SpecterPerk_included
    #endinput
#endif
#define __SpecterPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"

#define SPECTER_ATT_BONUS      50
#define SPECTER_CRIT_BONUS     25
#define SPECTER_SPEED_BONUS    25
#define SPECTER_HEALTH         60
#define SPECTER_PHASE_DURATION 4.0
#define SPECTER_PHASE_COOLDOWN 30

#define SPECTER_DATA_TIMER     (PERK_DATA_START_INDEX + 1)

methodmap SpecterPerk < SurvivorBasePerk
{

public     SpecterPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        SpecterPerk      sm      = view_as<SpecterPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SpecterPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, SpecterPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, SpecterPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SpecterPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, SpecterPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GRACE_END, SpecterPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, SpecterPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, SpecterPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, SpecterPerkFupdateCondStats);

        sm.Position = DP(SPECTER_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer
    {

public         get()
        {
            this.Position = DP(SPECTER_DATA_TIMER);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = DP(SPECTER_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk SpecterPerkFnew(int client)
{
    return new SpecterPerk(client);
}

FUNCTION(SpecterPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Specter");
}

FUNCTION(SpecterPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "SpecterPerk_shortdesc");
}

FUNCTION(SpecterPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "SpecterPerk_desc");
}

FUNCTION(SpecterPerk, updateClientPermStats))
{
    int client = _inst.client;
    addStat(client, ZFStatAtt, ZFStatTypePerm, SPECTER_ATT_BONUS);
    addStat(client, ZFStatCrit, ZFStatTypePerm, SPECTER_CRIT_BONUS);
    addStat(client, ZFStatSpeed, ZFStatTypePerm, SPECTER_SPEED_BONUS);
}

FUNCTION(SpecterPerk, onPlayerSpawn))
{
    // Set health and reset cooldown
    SetEntityHealth(_inst.client, SPECTER_HEALTH);
    _inst.perk_timer = 0;
}

FUNCTION(SpecterPerk, onPeriodic))
{
    int client = _inst.client;

    // Force health to SPECTER_HEALTH to prevent healing.
    // This also acts as a fallback for faulty respawns where onPlayerSpawn might not be called.
    if (GetClientHealth(client) > SPECTER_HEALTH)
    {
        SetEntityHealth(client, SPECTER_HEALTH);
    }

    if (_inst.perk_timer > 0)
    {
        _inst.perk_timer--;
        if (_inst.perk_timer == 0)
        {
            PrintHintText(client, "%t", "SpecterPerk_Hint_Ready");
        }
    }
}

FUNCTION(SpecterPerk, onCallForMedic))
{
    int client = _inst.client;
    if (_inst.perk_timer > 0)
    {
        PrintHintText(client, "%t", "SpecterPerk_Hint_Cooldown", _inst.perk_timer);
        return;
    }

    _inst.perk_timer = SPECTER_PHASE_COOLDOWN;
    TF2_AddCondition(client, TFCond_Ubercharged, SPECTER_PHASE_DURATION);
    TF2_AddCondition(client, TFCond_Bonked, SPECTER_PHASE_DURATION);
    PrintHintText(client, "%t", "SpecterPerk_Hint_Activated");
}

FUNCTION(SpecterPerk, updateCondStats), char[] buffer, int maxlen)
{
    if (_inst.perk_timer > 0)
    {
        Format(buffer, maxlen, "%t", "SpecterPerk_Status_Cooldown", _inst.perk_timer);
    }
    else {
        Format(buffer, maxlen, "%t", "SpecterPerk_Status_Ready", _inst.perk_timer);
    }
}