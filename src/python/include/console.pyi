from typing import Any, list, Callable, Union
from .admin import *
from .core import *
from .handles import *


class QueryCookie:
    """Console variable query helper values."""
    QUERYCOOKIE_FAILED: int = ...


class ReplySource:
    """Reply sources for commands."""
    SM_REPLY_TO_CHAT: int = ...
    SM_REPLY_TO_CONSOLE: int = ...


def ServerCommand(format: str, _0: Any, *args: Any) -> None:
    """Executes a server command as if it were on the server console (or RCON)

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def ServerCommandEx(buffer: str, maxlen: int, format: str, _0: Any, *args: Any) -> None:
    """Executes a server command as if it were on the server console (or RCON) 
and stores the printed text into buffer.

Warning: This calls ServerExecute internally and may have issues if
certain commands are in the buffer, only use when you really need
the response.
Also, on L4D2 this will not print the command output to the server console.

@param buffer        String to store command result into.
@param maxlen        Length of buffer.
@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def InsertServerCommand(format: str, _0: Any, *args: Any) -> None:
    """Inserts a server command at the beginning of the server command buffer.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def ServerExecute() -> None:
    """Executes every command in the server's command buffer, rather than once per frame."""
    pass
def ClientCommand(client: int, fmt: str, _0: Any, *args: Any) -> None:
    """Executes a client command.  Note that this will not work on clients unless
they have cl_restrict_server_commands set to 0.

@param client        Index of the client.
@param fmt           Format of the client command.
@param ...           Format parameters
@error               Invalid client index, or client not connected."""
    pass
def FakeClientCommand(client: int, fmt: str, _0: Any, *args: Any) -> None:
    """Executes a client command on the server without being networked.

FakeClientCommand() overwrites the command tokenization buffer.  This can 
cause undesired effects because future calls to GetCmdArg* will return 
data from the FakeClientCommand(), not the parent command.  If you are in 
a hook where this matters (for example, a "say" hook), you should use 
FakeClientCommandEx() instead.

@param client        Index of the client.
@param fmt           Format of the client command.
@param ...           Format parameters
@error               Invalid client index, or client not connected."""
    pass
def FakeClientCommandEx(client: int, fmt: str, _0: Any, *args: Any) -> None:
    """Executes a client command on the server without being networked.  The 
execution of the client command is delayed by one frame to prevent any 
re-entrancy issues that might surface with FakeClientCommand().

@param client        Index of the client.
@param fmt           Format of the client command.
@param ...           Format parameters
@error               Invalid client index, or client not connected."""
    pass
def FakeClientCommandKeyValues(client: int, kv: Any) -> None:
    """Executes a KeyValues client command on the server without being networked.

@param client        Index of the client.
@param kv            KeyValues data to be sent.
@error               Invalid client index, client not connected,
                     or unsupported on current game."""
    pass
def PrintToServer(format: str, _0: Any, *args: Any) -> None:
    """Sends a message to the server console.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def PrintToConsole(client: int, format: str, _0: Any, *args: Any) -> None:
    """Sends a message to a client's console.

@param client        Client index.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error               If the client is not connected an error will be thrown."""
    pass
def PrintToConsoleAll(format: str, _0: Any, *args: Any) -> None:
    """Sends a message to every client's console.

@param format        Formatting rules.
@param ...           Variable number of format parameters."""
    pass
def ReplyToCommand(client: int, format: str, _0: Any, *args: Any) -> None:
    """Replies to a message in a command.

A client index of 0 will use PrintToServer().
If the command was from the console, PrintToConsole() is used.
If the command was from chat, PrintToChat() is used.

@param client        Client index, or 0 for server.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error               If the client is not connected or invalid."""
    pass
def GetCmdReplySource() -> ReplySource:
    """Returns the current reply source of a command.

@return              ReplySource value."""
    pass
def SetCmdReplySource(source: ReplySource) -> ReplySource:
    """Sets the current reply source of a command.

Only use this if you know what you are doing.  You should save the old value
and restore it once you are done.

