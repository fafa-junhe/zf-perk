#if defined __AlphaPerk_included
#endinput
#endif
#define __AlphaPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/vector.inc"
#include "../../../../include/sorting.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_ALPHA_RADIUSSQ (500.0 * 500.0)
#define ZF_ALPHA_ATTACK 10
#define ZF_ALPHA_PERM_ATTACK 15
#define ZF_ALPHA_ATTACK_MINION 10
#define ZF_ALPHA_REGEN 4
#define ZF_ALPHA_REGEN_MINION 12
#define ZF_ALPHA_SUMMON_LIMIT 5
#define ZF_ALPHA_TIMER_MINION 15

// Data slots
#define ALPHA_DATA_TIMER (PERK_DATA_START_INDEX + 0)

methodmap AlphaPerk < ZombieBasePerk {
    public AlphaPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        AlphaPerk sm = view_as<AlphaPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, AlphaPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AlphaPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AlphaPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AlphaPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, AlphaPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, AlphaPerkFonAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, AlphaPerkFupdateClientPermStats);
        // Initialize data
        sm.Position = view_as<DataPackPos>(ALPHA_DATA_TIMER);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(ALPHA_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(ALPHA_DATA_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk AlphaPerkFnew(int client) {
    return new AlphaPerk(client);
}

FUNCTION(AlphaPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Alpha");
}

FUNCTION(AlphaPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AlphaPerk_shortdesc");
}

FUNCTION(AlphaPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AlphaPerk_desc");
}

FUNCTION_INT(AlphaPerk, getCategory))
{
    return 3;
}

// 新增 onKill 方法
FUNCTION(AlphaPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    // 如果受害者是幸存者
    if (validClient(victim) && victim != killer)
    {
        // 将 AlphaPerk 的拥有者设置为这个受害者的主人
        zf_perkAlphaMaster[victim] = _inst.client;
        PrintHintText(_inst.client, "%t", "AlphaPerk_Hint_NewMinion", victim);
    }
}

// 新增 onAssistKill 方法
FUNCTION(AlphaPerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    // 逻辑和 onKill 完全一样
    if (validClient(victim) && victim != assist)
    {
        zf_perkAlphaMaster[victim] = _inst.client;
        PrintHintText(_inst.client, "%t", "AlphaPerk_Hint_NewMinion", victim);
    }
}
// 辅助函数，用于计算随从数量
stock int countMinions(int master) {
    int count = 0;
    for(int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i) && IsPlayerAlive(i) && isZom(i) && zf_perkAlphaMaster[i] == master) {
            count++;
        }
    }
    return count;
}
FUNCTION(AlphaPerk, onCallForMedic)) {
    int client = _inst.client;

    // --- 调试日志 ---
  

    if (_inst.perk_timer > 0) {
      
        return;
    }

    int minionCount = countMinions(client);
  

    if (minionCount == 0) {
      
        PrintHintText(client, "%t", "AlphaPerk_Hint_NoMinions");
        return;
    }

  
    _inst.perk_timer = ZF_ALPHA_TIMER_MINION;
    doAlphaSummon(client);

  
}
FUNCTION(AlphaPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_ALPHA_PERM_ATTACK);
}
stock void doAlphaSummon(int client)
{
    int validMinions[MAXPLAYERS+1];
    int validMinionCount = 0;
    float clientPos[3];
    float clientAng[3];
    float summonVel[3];

    GetClientAbsOrigin(client, clientPos);
    GetClientAbsAngles(client, clientAng);

    for(int i = 1; i <= MaxClients; i++)
    {
        if(IsClientInGame(i) && IsPlayerAlive(i) && isZom(i) && zf_perkAlphaMaster[i] == client)
        {
            validMinions[validMinionCount++] = i;
        }
    }

    if (validMinionCount == 0) return;

    SortIntegers(validMinions, validMinionCount, Sort_Random);
    int summonCount = 0;
    for(int i = 0; i < validMinionCount; i++)
    {
        if(i >= ZF_ALPHA_SUMMON_LIMIT) break;

        summonVel[0] = GetRandomFloat(-100.0, 100.0);
        summonVel[1] = GetRandomFloat(-100.0, 100.0);
        summonVel[2] = GetRandomFloat(300.0, 400.0);
        TeleportEntity(validMinions[i], clientPos, clientAng, summonVel);
        ClientCommand(validMinions[i], "voicemenu 1 0");
        PrintHintText(validMinions[i], "%t", "AlphaPerk_Hint_Summoned");
        fxTeleportTrail(validMinions[i], 5.0);
        ++summonCount;
    }

    if(summonCount > 0)
        PrintHintText(client, "%t", "AlphaPerk_Hint_SummonCount", summonCount);
}

FUNCTION(AlphaPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;
    float client_pos[3];
    GetClientAbsOrigin(client, client_pos);

    int attack_bonus = 0;
    int regen_bonus = 0;

    for (int i = 1; i <= MaxClients; i++) {
        if (i == client || !validLivingZom(i)) {
            continue;
        }

        float target_pos[3];
        GetClientAbsOrigin(i, target_pos);

        if (GetVectorDistance(client_pos, target_pos, true) <= ZF_ALPHA_RADIUSSQ) {
            if (zf_perkAlphaMaster[i] == client) {
                attack_bonus += ZF_ALPHA_ATTACK_MINION;
                regen_bonus += ZF_ALPHA_REGEN_MINION;
            } else {
                attack_bonus += ZF_ALPHA_ATTACK;
                regen_bonus += ZF_ALPHA_REGEN;
            }
        }
    }

    if (attack_bonus > 0) {
        addStat(client, ZFStatAtt, ZFStatTypeCond, attack_bonus);
    }
    if (regen_bonus > 0) {
        addHealth(client, regen_bonus);
    }

    if (_inst.perk_timer > 0) {
            Format(buffer, maxlen, "%t", "AlphaPerk_Status_Cooldown", _inst.perk_timer);
    } else {
            Format(buffer, maxlen, "%t", "AlphaPerk_Status_Ready");
    }
}

FUNCTION(AlphaPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        // --- 调试日志 ---
        // 为了防止刷屏，可以加个条件
        if (_inst.perk_timer == ZF_ALPHA_TIMER_MINION || _inst.perk_timer < 5) {
          
        }
        _inst.perk_timer--;
    }
}