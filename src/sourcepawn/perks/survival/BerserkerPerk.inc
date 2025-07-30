#if defined __BerserkerPerk_included
#endinput
#endif
#define __BerserkerPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"

#define BERSERKER_DEF_REDUCTION -25
#define BERSERKER_MAX_ROF_BONUS 150
#define BERSERKER_MAX_LIFESTEAL_BONUS 0.50 // 50%
#define BERSERKER_HP_THRESHOLD 0.25 // 25%

#define BERSERKER_DATA_LIFESTEAL_RATE (PERK_DATA_START_INDEX + 0)

methodmap BerserkerPerk < SurvivorBasePerk {
    public BerserkerPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        BerserkerPerk sm = view_as<BerserkerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, BerserkerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, BerserkerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, BerserkerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, BerserkerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, BerserkerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, BerserkerPerkFonDealDamagePost);

        sm.Position = view_as<DataPackPos>(BERSERKER_DATA_LIFESTEAL_RATE);
        sm.WriteFloat(0.0);

        return sm;
    }

    property float lifesteal_rate {
        public get() {
            this.Position = view_as<DataPackPos>(BERSERKER_DATA_LIFESTEAL_RATE);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(BERSERKER_DATA_LIFESTEAL_RATE);
            this.WriteFloat(value);
        }
    }
}

stock SurvivorBasePerk BerserkerPerkFnew(int client) {
    return new BerserkerPerk(client);
}

FUNCTION(BerserkerPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Berserker");
}

FUNCTION(BerserkerPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "BerserkerPerk_shortdesc");
}

FUNCTION(BerserkerPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "BerserkerPerk_desc");
}

FUNCTION(BerserkerPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, BERSERKER_DEF_REDUCTION);
}

FUNCTION(BerserkerPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;
    int health = GetClientHealth(client);
    int max_health = GetEntProp(client, Prop_Data, "m_iMaxHealth");
    if (max_health <= 0) return;

    float health_frac = float(health) / float(max_health);
    float rof_bonus = 0.0;
    float lifesteal_bonus = 0.0;

    if (health_frac <= BERSERKER_HP_THRESHOLD) {
        rof_bonus = float(BERSERKER_MAX_ROF_BONUS);
        lifesteal_bonus = BERSERKER_MAX_LIFESTEAL_BONUS;
    } else if (health_frac < 1.0) {
        // Linear interpolation between 100% hp (0 bonus) and 25% hp (max bonus)
        float scale = 1.0 - ((health_frac - BERSERKER_HP_THRESHOLD) / (1.0 - BERSERKER_HP_THRESHOLD));
        rof_bonus = float(BERSERKER_MAX_ROF_BONUS) * scale;
        lifesteal_bonus = BERSERKER_MAX_LIFESTEAL_BONUS * scale;
    }

    if (rof_bonus > 0) {
        addStat(client, ZFStatRof, ZFStatTypeCond, RoundFloat(rof_bonus));
    }
    
    _inst.lifesteal_rate = lifesteal_bonus;

    if (lifesteal_bonus > 0) {
Format(buffer, maxlen, "%t", "BerserkerPerk_Status_Active", RoundFloat(rof_bonus), RoundFloat(lifesteal_bonus * 100.0));
    } else {
        strcopy(buffer, maxlen, "");
    }
}

FUNCTION(BerserkerPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    int client = _inst.client;
    if (attacker != client || _inst.lifesteal_rate <= 0.0) {
        return;
    }

    if (damagetype & DMG_CLUB) {
        int heal_amount = RoundFloat(damage * _inst.lifesteal_rate);
        if (heal_amount > 0) {
            addHealth(client, heal_amount);
        }
    }
}
