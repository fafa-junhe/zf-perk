#if defined __CharitablePerk_included
#endinput
#endif
#define __CharitablePerk_included

#include "../../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_CHARITABLE_MAX_ITEMS 5
#define ZF_CHARITABLE_POINTS_ASSIST 2
#define ZF_CHARITABLE_POINTS_KILL 2
#define ZF_CHARITABLE_POINTS_GIFT 4
#define ZF_CHARITABLE_GIFT_BONUS_HEALTH 75
#define ZF_CHARITABLE_GIFT_BONUS_MIN 10
#define ZF_CHARITABLE_GIFT_BONUS_MAX 30
#define ZF_CHARITABLE_GIFT_DURATION 20

methodmap CharitablePerk < SurvivorBasePerk {
    public CharitablePerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        CharitablePerk sm = view_as<CharitablePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, CharitablePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, CharitablePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, CharitablePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, CharitablePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, CharitablePerk_onKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, CharitablePerk_onAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, CharitablePerkFonCallForMedic);

        return sm;
    }
}

stock SurvivorBasePerk CharitablePerkFnew(int client) {
    return new CharitablePerk(client);
}

FUNCTION(CharitablePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Charitable");
}

FUNCTION(CharitablePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CharitablePerk_shortdesc");
}

FUNCTION(CharitablePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "CharitablePerk_desc");
}

FUNCTION(CharitablePerk, updateCondStats), char[] buffer, int maxlen) {
    int gifts = zf_perkState[_inst.client] / ZF_CHARITABLE_POINTS_GIFT;
Format(buffer, maxlen, "%t", "CharitablePerk_Status_Gifts", gifts);
}

public void CharitablePerk_onKill(CharitablePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    zf_perkState[client] += ZF_CHARITABLE_POINTS_KILL;
}

public void CharitablePerk_onAssistKill(CharitablePerk _inst, int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    zf_perkState[client] += ZF_CHARITABLE_POINTS_ASSIST;
}

FUNCTION(CharitablePerk, onCallForMedic)) {
    int client = _inst.client;
    if (zf_perkState[client] >= ZF_CHARITABLE_POINTS_GIFT) {
        int itemIndex = getFreeItemIndex(client, ZF_CHARITABLE_MAX_ITEMS);
        if (itemIndex != -1) {
            zf_perkState[client] -= ZF_CHARITABLE_POINTS_GIFT;
            
            int entity = doItemThrow(client, ZFMDL_PRESENT[GetRandomInt(0, 2)], 800.0);
            if (entity != -1) {
                zf_item[client][itemIndex] = entity;
                SDKHook(entity, SDKHook_Touch, perk_OnCharitableGiftTouched);
            }
        }
    }
}
