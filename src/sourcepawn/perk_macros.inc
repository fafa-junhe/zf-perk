
#pragma newdecls required



#if defined _perk_macros_included
#endinput
#endif
#define _perk_macros_included


// --- DataPack Vector Property Macros (Size 3) ---
// NOTE: These are currently hardcoded for size 3 vectors (e.g., float[3]).

/**
 * Reads an expanded vector property from a DataPack object into a local array.
 *
 * @param dp_obj        The DataPack object instance (e.g., _inst).
 * @param prop_name     The base name of the property (e.g., decoy_pos).
 * @param array_var     The local array variable (e.g., float my_vec[3]) to read into.
 */
#define DP_READ_VECTOR_PROP(%1,%2,%3) {%3[0] = %1.%2_1; %3[1] = %1.%2_2; %3[2] = %1.%2_3;}IsCharAlpha(0)

/**
 * Writes a local array into an expanded vector property of a DataPack object.
 *
 * @param dp_obj        The DataPack object instance (e.g., _inst).
 * @param prop_name     The base name of the property (e.g., decoy_pos).
 * @param array_var     The local array variable (e.g., float my_vec[3]) to write from.
 */
#define DP_WRITE_VECTOR_PROP(%1,%2,%3) {%1.%2_1 = %3[0]; %1.%2_2 = %3[1]; %1.%2_3 = %3[2];}IsCharAlpha(0)

#define RegisterPerk(%1,%2,%3,%4,%5) \
        { \
            %1 perk = new %1(-1); \
            char classname[64]; \
            perk.getName(classname, sizeof(classname)); \
            LogMessage("[ZF] Register %s perk with classname %s", %5, classname); \
            if (%2.ContainsKey(classname)){ \
                LogError("[ZF] Attempted to register %s perk with duplicate classname: '%s'", %5, classname); \
            } \
            else { \
                StringMap perkInfo = new StringMap(); \
                char shortDesc[128]; \
                perk.getShortdesc(shortDesc, sizeof(shortDesc)); \
                char longDesc[1024]; \
                perk.getDesc(longDesc, sizeof(longDesc)); \
                perkInfo.SetString("name", classname); \
                perkInfo.SetString("shortDesc", shortDesc); \
                perkInfo.SetString("longDesc", longDesc); \
                %2.SetValue(classname, perkInfo); \
                %3.Push(perkInfo); \
                %4.Push(view_as<Function>(%1Fnew)); \
                LogMessage("[ZF] %s perk: classname %s shortDesc %s longDesc", %5, classname, shortDesc, longDesc); \
            } \
        } LogMessage("[ZF] register successed.")

#define RegisterSurvivorPerk(%1) RegisterPerk(%1, g_SurPerkRegistry, g_SurPerkTypes, g_SurPerkCreators, "survivor")
#define RegisterZombiePerk(%1) RegisterPerk(%1, g_ZomPerkRegistry, g_ZomPerkTypes, g_ZomPerkCreators, "zombie")


#define PERK_REGISTER_VTABLE(%0,%1,%2) \
    (%0).Position = view_as<DataPackPos>(%1); \
    (%0).WriteFunction(%2)

#define START_VTABLE_CALL(%0) \
    this.Position = view_as<DataPackPos>(%0); \
    Function func = this.ReadFunction(); \
    this.Reset(); \
    Call_StartFunction(INVALID_HANDLE, func); \
    Call_PushCell(this)

#define PERK_IMPL_GET_STRING(%0,%1,%2) \
    START_VTABLE_CALL(%0); \
    Call_PushStringEx(%1, %2, SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK); \
    Call_PushCell(%2); \
    Call_Finish()

#define PERK_IMPL_EVENT_VOID(%0) \
    START_VTABLE_CALL(%0); \
    Call_Finish()

#define PERK_IMPL_EVENT_CELL(%0,%1) \
    START_VTABLE_CALL(%0); \
    Call_PushCell(%1); \
    Call_Finish()

#define PERK_IMPL_EVENT_CELL_CELL(%0,%1,%2) \
    START_VTABLE_CALL(%0); \
    Call_PushCell(%1); \
    Call_PushCell(%2); \
    Call_Finish()

#define PERK_IMPL_onPlayerRunCmd(%0,%1,%2,%3,%4) \
    START_VTABLE_CALL(VTABLE_ON_PLAYER_RUN_CMD); \
    Call_PushCell(%0); \
    Call_PushCell(%1); \
    Call_PushArray(%2, sizeof(%2)); \
    Call_PushArray(%3, sizeof(%3)); \
    Call_PushCell(%4); \
    Call_Finish()

#define PERK_IMPL_onPlayerDeath(%0,%1,%2,%3,%4) \
    START_VTABLE_CALL(VTABLE_ON_PLAYER_DEATH); \
    Call_PushCell(%0); \
    Call_PushCell(%1); \
    Call_PushCell(%2); \
    Call_PushCell(%3); \
    Call_PushCell(%4); \
    Call_Finish()

#define PERK_IMPL_onTakeDamage(%0,%1,%2,%3,%4) \
    float __retval; \
    START_VTABLE_CALL(VTABLE_ON_TAKE_DAMAGE); \
    Call_PushCell(%0); \
    Call_PushCell(%1); \
    Call_PushCell(%2); \
    Call_PushFloat(%3); \
    Call_PushCell(%4); \
    Call_Finish(__retval); \
    return __retval

#define PERK_IMPL_onTakeDamagePost(%0,%1,%2,%3,%4) \
    START_VTABLE_CALL(VTABLE_ON_TAKE_DAMAGE_POST); \
    Call_PushCell(%0); \
    Call_PushCell(%1); \
    Call_PushCell(%2); \
    Call_PushFloat(%3); \
    Call_PushCell(%4); \
    Call_Finish()

#define PERK_IMPL_updateCondStats(%0,%1) \
    START_VTABLE_CALL(VTABLE_UPDATE_COND_STATS); \
    Call_PushStringEx(%0, %1, SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK); \
    Call_PushCell(%1); \
    Call_Finish()

#define PERK_IMPL_doItemThrow(%0,%1,%2) \
    START_VTABLE_CALL(VTABLE_DO_ITEM_THROW); \
    Call_PushString(%0); \
    Call_PushFloat(%1); \
    Call_PushArray(%2, sizeof(%2)); \
    Call_Finish()

/**
 * Creates a perk function definition.
 *
 * @param %1    Perk methodmap name (e.g. JuggernautPerk)
 * @param %2    Function name (e.g. getName)
 *
 * Usage:
 * FUNCTION(MyPerk, MyFunction)) // for no arguments
 * FUNCTION(MyPerk, MyFunction), arg1, arg2) // for arguments
 */
#define FUNCTION(%1,%2) public void %1F%2(%1 _inst
#define FUNCTION_FLOAT(%1,%2) public float %1F%2(%1 _inst