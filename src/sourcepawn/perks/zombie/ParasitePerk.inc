#if defined __ParasitePerk_included
#endinput
#endif
#define __ParasitePerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include <tf2_stocks>

// --- Defines from new_perks.md ---
#define ZF_PARASITE_HEALTH_PENALTY -50
#define ZF_PARASITE_LIFESTEAL_PER_HIT 15.0
#define ZF_PARASITE_UBER_STEAL_PER_HIT 0.10  // Steal 10% of current Uber per hit
#define ZF_PARASITE_AMMO_STEAL_PERCENT 0.10 // Steal 10% of current clip ammo
#define ZF_PARASITE_UBER_TO_HEALTH 1.0 // 1% uber = 1 health
#define ZF_PARASITE_AMMO_TO_HEALTH 0.5 // 2 ammo = 1 health
#define ZF_PARASITE_ENERGY_PER_HIT 10
#define ZF_PARASITE_ENERGY_FOR_BURST 100.0
#define ZF_PARASITE_BURST_DURATION 10
#define ZF_PARASITE_WEAKNESS_DURATION 5
#define ZF_PARASITE_BURST_SPEED 75
#define ZF_PARASITE_BURST_ATTACK 100
#define ZF_PARASITE_WEAKNESS_SPEED -25
#define ZF_PARASITE_WEAKNESS_ATTACK -50

// --- Data Slots ---
#define PARASITE_DATA_ENERGY            (PERK_DATA_START_INDEX + 0)
#define PARASITE_DATA_BURST_TIMER       (PERK_DATA_START_INDEX + 1)
#define PARASITE_DATA_WEAKNESS_TIMER    (PERK_DATA_START_INDEX + 2)

methodmap ParasitePerk < ZombieBasePerk {
    public ParasitePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ParasitePerk sm = view_as<ParasitePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ParasitePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ParasitePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ParasitePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, ParasitePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, ParasitePerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, ParasitePerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, ParasitePerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, ParasitePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, ParasitePerkFonPlayerSpawn);

        sm.energy = 0;
        sm.burst_timer = 0;
        sm.weakness_timer = 0;

        return sm;
    }

    property int energy {
        public get() {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_ENERGY);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_ENERGY);
            this.WriteCell(value);
        }
    }

    property int burst_timer {
        public get() {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_BURST_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_BURST_TIMER);
            this.WriteCell(value);
        }
    }

    property int weakness_timer {
        public get() {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_WEAKNESS_TIMER);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(PARASITE_DATA_WEAKNESS_TIMER);
            this.WriteCell(value);
        }
    }
}

stock BasePerk ParasitePerkFnew(int client) {
    return new ParasitePerk(client);
}

FUNCTION(ParasitePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Parasite");
}

FUNCTION(ParasitePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ParasitePerk_shortdesc");
}

FUNCTION(ParasitePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ParasitePerk_desc");
}

FUNCTION(ParasitePerk, onPlayerSpawn)) {
    _inst.energy = 0;
    _inst.burst_timer = 0;
    _inst.weakness_timer = 0;
}

FUNCTION(ParasitePerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, ZF_PARASITE_HEALTH_PENALTY);
}

