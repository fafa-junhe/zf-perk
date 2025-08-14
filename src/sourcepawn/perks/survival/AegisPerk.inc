#if defined __AegisPerk_included
    #endinput
#endif
#define __AegisPerk_included

#include "../../perk_structs.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include <sdkhooks>
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"

#define AEGIS_MAX_ITEMS 5
#define AEGIS_COOLDOWN 10
#define AEGIS_DEFENSE_BONUS 10

#define AEGIS_DATA_TIMER (PERK_DATA_START_INDEX + 0)

#define AEGIS_DATA_PLACED_COUNT (PERK_DATA_START_INDEX + 1)
 
methodmap AegisPerk < SurvivorBasePerk
{
    public AegisPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AegisPerk    sm      = view_as<AegisPerk>(sm_base);
 
        PERK_REGISTER_BASIC_INFO(sm, AegisPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AegisPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AegisPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, AegisPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AegisPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, AegisPerkFonPlayerSpawn);
 
        sm.Position = view_as<DataPackPos>(AEGIS_DATA_TIMER);
        sm.WriteCell(0);
        sm.placed_count = 0;
 
        return sm;
    }
 
    property int perk_timer
    {
        public get()
        {
            this.Position = view_as<DataPackPos>(AEGIS_DATA_TIMER);
            return this.ReadCell();
        }
        public set(int value)
        {
            this.Position = view_as<DataPackPos>(AEGIS_DATA_TIMER);
            this.WriteCell(value);
        }
    }
 
    property int placed_count
    {
        public get()
        {
            this.Position = view_as<DataPackPos>(AEGIS_DATA_PLACED_COUNT);
            return this.ReadCell();
        }
        public set(int value)
        {
            this.Position = view_as<DataPackPos>(AEGIS_DATA_PLACED_COUNT);
            this.WriteCell(value);
        }
    }
}

stock SurvivorBasePerk AegisPerkFnew(int client)
{
    return new AegisPerk(client);
}

FUNCTION(AegisPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Aegis");
}

FUNCTION(AegisPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "AegisPerk_shortdesc");
}

FUNCTION(AegisPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "AegisPerk_desc");
}

FUNCTION_INT(AegisPerk, getCategory))
{
    return 3;
}

FUNCTION(AegisPerk, onPeriodic))
{
    if (_inst.perk_timer > 0)
    {
        _inst.perk_timer--;
    }
}

FUNCTION(AegisPerk, onPlayerSpawn))
{
    _inst.placed_count = 0;
}
 
FUNCTION(AegisPerk, updateCondStats), char[] buffer, int maxlen)
{
    if (_inst.perk_timer > 0)
    {
        Format(buffer, maxlen, "%t", "AegisPerk_Status_Cooldown", _inst.perk_timer);
    }
    else
    {
        int remaining = AEGIS_MAX_ITEMS - _inst.placed_count;
        if (remaining <= 0)
        {
            Format(buffer, maxlen, "%t", "AegisPerk_Status_Full");
        }
        else
        {
            Format(buffer, maxlen, "%t", "AegisPerk_Status_Ready", remaining, AEGIS_MAX_ITEMS);
        }
    }
}
 
FUNCTION(AegisPerk, onCallForMedic))
{
    int client = _inst.client;
    if (roundState() == RoundGrace)
    {
        PrintHintText(client, "%t", "Ability_Cannot_Use_Grace");
        return;
    }

    if (_inst.perk_timer > 0 || roundState() == RoundGrace)
    {
        PrintHintText(client, "%t", "AegisPerk_Hint_Cooldown", _inst.perk_timer);
        return;
    }
 
    if (!isGrounded(client))
    {
        PrintHintText(client, "%t", "AegisPerk_Hint_NotGrounded");
        return;
    }
 
    if (_inst.placed_count >= AEGIS_MAX_ITEMS)
    {
        PrintHintText(client, "%t", "AegisPerk_Hint_Full");
        return;
    }
 
    int itemIndex = getFreeItemIndex(client, AEGIS_MAX_ITEMS);
    if (itemIndex == -1)
    {
        // This case should not be reached if onDeath cleans up properly, but it's a good safeguard.
        PrintHintText(client, "%t", "AegisPerk_Hint_Full");
        return;
    }
    
    float eye_pos[3], ang[3], fwd[3], right[3];
    GetClientAbsOrigin(client, eye_pos);
    GetClientEyeAngles(client, ang);
    ang[0] = 0.0;
    ang[2] = 0.0;
    eye_pos[2] += 25.0;

    GetAngleVectors(ang, fwd, right, NULL_VECTOR);

    float start_pos[3];
    ScaleVector(fwd, 50.0);
    AddVectors(eye_pos, fwd, start_pos);
    bool  clientsNearby    = false;

    float otherPos[3];
    for (int i = 1; i <= MaxClients; i++)
    {
        if (!IsClientInGame(i) || !IsPlayerAlive(i)) continue;

        // 检查玩家
        if (i == client) continue;
        GetClientAbsOrigin(i, otherPos);
        if (GetVectorDistance(start_pos, otherPos, true) <= (80 * 80))
        {
            clientsNearby = true;
            break;
        }
    }
    if (clientsNearby)
    {
        PrintHintText(client, "%t", "AegisPerk_Hint_PlayersNearby");
        return;
    }
    _inst.perk_timer = AEGIS_COOLDOWN;
    _inst.placed_count++;
 
    ang[1] -= 180;
 
    int shield = fxCreateModelStatic(ZFMDL_AEGIS_SHIELD, client, false, true, start_pos, ang);
    setItemMetadata(shield, client);
    if (shield != -1)
    {
        zf_item[client][itemIndex] = shield;
        SDKHook(shield, SDKHook_OnTakeDamage, onShieldTakeDamage);
    }
}

FUNCTION(AegisPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int client = _inst.client;
    for (int i = 0; i < AEGIS_MAX_ITEMS; i++)
    {
        removeItem(client, i);
    }
}

public Action onShieldTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype)
{
    if (validClient(attacker) && attackWasMelee(attacker, inflictor, damagetype))
    {
        SDKUnhook(victim, SDKHook_OnTakeDamage, onShieldTakeDamage);

        addStat(attacker, ZFStatDef, ZFStatTypePerm, AEGIS_DEFENSE_BONUS);
        PrintHintText(attacker, "%t", "AegisPerk_Hint_ShieldEquipped");
        SetVariantString("Open")
        AcceptEntityInput(victim, "SetAnimation");

        DataPack hPack = new DataPack();
        hPack.WriteCell(victim);
        CreateTimer(1.0, timer_deleteShield, hPack);
    }
    return Plugin_Continue;
}

public Action timer_deleteShield(Handle timer, DataPack hPack)
{
    hPack.Reset();
    int victim = hPack.ReadCell();
    delete hPack;

    if (victim > 0 && IsValidEntity(victim))
    {
        int owner = getItemMetadata(victim);
        if(validSur(owner))
        {
            for (int i = 0; i < AEGIS_MAX_ITEMS; i++)
            {
                if (zf_item[owner][i] == victim)
                {
                    removeItem(owner, i);
                    break;
                }
            }
        }
        else
        {
            AcceptEntityInput(victim, "Kill");
        }
    }
    return Plugin_Stop;
}