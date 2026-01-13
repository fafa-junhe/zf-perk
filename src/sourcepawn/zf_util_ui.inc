#pragma newdecls required

#if defined _ZF_UTIL_UI_INC
  #endinput
#endif
#define _ZF_UTIL_UI_INC

#include <sdkhooks>

#include <sdktools>

#include <sourcemod>
#include <halflife>
#include "zf_perk.inc"

#define PLUGIN_VERSION "4.5.4fix1"

Handle zf_hudLine0;
Handle zf_hudLine1;
Handle zf_hudLine2;
Handle zf_hudAtt;
Handle zf_hudDef;
Handle zf_hudCrit;
Handle zf_hudSpeed;
int         g_iDebugTarget[MAXPLAYERS + 1];
static int  g_iInspectTarget[MAXPLAYERS + 1];
int         g_iDebugStat[MAXPLAYERS + 1];

/**
 * 格式化统计数值字符串
 * @param value 要格式化的数值
 * @param buffer 存储结果的缓冲区
 * @param maxlen 缓冲区最大长度
 */
stock void FormatStatString(int value, char[] buffer, int maxlen)
{
    Format(buffer, maxlen, "%s%d%%", value > 0 ? "+" : "", value);
}

/**
 * 根据统计数值获取HUD颜色分量
 * @param value 统计数值
 * @param r 红色分量(输出)
 * @param g 绿色分量(输出)
 * @param b 蓝色分量(输出)
 */
stock void GetStatColorComponents(int value, int &r, int &g, int &b)
{
    r = 255, g = 255, b = 255;
    float factor = 0.0;

    if (value > 0)
    {
        // Positive: White -> Green
        factor = float(value) / 50.0;
        if (factor > 1.0) factor = 1.0;
        r = RoundFloat(255.0 * (1.0 - factor));
        b = RoundFloat(255.0 * (1.0 - factor));
    }
    else if (value < 0)
    {
        // Negative: White -> Red
        factor = float(value) / -50.0;
        if (factor > 1.0) factor = 1.0;
        g = RoundFloat(255.0 * (1.0 - factor));
        b = RoundFloat(255.0 * (1.0 - factor));
    }
}


/**
 * 内部帮助函数，用于显示单个统计HUD项目
 */
stock void _ShowSingleStatHud(int client, Handle hudHandle, float x, float y, int value, const char[] transKey, int alpha)
{
    char s_val[10];
    char buffer[128];
    int r, g, b;
    FormatStatString(value, s_val, sizeof(s_val));
    Format(buffer, sizeof(buffer), "\x04%T \x03%s", transKey, client, s_val);
    GetStatColorComponents(value, r, g, b);
    SetHudTextParams(x, y, 1.1, r, g, b, alpha, 0, 0.0, 1.0, 0.1);
    ShowSyncHudText(client, hudHandle, buffer);
}

/**
 * 显示玩家统计HUD
 * @param client 客户端索引
 * @param x X坐标
 * @param y Y坐标
 * @param att 攻击值
 * @param def 防御值
 * @param crit 暴击值
 * @param speed 速度值
 * @param alpha 透明度
 */
stock void ShowPlayerStatsHud(int client, float x, float y, int att, int def, int crit, int speed, int alpha = 150)
{
    float y_offset = 0.04; // Y轴偏移

    // 使用帮助函数来简化显示
    _ShowSingleStatHud(client, zf_hudAtt,   x,         y,                  att,   "ZF_HUD_Attack",  alpha);
    _ShowSingleStatHud(client, zf_hudDef,   x + 0.1,   y - 0.001,          def,   "ZF_HUD_Defense", alpha);
    _ShowSingleStatHud(client, zf_hudCrit,  x,         y + y_offset,       crit,  "ZF_HUD_Crit",    alpha);
    _ShowSingleStatHud(client, zf_hudSpeed, x + 0.1,   y + y_offset - 0.001, speed, "ZF_HUD_Speed",   alpha);
}
// FX.HUD
void utilUiInit()
{
    zf_hudLine0 = CreateHudSynchronizer();
    zf_hudLine1 = CreateHudSynchronizer();
    zf_hudLine2 = CreateHudSynchronizer();
    zf_hudAtt = CreateHudSynchronizer();
    zf_hudDef = CreateHudSynchronizer();
    zf_hudCrit = CreateHudSynchronizer();
    zf_hudSpeed = CreateHudSynchronizer();
}
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

