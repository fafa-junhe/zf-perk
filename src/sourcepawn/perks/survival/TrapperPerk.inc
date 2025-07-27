#if defined __TrapperPerk_included
#endinput
#endif
#define __TrapperPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_TRAPPER_MAX_ITEMS 5
#define ZF_TRAPPER_DAMAGE 200
#define ZF_TRAPPER_RADIUS 150
#define ZF_TRAPPER_RADIUSSQ (200 * 200)
#define ZF_TRAPPER_TIMER 20

methodmap TrapperPerk < SurvivorBasePerk {
    public TrapperPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        TrapperPerk sm = view_as<TrapperPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, TrapperPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, TrapperPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, TrapperPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, TrapperPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, TrapperPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, TrapperPerkFonPlayerDeath);

        return sm;
    }
}

stock SurvivorBasePerk TrapperPerkFnew(int client) {
    return new TrapperPerk(client);
}

FUNCTION(TrapperPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Trapper");
}

FUNCTION(TrapperPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Place mines");
}

FUNCTION(TrapperPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Call for a medic to place a mine that explodes when a zombie gets near. Max 5 mines. 20s cooldown.");
}

FUNCTION(TrapperPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;

    for (int i = 0; i < ZF_TRAPPER_MAX_ITEMS; i++) {
        int mine = zf_item[client][i];
        if (mine > 0 && IsValidEntity(mine)) {
            float mine_pos[3];
            GetEntPropVector(mine, Prop_Send, "m_vecOrigin", mine_pos);
            bool triggered = false;

            for (int j = 1; j <= MaxClients; j++) {
                if (validLivingZom(j)) {
                    float zom_pos[3];
                    GetClientAbsOrigin(j, zom_pos);
                    if (GetVectorDistance(mine_pos, zom_pos, true) <= ZF_TRAPPER_RADIUSSQ) {
                        if (isMagnetic(j)) {
                            fxSpark(mine);
                            removeItem(client, i);
                            triggered = true;
                            break;
                        }

                        TF2_IgnitePlayer(j, client);
                        applyDamageRadial(client, ZF_TRAPPER_DAMAGE, mine_pos, ZF_TRAPPER_RADIUS, true);
                        removeItem(client, i);
                        triggered = true;
                        break;
                    }
                }
            }
            if (!triggered) {
                // Play idle sound? The doc mentions ZFSND_TICK.
            }
        }
    }

    if (zf_perkTimer[client] > 0) {
        Format(buffer, maxlen, "(Cooldown: %ds)", zf_perkTimer[client]);
    } else {
        if (getFreeItemIndex(client, ZF_TRAPPER_MAX_ITEMS) == -1) {
            Format(buffer, maxlen, "(Mines Full)");
        } else {
            Format(buffer, maxlen, "(Ready to Trap)");
        }
    }
}

FUNCTION(TrapperPerk, onCallForMedic)) {
    int client = _inst.client;

    if (zf_perkTimer[client] > 0) {
        return;
    }
    if (!isGrounded(client) || !isCrouching(client)) {
        return;
    }
    int itemIndex = getFreeItemIndex(client, ZF_TRAPPER_MAX_ITEMS);
    if (itemIndex == -1) {
        return;
    }

    float pos[3], ang[3];
    GetClientEyePosition(client, pos);
    GetClientEyeAngles(client, ang);
    
    float fwd[3];
    GetAngleVectors(ang, fwd, NULL_VECTOR, NULL_VECTOR);
    pos[0] += fwd[0] * 30.0;
    pos[1] += fwd[1] * 30.0;
    pos[2] -= 30.0; // Place it on the ground
    ang[0] = 90.0; // Lay flat

    int entity = fxCreateModelStatic(ZFMDL_MINE, client, true, true, pos, ang);
    if (entity != -1) {
        zf_item[client][itemIndex] = entity;
        zf_perkTimer[client] = ZF_TRAPPER_TIMER;
        EmitSoundToClient(client, "vo/engineer_mvm_build_sentry_01.mp3");
    }
}

FUNCTION(TrapperPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client) {
        removeItems(_inst.client);
    }
}

stock bool isMagnetic(int client) {
    if (!validClient(client) || g_hPerks[client] == null) {
        return false;
    }
    char perkName[32];
    g_hPerks[client].getName(perkName, sizeof(perkName));
    return StrEqual(perkName, "Magnetic");
}