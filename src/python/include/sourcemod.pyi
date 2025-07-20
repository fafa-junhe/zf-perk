from typing import Any, list, Callable, Union
from .admin import *
from .adt import *
from .banning import *
from .bitbuffer import *
from .clients import *
from .commandfilters import *
from .commandline import *
from .console import *
from .convars import *
from .core import *
from .dbi import *
from .entity import *
from .entity_prop_stocks import *
from .entitylump import *
from .events import *
from .files import *
from .float import *
from .functions import *
from .halflife import *
from .handles import *
from .helpers import *
from .keyvalues import *
from .lang import *
from .logging import *
from .menus import *
from .nextmap import *
from .protobuf import *
from .sorting import *
from .string import *
from .textparse import *
from .timers import *
from .usermessages import *
from .vector import *


class APLRes:
    APLRes_Failure: int = ...
    APLRes_SilentFailure: int = ...
    APLRes_Success: int = ...


class Address:
    Address_Null: int = ...


class FeatureStatus:
    """Feature statuses."""
    FeatureStatus_Available: int = ...
    FeatureStatus_Unavailable: int = ...
    FeatureStatus_Unknown: int = ...


class FeatureType:
    """Feature types."""
    FeatureType_Capability: int = ...
    FeatureType_Native: int = ...


class NumberType:
    """Represents how many bytes we can read from an address with one load"""
    NumberType_Int16: int = ...
    NumberType_Int32: int = ...
    NumberType_Int8: int = ...


class Plugin:
    """Plugin public information."""
    author: str = ...
    description: str = ...
    name: str = ...
    url: str = ...
    version: str = ...


def GameData(file: str) -> Any:
    pass
def GetOffset(key: str) -> int:
    pass
def GetKeyValue(key: str, buffer: str, maxlen: int) -> bool:
    pass
def GetAddress(name: str) -> Address:
    pass
def GetMemSig(name: str) -> Address:
    pass
def OnPluginStart() -> None:
    """Called when the plugin is fully initialized and all known external references
are resolved. This is only called once in the lifetime of the plugin, and is
paired with OnPluginEnd().

If any run-time error is thrown during this callback, the plugin will be marked
as failed."""
    pass
def AskPluginLoad(myself: Any, late: bool, error: str, err_max: int) -> bool:
    pass
def AskPluginLoad2(myself: Any, late: bool, error: str, err_max: int) -> APLRes:
    """Called before OnPluginStart, in case the plugin wants to check for load failure.
This is called even if the plugin type is "private."  Any natives from modules are
not available at this point.  Thus, this forward should only be used for explicit
pre-emptive things, such as adding dynamic natives, setting certain types of load
filters (such as not loading the plugin for certain games).

@note It is not safe to call externally resolved natives until OnPluginStart().
@note Any sort of RTE in this function will cause the plugin to fail loading.
@note If you do not return anything, it is treated like returning success.
@note If a plugin has an AskPluginLoad2(), AskPluginLoad() will not be called.

@param myself        Handle to the plugin.
@param late          Whether or not the plugin was loaded "late" (after map load).
@param error         Error message buffer in case load failed.
@param err_max       Maximum number of characters for error message buffer.
@return              APLRes_Success for load success, APLRes_Failure or APLRes_SilentFailure otherwise"""
    pass
def OnPluginEnd() -> None:
    """Called when the plugin is about to be unloaded.

It is not necessary to close any handles or remove hooks in this function.
SourceMod guarantees that plugin shutdown automatically and correctly releases
all resources."""
    pass
def OnPluginPauseChange(pause: bool) -> None:
    """Called when the plugin's pause status is changing.

@param pause         True if the plugin is being paused, false otherwise."""
    pass
def OnGameFrame() -> None:
    """Called before every server frame.  Note that you should avoid
doing expensive computations or declaring large local arrays."""
    pass
def OnMapInit(mapName: str) -> None:
    """Called when the map starts loading.

@param mapName Name of the map"""
    pass
def OnMapStart() -> None:
    """Called when the map is loaded."""
    pass