/**
 * 获取指定玩家的Perk显示名称，用于HUD
 * @param client        观察者客户端
 * @param target        目标玩家
 * @param perkName      存储Perk名称的缓冲区
 * @param maxlen        缓冲区长度
 * @return              如果成功获取到Perk名称则返回true，否则返回false
 */
stock bool _GetPlayerPerkDisplayName(int client, int target, char[] perkName, int maxlen)
{
    if (isSur(target))
    {
        GetPerkDisplayName(client, prefGet(target, SurPerk), false, perkName, maxlen);
        return true;
    }
    else if (isZom(target))
    {
        GetPerkDisplayName(client, prefGet(target, ZomPerk), true, perkName, maxlen);
        return true;
    }
    perkName[0] = '\0';
    return false;
}

void updateHud(int client)
{
    if (!IsClientInGame(client) || (zf_lastButtons[client] & IN_SCORE))
        return;

    char strHudPerk[128];
    char perkName[64];
    int target = g_iInspectTarget[client];

    // 如果正在检视队友，则显示队友信息
    if (target > 0 && IsClientInGame(target) && IsPlayerAlive(target))
    {
        if (_GetPlayerPerkDisplayName(client, target, perkName, sizeof(perkName)))
        {
            char targetName[MAX_NAME_LENGTH];
            GetClientName(target, targetName, sizeof(targetName));
           char inspectingText[128];
           Format(inspectingText, sizeof(inspectingText), "%T", "ZF_HUD_Inspecting", client, targetName);
           SetHudTextParams(0.15, 0.76, 2.1, 200, 200, 200, 150);
           ShowSyncHudText(client, zf_hudLine1, inspectingText);

           Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
           SetHudTextParams(0.15, 0.80, 2.1, 200, 200, 200, 150);
           if (zf_perkStr[target][0] != '\0')
               ShowSyncHudText(client, zf_hudLine0, "%s (%s)", strHudPerk, zf_perkStr[target]);
           else
               ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        }

        int att = getStat(target, ZFStatAtt);
        int def = getStat(target, ZFStatDef);
        int crit = getStat(target, ZFStatCrit);
        int speed = getStat(target, ZFStatSpeed);
        ShowPlayerStatsHud(client, 0.14, 0.84, att, def, crit, speed, 150);
    }
    // 玩家存活: 显示自己的Perk和状态
    else if (IsPlayerAlive(client) && (isSur(client) || isZom(client)))
    {
       ShowSyncHudText(client, zf_hudLine1, ""); // 清除检视行
        if (_GetPlayerPerkDisplayName(client, client, perkName, sizeof(perkName)))
        {
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Perk", client, perkName);
            SetHudTextParams(0.15, 0.80, 2.1, 200, 200, 200, 150);
            if (zf_perkStr[client][0] != '\0')
                ShowSyncHudText(client, zf_hudLine0, "%s (%s)", strHudPerk, zf_perkStr[client]);
            else
                ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        }

        int att = getStat(client, ZFStatAtt);
        int def = getStat(client, ZFStatDef);
        int crit = getStat(client, ZFStatCrit);
        int speed = getStat(client, ZFStatSpeed);
        ShowPlayerStatsHud(client, 0.14, 0.84, att, def, crit, speed, 150);
    }
    // 玩家死亡: 显示击杀者的Perk
    else if (!IsPlayerAlive(client))
    {
        int killer = zf_lastKiller[client];
        if (validClient(killer) && killer != client && _GetPlayerPerkDisplayName(client, killer, perkName, sizeof(perkName)))
        {
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_KilledBy", client, perkName);
            SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
            ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        }
    }
    // 观察者: 显示观察目标的Perk
    else
    {
        int spectate = GetEntPropEnt(client, Prop_Send, "m_hObserverTarget");
        if (validClient(spectate) && _GetPlayerPerkDisplayName(client, spectate, perkName, sizeof(perkName)))
        {
            Format(strHudPerk, sizeof(strHudPerk), "%T", "ZF_HUD_Spectating", client, perkName);
            SetHudTextParams(0.15, 0.80, 2.1, 250, 200, 200, 150);
            ShowSyncHudText(client, zf_hudLine0, strHudPerk);
        }
    }
}

