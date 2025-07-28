#if defined __AlchemistPerk_included
#endinput
#endif
#define __AlchemistPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdkhooks>
#include <handles>

#define ALCHEMIST_POTION_GEN_TIME 40
#define ALCHEMIST_MAX_POTIONS 3
#define ALCHEMIST_AOE_DURATION 10.0
#define ALCHEMIST_AOE_RADIUS 180.0
#define ALCHEMIST_AOE_RADIUS_SQ (ALCHEMIST_AOE_RADIUS * ALCHEMIST_AOE_RADIUS)
#define ALCHEMIST_ALLY_HEAL 5
#define ALCHEMIST_ALLY_SPEED_BONUS 20
#define ALCHEMIST_ZOMBIE_SLOW -30
#define ALCHEMIST_ZOMBIE_DAMAGE 5.0
#define ALCHEMIST_POTION_MODEL "models/weapons/w_models/w_jar.mdl"

// We'll use the global zf_item array to track the puddle entities
#define ALCHEMIST_MAX_PUDDLES MAX_ITEMS 

methodmap AlchemistPerk < SurvivorBasePerk {
    public AlchemistPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AlchemistPerk sm = view_as<AlchemistPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, AlchemistPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, AlchemistPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, AlchemistPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, AlchemistPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AlchemistPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AlchemistPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, AlchemistPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AlchemistPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, AlchemistPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, AlchemistPerkFonDeath);

        return sm;
    }
}

stock SurvivorBasePerk AlchemistPerkFnew(int client) {
    return new AlchemistPerk(client);
}

FUNCTION(AlchemistPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Alchemist");
}

FUNCTION(AlchemistPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AlchemistPerk_shortdesc");
}

FUNCTION(AlchemistPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "AlchemistPerk_desc");
}

FUNCTION(AlchemistPerk, onPlayerSpawn)) {
    int client = _inst.client;
    zf_perkState[client] = 0; // Potion count
    zf_perkTimer[client] = ALCHEMIST_POTION_GEN_TIME; // Time to next potion
    // Clean up any existing puddles from this player
    AlchemistPerkFonRemove(_inst);
}

FUNCTION(AlchemistPerk, onPeriodic)) {
    int client = _inst.client;
    if (zf_perkState[client] < ALCHEMIST_MAX_POTIONS) {
        if (zf_perkTimer[client] > 0) {
            zf_perkTimer[client]--;
        } else {
            zf_perkState[client]++;
            zf_perkTimer[client] = ALCHEMIST_POTION_GEN_TIME;
            PrintHintText(client, "一瓶药剂已调和完毕! (总计: %d)", zf_perkState[client]);
        }
    }

    // Puddle effect logic
    for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++) {
        int puddle = zf_item[client][i];
        if (IsValidEntity(puddle)) {
            float puddle_pos[3];
            GetEntPropVector(puddle, Prop_Send, "m_vecOrigin", puddle_pos);

            for (int p = 1; p <= MaxClients; p++) {
                if (!IsClientInGame(p) || !IsPlayerAlive(p)) continue;

                float player_pos[3];
                GetClientAbsOrigin(p, player_pos);

                if (GetVectorDistance(player_pos, puddle_pos, true) < ALCHEMIST_AOE_RADIUS_SQ) {
                    if (isSur(p)) {
                        addHealth(p, ALCHEMIST_ALLY_HEAL);
                    } else if (isZom(p)) {
                        SDKHooks_TakeDamage(p, puddle, client, ALCHEMIST_ZOMBIE_DAMAGE, DMG_GENERIC);
                    }
                }
            }
        }
    }
}

FUNCTION(AlchemistPerk, onCallForMedic)) {
    int client = _inst.client;
    if (zf_perkState[client] <= 0) {
        PrintHintText(client, "你没有药剂了!");
        return;
    }

    int item_index = getFreeItemIndex(client, ALCHEMIST_MAX_PUDDLES);
    if (item_index == -1) {
        PrintHintText(client, "你制造的药剂区域已达上限!");
        return;
    }

    zf_perkState[client]--;
    int color[3] = {100, 255, 100};
    int item = doItemThrow(client, ALCHEMIST_POTION_MODEL, 800.0, color);
    if (item != -1) {
        setItemMetadata(item, item_index); // Store our item index in the item's metadata
        SDKHook(item, SDKHook_StartTouch, Alchemist_OnPotionCollide);
    }
}

