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

#define VOLATILE_ENERGY_STORE_RATIO 0.5 // 储存50%的伤害
#define VOLATILE_ENERGY_DECAY 8.0      // 每秒衰减5点能量
#define VOLATILE_MAX_ENERGY 150.0      // 最大能量
#define VOLATILE_EXPLOSION_RADIUS 200.0
#define VOLATILE_SUICIDE_MULTIPLIER 0.5 // 主动引爆的伤害倍率

#define VOLATILE_DATA_ENERGY (PERK_DATA_START_INDEX + 0)

methodmap VolatilePerk < ZombieBasePerk {
    public VolatilePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VolatilePerk sm = view_as<VolatilePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, VolatilePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, VolatilePerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, VolatilePerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, VolatilePerkFonTakeDamage);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, VolatilePerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, VolatilePerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, VolatilePerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, VolatilePerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, VolatilePerkFonPlayerSpawn);

        sm.energy = 0.0;
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

FUNCTION(VolatilePerk, onPlayerSpawn)) {
    _inst.energy = 0.0;
}

FUNCTION_FLOAT(VolatilePerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (victim == _inst.client) {
        _inst.energy = fMin(VOLATILE_MAX_ENERGY, _inst.energy + damage * VOLATILE_ENERGY_STORE_RATIO);
    }
    return damage;
}

FUNCTION(VolatilePerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    // Only explode if the player killed themselves (via 'E' key or suicide command)
    if (victim == _inst.client && killer == victim && _inst.energy > 0) {
        float explosion_damage = _inst.energy * VOLATILE_SUICIDE_MULTIPLIER;

        applyDamageRadialAtClient(_inst.client, RoundToNearest(explosion_damage), RoundToNearest(VOLATILE_EXPLOSION_RADIUS), true);
        fxCreateParticle("rocket_explosion_classic_crit_red", _inst.client, AttachNone, 2.0);
    }
}

FUNCTION(VolatilePerk, onCallForMedic)) {
    int client = _inst.client;
    if (_inst.energy > 0) {
        // The onDeath function will handle the explosion now.
        // We just need to kill the player.
        ForcePlayerSuicide(client);
    } else {
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