@param source        New ReplySource value.
@return              Old ReplySource value."""
    pass
def IsChatTrigger() -> bool:
    """Returns whether the current say hook is a chat trigger.

This function is only meaningful inside say or say_team hooks.

@return              True if a chat trigger, false otherwise."""
    pass
def GetPublicChatTriggers(buffer: str, maxlength: int) -> int:
    """Get the list of characters used for public chat triggers.

@param buffer        Buffer to use for storing the string.
@param maxlength     Maximum length of the buffer.
@return              Length of string written to buffer."""
    pass
def GetSilentChatTriggers(buffer: str, maxlength: int) -> int:
    """Get the list of characters used for silent chat triggers.

@param buffer        Buffer to use for storing the string.
@param maxlength     Maximum length of the buffer.
@return              Length of string written to buffer."""
    pass
def ShowActivity2(client: int, tag: str, format: str, _0: Any, *args: Any) -> None:
    """Displays usage of an admin command to users depending on the 
setting of the sm_show_activity cvar.  All users receive a message 
in their chat text, except for the originating client, who receives 
the message based on the current ReplySource.

@param client        Client index doing the action, or 0 for server.
@param tag           Tag to prepend to the message.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error"""
    pass
def ShowActivity(client: int, format: str, _0: Any, *args: Any) -> None:
    """Displays usage of an admin command to users depending on the 
setting of the sm_show_activity cvar.  

This version does not display a message to the originating client 
if used from chat triggers or menus.  If manual replies are used 
for these cases, then this function will suffice.  Otherwise, 
ShowActivity2() is slightly more useful.

@param client        Client index doing the action, or 0 for server.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error"""
    pass
def ShowActivityEx(client: int, tag: str, format: str, _0: Any, *args: Any) -> None:
    """Same as ShowActivity(), except the tag parameter is used instead of
"[SM] " (note that you must supply any spacing).

@param client        Client index doing the action, or 0 for server.
@param tag           Tag to display with.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@error"""
    pass
def FormatActivitySource(client: int, target: int, namebuf: str, maxlength: int) -> bool:
    """Given an originating client and a target client, returns the string 
that describes the originating client according to the sm_show_activity cvar.

For example, "ADMIN", "PLAYER", or a player's name could be placed in this buffer.

@param client        Originating client; may be 0 for server console.
@param target        Targeted client.
@param namebuf       Name buffer.
@param maxlength     Maximum size of the name buffer.
@return              True if activity should be shown.  False otherwise.  In either 
                     case, the name buffer is filled.  The return value can be used 
                     to broadcast a "safe" name to all players regardless of the 
                     sm_show_activity filters.
@error               Invalid client index or client not connected."""
    pass
def RegServerCmd(cmd: str, callback: Any, description: str = ..., flags: int = ...) -> None:
    """Creates a server-only console command, or hooks an already existing one.  

Server commands are case sensitive.

@param cmd           Name of the command to hook or create.
@param callback      A function to use as a callback for when the command is invoked.
@param description   Optional description to use for command creation.
@param flags         Optional flags to use for command creation.
@error               Command name is the same as an existing convar."""
    pass
def RegConsoleCmd(cmd: str, callback: Any, description: str = ..., flags: int = ...) -> None:
    """Creates a console command, or hooks an already existing one.

Console commands are case sensitive.  However, if the command already exists in the game, 
a client may enter the command in any case.  SourceMod corrects for this automatically, 
and you should only hook the "real" version of the command.

@param cmd           Name of the command to hook or create.
@param callback      A function to use as a callback for when the command is invoked.
@param description   Optional description to use for command creation.
@param flags         Optional flags to use for command creation.
@error               Command name is the same as an existing convar."""
    pass
def RegAdminCmd(cmd: str, callback: Any, adminflags: int, description: str = ..., group: str = ..., flags: int = ...) -> None:
    """Creates a console command as an administrative command.  If the command does not exist,
it is created.  When this command is invoked, the access rights of the player are 
automatically checked before allowing it to continue.

Admin commands are case sensitive from both the client and server.

