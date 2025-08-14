#if defined __BountyHunterPerk_included
    #endinput
#endif
#define __BountyHunterPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define BOUNTY_SCAN_RADIUS        2000.0
#define BOUNTY_SCAN_RADIUS_SQ     (BOUNTY_SCAN_RADIUS * BOUNTY_SCAN_RADIUS)
#define BOUNTY_SCAN_DURATION      5.0
#define BOUNTY_SCAN_COOLDOWN      20
#define BOUNTY_KILL_DAMAGE_BONUS  7
#define BOUNTY_KILL_CRIT_BONUS    7
#define BOUNTY_KILL_HEALTH_BONUS  25

// Data slots
#define BOUNTY_DATA_COOLDOWN      (PERK_DATA_START_INDEX + 0)
#define BOUNTY_DATA_SCAN_END_TIME (PERK_DATA_START_INDEX + 1)

methodmap BountyHunterPerk < SurvivorBasePerk
{

public     BountyHunterPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        BountyHunterPerk sm      = view_as<BountyHunterPerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, BountyHunterPerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, BountyHunterPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, BountyHunterPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, BountyHunterPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, BountyHunterPerkFOnGameFrame);

        // Initialize data
        sm.Position = view_as<DataPackPos>(BOUNTY_DATA_COOLDOWN);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(BOUNTY_DATA_SCAN_END_TIME);
        sm.WriteFloat(0.0);

        return sm;
    }

    property int cooldown_timer
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(BOUNTY_DATA_COOLDOWN);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(BOUNTY_DATA_COOLDOWN);
            this.WriteCell(value);
        }
    }

    property float scan_end_time
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(BOUNTY_DATA_SCAN_END_TIME);
            return this.ReadFloat();
        }

public         set(float value)
        {
            this.Position = view_as<DataPackPos>(BOUNTY_DATA_SCAN_END_TIME);
            this.WriteFloat(value);
        }
    }
}

stock SurvivorBasePerk BountyHunterPerkFnew(int client)
{
    return new BountyHunterPerk(client);
}

FUNCTION(BountyHunterPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "BountyHunter");
}

FUNCTION(BountyHunterPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "BountyHunterPerk_shortdesc");
}

FUNCTION(BountyHunterPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "BountyHunterPerk_desc");
}

stock bool isAoeZombie(int client)
{
    if (!validClient(client) || g_hPerks[client] == null)
    {
        return false;
    }
    char perkName[32];
    g_hPerks[client].getName(perkName, sizeof(perkName));
    // AOE zombies are Toxic and Static Field
    return StrEqual(perkName, "Toxic") || StrEqual(perkName, "StaticField") || StrEqual(perkName, "GravityWarper") || StrEqual(perkName, "Swarming") || StrEqual(perkName, "Magnetic");
}

FUNCTION(BountyHunterPerk, onCallForMedic))
{
    int client = _inst.client;

    if (_inst.cooldown_timer > 0)
    {
        PrintHintText(client, "%t", "BountyHunterPerk_Hint_Cooldown", _inst.cooldown_timer);
        return;
    }

    _inst.cooldown_timer = BOUNTY_SCAN_COOLDOWN;

    float my_pos[3];
    GetClientAbsOrigin(client, my_pos);
    bool found = false;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (validLivingZom(i) && isAoeZombie(i))
        {
            float zom_pos[3];
            GetClientAbsOrigin(i, zom_pos);
            if (GetVectorDistance(my_pos, zom_pos, true) <= BOUNTY_SCAN_RADIUS_SQ)
            {
                // Start scanning
                _inst.scan_end_time = GetGameTime() + BOUNTY_SCAN_DURATION;
                found               = true;
                // Since we found at least one, we can break the loop.
                // The updateCondStats will handle drawing beams to all valid targets.
                break;
            }
        }
    }

    if (found)
    {
        PrintHintText(client, "%t", "BountyHunterPerk_Hint_Found");
    }
    else {
        PrintHintText(client, "%t", "BountyHunterPerk_Hint_NotFound");
    }
}

FUNCTION(BountyHunterPerk, onPeriodic))
{
    if (_inst.cooldown_timer > 0)
    {
        _inst.cooldown_timer--;
    }
}

FUNCTION(BountyHunterPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int client = _inst.client;
    if (isAoeZombie(victim))
    {
        addStat(client, ZFStatAtt, ZFStatTypePerm, BOUNTY_KILL_DAMAGE_BONUS);
        addStat(client, ZFStatCrit, ZFStatTypePerm, BOUNTY_KILL_CRIT_BONUS);
        addHealth(client, BOUNTY_KILL_HEALTH_BONUS, true);
        PrintHintText(client, "%t", "BountyHunterPerk_Hint_Bonus");
    }
}

FUNCTION_INT(BountyHunterPerk, getCategory))
{
    return 1;
}

FUNCTION(BountyHunterPerk, updateCondStats), char[] buffer, int maxlen) {
    if (_inst.cooldown_timer > 0)
    {
        Format(buffer, maxlen, "%t", "BountyHunterPerk_Status_Cooldown", _inst.cooldown_timer);
    }
    else {
        Format(buffer, maxlen, "%t", "BountyHunterPerk_Status_Ready");
    }

}
FUNCTION(BountyHunterPerk, OnGameFrame))
{
    int client = _inst.client;
    if (_inst.scan_end_time > GetGameTime())
    {
        float my_pos[3];
        GetClientAbsOrigin(client, my_pos);
        bool found_in_frame = false;

        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingZom(i) && isAoeZombie(i))
            {
                float zom_pos[3];
                GetClientAbsOrigin(i, zom_pos);
                if (GetVectorDistance(my_pos, zom_pos, true) <= BOUNTY_SCAN_RADIUS_SQ)
                {
                    int  color[4];
                    char perkName[32];
                    g_hPerks[i].getName(perkName, sizeof(perkName));

                    if (StrEqual(perkName, "StaticField"))
                    {
                        color[0] = 255;
                        color[1] = 255;
                        color[2] = 255;
                        color[3] = 200;    // White
                    }
                    else if (StrEqual(perkName, "GravityWarper")) {
                        color[0] = 138;
                        color[1] = 43;
                        color[2] = 226;
                        color[3] = 200;    // Purple
                    }
                    else if (StrEqual(perkName, "Swarming")) {
                        color[0] = 139;
                        color[1] = 69;
                        color[2] = 19;
                        color[3] = 200;    // Brown
                    }
                    else if (StrEqual(perkName, "Magnetic")) {
                        color[0] = 0;
                        color[1] = 191;
                        color[2] = 255;
                        color[3] = 200;    // Deep Sky Blue
                    }
                    else {    // Toxic
                        color[0] = 0;
                        color[1] = 255;
                        color[2] = 0;
                        color[3] = 200;    // Green
                    }
                    fxSprite(client, i, color, ZFSPR_DEATH_INDEX);
                    found_in_frame = true;
                }
            }
        }
        if (found_in_frame)
        {
            char buffer[64];
            Format(buffer, sizeof(buffer), "%t", "BountyHunterPerk_Status_Scanning");
            return;
        }
    }

    
}