void updateTeammateHud(int client, int target)
{
    if (!IsClientInGame(client))
        return;

    // 条件检查，如果不满足则清除目标
    if (target <= 0 || !IsClientInGame(target) || !IsPlayerAlive(target) || (GetClientTeam(target) != GetClientTeam(client) && !GetConVarBool(zf_cvDebug)) || target == client)
    {
        g_iInspectTarget[client] = 0;
        return;
    }

    // 设置检视目标
    g_iInspectTarget[client] = target;
}

////////////////////////////////////////////////////////////
//
// Perk Menu Functionality
//
////////////////////////////////////////////////////////////

#define PERKS_PER_PAGE 7

int zf_menuCategory[MAXPLAYERS + 1];

//
// Generic Perk List Menu
//
public void panel_PrintPerkCategoryList(int client, bool isZombie)
{
    // Force update perk availability right before showing the menu
    SZF_UpdatePerkAvailability();

    Menu menu = new Menu(isZombie ? MenuHandler_ZomPerkCategoryList : MenuHandler_SurPerkCategoryList);

    char title[128];
    Format(title, sizeof(title), "%T", isZombie ? "ZF_Menu_SelectZomPerk_Title" : "ZF_Menu_SelectSurPerk_Title", client);
    menu.SetTitle(title);
    menu.ExitButton = true;
    menu.ExitBackButton = true;

    // Display categories
    for (int i = 1; i <= 5; i++)
    {
        if (!isZombie && i == 4) { continue;}
        char sCategoryIndex[16];
        IntToString(i, sCategoryIndex, sizeof(sCategoryIndex));
        char categoryNameKey[64];
        Format(categoryNameKey, sizeof(categoryNameKey), "ZF_PerkCategory_%d", i);
        char categoryName[128];
        Format(categoryName, sizeof(categoryName), "%T", categoryNameKey, client);
        
        menu.AddItem(sCategoryIndex, categoryName);
    }

    menu.Display(client, MENU_TIME_FOREVER);
}

public void panel_PrintPerkList(int client, bool isZombie, int category)
{
    Menu menu = new Menu(isZombie ? MenuHandler_ZomPerkList : MenuHandler_SurPerkList);

    char title[256];
    char categoryDescKey[64];
    Format(categoryDescKey, sizeof(categoryDescKey), "ZF_PerkCategory_%d_Desc", category);
    Format(title, sizeof(title), "%T", categoryDescKey, client);
    menu.SetTitle(title);
    menu.ExitButton = true;
    menu.ExitBackButton = true;

    ArrayList perkTypes = isZombie ? g_ZomPerkTypes : g_SurPerkTypes;
    int perkCount = perkTypes.Length;

    for (int i = 0; i < perkCount; i++)
    {
        StringMap perkInfo = view_as<StringMap>(perkTypes.Get(i));
        int perkCategory;
        perkInfo.GetValue("category", perkCategory);

        if (perkCategory == category)
        {
            if (isZombie)
            {
                if (!zomPerkEnabled(i))
                    continue;
            }
            else
            {
                if (!surPerkEnabled(i))
                    continue;
            }
            char perkName[64];
            if (isZombie)
                GetPerkName(i, true, perkName, sizeof(perkName));
            else
                GetPerkName(i, false, perkName, sizeof(perkName));


            char sPerkIndex[16];
            IntToString(i, sPerkIndex, sizeof(sPerkIndex));
            char displayName[128];
            if (isZombie)
                GetPerkDisplayShortDesc(client, i, true, displayName, sizeof(displayName));
            else
                GetPerkDisplayShortDesc(client, i, false, displayName, sizeof(displayName));

            int limit = isZombie ? zf_zomPerksLimit[i] : zf_surPerksLimit[i];
            int current = isZombie ? getZomPerkCount(i) : getSurPerkCount(i);
            char countStr[32];
            if (limit != -1)
                Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
            else
                Format(countStr, sizeof(countStr), " [%d]", current);
            
            StrCat(displayName, sizeof(displayName), countStr);
            menu.AddItem(sPerkIndex, displayName);
        }
    }

    menu.Display(client, MENU_TIME_FOREVER);
}

