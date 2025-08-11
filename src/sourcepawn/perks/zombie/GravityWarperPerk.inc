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

#define GRAVITY_WARPER_RADIUS 600.0
#define GRAVITY_WARPER_MULTIPLIER 1.75 // 大于1.0的重力会降低跳跃高度
#define GRAVITY_WARPER_DEFEND 25
#define GRAVITY_WARPER_AURA_PARTICLE "teleporter_blue_exit_level3"

#define GRAVITY_WARPER_DATA_AURA_FX (PERK_DATA_START_INDEX + 0)
#define GRAVITY_WARPER_DATA_AFFECTED_SURVIVORS (PERK_DATA_START_INDEX + 1)

methodmap GravityWarperPerk < ZombieBasePerk {
    public GravityWarperPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        GravityWarperPerk sm = view_as<GravityWarperPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, GravityWarperPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, GravityWarperPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, GravityWarperPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, GravityWarperPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, GravityWarperPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, GravityWarperPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GravityWarperPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, GravityWarperPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, GravityWarperPerkFonRemove);
        
        sm.aura_fx = -1;
        sm.affected_survivors = new ArrayList();
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

    property ArrayList affected_survivors {
        public get() {
            this.Position = view_as<DataPackPos>(GRAVITY_WARPER_DATA_AFFECTED_SURVIVORS);
            return view_as<ArrayList>(this.ReadCell());
        }
        public set(ArrayList value) {
            this.Position = view_as<DataPackPos>(GRAVITY_WARPER_DATA_AFFECTED_SURVIVORS);
            this.WriteCell(view_as<int>(value));
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
FUNCTION(GravityWarperPerk, onPeriodic)) {
    int client = _inst.client;
    float owner_pos[3];
    GetClientAbsOrigin(client, owner_pos);

    ArrayList current_survivors = new ArrayList();

    // 检查所有幸存者
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            float sur_pos[3];
            GetClientAbsOrigin(i, sur_pos);

            if (GetVectorDistance(owner_pos, sur_pos) <= GRAVITY_WARPER_RADIUS) {
                // 幸存者在光环内
                current_survivors.Push(i);
                if (_inst.affected_survivors.FindValue(i) == -1) {
                    // 新进入光环的幸存者
                    SetEntityGravity(i, GRAVITY_WARPER_MULTIPLIER);
                }
            }
        }
    }

    // 检查离开光环的幸存者
    for (int i = 0; i < _inst.affected_survivors.Length; i++) {
        int survivor = _inst.affected_survivors.Get(i);
        if (current_survivors.FindValue(survivor) == -1) {
            // 幸存者离开了光环，重置重力
            if (IsClientInGame(survivor)) {
                SetEntityGravity(survivor, 1.0);
            }
        }
    }

    // 更新受影响的幸存者列表
    delete _inst.affected_survivors;
    _inst.affected_survivors = current_survivors;
}

FUNCTION(GravityWarperPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, GRAVITY_WARPER_DEFEND);
}

FUNCTION(GravityWarperPerk, onRemove)) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
    // 重置所有受影响幸存者的重力
    for (int i = 0; i < _inst.affected_survivors.Length; i++) {
        int survivor = _inst.affected_survivors.Get(i);
        if (IsClientInGame(survivor)) {
            SetEntityGravity(survivor, 1.0);
        }
    }
    _inst.affected_survivors.Clear();
    delete _inst.affected_survivors;
}

FUNCTION(GravityWarperPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (_inst.aura_fx != -1 && IsValidEntity(_inst.aura_fx)) {
        AcceptEntityInput(_inst.aura_fx, "Kill");
        _inst.aura_fx = -1;
    }
    // 重置所有受影响幸存者的重力
    for (int i = 0; i < _inst.affected_survivors.Length; i++) {
        int survivor = _inst.affected_survivors.Get(i);
        if (IsClientInGame(survivor)) {
            SetEntityGravity(survivor, 1.0);
        }
    }
    _inst.affected_survivors.Clear();
}

FUNCTION(GravityWarperPerk, updateCondStats), char[] buffer, int maxlen) {
    Format(buffer, maxlen, "%t", "GravityWarperPerk_Status_Active");
}