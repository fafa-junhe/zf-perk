// Generated from D:\workspace\code\tf2\zf\src\python\perks\BasePerk.py

#if defined __BasePerk_included
#endinput
#endif
#define __BasePerk_included

#include "../zf_util_base.inc"
#include <adt_trie>

#define BASE_PERK_NAME "Unselected"
#define BASE_PERK_SHORTDESC "Unselected"
#define BASE_PERK_DESC "Please select one perk to check info"

stock StringMap BasePerk_new(int client) {
    StringMap sm = new StringMap();
    setParam(sm, "owner", client);
    setParamString(sm, "test", "orrrrr");
    return sm;
}

stock void BasePerk_getName(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_NAME);
}

stock void BasePerk_getShortdesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_SHORTDESC);
}

stock void BasePerk_getDesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, BASE_PERK_DESC);
}

stock void BasePerk_onPlayerRunCmd(StringMap self, int buttons, int impulse, any vel, any angles, int weapon) {
}

stock void BasePerk_onAmmoPickup(StringMap self, int pickup) {
}

stock void BasePerk_onCalcIsAttackCritical(StringMap self) {
}

stock void BasePerk_onCallForMedic(StringMap self) {
}

stock void BasePerk_onGameFrame(StringMap self) {
}

stock void BasePerk_onGraceEnd(StringMap self) {
}

stock void BasePerk_onMedPickup(StringMap self, int pickup) {
}

stock void BasePerk_onPeriodic(StringMap self) {
}

stock void BasePerk_onPlayerDeath(StringMap self, int victim, int killer, int assist, int inflictor, int damagetype) {
}

stock void BasePerk_onPlayerSpawn(StringMap self) {
}

stock void BasePerk_onRemove(StringMap self) {
}

stock void BasePerk_onSetTransmit(StringMap self, int entity, int client) {
}

stock void BasePerk_onTakeDamage(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void BasePerk_onTakeDamagePost(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void BasePerk_onTouch(StringMap self, int toucher, int touchee) {
}

stock void BasePerk_updateClientPermStats(StringMap self) {
}

stock void BasePerk_updateCondStats(StringMap self) {
}

stock void BasePerk_doItemThrow(StringMap self, const char[] model, float force, any color) {
}


