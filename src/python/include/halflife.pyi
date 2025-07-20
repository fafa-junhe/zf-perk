from typing import Any, list, Callable, Union
from .clients import *
from .handles import *


class ClientRangeType:
    RangeType_Audibility: int = ...
    RangeType_Visibility: int = ...


class DialogType:
    """< Treat msg as a filename to be opened """
    DialogType_AskConnect: int = ...
    DialogType_Entry: int = ...
    DialogType_Menu: int = ...
    DialogType_Msg: int = ...
    DialogType_Text: int = ...


class EngineVersion:
    Engine_AlienSwarm: int = ...
    Engine_BlackMesa: int = ...
    Engine_Blade: int = ...
    Engine_BloodyGoodTime: int = ...
    Engine_CSGO: int = ...
    Engine_CSS: int = ...
    Engine_Contagion: int = ...
    Engine_DODS: int = ...
    Engine_DOI: int = ...
    Engine_DOTA: int = ...
    Engine_DarkMessiah: int = ...
    Engine_EYE: int = ...
    Engine_HL2DM: int = ...
    Engine_Insurgency: int = ...
    Engine_Left4Dead2: int = ...
    Engine_Left4Dead: int = ...
    Engine_MCV: int = ...
    Engine_NuclearDawn: int = ...
    Engine_Original: int = ...
    Engine_PVKII: int = ...
    Engine_Portal2: int = ...
    Engine_SDK2013: int = ...
    Engine_SourceSDK2006: int = ...
    Engine_SourceSDK2007: int = ...
    Engine_TF2: int = ...
    Engine_Unknown: int = ...


class FindMapResult:
    FindMap_Found: int = ...
    FindMap_FuzzyMatch: int = ...
    FindMap_NonCanonical: int = ...
    FindMap_NotFound: int = ...
    FindMap_PossiblyAvailable: int = ...


def LogToGame(format: str, _0: Any, *args: Any) -> None:
    """Logs a generic message to the HL2 logs.

@param format        String format.
@param ...           Format arguments."""
    pass
def SetRandomSeed(seed: int) -> None:
    """Sets the seed value for the global Half-Life 2 Random Stream.

@param seed         Seed value."""
    pass
def GetRandomFloat(fMin: float = ..., fMax: float = ...) -> float:
    """Returns a random floating point number from the Half-Life 2 Random Stream.

@param fMin          Minimum random bound.
@param fMax          Maximum random bound.
@return              A random number between (inclusive) fMin and fMax."""
    pass
def GetRandomInt(nmin: int, nmax: int) -> int:
    """Returns a random number from the Half-Life 2 Random Stream.

@param nmin          Minimum random bound.
@param nmax          Maximum random bound.
@return              A random number between (inclusive) nmin and nmax."""
    pass
def IsMapValid(map: str) -> bool:
    """Returns whether a map is valid or not.

@param map           Map name, excluding .bsp extension.
@return              True if valid, false otherwise."""
    pass
def FindMap(map: str, foundmap: str, maxlen: int) -> FindMapResult:
    """Returns whether a full or partial map name is found or can be resolved

@param map           Map name (usually same as map path relative to maps/ dir,
                     excluding .bsp extension).
@param foundmap      Resolved map name. If the return is FindMap_FuzzyMatch
                     or FindMap_NonCanonical the buffer will be the full path.
@param maxlen        Maximum length to write to map var.
@return              Result of the find operation. Not all result types are supported on all games."""
    pass
def GetMapDisplayName(map: str, displayName: str, maxlen: int) -> bool:
    """Get the display name of a workshop map.

Note: You do not need to call FindMap first.  This native will call FindMap internally.

@param map           Map name (usually same as map path relative to maps/ dir,
                     excluding .bsp extension).
@param displayName   Map's display name, i.e. cp_mymapname or de_mymapname.
                     If FindMap returns FindMap_PossiblyAvailable or FindMap_NotFound,
                     the map cannot be resolved and this native will return false,
                     but displayName will be a copy of map.
@param maxlen        Maximum length to write to displayName var.
@return              true if FindMap returns FindMap_Found, FindMap_FuzzyMatch, or
                     FindMap_NonCanonical.
                     false if FindMap returns FindMap_PossiblyAvailable or FindMap_NotFound."""
    pass
