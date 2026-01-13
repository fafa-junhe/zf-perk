#if defined __VolatilePerk_included
#endinput
#endif
#define __VolatilePerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_util_base.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_fx.inc"

#define VOLATILE_ENERGY_STORE_RATIO 1.0 // 储存100%的伤害
#define VOLATILE_ENERGY_DECAY 3.0      // 每秒衰减3点能量
#define VOLATILE_MAX_ENERGY 150.0      // 最大能量
#define VOLATILE_EXPLOSION_RADIUS 500.0
#define VOLATILE_SUICIDE_MULTIPLIER 0.7 // 主动引爆的伤害倍率

#define VOLATILE_DATA_ENERGY (PERK_DATA_START_INDEX + 0)

methodmap VolatilePerk < ZombieBasePerk {
    public VolatilePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VolatilePerk sm = view_as<VolatilePerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, VolatilePerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, VolatilePerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, VolatilePerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, VolatilePerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, VolatilePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, VolatilePerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, VolatilePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, VolatilePerkFonRemove);

        sm.energy = 0.0;
        sm.hasExploded = false;
        return sm;
    }

    property float energy {
        public get() {
            this.Position = view_as<DataPackPos>(VOLATILE_DATA_ENERGY);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(VOLATILE_DATA_ENERGY);
            this.WriteFloat(value);
        }
    }

    property bool hasExploded {
        public get() {
            this.Position = view_as<DataPackPos>(VOLATILE_DATA_ENERGY + 1);
            return this.ReadCell();
        }
        public set(bool value) {
            this.Position = view_as<DataPackPos>(VOLATILE_DATA_ENERGY + 1);
            this.WriteCell(value);
        }
    }
}

stock BasePerk VolatilePerkFnew(int client) {
    return new VolatilePerk(client);
}

FUNCTION(VolatilePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Volatile");
}

FUNCTION(VolatilePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VolatilePerk_shortdesc");
}

FUNCTION(VolatilePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VolatilePerk_desc");
}


FUNCTION_INT(VolatilePerk, getCategory))
{
    return 1;
}



FUNCTION(VolatilePerk, onPlayerSpawn)) {
    _inst.energy = 0.0;
    _inst.hasExploded = false;
}

FUNCTION_FLOAT(VolatilePerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    int client = _inst.client;
    if (victim != client) {
        return damage;
    }
    if (client == attacker) {
        return damage;
    }

    // Store energy from any incoming damage
    _inst.energy = fMin(VOLATILE_MAX_ENERGY, _inst.energy + damage * VOLATILE_ENERGY_STORE_RATIO);

    // Explosion is now only triggered by pressing 'E'.
    // This function only stores energy from damage taken.

    return damage;
}

FUNCTION(VolatilePerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.energy > 0 && !_inst.hasExploded) {
        _inst.hasExploded = true;

        float explosion_damage = _inst.energy * VOLATILE_SUICIDE_MULTIPLIER;

        float pos[3];
        GetClientAbsOrigin(client, pos);
        applyDamageRadial(client, RoundToNearest(explosion_damage), pos, RoundToNearest(VOLATILE_EXPLOSION_RADIUS), true, GetClientTeam(client));
        fxCreateParticle("rocket_explosion_classic_crit_red", client, AttachNone, 2.0);

        ForcePlayerSuicide(client);
    } else if (_inst.energy <= 0) {
        PrintHintText(client, "%t", "ZF_Volatile_No_Energy");
    }
}

FUNCTION(VolatilePerk, onPeriodic)) {
    if (_inst.energy > 0) {
        _inst.energy = fMax(0.0, _inst.energy - VOLATILE_ENERGY_DECAY);
    }
}

FUNCTION(VolatilePerk, updateCondStats), char[] buffer, int maxlen) {
    Format(buffer, maxlen, "%t", "ZF_Volatile_Stored_Energy", _inst.energy);
}

FUNCTION(VolatilePerk, updateClientPermStats)) {
    createAura(_inst.client, ZFPART_AURAVOLATILE, AttachBase);
}

FUNCTION(VolatilePerk, onRemove)) {
    if (validAura(_inst.client)) {
        removeAura(_inst.client);
    }
}