#if defined __GravityWarperPerk_included
#endinput
#endif
#define __GravityWarperPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_util_base.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_fx.inc"
#include <sdktools_functions>

#define GRAVITY_WARPER_RADIUS 350.0
#define GRAVITY_WARPER_MULTIPLIER 1.75 // 大于1.0的重力会降低跳跃高度
#define GRAVITY_WARPER_AURA_PARTICLE "particles/fluid_vortex_01.pcf"

#define GRAVITY_WARPER_DATA_AURA_FX (PERK_DATA_START_INDEX + 0)

methodmap GravityWarperPerk < ZombieBasePerk {
    public GravityWarperPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        GravityWarperPerk sm = view_as<GravityWarperPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, GravityWarperPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, GravityWarperPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, GravityWarperPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, GravityWarperPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, GravityWarperPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GravityWarperPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, GravityWarperPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, GravityWarperPerkFonRemove);
        
        sm.aura_fx = -1;
        return sm;
    }

    property int aura_fx {
        public get() {
            this.Position = view_as<DataPackPos>(GRAVITY_WARPER_DATA_AURA_FX);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(GRAVITY_WARPER_DATA_AURA_FX);
            this.WriteCell(value);
        }
    }
}

stock BasePerk GravityWarperPerkFnew(int client) {
    return new GravityWarperPerk(client);
}

FUNCTION(GravityWarperPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "GravityWarper");
}

FUNCTION(GravityWarperPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "GravityWarperPerk_shortdesc");
}

FUNCTION(GravityWarperPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "GravityWarperPerk_desc");
}

FUNCTION(GravityWarperPerk, onPlayerSpawn)) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
    }
    _inst.aura_fx = fxCreateParticle(GRAVITY_WARPER_AURA_PARTICLE, _inst.client, AttachBase, -1.0);
}

// 一个简化的重置函数，用于在职业失效时清理效果。
stock void ResetAllPlayersGravity() {
    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            SetEntityGravity(i, 1.0);
        }
    }
}

FUNCTION(GravityWarperPerk, onPeriodic)) {
    int client = _inst.client;
    float owner_pos[3];
    GetClientAbsOrigin(client, owner_pos);

    // 这个实现有一个已知的缺陷：如果一个幸存者离开了这个光环，
    // 他的重力不会被自动重置，直到这个僵尸死亡。
    // 一个更完善的实现需要复杂的状体管理来处理多个光环的叠加和离开。
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            float sur_pos[3];
            GetClientAbsOrigin(i, sur_pos);
            if (GetVectorDistance(owner_pos, sur_pos) <= GRAVITY_WARPER_RADIUS) {
                SetEntityGravity(i, GRAVITY_WARPER_MULTIPLIER);
            }
        }
    }
}

FUNCTION(GravityWarperPerk, onRemove)) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
    // 当拥有此技能的玩家移除它时，重置所有人的重力以确保安全。
    ResetAllPlayersGravity();
}

FUNCTION(GravityWarperPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
    // 当拥有此技能的玩家死亡时，重置所有人的重力。
    ResetAllPlayersGravity();
}

FUNCTION(GravityWarperPerk, updateCondStats), char[] buffer, int maxlen) {
    Format(buffer, maxlen, "Gravity Aura Active");
}