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
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_HUNTER_ATTACK 50
#define ZF_HUNTER_DURATION 10
#define ZF_HUNTER_RADIUSSQ (85 * 85)
#define ZF_HUNTER_RESPAWNTIME 5.5

// Data slots
#define HUNTER_DATA_PLACED_SPAWN (PERK_DATA_START_INDEX + 0)
#define HUNTER_DATA_SPAWN_POS (PERK_DATA_START_INDEX + 1) // Size: 3

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
        return;
    }
    int client = _inst.client;
    if(_inst.placed_spawn)
    {
        PrintHintText(client, "你已经放过了你的重生点.");
        ClientCommand(client, "voicemenu 2 5");
    }
    else if(!isGrounded(client))
    {
        PrintHintText(client, "你必须站在地上才能布置重生点.");
        ClientCommand(client, "voicemenu 2 5");        
    }
    else if(isCrouching(client))
    {
        PrintHintText(client, "你不需要蹲着来布置重生点.");
        ClientCommand(client, "voicemenu 2 5");
    }  
    float pos[3];
    GetClientAbsOrigin(client, pos);
    
    removeAura(client); // Remove previous aura if any
    createAura(client, ZFPART_AURAVORTEXBLU, AttachNone);

    _inst.setSpawnPos(pos);
    _inst.placed_spawn = true;
    ClientCommand(client, "voicemenu 0 6");
    PrintHintText(client, "重生点已放置。");
}

FUNCTION(HunterPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;

    if (!_inst.placed_spawn) {
        strcopy(buffer, maxlen, "可以放置重生点");
        return;
    }

    if (!validAura(client)) {
        strcopy(buffer, maxlen, "重生点已被摧毁");
        return;
    }
    
    bool destroyed = false;
    float spawn_pos[3];
    _inst.getSpawnPos(spawn_pos);
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i)) {
            float sur_pos[3];
            GetClientAbsOrigin(i, sur_pos);
            if (GetVectorDistance(spawn_pos, sur_pos, true) < ZF_HUNTER_RADIUSSQ) {
                removeAura(client);
                PrintHintText(client, "你的重生点已被幸存者摧毁！");
                destroyed = true;
                break;
            }
        }
    }
    
    if(destroyed) {
        strcopy(buffer, maxlen, "重生点已被摧毁");
    } else {
        strcopy(buffer, maxlen, "重生点激活中");
    }
}
