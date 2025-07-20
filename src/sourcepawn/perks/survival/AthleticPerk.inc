// Generated from D:\workspace\code\tf2\zf\src\python\perks\survival\AthleticPerk.py

#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <adt_trie>

#define ZF_ATHLETIC_ATTACK -40
#define ZF_ATHLETIC_CRIT -100
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100
#define ATHLETIC_PERK_NAME "Athletic"
#define ATHLETIC_PERK_SHORTDESC "Faster movement and ROF"
#define ATHLETIC_PERK_DESC "Faster movement and ROF!!"

stock StringMap AthleticPerk_new(int client) {
    StringMap sm = SurvivorBasePerk_new(client);
    setParamString(sm, "test", "okkk");
    return sm;
}

stock void AthleticPerk_getName(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_NAME);
}

stock void AthleticPerk_getShortdesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_SHORTDESC);
}

stock void AthleticPerk_getDesc(char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, ATHLETIC_PERK_DESC);
}

stock void AthleticPerk_onPlayerRunCmd(StringMap self, int buttons, int impulse, any vel, any angles, int weapon) {
}

stock void AthleticPerk_onAmmoPickup(StringMap self, int pickup) {
}

stock void AthleticPerk_onCalcIsAttackCritical(StringMap self) {
}

stock void AthleticPerk_onCallForMedic(StringMap self) {
}

stock void AthleticPerk_onGameFrame(StringMap self) {
}

stock void AthleticPerk_onGraceEnd(StringMap self) {
}

stock void AthleticPerk_onMedPickup(StringMap self, int pickup) {
}

stock void AthleticPerk_onPeriodic(StringMap self) {
}

stock void AthleticPerk_onPlayerDeath(StringMap self, int victim, int killer, int assist, int inflictor, int damagetype) {
}

stock void AthleticPerk_onPlayerSpawn(StringMap self) {
}

stock void AthleticPerk_onRemove(StringMap self) {
}

stock void AthleticPerk_onSetTransmit(StringMap self, int entity, int client) {
}

stock void AthleticPerk_onTakeDamage(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void AthleticPerk_onTakeDamagePost(StringMap self, int victim, int attacker, int inflictor, float damage, int damagetype) {
}

stock void AthleticPerk_onTouch(StringMap self, int toucher, int touchee) {
}

stock void AthleticPerk_updateClientPermStats(StringMap self) {
    addStat(getParam(self, "owner"), ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
    addStat(getParam(self, "owner"), ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
    addStat(getParam(self, "owner"), ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
    addStat(getParam(self, "owner"), ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
}

stock void AthleticPerk_updateCondStats(StringMap self) {
}

stock void AthleticPerk_doItemThrow(StringMap self, const char[] model, float force, any color) {
}

stock void AthleticPerk_onCharitableGiftTouched(StringMap self, int entity, int other) {
}


