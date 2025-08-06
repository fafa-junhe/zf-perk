#if defined __AlchemistPerk_included
    #endinput
#endif
#define __AlchemistPerk_included

#include "SurvivorBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"

#define ALCHEMIST_POTION_GEN_TIME  25
#define ALCHEMIST_MAX_POTIONS      3
#define ALCHEMIST_AOE_DURATION     7.0
#define ALCHEMIST_AOE_RADIUS       180.0
#define ALCHEMIST_AOE_RADIUS_SQ    (ALCHEMIST_AOE_RADIUS * ALCHEMIST_AOE_RADIUS)
#define ALCHEMIST_ALLY_HEAL        5
#define ALCHEMIST_ALLY_SPEED_BONUS 20
#define ALCHEMIST_ZOMBIE_SLOW      -30
#define ALCHEMIST_ZOMBIE_DAMAGE    10.0
#define ALCHEMIST_POTION_MODEL     "models/props_halloween/hwn_flask_vial.mdl"

// Data slots
#define ALCHEMIST_DATA_PROJECTILES (PERK_DATA_START_INDEX + 0)

// We'll use the global zf_item array to track the puddle entities
#define ALCHEMIST_MAX_PUDDLES      MAX_ITEMS

methodmap AlchemistPerk < SurvivorBasePerk
{

public     AlchemistPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        AlchemistPerk    sm      = view_as<AlchemistPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, AlchemistPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, AlchemistPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, AlchemistPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, AlchemistPerkFonPlayerSpawn);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, AlchemistPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, AlchemistPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, AlchemistPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, AlchemistPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, AlchemistPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, AlchemistPerkFonDeath);

        sm.projectiles = new DataPack();

        return sm;
    }

    property DataPack projectiles
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(ALCHEMIST_DATA_PROJECTILES);
            return view_as<DataPack>(this.ReadCell());
        }

public         set(DataPack value)
        {
            this.Position = view_as<DataPackPos>(ALCHEMIST_DATA_PROJECTILES);
            this.WriteCell(view_as<int>(value));
        }
    }
}

stock SurvivorBasePerk AlchemistPerkFnew(int client)
{
    return new AlchemistPerk(client);
}

FUNCTION(AlchemistPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Alchemist");
}

FUNCTION(AlchemistPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "AlchemistPerk_shortdesc");
}

FUNCTION(AlchemistPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "AlchemistPerk_desc");
}

FUNCTION(AlchemistPerk, onPlayerSpawn))
{
    int client           = _inst.client;
    zf_perkState[client] = 1;                            // Start with 1 potion
    zf_perkTimer[client] = ALCHEMIST_POTION_GEN_TIME;    // Time to next potion
    // Clean up any existing puddles from this player
    AlchemistPerkFonRemove(_inst);
    if (_inst.projectiles == null)
    {
        _inst.projectiles = new DataPack();
    }
    _inst.projectiles.Reset();
}

FUNCTION(AlchemistPerk, onPeriodic))
{
    int client = _inst.client;


    if (zf_perkState[client] < ALCHEMIST_MAX_POTIONS)
    {
        if (zf_perkTimer[client] > 0)
        {
            zf_perkTimer[client]--;
        }
        else
        {
            zf_perkState[client]++;
            zf_perkTimer[client] = ALCHEMIST_POTION_GEN_TIME;
            PrintHintText(client, "%t", "AlchemistPerk_Hint_PotionReady", zf_perkState[client]);
        }
    }

    // --- Puddle effect logic ---
    for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++)
    {
        int puddle = zf_item[client][i];

        if (!IsValidEntity(puddle))
        {
            continue;
        }

        if (!HasEntProp(puddle, Prop_Data, "m_ModelName"))
        {
            zf_item[client][i] = -1;
            continue;
        }

        char model_name[PLATFORM_MAX_PATH];
        GetEntPropString(puddle, Prop_Data, "m_ModelName", model_name, sizeof(model_name));

        if (strcmp(model_name, ALCHEMIST_POTION_MODEL) != 0)
        {
            zf_item[client][i] = -1;
            continue;
        }

        float puddle_pos[3];
        GetEntPropVector(puddle, Prop_Send, "m_vecOrigin", puddle_pos);

        for (int p = 1; p <= MaxClients; p++)
        {
            if (!IsClientInGame(p) || !IsPlayerAlive(p)) continue;

            float player_pos[3];
            GetClientAbsOrigin(p, player_pos);
            float distSqr = GetVectorDistance(player_pos, puddle_pos, true);

            if (distSqr < ALCHEMIST_AOE_RADIUS_SQ)
            {

                if (validSur(p))
                {
                    addStatTempStack(p, ZFStatSpeed, ALCHEMIST_ALLY_SPEED_BONUS, 5);
                }
                else if (validZom(p))
                {
                    SDKHooks_TakeDamage(p, client, client, ALCHEMIST_ZOMBIE_DAMAGE);
                    addStat(p, ZFStatSpeed, ZFStatTypeCond, ALCHEMIST_ZOMBIE_SLOW);
                }
            }
        }
    }
}

FUNCTION(AlchemistPerk, onCallForMedic))
{
    int client = _inst.client;
    if (zf_perkState[client] <= 0)
    {
        PrintHintText(client, "%t", "AlchemistPerk_Hint_NoPotions");
        return;
    }

    int item_index = getFreeItemIndex(client, ALCHEMIST_MAX_PUDDLES);
    if (item_index == -1)
    {
        PrintHintText(client, "%t", "AlchemistPerk_Hint_PuddlesFull");
        return;
    }

    zf_perkState[client]--;
    int   color[3] = { 100, 255, 100 };

    float pos[3], ang[3];
    GetClientEyePosition(client, pos);
    GetClientEyeAngles(client, ang);

    int item = fxCreateModelThrown(ALCHEMIST_POTION_MODEL, client, pos, ang, 800.0, color);
    if (item != -1)
    {
        setItemMetadata(item, item_index);    // Store our item index in the item's metadata

        // NEW: track projectile
        DataPack projectiles = _inst.projectiles;
        projectiles.WriteCell(item);
        projectiles.WriteFloat(pos[0]);
        projectiles.WriteFloat(pos[1]);
        projectiles.WriteFloat(pos[2]);
    }
}