def IsDedicatedServer() -> bool:
    """Returns whether the server is dedicated.

@return              True if dedicated, false otherwise."""
    pass
def GetEngineTime() -> float:
    """Returns a high-precision time value for profiling the engine.

@return              A floating point time value."""
    pass
def GetGameTime() -> float:
    """Returns the game time based on the game tick.

@return              Game tick time."""
    pass
def GetGameTickCount() -> int:
    """Returns the game's internal tick count.

@return              Game tick count."""
    pass
def GetGameFrameTime() -> float:
    """Returns the time the Game took processing the last frame.

@return              Game frame time."""
    pass
def GetGameDescription(buffer: str, maxlength: int, original: bool = ...) -> int:
    """Returns the game description from the mod.

@param buffer        Buffer to store the description.
@param maxlength     Maximum size of the buffer.
@param original      If true, retrieves the original game description,
                     ignoring any potential hooks from plugins.
@return              Number of bytes written to the buffer (UTF-8 safe)."""
    pass
def GetGameFolderName(buffer: str, maxlength: int) -> int:
    """Returns the name of the game's directory.

@param buffer        Buffer to store the directory name.
@param maxlength     Maximum size of the buffer.
@return              Number of bytes written to the buffer (UTF-8 safe)."""
    pass
def GetCurrentMap(buffer: str, maxlength: int) -> int:
    """Returns the current map name.

@param buffer        Buffer to store map name.
@param maxlength     Maximum length of buffer.
@return              Number of bytes written (UTF-8 safe)."""
    pass
def PrecacheModel(model: str, preload: bool = ...) -> int:
    """Precaches a given model.

@param model         Name of the model to precache.
@param preload       If preload is true the file will be precached before level startup.
@return              Returns the model index, 0 for error."""
    pass
def PrecacheSentenceFile(file: str, preload: bool = ...) -> int:
    """Precaches a given sentence file.

@param file          Name of the sentence file to precache.
@param preload       If preload is true the file will be precached before level startup.
@return              Returns a sentence file index."""
    pass
def PrecacheDecal(decal: str, preload: bool = ...) -> int:
    """Precaches a given decal.

@param decal         Name of the decal to precache.
@param preload       If preload is true the file will be precached before level startup.
@return              Returns a decal index."""
    pass
def PrecacheGeneric(generic: str, preload: bool = ...) -> int:
    """Precaches a given generic file.

@param generic       Name of the generic file to precache.
@param preload       If preload is true the file will be precached before level startup.
@return              Returns a generic file index."""
    pass
def IsModelPrecached(model: str) -> bool:
    """Returns if a given model is precached.

@param model         Name of the model to check.
@return              True if precached, false otherwise."""
    pass
def IsDecalPrecached(decal: str) -> bool:
    """Returns if a given decal is precached.

@param decal         Name of the decal to check.
@return              True if precached, false otherwise."""
    pass
def IsGenericPrecached(generic: str) -> bool:
    """Returns if a given generic file is precached.

@param generic       Name of the generic file to check.
@return              True if precached, false otherwise."""
    pass
def PrecacheSound(sound: str, preload: bool = ...) -> bool:
    """Precaches a given sound.

@param sound         Name of the sound to precache.
@param preload       If preload is true the file will be precached before level startup.
@return              True if successfully precached, false otherwise."""
    pass
def IsSoundPrecached(sound: str) -> bool:
    pass
def CreateDialog(client: int, kv: Any, type: DialogType) -> None:
    """Creates different types of ingame messages.

Note: On many newer games (Left 4 Dead/2008+), the display of this to clients is broken.
    Additionally, as of 2018, some games also require the client to have cl_showpluginmessages
    set to 1, a non-default value, for this to function.

@param client        Index of the client.
@param kv            KeyValues handle to set the menu keys and options. (Check iserverplugin.h for more information).
@param type          Message type to display ingame.
@error               Invalid client index, or client not in game."""
    pass
def GuessSDKVersion() -> int:
    pass
