#if defined __weapondata_0_included
 #endinput
#endif
#define __weapondata_0_included


/*
    GetMaxAmmo & GetMaxClip
    By: Chdata

    Only set up for TF2 as of now.

    It is required to place OnPluginStart_RegisterWeaponData() into OnPluginStart(), or else the functions won't work.
*/

static Handle:fnGetMaxAmmo;
static Handle:fnGetMaxClip1;
static Handle:fnGetDefaultClip1;
static Handle:fnGetSlot;

stock GetMaxAmmo(iClient, iAmmoType, TFClassType:iClass)
{ 
    if (iAmmoType == -1 || !iClass) // Means the weapon doesn't use ammo (or you haven't chosen a class) and this function will merely return garbage values.
    {
        return -1;
    }

    if (fnGetMaxAmmo == INVALID_HANDLE)
    {
        LogError("SDKCall for GetMaxAmmo is invalid!");
        return -1;
    }
     
    return SDKCall(fnGetMaxAmmo, iClient, iAmmoType, iClass);
}

stock GetWeaponMaxAmmo(iClient, iWeapon)
{
    return GetMaxAmmo(iClient, GetEntProp(iWeapon, Prop_Send, "m_iPrimaryAmmoType", 1), TF2_GetPlayerClass(iClient));
}

stock GetSlotMaxAmmo(iClient, iSlot, TFClassType:iClass = TFClass_Unknown)
{
    new iWep = GetPlayerWeaponSlot(iClient, iSlot);
    if (iWep == -1)
    {
        return -1;
    }

    if (iClass == TFClass_Unknown)
    {
        iClass = TF2_GetPlayerClass(iClient);
    }

    return GetMaxAmmo(iClient, GetEntProp(iWep, Prop_Send, "m_iPrimaryAmmoType", 1), iClass);
}

/*
    Remember to validate the weapon entity before passing it.
*/
stock GetMaxClip(iWeapon)
{ 
    if (fnGetMaxClip1 == INVALID_HANDLE)
    {
        LogError("SDKCall for GetMaxClip1 is invalid!");
        return -1;
    }
     
    return SDKCall(fnGetMaxClip1, iWeapon);
}

/*
    Generally seems to always be the same as GetMaxClip
*/
stock GetDefaultClip(iWeapon)
{ 
    if (fnGetDefaultClip1 == INVALID_HANDLE)
    {
        LogError("SDKCall for GetDefaultClip1 is invalid!");
        return -1;
    }
     
    return SDKCall(fnGetDefaultClip1, iWeapon);
}

/*
    Gets the weapon slot the weapon is in; identical to the TFWeaponSlot_ values
*/
stock GetSlot(iWeapon)
{ 
    if (fnGetSlot == INVALID_HANDLE)
    {
        LogError("SDKCall for GetSlot is invalid!");
        return -1;
    }
     
    return SDKCall(fnGetSlot, iWeapon);
}

stock OnPluginStart_RegisterWeaponData()
{
    new Handle:hGameData = LoadGameConfigFile("weapon.data");
    if (hGameData == INVALID_HANDLE)
    {
        SetFailState("Unable to load required gamedata (weapon.data.txt)");
    }
    
    //---------------------

    StartPrepSDKCall(SDKCall_Player);
    PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CTFPlayer::GetMaxAmmo");
    PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
    PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
    PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);

    fnGetMaxAmmo = EndPrepSDKCall();
    if (fnGetMaxAmmo == INVALID_HANDLE)
    {
        SetFailState("Couldn't load SDK function (CTFPlayer::GetMaxAmmo). SDK call failed");
    }

    //---------------------

    StartPrepSDKCall(SDKCall_Entity);
    PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CTFWeaponBase::GetMaxClip1");
    PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);

    fnGetMaxClip1 = EndPrepSDKCall();
    if (fnGetMaxClip1 == INVALID_HANDLE)
    {
        SetFailState("Couldn't load SDK function (CTFWeaponBase::GetMaxClip1). SDK call failed");
    }

    //---------------------

    StartPrepSDKCall(SDKCall_Entity);
    PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CTFWeaponBase::GetDefaultClip1");
    PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);

    fnGetDefaultClip1 = EndPrepSDKCall();
    if (fnGetDefaultClip1 == INVALID_HANDLE)
    {
        SetFailState("Couldn't load SDK function (CTFWeaponBase::GetDefaultClip1). SDK call failed");
    }

    //---------------------

    StartPrepSDKCall(SDKCall_Entity);
    PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CBaseCombatWeapon::GetSlot");
    PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);

    fnGetSlot = EndPrepSDKCall();
    if (fnGetSlot == INVALID_HANDLE)
    {
        SetFailState("Couldn't load SDK function (CBaseCombatWeapon::GetSlot). SDK call failed");
    }

    //---------------------
    
    CloseHandle(hGameData);
}
