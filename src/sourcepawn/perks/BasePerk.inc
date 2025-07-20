// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/BasePerk.py

#if defined __BasePerk_included
#endinput
#endif
#define __BasePerk_included

#include <adt_trie>

#define BASE_PERK_NAME "Unselected"
#define BASE_PERK_SHORTDESC "Unselected"
#define BASE_PERK_DESC "Please select one perk to check info"

methodmap BasePerk < StringMap {
    public BasePerk(int client) {
        BasePerk sm = view_as<BasePerk>(new StringMap());
        sm.setParam("owner", client);
        return sm;
    }

    public int getParam(const char[] key) {
        int value;
        this.GetValue(key, value);
        return value;
    }

    public void setParam(const char[] key, int value) {
        this.SetValue(key, value);
    }

    public void getParamString(const char[] key, char[] buffer, int maxlen) {
        this.GetString(key, buffer, maxlen);
    }

    public void setParamString(const char[] key, const char[] value) {
        this.SetString(key, value);
    }

    public void getName(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, BASE_PERK_NAME);
    }

    public void getShortdesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, BASE_PERK_SHORTDESC);
    }

    public void getDesc(char[] buffer, int maxlen) {
        strcopy(buffer, maxlen, BASE_PERK_DESC);
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

}

