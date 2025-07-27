#if defined __LeaderPerk_included
#endinput
#endif
#define __LeaderPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_LEADER_SELF_CRIT 15
#define ZF_LEADER_OTHERS_ATTACK 15
#define ZF_LEADER_OTHERS_RADIUSSQ (350 * 350)
#define ZF_LEADER_RALLY_SELF_ATTACK 5
#define ZF_LEADER_RALLY_SELF_DEFEND 5
#define ZF_LEADER_RALLY_OTHERS_ATTACK 15
#define ZF_LEADER_RALLY_OTHERS_DEFEND 15
#define ZF_LEADER_RALLY_DURATION 90
#define ZF_LEADER_RALLY_COOLDOWN 150
#define ZF_LEADER_RALLY_RADIUSSQ (400 * 400)

methodmap LeaderPerk < SurvivorBasePerk {
    public LeaderPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        LeaderPerk sm = view_as<LeaderPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, LeaderPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, LeaderPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, LeaderPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, LeaderPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, LeaderPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, LeaderPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, LeaderPerkFonPlayerDeath);
        // TODO: PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_EFFECTS, LeaderPerkFupdateClientPermEffects);

        return sm;
    }
}

stock SurvivorBasePerk LeaderPerkFnew(int client) {
    return new LeaderPerk(client);
}

FUNCTION(LeaderPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Leader");
}

FUNCTION(LeaderPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Place a buff banner");
}

FUNCTION(LeaderPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "You have a crit bonus and provide a passive attack bonus to nearby survivors. Call for a medic to place a banner that gives attack and defense to nearby survivors. Banner lasts 90s, 150s cooldown.");
}

FUNCTION(LeaderPerk, updateClientPermEffects)) {
    createAura(_inst.client, ZFPART_AURAINRED, AttachBase);
}

FUNCTION(LeaderPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatCrit, ZFStatTypePerm, ZF_LEADER_SELF_CRIT);
}

FUNCTION(LeaderPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;
    float client_pos[3];
    GetClientAbsOrigin(client, client_pos);

    // Passive aura
    for (int i = 1; i <= MaxClients; i++) {
        if (validLivingSur(i) && i != client) {
            float target_pos[3];
            GetClientAbsOrigin(i, target_pos);
            if (GetVectorDistance(client_pos, target_pos, true) <= ZF_LEADER_OTHERS_RADIUSSQ) {
                addStat(i, ZFStatAtt, ZFStatTypeCond, ZF_LEADER_OTHERS_ATTACK);
            }
        }
    }

    // Rally banner
    int rallyBanner = zf_item[client][0];
    if (rallyBanner > 0 && IsValidEntity(rallyBanner)) {
        if (zf_perkTimer[client] > 0 && zf_perkTimer[client] <= (ZF_LEADER_RALLY_COOLDOWN - ZF_LEADER_RALLY_DURATION)) {
             removeItem(client, 0);
        } else {
            float rally_pos[3];
            GetEntPropVector(rallyBanner, Prop_Send, "m_vecOrigin", rally_pos);
            int allies_near_rally = 0;

            for (int i = 1; i <= MaxClients; i++) {
                if (validLivingSur(i)) {
                    float target_pos[3];
                    GetClientAbsOrigin(i, target_pos);
                    if (GetVectorDistance(rally_pos, target_pos, true) <= ZF_LEADER_RALLY_RADIUSSQ) {
                        if (i != client) {
                            allies_near_rally++;
                        }
                        addStat(i, ZFStatAtt, ZFStatTypeCond, ZF_LEADER_RALLY_OTHERS_ATTACK);
                        addStat(i, ZFStatDef, ZFStatTypeCond, ZF_LEADER_RALLY_OTHERS_DEFEND);
                    }
                }
            }
            addStat(client, ZFStatAtt, ZFStatTypeCond, ZF_LEADER_RALLY_SELF_ATTACK * allies_near_rally);
            addStat(client, ZFStatDef, ZFStatTypeCond, ZF_LEADER_RALLY_SELF_DEFEND * allies_near_rally);
        }
    }
    
    if (zf_perkTimer[client] > 0) {
        Format(buffer, maxlen, "(Cooldown: %ds)", zf_perkTimer[client]);
    } else {
        Format(buffer, maxlen, "(Ready to Rally)");
    }
}

FUNCTION(LeaderPerk, onCallForMedic)) {
    int client = _inst.client;
    if (zf_perkTimer[client] > 0) {
        return;
    }
    if (!isGrounded(client) || !isCrouching(client)) {
        return;
    }

    float pos[3], ang[3];
    GetClientAbsOrigin(client, pos);
    GetClientEyeAngles(client, ang);
    ang[0] = 0.0;
    ang[2] = 0.0;

    int entity = fxCreateModelStatic(ZFMDL_FLAG, client, true, true, pos, ang);
    if (entity != -1) {
        zf_item[client][0] = entity;
        zf_perkTimer[client] = ZF_LEADER_RALLY_COOLDOWN;
    }
}

FUNCTION(LeaderPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client) {
        removeItem(_inst.client, 0);
    }
}
