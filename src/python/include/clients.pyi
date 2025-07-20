from typing import Any, list, Callable, Union
from .admin import *
from .core import *


class AuthIdType:
    """Auth string types.

Note that for the Steam2 and Steam3 types, the following ids are
also valid values:
"STEAM_ID_PENDING" - Authentication is pending.
"STEAM_ID_LAN" - Authentication is disabled because of being on a LAN server.
"BOT" - The client is a bot."""
    AuthId_Engine: int = ...
    AuthId_Steam2: int = ...
    AuthId_Steam3: int = ...
    AuthId_SteamID64: int = ...


class NetFlow:
    """Network flow directions."""
    NetFlow_Both: int = ...
    NetFlow_Incoming: int = ...
    NetFlow_Outgoing: int = ...


def OnClientConnect(client: int, rejectmsg: str, maxlen: int) -> bool:
    """Called on client connection.  If you return true, the client will be allowed in the server.
If you return false (or return nothing), the client will be rejected.  If the client is 
rejected by this forward or any other, OnClientDisconnect will not be called.

Note: Do not write to rejectmsg if you plan on returning true.  If multiple plugins write
to the string buffer, it is not defined which plugin's string will be shown to the client,
but it is guaranteed one of them will.

@param client        Client index.
@param rejectmsg     Buffer to store the rejection message when the connection is refused.
@param maxlen        Maximum number of characters for rejection buffer.
@return              True to validate client's connection, false to refuse it."""
    pass
def OnClientConnected(client: int) -> None:
    """Called once a client successfully connects.  This callback is paired with OnClientDisconnect.

@param client        Client index."""
    pass
def OnClientPutInServer(client: int) -> None:
    """Called when a client is entering the game.

Whether a client has a steamid is undefined until OnClientAuthorized
is called, which may occur either before or after OnClientPutInServer.
Similarly, use OnClientPostAdminCheck() if you need to verify whether 
connecting players are admins.

GetClientCount() will include clients as they are passed through this 
function, as clients are already in game at this point.

@param client        Client index."""
    pass
def OnClientDisconnect(client: int) -> None:
    """Called when a client is disconnecting from the server.

@param client        Client index."""
    pass
def OnClientDisconnect_Post(client: int) -> None:
    """Called when a client is disconnected from the server.

@param client        Client index."""
    pass
def OnClientCommand(client: int, args: int) -> Any:
    """Called when a client is sending a command.

As of SourceMod 1.3, the client is guaranteed to be in-game.
Use command listeners (console.inc) for more advanced hooks.

@param client        Client index.
@param args          Number of arguments.
@return              Plugin_Handled blocks the command from being sent,
                     and Plugin_Continue resumes normal functionality."""
    pass
def OnClientCommandKeyValues(client: int, kv: Any) -> Any:
    """Called when a client is sending a KeyValues command.

@param client        Client index.
@param kv            Editable KeyValues data to be sent as the command.
                     (This handle should not be stored and will be closed
                     after this forward completes.)
@return              Plugin_Handled blocks the command from being sent,
                     and Plugin_Continue resumes normal functionality."""
    pass
def OnClientCommandKeyValues_Post(client: int, kv: Any) -> None:
    """Called after a client has sent a KeyValues command.

@param client        Client index.
@param kv            KeyValues data sent as the command.
                     (This handle should not be stored and will be closed
                     after this forward completes.)"""
    pass
def OnClientSettingsChanged(client: int) -> None:
    """Called whenever the client's settings are changed.

@param client        Client index."""
    pass
def OnClientAuthorized(client: int, auth: str) -> None:
    """Called when a client receives an auth ID.  The state of a client's 
authorization as an admin is not guaranteed here.  Use 
OnClientPostAdminCheck() if you need a client's admin status.

This is called by bots, but the ID will be "BOT".

@param client        Client index.
@param auth          Client Steam2 id, if available, else engine auth id."""
    pass
