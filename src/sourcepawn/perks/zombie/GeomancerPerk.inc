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
#include <tf2>

#define GEOMANCER_SPEED -20
#define GEOMANCER_COOLDOWN 15.0
#define GEOMANCER_ROCK_DISTANCE 200.0
#define GEOMANCER_ROCK_RISE_HEIGHT 96.0
#define GEOMANCER_ROCK_ANIM_TIME 0.6
#define GEOMANCER_ROCK_ANIM_DELAY 0.15
#define GEOMANCER_ROCK_LIFETIME 2.0
#define GEOMANCER_ROCK_UPWARD_FORCE 800.0
#define GEOMANCER_ROCK_HIT_RADIUS 500.0
#define GEOMANCER_WALL_MODEL "models/props_mining/rock005.mdl"

#define GEOMANCER_DATA_COOLDOWN (PERK_DATA_START_INDEX + 0)
#define GEOMANCER_DATA_WALLS_HANDLE (PERK_DATA_START_INDEX + 1) // Will store ArrayList handle

methodmap GeomancerPerk < ZombieBasePerk {
    public GeomancerPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        GeomancerPerk sm = view_as<GeomancerPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, GeomancerPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, GeomancerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, GeomancerPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, GeomancerPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GeomancerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, GeomancerPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, GeomancerPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, GeomancerPerkFonPlayerSpawn);

        sm.cooldown = 0.0;
        sm.walls = new ArrayList();
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

    property ArrayList walls {
        public get() {
            this.Position = view_as<DataPackPos>(GEOMANCER_DATA_WALLS_HANDLE);
            return view_as<ArrayList>(this.ReadCell());
        }
        public set(ArrayList value) {
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

FUNCTION_INT(GeomancerPerk, getCategory))
{
    return 3;
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

    if (_inst.walls.Length > 0) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_Full");
        return;
    }

    float eye_pos[3], ang[3], fwd[3], right[3];
    GetClientEyePosition(client, eye_pos);
    GetClientEyeAngles(client, ang);
    ang[0] = 0.0;
    ang[2] = 0.0;

    GetAngleVectors(ang, fwd, right, NULL_VECTOR);

    float start_pos[3];
    ScaleVector(fwd, GEOMANCER_ROCK_DISTANCE);
    AddVectors(eye_pos, fwd, start_pos);

    float trace_end[3];
    trace_end = start_pos;
    trace_end[2] -= 4096.0;

    TR_TraceRayFilter(start_pos, trace_end, MASK_SOLID, RayType_EndPoint, TraceRayToGround, client, TRACE_WORLD_ONLY);
    if (!TR_DidHit()) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_NoGround");
        return;
    }
    TR_GetEndPosition(start_pos);

    float player_pos[3];
    GetClientAbsOrigin(client, player_pos);
    if (GetVectorDistance(player_pos, start_pos, true) < 64.0 * 64.0) {
        PrintHintText(client, "%t", "GeomancerPerk_Hint_TooClose");
        return;
    }

    _inst.cooldown = GEOMANCER_COOLDOWN;

    // Create a single rock at the target position
    float create_pos[3];
    create_pos = start_pos;
    create_pos[2] -= GEOMANCER_ROCK_RISE_HEIGHT;

    int wall = fxCreateModelStatic(GEOMANCER_WALL_MODEL, client, false, false, create_pos, ang);
    if (IsValidEntity(wall)) {
        DataPack rock_data = new DataPack();
        rock_data.WriteCell(wall);
        rock_data.WriteFloat(start_pos[0]); // Store original ground position
        rock_data.WriteFloat(start_pos[1]);
        rock_data.WriteFloat(start_pos[2]);
        rock_data.WriteFloat(ang[0]);
        rock_data.WriteFloat(ang[1]);
        rock_data.WriteFloat(ang[2]);
        _inst.walls.Push(rock_data);
    }

    if (_inst.walls.Length > 0) {
        DataPack anim_pack = new DataPack();
        anim_pack.WriteCell(client);
        anim_pack.WriteCell(0); // 0 for rising state
        anim_pack.WriteCell(0); // current pillar index
        anim_pack.WriteFloat(GetGameTime());
        CreateTimer(GEOMANCER_ROCK_ANIM_DELAY, Geomancer_AnimationTimer, anim_pack);
        
        // Play particle effect at start
        float ground_pos[3];
        TR_GetEndPosition(ground_pos);
        float client_pos[3];
        GetClientAbsOrigin(client, client_pos);
        float offset[3];
        SubtractVectors(ground_pos, client_pos, offset);
        fxCreateParticle("underworld_smoke02", client, AttachNone, 2.0, offset);
    } else {
        _inst.cooldown = 0.0; // Failed to create any rock
    }
}

