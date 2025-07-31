#if defined _zf_zombie_visuals_included
#endinput
#endif
#define _zf_zombie_visuals_included

#include <tf2attributes>

#define SPAWN_SOUND "/player/taunt_yeti_roar_first.wav"

Handle g_hEquipWearable;
bool g_bIsCosmeticZombie[MAXPLAYERS+1];

stock bool GiveVoodooItem(int client, int itemindex)
{
	if (g_hEquipWearable == null) return false;

	int soul = CreateEntityByName("tf_wearable");
	
	if (!IsValidEntity(soul))
	{
		return false;
	}
	
	char entclass[64];
	GetEntityNetClass(soul, entclass, sizeof(entclass));
	SetEntData(soul, FindSendPropInfo(entclass, "m_iItemDefinitionIndex"), itemindex);
	SetEntData(soul, FindSendPropInfo(entclass, "m_bInitialized"), 1); 	
	SetEntData(soul, FindSendPropInfo(entclass, "m_iEntityLevel"), 6);
	SetEntData(soul, FindSendPropInfo(entclass, "m_iEntityQuality"), 13);
	SetEntProp(soul, Prop_Send, "m_bValidatedAttachedEntity", 1);		
	
	DispatchSpawn(soul);
	SDKCall(g_hEquipWearable, client, soul);
	return true;
}

public Action Timer_ApplyZombieAttributes(Handle timer, int client)	
{
	if (validClient(client) && IsPlayerAlive(client) && isZom(client))
	{
		TF2Attrib_SetByName(client, "player skin override", 1.0);
		TF2Attrib_SetByName(client, "zombiezombiezombiezombie", 1.0);
		TF2Attrib_SetByName(client, "SPELL: Halloween voice modulation", 1.0);		
		SetEntProp(client, Prop_Send, "m_bForcedSkin", 1);

        int skin = 0;
        // Per user request, BLU team are zombies.
        // Skin values from original script: 5 for BLU, 23 for BLU spy.
        skin = (TF2_GetPlayerClass(client) == TFClass_Spy) ? 23 : 5;
		SetEntProp(client, Prop_Send, "m_nForcedSkin", skin);
	}
	return Plugin_Handled;	
}

stock void ApplyZombieVisuals(int client)
{
    if (!validClient(client) || !isZom(client)) return;

	switch(TF2_GetPlayerClass(client))
	{
        case TFClass_Scout: GiveVoodooItem(client, 5617);
        case TFClass_Soldier: GiveVoodooItem(client, 5618);
        case TFClass_Pyro: GiveVoodooItem(client, 5624);
        case TFClass_DemoMan: GiveVoodooItem(client, 5620);
        case TFClass_Heavy: GiveVoodooItem(client, 5619);
        case TFClass_Engineer: GiveVoodooItem(client, 5621);
        case TFClass_Medic: GiveVoodooItem(client, 5622);
        case TFClass_Sniper: GiveVoodooItem(client, 5625);
        case TFClass_Spy: GiveVoodooItem(client, 5623);
	}

	g_bIsCosmeticZombie[client] = true;	
	
	CreateTimer(0.1, Timer_ApplyZombieAttributes, client, TIMER_FLAG_NO_MAPCHANGE);
}

stock void RemoveZombieVisuals(int client)
{
	if (validClient(client) && g_bIsCosmeticZombie[client])
	{
		TF2Attrib_SetByName(client, "player skin override", 0.0);
		TF2Attrib_SetByName(client, "zombiezombiezombiezombie", 0.0);
		TF2Attrib_SetByName(client, "SPELL: Halloween voice modulation", 0.0);			
		SetEntProp(client, Prop_Send, "m_bForcedSkin", 0);
		SetEntProp(client, Prop_Send, "m_nForcedSkin", 0);			

		g_bIsCosmeticZombie[client] = false;
        
		if (IsPlayerAlive(client)) {
		    ForcePlayerSuicide(client);
        }
	}
}

stock void ZombieVisuals_OnPlayerInventory(int client) 
{
	if(g_bIsCosmeticZombie[client])
	{
		if(!TF2_IsPlayerInCondition(client, TFCond_Disguised))
		{	
			ApplyZombieVisuals(client);
		}
	}
}

stock void ZombieVisuals_OnConditionAdded(int client, TFCond cond)
{
	if(g_bIsCosmeticZombie[client] && cond == TFCond_Disguised)
	{
		TF2Attrib_SetByName(client, "player skin override", 0.0);
		TF2Attrib_SetByName(client, "zombiezombiezombiezombie", 0.0);
		TF2Attrib_SetByName(client, "SPELL: Halloween voice modulation", 0.0);			
		SetEntProp(client, Prop_Send, "m_bForcedSkin", 0);
		SetEntProp(client, Prop_Send, "m_nForcedSkin", 0);		
	}
}

stock void ZombieVisuals_OnConditionRemoved(int client, TFCond cond)
{
	if(g_bIsCosmeticZombie[client] && cond == TFCond_Disguised)
	{
		ApplyZombieVisuals(client);	
	}
}

stock void InitZombieVisuals()
{
    GameData hTF2 = new GameData("sm-tf2.games");
	if (!hTF2) {
		SetFailState("Missing gamedata file 'sm-tf2.games'.");
        return;
    }

	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetVirtual(hTF2.GetOffset("RemoveWearable") - 1);
	PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
	g_hEquipWearable = EndPrepSDKCall();

	if (!g_hEquipWearable) {
		SetFailState("Failed to create call: CBasePlayer::EquipWearable");
    }

	delete hTF2;

    PrecacheSound(SPAWN_SOUND, true);
}