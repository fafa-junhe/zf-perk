#if defined __WisePerk_included
#endinput
#endif
#define __WisePerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_WISE_ATTACK_KILL 3
#define ZF_WISE_ATTACK_ASSIST 2
#define ZF_WISE_DEFEND -1
#define ZF_WISE_DEFEND_LIMIT 20
#define ZF_WISE_ATTACK_DEBUFF -10
#define ZF_WISE_DEFEND_DEBUFF 10

methodmap WisePerk < SurvivorBasePerk {
    public WisePerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        WisePerk sm = view_as<WisePerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, WisePerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, WisePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, WisePerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, WisePerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, WisePerkFonAssistKill);

        return sm;
    }
}

stock SurvivorBasePerk WisePerkFnew(int client) {
    return new WisePerk(client);
}

FUNCTION(WisePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Wise");
}

FUNCTION(WisePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "WisePerk_shortdesc");
}

FUNCTION(WisePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "WisePerk_desc");
}

FUNCTION_INT(WisePerk, getCategory))
{
    return 5;
}

FUNCTION(WisePerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_WISE_ATTACK_DEBUFF);
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_WISE_DEFEND_DEBUFF);
}

FUNCTION_FLOAT(WisePerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    int client = _inst.client;
    if (victim == client && validZom(attacker) && attackWasMelee(attacker, inflictor, damagetype)) {
        if (getStat(client, ZFStatDef) < ZF_WISE_DEFEND_LIMIT) {
            addStat(client, ZFStatDef, ZFStatTypePerm, ZF_WISE_DEFEND);
        }
    }
    return damage;
}

FUNCTION(WisePerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    if (validZom(victim)) {
        addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_WISE_ATTACK_KILL);
    }
}

FUNCTION(WisePerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    if (validZom(victim)) {
        addStat(client, ZFStatAtt, ZFStatTypePerm, ZF_WISE_ATTACK_ASSIST);
    }
}