public bool TraceRayToGround(int entity, int mask) {
    return entity == 0;
}

stock void Geomancer_SwapToSolidWalls(GeomancerPerk perk) {
    if (perk == null || perk.walls == null) {
        return;
    }

    int client = perk.client;

    for (int i = 0; i < perk.walls.Length; i++) {
        Handle rock_pack_handle = perk.walls.Get(i);
        DataPack rock_pack = view_as<DataPack>(rock_pack_handle);
        rock_pack.Reset();

        int old_wall = rock_pack.ReadCell();
        if (IsValidEntity(old_wall)) {
            AcceptEntityInput(old_wall, "Kill");
        }

        float ground_pos[3], ang[3];
        ground_pos[0] = rock_pack.ReadFloat();
        ground_pos[1] = rock_pack.ReadFloat();
        ground_pos[2] = rock_pack.ReadFloat();
        ang[0] = rock_pack.ReadFloat();
        ang[1] = rock_pack.ReadFloat();
        ang[2] = rock_pack.ReadFloat();

        // The solid wall should appear at the final position of the rising animation, which is ground_pos.
        int new_wall = fxCreateModelStatic(GEOMANCER_WALL_MODEL, client, false, true, ground_pos, ang);
        
        // Update the wall entity in the datapack
        rock_pack.Position = view_as<DataPackPos>(0);
        rock_pack.WriteCell(new_wall);
    }
}

public Action Geomancer_StartFallingTimer(Handle timer, DataPack pack) {
    pack.Reset();
    int client = pack.ReadCell();

    BasePerk perk_base = g_hPerks[client];
    if (perk_base == null) {
        delete pack;
        return Plugin_Stop;
    }
    GeomancerPerk perk = view_as<GeomancerPerk>(perk_base);
    if (perk == null || perk.walls.Length == 0) {
        Geomancer_ClearWalls(perk);
        delete pack;
        return Plugin_Stop;
    }

    // Now, create the *actual* animation pack
    DataPack anim_pack = new DataPack();
    anim_pack.WriteCell(client);
    anim_pack.WriteCell(1); // state: falling
    anim_pack.WriteCell(0); // index
    anim_pack.WriteFloat(GetGameTime()); // startTime is NOW
    
    // Start the animation immediately
    CreateTimer(0.02, Geomancer_AnimationTimer, anim_pack);

    // Play particle effect at start of fall
    Handle rock_pack_handle = perk.walls.Get(0);
    DataPack rock_pack = view_as<DataPack>(rock_pack_handle);
    rock_pack.Reset();
    rock_pack.ReadCell(); // skip wall entity
    float ground_pos[3];
    ground_pos[0] = rock_pack.ReadFloat();
    ground_pos[1] = rock_pack.ReadFloat();
    ground_pos[2] = rock_pack.ReadFloat();
    float client_pos[3];
    GetClientAbsOrigin(client, client_pos);
    float offset[3];
    SubtractVectors(ground_pos, client_pos, offset);
    fxCreateParticle("underworld_smoke02", client, AttachNone, 2.0, offset);

    delete pack; // delete the intermediate pack
    return Plugin_Stop;
}