FUNCTION(AlchemistPerk, onGameFrame))
{
    // --- Projectile collision logic (New) ---
    DataPack projectiles = _inst.projectiles;
    if (projectiles != null && projectiles != INVALID_HANDLE)
    {
        DataPack new_projectiles = new DataPack();
        projectiles.Reset();
        float prev_pos[3], current_pos[3], hit_pos[3], hit_vec[3];

        while (projectiles.IsReadable())
        {
            int projectile = projectiles.ReadCell();
            prev_pos[0]    = projectiles.ReadFloat();
            prev_pos[1]    = projectiles.ReadFloat();
            prev_pos[2]    = projectiles.ReadFloat();

            if (!IsValidEdict(projectile)) continue;

            if (!HasEntProp(projectile, Prop_Data, "m_ModelName"))
            {
                continue;
            }
            char model_name[PLATFORM_MAX_PATH];
            GetEntPropString(projectile, Prop_Data, "m_ModelName", model_name, sizeof(model_name));
            if (strcmp(model_name, ALCHEMIST_POTION_MODEL) != 0)
            {
                continue;
            }

            GetEntPropVector(projectile, Prop_Send, "m_vecOrigin", current_pos);

            if (doItemCollide(projectile, prev_pos, hit_pos, hit_vec))
            {
                Alchemist_CreatePuddle(projectile, hit_pos);
            }
            else {
                new_projectiles.WriteCell(projectile);
                new_projectiles.WriteFloat(current_pos[0]);
                new_projectiles.WriteFloat(current_pos[1]);
                new_projectiles.WriteFloat(current_pos[2]);
            }
        }
        delete _inst.projectiles;
        _inst.projectiles = new_projectiles;
    }

}

FUNCTION(AlchemistPerk, updateCondStats), char[] buffer, int maxlen)
{
    int potions = zf_perkState[_inst.client];
    if (potions < ALCHEMIST_MAX_POTIONS)
    {
        Format(buffer, maxlen, "%t", "AlchemistPerk_Status_Generating", potions, ALCHEMIST_MAX_POTIONS, zf_perkTimer[_inst.client]);
    }
    else {
        Format(buffer, maxlen, "%t", "AlchemistPerk_Status_Full", potions, ALCHEMIST_MAX_POTIONS);
    }
}

FUNCTION(AlchemistPerk, onRemove))
{
    // Clean up puddles
    for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++)
    {
        if (validItem(_inst.client, i))
        {
            removeItem(_inst.client, i);
        }
    }

    // Clean projectiles
    DataPack projectiles = _inst.projectiles;
    if (projectiles != null && projectiles != INVALID_HANDLE)
    {
        projectiles.Reset();
        while (projectiles.IsReadable())
        {
            int projectile_entity = projectiles.ReadCell();
            if (IsValidEdict(projectile_entity))
            {
                AcceptEntityInput(projectile_entity, "Kill");
            }
            projectiles.ReadFloat();    // x
            projectiles.ReadFloat();    // y
            projectiles.ReadFloat();    // z
        }
        projectiles.Reset();
    }
}

FUNCTION(AlchemistPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype)
{
    if (victim == _inst.client)
    {
        AlchemistPerkFonRemove(_inst);
    }
}

stock void Alchemist_CreatePuddle(int item, const float origin[3])
{
    int owner = GetEntPropEnt(item, Prop_Data, "m_hOwnerEntity");
    if (!validClient(owner))
    {
        if (IsValidEdict(item)) AcceptEntityInput(item, "Kill");
        return;
    }

    // Create an invisible anchor entity for the puddle
    int puddle = fxCreateModelStatic("models/props_halloween/hwn_flask_vial.mdl", owner, false, false, origin, view_as<float>({ 0.0, 0.0, 0.0 }));
    SetEntityRenderMode(puddle, RENDER_NONE);    // Make it invisible

    fxCreateParticle(ZFPART_AURAPOTIONPUDDLE, puddle, AttachBase, ALCHEMIST_AOE_DURATION);
    fxCreateSoundToAll(ZFSND_GLASSBROKEN, puddle);

    int item_index = getItemMetadata(item);
    if (item_index >= 0 && item_index < ALCHEMIST_MAX_PUDDLES)
    {
        zf_item[owner][item_index] = puddle;
    }

    CreateTimer(ALCHEMIST_AOE_DURATION, Alchemist_RemovePuddle, puddle, TIMER_FLAG_NO_MAPCHANGE);

    if (IsValidEdict(item)) AcceptEntityInput(item, "Kill");
}

public Action Alchemist_RemovePuddle(Handle timer, any puddle)
{
    if (puddle != -1)
    {
        int owner = GetEntPropEnt(puddle, Prop_Data, "m_hOwnerEntity");
        if (validClient(owner))
        {
            for (int i = 0; i < ALCHEMIST_MAX_PUDDLES; i++)
            {
                if (zf_item[owner][i] == puddle)
                {
                    zf_item[owner][i] = -1;
                    break;
                }
            }
        }
        AcceptEntityInput(puddle, "Kill");
    }
    return Plugin_Stop;
}