def OnMapEnd() -> None:
    """Called right before a map ends."""
    pass
def OnConfigsExecuted() -> None:
    """Called when the map has loaded, servercfgfile (server.cfg) has been
executed, and all plugin configs are done executing.  This is the best
place to initialize plugin functions which are based on cvar data.

@note This will always be called once and only once per map.  It will be
      called after OnMapStart()."""
    pass
def OnAutoConfigsBuffered() -> None:
    """This is called once, right after OnMapStart() but any time before
OnConfigsExecuted().  It is called after the "exec sourcemod.cfg"
command and all AutoExecConfig() exec commands have been added to
the ServerCommand() buffer.

If you need to load per-map settings that override default values,
adding commands to the ServerCommand() buffer here will guarantee
that they're set before OnConfigsExecuted().

Unlike OnMapStart() and OnConfigsExecuted(), this is not called on
late loads that occur after OnMapStart()."""
    pass
def OnServerCfg() -> None:
    pass
def OnAllPluginsLoaded() -> None:
    """Called after all plugins have been loaded.  This is called once for
every plugin.  If a plugin late loads, it will be called immediately
after OnPluginStart()."""
    pass
def GetMyHandle() -> Any:
    """Returns the calling plugin's Handle.

@return              Handle of the calling plugin."""
    pass
def PluginIterator() -> Any:
    pass
def Next() -> bool:
    pass
def get() -> Any:
    pass
def GetPluginIterator() -> Any:
    """Returns an iterator that can be used to search through plugins.

@return              Handle to iterate with.  Must be closed via
                     CloseHandle().
@error               Invalid Handle."""
    pass
def MorePlugins(iter: Any) -> bool:
    """Returns whether there are more plugins available in the iterator.

@param iter          Handle to the plugin iterator.
@return              True on more plugins, false otherwise.
@error               Invalid Handle."""
    pass
def ReadPlugin(iter: Any) -> Any:
    """Returns the current plugin in the iterator and advances the iterator.

@param iter          Handle to the plugin iterator.
@return              Current plugin the iterator is at, before
                     the iterator is advanced.
@error               Invalid Handle."""
    pass
def GetPluginStatus(plugin: Any) -> PluginStatus:
    """Returns a plugin's status.

@param plugin        Plugin Handle (INVALID_HANDLE uses the calling plugin).
@return              Status code for the plugin.
@error               Invalid Handle."""
    pass
def GetPluginFilename(plugin: Any, buffer: str, maxlength: int) -> None:
    """Retrieves a plugin's file name relative to the plugins folder.

@param plugin        Plugin Handle (INVALID_HANDLE uses the calling plugin).
@param buffer        Buffer to the store the file name.
@param maxlength     Maximum length of the name buffer.
@error               Invalid Handle."""
    pass
def IsPluginDebugging(plugin: Any) -> bool:
    """Retrieves whether or not a plugin is being debugged.

@param plugin        Plugin Handle (INVALID_HANDLE uses the calling plugin).
@return              True if being debugged, false otherwise.
@error               Invalid Handle."""
    pass
def GetPluginInfo(plugin: Any, info: PluginInfo, buffer: str, maxlength: int) -> bool:
    """Retrieves a plugin's public info.

@param plugin        Plugin Handle (INVALID_HANDLE uses the calling plugin).
@param info          Plugin info property to retrieve.
@param buffer        Buffer to store info in.
@param maxlength     Maximum length of buffer.
@return              True on success, false if property is not available.
@error               Invalid Handle."""
    pass
def FindPluginByNumber(order_num: int) -> Any:
    """Finds a plugin by its order in the list from the "plugins list" server
"sm" command.  You should not use this function to loop through all plugins,
use the iterator instead.  Looping through all plugins using this native
is O(n^2), whereas using the iterator is O(n).

@param order_num     Number of the plugin as it appears in "sm plugins list".
@return              Plugin Handle on success, INVALID_HANDLE if no plugin
                     matches the given number."""
    pass
