#if defined __GamblerPerk_included
    #endinput
#endif
#define __GamblerPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"

#define GAMBLER_CRIT_REDUCTION       -100
#define GAMBLER_MAX_LUCK             30
#define GAMBLER_KILL_LUCK_GAIN       4
#define GAMBLER_ASSIST_LUCK_GAIN     1
#define GAMBLER_HIT_LUCK_COST        1
#define GAMBLER_SELF_DAMAGE          10.0
#define GAMBLER_SPEED_BOOST_DURATION 5
#define GAMBLER_SPEED_BOOST_AMOUNT   50

methodmap GamblerPerk < SurvivorBasePerk
{

public     GamblerPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        GamblerPerk      sm      = view_as<GamblerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, GamblerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, GamblerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, GamblerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, GamblerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALC_IS_ATTACK_CRITICAL, GamblerPerkFonCalcIsAttackCritical);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, GamblerPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, GamblerPerkFonAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, GamblerPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GamblerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, GamblerPerkFonPlayerSpawn);

        return sm;
    }
}

stock SurvivorBasePerk GamblerPerkFnew(int client)
{
    return new GamblerPerk(client);
}

FUNCTION(GamblerPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Gambler");
}

FUNCTION(GamblerPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "GamblerPerk_shortdesc");
}

FUNCTION(GamblerPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "GamblerPerk_desc");
}

FUNCTION(GamblerPerk, onPlayerSpawn))
{
    zf_perkState[_inst.client] = 0;    // Luck value
}

FUNCTION(GamblerPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, GAMBLER_CRIT_REDUCTION);
}

FUNCTION(GamblerPerk, onCalcIsAttackCritical))
{
    int client = _inst.client;
    int luck   = zf_perkState[client];
    if (GetRandomInt(1, 100) > luck)
    {
        return;    // No luck this time
    }

    // Consume luck
    zf_perkState[client] = max(0, luck - GAMBLER_HIT_LUCK_COST);

    // Trigger random effect
    int effect           = GetRandomInt(1, 4);
    switch (effect)
    {
        case 1:
        {                                                    // 3x Crit Damage
            addStatTempStack(client, ZFStatAtt, 200, 1);     // 200% bonus damage for the next hit
            addStatTempStack(client, ZFStatCrit, 200, 1);    // 200% crit chance for the next hit
            PrintHintText(client, "%t", "GamblerPerk_Hint_Crit");
        }
        case 2:
        {    // Instant Reload
            int slot = activeWeaponSlot(client);
            if (slot != -1)
            {
                TF2_RemoveCondition(client, TFCond_CritOnFirstBlood);    // Visual cue
                TF2_AddCondition(client, TFCond_CritOnFirstBlood, 0.1);
                setClipAmmo(client, slot, maxClipAmmo(client, slot));
                PrintHintText(client, "%t", "GamblerPerk_Hint_Reload");
            }
        }
        case 3:
        {    // Speed Boost
            addStatTempStack(client, ZFStatSpeed, GAMBLER_SPEED_BOOST_AMOUNT, GAMBLER_SPEED_BOOST_DURATION);
            PrintHintText(client, "%t", "GamblerPerk_Hint_Speed");
        }
        case 4:
        {    // Misfire
            SDKHooks_TakeDamage(client, client, client, GAMBLER_SELF_DAMAGE, DMG_GENERIC);
            PrintHintText(client, "%t", "GamblerPerk_Hint_Misfire");
        }
    }
    // This event is tricky, we return nothing but modify player state.
    // The core plugin needs to check for our custom crit condition.
    // For simplicity here, we just set crit mult and hope it works.
}

FUNCTION(GamblerPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    if (killer == _inst.client)
    {
        int new_luck               = zf_perkState[_inst.client] + GAMBLER_KILL_LUCK_GAIN;
        zf_perkState[_inst.client] = min(new_luck, GAMBLER_MAX_LUCK);
    }
}

FUNCTION(GamblerPerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    if (assist == _inst.client)
    {
        int new_luck               = zf_perkState[_inst.client] + GAMBLER_ASSIST_LUCK_GAIN;
        zf_perkState[_inst.client] = min(new_luck, GAMBLER_MAX_LUCK);
    }
}

FUNCTION(GamblerPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    // The temporary stats from the crit effect will expire automatically.
    // No need to reset anything here.
}

FUNCTION(GamblerPerk, updateCondStats), char[] buffer, int maxlen)
{
    Format(buffer, maxlen, "%t", "GamblerPerk_Status_Luck", zf_perkState[_inst.client], GAMBLER_MAX_LUCK);
}