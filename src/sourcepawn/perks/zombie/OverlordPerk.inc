#if defined __OverlordPerk_included
#endinput
#endif
#define __OverlordPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdkhooks>

// --- Defines ---
#define OVERLORD_DEFEND 25      // 极高生命值 (通过防御模拟)
#define OVERLORD_SPEED -50      // 移动速度非常慢
#define OVERLORD_COOLDOWN 30.0  // 冷却时间
#define OVERLORD_MAX_CREEPS 8   // 最多8个菌毯
#define OVERLORD_CREEP_HEALTH 400 // 菌毯生命值
#define OVERLORD_CREEP_RADIUS 250.0 // 菌毯光环半径
#define OVERLORD_CREEP_LIFETIME 60.0 // 菌毯持续时间
#define OVERLORD_CREEP_REPRODUCE_TIME 5.0 // 菌毯繁殖时间
#define OVERLORD_BUFF_SPEED 20  // 菌毯速度加成
#define OVERLORD_BUFF_REGEN 5   // 菌毯回血加成

// --- Data Slots ---
#define OVERLORD_DATA_COOLDOWN      (PERK_DATA_START_INDEX + 0)
#define OVERLORD_DATA_CREEPS_HANDLE (PERK_DATA_START_INDEX + 1) // DataPack handle for creep entities. Format: [entity, creation_time, last_reproduce_time]

methodmap OverlordPerk < ZombieBasePerk {
    public OverlordPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        OverlordPerk sm = view_as<OverlordPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, OverlordPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, OverlordPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, OverlordPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, OverlordPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, OverlordPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, OverlordPerkFonRemove);

        sm.cooldown = 0.0;
        sm.creeps = new DataPack();

        return sm;
    }

    property float cooldown {
        public get() {
            this.Position = view_as<DataPackPos>(OVERLORD_DATA_COOLDOWN);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(OVERLORD_DATA_COOLDOWN);
            this.WriteFloat(value);
        }
    }

    property DataPack creeps {
        public get() {
            this.Position = view_as<DataPackPos>(OVERLORD_DATA_CREEPS_HANDLE);
            return view_as<DataPack>(this.ReadCell());
        }
        public set(DataPack value) {
            this.Position = view_as<DataPackPos>(OVERLORD_DATA_CREEPS_HANDLE);
            this.WriteCell(view_as<int>(value));
        }
    }
}

stock BasePerk OverlordPerkFnew(int client) {
    return new OverlordPerk(client);
}

stock int getCreepCount(DataPack creeps) {
    if (creeps == null) return 0;
    creeps.Reset();
    int count = 0;
    while(creeps.IsReadable()) {
        creeps.ReadCell(); // entity
        creeps.ReadFloat(); // creation_time
        creeps.ReadFloat(); // last_reproduce_time
        count++;
    }
    return count;
}

FUNCTION(OverlordPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Overlord");
}

FUNCTION(OverlordPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "OverlordPerk_shortdesc");
}

FUNCTION(OverlordPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "OverlordPerk_desc");
}

FUNCTION_INT(OverlordPerk, getCategory))
{
    return 3;
}

FUNCTION(OverlordPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, OVERLORD_DEFEND);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, OVERLORD_SPEED);
}

FUNCTION(OverlordPerk, onCallForMedic)) {
    if (_inst.cooldown > GetGameTime()) {
        PrintHintText(_inst.client, "%t", "OverlordPerk_Error_Cooldown");
        return;
    }

    DataPack creeps = _inst.creeps;
    if (getCreepCount(creeps) >= OVERLORD_MAX_CREEPS) {
        PrintHintText(_inst.client, "%t", "OverlordPerk_Error_Max_Creeps");
        return;
    }

    int client = _inst.client;
    _inst.cooldown = GetGameTime() + OVERLORD_COOLDOWN;

    float start_pos[3];
    GetClientAbsOrigin(client, start_pos);

    // Find ground below player
    float end_pos[3];
    end_pos = start_pos;
    end_pos[2] -= 512.0; // Trace down to find ground

    Handle trace = TR_TraceRayEx(start_pos, end_pos, MASK_SOLID_BRUSHONLY, RayType_EndPoint);
    if (TR_DidHit(trace)) {
        float ground_pos[3];
        TR_GetEndPosition(ground_pos, trace);
        ground_pos[2] += 2.0; // Place slightly above ground
        createCreepAt(_inst, ground_pos);
        PrintHintText(client, "%t", "OverlordPerk_Creep_Placed");
    }
    CloseHandle(trace);
}

stock bool traceEntityFilter(int entity, int contentsMask) {
    if (entity == entity && contentsMask == contentsMask)
        return true;
    return true;
}

stock void createCreepAt(OverlordPerk inst, float pos[3]) {
    DataPack creeps = inst.creeps;
    if (getCreepCount(creeps) >= OVERLORD_MAX_CREEPS) {
        return;
    }

    float ang[3] = {0.0, 0.0, 0.0};
    ang[1] = GetRandomFloat(0.0, 360.0); // Randomize yaw

    int creep_entity = fxCreateModelStatic(ZFMDL_HAYBALE, inst.client, true, false, pos, ang, {128, 0, 128});
    if (IsValidEdict(creep_entity)) {
        SetEntProp(creep_entity, Prop_Data, "m_takedamage", 2); // DAMAGE_YES
        
        float now = GetGameTime();
        creeps.WriteCell(creep_entity);
        creeps.WriteFloat(now); // Creation time
        creeps.WriteFloat(now); // Last reproduce time

        setItemMetadata(creep_entity, OVERLORD_CREEP_HEALTH);
        SDKHook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
    }
}