def SetFailState(string: str, _0: Any, *args: Any) -> None:
    """Causes the plugin to enter a failed state.  An error will be thrown and
the plugin will be paused until it is unloaded or reloaded.

For backwards compatibility, if no extra arguments are passed, no
formatting is applied.  If one or more additional arguments is passed,
the string is formatted using Format().  If any errors are encountered
during formatting, both the format specifier string and an additional
error message are written.

This function does not return, and no further code in the plugin is
executed.

@param string        Format specifier string.
@param ...           Formatting arguments.
@error               Always throws SP_ERROR_ABORT."""
    pass
def ThrowError(fmt: str, _0: Any, *args: Any) -> None:
    """Aborts the current callback and throws an error.  This function
does not return in that no code is executed following it.

@param fmt           String format.
@param ...           Format arguments.
@noreturn
@error               Always!"""
    pass
def LogStackTrace(fmt: str, _0: Any, *args: Any) -> None:
    """Logs a stack trace from the current function call. Code
execution continues after the call

@param fmt           Format string to send with the stack trace.
@param ...           Format arguments.
@error               Always logs a stack trace."""
    pass
def GetTime(bigStamp: list[int] = ...) -> int:
    """Gets the system time as a unix timestamp.

@param bigStamp      Optional array to store the 64bit timestamp in.
@return              32bit timestamp (number of seconds since unix epoch)."""
    pass
def FormatTime(buffer: str, maxlength: int, format: str, stamp: int = ...) -> None:
    """Produces a date and/or time string value for a timestamp.

See this URL for valid parameters:
https://cplusplus.com/reference/ctime/strftime/

Note that available parameters depends on support from your operating system.
In particular, ones highlighted in yellow on that page are not currently
available on Windows and should be avoided for portable plugins.

@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer.
@param format        Formatting rules (passing NULL_STRING will use the rules defined in sm_datetime_format).
@param stamp         Optional time stamp.
@error               Buffer too small or invalid time format."""
    pass
def ParseTime(dateTime: str, format: str) -> int:
    """Parses a string representing a date and/or time into a unix timestamp.
The timezone is always interpreted as UTC/GMT.

See this URL for valid parameters:
https://en.cppreference.com/w/cpp/io/manip/get_time

Note that available parameters depends on support from your operating system.
In particular, ones highlighted in yellow on that page are not currently
available on Windows and should be avoided for portable plugins.

@param dateTime     Date and/or time string.
@param format       Formatting rules (passing NULL_STRING will use the rules defined in sm_datetime_format).
@return             32bit timestamp (number of seconds since unix epoch).
@error              Invalid date/time string or time format."""
    pass
def LoadGameConfigFile(file: str) -> Any:
    """Loads a game config file.

@param file          File to load.  The path must be relative to the 'gamedata' folder under the config folder
                     and the extension should be omitted.
@return              A handle to the game config file or INVALID_HANDLE on failure."""
    pass
def GameConfGetOffset(gc: Any, key: str) -> int:
    """Returns an offset value.

@param gc            Game config handle.
@param key           Key to retrieve from the offset section.
@return              An offset, or -1 on failure."""
    pass
def GameConfGetKeyValue(gc: Any, key: str, buffer: str, maxlen: int) -> bool:
    """Gets the value of a key from the "Keys" section.

@param gc            Game config handle.
@param key           Key to retrieve from the Keys section.
@param buffer        Destination string buffer.
@param maxlen        Maximum length of output string buffer.
@return              True if key existed, false otherwise."""
    pass
def GameConfGetAddress(gameconf: Any, name: str) -> Address:
    """Finds an address calculation in a GameConfig file,
performs LoadFromAddress on it as appropriate, then returns the final address.

@param gameconf      Game config handle.
@param name          Name of the property to find.
@return              An address calculated on success, or 0 on failure."""
    pass
def GetSysTickCount() -> int:
    """Returns the operating system's "tick count," which is a number of
milliseconds since the operating system loaded.  This can be used
for basic benchmarks.

@return              Tick count in milliseconds."""
    pass
