#if defined _ZF_PERK_VTABLE_INC
  #endinput
#endif
#define _ZF_PERK_VTABLE_INC

// =================================================================================
// == VTable: Function Pointers
// == These indices point to the function pointers within the BasePerk DataPack.
// == They act as our "virtual methods".
// =================================================================================

#define VTABLE_GET_NAME                     0
#define VTABLE_GET_SHORT_DESC               1
#define VTABLE_GET_LONG_DESC                2
#define VTABLE_GET_DISPLAY_NAME             25
// Slot 3 is reserved for data
#define VTABLE_ON_PLAYER_RUN_CMD            4
#define VTABLE_ON_AMMO_PICKUP               5
#define VTABLE_ON_CALC_IS_ATTACK_CRITICAL   6
#define VTABLE_ON_CALL_FOR_MEDIC            7
#define VTABLE_ON_GAME_FRAME                8
#define VTABLE_ON_GRACE_END                 9
#define VTABLE_ON_MED_PICKUP                10
#define VTABLE_ON_PERIODIC                  11
#define VTABLE_ON_PLAYER_DEATH              12
#define VTABLE_ON_PLAYER_SPAWN              13
#define VTABLE_ON_REMOVE                    14
#define VTABLE_ON_SET_TRANSMIT              15
#define VTABLE_ON_TAKE_DAMAGE               16
#define VTABLE_ON_TAKE_DAMAGE_POST          17
#define VTABLE_ON_TOUCH                     18
#define VTABLE_UPDATE_CLIENT_PERM_STATS     19
#define VTABLE_UPDATE_COND_STATS            20
#define VTABLE_DO_ITEM_THROW                21
#define VTABLE_ON_FENCE_TAKE_DAMAGE         22
#define VTABLE_ON_DEATH                     23
#define VTABLE_ON_PLAYER_TOUCH              24
#define VTABLE_ON_ASSIST_KILL               26
#define VTABLE_ON_DEAL_DAMAGE_POST          35
#define VTABLE_ON_KILL                      36
#define VTABLE_ON_BUILDING_TAKE_DAMAGE      37
#define VTABLE_GET_CATEGORY                 38

#define PERK_VTABLE_SIZE                    39


// =================================================================================
// == Data Slots
// == These indices point to the data stored within the Perk DataPack.
// =================================================================================

// -- Base Perk Data --
#define PERK_DATA_CLIENT                    3

// -- Perk-Specific Data Start Index --
// Perks that need to store their own data should start from this index
// to avoid conflicts with the base perk's vtable and data.
// Each level of "inheritance" should increment this.
#define PERK_DATA_START_INDEX               39

/*
    Example Usage for a new perk 'MyPerk' that inherits from 'BasePerk':
    #define MYPERK_DATA_SOME_VAR      (PERK_DATA_START_INDEX + 0)
    #define MYPERK_DATA_ANOTHER_VAR   (PERK_DATA_START_INDEX + 1)

    Example for a perk 'MySurvivorPerk' that inherits from 'SurvivorBasePerk':
    // Assuming SurvivorBasePerk uses one data slot.
    #define SURVIVOR_PERK_DATA_END_INDEX    (PERK_DATA_START_INDEX + 0)
    #define MYSURVIVOR_PERK_START_INDEX     (SURVIVOR_PERK_DATA_END_INDEX + 1)
    #define MYSURVIVOR_PERK_DATA_SOME_VAR   (MYSURVIVOR_PERK_START_INDEX + 0)
*/
