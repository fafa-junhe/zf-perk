#if defined __GeomancerPerk_included
#endinput
#endif
#define __GeomancerPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_util_base.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_fx.inc"
#include <sdkhooks>
#include <vector>

#define GEOMANCER_SPEED -20
#define GEOMANCER_COOLDOWN 25.0
#define GEOMANCER_WALL_HEALTH 400
#define GEOMANCER_WALL_COUNT 5
#define GEOMANCER_WALL_DISTANCE 80.0
#define GEOMANCER_WALL_MODEL "models/props_mining/rock001.mdl"

#define GEOMANCER_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define GEOMANCER_DATA_WALLS_HANDLE (PERK_DATA_START_INDEX + 1)

methodmap GeomancerPerk < ZombieBasePerk {
    public GeomancerPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        GeomancerPerk sm = view_as<GeomancerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, GeomancerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, GeomancerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, GeomancerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, GeomancerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, GeomancerPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, GeomancerPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GeomancerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, GeomancerPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, GeomancerPerkFonRemove);

        sm.cooldown = 0.0;
        sm.walls = new DataPack();
        return sm;
    }

    property float cooldown {
        public get() {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_COOLDOWN);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }

    property DataPack walls {
        public get() {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_WALLS_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_WALLS_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }
}

stock BasePerk GeomancerPerkFnew(int client) {
    return new GeomancerPerk(client);
}

FUNCTION(GeomancerPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Geomancer");
}

FUNCTION(GeomancerPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "GeomancerPerk_shortdesc");
}

FUNCTION(GeomancerPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "GeomancerPerk_desc");
}

FUNCTION(GeomancerPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, GEOMANCER_SPEED);
}

FUNCTION(GeomancerPerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.cooldown > 0.0) {
        PrintHintText(client, "石墙正在冷却中: %.0fs", _inst.cooldown);
        return;
    }

    _inst.cooldown = GEOMANCER_COOLDOWN;

    float pos[3], ang[3], fwd[3];
    GetClientEyePosition(client, pos);
    GetClientEyeAngles(client, ang);
    GetAngleVectors(ang, fwd, NULL_VECTOR, NULL_VECTOR);

    for (int i = 0; i < GEOMANCER_WALL_COUNT; i++) {
        float wall_pos[3];
        ScaleVector(fwd, GEOMANCER_WALL_DISTANCE * (i + 1));
        AddVectors(pos, fwd, wall_pos);

        // Trace down to find the ground
        float down[3] = {0.0, 0.0, -1.0};
        TR_TraceRayFilter(wall_pos, down, MASK_SOLID, RayType_Infinite, TraceRayToGround, 0, TRACE_WORLD_ONLY);
        if (TR_DidHit()) {
            TR_GetEndPosition(wall_pos);
        }

        int wall = fxCreateModelStatic(GEOMANCER_WALL_MODEL, client, true, true, wall_pos, ang);
        if (IsValidEntity(wall)) {
            _inst.walls.WriteCell(wall);
            setItemMetadata(wall, GEOMANCER_WALL_HEALTH);
            SDKHook(wall, SDKHook_OnTakeDamage, OnWallTakeDamage);
        }
    }
}

public bool TraceRayToGround(int entity, int mask) {
    return entity == 0; // Only hit the world
}

public Action OnWallTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
    int health = getItemMetadata(victim);
    health -= RoundFloat(damage);
    setItemMetadata(victim, health);

    if (health <= 0) {
        fxPuffBig(victim);
        AcceptEntityInput(victim, "Kill");
    }
    return Plugin_Stop;
}

stock void Geomancer_ClearWalls(GeomancerPerk perk) {
    if (perk.walls != null) {
        perk.walls.Reset();
        while (perk.walls.IsReadable()) {
            int wall = perk.walls.ReadCell();
            if (IsValidEntity(wall)) {
                AcceptEntityInput(wall, "Kill");
            }
        }
        perk.walls.Reset();
    }
}

FUNCTION(GeomancerPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    Geomancer_ClearWalls(_inst);
}

FUNCTION(GeomancerPerk, onRemove)) {
    Geomancer_ClearWalls(_inst);
}

FUNCTION(GeomancerPerk, onPeriodic)) {
    if (_inst.cooldown > 0.0) {
        _inst.cooldown = fMax(0.0, _inst.cooldown - 1.0);
    }
}

FUNCTION(GeomancerPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown > 0.0) {
        Format(buffer, maxlen, "石墙冷却: %.0fs", _inst.cooldown);
    } else {
        Format(buffer, maxlen, "石墙已准备就绪");
    }
}