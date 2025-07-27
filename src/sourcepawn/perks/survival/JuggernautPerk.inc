
#if defined __JuggernautPerk_included
#endinput
#endif
#define __JuggernautPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sdkhooks.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sdktools_sound.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/vector.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_JUGGERNAUT_ATTACK 50
#define ZF_JUGGERNAUT_DEFEND -(50)
#define ZF_JUGGERNAUT_SPEED -(100)
#define ZF_JUGGERNAUT_FORCE 500.0
#define ZF_JUGGERNAUT_RADIUS 150.0
#define ZF_JUGGERNAUT_STUN_DURATION 1
#define ZF_JUGGERNAUT_STUN_SLOWDOWN -(100)

methodmap JuggernautPerk < SurvivorBasePerk {
    public JuggernautPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        JuggernautPerk sm = view_as<JuggernautPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, JuggernautPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, JuggernautPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, JuggernautPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, JuggernautPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, JuggernautPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, JuggernautPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk JuggernautPerkFnew(int client) {
    return new JuggernautPerk(client);
}


FUNCTION(JuggernautPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Juggernaut");
}

FUNCTION(JuggernautPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "High attack, low speed");
}

FUNCTION(JuggernautPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Your attack power is greatly increased, but your defense and movement speed are reduced.\nYou are immune to fall damage.\nIf you land near a zombie, the zombie will take slight damage, and be knocked back and stunned.");
}

FUNCTION(JuggernautPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_JUGGERNAUT_ATTACK);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_JUGGERNAUT_DEFEND);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_JUGGERNAUT_SPEED);
}

FUNCTION_FLOAT(JuggernautPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    // 如果是剑圣自己受到的摔落伤害
    if (victim == _inst.client && (damagetype & DMG_FALL))
    {
        // 直接将伤害清零，实现免疫
        return 0.0;
    }

    // 如果是剑圣自己打自己（比如火箭跳），也免疫伤害
    if (attacker == _inst.client && victim == _inst.client)
    {
        return 0.0;
    }
    
    return damage; // 其他情况，返回原始伤害
}

FUNCTION(JuggernautPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    int client = _inst.client;

    // --- 1. 处理地面冲击效果 ---
    // 条件：是剑圣自己受到了摔落伤害（即使伤害为0，这个事件依然会触发）
    if (victim == client && (damagetype & DMG_FALL))
    {
        float juggernautPos[3];
        GetClientAbsOrigin(client, juggernautPos);
        fxPuffBig(client); // 播放落地特效

        // 遍历所有玩家，寻找范围内的僵尸
        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingZom(i)) // 确保是存活的僵尸
            {
                float zombiePos[3];
                GetClientAbsOrigin(i, zombiePos);

                if (GetVectorDistance(juggernautPos, zombiePos) <= ZF_JUGGERNAUT_RADIUS)
                {
                    // a. 造成伤害
                    SDKHooks_TakeDamage(i, client, client, 1.0, DMG_CLUB); // 造成1点钝器伤害

                    // b. 施加减速
                    addStatTempExtend(i, ZFStatSpeed, ZF_JUGGERNAUT_STUN_SLOWDOWN, ZF_JUGGERNAUT_STUN_DURATION);

                    // c. 计算并施加击退 (正确的计算方式)
                    float knockback_dir[3];
                    SubtractVectors(zombiePos, juggernautPos, knockback_dir); // 正确方向：目标 - 源头
                    NormalizeVector(knockback_dir, knockback_dir);
                    
                    // 给予一个向上的力，让击退效果更明显
                    knockback_dir[2] += 0.5;
                    NormalizeVector(knockback_dir, knockback_dir);

                    ScaleVector(knockback_dir, ZF_JUGGERNAUT_FORCE);
                    
                    TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, knockback_dir);
                }
            }
        }
    }

    // --- 2. 处理近战攻击击退 ---
    // 条件：攻击者是剑圣，受害者是僵尸，伤害类型是近战
    if (attacker == client && validZom(victim) && (damagetype & DMG_CLUB))
    {
        float attackerPos[3];
        GetClientAbsOrigin(attacker, attackerPos);
        
        float victimPos[3];
        GetClientAbsOrigin(victim, victimPos);

        // 计算并施加击退
        float knockback_dir[3];
        SubtractVectors(victimPos, attackerPos, knockback_dir); // 正确方向：目标 - 源头
        NormalizeVector(knockback_dir, knockback_dir);

        // 同样给予一个向上的力
        knockback_dir[2] += 0.5;
        NormalizeVector(knockback_dir, knockback_dir);
        
        ScaleVector(knockback_dir, ZF_JUGGERNAUT_FORCE);
        
        TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, knockback_dir);
    }
}