@param cmd           String containing command to register.
@param callback      A function to use as a callback for when the command is invoked.
@param adminflags    Administrative flags (bitstring) to use for permissions.
@param description   Optional description to use for help.
@param group         String containing the command group to use.  If empty,
                     the plugin's filename will be used instead.
@param flags         Optional console flags.
@error               Command name is the same as an existing convar."""
    pass
def GetCmdArgs() -> int:
    """Returns the number of arguments from the current console or server command.
@note Unlike the HL2 engine call, this does not include the command itself.

@return              Number of arguments to the current command."""
    pass
def GetCmdArg(argnum: int, buffer: str, maxlength: int) -> int:
    """Retrieves a command argument given its index, from the current console or 
server command.
@note Argument indexes start at 1; 0 retrieves the command name.

@param argnum        Argument number to retrieve.
@param buffer        Buffer to use for storing the string.
@param maxlength     Maximum length of the buffer.
@return              Length of string written to buffer."""
    pass
def GetCmdArgInt(argnum: int) -> int:
    """Retrieves a numeric command argument given its index, from the current
console or server command. Will return 0 if the argument can not be
parsed as a number. Use GetCmdArgIntEx to handle that explicitly.

@param argnum        Argument number to retrieve.
@return              Value of the command argument."""
    pass
def GetCmdArgIntEx(argnum: int, value: int) -> bool:
    """Retrieves a numeric command argument given its index, from the current
console or server command. Returns false if the argument can not be
completely parsed as an integer.

@param argnum        Argument number to retrieve.
@param value         Populated with the value of the command argument.
@return              Whether the argument was entirely a numeric value."""
    pass
def GetCmdArgFloat(argnum: int) -> float:
    """Retrieves a float command argument given its index, from the current
console or server command. Will return 0.0 if the argument can not be
parsed as a number. Use GetCmdArgFloatEx to handle that explicitly.

@param argnum        Argument number to retrieve.
@return              Value of the command argument."""
    pass
def GetCmdArgFloatEx(argnum: int, value: float) -> bool:
    """Retrieves a float command argument given its index, from the current
console or server command. Returns false if the argument can not be
completely parsed as a floating point.

@param argnum        Argument number to retrieve.
@param value         Populated with the value of the command argument.
@return              Whether the argument was entirely a floating point value."""
    pass
def GetCmdArgString(buffer: str, maxlength: int) -> int:
    """Retrieves the entire command argument string in one lump from the current 
console or server command.

@param buffer        Buffer to use for storing the string.
@param maxlength     Maximum length of the buffer.
@return              Length of string written to buffer."""
    pass
def CommandIterator() -> Any:
    pass
def Next() -> bool:
    pass
def GetDescription(buffer: str, maxlen: int) -> None:
    pass
def GetName(buffer: str, maxlen: int) -> None:
    pass
def get() -> Any:
    pass
def GetCommandIterator() -> Any:
    """Gets a command iterator.  Must be freed with CloseHandle().

@return              A new command iterator."""
    pass
def ReadCommandIterator(iter: Any, name: str, nameLen: int, eflags: int = ..., desc: str = ..., descLen: int = ...) -> bool:
    """Reads a command iterator, then advances to the next command if any.
Only SourceMod specific commands are returned.

@param iter          Command iterator Handle.
@param name          Name buffer.
@param nameLen       Name buffer size.
@param eflags        Effective default flags of a command.
@param desc          Command description buffer.
@param descLen       Command description buffer size.
@return              True on success, false if there are no more commands."""
    pass
def CheckCommandAccess(client: int, command: str, flags: int, override_only: bool = ...) -> bool:
    """Returns whether a client has access to a given command string.  The string 
can be any override string, as overrides can be independent of 
commands.  This feature essentially allows you to create custom 
flags using the override system.

@param client        Client index.
@param command       Command name.  If the command is not found, the default 
                     flags are used.
@param flags         Flag string to use as a default, if the command or override 
                     is not found.
@param override_only If true, SourceMod will not attempt to find a matching 
                     command, and it will only use the default flags specified.
                     Otherwise, SourceMod will ignore the default flags if 
                     there is a matching admin command.