FUNCTION(ParasitePerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker == _inst.client && isSur(victim) && (damagetype & DMG_CLUB)) {
        // Lifesteal
        addHealth(attacker, RoundFloat(ZF_PARASITE_LIFESTEAL_PER_HIT));

        // Gain energy
        _inst.energy = view_as<int>(fMin(ZF_PARASITE_ENERGY_FOR_BURST, float(_inst.energy) + ZF_PARASITE_ENERGY_PER_HIT));

        // --- Resource Stealing ---
        int total_health_gain = 0;
        bool stole_ammo = false;
        bool stole_uber = false;

        // Steal Ammo from active weapon
        int active_weapon = GetEntDataEnt2(victim, FindSendPropInfo("CTFPlayer", "m_hActiveWeapon"));
        if (active_weapon != -1)
        {
            int clip = GetEntData(active_weapon, FindSendPropInfo("CBaseCombatWeapon", "m_iClip1"));
            if (clip > 0)
            {
                int stolen_ammo = RoundFloat(clip * ZF_PARASITE_AMMO_STEAL_PERCENT);
                if (stolen_ammo > 0)
                {
                    SetEntData(active_weapon, FindSendPropInfo("CBaseCombatWeapon", "m_iClip1"), clip - stolen_ammo);
                    total_health_gain += RoundFloat(stolen_ammo * ZF_PARASITE_AMMO_TO_HEALTH);
                    stole_ammo = true;
                }
            }
        }

        // Steal Uber if victim is a Medic
        if (TF2_GetPlayerClass(victim) == TFClass_Medic) {
            int medigun = GetPlayerWeaponSlot(victim, TFWeaponSlot_Secondary);
            if (medigun != -1)
            {
                char classname[64];
                GetEntityClassname(medigun, classname, sizeof(classname));

                // Check if the weapon is a medigun
                if (StrContains(classname, "tf_weapon_medigun", false) != -1 ||
                    strcmp(classname, "tf_weapon_kritzkrieg") == 0 ||
                    strcmp(classname, "tf_weapon_quickfix") == 0 ||
                    strcmp(classname, "tf_weapon_vaccinator") == 0)
                {
                    float current_uber = GetEntPropFloat(medigun, Prop_Send, "m_flChargeLevel");
                    if (current_uber > 0.0)
                    {
                        float stolen_uber_charge = current_uber * ZF_PARASITE_UBER_STEAL_PER_HIT;
                        SetEntPropFloat(medigun, Prop_Send, "m_flChargeLevel", current_uber - stolen_uber_charge);
                        total_health_gain += RoundFloat(stolen_uber_charge * 100.0 * ZF_PARASITE_UBER_TO_HEALTH);
                        stole_uber = true;
                    }
                }
            }
        }

        // Apply stolen health and give feedback
        if (total_health_gain > 0) {
            addHealth(attacker, total_health_gain);
            if (stole_ammo && stole_uber) {
                PrintHintText(attacker, "窃取了弹药和Uber, 转化了 %d 生命!", total_health_gain);
            } else if (stole_ammo) {
                PrintHintText(attacker, "窃取了弹药, 转化了 %d 生命!", total_health_gain);
            } else if (stole_uber) {
                PrintHintText(attacker, "窃取了Uber, 转化了 %d 生命!", total_health_gain);
            }
        }
    }
}

FUNCTION(ParasitePerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.energy >= ZF_PARASITE_ENERGY_FOR_BURST && _inst.burst_timer == 0 && _inst.weakness_timer == 0) {
        _inst.energy = 0;
        _inst.burst_timer = ZF_PARASITE_BURST_DURATION;

        addStatTempStack(client, ZFStatSpeed, ZF_PARASITE_BURST_SPEED, ZF_PARASITE_BURST_DURATION);
        addStatTempStack(client, ZFStatAtt, ZF_PARASITE_BURST_ATTACK, ZF_PARASITE_BURST_DURATION);

        PrintHintText(client, "超能爆发!");
    }
}

FUNCTION(ParasitePerk, onPeriodic)) {
    int client = _inst.client;
    if (_inst.burst_timer > 0) {
        _inst.burst_timer--;
        if (_inst.burst_timer == 0) {
            _inst.weakness_timer = ZF_PARASITE_WEAKNESS_DURATION;
            addStatTempStack(client, ZFStatSpeed, ZF_PARASITE_WEAKNESS_SPEED, ZF_PARASITE_WEAKNESS_DURATION);
            addStatTempStack(client, ZFStatAtt, ZF_PARASITE_WEAKNESS_ATTACK, ZF_PARASITE_WEAKNESS_DURATION);
            PrintHintText(client, "你变得虚弱了!");
        }
    }

    if (_inst.weakness_timer > 0) {
        _inst.weakness_timer--;
    }
}

FUNCTION(ParasitePerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.burst_timer > 0) {
        Format(buffer, maxlen, "超能爆发: %ds", _inst.burst_timer);
    } else if (_inst.weakness_timer > 0) {
        Format(buffer, maxlen, "虚弱: %ds", _inst.weakness_timer);
    } else {
        Format(buffer, maxlen, "能量: %d/%d", _inst.energy, ZF_PARASITE_ENERGY_FOR_BURST);
    }
}