def OnClientPreAdminCheck(client: int) -> Any:
    """Called once a client is authorized and fully in-game, but 
before admin checks are done.  This can be used to override 
the default admin checks for a client.  You should only use 
this for overriding; use OnClientPostAdminCheck() instead 
if you want notification.

Note: If handled/blocked, PostAdminCheck must be signalled 
manually via NotifyPostAdminCheck().

This callback is guaranteed to occur on all clients, and always 
after each OnClientPutInServer() call.

@param client        Client index.
@return              Plugin_Handled to block admin checks."""
    pass
def OnClientPostAdminFilter(client: int) -> None:
    """Called directly before OnClientPostAdminCheck() as a method to 
alter administrative permissions before plugins perform final 
post-connect operations.  

In general, do not use this function unless you are specifically 
attempting to change access permissions.  Use OnClientPostAdminCheck() 
instead if you simply want to perform post-connect authorization 
routines.

See OnClientPostAdminCheck() for more information.

@param client        Client index."""
    pass
def OnServerEnterHibernation() -> None:
    """Called directly before the server enters hibernation.
This is your last chance to do anything in the plugin before
hibernation occurs, as SV_Frame will no longer be called."""
    pass
def OnServerExitHibernation() -> None:
    """Called directly before the server leaves hibernation."""
    pass
def OnClientPostAdminCheck(client: int) -> None:
    """Called once a client is authorized and fully in-game, and 
after all post-connection authorizations have been performed.  

This callback is guaranteed to occur on all clients, and always 
after each OnClientPutInServer() call.

@param client        Client index."""
    pass
def OnClientLanguageChanged(client: int, language: int) -> None:
    """Called when the language was received from the player.

@param client        Client index.
@param language      Language number."""
    pass
def GetMaxClients() -> int:
    pass
def GetMaxHumanPlayers() -> int:
    """Returns the maximum number of human players allowed on the server.  This is 
a game-specific function used on newer games to limit the number of humans
that can join a game and can be lower than MaxClients. It is the number often
reflected in the server browser or when viewing the output of the status command.
On unsupported games or modes without overrides, it will return the same value
as MaxClients.

You should not globally cache the value to GetMaxHumanPlayers() because it can change across
game modes. You may still cache it locally.

@return              Maximum number of humans allowed."""
    pass
def GetClientCount(inGameOnly: bool = ...) -> int:
    """Returns the client count put in the server.

@param inGameOnly    If false connecting players are also counted.
@return              Client count in the server."""
    pass
def GetClientName(client: int, name: str, maxlen: int) -> bool:
    """Returns the client's name.

@param client        Player index.
@param name          Buffer to store the client's name.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@return              True on success, false otherwise.
@error               If the client is not connected an error will be thrown."""
    pass
def GetClientIP(client: int, ip: str, maxlen: int, remport: bool = ...) -> bool:
    """Retrieves a client's IP address.

@param client        Player index.
@param ip            Buffer to store the client's ip address.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@param remport       Remove client's port from the ip string (true by default).
@return              True on success, false otherwise.
@error               If the client is not connected or the index is invalid."""
    pass
def GetClientAuthString(client: int, auth: str, maxlen: int, validate: bool = ...) -> bool:
    pass
def GetClientAuthId(client: int, authType: AuthIdType, auth: str, maxlen: int, validate: bool = ...) -> bool:
    """Retrieves a client's authentication string (SteamID).

@param client        Player index.
@param authType      Auth id type and format to use.
@param auth          Buffer to store the client's auth id.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@param validate      Check backend validation status.
                     DO NOT PASS FALSE UNLESS YOU UNDERSTAND THE CONSEQUENCES,
                     You WILL KNOW if you need to use this, MOST WILL NOT.
@return              True on success, false otherwise.
@error               If the client is not connected or the index is invalid."""
    pass