// Internal handler for perk list selection
void _MenuHandler_PerkList_Internal(Menu menu, MenuAction action, int client, int item, bool isZombie)
{
    if (action == MenuAction_Select)
    {
        char sPerkIndex[16];
        menu.GetItem(item, sPerkIndex, sizeof(sPerkIndex));
        int perkIndex = StringToInt(sPerkIndex);
        panel_PrintPerkSelect(client, perkIndex, isZombie, zf_menuCategory[client]);
    }
    else if (action == MenuAction_Cancel && item == MenuCancel_ExitBack)
    {
        panel_PrintPerkCategoryList(client, isZombie);
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
}

public int MenuHandler_SurPerkList(Menu menu, MenuAction action, int client, int item) {
    _MenuHandler_PerkList_Internal(menu, action, client, item, false);
    return 0;
}

public int MenuHandler_ZomPerkList(Menu menu, MenuAction action, int client, int item) {
    _MenuHandler_PerkList_Internal(menu, action, client, item, true);
    return 0;
}

void _MenuHandler_PerkCategoryList_Internal(Menu menu, MenuAction action, int client, int item, bool isZombie)
{
    if (action == MenuAction_Select)
    {
        char sCategoryIndex[16];
        menu.GetItem(item, sCategoryIndex, sizeof(sCategoryIndex));
        int categoryIndex = StringToInt(sCategoryIndex);
        zf_menuCategory[client] = categoryIndex;
        panel_PrintPerkList(client, isZombie, categoryIndex);
    }
    else if (action == MenuAction_Cancel && item == MenuCancel_ExitBack)
    {
        panel_PrintMain(client);
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
}

public int MenuHandler_SurPerkCategoryList(Menu menu, MenuAction action, int client, int item) {
    _MenuHandler_PerkCategoryList_Internal(menu, action, client, item, false);
    return 0;
}

public int MenuHandler_ZomPerkCategoryList(Menu menu, MenuAction action, int client, int item) {
    _MenuHandler_PerkCategoryList_Internal(menu, action, client, item, true);
    return 0;
}

//
// Generic Perk Select Menu
//
public void panel_PrintPerkSelect(int client, int perk, bool isZombie, int category)
{
    zf_menuPerk[client] = perk; // Store perk index for the handler
    zf_menuCategory[client] = category; // Store category for back button

    Menu menu = new Menu(isZombie ? panel_HandleZomPerkSelect : panel_HandleSurPerkSelect);

    char title[128];
    char longDescKey[1024], longDescTranslated[1024];
    char buffer[128];
    char countStr[32];
    int limit, current;
    bool disabled;

    if (isZombie)
    {
        GetPerkDisplayName(client, perk, true, title, sizeof(title));
        GetPerkLongDesc(perk, true, longDescKey, sizeof(longDescKey));
        limit = zf_zomPerksLimit[perk];
        current = getZomPerkCount(perk);
        disabled = !zomPerkEnabled(perk);
    }
    else
    {
        GetPerkDisplayName(client, perk, false, title, sizeof(title));
        GetPerkLongDesc(perk, false, longDescKey, sizeof(longDescKey));
        limit = zf_surPerksLimit[perk];
        current = getSurPerkCount(perk);
        disabled = !surPerkEnabled(perk);
    }

    menu.SetTitle(title);

    // Description
    Format(longDescTranslated, sizeof(longDescTranslated), "%T", longDescKey, client);
    menu.AddItem("", longDescTranslated, ITEMDRAW_DISABLED);

    // Select option
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkSelect_Select", client);
    if (limit != -1)
    {
        if (current >= limit) disabled = true;
        Format(countStr, sizeof(countStr), " [%d/%d]", current, limit);
    }
    else
    {
        Format(countStr, sizeof(countStr), " [%d/∞]", current);
    }
    StrCat(buffer, sizeof(buffer), countStr);
    menu.AddItem("select", buffer, disabled ? ITEMDRAW_DISABLED : ITEMDRAW_DEFAULT);

    menu.ExitButton = true;
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

// Internal handler for perk selection
void _panel_HandlePerkSelect_Internal(Menu menu, MenuAction action, int client, int item, bool isZombie)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        menu.GetItem(item, info, sizeof(info));
        if (StrEqual(info, "select"))
        {
            if (isZombie)
                selectZomPerk(client, zf_menuPerk[client]);
            else
                selectSurPerk(client, zf_menuPerk[client]);
        }
    }
    else if (action == MenuAction_Cancel && item == MenuCancel_ExitBack)
    {
        panel_PrintPerkList(client, isZombie, zf_menuCategory[client]);
    }
    else if (action == MenuAction_End)
    {
        delete menu;
    }
}

public int panel_HandleSurPerkSelect(Menu menu, MenuAction action, int client, int item) {
    _panel_HandlePerkSelect_Internal(menu, action, client, item, false);
    return 0;
}

public int panel_HandleZomPerkSelect(Menu menu, MenuAction action, int client, int item) {
    _panel_HandlePerkSelect_Internal(menu, action, client, item, true);
    return 0;
}

////////////////////////////////////////////////////////////
//
// Help Functionality
//
////////////////////////////////////////////////////////////
public void help_printZFInfoChat(int client)
{
    if (client != 0){
        PrintToChat(client, "%t", "ZF_MenuHint");
    }
}

////////////////////////////////////////////////////////////
//
// Main Menu Functionality
//
////////////////////////////////////////////////////////////

//
// Main //edit:2023.6.29 by spectator
//
public void panel_PrintMain(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Title", client, PLUGIN_VERSION);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_SelectSurPerk", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_SelectZomPerk", client);
    DrawPanelItem(panel, buffer, 0);


    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Help", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_PerkHelp", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Credits", client);
    

    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Change_Language", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleMain, 30);
    CloseHandle(panel);
}

public void panel_HandleMain(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1: // Select Survivor Perk
            {
                panel_PrintPerkCategoryList(param1, false);
                return;
            }
            case 2: // Select Zombie Perk
            {
                panel_PrintPerkCategoryList(param1, true);
                return;
            }
            case 3:
            {
                panel_PrintHelp(param1);
                return;
            }
            case 4:
            {
                panel_PrintPerkHelp(param1);
                return;
            }
            case 5:
            {
                panel_PrintCredits(param1);
                return;
            }
            case 6:
            {
                panel_ChangeLanguage(param1);
                return;
            }
            case 7:    // Close
            {
                return;
            }
        }
    }
}