def GetEngineVersion() -> EngineVersion:
    """Gets the engine version that the currently-loaded SM core was compiled against.

The engine version values are not guaranteed to be in any particular order,
and should only be compared by (in)equality.

@return              An EngineVersion value."""
    pass
def PrintToChat(client: int, format: str, _0: Any, *args: Any) -> None:
    """Prints a message to a specific client in the chat area.

@param client        Client index.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error               Invalid client index, or client not in game."""
    pass
def PrintToChatAll(format: str, _0: Any, *args: Any) -> None:
    """Prints a message to all clients in the chat area.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def PrintCenterText(client: int, format: str, _0: Any, *args: Any) -> None:
    """Prints a message to a specific client in the center of the screen.

@param client        Client index.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error               Invalid client index, or client not in game."""
    pass
def PrintCenterTextAll(format: str, _0: Any, *args: Any) -> None:
    """Prints a message to all clients in the center of the screen.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def PrintHintText(client: int, format: str, _0: Any, *args: Any) -> None:
    """Prints a message to a specific client with a hint box.

@param client        Client index.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error               Invalid client index, or client not in game."""
    pass
def PrintHintTextToAll(format: str, _0: Any, *args: Any) -> None:
    """Prints a message to all clients with a hint box.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def ShowVGUIPanel(client: int, name: str, Kv: Any = ..., show: bool = ...) -> None:
    """Shows a VGUI panel to a specific client.

@param client        Client index.
@param name          Panel type name (Check viewport_panel_names.h to see a list of
                     some panel names).
@param Kv            KeyValues handle with all the data for the panel setup (Depends
                     on the panel type and may be unused).
@param show          True to show the panel, or false to remove it from the client screen.
@error               Invalid client index, or client not in game."""
    pass
def CreateHudSynchronizer() -> Any:
    """Creates a HUD synchronization object.  This object is used to automatically assign and
re-use channels for a set of messages.

The HUD has a hardcoded number of channels (usually 6) for displaying
text.  You can use any channel for any area of the screen.  Text on
different channels can overlap, but text on the same channel will
erase the old text first.  This overlapping and overwriting gets problematic.

A HUD synchronization object automatically selects channels for you based on
the following heuristics:
 - If channel X was last used by the object, and hasn't been modified again,
   channel X gets re-used.
 - Otherwise, a new channel is chosen based on the least-recently-used channel.

This ensures that if you display text on a sync object, that the previous text
displayed on it will always be cleared first.  This is because your new text
will either overwrite the old text on the same channel, or because another
channel has already erased your text.

Note that messages can still overlap if they are on different synchronization
objects, or they are displayed to manual channels.

These are particularly useful for displaying repeating or refreshing HUD text, in
addition to displaying multiple message sets in one area of the screen (for example,
center-say messages that may pop up randomly that you don't want to overlap each
other).

@return              New HUD synchronization object.
                     The Handle can be closed with CloseHandle().
                     If HUD text is not supported on this mod, then
                     INVALID_HANDLE is returned."""
    pass
def SetHudTextParams(x: float, y: float, holdTime: float, r: int, g: int, b: int, a: int, effect: int = ..., fxTime: float = ..., fadeIn: float = ..., fadeOut: float = ...) -> None:
    """Sets the HUD parameters for drawing text.  These parameters are stored
globally, although nothing other than this function and SetHudTextParamsEx
modify them.

You must call this function before drawing text.  If you are drawing
text to multiple clients, you can set the parameters once, since
they won't be modified.  However, as soon as you pass control back
to other plugins, you must reset the parameters next time you draw.

@param x             x coordinate, from 0 to 1.  -1.0 is the center.
@param y             y coordinate, from 0 to 1.  -1.0 is the center.
@param holdTime      Number of seconds to hold the text.
@param r             Red color value.
@param g             Green color value.
@param b             Blue color value.
@param a             Alpha transparency value.
@param effect        0/1 causes the text to fade in and fade out.
                     2 causes the text to flash[?].
