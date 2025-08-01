#if defined __ToxicPerk_included
#endinput
#endif
#define __ToxicPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"

#define ZF_TOXIC_DAMAGE_PENALTY    -50
#define ZF_TOXIC_POISON_DURATION   12.0
#define ZF_TOXIC_POISON_DAMAGE     10
#define ZF_TOXIC_AURA_RADIUS       150.0
#define ZF_TOXIC_AURA_DAMAGE       5.0
#define TOXIC_STILL_TIME           3.0

#define TOXIC_DATA_STILL_TIMER     (PERK_DATA_START_INDEX + 0)
#define TOXIC_DATA_FIELD_ACTIVE    (PERK_DATA_START_INDEX + 1)
#define TOXIC_DATA_AURA_ENT        (PERK_DATA_START_INDEX + 2)

methodmap ToxicPerk < ZombieBasePerk {
    public ToxicPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ToxicPerk sm = view_as<ToxicPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ToxicPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ToxicPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ToxicPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, ToxicPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, ToxicPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE_POST, ToxicPerkFonTakeDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, ToxicPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, ToxicPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, ToxicPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, ToxicPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, ToxicPerkFonDeath);

        sm.Position = view_as<DataPackPos>(TOXIC_DATA_STILL_TIMER);
        sm.WriteFloat(0.0);
        sm.Position = view_as<DataPackPos>(TOXIC_DATA_FIELD_ACTIVE);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(TOXIC_DATA_AURA_ENT);
        sm.WriteCell(-1);

        return sm;
    }

    property float still_timer
    {
        public get() {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_STILL_TIMER);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_STILL_TIMER);
            this.WriteFloat(value);
        }
    }

    property bool field_active
    {
        public get() {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_FIELD_ACTIVE);
            return this.ReadCell() != 0;
        }
        public set(bool value) {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_FIELD_ACTIVE);
            this.WriteCell(value ? 1 : 0);
        }
    }

    property int aura_ent
    {
        public get() {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_AURA_ENT);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(TOXIC_DATA_AURA_ENT);
            this.WriteCell(value);
        }
    }
}

stock BasePerk ToxicPerkFnew(int client) {
    return new ToxicPerk(client);
}

FUNCTION(ToxicPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Toxic");
}

FUNCTION(ToxicPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ToxicPerk_shortdesc");
}

FUNCTION(ToxicPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ToxicPerk_desc");
}

FUNCTION(ToxicPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, ZF_TOXIC_DAMAGE_PENALTY);
}

stock void ApplyPoisonTo(int target, int attacker) {
    if (!validLivingSur(target)) return;

    int poison_entity = CreateEntityByName("point_hurt");
    if (poison_entity != -1) {
        float target_pos[3];
        GetClientAbsOrigin(target, target_pos);
        TeleportEntity(poison_entity, target_pos, NULL_VECTOR, NULL_VECTOR);

        DispatchKeyValue(poison_entity, "Damage", "10");
        DispatchKeyValue(poison_entity, "DamageRadius", "1");
        DispatchKeyValue(poison_entity, "DamageType", "80000000"); // ZF_DMGTYPE_POISON
        DispatchKeyValue(poison_entity, "DamageDelay", "1.0");
        DispatchKeyValue(poison_entity, "DamageTarget", "!activator");
        DispatchSpawn(poison_entity);

        SetVariantEntity(target);
        AcceptEntityInput(poison_entity, "SetDamageTarget", attacker);
        AcceptEntityInput(poison_entity, "TurnOn");

        char entity_name[32];
        Format(entity_name, sizeof(entity_name), "poison_%d_%d", target, GetRandomInt(0, 9999));
        DispatchKeyValue(poison_entity, "targetname", entity_name);
        
        CreateTimer(ZF_TOXIC_POISON_DURATION, Timer_RemovePoison, poison_entity, TIMER_FLAG_NO_MAPCHANGE);
    }
}

public Action Timer_RemovePoison(Handle timer, any poison_entity) {
    if (IsValidEntity(poison_entity)) {
        AcceptEntityInput(poison_entity, "Kill");
    }
    return Plugin_Stop;
}

FUNCTION(ToxicPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (attacker == _inst.client && validLivingSur(victim) && attackWasMelee(attacker, inflictor, damagetype)) {
        ApplyPoisonTo(victim, attacker);
    }
}

FUNCTION(ToxicPerk, onTakeDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (victim == _inst.client && validLivingSur(attacker) && attackWasMelee(attacker, inflictor, damagetype)) {
        ApplyPoisonTo(attacker, victim);
    }
}

FUNCTION(ToxicPerk, onPeriodic)) {
    int client = _inst.client;
    float vel[3];
    GetEntPropVector(client, Prop_Data, "m_vecVelocity", vel);

    // Check if player is moving
    if (GetVectorLength(vel) >= 5.0) {
        _inst.still_timer = 0.0;
        if (_inst.field_active) {
            _inst.field_active = false;
            ToxicPerkF_RemoveAura(_inst);
        }
        return;
    }

    // Player is still, increment timer
    _inst.still_timer += 1.0;

    if (_inst.still_timer >= TOXIC_STILL_TIME) {
        if (!_inst.field_active) {
            _inst.field_active = true;
            ToxicPerkF_CreateAura(_inst);
        }
    }

    if (_inst.field_active) {
        float my_pos[3];
        GetClientAbsOrigin(client, my_pos);

        for (int i = 1; i <= MaxClients; i++) {
            if (validLivingSur(i)) {
                float sur_pos[3];
                GetClientAbsOrigin(i, sur_pos);
                if (GetVectorDistance(my_pos, sur_pos) <= ZF_TOXIC_AURA_RADIUS) {
                    SDKHooks_TakeDamage(i, client, client, ZF_TOXIC_AURA_DAMAGE, ZF_DMGTYPE_POISON);
                }
            }
        }
    }
}

FUNCTION(ToxicPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.field_active) {
        Format(buffer, maxlen, "%t", "ToxicPerk_Status_Active");
    } else {
        float vel[3];
        GetEntPropVector(_inst.client, Prop_Data, "m_vecVelocity", vel);
        if (_inst.still_timer > 0.0) {
            int time_to_activate = RoundFloat(TOXIC_STILL_TIME - _inst.still_timer);
            if (time_to_activate > 0) {
                Format(buffer, maxlen, "%t", "ToxicPerk_Status_Charging", time_to_activate);
            }
        } else if (GetVectorLength(vel) >= 5.0) {
             Format(buffer, maxlen, "%t", "ToxicPerk_Status_Gone");
        }
    }
}

FUNCTION(ToxicPerk, onPlayerSpawn)) {
    _inst.still_timer  = 0.0;
    _inst.field_active = false;
    ToxicPerkF_RemoveAura(_inst);
}

FUNCTION(ToxicPerk, onRemove)) {
    ToxicPerkF_RemoveAura(_inst);
}

FUNCTION(ToxicPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client) {
        ToxicPerkF_RemoveAura(_inst);
    }
}

stock void ToxicPerkF_CreateAura(ToxicPerk _inst) {
    if (IsValidEntity(_inst.aura_ent)) {
        return;
    }
    _inst.aura_ent = fxCreateParticle("utaunt_arcane_green_parent", _inst.client, AttachBase, -1.0);
}

stock void ToxicPerkF_RemoveAura(ToxicPerk _inst) {
    if (IsValidEntity(_inst.aura_ent)) {
        fxDeleteParticle(_inst.aura_ent);
        _inst.aura_ent = -1;
    }
}
