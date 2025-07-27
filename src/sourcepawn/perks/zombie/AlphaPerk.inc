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
#define ZF_ALPHA_ATTACK 5
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

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, AlphaPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, AlphaPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, AlphaPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AlphaPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AlphaPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AlphaPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, AlphaPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, AlphaPerkFonAssistKill);
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
    strcopy(buffer, maxlen, "召唤僵尸随从");
}

FUNCTION(AlphaPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "你能通过击杀幸存者或为随从助攻,使得死去的人类成为自己的随从。附近的每个僵尸和随从都能让你获得生命恢复和攻击加成。发医生语音可以召唤最多5个随从到身边,冷却时间15秒。“来自黑暗寒冬的仆人们、士兵们!听从我的召唤!”");
}


// 新增 onKill 方法
FUNCTION(AlphaPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    // 如果受害者是幸存者
    if (validClient(victim))
    {
        // 将 AlphaPerk 的拥有者设置为这个受害者的主人
        zf_perkAlphaMaster[victim] = _inst.client;
        PrintHintText(_inst.client, "%N 将成为你的随从。", victim); // 给 AlphaPerk 玩家一个提示
    }
}

// 新增 onAssistKill 方法
FUNCTION(AlphaPerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    // 逻辑和 onKill 完全一样
    if (validClient(victim))
    {
        zf_perkAlphaMaster[victim] = _inst.client;
        PrintHintText(_inst.client, "%N 将成为你的随从。", victim);
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
    ZF_LogDebug("[DEBUG] Alpha: onCallForMedic triggered. Current perk_timer: %d", _inst.perk_timer);

    if (_inst.perk_timer > 0) {
        ZF_LogDebug("[DEBUG] Alpha: Cooldown active, returning.");
        return;
    }

    int minionCount = countMinions(client);
    ZF_LogDebug("[DEBUG] Alpha: Found %d minions.", minionCount);

    if (minionCount == 0) {
        ZF_LogDebug("[DEBUG] Alpha: No minions, printing hint and returning.");
        PrintHintText(client, "你没有任何随从可以召唤！");
        return;
    }

    ZF_LogDebug("[DEBUG] Alpha: Summoning minions and setting timer.");
    _inst.perk_timer = ZF_ALPHA_TIMER_MINION;
    doAlphaSummon(client);

    ZF_LogDebug("[DEBUG] Alpha: Timer set to %d.", _inst.perk_timer);
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
        PrintHintText(validMinions[i], "你的主人召唤了你!");
        fxTeleportTrail(validMinions[i], 5.0);
        ++summonCount;
    }

    if(summonCount > 0)
        PrintHintText(client, "你召唤了 %d 个随从!", summonCount);
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
        Format(buffer, maxlen, "Summon Cooldown: %ds", _inst.perk_timer);
    } else {
        Format(buffer, maxlen, "Summon Ready");
    }
}

FUNCTION(AlphaPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        // --- 调试日志 ---
        // 为了防止刷屏，可以加个条件
        if (_inst.perk_timer == ZF_ALPHA_TIMER_MINION || _inst.perk_timer < 5) {
            ZF_LogDebug("[DEBUG] Alpha: onPeriodic ticking down timer. Now: %d", _inst.perk_timer - 1);
        }
        _inst.perk_timer--;
    }
}