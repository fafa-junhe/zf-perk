#if defined __HunterPerk_included
#endinput
#endif
#define __HunterPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include <sdkhooks>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_HUNTER_ATTACK 50
#define ZF_HUNTER_DURATION 10
#define ZF_HUNTER_RADIUSSQ (85 * 85)
#define ZF_HUNTER_RESPAWNTIME 5.5
#define HUNTER_SPAWN_HEALTH 1

// Data slots
#define HUNTER_DATA_PLACED_SPAWN (PERK_DATA_START_INDEX + 0)
#define HUNTER_DATA_SPAWN_POS (PERK_DATA_START_INDEX + 1) // Size: 3
#define HUNTER_ITEM_INDEX 0 // Use zf_item[client][0] to store the spawn point entity

methodmap HunterPerk < ZombieBasePerk {
    public HunterPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        HunterPerk sm = view_as<HunterPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, HunterPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, HunterPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, HunterPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, HunterPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, HunterPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, HunterPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, HunterPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, HunterPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, HunterPerkFonRemove);

        // Initialize data
        sm.Position = view_as<DataPackPos>(HUNTER_DATA_PLACED_SPAWN);
        sm.WriteCell(0);
        float empty_vec[3];
        sm.Position = view_as<DataPackPos>(HUNTER_DATA_SPAWN_POS);
        sm.WriteCellArray(empty_vec, 3);

        return sm;
    }

    property bool placed_spawn {
        public get() {
            this.Position = view_as<DataPackPos>(HUNTER_DATA_PLACED_SPAWN);
            return this.ReadCell() == 1;
        }
        public set(bool value) {
            this.Position = view_as<DataPackPos>(HUNTER_DATA_PLACED_SPAWN);
            this.WriteCell(value ? 1 : 0);
        }
    }

    property int spawn_entity {
        public get() {
            return zf_item[this.client][HUNTER_ITEM_INDEX];
        }
        public set(int value) {
            zf_item[this.client][HUNTER_ITEM_INDEX] = value;
        }
    }

    public void getSpawnPos(float buffer[3]) {
        this.Position = view_as<DataPackPos>(HUNTER_DATA_SPAWN_POS);
        this.ReadCellArray(buffer, 3);
    }
    public void setSpawnPos(const float value[3]) {
        this.Position = view_as<DataPackPos>(HUNTER_DATA_SPAWN_POS);
        this.WriteCellArray(value, 3);
    }
}

stock BasePerk HunterPerkFnew(int client) {
    return new HunterPerk(client);
}

FUNCTION(HunterPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Hunter");
}

FUNCTION(HunterPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "HunterPerk_shortdesc");
}

FUNCTION(HunterPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "HunterPerk_desc");
}

FUNCTION(HunterPerk, onPlayerSpawn)) {
    int client = _inst.client;
    _inst.placed_spawn = false;
    
    // Clean up the old spawn point entity if it exists
    if (IsValidEdict(_inst.spawn_entity)) {
        SDKUnhook(_inst.spawn_entity, SDKHook_OnTakeDamage, onSpawnPointTakeDamage);
        AcceptEntityInput(_inst.spawn_entity, "Kill");
        _inst.spawn_entity = -1;
    }

    if (validAura(client)) {
        float spawn_pos[3];
        _inst.getSpawnPos(spawn_pos);
        TeleportEntity(client, spawn_pos, NULL_VECTOR, NULL_VECTOR);
        addStatTempStack(client, ZFStatAtt, ZF_HUNTER_ATTACK, ZF_HUNTER_DURATION);
    }
}

FUNCTION(HunterPerk, onRemove)) {
    if (validAura(_inst.client)) {
        removeAura(_inst.client);
    }
    // Clean up the spawn point entity if it exists
    if (IsValidEdict(_inst.spawn_entity)) {
        SDKUnhook(_inst.spawn_entity, SDKHook_OnTakeDamage, onSpawnPointTakeDamage);
        AcceptEntityInput(_inst.spawn_entity, "Kill");
        _inst.spawn_entity = -1;
    }
}

FUNCTION(HunterPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (validAura(_inst.client)) {
        showAura(_inst.client);
        CreateTimer(ZF_HUNTER_RESPAWNTIME, Timer_RespawnPlayerHunter, GetClientUserId(_inst.client));
    }
}

