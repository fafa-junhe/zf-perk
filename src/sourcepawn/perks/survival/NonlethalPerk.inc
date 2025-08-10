
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

#define ZF_NONLETHAL_ATTACK_BULLET -(90)
#define ZF_NONLETHAL_FORCE 50.0

methodmap NonlethalPerk < SurvivorBasePerk {
    public NonlethalPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        NonlethalPerk sm = view_as<NonlethalPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, NonlethalPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, NonlethalPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, NonlethalPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, NonlethalPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, NonlethalPerkFonTakeDamage);

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

FUNCTION_FLOAT(NonlethalPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    // 如果攻击者不是自己，或者受害者不是僵尸，直接返回原始伤害
    if (attacker != _inst.client || !validZom(victim)) {
        return damage;
    }

    // 如果是子弹伤害，计算削弱后的伤害并返回
    if (damagetype & DMG_BULLET) {
        float modifiedDamage = damage * (1.0 + (ZF_NONLETHAL_ATTACK_BULLET / 100.0));
        return modifiedDamage;
    }

    // 如果不是子弹伤害，返回原始伤害
    return damage;
}

FUNCTION(NonlethalPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (((attacker != _inst.client)) || (!(validZom(victim)))) {
        return;
    }
    if ((damagetype & DMG_BULLET)) {
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
}

