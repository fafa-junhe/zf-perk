#if defined __MountaineerPerk_included
#endinput
#endif
#define __MountaineerPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdktools_trace>

#define MOUNTAINEER_CLIMB_SPEED 200.0
#define MOUNTAINEER_GRAPPLE_COOLDOWN 12
#define MOUNTAINEER_GRAPPLE_FORCE 1000.0

#define MOUNTAINEER_DATA_TIMER (PERK_DATA_START_INDEX + 0)
#define MOUNTAINEER_DATA_IS_CLIMBING (PERK_DATA_START_INDEX + 1)

methodmap MountaineerPerk < SurvivorBasePerk {
    public MountaineerPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        MountaineerPerk sm = view_as<MountaineerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, MountaineerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, MountaineerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, MountaineerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, MountaineerPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_RUN_CMD, MountaineerPerkFonPlayerRunCmd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, MountaineerPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, MountaineerPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, MountaineerPerkFupdateCondStats);

        sm.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_TIMER);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_IS_CLIMBING);
        sm.WriteCell(0);

        return sm;
    }

    property int perk_timer {
        public get() {
            this.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_TIMER);
            this.WriteCell(value);
        }
    }

    property bool is_climbing {
        public get() {
            this.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_IS_CLIMBING);
            return this.ReadCell() != 0;
        }
        public set(bool value) {
            this.Position = view_as<DataPackPos>(MOUNTAINEER_DATA_IS_CLIMBING);
            this.WriteCell(value ? 1 : 0);
        }
    }
}

stock SurvivorBasePerk MountaineerPerkFnew(int client) {
    return new MountaineerPerk(client);
}

FUNCTION(MountaineerPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Mountaineer");
}

FUNCTION(MountaineerPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MountaineerPerk_shortdesc");
}

FUNCTION(MountaineerPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MountaineerPerk_desc");
}

FUNCTION_FLOAT(MountaineerPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (victim == _inst.client && (damagetype & DMG_FALL)) {
        return 0.0;
    }
    return damage;
}

FUNCTION(MountaineerPerk, onPlayerRunCmd), int buttons, int impulse, float vel[3], float angles[3], int weapon) {
    int client = _inst.client;
    if (!(buttons & IN_JUMP)) {
        _inst.is_climbing = false;
        return;
    }

    // Wall climbing logic
    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    Handle trace = TR_TraceRayFilterEx(eye_pos, eye_ang, MASK_SOLID, RayType_Infinite, TraceFilterPlayers);
    if (TR_DidHit(trace)) {
        float end_pos[3];
        TR_GetEndPosition(end_pos, trace);
        float dist = GetVectorDistance(eye_pos, end_pos);
        if (dist < 80.0) {
            float normal[3];
            float forwarded[3];
            TR_GetPlaneNormal(trace, normal);
            GetAngleVectors(eye_ang, forwarded, NULL_VECTOR, NULL_VECTOR);
            // Check if player is facing the wall
            if (GetVectorDotProduct(forwarded, normal) < -0.5) {
                float climb_vel[3] = {0.0, 0.0, MOUNTAINEER_CLIMB_SPEED};
                TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, climb_vel);
                _inst.is_climbing = true;
            } else {
                _inst.is_climbing = false;
            }
        } else {
            _inst.is_climbing = false;
        }
    } else {
        _inst.is_climbing = false;
    }
    CloseHandle(trace);
}

FUNCTION(MountaineerPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.perk_timer > 0) {
        PrintHintText(client, "抓钩正在冷却中: %d秒", _inst.perk_timer);
        return;
    }

    float eye_pos[3], eye_ang[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, eye_ang);

    Handle trace = TR_TraceRayFilterEx(eye_pos, eye_ang, MASK_SOLID, RayType_Infinite, TraceFilterPlayers);
    if (TR_DidHit(trace)) {
        float end_pos[3];
        TR_GetEndPosition(end_pos, trace);
        
        float client_pos[3];
        GetClientAbsOrigin(client, client_pos);
        
        float direction[3];
        SubtractVectors(end_pos, client_pos, direction);
        NormalizeVector(direction, direction);
        ScaleVector(direction, MOUNTAINEER_GRAPPLE_FORCE);
        
        TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, direction);
        
        fxCreateSoundToClient("weapons/grapple_impact.wav", client);
        
        _inst.perk_timer = MOUNTAINEER_GRAPPLE_COOLDOWN;
    } else {
        PrintHintText(client, "抓钩没有命中任何东西!");
    }
    CloseHandle(trace);
}

FUNCTION(MountaineerPerk, onPeriodic)) {
    if (_inst.perk_timer > 0) {
        _inst.perk_timer--;
        if (_inst.perk_timer == 0) {
            PrintHintText(_inst.client, "抓钩已准备就绪!");
        }
    }
}

FUNCTION(MountaineerPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.is_climbing) {
        strcopy(buffer, maxlen, "正在爬墙");
    } else if (_inst.perk_timer > 0) {
        Format(buffer, maxlen, "抓钩冷却: %ds", _inst.perk_timer);
    } else {
        strcopy(buffer, maxlen, "抓钩已就绪");
    }
}

bool TraceFilterPlayers(int entity, int contentsMask, any data) {
    return entity > MaxClients;
}