def GetSteamAccountID(client: int, validate: bool = ...) -> int:
    """Returns the client's Steam account ID, a number uniquely identifying a given Steam account.
This number is the basis for the various display SteamID forms, see the AuthIdType enum for examples.

@param client        Client Index.
@param validate      Check backend validation status.
                     DO NOT PASS FALSE UNLESS YOU UNDERSTAND THE CONSEQUENCES,
                     You WILL KNOW if you need to use this, MOST WILL NOT.
@return              Steam account ID or 0 if not available.
@error               If the client is not connected or the index is invalid."""
    pass
def GetClientUserId(client: int) -> int:
    """Retrieves a client's user id, which is an index incremented for every client
that joins the server.

@param client        Player index.
@return              User id of the client.
@error               If the client is not connected or the index is invalid."""
    pass
def IsClientConnected(client: int) -> bool:
    """Returns if a certain player is connected.

@param client        Player index.
@return              True if player is connected to the server, false otherwise.
@error               Invalid client index."""
    pass
def IsClientInGame(client: int) -> bool:
    """Returns if a certain player has entered the game.

@param client        Player index (index does not have to be connected).
@return              True if player has entered the game, false otherwise.
@error               Invalid client index."""
    pass
def IsClientInKickQueue(client: int) -> bool:
    """Returns if a client is in the "kick queue" (i.e. the client will be kicked 
shortly and thus they should not appear as valid).

@param client        Player index (must be connected).
@return              True if in the kick queue, false otherwise.
@error               Invalid client index."""
    pass
def IsPlayerInGame(client: int) -> bool:
    pass
def IsClientAuthorized(client: int) -> bool:
    """Returns if a certain player has been authenticated.

@param client        Player index.
@return              True if player has been authenticated, false otherwise.
@error               Invalid client index."""
    pass
def IsFakeClient(client: int) -> bool:
    """Returns if a certain player is a fake client.

@param client        Player index.
@return              True if player is a fake client, false otherwise.
@error               Invalid client index, or client not connected."""
    pass
def IsClientSourceTV(client: int) -> bool:
    """Returns if a certain player is the SourceTV bot.

@param client        Player index.
@return              True if player is the SourceTV bot, false otherwise.
@error               Invalid client index, or client not connected."""
    pass
def IsClientReplay(client: int) -> bool:
    """Returns if a certain player is the Replay bot.

@param client        Player index.
@return              True if player is the Replay bot, false otherwise.
@error               Invalid client index, or client not connected."""
    pass
def IsClientObserver(client: int) -> bool:
    """Returns if a certain player is an observer/spectator.

@param client        Player index.
@return              True if player is an observer, false otherwise.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def IsPlayerAlive(client: int) -> bool:
    """Returns if the client is alive or dead.

Note: This function was originally in SDKTools and was moved to core.

@param client        Player's index.
@return              True if the client is alive, false otherwise.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientInfo(client: int, key: str, value: str, maxlen: int) -> bool:
    """Retrieves values from client replicated keys.

@param client        Player's index.
@param key           Key string.
@param value         Buffer to store value.
@param maxlen        Maximum length of valve (UTF-8 safe).
@return              True on success, false otherwise.
@error               Invalid client index, or client not connected."""
    pass
def GetClientTeam(client: int) -> int:
    """Retrieves a client's team index.

@param client        Player's index.
@return              Team index the client is on (mod specific).
@error               Invalid client index, client not in game, or no mod support."""
    pass
def SetUserAdmin(client: int, id: AdminId, temp: bool = ...) -> None:
    """Sets a client's AdminId.  

@param client        Player's index.
@param id            AdminId to set.  INVALID_ADMIN_ID removes admin permissions.
@param temp          True if the id should be freed on disconnect.
@error               Invalid client index, client not connected, or bogus AdminId."""
    pass
def GetUserAdmin(client: int) -> AdminId:
    """Retrieves a client's AdminId.

@param client        Player's index.
@return              AdminId of the client, or INVALID_ADMIN_ID if none.
@error               Invalid client index, or client not connected."""
    pass
def AddUserFlags(client: int, _0: AdminFlag, *args: Any) -> None:
    """Sets access flags on a client.  If the client is not an admin,
a temporary, anonymous AdminId is given.

@param client        Player's index.
@param ...           Flags to set on the client.
@error               Invalid client index, or client not connected."""
    pass
