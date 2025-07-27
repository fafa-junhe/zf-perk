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

#define ZF_FRIEND_ATTACK 25
#define ZF_FRIEND_REGEN 10
#define ZF_FRIEND_CRITTIME_INIT 0
#define ZF_FRIEND_CRITTIME_KILL 4
#define ZF_FRIEND_CRITTIME_ASSIST 2
#define ZF_FRIEND_RADIUSSQ (300 * 300)

methodmap FriendPerk < SurvivorBasePerk {
    public FriendPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        FriendPerk sm = view_as<FriendPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, FriendPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, FriendPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, FriendPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GRACE_END, FriendPerkFonGraceEnd);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, FriendPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, FriendPerkFonPlayerDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, FriendPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, FriendPerkFonPeriodic);

        return sm;
    }
}

stock SurvivorBasePerk FriendPerkFnew(int client) {
    return new FriendPerk(client);
}

FUNCTION(FriendPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Friend");
}

FUNCTION(FriendPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Bonus with a partner");
}

FUNCTION(FriendPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "During grace period, call for medic on a target to set them as a partner. (System will assign one if not chosen). Gain attack and regen when near your partner. Kills/assists with your partner grant crit time. When your partner dies, you gain crits for the stored duration.");
}

void FriendPerk_doFriendSelect(FriendPerk perk, int friend = -1) {
    int client = perk.client;
    if (friend == -1) {
        int targets[MAXPLAYERS];
        int count = 0;
        for (int i = 1; i <= MaxClients; i++) {
            if (validLivingSur(i) && i != client) {
                targets[count++] = i;
            }
        }
        if (count > 0) {
            friend = targets[GetRandomInt(0, count - 1)];
        }
    }

    if (friend != -1) {
        zf_perkState[client] = friend;
        zf_perkTimer[client] = ZF_FRIEND_CRITTIME_INIT;
        createIcon(client, friend, ZFSPR_HEART);
        PrintToChat(client, "[ZF] You are now friends with %N.", friend);
        PrintToChat(friend, "[ZF] You are now friends with %N.", client);
    }
}

FUNCTION(FriendPerk, onGraceEnd)) {
    int client = _inst.client;
    if (zf_perkState[client] == 0) {
        FriendPerk_doFriendSelect(_inst);
    }
}

FUNCTION(FriendPerk, onCallForMedic)) {
    int client = _inst.client;
    if (roundState() > RoundGrace) return;

    int target = GetClientAimTarget(client, false);
    if (validLivingSur(target) && target != client) {
        FriendPerk_doFriendSelect(_inst, target);
    }
}

FUNCTION(FriendPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    int client = _inst.client;
    int friend = zf_perkState[client];
    if (friend <= 0) return; // No friend or friend is dead

    if (isZom(victim)) {
        bool clientInvolved = (killer == client || assist == client);
        bool friendInvolved = (killer == friend || assist == friend);

        if (clientInvolved && friendInvolved) {
            zf_perkTimer[client] += ZF_FRIEND_CRITTIME_KILL;
        } else if (clientInvolved) {
            zf_perkTimer[client] += ZF_FRIEND_CRITTIME_ASSIST;
        }
    }
}

FUNCTION(FriendPerk, onPeriodic)) {
    int client = _inst.client;
    // If friend is marked as dead (-1) and we have crit time, decay it.
    if (zf_perkState[client] == -1 && zf_perkTimer[client] > 0) {
        zf_perkTimer[client]--;
    }
}

FUNCTION(FriendPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;
    int friend = zf_perkState[client];

    if (friend > 0) { // Friend is selected and supposed to be alive
        if (validLivingSur(friend)) {
            float clientPos[3];
            GetClientAbsOrigin(client, clientPos);
            float friendPos[3];
            GetClientAbsOrigin(friend, friendPos);

            if (GetVectorDistance(clientPos, friendPos, true) <= ZF_FRIEND_RADIUSSQ) {
                addHealth(client, ZF_FRIEND_REGEN);
                addHealth(friend, ZF_FRIEND_REGEN);
                addStat(client, ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);
                addStat(friend, ZFStatAtt, ZFStatTypeCond, ZF_FRIEND_ATTACK);
            }
        } else { // Friend has died
            zf_perkState[client] = -1; // Mark friend as dead
            if (zf_perkTimer[client] > 0) {
                addCondKritz(client, float(zf_perkTimer[client]));
                fxKritzStart(client);
                fxCreateParticle(ZFPART_AURAINRED, client, AttachBase, float(zf_perkTimer[client]));
                removeIcon(client);
            }
        }
    }

    Format(buffer, maxlen, "Crit Time: %ds", zf_perkTimer[client]);
}