def AutoExecConfig(autoCreate: bool = ..., name: str = ..., folder: str = ...) -> None:
    """Specifies that the given config file should be executed after plugin load.
OnConfigsExecuted() will not be called until the config file has executed,
but it will be called if the execution fails.

@param autoCreate    If true, and the config file does not exist, such a config
                     file will be automatically created and populated with
                     information from the plugin's registered cvars.
@param name          Name of the config file, excluding the .cfg extension.
                     If empty, <plugin.filename.cfg> is assumed.
@param folder        Folder under cfg/ to use.  By default this is "sourcemod.""""
    pass
def RegPluginLibrary(name: str) -> None:
    """Registers a library name for identifying as a dependency to
other plugins.

@param name          Library name."""
    pass
def LibraryExists(name: str) -> bool:
    """Returns whether a library exists.  This function should be considered
expensive; it should only be called on plugin to determine availability
of resources.  Use OnLibraryAdded()/OnLibraryRemoved() to detect changes
in libraries.

@param name          Library name of a plugin or extension.
@return              True if exists, false otherwise."""
    pass
def GetExtensionFileStatus(name: str, error: str = ..., maxlength: int = ...) -> int:
    """Returns the status of an extension, by filename.

@param name          Extension name (like "sdktools.ext").
@param error         Optional error message buffer.
@param maxlength     Length of optional error message buffer.
@return              -2 if the extension was not found.
                     -1 if the extension was found but failed to load.
                     0 if the extension loaded but reported an error.
                     1 if the extension is running without error."""
    pass
def OnLibraryAdded(name: str) -> None:
    """Called after a library is added.
A library is either a plugin name or extension name, as
exposed via its include file.

@param name          Library name."""
    pass
def OnLibraryRemoved(name: str) -> None:
    """Called right before a library is removed.
A library is either a plugin name or extension name, as
exposed via its include file.

@param name          Library name."""
    pass
def OnNotifyPluginUnloaded(plugin: Any) -> None:
    """Called when a plugin unloaded.

@param plugin        Plugin Handle who unloaded."""
    pass
def ReadMapList(array: Any = ..., serial: int = ..., str: str = ..., flags: int = ...) -> Any:
    """Loads a map list to an ADT Array.

A map list is a list of maps from a file.  SourceMod allows easy configuration of
maplists through addons/sourcemod/configs/maplists.cfg.  Each entry is given a
name and a file (for example, "rtv" => "rtv.cfg"), or a name and a redirection
(for example, "rtv" => "default").  This native will read a map list entry,
cache the file, and return the list of maps it holds.

Serial change numbers are used to identify if a map list has changed.  Thus, if
you pass a serial change number and it's equal to what SourceMod currently knows
about the map list, then SourceMod won't re-parse the file.

If the maps end up being read from the maps folder (MAPLIST_FLAG_MAPSFOLDER), they
are automatically sorted in alphabetical, ascending order.

Arrays created by this function are temporary and must be freed via CloseHandle().
Modifying arrays created by this function will not affect future return values or
or the contents of arrays returned to other plugins.

@param array         Array to store the map list.  If INVALID_HANDLE, a new blank
                     array will be created.  The blocksize should be at least 16;
                     otherwise results may be truncated.  Items are added to the array
                     as strings.  The array is never checked for duplicates, and it is
                     not read beforehand.  Only the serial number is used to detect
                     changes.
@param serial        Serial number to identify last known map list change.  If -1, the
                     the value will not be checked.  If the map list has since changed,
                     the serial is updated (even if -1 was passed).  If there is an error
                     finding a valid maplist, then the serial is set to -1.
@param str           Config name, or "default" for the default map list.  Config names
                     should be somewhat descriptive.  For example, the admin menu uses
                     a config name of "admin menu".  The list names can be configured
                     by users in addons/sourcemod/configs/maplists.cfg.
@param flags         MAPLIST_FLAG flags.
@return              On failure:
                     INVALID_HANDLE is returned, the serial is set to -1, and the input
                     array (if any) is left unchanged.
                     On no change:
                     INVALID_HANDLE is returned, the serial is unchanged, and the input
                     array (if any) is left unchanged.
                     On success:
                     A valid array Handle is returned, containing at least one map string.
                     If an array was passed, the return value is equal to the passed Array
                     Handle.  If the passed array was not cleared, it will have grown by at
                     least one item.  The serial number is updated to a positive number.
@error               Invalid array Handle that is not INVALID_HANDLE."""
    pass