def RemoveUserFlags(client: int, _0: AdminFlag, *args: Any) -> None:
    """Removes flags from a client.  If the client is not an admin,
this has no effect.

@param client        Player's index.
@param ...           Flags to remove from the client.
@error               Invalid client index, or client not connected."""
    pass
def SetUserFlagBits(client: int, flags: int) -> None:
    """Sets access flags on a client using bits instead of flags.  If the
client is not an admin, and flags not 0, a temporary, anonymous AdminId is given.

@param client        Player's index.
@param flags         Bitstring of flags to set on client.
@error               Invalid client index, or client not connected."""
    pass
def GetUserFlagBits(client: int) -> int:
    """Returns client access flags.  If the client is not an admin,
the result is always 0.

@param client        Player's index.
@return              Flags
@error               Invalid client index, or client not connected."""
    pass
def CanUserTarget(client: int, target: int) -> bool:
    """Returns whether a user can target another user.
This is a helper function for CanAdminTarget.

@param client        Player's index.
@param target        Target player's index.
@return              True if target is targettable by the player, false otherwise.
@error               Invalid or unconnected player indexers."""
    pass
def RunAdminCacheChecks(client: int) -> bool:
    """Runs through the Core-defined admin authorization checks on a player.
Has no effect if the player is already an admin.

Note: This function is based on the internal cache only.

@param client        Client index.
@return              True if access was changed, false if it did not.
@error               Invalid client index or client not in-game AND authorized."""
    pass
def NotifyPostAdminCheck(client: int) -> None:
    """Signals that a player has completed post-connection admin checks.
Has no effect if the player has already had this event signalled.

Note: This must be sent even if no admin id was assigned.

@param client        Client index.
@error               Invalid client index or client not in-game AND authorized."""
    pass
def CreateFakeClient(name: str) -> int:
    """Creates a fake client.

@param name          Name to use.
@return              Client index on success, 0 otherwise.
@error               No map is active."""
    pass
def SetFakeClientConVar(client: int, cvar: str, value: str) -> None:
    """Sets a convar value on a fake client.

@param client        Client index.
@param cvar          ConVar name.
@param value         ConVar value.
@error               Invalid client index, client not connected,
                     or client not a fake client."""
    pass
def GetClientHealth(client: int) -> int:
    """Returns the client's health.

@param client        Player's index.
@return              Health value.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientModel(client: int, model: str, maxlen: int) -> None:
    """Returns the client's model name.

@param client        Player's index.
@param model         Buffer to store the client's model name.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientWeapon(client: int, weapon: str, maxlen: int) -> None:
    """Returns the client's weapon name.

@param client        Player's index.
@param weapon        Buffer to store the client's weapon name.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientMaxs(client: int, vec: list[float]) -> None:
    """Returns the client's max size vector.

@param client        Player's index.
@param vec           Destination vector to store the client's max size.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientMins(client: int, vec: list[float]) -> None:
    """Returns the client's min size vector.

@param client        Player's index.
@param vec           Destination vector to store the client's min size.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientAbsAngles(client: int, ang: list[float]) -> None:
    """Returns the client's position angle.

@param client        Player's index.
@param ang           Destination vector to store the client's position angle.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientAbsOrigin(client: int, vec: list[float]) -> None:
    """Returns the client's origin vector.

@param client        Player's index.
@param vec           Destination vector to store the client's origin vector.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientArmor(client: int) -> int:
    """Returns the client's armor.

@param client        Player's index.
@return              Armor value.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientDeaths(client: int) -> int:
    """Returns the client's death count.

@param client        Player's index.
@return              Death count.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientFrags(client: int) -> int:
    """Returns the client's frag count.

@param client        Player's index.
@return              Frag count.
@error               Invalid client index, client not in game, or no mod support."""
    pass
def GetClientDataRate(client: int) -> int:
    """Returns the client's send data rate in bytes/sec.