stock Action onCreepTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
    int health = getItemMetadata(victim);
    health -= RoundFloat(damage);
    setItemMetadata(victim, health);

    if (health <= 0) {
        fxPuffSmall(victim);
        fxDeleteModel(victim); // This will unhook automatically
        return Plugin_Handled;
    }
    return Plugin_Changed;
}

stock void cleanupCreeps(OverlordPerk inst) {
    DataPack creeps = inst.creeps;
    if (creeps == null) return;

    creeps.Reset();
    while(creeps.IsReadable()) {
        int creep_entity = creeps.ReadCell();
        creeps.ReadFloat(); // skip creation time
        creeps.ReadFloat(); // skip reproduce time
        if (IsValidEdict(creep_entity)) {
            SDKUnhook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
            fxDeleteModel(creep_entity);
        }
    }
    delete creeps;
    inst.creeps = new DataPack(); // Prevent dangling pointer
}

FUNCTION(OverlordPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    cleanupCreeps(_inst);
}

FUNCTION(OverlordPerk, onRemove)) {
    cleanupCreeps(_inst);
}

FUNCTION(OverlordPerk, updateCondStats), char[] buffer, int maxlen) {
    DataPack old_creeps = _inst.creeps;
    DataPack new_creeps = new DataPack();
    _inst.creeps = new_creeps; // Temporarily point the instance to the new datapack

    int current_creep_count = getCreepCount(old_creeps);

    old_creeps.Reset();
    while(old_creeps.IsReadable()) {
        int creep_entity = old_creeps.ReadCell();
        float creation_time = old_creeps.ReadFloat();
        float last_reproduce_time = old_creeps.ReadFloat();

        if (!IsValidEdict(creep_entity) || (GetGameTime() - creation_time) > OVERLORD_CREEP_LIFETIME) {
            if (IsValidEdict(creep_entity)) {
                SDKUnhook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
                fxDeleteModel(creep_entity);
            }
            current_creep_count--;
            continue;
        }

        float creep_pos[3];
        GetEntPropVector(creep_entity, Prop_Data, "m_vecOrigin", creep_pos);

        // Reproduction logic
        float new_reproduce_time = last_reproduce_time;
        if (current_creep_count < OVERLORD_MAX_CREEPS && (GetGameTime() - last_reproduce_time) > OVERLORD_CREEP_REPRODUCE_TIME) {
            float new_pos[3];
            for (int i = 0; i < 5; i++) { // Try 5 times to find a spot
                float random_ang = GetRandomFloat(0.0, 360.0);
                float random_dist = GetRandomFloat(100.0, 200.0);
                new_pos[0] = creep_pos[0] + Cosine(DegToRad(random_ang)) * random_dist;
                new_pos[1] = creep_pos[1] + Sine(DegToRad(random_ang)) * random_dist;
                new_pos[2] = creep_pos[2];

                // More robust ground check
                float start_pos[3], end_pos[3];
                start_pos = new_pos;
                start_pos[2] += 32.0; // Start slightly above to avoid starting inside geometry
                end_pos = new_pos;
                end_pos[2] -= 256.0;   // End far below

                Handle trace = TR_TraceRayEx(start_pos, end_pos, MASK_SOLID_BRUSHONLY, RayType_EndPoint);
                if (TR_DidHit(trace)) {
                    float normal[3];
                    TR_GetPlaneNormal(trace, normal);
                    // Check if the surface is flat enough (a floor)
                    if (normal[2] > 0.7) {
                        TR_GetEndPosition(new_pos, trace);
                        new_pos[2] += 2.0; // Place it slightly above the ground
                        createCreepAt(_inst, new_pos); // This now adds to new_creeps
                        PrintHintText(_inst.client, "%t", "OverlordPerk_Creep_Reproduced");
                        new_reproduce_time = GetGameTime();
                        current_creep_count++;
                        CloseHandle(trace);
                        break;
                    }
                }
                CloseHandle(trace);
            }
        }
        
        // Add the original creep to the new list
        new_creeps.WriteCell(creep_entity);
        new_creeps.WriteFloat(creation_time);
        new_creeps.WriteFloat(new_reproduce_time);

        // Apply aura effect
        for (int i = 1; i <= MaxClients; i++) {
            if (validLivingZom(i)) {
                float zom_pos[3];
                GetClientAbsOrigin(i, zom_pos);
                if (GetVectorDistance(creep_pos, zom_pos) <= OVERLORD_CREEP_RADIUS) {
                    addStat(i, ZFStatSpeed, ZFStatTypeCond, OVERLORD_BUFF_SPEED);
                    addHealth(i, OVERLORD_BUFF_REGEN);
                }
            }
        }
    }

    delete old_creeps; // The old data is now gone
    // _inst.creeps still points to new_creeps, which is correct.

    // HUD String
    float cooldown_left = _inst.cooldown - GetGameTime();
    if (cooldown_left > 0) {
        Format(buffer, maxlen, "%t", "OverlordPerk_Status_Cooldown", RoundFloat(cooldown_left));
    } else {
        Format(buffer, maxlen, "%t", "OverlordPerk_Status_Ready", getCreepCount(_inst.creeps), OVERLORD_MAX_CREEPS);
    }
}