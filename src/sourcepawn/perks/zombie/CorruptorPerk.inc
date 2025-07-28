#if defined __CorruptorPerk_included
#endinput
#endif
#define __CorruptorPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include <sdkhooks>

// Perk constants from new_perks.md
#define CORRUPTOR_SPEED -10 // "移动速度降低"
#define CORRUPTOR_BUILDING_DAMAGE_BONUS 50 // "对工程师的建筑造成额外伤害"
#define CORRUPTOR_CORRUPT_DURATION 5.0
#define CORRUPTOR_CORRUPTED_LIFETIME 15.0

// Data slots
#define CORRUPTOR_DATA_TARGET_BUILDING (PERK_DATA_START_INDEX + 0)
#define CORRUPTOR_DATA_FIRST_HIT_TIME  (PERK_DATA_START_INDEX + 1)
#define CORRUPTOR_DATA_LAST_HIT_TIME   (PERK_DATA_START_INDEX + 2)

methodmap CorruptorPerk < ZombieBasePerk {
    public CorruptorPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        CorruptorPerk sm = view_as<CorruptorPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CorruptorPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CorruptorPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CorruptorPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, CorruptorPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, CorruptorPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CorruptorPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, CorruptorPerkFonPlayerSpawn);

        sm.target_building = -1;
        sm.first_hit_time = 0.0;
        sm.last_hit_time = 0.0;

        return sm;
    }

    property int target_building {
        public get() {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_TARGET_BUILDING);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_TARGET_BUILDING);
            this.WriteCell(value);
        }
    }

    property float first_hit_time {
        public get() {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_FIRST_HIT_TIME);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_FIRST_HIT_TIME);
            this.WriteFloat(value);
        }
    }

    property float last_hit_time {
        public get() {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_LAST_HIT_TIME);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(CORRUPTOR_DATA_LAST_HIT_TIME);
            this.WriteFloat(value);
        }
    }
}

stock BasePerk CorruptorPerkFnew(int client) {
    return new CorruptorPerk(client);
}

FUNCTION(CorruptorPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Corruptor");
}

FUNCTION(CorruptorPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CorruptorPerk_shortdesc");
}

FUNCTION(CorruptorPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CorruptorPerk_desc");
}

FUNCTION(CorruptorPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, CORRUPTOR_SPEED);
}

FUNCTION(CorruptorPerk, onPlayerSpawn)) {
    _inst.target_building = -1;
    _inst.first_hit_time = 0.0;
    _inst.last_hit_time = 0.0;
}

stock bool IsBuilding(int entity) {
    if (entity <= 0 || !IsValidEntity(entity)) {
        return false;
    }
    char classname[64];
    GetEdictClassname(entity, classname, sizeof(classname));
    return (StrContains(classname, "obj_", false) != -1);
}

stock void doCorruptBuilding(int owner, int building) {
    // Change team and ownership
    SetEntProp(building, Prop_Send, "m_iTeamNum", zomTeam());
    SetEntPropEnt(building, Prop_Send, "m_hOwnerEntity", owner);
    
    // For sentries, make them active
    char classname[64];
    GetEdictClassname(building, classname, sizeof(classname));
    if (StrEqual(classname, "obj_sentrygun")) {
        SetEntProp(building, Prop_Send, "m_bDisabled", 0);
    }

    // Create a timer to destroy the building
    DataPack pack = new DataPack();
    pack.WriteCell(building);
    CreateTimer(CORRUPTOR_CORRUPTED_LIFETIME, timer_DestroyBuilding, pack);
    
    PrintHintText(owner, "建筑已被腐化！");
}

public Action timer_DestroyBuilding(Handle timer, DataPack pack) {
    pack.Reset();
    int building = pack.ReadCell();
    if (IsValidEntity(building)) {
        SDKHooks_TakeDamage(building, 0, 0, 9999.0, DMG_BLAST | DMG_ALWAYSGIB);
    }
    delete pack;
    return Plugin_Stop;
}

FUNCTION(CorruptorPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    int client = _inst.client;
    if (client != attacker) return;

    if (!IsBuilding(victim)) {
        _inst.target_building = -1;
        return;
    }

    // Apply bonus damage
    SDKHooks_TakeDamage(victim, client, client, damage * (CORRUPTOR_BUILDING_DAMAGE_BONUS / 100.0), DMG_CLUB);

    // Corruption logic
    if (victim != _inst.target_building) {
        _inst.target_building = victim;
        _inst.first_hit_time = GetGameTime();
    }
    
    _inst.last_hit_time = GetGameTime();

    if ((GetGameTime() - _inst.first_hit_time) >= CORRUPTOR_CORRUPT_DURATION) {
        doCorruptBuilding(client, victim);
        _inst.target_building = -1;
    }
}

FUNCTION(CorruptorPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.target_building != -1) {
        if (GetGameTime() - _inst.last_hit_time > 1.5) {
            _inst.target_building = -1;
        } else {
            int progress = RoundFloat(((GetGameTime() - _inst.first_hit_time) / CORRUPTOR_CORRUPT_DURATION) * 100.0);
            if (progress > 100) progress = 100;
            Format(buffer, maxlen, "正在腐化: %d%%", progress);
        }
    }
}