FUNCTION(AlchemistPerk, onGameFrame)) {
    int client = _inst.client;
    // Puddle effect logic for conditional stats
    for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++) {
        int puddle = zf_item[client][i];
        if (IsValidEntity(puddle)) {
            float puddle_pos[3];
            GetEntPropVector(puddle, Prop_Send, "m_vecOrigin", puddle_pos);

            for (int p = 1; p <= MaxClients; p++) {
                if (!IsClientInGame(p) || !IsPlayerAlive(p)) continue;

                float player_pos[3];
                GetClientAbsOrigin(p, player_pos);

                if (GetVectorDistance(player_pos, puddle_pos, true) < ALCHEMIST_AOE_RADIUS_SQ) {
                    if (isSur(p)) {
                        addStat(p, ZFStatSpeed, ZFStatTypeCond, ALCHEMIST_ALLY_SPEED_BONUS);
                    } else if (isZom(p)) {
                        addStat(p, ZFStatSpeed, ZFStatTypeCond, ALCHEMIST_ZOMBIE_SLOW);
                    }
                }
            }
        }
    }
}

FUNCTION(AlchemistPerk, updateCondStats), char[] buffer, int maxlen) {
    int potions = zf_perkState[_inst.client];
    if (potions < ALCHEMIST_MAX_POTIONS) {
        Format(buffer, maxlen, "药剂: %d/%d | 下一瓶: %ds", potions, ALCHEMIST_MAX_POTIONS, zf_perkTimer[_inst.client]);
    } else {
        Format(buffer, maxlen, "药剂: %d/%d (已满)", potions, ALCHEMIST_MAX_POTIONS);
    }
}

FUNCTION(AlchemistPerk, onRemove)) {
    for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++) {
        if (validItem(_inst.client, i)) {
            removeItem(_inst.client, i);
        }
    }
}

FUNCTION(AlchemistPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client) {
        AlchemistPerkFonRemove(_inst);
    }
}

public void Alchemist_OnPotionCollide(int item, int other) {
    if (other == GetEntPropEnt(item, Prop_Data, "m_hOwnerEntity")) return;

    int owner = GetEntPropEnt(item, Prop_Data, "m_hOwnerEntity");
    if (!validClient(owner)) return;

    float origin[3];
    GetEntPropVector(item, Prop_Send, "m_vecOrigin", origin);

    // Create an invisible anchor entity for the puddle
    int puddle = fxCreateModelStatic("models/player/player_spy.mdl", owner, false, false, origin, view_as<float>({0.0, 0.0, 0.0}));
    SetEntityRenderMode(puddle, RENDER_NONE); // Make it invisible
    
    // Attach a more visible particle effect
    fxCreateParticle("particles/burning_fx/burning_gas_green_a.pcf", puddle, AttachBase, ALCHEMIST_AOE_DURATION);
    fxCreateSoundToAll("ambient/fluids/spill.wav", puddle);

    int item_index = getItemMetadata(item);
    zf_item[owner][item_index] = puddle;

    // Timer to remove the puddle
    CreateTimer(ALCHEMIST_AOE_DURATION, Alchemist_RemovePuddle, puddle, TIMER_FLAG_NO_MAPCHANGE);

    // Remove the thrown bottle
    AcceptEntityInput(item, "Kill");
}

public Action Alchemist_RemovePuddle(Handle timer, any puddle) {
    if (puddle != -1) {
        int owner = GetEntPropEnt(puddle, Prop_Data, "m_hOwnerEntity");
        if (validClient(owner)) {
            for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++) {
                if (zf_item[owner][i] == puddle) {
                    zf_item[owner][i] = -1;
                    break;
                }
            }
        }
        AcceptEntityInput(puddle, "Kill");
    }
    return Plugin_Stop;
}

public void Alchemist_OnPotionDestroy(int item) {
    int owner = GetEntPropEnt(item, Prop_Data, "m_hOwnerEntity");
    if (validClient(owner)) {
        int item_index = getItemMetadata(item);
        if (item_index != -1) {
            // Ensure it's not a puddle before clearing
            if (zf_item[owner][item_index] != -1 && !IsValidEntity(zf_item[owner][item_index])) {
                 zf_item[owner][item_index] = -1;
            }
        }
    }
}