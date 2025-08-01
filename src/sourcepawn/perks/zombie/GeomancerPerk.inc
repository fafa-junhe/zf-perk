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
#define GEOMANCER_COOLDOWN 30.0
#define GEOMANCER_WALL_HEALTH 300
#define GEOMANCER_WALL_COUNT 5
#define GEOMANCER_WALL_DISTANCE 80.0
#define GEOMANCER_WALL_LIFETIME 20.0
#define GEOMANCER_WALL_MODEL "models/props_mining/rock005.mdl"

#define GEOMANCER_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define GEOMANCER_DATA_WALLS_HANDLE (PERK_DATA_START_INDEX + 1)
#define GEOMANCER_DATA_WALL_COUNT (PERK_DATA_START_INDEX + 2)

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
        sm.wallCount = 0;
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

    property int wallCount {
        public get() {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_WALL_COUNT);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_WALL_COUNT);
            this.WriteCell(value);
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
        PrintHintText(client, "%t", "GeomancerPerk_Hint_Cooldown", RoundFloat(_inst.cooldown));
        return;
    }

    if (!(GetEntityFlags(client) & FL_DUCKING)) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_Crouch");
        return;
    }

    if (_inst.wallCount >= GEOMANCER_WALL_COUNT) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_Full");
        return;
    }

    float eye_pos[3], ang[3], fwd[3], wall_pos[3];
    GetClientEyePosition(client, eye_pos); // Start from player's eyes to avoid starting inside the ground.
    GetClientEyeAngles(client, ang);
    ang[0] = 0.0; // We only want horizontal direction.
    ang[2] = 0.0;

    GetAngleVectors(ang, fwd, NULL_VECTOR, NULL_VECTOR);

    // We want to find a suitable ground position in front of the player.
    // First, let's find a point on the ground right under the player's target.
    float target_pos[3];
    ScaleVector(fwd, GEOMANCER_WALL_DISTANCE);
    AddVectors(eye_pos, fwd, target_pos);

    // Now, trace down from this target point to find the actual ground.
    float trace_end[3];
    trace_end = target_pos;
    trace_end[2] -= 4096.0; // A very long trace downwards.

    TR_TraceRayFilter(target_pos, trace_end, MASK_SOLID, RayType_EndPoint, TraceRayToGround, client, TRACE_WORLD_ONLY);

    if (!TR_DidHit()) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_NoGround");
        return; // Don't set cooldown if we can't find a valid spot.
    }

    TR_GetEndPosition(wall_pos);

    // Check if the position is too close to the player
    float player_pos[3];
    GetClientAbsOrigin(client, player_pos);
    if (GetVectorDistance(player_pos, wall_pos, true) < 64.0 * 64.0) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_TooClose");
        return;
    }

    _inst.cooldown = GEOMANCER_COOLDOWN;

    int wall = fxCreateModelStatic(GEOMANCER_WALL_MODEL, client, true, true, wall_pos, ang);
    if (IsValidEntity(wall)) {
        _inst.walls.WriteCell(wall);
        _inst.walls.WriteFloat(GetGameTime());
        _inst.wallCount++;
        setItemMetadata(wall, GEOMANCER_WALL_HEALTH);
        SDKHook(wall, SDKHook_OnTakeDamage, OnWallTakeDamage);
    } else {
        // If creating the wall fails for some reason, reset the cooldown.
        _inst.cooldown = 0.0;
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
            perk.walls.ReadFloat(); // Skip creation time
            if (IsValidEntity(wall)) {
                AcceptEntityInput(wall, "Kill");
            }
        }
        perk.walls.Reset();
        perk.wallCount = 0;
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

    if (_inst.wallCount == 0) {
        return;
    }

    DataPack newWalls = new DataPack();
    int newCount = 0;
    _inst.walls.Reset();

    while (_inst.walls.IsReadable()) {
        int wall = _inst.walls.ReadCell();
        float creationTime = _inst.walls.ReadFloat();

        if (IsValidEntity(wall) && (GetGameTime() - creationTime < GEOMANCER_WALL_LIFETIME)) {
            newWalls.WriteCell(wall);
            newWalls.WriteFloat(creationTime);
            newCount++;
        } else {
            if (IsValidEntity(wall)) {
                fxPuffBig(wall);
                AcceptEntityInput(wall, "Kill");
            }
        }
    }

    // If the count differs, we need to update
    if (newCount != _inst.wallCount) {
        delete _inst.walls;
        _inst.walls = newWalls;
        _inst.wallCount = newCount;
    } else {
        delete newWalls;
    }
}

FUNCTION(GeomancerPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown > 0.0) {
        Format(buffer, maxlen, "%t", "GeomancerPerk_Status_Cooldown", RoundFloat(_inst.cooldown));
    } else {
        Format(buffer, maxlen, "%t", "GeomancerPerk_Status_Ready");
    }
}