public Action Geomancer_AnimationTimer(Handle timer, DataPack pack) {
    pack.Reset();
    int client = pack.ReadCell();
    int state = pack.ReadCell(); // 0: rising, 1: falling
    int index = pack.ReadCell();
    float startTime = pack.ReadFloat();

    BasePerk perk_base = g_hPerks[client];
    if (perk_base == null) {
        delete pack;
        return Plugin_Stop;
    }
    GeomancerPerk perk = view_as<GeomancerPerk>(perk_base);
    if (perk == null || index >= perk.walls.Length) {
        Geomancer_ClearWalls(perk);
        delete pack;
        return Plugin_Stop;
    }

    Handle rock_pack_handle = perk.walls.Get(index);
    DataPack rock_pack = view_as<DataPack>(rock_pack_handle);
    rock_pack.Reset();
    int wall = rock_pack.ReadCell();
    float ground_pos[3];
    ground_pos[0] = rock_pack.ReadFloat();
    ground_pos[1] = rock_pack.ReadFloat();
    ground_pos[2] = rock_pack.ReadFloat();

    if (!IsValidEntity(wall)) {
        // Wall destroyed prematurely, just continue the sequence
        index++;
        if (index < perk.walls.Length) {
            pack.Position = view_as<DataPackPos>(8); // pos of index
            pack.WriteCell(index);
            pack.Position = view_as<DataPackPos>(12); // pos of startTime
            pack.WriteFloat(GetGameTime());
            CreateTimer(GEOMANCER_ROCK_ANIM_DELAY, Geomancer_AnimationTimer, pack);
        } else {
            if (state == 0) { // Finished rising
                Geomancer_SwapToSolidWalls(perk);
                // Create an intermediate pack for the delay timer
                DataPack delay_pack = new DataPack();
                delay_pack.WriteCell(client);
                CreateTimer(GEOMANCER_ROCK_LIFETIME, Geomancer_StartFallingTimer, delay_pack);
                delete pack; // Delete the old rising animation pack
            } else { // Finished falling
                Geomancer_ClearWalls(perk);
                delete pack;
            }
        }
        return Plugin_Stop;
    }

    float progress = (GetGameTime() - startTime) / GEOMANCER_ROCK_ANIM_TIME;
    progress = fMin(1.0, progress);

    float new_pos[3];
    new_pos = ground_pos;

    if (state == 0) { // Rising
        new_pos[2] = ground_pos[2] - GEOMANCER_ROCK_RISE_HEIGHT + (GEOMANCER_ROCK_RISE_HEIGHT * progress);
        TeleportEntity(wall, new_pos, NULL_VECTOR, NULL_VECTOR);

        // Apply upward force to survivors
        float radius_sq = GEOMANCER_ROCK_HIT_RADIUS * GEOMANCER_ROCK_HIT_RADIUS;
        for (int i = 1; i <= MaxClients; i++) {
            if (IsClientInGame(i) && IsPlayerAlive(i) && GetClientTeam(i) == 2) {
                float surv_pos[3];
                GetClientAbsOrigin(i, surv_pos);
                if (GetVectorDistance(new_pos, surv_pos, true) < radius_sq) {
                    float vel[3];
                    GetEntPropVector(i, Prop_Data, "m_vecVelocity", vel);
                    vel[2] = GEOMANCER_ROCK_UPWARD_FORCE;
                    TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, vel);

                    PrintHintText(client, "%t", "GeomancerPerk_Hint_Hit_Attacker", i);
                    PrintHintText(i, "%t", "GeomancerPerk_Hint_Hit_Victim");
                }
            }
        }
    } else { // Falling
        new_pos[2] = ground_pos[2] - (GEOMANCER_ROCK_RISE_HEIGHT * progress);
        TeleportEntity(wall, new_pos, NULL_VECTOR, NULL_VECTOR);
    }

    if (progress >= 1.0) {
        // The final ClearWalls call will handle killing the entity.
        // This avoids potential issues with the entity being killed before the timer logic is fully resolved.
        /*
        if (state == 1) { // Finished falling
            AcceptEntityInput(wall, "Kill");
        }
        */

        index++;
        if (index < perk.walls.Length) {
            // Animate next pillar
            pack.Position = view_as<DataPackPos>(8);
            pack.WriteCell(index);
            pack.Position = view_as<DataPackPos>(12);
            pack.WriteFloat(GetGameTime());
            CreateTimer(GEOMANCER_ROCK_ANIM_DELAY, Geomancer_AnimationTimer, pack);
        } else {
            // Finished sequence for this state
            if (state == 0) { // Finished rising
                Geomancer_SwapToSolidWalls(perk);
                // Create an intermediate pack for the delay timer
                DataPack delay_pack = new DataPack();
                delay_pack.WriteCell(client);
                CreateTimer(GEOMANCER_ROCK_LIFETIME, Geomancer_StartFallingTimer, delay_pack);
                delete pack; // Delete the old rising animation pack
            } else { // Finished falling
                Geomancer_ClearWalls(perk);
                delete pack;
            }
        }
    } else {
        // Continue animation for current pillar
        CreateTimer(0.02, Geomancer_AnimationTimer, pack);
    }
    return Plugin_Stop;
}

stock void Geomancer_ClearWalls(GeomancerPerk perk) {
    if (perk.walls != null) {
        for (int i = 0; i < perk.walls.Length; i++) {
            Handle rock_pack_handle = perk.walls.Get(i);
            if (rock_pack_handle != null) {
                DataPack rock_pack = view_as<DataPack>(rock_pack_handle);
                rock_pack.Reset();
                int wall = rock_pack.ReadCell();
                if (IsValidEntity(wall)) {
                    AcceptEntityInput(wall, "Kill");
                }
                delete rock_pack;
            }
        }
        perk.walls.Clear();
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
        Format(buffer, maxlen, "%t", "GeomancerPerk_Status_Cooldown", RoundFloat(_inst.cooldown));
    } else if (_inst.walls.Length > 0) {
        Format(buffer, maxlen, "%t", "GeomancerPerk_Status_Active");
    } else {
        Format(buffer, maxlen, "%t", "GeomancerPerk_Status_Ready");
    }
}

FUNCTION(GeomancerPerk, onPlayerSpawn)) {
    _inst.cooldown = 0.0;
    Geomancer_ClearWalls(_inst);
}