//
// Main.Help
//
public void panel_PrintHelp(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Help", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Overview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_SurvivorOverview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ZombieOverview", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ClassSurvivor", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_ClassZombie", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelp, 30);
    CloseHandle(panel);
}

public void panel_HandleHelp(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintHelpOverview(param1);
                return;
            }
            case 2:
            {
                panel_PrintHelpTeam(param1, surTeam());
                return;
            }
            case 3:
            {
                panel_PrintHelpTeam(param1, zomTeam());
                return;
            }
            case 4:
            {
                panel_PrintHelpSurClass(param1);
                return;
            }
            case 5:
            {
                panel_PrintHelpZomClass(param1);
                return;
            }
            case 6:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Overview
//
public void panel_PrintHelpOverview(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_Overview", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    char fullText[512];
    Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Overview_Full", client);
    char lines[4][128];
    int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
    for (int i = 0; i < numLines; i++)
    {
        DrawPanelText(panel, lines[i]);
    }

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpOverview, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpOverview(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Team
//
public void panel_PrintHelpTeam(int client, int team)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    if (team == surTeam())
    {
        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_SurvivorTeam", client);
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_SurvivorClasses_Full", client);
        DrawPanelText(panel, buffer);

        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_SurvivorWeapons", client);
        DrawPanelText(panel, buffer);

        DrawPanelText(panel, "----------------------------------------");
    }
    else if (team == zomTeam()) {
        Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_ZombieTeam", client);
        SetPanelTitle(panel, buffer, false);

        DrawPanelText(panel, "----------------------------------------");

        char fullText[512];
        Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_ZombieTeam_Full", client);
        char lines[4][128];
        int  numLines = ExplodeString(fullText, "\n", lines, 4, 128);
        for (int i = 0; i < numLines; i++)
        {
            DrawPanelText(panel, lines[i]);
        }

        DrawPanelText(panel, "----------------------------------------");
    }
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpTeam, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpTeam(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.Help.Class
//
public void panel_PrintHelpSurClass(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_SurvivorClasses", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Soldier", client);
    DrawPanelItem(panel, buffer, 0);


    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Medic", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Demoman", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Pyro", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Engineer", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpSurClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpSurClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintClass(param1, TFClass_Soldier);
                return;
            }
            case 3:
            {
                panel_PrintClass(param1, TFClass_Medic);
                return;
            }
            case 4:
            {
                panel_PrintClass(param1, TFClass_DemoMan);
                return;
            }
            case 5:
            {
                panel_PrintClass(param1, TFClass_Pyro);
                return;
            }
            case 6:
            {
                panel_PrintClass(param1, TFClass_Engineer);
                return;
            }
            case 7:    // Close
            {
                return;
            }
        }
    }
}