def SetMapListCompatBind(name: str, file: str) -> None:
    """Makes a compatibility binding for map lists.  For example, if a function previously used
"clam.cfg" for map lists, this function will insert a "fake" binding to "clam.cfg" that
will be overridden if it's in the maplists.cfg file.

@param name          Configuration name that would be used with ReadMapList().
@param file          Default file to use."""
    pass
def OnClientFloodCheck(client: int) -> bool:
    """Called when a client has sent chat text.  This must return either true or
false to indicate that a client is or is not spamming the server.

The return value is a hint only.  Core or another plugin may decide
otherwise.

@param client        Client index.  The server (0) will never be passed.
@return              True if client is spamming the server, false otherwise."""
    pass
def OnClientFloodResult(client: int, blocked: bool) -> None:
    """Called after a client's flood check has been computed.  This can be used
by antiflood algorithms to decay/increase flooding weights.

Since the result from "OnClientFloodCheck" isn't guaranteed to be the
final result, it is generally a good idea to use this to play with other
algorithms nicely.

@param client        Client index.  The server (0) will never be passed.
@param blocked       True if client flooded last "say", false otherwise."""
    pass
def CanTestFeatures() -> bool:
    """Returns whether "GetFeatureStatus" will work. Using this native
or this function will not cause SourceMod to fail loading on older versions,
however, GetFeatureStatus will only work if this function returns true.

@return              True if GetFeatureStatus will work, false otherwise."""
    pass
def GetFeatureStatus(type: FeatureType, name: str) -> FeatureStatus:
    """Returns whether a feature exists, and if so, whether it is usable.

@param type          Feature type.
@param name          Feature name.
@return              Feature status."""
    pass
def RequireFeature(type: FeatureType, name: str, fmt: str = ..., _0: Any = ..., *args: Any) -> None:
    """Requires that a given feature is available. If it is not, SetFailState()
is called with the given message.

@param type          Feature type.
@param name          Feature name.
@param fmt           Message format string, or empty to use default.
@param ...           Message format parameters, if any."""
    pass
def LoadFromAddress(addr: Address, size: NumberType) -> Any:
    """Load up to 4 bytes from a memory address.

@param addr          Address to a memory location.
@param size          How many bytes should be read.
                     If loading a floating-point value, use NumberType_Int32.
@return              The value that is stored at that address.
@error               Address is null or pointing to reserved memory."""
    pass
def StoreToAddress(addr: Address, data: Any, size: NumberType, updateMemAccess: bool = ...) -> None:
    """Store up to 4 bytes to a memory address.

@param addr                     Address to a memory location.
@param data                     Value to store at the address.
@param size                     How many bytes should be written.
                                If storing a floating-point value, use NumberType_Int32.
@param updateMemAccess          If true, SourceMod will set read / write / exec permissions
                                on the memory page being written to.
@error                          Address is null or pointing to reserved memory."""
    pass
def FrameIterator() -> Any:
    pass
def Reset() -> None:
    pass
def GetFunctionName(buffer: str, maxlen: int) -> None:
    pass
def GetFilePath(buffer: str, maxlen: int) -> None:
    pass
fmt: str = ...  # "", any ...)
MAPLIST_FLAG_MAPSFOLDER: Any = ...  # (1<<0)    /**< On failure, use all maps in the maps folder. */
MAPLIST_FLAG_CLEARARRAY: Any = ...  # (1<<1)    /**< If an input array is specified, clear it before adding. */
MAPLIST_FLAG_NO_DEFAULT: Any = ...  # (1<<2)    /**< Do not read "default" or "mapcyclefile" on failure. */
flags: int = ...