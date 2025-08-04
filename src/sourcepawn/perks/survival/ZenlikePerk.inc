#if defined __ZenlikePerk_included
    #endinput
#endif
#define __ZenlikePerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_ZENLIKE_CRIT_INC 3
#define ZF_ZENLIKE_CRIT_DEC 25
#define ZF_ZENLIKE_HEAL     1
#define ZF_ZENLIKE_REGEN    1

methodmap ZenlikePerk < SurvivorBasePerk
{

public     ZenlikePerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        ZenlikePerk      sm      = view_as<ZenlikePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ZenlikePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ZenlikePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ZenlikePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, ZenlikePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALC_IS_ATTACK_CRITICAL, ZenlikePerkFonCalcIsAttackCritical);

        return sm;
    }
}

stock SurvivorBasePerk ZenlikePerkFnew(int client)
{
    return new ZenlikePerk(client);
}

FUNCTION(ZenlikePerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Zenlike");
}

FUNCTION(ZenlikePerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ZenlikePerk_shortdesc");
}

FUNCTION(ZenlikePerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "ZenlikePerk_desc");
}

FUNCTION(ZenlikePerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;

    if (isGrounded(client) && isCrouching(client) && isNotMoving(client))
    {
        zf_perkState[client] = min(100, zf_perkState[client] + ZF_ZENLIKE_CRIT_INC);
        if (!getCond(client, ZFCondPoisoned))
        {
            addHealth(client, ZF_ZENLIKE_REGEN);
        }
        // Assuming a function to heal poison exists, like `healPoison(client, ZF_ZENLIKE_HEAL);`
    }

    addStat(client, ZFStatCrit, ZFStatTypeCond, zf_perkState[client]);

    if (zf_perkState[client] > 0)
    {
        Format(buffer, maxlen, "%t", "ZenlikePerk_Status_Crit", zf_perkState[client]);
    }
    else {
        strcopy(buffer, maxlen, "");
    }
}

FUNCTION(ZenlikePerk, onCalcIsAttackCritical))
{
    CreateTimer(0.1, tZenlikeAttack, _inst.client);
}

public Action tZenlikeAttack(Handle timer, any client)
{
    if (validLivingSur(client))
    {
        zf_perkState[client] = max(0, zf_perkState[client] - ZF_ZENLIKE_CRIT_DEC);
    }
    return Plugin_Stop;
}