@return              True if the client has access, false otherwise."""
    pass
def CheckAccess(id: AdminId, command: str, flags: int, override_only: bool = ...) -> bool:
    """Returns whether an admin has access to a given command string.  The string 
can be any override string, as overrides can be independent of 
commands.  This feature essentially allows you to create custom flags
using the override system.

@param id            AdminId of the admin.
@param command       Command name.  If the command is not found, the default 
                     flags are used.
@param flags         Flag string to use as a default, if the command or override 
                     is not found.
@param override_only If true, SourceMod will not attempt to find a matching 
                     command, and it will only use the default flags specified.
                     Otherwise, SourceMod will ignore the default flags if 
                     there is a matching admin command.
@return              True if the admin has access, false otherwise."""
    pass
def GetCommandFlags(name: str) -> int:
    """Returns the bitstring of flags of a command.

@param name          Name of the command.
@return              A bitstring containing the FCVAR_* flags that are enabled 
                     or INVALID_FCVAR_FLAGS if command not found."""
    pass
def SetCommandFlags(name: str, flags: int) -> bool:
    """Sets the bitstring of flags of a command.

@param name          Name of the command.
@param flags         A bitstring containing the FCVAR_* flags to enable.
@return              True on success, otherwise false."""
    pass
def FindFirstConCommand(buffer: str, max_size: int, isCommand: bool, flags: int = ..., description: str = ..., descrmax_size: int = ...) -> Any:
    """Starts a ConCommandBase search, traversing the list of ConVars and 
ConCommands.  If a Handle is returned, the next entry must be read 
via FindNextConCommand().  The order of the list is undefined.

@param buffer        Buffer to store entry name.
@param max_size      Maximum size of the buffer.
@param isCommand     Variable to store whether the entry is a command. 
                     If it is not a command, it is a ConVar.
@param flags         Variable to store entry flags.
@param description   Buffer to store the description, empty if no description present.
@param descrmax_size Maximum size of the description buffer.
@return              On success, a ConCmdIter Handle is returned, which 
                        can be read via FindNextConCommand(), and must be 
                        closed via CloseHandle().  Additionally, the output 
                        parameters will be filled with information of the 
                        first ConCommandBase entry.
                        On failure, INVALID_HANDLE is returned, and the 
                        contents of outputs is undefined."""
    pass
def FindNextConCommand(search: Any, buffer: str, max_size: int, isCommand: bool, flags: int = ..., description: str = ..., descrmax_size: int = ...) -> bool:
    """Reads the next entry in a ConCommandBase iterator.

@param search        ConCmdIter Handle to search.
@param buffer        Buffer to store entry name.
@param max_size      Maximum size of the buffer.
@param isCommand     Variable to store whether the entry is a command.
                        If it is not a command, it is a ConVar.
@param flags         Variable to store entry flags.
@param description   Buffer to store the description, empty if no description present.
@param descrmax_size Maximum size of the description buffer.
@return              On success, the outputs are filled, the iterator is 
                        advanced to the next entry, and true is returned.  
                        If no more entries exist, false is returned, and the 
                        contents of outputs is undefined."""
    pass
def AddServerTag(tag: str) -> None:
    """Adds an informational string to the server's public "tags".
This string should be a short, unique identifier.

Note: Tags are automatically removed when a plugin unloads.
Note: Currently, this function does nothing because of bugs in the Valve master.

@param tag           Tag string to append."""
    pass
def RemoveServerTag(tag: str) -> None:
    """Removes a tag previously added by the calling plugin.

@param tag           Tag string to remove."""
    pass
def AddCommandListener(callback: Any, command: str = ...) -> bool:
    """Adds a callback that will fire when a command is sent to the server.

Registering commands is designed to create a new command as part of the UI,
whereas this is a lightweight hook on a command string, existing or not.
Using Reg*Cmd to intercept is in poor practice, as it physically creates a
new command and can slow down dispatch in general.

To see if this feature is available, use FeatureType_Capability and 
FEATURECAP_COMMANDLISTENER.

