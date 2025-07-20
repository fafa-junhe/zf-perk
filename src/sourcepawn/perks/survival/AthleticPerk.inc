// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/AthleticPerk.py

#if defined __AthleticPerk_included
#endinput
#endif
#define __AthleticPerk_included

#include "../../zf_perk.inc"
#include "../perk_structs.inc"
#include "SurvivalBasePerk.inc"

#define ZF_ATHLETIC_ATTACK -40
#define ZF_ATHLETIC_CRIT -100
#define ZF_ATHLETIC_ROF 100
#define ZF_ATHLETIC_SPEED 100
#define ATHLETIC_PERK_NAME "Athletic"
#define ATHLETIC_PERK_SHORTDESC "Faster movement and ROF"
#define ATHLETIC_PERK_DESC ""

methodmap AthleticPerk < SurvivalBasePerk {
    public AthleticPerk(int client) {
        AthleticPerk sm = view_as<AthleticPerk>(new SurvivalBasePerk(client));
        return sm;
    }

    public void getName(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, ATHLETIC_PERK_NAME);
    }

    public void getShortdesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, ATHLETIC_PERK_SHORTDESC);
    }

    public void getDesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, ATHLETIC_PERK_DESC);
    }

    public void onPlayerRunCmd(int buttons, int impulse, any vel, any angles, int weapon) {
    }

    public void onAmmoPickup(int pickup) {
    }

    public void onCalcIsAttackCritical() {
    }

    public void onCallForMedic() {
    }

    public void onGameFrame() {
    }

    public void onGraceEnd() {
    }

    public void onMedPickup(int pickup) {
    }

    public void onPeriodic() {
    }

    public void onPlayerDeath(int victim, int killer, int assist, int inflictor, int damagetype) {
    }

    public void onPlayerSpawn() {
    }

    public void onRemove() {
    }

    public void onSetTransmit(int entity, int client) {
    }

    public void onTakeDamage(int victim, int attacker, int inflictor, float damage, int damagetype) {
    }

    public void onTakeDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype) {
    }

    public void onTouch(int toucher, int touchee) {
    }

    public void updateClientPermStats() {
        addStat(this.getParam("owner"), ZFStatAtt, ZFStatTypePerm, ZF_ATHLETIC_ATTACK);
        addStat(this.getParam("owner"), ZFStatCrit, ZFStatTypePerm, ZF_ATHLETIC_CRIT);
        addStat(this.getParam("owner"), ZFStatRof, ZFStatTypePerm, ZF_ATHLETIC_ROF);
        addStat(this.getParam("owner"), ZFStatSpeed, ZFStatTypePerm, ZF_ATHLETIC_SPEED);
    }

    public void updateCondStats() {
    }

    public void doItemThrow(char[] model, int maxlen_model, float force, any color) {
    }

    public void onCharitableGiftTouched(int entity, int other) {
    }

}

