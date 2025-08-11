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
#define ZF_COMBUSTIBLE_RADIUS 200
#define ZF_COMBUSTIBLE_RESPAWNTIME 4.5

methodmap CombustiblePerk < ZombieBasePerk {
    property bool hasExploded {
        public get() {
            this.Position = view_as<DataPackPos>(PERK_DATA_START_INDEX + 0);
            return this.ReadCell();
        }
        public set(bool value) {
            this.Position = view_as<DataPackPos>(PERK_DATA_START_INDEX + 0);
            this.WriteCell(value);
        }
    }

    public CombustiblePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CombustiblePerk sm = view_as<CombustiblePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CombustiblePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CombustiblePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CombustiblePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CombustiblePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, CombustiblePerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CombustiblePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, CombustiblePerkFonPlayerSpawn);

        sm.hasExploded = false;
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

FUNCTION(CombustiblePerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    if (victim != client) return;


    // 如果本条命已经爆炸过，则直接返回
    if (_inst.hasExploded) {
        return;
    }

    // 检查是否是来自幸存者的非近战攻击
    if (validSur(killer) && !(damagetype & DMG_CLUB)) {
        _inst.hasExploded = true;
        
        // 触发爆炸
        int explosionDamage = (TF2_GetPlayerClass(victim) == TFClass_Heavy) ? ZF_COMBUSTIBLE_DAMAGE_HEAVY : ZF_COMBUSTIBLE_DAMAGE;
        
        float pos[3];
        GetClientAbsOrigin(victim, pos); // 在玩家实体失效前获取其最后位置
        applyDamageRadial(victim, explosionDamage, pos, ZF_COMBUSTIBLE_RADIUS, true, GetClientTeam(victim));
        fxExplosionBig(victim);
        
        // 设置重生计时器
        CreateTimer(ZF_COMBUSTIBLE_RESPAWNTIME, Timer_RespawnClient, victim, TIMER_FLAG_NO_MAPCHANGE);
    } 
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
    _inst.hasExploded = false;
    int client = _inst.client;
    // 禁用间谍的PDA2槽（手表通常在这里）
    if (TF2_GetPlayerClass(client) == TFClass_Spy) {
        stripWeaponSlot(client, 3); // 2是PDA2槽位
    }
}

FUNCTION(CombustiblePerk, updateCondStats), char[] buffer, int maxlen) {
    // Visual hint for the player
    fxExplosionBig(_inst.client, false);
    Format(buffer, maxlen, "%t", "CombustiblePerk_Status_Ready");
}
