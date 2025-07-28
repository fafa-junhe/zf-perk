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
#define OVERLORD_DEFEND 75      // 极高生命值 (通过防御模拟)
#define OVERLORD_SPEED -40      // 移动速度非常慢
#define OVERLORD_COOLDOWN 30.0  // 冷却时间
#define OVERLORD_MAX_CREEPS 3   // 最多3个菌毯
#define OVERLORD_CREEP_HEALTH 400 // 菌毯生命值
#define OVERLORD_CREEP_RADIUS 300.0 // 菌毯光环半径
#define OVERLORD_CREEP_LIFETIME 60.0 // 菌毯持续时间
#define OVERLORD_BUFF_SPEED 20  // 菌毯速度加成
#define OVERLORD_BUFF_REGEN 5   // 菌毯回血加成

// --- Data Slots ---
#define OVERLORD_DATA_COOLDOWN      (PERK_DATA_START_INDEX + 0)
#define OVERLORD_DATA_CREEPS_HANDLE (PERK_DATA_START_INDEX + 1) // DataPack handle for creep entities

methodmap OverlordPerk < ZombieBasePerk {
    public OverlordPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        OverlordPerk sm = view_as<OverlordPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, OverlordPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, OverlordPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, OverlordPerkFgetDesc);
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
        creeps.ReadFloat(); // time
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

FUNCTION(OverlordPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, OVERLORD_DEFEND);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, OVERLORD_SPEED);
}

FUNCTION(OverlordPerk, onCallForMedic)) {
    if (_inst.cooldown > GetGameTime()) {
        PrintHintText(_inst.client, "技能尚未冷却。");
        return;
    }

    DataPack creeps = _inst.creeps;
    if (getCreepCount(creeps) >= OVERLORD_MAX_CREEPS) {
        PrintHintText(_inst.client, "你已放置了最大数量的菌毯。");
        return;
    }

    int client = _inst.client;
    _inst.cooldown = GetGameTime() + OVERLORD_COOLDOWN;

    float pos[3], ang[3];
    GetClientAbsOrigin(client, pos);
    GetClientAbsAngles(client, ang);
    ang[0] = 0.0; // Make it flat on the ground

    // Using a placeholder model, e.g., haybale from SickPerk
    int creep_entity = fxCreateModelStatic(ZFMDL_HAYBALE, client, true, false, pos, ang, {128, 0, 128});
    if (IsValidEdict(creep_entity)) {
        creeps.WriteCell(creep_entity);
        creeps.WriteFloat(GetGameTime()); // Creation time
        setItemMetadata(creep_entity, OVERLORD_CREEP_HEALTH);
        SDKHook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
        PrintHintText(client, "菌毯已放置！");
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
        if (IsValidEdict(creep_entity)) {
            SDKUnhook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
            fxDeleteModel(creep_entity);
        }
    }
    delete creeps;
    inst.creeps = new DataPack();
}

FUNCTION(OverlordPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    cleanupCreeps(_inst);
}

FUNCTION(OverlordPerk, onRemove)) {
    cleanupCreeps(_inst);
}

FUNCTION(OverlordPerk, updateCondStats), char[] buffer, int maxlen) {
    DataPack creeps = _inst.creeps;
    DataPack new_creeps = new DataPack();

    creeps.Reset();
    while(creeps.IsReadable()) {
        int creep_entity = creeps.ReadCell();
        float creation_time = creeps.ReadFloat();

        if (!IsValidEdict(creep_entity) || (GetGameTime() - creation_time) > OVERLORD_CREEP_LIFETIME) {
            if (IsValidEdict(creep_entity)) {
                SDKUnhook(creep_entity, SDKHook_OnTakeDamage, onCreepTakeDamage);
                fxDeleteModel(creep_entity);
            }
            continue;
        }

        new_creeps.WriteCell(creep_entity);
        new_creeps.WriteFloat(creation_time);

        float creep_pos[3];
        GetEntPropVector(creep_entity, Prop_Data, "m_vecOrigin", creep_pos);

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

    delete _inst.creeps;
    _inst.creeps = new_creeps;

    // HUD String
    float cooldown_left = _inst.cooldown - GetGameTime();
    if (cooldown_left > 0) {
        Format(buffer, maxlen, "菌毯冷却: %ds", RoundFloat(cooldown_left));
    } else {
        Format(buffer, maxlen, "菌毯已就绪 (%d/%d)", getCreepCount(_inst.creeps), OVERLORD_MAX_CREEPS);
    }
}