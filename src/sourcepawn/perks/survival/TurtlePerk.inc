#if defined __TurtlePerk_included
    #endinput
#endif
#define __TurtlePerk_included

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

#define ZF_TURTLE_ATTACK        -40
#define ZF_TURTLE_DEFEND        75
#define ZF_TURTLE_SPEED         -100
#define ZF_TURTLE_CRIT          -100
#define ZF_TURTLE_STUN_DURATION 1.0

methodmap TurtlePerk < SurvivorBasePerk
{

public     TurtlePerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        TurtlePerk       sm      = view_as<TurtlePerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, TurtlePerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, TurtlePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, TurtlePerkFonTakeDamage);

        return sm;
    }
}

stock SurvivorBasePerk TurtlePerkFnew(int client)
{
    return new TurtlePerk(client);
}

FUNCTION(TurtlePerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Turtle");
}

FUNCTION(TurtlePerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "TurtlePerk_shortdesc");
}

FUNCTION(TurtlePerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "TurtlePerk_desc");
}

FUNCTION_INT(TurtlePerk, getCategory))
{
    return 2;
}

FUNCTION(TurtlePerk, updateClientPermStats))
{
    int client = _inst.client;
    addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_TURTLE_ATTACK);
    addStat(client, ZFStatDef, ZFStatTypePerm, ZF_TURTLE_DEFEND);
    addStat(client, ZFStatSpeed, ZFStatTypePerm, ZF_TURTLE_SPEED);
    addStat(client, ZFStatCrit, ZFStatTypePerm, ZF_TURTLE_CRIT);
}

FUNCTION_FLOAT(TurtlePerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    if (victim == _inst.client && attackWasBackstab(attacker, inflictor, damage))
    {
        PrintHintText(victim, "%t", "TurtlePerk_Hint_BackstabBlocked");
        TF2_StunPlayer(attacker, ZF_TURTLE_STUN_DURATION, _, 0, attacker);
        fxSpark(victim);
        return 0.0;
    }
    return damage;
}