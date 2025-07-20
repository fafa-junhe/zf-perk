// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/survival/SurvivalBasePerk.py

#if defined __SurvivalBasePerk_included
#endinput
#endif
#define __SurvivalBasePerk_included

#include "../BasePerk.inc"

#define SURVIVAL_BASE_PERK_NAME "Unselected"
#define SURVIVAL_BASE_PERK_SHORTDESC "Unselected"
#define SURVIVAL_BASE_PERK_DESC "Please select one perk to check info"

methodmap SurvivalBasePerk < BasePerk {
    public SurvivalBasePerk(int client) {
        SurvivalBasePerk sm = view_as<SurvivalBasePerk>(new BasePerk(client));
        return sm;
    }

    public void getName(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, SURVIVAL_BASE_PERK_NAME);
    }

    public void getShortdesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, SURVIVAL_BASE_PERK_SHORTDESC);
    }

    public void getDesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, SURVIVAL_BASE_PERK_DESC);
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
    }

    public void updateCondStats() {
    }

    public void doItemThrow(char[] model, int maxlen_model, float force, any color) {
    }

    public void onCharitableGiftTouched(int entity, int other) {
    }

}