public void panel_PrintHelpZomClass(int client)
{
    Handle panel = CreatePanel();
    char   buffer[128];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_ZombieClasses", client);
    SetPanelTitle(panel, buffer, false);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Scout", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Heavy", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Spy", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Class_Sniper", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleHelpZomClass, 30);
    CloseHandle(panel);
}

public void panel_HandleHelpZomClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:
            {
                panel_PrintClass(param1, TFClass_Scout);
                return;
            }
            case 2:
            {
                panel_PrintClass(param1, TFClass_Heavy);
                return;
            }
            case 3:
            {
                panel_PrintClass(param1, TFClass_Spy);
                return;
            }
            case 4:
            {
                panel_PrintClass(param1, TFClass_Sniper);
                return;
            }
            case 5:    // Close
            {
                return;
            }
        }
    }
}

public void panel_PrintClass(int client, TFClassType class)
{
    Handle panel = CreatePanel();
    char   buffer[256];
    switch (class)
    {
        case TFClass_Soldier:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Soldier", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Soldier_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Pyro:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Pyro", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Pyro_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_DemoMan:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Demoman", client);
            SetPanelTitle(panel, buffer);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Demoman_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Engineer:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Engineer", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Engineer_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Medic:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Medic", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Medic_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Sniper:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Sniper", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Sniper_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Scout:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Scout", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Scout_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Heavy:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Heavy", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Heavy_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        case TFClass_Spy:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Help_Class_Spy", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Spy_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
        default:
        {
            Format(buffer, sizeof(buffer), "%T", "ZF_Class_Spectator", client);
            SetPanelTitle(panel, buffer, false);
            DrawPanelText(panel, "----------------------------------------");
            char fullText[1024];
            Format(fullText, sizeof(fullText), "%T", "ZF_Help_Text_Spectator_Full", client);
            char lines[10][128];
            int  numLines = ExplodeString(fullText, "\n", lines, 10, 128);
            for (int i = 0; i < numLines; i++)
            {
                DrawPanelText(panel, lines[i]);
            }
            DrawPanelText(panel, "----------------------------------------");
        }
    }
    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleClass, 8);
    CloseHandle(panel);
}

public void panel_HandleClass(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

//
// Main.PerkHelp
//
void panel_PrintPerkHelp(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Title_PerkHelp", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_PerkHelp1", client);
    DrawPanelText(panel, buffer);

    Format(buffer, sizeof(buffer), "%T", "ZF_Help_Text_PerkHelp2", client);
    DrawPanelText(panel, buffer);

    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back_Brackets", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close_Brackets", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandlePerkHelp, 30);
    CloseHandle(panel);
}