@param fxTime        Duration of chosen effect (may not apply to all effects).
@param fadeIn        Number of seconds to spend fading in.
@param fadeOut       Number of seconds to spend fading out."""
    pass
def SetHudTextParamsEx(x: float, y: float, holdTime: float, color1: list[int], color2: list[int] = ..., effect: int = ..., fxTime: float = ..., fadeIn: float = ..., fadeOut: float = ...) -> None:
    """Sets the HUD parameters for drawing text.  These parameters are stored
globally, although nothing other than this function and SetHudTextParams
modify them.

This is the same as SetHudTextParams(), except it lets you set the alternate
color for when effects require it.

@param x             x coordinate, from 0 to 1.  -1.0 is the center.
@param y             y coordinate, from 0 to 1.  -1.0 is the center.
@param holdTime      Number of seconds to hold the text.
@param color1        First color set, array values being [red, green, blue, alpha]
@param color2        Second color set, array values being [red, green, blue, alpha]
@param effect        0/1 causes the text to fade in and fade out.
                     2 causes the text to flash[?].
@param fxTime        Duration of chosen effect (may not apply to all effects).
@param fadeIn        Number of seconds to spend fading in.
@param fadeOut       Number of seconds to spend fading out."""
    pass
def ShowSyncHudText(client: int, sync: Any, message: str, _0: Any, *args: Any) -> int:
    """Shows a synchronized HUD message to a client.

As of this writing, only TF, HL2MP, and SourceForts support HUD Text.

@param client        Client index to send the message to.
@param sync          Synchronization object.
@param message       Message text or formatting rules.
@param ...           Message formatting parameters.
@return              -1 on failure, anything else on success.
                     This function fails if the mod does not support it.
@error               Invalid client index, client not in game, or sync object not valid."""
    pass
def ClearSyncHud(client: int, sync: Any) -> None:
    """Clears the text on a synchronized HUD channel.

This is not the same as sending "" because it guarantees that it won't
overwrite text on another channel.  For example, consider the scenario:

1. Your synchronized message goes to channel 3.
2. Someone else's non-synchronized message goes to channel 3.

If you were to simply send "" on your synchronized message,
then someone else's text could be overwritten.

@param client        Client index to send the message to.
@param sync          Synchronization object.
@error               Invalid client index, client not in game, or sync object not valid."""
    pass
def ShowHudText(client: int, channel: int, message: str, _0: Any, *args: Any) -> int:
    """Shows a HUD message to a client on the given channel.

Note: while many games support HUD Text, not all do.

@param client        Client index to send the message to.
@param channel       A channel number.
                     If -1, then a channel will automatically be selected
                     based on the least-recently-used channel.  If the
                     channel is any other number, it will be modulo'd with
                     the channel count to get a final channel number.
@param message       Message text or formatting rules.
@param ...           Message formatting parameters.
@return              -1 on failure (lack of mod support).
                     Any other return value is the channel number that was
                     used to render the text.
@error               Invalid client index, or client not in game."""
    pass
def ShowMOTDPanel(client: int, title: str, msg: str, type: int = ...) -> None:
    """Shows a MOTD panel to a specific client.

@param client        Client index.
@param title         Title of the panel (printed on the top border of the window).
@param msg           Contents of the panel, it can be treated as an url, filename or plain text
                     depending on the type parameter (WARNING: msg has to be 192 bytes maximum!)
@param type          Determines the way to treat the message body of the panel.
@error               Invalid client index, or client not in game."""
    pass
def DisplayAskConnectBox(client: int, time: float, ip: str, password: str = ...) -> None:
    """Displays a panel asking the client to connect to a specified IP.

Note: On many newer games (Left 4 Dead/2008+), the display of this to clients is broken.
    Additionally, as of 2018, some games also require the client to have cl_showpluginmessages
    set to 1, a non-default value, for this to function.

@param client        Client index.
@param time          Duration to hold the panel on the client's screen.
@param ip            Destination IP.
@param password      Password to connect to the destination IP. The client will be able to see this.
@error               Invalid client index, or client not in game."""
    pass
def EntIndexToEntRef(entity: int) -> int:
    """Converts an entity index into a serial encoded entity reference.

@param entity        Entity index.
@return              Entity reference or -1 on invalid entity.
@error               Entity index >= GetMaxEntities() or < 0"""
    pass
def EntRefToEntIndex(ref: int) -> int:
    """Retrieves the entity index from a reference or validates an entity index.