public Action Timer_RespawnPlayerHunter(Handle timer, any userid) {
    int client = GetClientOfUserId(userid);
    if (client > 0 && !IsPlayerAlive(client)) {
        TF2_RespawnPlayer(client);
    }
    return Plugin_Stop;
}

FUNCTION(HunterPerk, onCallForMedic)) {
    if (_inst.placed_spawn) {
        PrintHintText(_inst.client, "%t", "HunterPerk_Hint_AlreadyPlaced");
        return;
    }
    int client = _inst.client;
    if(!isGrounded(client))
    {
        PrintHintText(client, "%t", "HunterPerk_Hint_NotGrounded");
        ClientCommand(client, "voicemenu 2 5");
    }
    else if(isCrouching(client))
    {
        PrintHintText(client, "%t", "HunterPerk_Hint_NoCrouch");
        ClientCommand(client, "voicemenu 2 5");
    }
    float pos[3];
    GetClientAbsOrigin(client, pos);
    
    removeAura(client); // Remove previous aura if any
    createAura(client, ZFPART_AURAVORTEXBLU, AttachNone);
    pos[2] += 10;
    // Create the invisible, damageable entity
    int spawn_entity = fxCreateModelStatic(ZFMDL_PRESENT[1], client, true, true, pos, {0.0, 0.0, 0.0});
    if (IsValidEdict(spawn_entity)) {
        SetEntityRenderMode(spawn_entity, RENDER_NONE); // Invisible
        SDKHook(spawn_entity, SDKHook_OnTakeDamage, onSpawnPointTakeDamage);
        _inst.spawn_entity = spawn_entity;
    }

    _inst.setSpawnPos(pos);
    _inst.placed_spawn = true;
    ClientCommand(client, "voicemenu 0 6");
    PrintHintText(client, "%t", "HunterPerk_Hint_Placed");
}

FUNCTION(HunterPerk, onPeriodic)) {
    if (!_inst.placed_spawn || !IsValidEdict(_inst.spawn_entity)) {
        return;
    }

    float spawn_pos[3];
    GetEntPropVector(_inst.spawn_entity, Prop_Send, "m_vecOrigin", spawn_pos);

    for (int i = 1; i <= MaxClients; i++) {
        if (validClient(i) && IsPlayerAlive(i) && isSur(i)) {
            float surv_pos[3];
            GetClientAbsOrigin(i, surv_pos);

            if (GetVectorDistance(spawn_pos, surv_pos, true) < ZF_HUNTER_RADIUSSQ) {
                destroySpawnPoint(_inst.client, i);
                break;
            }
        }
    }
}

stock void destroySpawnPoint(int owner, int destroyer) {
    if (!validClient(owner)) {
        return;
    }

    BasePerk perk_base = g_hPerks[owner];
    if (perk_base == null) {
        return;
    }
    HunterPerk perk = view_as<HunterPerk>(perk_base);
    if (perk == null) {
        return;
    }

    int spawn_entity = perk.spawn_entity;
    if (!IsValidEdict(spawn_entity)) {
        return;
    }

    if (validAura(owner)) {
        removeAura(owner);
        PrintHintText(owner, "%t", "HunterPerk_Hint_Destroyed");
    }

    PrintHintText(destroyer, "%t", "HunterPerk_Status_Destroyed");

    SDKUnhook(spawn_entity, SDKHook_OnTakeDamage, onSpawnPointTakeDamage);
    AcceptEntityInput(spawn_entity, "Kill");
    
    perk.spawn_entity = -1;
}

stock Action onSpawnPointTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype) {
    int owner = GetEntPropEnt(victim, Prop_Data, "m_hOwnerEntity");
    if (!validClient(owner) || !isSur(attacker)) {
        return Plugin_Continue;
    }
 
    destroySpawnPoint(owner, attacker);
 
    return Plugin_Handled; // Prevent further damage processing
}

FUNCTION(HunterPerk, updateCondStats), char[] buffer, int maxlen) {
    if (!_inst.placed_spawn) {
        Format(buffer, maxlen, "%t", "HunterPerk_Status_CanPlace");
        return;
    }

    if (!validAura(_inst.client)) {
        Format(buffer, maxlen, "%t", "HunterPerk_Status_Destroyed");
        return;
    }
    
    Format(buffer, maxlen, "%t", "HunterPerk_Status_Active");
}