@param client        Player's index.
@return              Data rate.
@error               Invalid client index, client not connected, or fake client."""
    pass
def IsClientTimingOut(client: int) -> bool:
    """Returns if a client is timing out

@param client        Player's index.
@return              True if client is timing out, false otherwise.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientTime(client: int) -> float:
    """Returns the client's connection time in seconds.

@param client        Player's index.
@return              Connection time.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientLatency(client: int, flow: NetFlow) -> float:
    """Returns the client's current latency (RTT), more accurate than GetAvgLatency but jittering.

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Latency, or -1 if network info is not available.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientAvgLatency(client: int, flow: NetFlow) -> float:
    """Returns the client's average packet latency in seconds.

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Latency, or -1 if network info is not available.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientAvgLoss(client: int, flow: NetFlow) -> float:
    """Returns the client's average packet loss, values go from 0 to 1 (for percentages).

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Average packet loss, or -1 if network info is not available.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientAvgChoke(client: int, flow: NetFlow) -> float:
    """Returns the client's average packet choke, values go from 0 to 1 (for percentages).

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Average packet loss, or -1 if network info is not available.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientAvgData(client: int, flow: NetFlow) -> float:
    """Returns the client's data flow in bytes/sec.

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Data flow.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientAvgPackets(client: int, flow: NetFlow) -> float:
    """Returns the client's average packet frequency in packets/sec.

@param client        Player's index.
@param flow          Traffic flowing direction.
@return              Packet frequency.
@error               Invalid client index, client not connected, or fake client."""
    pass
def GetClientOfUserId(userid: int) -> int:
    """Translates an userid index to the real player index.

@param userid        Userid value.
@return              Client value.
@error               Returns 0 if invalid userid."""
    pass
def KickClient(client: int, format: str = ..., _0: Any = ..., *args: Any) -> None:
    """Disconnects a client from the server as soon as the next frame starts.

Note: Originally, KickClient() was immediate.  The delay was introduced 
because despite warnings, plugins were using it in ways that would crash. 
The new safe version can break cases that rely on immediate disconnects, 
but ensures that plugins do not accidentally cause crashes.

If you need immediate disconnects, use KickClientEx().

Note: IsClientInKickQueue() will return true before the kick occurs.

@param client        Client index.
@param format        Optional formatting rules for disconnect reason.
                     Note that a period is automatically appended to the string by the engine.
@param ...           Variable number of format parameters.
@error               Invalid client index, or client not connected."""
    pass
def KickClientEx(client: int, format: str = ..., _0: Any = ..., *args: Any) -> None:
    """Immediately disconnects a client from the server.

Kicking clients from certain events or callbacks may cause crashes.  If in 
doubt, create a short (0.1 second) timer to kick the client in the next 
available frame.

@param client        Client index.
@param format        Optional formatting rules for disconnect reason.
                     Note that a period is automatically appended to the string by the engine.
@param ...           Variable number of format parameters.
@error               Invalid client index, or client not connected."""
    pass
def ChangeClientTeam(client: int, team: int) -> None:
    """Changes a client's team through the mod's generic team changing function.
On CS:S, this will kill the player.

@param client        Client index.
@param team          Mod-specific team index.
@error               Invalid client index, client not in game, or lack of 
                     mod support."""
    pass
def GetClientSerial(client: int) -> int:
    """Returns the clients unique serial identifier.

@param client        Client index.
@return              Serial number.
@error               Invalid client index, or client not connected."""
    pass
def GetClientFromSerial(serial: int) -> int:
    """Returns the client index by its serial number.

@param serial        Serial number.
@return              Client index, or 0 for invalid serial."""
    pass
MAXPLAYERS: Any = ...  # 101  /**< Maximum number of players SourceMod supports */
MAX_NAME_LENGTH: Any = ...  # 128 /**< Maximum buffer required to store a client name */
MAX_AUTHID_LENGTH: Any = ...  # 64 /**< Maximum buffer required to store any AuthID type */