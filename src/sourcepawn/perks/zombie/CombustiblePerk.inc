#if defined __CombustiblePerk_included
#endinput
#endif
#define __CombustiblePerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_COMBUSTIBLE_DAMAGE 120
#define ZF_COMBUSTIBLE_DAMAGE_HEAVY 200
#define ZF_COMBUSTIBLE_DEFEND -200
#define ZF_COMBUSTIBLE_RADIUS 300
#define ZF_COMBUSTIBLE_RESPAWNTIME 4.5

methodmap CombustiblePerk < ZombieBasePerk {
    public CombustiblePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CombustiblePerk sm = view_as<CombustiblePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CombustiblePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CombustiblePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CombustiblePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CombustiblePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, CombustiblePerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CombustiblePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, CombustiblePerkFonPlayerSpawn);

        return sm;
    }
}

stock BasePerk CombustiblePerkFnew(int client) {
    return new CombustiblePerk(client);
}

FUNCTION(CombustiblePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Combustible");
}

FUNCTION(CombustiblePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CombustiblePerk_shortdesc");
}

FUNCTION(CombustiblePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CombustiblePerk_desc");
}

FUNCTION(CombustiblePerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_COMBUSTIBLE_DEFEND);
}

FUNCTION_FLOAT(CombustiblePerk, onTakeDamage), int victim, int attacker,  int inflictor, float damage, int damagetype) {
    int client = _inst.client;
    if (victim != client) return damage;

    // 检查是否是来自幸存者的非近战致命一击
    if (isSur(attacker) && !(damagetype & DMG_CLUB)) {
        if (damage >= GetClientHealth(client)) {
            // 是致命一击，触发爆炸
            int explosionDamage = (TF2_GetPlayerClass(victim) == TFClass_Heavy) ? ZF_COMBUSTIBLE_DAMAGE_HEAVY : ZF_COMBUSTIBLE_DAMAGE;
            
            // 此刻 victim 仍然有效，可以作为 attacker
            applyDamageRadialAtClient(victim, explosionDamage, ZF_COMBUSTIBLE_RADIUS, true);
            fxExplosionBig(victim);
            
            // 设置重生计时器
            CreateTimer(ZF_COMBUSTIBLE_RESPAWNTIME, Timer_RespawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
        }
    }
    return damage; // 返回原始伤害，让玩家正常死亡
}

public Action Timer_RespawnClient(Handle timer, any client) {
    if (client > 0 && IsClientInGame(client) && !IsPlayerAlive(client)) {
        TF2_RespawnPlayer(client);
    }
    return Plugin_Stop;
}

// 实现 onPlayerSpawn
FUNCTION(CombustiblePerk, onPlayerSpawn))
{
    int client = _inst.client;
    // 禁用间谍的PDA2槽（手表通常在这里）
    if (TF2_GetPlayerClass(client) == TFClass_Spy) {
        stripWeaponSlot(client, 3); // 2是PDA2槽位
    }
    // 禁用侦察兵的副武器槽（Bonk是副武器）
    if (TF2_GetPlayerClass(client) == TFClass_Scout) {
        // 你需要找到 Bonk 的武器名或定义，然后针对性禁用
        // 一个更通用的方法是检查玩家的所有武器
        for (int i = 0; i < 3; i++) { // 遍历主、副、近战槽
            int weapon = GetPlayerWeaponSlot(client, i);
            if (IsValidEntity(weapon)) {
                char weapon_name[64];
                GetEdictClassname(weapon, weapon_name, sizeof(weapon_name));
                if (StrEqual(weapon_name, "tf_weapon_lunchbox_drink")) {
                    stripWeaponSlot(client, i);
                    break;
                }
            }
        }
    }
}

FUNCTION(CombustiblePerk, updateCondStats), char[] buffer, int maxlen) {
    // Visual hint for the player
    fxExplosionTiny(_inst.client);
    strcopy(buffer, maxlen, "Ready to explode on death");
}