@param callback      Callback.
@param command       Command, or if not specified, a global listener.
                     The command is case insensitive.
@return              True if this feature is available on the current game,
                     false otherwise."""
    pass
def RemoveCommandListener(callback: Any, command: str = ...) -> None:
    """Removes a previously added command listener, in reverse order of being added.

@param callback      Callback.
@param command       Command, or if not specified, a global listener.
                     The command is case insensitive.
@error               Callback has no active listeners."""
    pass
def CommandExists(command: str) -> bool:
    """Returns true if the supplied command exists.

@param command       Command to find.
@return              True if command is found, false otherwise."""
    pass
def OnClientSayCommand(client: int, command: str, sArgs: str) -> Any:
    """Global listener for the chat commands.

@param client        Client index.
@param command       Command name.
@param sArgs         Chat argument string.

@return              An Action value. Returning Plugin_Handled bypasses the game function call.
                     Returning Plugin_Stop bypasses the post hook as well as the game function."""
    pass
def OnClientSayCommand_Post(client: int, command: str, sArgs: str) -> None:
    """Global post listener for the chat commands.

@param client        Client index.
@param command       Command name.
@param sArgs         Chat argument string."""
    pass
INVALID_FCVAR_FLAGS: Any = ...  # (-1)
FCVAR_PLUGIN: Any = ...  # 0
FCVAR_LAUNCHER: Any = ...  # (1<<1)
FCVAR_NONE: Any = ...  # 0
FCVAR_UNREGISTERED: Any = ...  # (1<<0)
FCVAR_DEVELOPMENTONLY: Any = ...  # (1<<1)
FCVAR_GAMEDLL: Any = ...  # (1<<2)
FCVAR_CLIENTDLL: Any = ...  # (1<<3)
FCVAR_MATERIAL_SYSTEM: Any = ...  # (1<<4)
FCVAR_HIDDEN: Any = ...  # (1<<4)
FCVAR_PROTECTED: Any = ...  # (1<<5)
FCVAR_SPONLY: Any = ...  # (1<<6)
FCVAR_ARCHIVE: Any = ...  # (1<<7)
FCVAR_NOTIFY: Any = ...  # (1<<8)
FCVAR_USERINFO: Any = ...  # (1<<9)
FCVAR_PRINTABLEONLY: Any = ...  # (1<<10)
FCVAR_UNLOGGED: Any = ...  # (1<<11)
FCVAR_NEVER_AS_STRING: Any = ...  # (1<<12)
FCVAR_REPLICATED: Any = ...  # (1<<13)
FCVAR_CHEAT: Any = ...  # (1<<14)
FCVAR_SS: Any = ...  # (1<<15)
FCVAR_DEMO: Any = ...  # (1<<16)
FCVAR_DONTRECORD: Any = ...  # (1<<17)
FCVAR_SS_ADDED: Any = ...  # (1<<18)
FCVAR_RELEASE: Any = ...  # (1<<19)
FCVAR_RELOAD_MATERIALS: Any = ...  # (1<<20)
FCVAR_RELOAD_TEXTURES: Any = ...  # (1<<21)
FCVAR_NOT_CONNECTED: Any = ...  # (1<<22)
FCVAR_MATERIAL_SYSTEM_THREAD: Any = ...  # (1<<23)
FCVAR_ARCHIVE_XBOX: Any = ...  # (1<<24)
FCVAR_ARCHIVE_GAMECONSOLE: Any = ...  # (1<<24)
FCVAR_ACCESSIBLE_FROM_THREADS: Any = ...  # (1<<25)
FCVAR_SERVER_CAN_EXECUTE: Any = ...  # (1<<28)
FCVAR_SERVER_CANNOT_QUERY: Any = ...  # (1<<29)
FCVAR_CLIENTCMD_CAN_EXECUTE: Any = ...  # (1<<30)
FEATURECAP_COMMANDLISTENER: Any = ...  # "command listener"
SrvCmd: Any = ...
ConCmd: Any = ...
flags: int = ...
len: int = ...
descLen: int = ...
override_only: bool = ...
CommandListener: Any = ...