#if defined __CarpenterPerk_included
    #endinput
#endif
#define __CarpenterPerk_included

#include "../../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#include <sdkhooks>
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#define CARPENTER_ATTACK               -40
#define CARPENTER_DEFEND               25
#define CARPENTER_BARRICADE_HEALTH     350
#define CARPENTER_COOLDOWN             13
#define CARPENTER_MAX_ITEMS            5
#define CARPENTER_DROP_RADSQ_BARRICADE (250 * 250)
#define CARPENTER_DROP_RADSQ_CLIENT    (150 * 150)

#define CARPENTER_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap CarpenterPerk < SurvivorBasePerk
{
    public CarpenterPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        CarpenterPerk    sm      = view_as<CarpenterPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, CarpenterPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CarpenterPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CarpenterPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, CarpenterPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, CarpenterPerkFonPeriodic);

        sm.Position = view_as<DataPackPos>(CARPENTER_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer
    {
        public get()
        {
            this.Position = view_as<DataPackPos>(CARPENTER_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value)
        {
            this.Position = view_as<DataPackPos>(CARPENTER_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk CarpenterPerkFnew(int client)
{
    return new CarpenterPerk(client);
}

FUNCTION(CarpenterPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Carpenter");
}

FUNCTION(CarpenterPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "CarpenterPerk_shortdesc");
}

FUNCTION(CarpenterPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "CarpenterPerk_desc");
}

FUNCTION(CarpenterPerk, updateClientPermStats))
{
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, CARPENTER_ATTACK);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, CARPENTER_DEFEND);
}

FUNCTION(CarpenterPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;

    if (_inst.perk_timer > 0)
    {
        Format(buffer, maxlen, "%t", "CarpenterPerk_Status_Cooldown", _inst.perk_timer);
    }
    else {
        if (getFreeItemIndex(client, CARPENTER_MAX_ITEMS) == -1)
        {
            Format(buffer, maxlen, "%t", "CarpenterPerk_Status_Full");
        }
        else {
            Format(buffer, maxlen, "%t", "CarpenterPerk_Status_Ready");
        }
    }
}
stock bool isCarpenter(int client)
{
    if (!validClient(client) || g_hPerks[client] == null)
    {
        return false;
    }
    char perkName[32];
    g_hPerks[client].getName(perkName, sizeof(perkName));
    return StrEqual(perkName, "Carpenter");
}

FUNCTION(CarpenterPerk, onPeriodic))
{
    if (_inst.perk_timer > 0)
    {
        _inst.perk_timer--;
    }
}

FUNCTION_INT(CarpenterPerk, getCategory))
{
    return 3;
}

// vvvvvvvvvvvvvvvvvv MODIFIED LOGIC BELOW vvvvvvvvvvvvvvvvvv
FUNCTION(CarpenterPerk, onCallForMedic))
{
    int client = _inst.client;

    if (_inst.perk_timer > 0)
        return;

    if (!isGrounded(client))
    {
        PrintHintText(client, "%t", "CarpenterPerk_Hint_NotGrounded");
        return;
    }

    if (!isCrouching(client))
    {
        PrintHintText(client, "%t", "CarpenterPerk_Hint_NotCrouching");
        return;
    }

    bool  clientsNearby    = false;
    bool  barricadesNearby = false;
    float clientPos[3], otherPos[3];

    GetClientAbsOrigin(client, clientPos);

    for (int i = 1; i <= MaxClients; i++)
    {
        if (!IsClientInGame(i) || !IsPlayerAlive(i)) continue;

        // 检查障碍物
        if (isSur(i) && isCarpenter(i))
        {
            for (int j = 0; j < CARPENTER_MAX_ITEMS; j++)
            {
                if (validItem(i, j))
                {
                    if (GetVectorDistance(clientPos, zf_perkPos[i][j], true) <= CARPENTER_DROP_RADSQ_BARRICADE)
                    {
                        barricadesNearby = true;
                        break;
                    }
                }
            }
            if (barricadesNearby) break;
        }

        // 检查玩家
        if (i == client) continue;
        GetClientAbsOrigin(i, otherPos);
        if (GetVectorDistance(clientPos, otherPos, true) <= CARPENTER_DROP_RADSQ_CLIENT)
        {
            clientsNearby = true;
            break;
        }
    }

    if (clientsNearby)
    {
        PrintHintText(client, "%t", "CarpenterPerk_Hint_PlayersNearby");
        return;
    }

    // if (barricadesNearby)
    // {
    //     PrintHintText(client, "%t", "CarpenterPerk_Hint_BarricadesNearby");
    //     return;
    // }

    int itemIndex = getFreeItemIndex(client, CARPENTER_MAX_ITEMS);
    if (itemIndex < 0)
        return;

    _inst.perk_timer = CARPENTER_COOLDOWN;

    removeItem(client, itemIndex);

    // 生成障碍物并记录位置
    float physPos[3];
    zf_item[client][itemIndex]    = doCarpenterBuild(client, physPos);
    zf_perkPos[client][itemIndex] = physPos;

    setItemMetadata(zf_item[client][itemIndex], CARPENTER_BARRICADE_HEALTH);
    SDKHook(zf_item[client][itemIndex], SDKHook_OnTakeDamage, perk_OnFenceTakeDamage);
}
stock doCarpenterBuild(int client, float physPos[3])
{
    float pos[3];
    float ang[3];
    float vecFwd[3];
    float vecRgt[3];
    GetClientAbsOrigin(client, pos);
    GetClientAbsAngles(client, ang);

    // Move item in front of client.
    GetAngleVectors(ang, vecFwd, vecRgt, NULL_VECTOR);
    ScaleVector(vecFwd, 45.0);
    ScaleVector(vecRgt, -30.0);      // Adjust for ZFMDL_FENCE
    AddVectors(pos, vecFwd, pos);    // Adjust for ZFMDL_FENCE
    AddVectors(pos, vecRgt, pos);    // Adjust for ZFMDL_FENCE
    pos[2] += 120.0;
    ang[0] += GetRandomFloat(-4.0, 4.0);     // Adjust for ZFMDL_FENCE
    ang[1] += GetRandomFloat(85.0, 95.0);    // Adjust for ZFMDL_FENCE

    // Calculate physical position (used for distance calculations)
    physPos[0] = pos[0];
    physPos[1] = pos[1];
    physPos[2] = pos[2] - 120.0;
    SubtractVectors(physPos, vecRgt, physPos);

    // Barricade build effects
    float off[3] = { 0.0, ... };
    AddVectors(off, vecFwd, off);
    fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);

    ScaleVector(vecRgt, 1.3);
    AddVectors(vecFwd, vecRgt, off);
    fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);
    SubtractVectors(vecFwd, vecRgt, off);
    fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);

    ScaleVector(vecRgt, 1.3);
    AddVectors(vecFwd, vecRgt, off);
    fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);
    SubtractVectors(vecFwd, vecRgt, off);
    fxCreateParticle(ZFPART_PUFFBIG, client, AttachNone, 4.0, off);

    fxCreateSoundToAll(ZFSND_WOOD_HIT[GetRandomInt(0, 1)], client);

    // Build solid item in front of client.
    return fxCreateModelStatic(ZFMDL_FENCE, client, false, true, pos, ang);
}