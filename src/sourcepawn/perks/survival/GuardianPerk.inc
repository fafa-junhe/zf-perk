#if defined __GuardianPerk_included
    #endinput
#endif
#define __GuardianPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"

#define GUARDIAN_STILL_TIME        3.0
#define GUARDIAN_AURA_RADIUS       250.0
#define GUARDIAN_AURA_RADIUS_SQ    (GUARDIAN_AURA_RADIUS * GUARDIAN_AURA_RADIUS)
#define GUARDIAN_DEF_BONUS         30
#define GUARDIAN_ZOMBIE_PUSH       350.0

#define GUARDIAN_DATA_STILL_TIMER  (PERK_DATA_START_INDEX + 0)
#define GUARDIAN_DATA_FIELD_ACTIVE (PERK_DATA_START_INDEX + 1)
#define GUARDIAN_DATA_AURA_ENT     (PERK_DATA_START_INDEX + 2)

methodmap GuardianPerk < SurvivorBasePerk
{

public     GuardianPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        GuardianPerk     sm      = view_as<GuardianPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, GuardianPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, GuardianPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, GuardianPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, GuardianPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, GuardianPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, GuardianPerkFonDeath);

        sm.Position = view_as<DataPackPos>(GUARDIAN_DATA_STILL_TIMER);
        sm.WriteFloat(0.0);
        sm.Position = view_as<DataPackPos>(GUARDIAN_DATA_FIELD_ACTIVE);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(GUARDIAN_DATA_AURA_ENT);
        sm.WriteCell(-1);

        return sm;
    }

    property float still_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_STILL_TIMER);
            return this.ReadFloat();
        }

public         set(float value)
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_STILL_TIMER);
            this.WriteFloat(value);
        }
    }

    property bool field_active
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_FIELD_ACTIVE);
            return this.ReadCell() != 0;
        }

public         set(bool value)
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_FIELD_ACTIVE);
            this.WriteCell(value ? 1 : 0);
        }
    }

    property int aura_ent
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_AURA_ENT);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(GUARDIAN_DATA_AURA_ENT);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk GuardianPerkFnew(int client)
{
    return new GuardianPerk(client);
}

FUNCTION(GuardianPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Guardian");
}

FUNCTION(GuardianPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "GuardianPerk_shortdesc");
}

FUNCTION(GuardianPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "GuardianPerk_desc");
}

FUNCTION_INT(GuardianPerk, getCategory))
{
    return 2;
}

FUNCTION(GuardianPerk, onPlayerSpawn))
{
    _inst.still_timer  = 0.0;
    _inst.field_active = false;
    GuardianPerkF_RemoveAura(_inst);
}

FUNCTION(GuardianPerk, onPeriodic))
{
    int   client = _inst.client;
    float vel[3];
    GetEntPropVector(client, Prop_Data, "m_vecVelocity", vel);

    // Check if player is moving
    if (GetVectorLength(vel) >= 5.0)
    {
        _inst.still_timer = 0.0;
        if (_inst.field_active)
        {
            _inst.field_active = false;
            GuardianPerkF_RemoveAura(_inst);
        }
        return;
    }

    // Player is still, increment timer
    _inst.still_timer += 1.0;

    if (_inst.still_timer >= GUARDIAN_STILL_TIME)
    {
        if (!_inst.field_active)
        {
            _inst.field_active = true;
            GuardianPerkF_CreateAura(_inst);
        }
    }
}

FUNCTION(GuardianPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;
    if (_inst.field_active)
    {
        Format(buffer, maxlen, "%t", "GuardianPerk_Status_Active");

        // Apply aura effects
        addStat(client, ZFStatDef, ZFStatTypeCond, GUARDIAN_DEF_BONUS);
        float client_pos[3];
        GetClientAbsOrigin(client, client_pos);

        for (int i = 1; i <= MaxClients; i++)
        {
            if (!validClient(i) || !IsPlayerAlive(i) || i == client) continue;

            float target_pos[3];
            GetClientAbsOrigin(i, target_pos);

            if (GetVectorDistance(client_pos, target_pos, true) <= GUARDIAN_AURA_RADIUS_SQ)
            {
                if (isSur(i))
                {
                    addStat(i, ZFStatDef, ZFStatTypeCond, GUARDIAN_DEF_BONUS);
                }
                else if (isZom(i)) {
                    float push_vec[3];
                    SubtractVectors(target_pos, client_pos, push_vec);
                    NormalizeVector(push_vec, push_vec);
                    ScaleVector(push_vec, GUARDIAN_ZOMBIE_PUSH);
                    TeleportEntity(i, NULL_VECTOR, NULL_VECTOR, push_vec);
                }
            }
        }
    }
    else {
        float vel[3];
        GetEntPropVector(client, Prop_Data, "m_vecVelocity", vel);
        if (_inst.still_timer > 0.0)
        {
            int time_to_activate = RoundFloat(GUARDIAN_STILL_TIME - _inst.still_timer);
            if (time_to_activate > 0)
            {
                Format(buffer, maxlen, "%t", "GuardianPerk_Status_Charging", time_to_activate);
            }
        }
        else if (GetVectorLength(vel) >= 5.0) {
            Format(buffer, maxlen, "%t", "GuardianPerk_Status_Gone");
        }
    }
}

FUNCTION(GuardianPerk, onRemove))
{
    GuardianPerkF_RemoveAura(_inst);
}

FUNCTION(GuardianPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    if (victim == _inst.client)
    {
        GuardianPerkF_RemoveAura(_inst);
    }
}

stock void GuardianPerkF_CreateAura(GuardianPerk _inst)
{
    if (IsValidEntity(_inst.aura_ent))
    {
        return;
    }
    // Directly create the particle and store its index.
    _inst.aura_ent = fxCreateParticle(ZFPART_AURAGUARDIAN, _inst.client, AttachBase, -1.0);
}

stock void GuardianPerkF_RemoveAura(GuardianPerk _inst)
{
    if (IsValidEntity(_inst.aura_ent))
    {
        fxDeleteParticle(_inst.aura_ent);
        _inst.aura_ent = -1;
    }
}