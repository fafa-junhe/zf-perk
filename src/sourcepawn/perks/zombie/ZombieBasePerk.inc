// Generated from D:\workspace\code\tf2\zf\src\python\perks\zombie\ZombieBasePerk.py

#if defined __ZombieBasePerk_included
#endinput
#endif
#define __ZombieBasePerk_included

#include "../../zf_util_base.inc"
#include "../BasePerk.inc"
#include <adt_trie>

#define ZOMBIE_BASE_PERK_NAME "ZombieUnselected"
#define ZOMBIE_BASE_PERK_SHORTDESC "Unselected"
#define ZOMBIE_BASE_PERK_DESC "Please select one perk to check info"

stock StringMap ZombieBasePerk_new(int client) {
    StringMap sm = BasePerk_new(client);
    setParamString(sm, "test", "zooooooo");
    return sm;
}

stock void ZombieBasePerk_getName(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ZOMBIE_BASE_PERK_NAME);
}

stock void ZombieBasePerk_getShortdesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ZOMBIE_BASE_PERK_SHORTDESC);
}

stock void ZombieBasePerk_getDesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ZOMBIE_BASE_PERK_DESC);
}

stock void ZombieBasePerk_onPlayerRunCmd(StringMap self, int buttons, int impulse, any vel, any angles, int weapon) {
}

stock void ZombieBasePerk_onAmmoPickup(StringMap self, int pickup) {
}

stock void ZombieBasePerk_onCalcIsAttackCritical(StringMap self) {
}

stock void ZombieBasePerk_onCallForMedic(StringMap self) {
}

stock void ZombieBasePerk_onGameFrame(StringMap self) {
}

stock void ZombieBasePerk_onGraceEnd(StringMap self) {
}

stock void ZombieBasePerk_onMedPickup(StringMap self, int pickup) {
}

stock void ZombieBasePerk_onPeriodic(StringMap self) {
}

stock void ZombieBasePerk_onPlayerDeath(StringMap self, int victim, int killer, int assist, int inflictor, int damagetype) {
}

stock void ZombieBasePerk_onPlayerSpawn(StringMap self) {
}

stock void ZombieBasePerk_onRemove(StringMap self) {
}

stock void ZombieBasePerk_onSetTransmit(StringMap self, int entity, int client) {
}

stock void ZombieBasePerk_onTakeDamage(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void ZombieBasePerk_onTakeDamagePost(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void ZombieBasePerk_onTouch(StringMap self, int toucher, int touchee) {
}

stock void ZombieBasePerk_updateClientPermStats(StringMap self) {
}

stock void ZombieBasePerk_updateCondStats(StringMap self) {
}

stock void ZombieBasePerk_doItemThrow(StringMap self, const char[] model, float force, any color) {
}


