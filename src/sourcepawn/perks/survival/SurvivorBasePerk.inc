// Generated from D:\workspace\code\tf2\zf\src\python\perks\survival\SurvivorBasePerk.py

#if defined __SurvivorBasePerk_included
#endinput
#endif
#define __SurvivorBasePerk_included

#include "../../zf_util_base.inc"
#include "../BasePerk.inc"
#include <adt_trie>

#define SURVIVOR_BASE_PERK_NAME "SurvivorUnselected"
#define SURVIVOR_BASE_PERK_SHORTDESC "Unselected"
#define SURVIVOR_BASE_PERK_DESC "Please select one perk to check info"

stock StringMap SurvivorBasePerk_new(int client) {
    StringMap sm = BasePerk_new(client);
    setParamString(sm, "test", "nooooo");
    return sm;
}

stock void SurvivorBasePerk_getName(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, SURVIVOR_BASE_PERK_NAME);
}

stock void SurvivorBasePerk_getShortdesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, SURVIVOR_BASE_PERK_SHORTDESC);
}

stock void SurvivorBasePerk_getDesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, SURVIVOR_BASE_PERK_DESC);
}

stock void SurvivorBasePerk_onPlayerRunCmd(StringMap self, int buttons, int impulse, any vel, any angles, int weapon) {
}

stock void SurvivorBasePerk_onAmmoPickup(StringMap self, int pickup) {
}

stock void SurvivorBasePerk_onCalcIsAttackCritical(StringMap self) {
}

stock void SurvivorBasePerk_onCallForMedic(StringMap self) {
}

stock void SurvivorBasePerk_onGameFrame(StringMap self) {
}

stock void SurvivorBasePerk_onGraceEnd(StringMap self) {
}

stock void SurvivorBasePerk_onMedPickup(StringMap self, int pickup) {
}

stock void SurvivorBasePerk_onPeriodic(StringMap self) {
}

stock void SurvivorBasePerk_onPlayerDeath(StringMap self, int victim, int killer, int assist, int inflictor, int damagetype) {
}

stock void SurvivorBasePerk_onPlayerSpawn(StringMap self) {
}

stock void SurvivorBasePerk_onRemove(StringMap self) {
}

stock void SurvivorBasePerk_onSetTransmit(StringMap self, int entity, int client) {
}

stock void SurvivorBasePerk_onTakeDamage(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void SurvivorBasePerk_onTakeDamagePost(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void SurvivorBasePerk_onTouch(StringMap self, int toucher, int touchee) {
}

stock void SurvivorBasePerk_updateClientPermStats(StringMap self) {
}

stock void SurvivorBasePerk_updateCondStats(StringMap self) {
}

stock void SurvivorBasePerk_doItemThrow(StringMap self, const char[] model, float force, any color) {
}

stock void SurvivorBasePerk_onCharitableGiftTouched(StringMap self, int entity, int other) {
}


