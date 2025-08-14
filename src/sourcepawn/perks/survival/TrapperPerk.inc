#if defined __TrapperPerk_included
    #endinput
#endif
#define __TrapperPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_TRAPPER_MAX_ITEMS 5
#define ZF_TRAPPER_DAMAGE    200
#define ZF_TRAPPER_RADIUS    150
#define ZF_TRAPPER_RADIUSSQ  (200 * 200)
#define ZF_TRAPPER_TIMER     20

methodmap TrapperPerk < SurvivorBasePerk
{

public     TrapperPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        TrapperPerk      sm      = view_as<TrapperPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, TrapperPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, TrapperPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, TrapperPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, TrapperPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, TrapperPerkFonPeriodic);

        return sm;
    }
}

stock SurvivorBasePerk TrapperPerkFnew(int client)
{
    return new TrapperPerk(client);
}

FUNCTION(TrapperPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Trapper");
}

FUNCTION(TrapperPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "TrapperPerk_shortdesc");
}

FUNCTION(TrapperPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "TrapperPerk_desc");
}

FUNCTION_INT(TrapperPerk, getCategory))
{
    return 3;
}

FUNCTION(TrapperPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;

    for (int i = 0; i < ZF_TRAPPER_MAX_ITEMS; i++)
    {
        int mine = zf_item[client][i];
        if (mine > 0 && IsValidEntity(mine))
        {
            float mine_pos[3];
            GetEntPropVector(mine, Prop_Send, "m_vecOrigin", mine_pos);

            int  firstTarget       = -1;    // 用来记录第一个触发地雷的僵尸
            bool magneticTriggered = false;

            // 第一次循环：检查是否有僵尸触发了地雷
            for (int j = 1; j <= MaxClients; j++)
            {
                if (validLivingZom(j))
                {
                    float zom_pos[3];
                    GetClientAbsOrigin(j, zom_pos);
                    if (GetVectorDistance(mine_pos, zom_pos, true) <= ZF_TRAPPER_RADIUSSQ)
                    {
                        // 磁力僵尸优先触发
                        if (isMagnetic(j))
                        {
                            magneticTriggered = true;
                            break;
                        }
                        // 记录第一个普通触发者
                        if (firstTarget == -1)
                        {
                            firstTarget = j;
                        }
                    }
                }
            }

            // 处理触发结果
            if (magneticTriggered)
            {
                fxSpark(mine);    // 磁力触发
                removeItem(client, i);
                continue;    // 处理下一个地雷
            }

            if (firstTarget != -1)
            {
                // 普通爆炸触发
                // 此时我们不再需要循环，因为 applyDamageRadial 会处理范围内的所有敌人

                // applyDamageRadial 本身会创建爆炸特效，所以 doFx 设为 true
                // 你需要在 applyDamageRadial 内部让 env_explosion 实体也造成点燃
                applyDamageRadial(client, ZF_TRAPPER_DAMAGE, mine_pos, ZF_TRAPPER_RADIUS, true);

                // 移除地雷模型
                removeItem(client, i);
            }
        }
    }

    if (zf_perkTimer[client] > 0)
    {
        Format(buffer, maxlen, "%t", "TrapperPerk_Status_Cooldown", zf_perkTimer[client]);
    }
    else {
        if (getFreeItemIndex(client, ZF_TRAPPER_MAX_ITEMS) == -1)
        {
            Format(buffer, maxlen, "%t", "TrapperPerk_Status_Full");
        }
        else {
            Format(buffer, maxlen, "%t", "TrapperPerk_Status_Ready");
        }
    }
}

public bool TraceFilter_JustWorld(int entity, int contentsMask)
{
    // entity 0 is the world
    return entity == 0;
}

FUNCTION(TrapperPerk, onPeriodic))
{
    int client = _inst.client;
    if (zf_perkTimer[client] > 0)
    {
        zf_perkTimer[client]--;
    }
}

FUNCTION(TrapperPerk, onCallForMedic))
{
    int client = _inst.client;

    if (zf_perkTimer[client] > 0)
    {
        PrintHintText(client, "%t", "TrapperPerk_Hint_Cooldown", zf_perkTimer[client]);
        return;
    }

    if (!isGrounded(client))
    {
        PrintHintText(client, "%t", "TrapperPerk_Hint_NotGrounded");
        return;
    }
    if (!isCrouching(client))
    {
        PrintHintText(client, "%t", "TrapperPerk_Hint_NotCrouching");
        return;
    }

    int itemIndex = getFreeItemIndex(client, ZF_TRAPPER_MAX_ITEMS);
    if (itemIndex == -1)
    {
        // HUD 已经显示了 "(Mines Full)"，但一个即时提示更好
        PrintHintText(client, "%t", "TrapperPerk_Hint_Full");
        return;
    }

    float startPos[3];
    float ang[3];
    GetClientAbsOrigin(client, startPos);    // 从玩家脚底开始
    GetClientEyeAngles(client, ang);

    float fwd[3];
    GetAngleVectors(ang, fwd, NULL_VECTOR, NULL_VECTOR);

    // 在玩家前方一段距离处开始向下追踪
    startPos[0] += fwd[0] * 50.0;
    startPos[1] += fwd[1] * 50.0;
    startPos[2] += 20.0;    // 稍微抬高一点，以防玩家卡在斜坡里

    float endPos[3];
    endPos[0] = startPos[0];
    endPos[1] = startPos[1];
    endPos[2] = startPos[2];

    endPos[2] -= 100.0;    // 向下追踪100个单位

    // 创建一个只与世界（地面）碰撞的过滤器
    Handle trace = TR_TraceRayFilterEx(startPos, endPos, MASK_SOLID_BRUSHONLY, RayType_EndPoint, TraceFilter_JustWorld);

    float  finalPos[3];
    if (TR_DidHit(trace))
    {
        TR_GetEndPosition(finalPos, trace);    // 获取精确的地面位置
    }
    else {
        // 如果没有找到地面（比如玩家在悬崖边），可以给个提示或取消放置
        CloseHandle(trace);
        PrintHintText(client, "%t", "TrapperPerk_Hint_CannotPlace");
        return;
    }
    CloseHandle(trace);

    float mineAng[3] = { 0.0, 0.0, 0.0 };    // 让地雷平躺，并朝向玩家看的方向
    mineAng[1]       = ang[1];
    int entity       = fxCreateModelStatic(ZFMDL_MINE, client, true, false, finalPos, mineAng);
    if (entity != -1)
    {
        zf_item[client][itemIndex] = entity;
        zf_perkTimer[client]       = ZF_TRAPPER_TIMER;
        EmitSoundToClient(client, "vo/engineer_mvm_build_sentry_01.mp3");
    }
}

FUNCTION(TrapperPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int client = _inst.client;
    for (int i = 0; i < ZF_TRAPPER_MAX_ITEMS; i++)
    {
        if (validItem(client, i))
        {
            float mine_pos[3];
            GetEntPropVector(zf_item[client][i], Prop_Send, "m_vecOrigin", mine_pos);
            fxExplosionBig(zf_item[client][i]);
            removeItem(client, i);
        }
    }
}

stock bool isMagnetic(int client)
{
    if (!validClient(client) || g_hPerks[client] == null)
    {
        return false;
    }
    char perkName[32];
    g_hPerks[client].getName(perkName, sizeof(perkName));
    return StrEqual(perkName, "Magnetic");
}