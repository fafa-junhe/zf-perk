#if defined __FriendPerk_included
    #endinput
#endif
#define __FriendPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_FRIEND_ATTACK           25
#define ZF_FRIEND_REGEN            10
#define ZF_FRIEND_CRITTIME_INIT    5
#define ZF_FRIEND_CRITTIME_KILL    4
#define ZF_FRIEND_CRITTIME_ASSIST  2
#define ZF_FRIEND_RADIUSSQ         (300 * 300)

#define FRIEND_PERK_DATA_FRIEND    DP(27)
#define FRIEND_PERK_DATA_CRIT_TIME DP(28)

methodmap FriendPerk < SurvivorBasePerk
{
    property int friendId
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(FRIEND_PERK_DATA_FRIEND);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(FRIEND_PERK_DATA_FRIEND);
            this.WriteCell(value);
        }
    }
    property int critTime
    {

public         get()
        {
            this.Position = view_as<DataPackPos>(FRIEND_PERK_DATA_CRIT_TIME);
            return this.ReadCell();
        }

public         set(int value)
        {
            this.Position = view_as<DataPackPos>(FRIEND_PERK_DATA_CRIT_TIME);
            this.WriteCell(value);
        }
    }

public     FriendPerk(int client)
    {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        FriendPerk       sm      = view_as<FriendPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, FriendPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, FriendPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, FriendPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GRACE_END, FriendPerkFonGraceEnd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, FriendPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, FriendPerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, FriendPerkFonAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, FriendPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, FriendPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, FriendPerkFOnGameFrame);

        sm_base.Position = FRIEND_PERK_DATA_FRIEND;
        sm_base.WriteCell(0);    // friendId
        sm_base.Position = FRIEND_PERK_DATA_CRIT_TIME;
        sm_base.WriteCell(0);    // critTime
        return sm;
    }
}

stock SurvivorBasePerk FriendPerkFnew(int client)
{
    return new FriendPerk(client);
}

FUNCTION(FriendPerk, getName), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "Friend");
}

FUNCTION(FriendPerk, getShortdesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "FriendPerk_shortdesc");
}

FUNCTION(FriendPerk, getDesc), char[] buffer, int maxlen)
{
    strcopy(buffer, maxlen, "FriendPerk_desc");
}

void FriendPerk_doFriendSelect(FriendPerk perk, int friend = -1)
{
    int client = perk.client;
    if (friend == -1)
    {
        int targets[MAXPLAYERS];
        int count = 0;
        for (int i = 1; i <= MaxClients; i++)
        {
            if (validLivingSur(i) && i != client)
            {
                targets[count++] = i;
            }
        }
        if (count > 0)
        {
            friend = targets[GetRandomInt(0, count - 1)];
        }
    }

    if (friend != -1)
    {
        perk.friendId = friend;
        perk.critTime = ZF_FRIEND_CRITTIME_INIT;
        PrintToChat(client, "%t", "FriendPerk_Chat_NewFriend", friend);
        PrintToChat(friend, "%t", "FriendPerk_Chat_NewFriend", client);
    }
}

FUNCTION(FriendPerk, onGraceEnd))
{
    if (_inst.friendId == 0)
    {
        FriendPerk_doFriendSelect(_inst);
    }
    _inst.critTime = ZF_FRIEND_CRITTIME_INIT;
}

FUNCTION(FriendPerk, onCallForMedic))
{
    int client = _inst.client;
    if (roundState() > RoundGrace) return;

    int target = GetClientAimTarget(client, false);
    if (validLivingSur(target) && target != client)
    {
        FriendPerk_doFriendSelect(_inst, target);
    }
}

FUNCTION(FriendPerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int friend = _inst.friendId;
    if (friend <= 0 || !isZom(victim)) return;

    bool friendInvolved = (killer == friend || assist == friend);
    if (friendInvolved)
    {
        _inst.critTime += ZF_FRIEND_CRITTIME_KILL;
    }
    else {
        _inst.critTime += ZF_FRIEND_CRITTIME_ASSIST;
    }
}

FUNCTION(FriendPerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype)
{
    int friend = _inst.friendId;
    if (friend <= 0 || !isZom(victim)) return;

    bool friendInvolved = (killer == friend || assist == friend);
    if (friendInvolved)
    {
        _inst.critTime += ZF_FRIEND_CRITTIME_KILL;
    }
    // No points for just assisting a non-friend kill
}

FUNCTION(FriendPerk, onPeriodic))
{
    // If friend is marked as dead (-1) and we have crit time, decay it.
    if (_inst.friendId == -1 && _inst.critTime > 0)
    {
        _inst.critTime--;
    }
}

FUNCTION(FriendPerk, OnGameFrame))
{
    int client = _inst.client;
    int friend = _inst.friendId;

    if (friend > 0 && validLivingSur(friend))
    {
        float clientPos[3];
        GetClientAbsOrigin(client, clientPos);
        float friendPos[3];
        GetClientAbsOrigin(friend, friendPos);
        if (GetVectorDistance(clientPos, friendPos, true) <= ZF_FRIEND_RADIUSSQ)
        {
            fxSprite(client, friend, { 10, 50, 200, 200 }, ZFSPR_DOMINATED_INDEX);
            fxSprite(friend, client, { 10, 50, 200, 200 }, ZFSPR_DOMINATED_INDEX);
        }
    }
}

FUNCTION(FriendPerk, updateCondStats), char[] buffer, int maxlen)
{
    int client = _inst.client;
    int friend = _inst.friendId;

    if (friend > 0)
    {    // Friend is selected and supposed to be alive
        if (validLivingSur(friend))
        {
            float clientPos[3];
            GetClientAbsOrigin(client, clientPos);
            float friendPos[3];
            GetClientAbsOrigin(friend, friendPos);

            if (GetVectorDistance(clientPos, friendPos, true) <= ZF_FRIEND_RADIUSSQ)
            {
                addHealth(client, ZF_FRIEND_REGEN);
                addHealth(friend, ZF_FRIEND_REGEN);
                addStat(client, ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);
                addStat(friend, ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);
            }
        }
        else {                      // Friend has died
            _inst.friendId = -1;    // Mark friend as dead
            if (_inst.critTime > 0)
            {
                addCondKritz(client, float(_inst.critTime));
                fxKritzStart(client);
                fxCreateParticle(ZFPART_AURAINRED, client, AttachBase, float(_inst.critTime));
            }
        }
    }

    Format(buffer, maxlen, "%t", "FriendPerk_Status_CritTime", _inst.critTime);
}