The input ref is checked that it is still valid and refers to the same entity.

@param ref           Entity reference or index.
@return              Entity index or returns INVALID_ENT_REFERENCE if ref is invalid."""
    pass
def MakeCompatEntRef(ref: int) -> int:
    """Converts a reference into a backwards compatible version.

@param ref           Entity reference.
@return              Bcompat reference."""
    pass
def GetClientsInRange(origin: list[float], rangeType: ClientRangeType, clients: list[int], size: int) -> int:
    """Find clients that are potentially in range of a position.

@param origin        Coordinates from which to test range.
@param rangeType     Range type to use for filtering clients.
@param clients       Array to which found client indexes will be written.
@param size          Maximum size of clients array.
@return              Number of client indexes written to clients array."""
    pass
def GetServerAuthId(authType: AuthIdType, auth: str, maxlen: int) -> None:
    """Retrieves the server's authentication string (SteamID).

Note: If called before server is connected to Steam, auth id
will be invalid ([I:0:1], 1, etc.)

@param authType      Auth id type and format to use.
                     (Only AuthId_Steam3 and AuthId_SteamID64 are supported)
@param auth          Buffer to store the server's auth id.
@param maxlen        Maximum length of string buffer (includes NULL terminator).
@error               Invalid AuthIdType given."""
    pass
def GetServerSteamAccountId() -> int:
    """Returns the server's Steam account ID.

@return              Steam account ID or 0 if not available."""
    pass
SOURCE_SDK_UNKNOWN: Any = ...  # 0      /**< Could not determine the engine version */
SOURCE_SDK_ORIGINAL: Any = ...  # 10      /**< Original Source engine (still used by "The Ship") */
SOURCE_SDK_DARKMESSIAH: Any = ...  # 15      /**< Modified version of original engine used by Dark Messiah (no SDK) */
SOURCE_SDK_EPISODE1: Any = ...  # 20      /**< SDK+Engine released after Episode 1 */
SOURCE_SDK_EPISODE2: Any = ...  # 30      /**< SDK+Engine released after Episode 2/Orange Box */
SOURCE_SDK_BLOODYGOODTIME: Any = ...  # 32      /**< Modified version of ep2 engine used by Bloody Good Time (no SDK) */
SOURCE_SDK_EYE: Any = ...  # 33      /**< Modified version of ep2 engine used by E.Y.E Divine Cybermancy (no SDK) */
SOURCE_SDK_CSS: Any = ...  # 34      /**< Sometime-older version of Source 2009 SDK+Engine, used for Counter-Strike: Source */
SOURCE_SDK_EPISODE2VALVE: Any = ...  # 35      /**< SDK+Engine released after Episode 2/Orange Box, "Source 2009" or "Source MP" */
SOURCE_SDK_LEFT4DEAD: Any = ...  # 40      /**< Engine released after Left 4 Dead (no SDK yet) */
SOURCE_SDK_LEFT4DEAD2: Any = ...  # 50      /**< Engine released after Left 4 Dead 2 (no SDK yet) */
SOURCE_SDK_ALIENSWARM: Any = ...  # 60      /**< SDK+Engine released after Alien Swarm */
SOURCE_SDK_CSGO: Any = ...  # 80      /**< Engine released after CS:GO (no SDK yet) */
SOURCE_SDK_DOTA: Any = ...  # 90      /**< Engine released after Dota 2 (no SDK) */
MOTDPANEL_TYPE_TEXT: Any = ...  # 0      /**< Treat msg as plain text */
MOTDPANEL_TYPE_INDEX: Any = ...  # 1      /**< Msg is auto determined by the engine */
MOTDPANEL_TYPE_URL: Any = ...  # 2      /**< Treat msg as an URL link */
MOTDPANEL_TYPE_FILE: Any = ...  # 3      /**< Treat msg as a filename to be opened */
INVALID_ENT_REFERENCE: Any = ...  # 0xFFFFFFFF
fxTime: float = ...
color2: int = ...
fadeIn: float = ...
kv: Any = ...