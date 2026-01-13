
#if defined __NonlethalPerk_included
#endinput
#endif
#define __NonlethalPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/sdktools_functions.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/vector.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_NONLETHAL_ATTACK -(50)
#define ZF_NONLETHAL_FORCE 400.0

methodmap NonlethalPerk < SurvivorBasePerk {
    public NonlethalPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        NonlethalPerk sm = view_as<NonlethalPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, NonlethalPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, NonlethalPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, NonlethalPerkFupdateClientPermStats);

        return sm;
    }
}

stock SurvivorBasePerk NonlethalPerkFnew(int client) {
    return new NonlethalPerk(client);
}


FUNCTION(NonlethalPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Nonlethal");
}

FUNCTION(NonlethalPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "NonlethalPerk_shortdesc");
}

FUNCTION(NonlethalPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "NonlethalPerk_desc");
}

FUNCTION_INT(NonlethalPerk, getCategory))
{
    return 2;
}

FUNCTION(NonlethalPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (((attacker != _inst.client)) || (!(validZom(victim)))) {
        return;
    }
    float attacker_pos[3];
    GetClientAbsOrigin(attacker, attacker_pos);
    float victim_pos[3];
    GetClientAbsOrigin(victim, victim_pos);
    float knockback_dir[3];
    SubtractVectors(victim_pos, attacker_pos, knockback_dir);
    NormalizeVector(knockback_dir, knockback_dir);
    float force = (ZF_NONLETHAL_FORCE * damage);
    float current_velocity[3];
    GetEntPropVector(victim, Prop_Data, "m_vecVelocity", current_velocity);
    float knockback_velocity[3];
    ScaleVector(knockback_dir, force);
    AddVectors(current_velocity, knockback_dir, knockback_velocity);
    TeleportEntity(victim, NULL_VECTOR, NULL_VECTOR, knockback_velocity);
}

FUNCTION(NonlethalPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_NONLETHAL_ATTACK);
}