public void panel_HandlePerkHelp(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintHelp(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

stock void GetClassNameFromEnum(TFClassType classType, char[] buffer, int maxlen)
{
    switch (classType)
    {
        case TFClass_Unknown:
        {
            strcopy(buffer, maxlen, "Unknown");
        }
        case TFClass_Scout:
        {
            strcopy(buffer, maxlen, "Scout");
        }
        case TFClass_Sniper:
        {
            strcopy(buffer, maxlen, "Sniper");
        }
        case TFClass_Soldier:
        {
            strcopy(buffer, maxlen, "Soldier");
        }
        case TFClass_DemoMan:
        {
            strcopy(buffer, maxlen, "Demoman");
        }
        case TFClass_Medic:
        {
            strcopy(buffer, maxlen, "Medic");
        }
        case TFClass_Heavy:
        {
            strcopy(buffer, maxlen, "Heavy");
        }
        case TFClass_Pyro:
        {
            strcopy(buffer, maxlen, "Pyro");
        }
        case TFClass_Spy:
        {
            strcopy(buffer, maxlen, "Spy");
        }
        case TFClass_Engineer:
        {
            strcopy(buffer, maxlen, "Engineer");
        }
        default:
        {
            strcopy(buffer, maxlen, "Spectator");
        }
    }
}

public int Sort_Kills(int index1, int index2, Handle array, Handle hndl)
{
    DataPack pack1 = view_as<DataPack>(GetArrayCell(array, index1));
    DataPack pack2 = view_as<DataPack>(GetArrayCell(array, index2));

    pack1.Reset();
    pack1.ReadCell();    // skip client
    int kills1 = pack1.ReadCell();

    pack2.Reset();
    pack2.ReadCell();    // skip client
    int kills2 = pack2.ReadCell();

    return kills2 - kills1;
}


//
// Main.Credits
//
public void panel_PrintCredits(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Credits", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Recode", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original_Translation", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Original_Balancing", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Reremake", client);
    DrawPanelText(panel, buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Credits_Special_thanks", client);
    DrawPanelText(panel, buffer);
    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleCredits, 30);
    CloseHandle(panel);
}

//
// Main.Credits
//
public void panel_ChangeLanguage(int client)
{
    Handle panel = CreatePanel();
    char   buffer[256];

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Change_Language", client);
    SetPanelTitle(panel, buffer, false);

    DrawPanelText(panel, "----------------------------------------");
    Format(buffer, sizeof(buffer), "%T", "ZF_Change_Language_Guide", client);
    DrawPanelText(panel, buffer);
    DrawPanelText(panel, "----------------------------------------");

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Back", client);
    DrawPanelItem(panel, buffer, 0);

    Format(buffer, sizeof(buffer), "%T", "ZF_Menu_Close", client);
    DrawPanelItem(panel, buffer, 0);
    SendPanelToClient(panel, client, panel_HandleCredits, 30);
    CloseHandle(panel);
}

public void panel_HandleCredits(Handle menu, MenuAction action, int param1, int param2)
{
    if (action == MenuAction_Select)
    {
        switch (param2)
        {
            case 1:    // Back
            {
                panel_PrintMain(param1);
                return;
            }
            case 2:    // Close
            {
                return;
            }
        }
    }
}

////////////////////////////////////////////////////////////
//
// Debug Menu Functionality
//
////////////////////////////////////////////////////////////
public Action cmd_zfDebugMenu(int client, int args)
{
    if (!GetConVarBool(zf_cvDebug))
    {
        PrintToChat(client, "%T", "ZF_Debug_Not_Enabled", client);
        return Plugin_Handled;
    }

    if (!CheckCommandAccess(client, "zfdebug", ADMFLAG_ROOT))
    {
        PrintToChat(client, "%T", "ZF_Debug_No_Access", client);
        return Plugin_Handled;
    }

    panel_DebugPlayerList(client);
    return Plugin_Handled;
}

public void panel_DebugPlayerList(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugPlayerList);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Menu_Title", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i))
        {
            char info[32];
            IntToString(i, info, sizeof(info));
            char name[MAX_NAME_LENGTH];
            GetClientName(i, name, sizeof(name));
            AddMenuItem(menu, info, name);
        }
    }
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugPlayerList(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int target             = StringToInt(info);
        g_iDebugTarget[client] = target;
        panel_DebugActionMenu(client);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugActionMenu(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugActionMenu);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Action_Title", client, g_iDebugTarget[client]);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    char buffer[128];
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Modify_Stats", client);
    AddMenuItem(menu, "stats", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Team", client);
    AddMenuItem(menu, "team", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Class", client);
    AddMenuItem(menu, "class", buffer);
    Format(buffer, sizeof(buffer), "%T", "ZF_Debug_Change_Perk", client);
    AddMenuItem(menu, "perk", buffer);

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugActionMenu(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        if (StrEqual(info, "stats"))
        {
            panel_DebugModifyStats(client);
        }
        else if (StrEqual(info, "team"))
        {
            panel_DebugChangeTeam(client);
        }
        else if (StrEqual(info, "class"))
        {
            panel_DebugChangeClass(client);
        }
        else if (StrEqual(info, "perk"))
        {
            // TODO: Implement perk change menu
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugChangeTeam(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugChangeTeam);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Change_Team", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    AddMenuItem(menu, "2", "Survivor");
    AddMenuItem(menu, "3", "Zombie");
    AddMenuItem(menu, "1", "Spectator");

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugChangeTeam(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int team   = StringToInt(info);
        int target = g_iDebugTarget[client];
        if (team == surTeam())
            spawnClient(target, surTeam());
        else if (team == zomTeam())
            spawnClient(target, zomTeam());
        else
            ChangeClientTeamEx(target, team);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugChangeClass(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugChangeClass);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Change_Class", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, true);

    if (isSur(g_iDebugTarget[client]))
    {
        AddMenuItem(menu, "soldier", "Soldier");
        AddMenuItem(menu, "pyro", "Pyro");
        AddMenuItem(menu, "demoman", "Demoman");
        AddMenuItem(menu, "engineer", "Engineer");
        AddMenuItem(menu, "medic", "Medic");
    }
    else if (isZom(g_iDebugTarget[client]))
    {
        AddMenuItem(menu, "scout", "Scout");
        AddMenuItem(menu, "heavyweapons", "Heavy");
        AddMenuItem(menu, "spy", "Spy");
        AddMenuItem(menu, "sniper", "Sniper");
    }

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

// --- Start of Debug Stat Modification Menus ---

// This is an assumption as perk_structs.inc is not available.
// Based on HUD code, there are at least 4 stats.
#if !defined TOTAL_ZFSTATS
    #define TOTAL_ZFSTATS 4
enum ZFStat
{
    ZFStatAtt,
    ZFStatDef,
    ZFStatCrit,
    ZFStatSpeed
};
#endif

stock void GetStatNameFromEnum(ZFStat stat, char[] buffer, int maxlen)
{
    switch (stat)
    {
        case ZFStatAtt:
        {
            strcopy(buffer, maxlen, "Attack");
        }
        case ZFStatDef:
        {
            strcopy(buffer, maxlen, "Defense");
        }
        case ZFStatCrit:
        {
            strcopy(buffer, maxlen, "Critical Hit");
        }
        case ZFStatSpeed:
        {
            strcopy(buffer, maxlen, "Speed");
        }
        default:
        {
            strcopy(buffer, maxlen, "Unknown Stat");
        }
    }
}

public void panel_DebugModifyStats(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugModifyStats);
    char title[128];
    Format(title, sizeof(title), "%T", "ZF_Debug_Modify_Stats", client);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, false);

    char statName[64];
    char statIndexStr[4];
    for (int i = 0; i < TOTAL_ZFSTATS; i++)
    {
        GetStatNameFromEnum(view_as<ZFStat>(i), statName, sizeof(statName));
        IntToString(i, statIndexStr, sizeof(statIndexStr));
        AddMenuItem(menu, statIndexStr, statName);
    }

    AddMenuItem(menu, "back", "Back");
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugModifyStats(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        if (StrEqual(info, "back"))
        {
            panel_DebugActionMenu(client);
        }
        else
        {
            g_iDebugStat[client] = view_as<ZFStat>(StringToInt(info));
            panel_DebugModifyStatValue(client);
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

public void panel_DebugModifyStatValue(int client)
{
    Menu menu = CreateMenu(panel_HandleDebugModifyStatValue);

    char title[128];
    char statName[64];
    char targetName[MAX_NAME_LENGTH];
    GetClientName(g_iDebugTarget[client], targetName, sizeof(targetName));
    GetStatNameFromEnum(view_as<ZFStat>(g_iDebugStat[client]), statName, sizeof(statName));

    Format(title, sizeof(title), "Modify %s for %s", statName, targetName);
    SetMenuTitle(menu, title);
    SetMenuExitButton(menu, false);

    AddMenuItem(menu, "100", "+100");
    AddMenuItem(menu, "10", "+10");
    AddMenuItem(menu, "1", "+1");
    AddMenuItem(menu, "-1", "-1");
    AddMenuItem(menu, "-10", "-10");
    AddMenuItem(menu, "-100", "-100");
    AddMenuItem(menu, "back", "Back");

    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int panel_HandleDebugModifyStatValue(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));

        if (StrEqual(info, "back"))
        {
            panel_DebugModifyStats(client);
        }
        else
        {
            int    value  = StringToInt(info);
            int    target = g_iDebugTarget[client];
            ZFStat stat   = view_as<ZFStat>(g_iDebugStat[client]);

            addStat(target, stat, ZFStatTypePerm, value);

            // Re-display the menu to show it was successful and allow more changes
            panel_DebugModifyStatValue(client);
        }
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}

// --- End of Debug Stat Modification Menus ---
public int panel_HandleDebugChangeClass(Menu menu, MenuAction action, int client, int item)
{
    if (action == MenuAction_Select)
    {
        char info[32];
        GetMenuItem(menu, item, info, sizeof(info));
        int target = g_iDebugTarget[client];
        TF2_SetPlayerClass(target, TF2_GetClass(info));
        TF2_RespawnPlayer(target);
    }
    else if (action == MenuAction_End)
    {
        CloseHandle(menu);
    }
    return 0;
}
