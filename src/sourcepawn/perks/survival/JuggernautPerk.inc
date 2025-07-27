
#if defined __JuggernautPerk_included
#endinput
#endif
#define __JuggernautPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sdkhooks.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sdktools_sound.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/vector.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_JUGGERNAUT_ATTACK 50
#define ZF_JUGGERNAUT_DEFEND -(50)
#define ZF_JUGGERNAUT_SPEED -(100)
#define ZF_JUGGERNAUT_FORCE 500.0
#define ZF_JUGGERNAUT_RADIUS 150.0
#define ZF_JUGGERNAUT_STUN_DURATION 1
#define ZF_JUGGERNAUT_STUN_SLOWDOWN -(100)

methodmap JuggernautPerk < SurvivorBasePerk {
    public JuggernautPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        JuggernautPerk sm = view_as<JuggernautPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, JuggernautPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, JuggernautPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, JuggernautPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, JuggernautPerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, JuggernautPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, JuggernautPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk JuggernautPerkFnew(int client) {
    return new JuggernautPerk(client);
}


FUNCTION(JuggernautPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Juggernaut");
}

FUNCTION(JuggernautPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "High attack, low speed");
}

FUNCTION(JuggernautPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Your attack power is greatly increased, but your defense and movement speed are reduced.\nYou are immune to fall damage.\nIf you land near a zombie, the zombie will take slight damage, and be knocked back and stunned.");
}

FUNCTION(JuggernautPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_JUGGERNAUT_ATTACK);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_JUGGERNAUT_DEFEND);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, ZF_JUGGERNAUT_SPEED);
}

FUNCTION_FLOAT(JuggernautPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if ((victim != _inst.client)) {
        return damage;
    }
    if ((attacker == _inst.client)) {
        return 0.0;
    }
    if ((damagetype & DMG_FALL)) {
        float client_pos[3] = {0.0, 0.0, 0.0};
        GetEntPropVector(_inst.client, Prop_Send, "m_vecOrigin", client_pos);
        fxPuffBig(_inst.client);
        for (int i = 1; i < (MAXPLAYERS + 1); i += 1)
        {
            if ((validClient(i)) && (IsPlayerAlive(i)) && (isZom(i))) {
                float target_pos[3] = {0.0, 0.0, 0.0};
                GetEntPropVector(i, Prop_Send, "m_vecOrigin", target_pos);
                if ((GetVectorDistance(client_pos, target_pos) <= ZF_JUGGERNAUT_RADIUS)) {
                    SDKHooks_TakeDamage(i, _inst.client, _inst.client, 1.0);
                    addStatTempExtend(i, ZFStatSpeed, ZF_JUGGERNAUT_STUN_SLOWDOWN, ZF_JUGGERNAUT_STUN_DURATION);
                    float knockback_dir[3] = {0.0, 0.0, 0.0};
                    ScaleVector(client_pos, -(1.0));
                    AddVectors(target_pos, client_pos, knockback_dir);
                    NormalizeVector(knockback_dir, knockback_dir);
                    float current_velocity[3] = {0.0, 0.0, 0.0};
                    GetEntPropVector(i, Prop_Data, "m_vecVelocity", current_velocity);
                    float knockback_velocity[3] = {0.0, 0.0, 0.0};
                    ScaleVector(knockback_dir, ZF_JUGGERNAUT_FORCE);
                    AddVectors(current_velocity, knockback_dir, knockback_velocity);
                    TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, knockback_velocity);
                }
            }
        }
        return 0.0;
    }
    return damage;
}


FUNCTION(JuggernautPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (((attacker != _inst.client)) || (!(validZom(victim)))) {
        return;
    }
    if ((damagetype & DMG_CLUB)) {
        float attacker_pos[3] = {0.0, 0.0, 0.0};
        GetEntPropVector(attacker, Prop_Send, "m_vecOrigin", attacker_pos);
        float victim_pos[3] = {0.0, 0.0, 0.0};
        GetEntPropVector(victim, Prop_Send, "m_vecOrigin", victim_pos);
        ScaleVector(attacker_pos, -(1.0));
        float result_pos[3] = {0.0, 0.0, 0.0};
        AddVectors(victim_pos, attacker_pos, result_pos);
        NormalizeVector(result_pos, result_pos);
        float current_velocity[3] = {0.0, 0.0, 0.0};
        GetEntPropVector(victim, Prop_Data, "m_vecVelocity", current_velocity);
        ScaleVector(result_pos, ZF_JUGGERNAUT_FORCE);
        float knockback_velocity[3] = {0.0, 0.0, 0.0};
        AddVectors(current_velocity, result_pos, knockback_velocity);
        TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, knockback_velocity);
    }
}

