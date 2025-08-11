#if defined __HolyPerk_included
    #endinput
#endif
#define __HolyPerk_included

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

#define ZF_HOLY_DEFENCE  20
#define ZF_HOLY_RADIUSSQ (400 * 400)
#define ZF_HOLY_REGEN    20

methodmap HolyPerk < SurvivorBasePerk
{

public     HolyPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        HolyPerk         sm      = view_as<HolyPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, HolyPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, HolyPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, HolyPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, HolyPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, HolyPerkFupdateCondStats);

        return sm;
    }
}

stock SurvivorBasePerk HolyPerkFnew(int client)
{
    return new HolyPerk(client);
}

FUNCTION(HolyPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Holy");
}

FUNCTION(HolyPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "HolyPerk_shortdesc");
}

FUNCTION(HolyPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "HolyPerk_desc");
}

FUNCTION(HolyPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_HOLY_DEFENCE);
    createAura(_inst.client, ZFPART_AURAGLOWBEAMS, AttachBase);
}

FUNCTION(HolyPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;

    if (isGrounded(client) && isCrouching(client) && isNotMoving(client))
    {
        float client_pos[3];
        GetClientAbsOrigin(client, client_pos);

        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingSur(i))
            {
                float target_pos[3];
                GetClientAbsOrigin(i, target_pos);

                if (GetVectorDistance(client_pos, target_pos, true) <= ZF_HOLY_RADIUSSQ)
                {
                    addHealth(i, ZF_HOLY_REGEN);
                }
            }
        }
        Format(buffer, maxlen, "%t", "HolyPerk_Status_Healing");
    }
    else {
        strcopy(buffer, maxlen, "");
    }
}