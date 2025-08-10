#pragma newdecls required

#if defined _ZF_UTIL_UI_INC
  #endinput
#endif
#define _ZF_UTIL_UI_INC

#include "zf_perk.inc"

// FX.HUD
Handle zf_hudLine0;
Handle zf_hudLine1;
Handle zf_hudLine2;
Handle zf_hudTeammateLine0;
Handle zf_hudTeammateLine1;
Handle zf_hudTeammateLine2;
Handle zf_hudTeammateLine3;
////////////////////////////////////////////////////////////
//
// Perk HUD Logic
//
////////////////////////////////////////////////////////////
void clearHud(int client)
{
  if (IsClientInGame(client))
  {
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine0, "");
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine1, "");
    SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
    ShowSyncHudText(client, zf_hudLine2, "");
    updateTeammateHud(client, 0);
  }
}

void updateHud(int client)
{
  char strHudPerk[48];
  char strHudBonuses[48];
  
  if(IsClientInGame(client) && !(zf_lastButtons[client] & IN_SCORE))
  {
    if(isSur(client) || isZom(client))
    {
      if(IsPlayerAlive(client))
      {
        // HUD Element: Perk Selection
        if(isSur(client))
        {
          char perkName[48];
          GetSurPerkDisplayName(client, prefGet(client, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else if(isZom(client))
        {
          char perkName[48];
          GetZomPerkDisplayName(client, prefGet(client, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.80, 2.1, 200, 200, 200, 150);
        if (strcmp(zf_perkStr[client], "") != 0)
          ShowSyncHudText(client, zf_hudLine0, "%s (%s)", strHudPerk, zf_perkStr[client]);
        else 
          ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        
        // HUD Element: Bonuses 1 (Attack, Defense)
        // HUD Element: Bonuses 1 (Attack, Defense)
        char s_att[10], s_def[10];
        char s_crit[10], s_speed[10];

        int att = getStat(client, ZFStatAtt);
        int def = getStat(client, ZFStatDef);
        int crit = getStat(client, ZFStatCrit);
        int speed = getStat(client, ZFStatSpeed);
        if (att > 0){
          Format(s_att, sizeof(s_att), "+%d", getStat(client, ZFStatAtt));
        } else {
          Format(s_att, sizeof(s_att), "%d", getStat(client, ZFStatAtt));
        }
        if (def > 0){
          Format(s_def, sizeof(s_def), "+%d", getStat(client, ZFStatDef));
        } else {
          Format(s_def, sizeof(s_def), "%d", getStat(client, ZFStatDef));
        }
        if (crit > 0){
          Format(s_crit, sizeof(s_crit), "+%d", getStat(client, ZFStatCrit));
        } else {
          Format(s_crit, sizeof(s_crit), "%d", getStat(client, ZFStatCrit));
        }
        if (speed > 0){
          Format(s_speed, sizeof(s_speed), "+%d", getStat(client, ZFStatSpeed));
        } else {
          Format(s_speed, sizeof(s_speed), "%d", getStat(client, ZFStatSpeed));
        }
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses1", client, s_att, s_def);
        SetHudTextParams(0.15, 0.83, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine1, "%s", strHudBonuses);
        
        // HUD Element: Bonuses 2 (Crit, Speed)
        Format(strHudBonuses, sizeof(strHudBonuses), "%T", "ZF_HUD_Bonuses2", client, s_crit, s_speed);
        SetHudTextParams(0.15, 0.86, 2.1, 150, 150, 150, 150);
        ShowSyncHudText(client, zf_hudLine2, "%s", strHudBonuses);
      }
      else
      {
        // HUD Element: Killer's perk
        int killer = zf_lastKiller[client];
        if(validClient(killer) && (killer != client))
        {
          if(isSur(killer))
          {
            char perkName[48];
            GetSurPerkDisplayName(client, prefGet(killer, SurPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else if(isZom(killer))
          {
            char perkName[48];
            GetZomPerkDisplayName(client, prefGet(killer, ZomPerk), perkName, sizeof(perkName));
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
          }
          else
          {
            strHudPerk = "";
          }
          SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
          ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);
        }
      }
    }
    else
    {
      // HUD Element: Specator target's perk
      int spectate = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
      if(validClient(spectate))
      {
        if(isSur(spectate))
        {
          char perkName[48];
          GetSurPerkDisplayName(client, prefGet(spectate, SurPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else if(isZom(spectate))
        {
          char perkName[48];
          GetZomPerkDisplayName(client, prefGet(spectate, ZomPerk), perkName, sizeof(perkName));
          Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
        }
        else
        {
          strHudPerk = "";
        }
        SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
        ShowSyncHudText(client, zf_hudLine0, "%s", strHudPerk);
      }
    }
  }
}

void updateTeammateHud(int client, int target)
{
    if (!IsClientInGame(client))
    {
        return;
    }

    if (target == 0 || !IsClientInGame(target) || !IsPlayerAlive(target) || (GetClientTeam(target) != GetClientTeam(client) && !GetConVarBool(zf_cvDebug)) || target == client)
    {
        // Clear HUD
        SetHudTextParams(0.0, 0.0, 0.01, 0, 0, 0, 0);
        ShowSyncHudText(client, zf_hudTeammateLine0, "");
        ShowSyncHudText(client, zf_hudTeammateLine1, "");
        ShowSyncHudText(client, zf_hudTeammateLine2, "");
        ShowSyncHudText(client, zf_hudTeammateLine3, "");
        return;
    }

    // Display HUD
    char buffer[128];
    char targetName[MAX_NAME_LENGTH];
    GetClientName(target, targetName, sizeof(targetName));

    // Line 0: Player Name
    SetHudTextParams(0.80, 0.40, 1.1, 200, 200, 200, 150);
    ShowSyncHudText(client, zf_hudTeammateLine0, "%s", targetName);

    // Line 1: Perk Name
    char perkName[64];
    if (isSur(target))
    {
        GetSurPerkDisplayName(client, prefGet(target, SurPerk), perkName, sizeof(perkName));
    }
    else if (isZom(target))
    {
        GetZomPerkDisplayName(client, prefGet(target, ZomPerk), perkName, sizeof(perkName));
    }
    else
    {
        perkName[0] = '\0';
    }

    if (perkName[0] != '\0' && !StrEqual(perkName, "None", false))
    {
        Format(buffer, sizeof(buffer), "%T", "ZF_HUD_Perk", client, perkName);
        SetHudTextParams(0.80, 0.43, 1.1, 200, 200, 200, 150);
        ShowSyncHudText(client, zf_hudTeammateLine1, buffer);
    }
    else
    {
        ShowSyncHudText(client, zf_hudTeammateLine1, "");
    }


    // Line 2 & 3: Stats
    char s_att[10], s_def[10];
    char s_crit[10], s_speed[10];

    int att = getStat(target, ZFStatAtt);
    int def = getStat(target, ZFStatDef);
    int crit = getStat(target, ZFStatCrit);
    int speed = getStat(target, ZFStatSpeed);

    Format(s_att, sizeof(s_att), "%s%d", att >= 0 ? "+" : "", att);
    Format(s_def, sizeof(s_def), "%s%d", def >= 0 ? "+" : "", def);
    Format(s_crit, sizeof(s_crit), "%s%d", crit >= 0 ? "+" : "", crit);
    Format(s_speed, sizeof(s_speed), "%s%d", speed >= 0 ? "+" : "", speed);

    Format(buffer, sizeof(buffer), "%T", "ZF_HUD_Bonuses1", client, s_att, s_def);
    SetHudTextParams(0.80, 0.46, 1.1, 150, 150, 150, 150);
    ShowSyncHudText(client, zf_hudTeammateLine2, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_HUD_Bonuses2", client, s_crit, s_speed);
    SetHudTextParams(0.80, 0.49, 1.1, 150, 150, 150, 150);
    ShowSyncHudText(client, zf_hudTeammateLine3, buffer);
}

////////////////////////////////////////////////////////////
//
// Perk Menu Functionality
//
////////////////////////////////////////////////////////////

#define PERKS_PER_PAGE 7

//
// Survivor Perk List Menu
//
public void panel_PrintSurPerkList(int client, bool resetPage)
{
    Menu menu = new Menu(MenuHandler_SurPerkList);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectSurPerk_Title", client);
    menu.SetTitle(title);
    menu.ExitButton = true;
    menu.ExitBackButton = true; // Enable "Back" button to go to the previous menu

    int perkCount = GetTotalSurPerks();
    for (int i = 0; i < perkCount; i++)
    {
        char sPerkIndex[16];
        IntToString(i, sPerkIndex, sizeof(sPerkIndex));
        char displayName[128];
        GetSurPerkDisplayShortDesc(client, i, displayName, sizeof(displayName));

        int limit = zf_surPerksLimit[i];
        int current = getSurPerkCount(i);
        char countStr[32];
        if (limit != -1)
        {
            Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
            StrCat(displayName, sizeof(displayName), countStr);
            menu.AddItem(sPerkIndex, displayName);
        }
        else
        {
            Format(countStr, sizeof(countStr), " [%d]", current);
            StrCat(displayName, sizeof(displayName), countStr);
            menu.AddItem(sPerkIndex, displayName);
        }
    }

    menu.Display(client, MENU_TIME_FOREVER);
}

public int MenuHandler_SurPerkList(Menu menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        int client = param1;
        char sPerkIndex[16];
        menu.GetItem(param2, sPerkIndex, sizeof(sPerkIndex));
        int perkIndex = StringToInt(sPerkIndex);
        panel_PrintSurPerkSelect(client, perkIndex);
    }
    else if (action == MenuAction_Cancel)
    {
        // If the user selected "Back", show the main menu again.
        if (param2 == MenuCancel_ExitBack)
        {
            panel_PrintMain(param1);
        }
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
    return 0;
}

public void panel_PrintZomPerkList(int client, bool resetPage)
{
    Menu menu = new Menu(MenuHandler_ZomPerkList);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Menu_SelectZomPerk_Title", client);
    menu.SetTitle(title);
    menu.ExitButton = true;
    menu.ExitBackButton = true; // Enable "Back" button to go to the previous menu

    int perkCount = GetTotalZomPerks();
    for (int i = 0; i < perkCount; i++)
    {
        char sPerkIndex[16];
        IntToString(i, sPerkIndex, sizeof(sPerkIndex));
        char displayName[128];
        GetZomPerkDisplayShortDesc(client, i, displayName, sizeof(displayName));

        int limit = zf_zomPerksLimit[i];
        int current = getZomPerkCount(i);
        char countStr[32];
        if (limit != -1)
        {
            Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
            StrCat(displayName, sizeof(displayName), countStr);
            menu.AddItem(sPerkIndex, displayName);
        }
        else
        {
            Format(countStr, sizeof(countStr), " [%d]", current);
            StrCat(displayName, sizeof(displayName), countStr);
            menu.AddItem(sPerkIndex, displayName);
        }
    }

    menu.Display(client, MENU_TIME_FOREVER);
}

public int MenuHandler_ZomPerkList(Menu menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        int client = param1;
        char sPerkIndex[16];
        menu.GetItem(param2, sPerkIndex, sizeof(sPerkIndex));
        int perkIndex = StringToInt(sPerkIndex);
        panel_PrintZomPerkSelect(client, perkIndex);
    }
    else if (action == MenuAction_Cancel)
    {
        // If the user selected "Back", show the main menu again.
        if (param2 == MenuCancel_ExitBack)
        {
            panel_PrintMain(param1);
        }
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
    return 0;
}

// 
// Survivor Perk Select Menu
//
public void panel_PrintSurPerkSelect(int client, int perk)
{
    zf_menuPerk[client] = perk; // Store the perk index for the handler

    Menu menu = new Menu(panel_HandleSurPerkSelect);
    
    char title[128];
    GetSurPerkDisplayName(client, perk, title, sizeof(title));
    menu.SetTitle(title);
    
    // Add description as a non-selectable item. We can add a newline for spacing.
    char longDescKey[1024], longDescTranslated[1024];
    GetSurPerkLongDesc(perk, longDescKey, sizeof(longDescKey));
    Format(longDescTranslated, sizeof(longDescTranslated), "%T", longDescKey, client);
    menu.AddItem(longDescTranslated, longDescTranslated, ITEMDRAW_DISABLED);


    char buffer[128];
    // Option 1: Select
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Select", client);
    int limit = zf_surPerksLimit[perk];
    bool disabled = !surPerkEnabled(perk);
    int current = getSurPerkCount(perk);
    char countStr[32];
    if (limit != -1)
    {
        if (current >= limit)
        {
            disabled = true;
        }
        Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
        StrCat(buffer, sizeof(buffer), countStr);
    }
    else
    {
        Format(countStr, sizeof(countStr), " [%d/∞]", current);
        StrCat(buffer, sizeof(buffer), countStr);
    }
    menu.AddItem("select", buffer, disabled ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);

    // Option 2: Back is handled by ExitButton
    menu.ExitButton = true;
    menu.ExitBackButton = true; // This will show "Back" on the previous menu if selected

    menu.Display(client, MENU_TIME_FOREVER);
}

public int panel_HandleSurPerkSelect(Menu menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        int client = param1;
        char info[32];
        menu.GetItem(param2, info, sizeof(info));

        if (StrEqual(info, "select"))
        {
            selectSurPerk(client, zf_menuPerk[client]);
            // Close the menu after selection
            // No need to do anything, menu will close.
        }
    }
    else if (action == MenuAction_Cancel)
    {
        // If user selected "Back", redisplay the perk list.
        if (param2 == MenuCancel_ExitBack)
        {
            panel_PrintSurPerkList(param1, true);
        }
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
    return 0;
}

// 
// Zombie Perk Select Menu
//
public void panel_PrintZomPerkSelect(int client, int perk)
{
    zf_menuPerk[client] = perk; // Store the perk index for the handler

    Menu menu = new Menu(panel_HandleZomPerkSelect);
    
    char title[128];
    GetZomPerkDisplayName(client, perk, title, sizeof(title));
    menu.SetTitle(title);

    // Add description as a non-selectable item.
    char longDescKey[1024], longDescTranslated[1024];
    GetZomPerkLongDesc(perk, longDescKey, sizeof(longDescKey));
    Format(longDescTranslated, sizeof(longDescTranslated), "%T", longDescKey, client);
    menu.AddItem(longDescTranslated, longDescTranslated, ITEMDRAW_DISABLED);

    char buffer[128];
    // Option 1: Select
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Select", client);
    int limit = zf_zomPerksLimit[perk];
    bool disabled = !zomPerkEnabled(perk);
    int current = getZomPerkCount(perk);
    char countStr[32];
    if (limit != -1)
    {
        if (current >= limit)
        {
            disabled = true;
        }
        Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
        StrCat(buffer, sizeof(buffer), countStr);
    }
    else
    {
        Format(countStr, sizeof(countStr), " [%d/∞]", current);
        StrCat(buffer, sizeof(buffer), countStr);
    }
    menu.AddItem("select", buffer, disabled ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);

    // Option 2: Back is handled by ExitButton
    menu.ExitButton = true;
    menu.ExitBackButton = true; // This will show "Back" on the previous menu if selected

    menu.Display(client, MENU_TIME_FOREVER);
}

public int panel_HandleZomPerkSelect(Menu menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        int client = param1;
        char info[32];
        menu.GetItem(param2, info, sizeof(info));

        if (StrEqual(info, "select"))
        {
            selectZomPerk(client, zf_menuPerk[client]);
        }
    }
    else if (action == MenuAction_Cancel)
    {
        // If user selected "Back", redisplay the perk list.
        if (param2 == MenuCancel_ExitBack)
        {
            panel_PrintZomPerkList(param1, true);
        }
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
    return 0;
}
