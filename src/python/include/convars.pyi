from typing import Any, list, Callable, Union
from .console import *
from .handles import *


class ConVarBounds:
    """Console variable bound values used with Get/SetConVarBounds()"""
    ConVarBound_Lower: int = ...
    ConVarBound_Upper: int = ...


class ConVarQueryResult:
    """Console variable query result values."""
    ConVarQuery_Cancelled: int = ...
    ConVarQuery_NotFound: int = ...
    ConVarQuery_NotValid: int = ...
    ConVarQuery_Okay: int = ...
    ConVarQuery_Protected: int = ...


ConVarQueryFinished = Union[
    Callable[[QueryCookie, int, ConVarQueryResult, str, str, Any], None],
    Callable[[QueryCookie, int, ConVarQueryResult, str, str], None]
]
def CreateConVar(name: str, defaultValue: str, description: str = ..., flags: int = ..., hasMin: bool = ..., min: float = ..., hasMax: bool = ..., max: float = ...) -> Any:
    """Creates a new console variable.

@param name          Name of new convar.
@param defaultValue  String containing the default value of new convar.
@param description   Optional description of the convar.
@param flags         Optional bitstring of flags determining how the convar should be handled. See FCVAR_* constants for more details.
@param hasMin        Optional boolean that determines if the convar has a minimum value.
@param min           Minimum floating point value that the convar can have if hasMin is true.
@param hasMax        Optional boolean that determines if the convar has a maximum value.
@param max           Maximum floating point value that the convar can have if hasMax is true.
@return              A handle to the newly created convar. If the convar already exists, a handle to it will still be returned.
@error               Convar name is blank or is the same as an existing console command."""
    pass
def FindConVar(name: str) -> Any:
    """Searches for a console variable.

@param name          Name of convar to find.
@return              A ConVar object if found; null otherwise."""
    pass
def get() -> Any:
    pass
def set(b: bool) -> Any:
    pass
def SetBool(value: bool, replicate: bool = ..., notify: bool = ...) -> None:
    pass
def SetInt(value: int, replicate: bool = ..., notify: bool = ...) -> None:
    pass
def SetFloat(value: float, replicate: bool = ..., notify: bool = ...) -> None:
    pass
def GetString(value: str, maxlength: int) -> None:
    pass
def SetString(value: str, replicate: bool = ..., notify: bool = ...) -> None:
    pass
def RestoreDefault(replicate: bool = ..., notify: bool = ...) -> None:
    pass
def GetDefault(value: str, maxlength: int) -> int:
    pass
def GetBounds(type: ConVarBounds, value: float) -> bool:
    pass
def SetBounds(type: ConVarBounds, set: bool, value: float = ...) -> None:
    pass
def GetName(name: str, maxlength: int) -> None:
    pass
def GetDescription(buffer: str, maxlength: int) -> None:
    pass
def ReplicateToClient(client: int, value: str) -> bool:
    pass
def AddChangeHook(callback: Any) -> None:
    pass
def RemoveChangeHook(callback: Any) -> None:
    pass
def HookConVarChange(convar: Any, callback: Any) -> None:
    """Creates a hook for when a console variable's value is changed.

@param convar        Handle to the convar.
@param callback      An OnConVarChanged function pointer.
@error               Invalid or corrupt Handle or invalid callback function."""
    pass
def UnhookConVarChange(convar: Any, callback: Any) -> None:
    """Removes a hook for when a console variable's value is changed.

@param convar        Handle to the convar.
@param callback      An OnConVarChanged function pointer.
@error               Invalid or corrupt Handle, invalid callback function, or no active hook on convar."""
    pass
def GetConVarBool(convar: Any) -> bool:
    """Returns the boolean value of a console variable.

@param convar        Handle to the convar.
@return              The boolean value of the convar.
@error               Invalid or corrupt Handle."""
    pass
def SetConVarBool(convar: Any, value: bool, replicate: bool = ..., notify: bool = ...) -> None:
    """Sets the boolean value of a console variable.

Note: The replicate and notify params are only relevant for the original, Dark Messiah, and
Episode 1 engines. Newer engines automatically do these things when the convar value is changed.

@param convar        Handle to the convar.
@param value         New boolean value.
@param replicate     If set to true, the new convar value will be set on all clients.
                     This will only work if the convar has the FCVAR_REPLICATED flag
                     and actually exists on clients.
@param notify        If set to true, clients will be notified that the convar has changed.
                     This will only work if the convar has the FCVAR_NOTIFY flag.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarInt(convar: Any) -> int:
    """Returns the integer value of a console variable.

@param convar        Handle to the convar.
@return              The integer value of the convar.
@error               Invalid or corrupt Handle."""
    pass
def SetConVarInt(convar: Any, value: int, replicate: bool = ..., notify: bool = ...) -> None:
    """Sets the integer value of a console variable.

Note: The replicate and notify params are only relevant for the original, Dark Messiah, and
Episode 1 engines. Newer engines automatically do these things when the convar value is changed.

@param convar        Handle to the convar.
@param value         New integer value.
@param replicate     If set to true, the new convar value will be set on all clients.
                     This will only work if the convar has the FCVAR_REPLICATED flag
                     and actually exists on clients.
@param notify        If set to true, clients will be notified that the convar has changed.
                     This will only work if the convar has the FCVAR_NOTIFY flag.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarFloat(convar: Any) -> float:
    """Returns the floating point value of a console variable.

@param convar        Handle to the convar.
@return              The floating point value of the convar.
@error               Invalid or corrupt Handle."""
    pass
def SetConVarFloat(convar: Any, value: float, replicate: bool = ..., notify: bool = ...) -> None:
    """Sets the floating point value of a console variable.

Note: The replicate and notify params are only relevant for the original, Dark Messiah, and
Episode 1 engines. Newer engines automatically do these things when the convar value is changed.

@param convar        Handle to the convar.
@param value         New floating point value.
@param replicate     If set to true, the new convar value will be set on all clients.
                     This will only work if the convar has the FCVAR_REPLICATED flag
                     and actually exists on clients.
@param notify        If set to true, clients will be notified that the convar has changed.
                     This will only work if the convar has the FCVAR_NOTIFY flag.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarString(convar: Any, value: str, maxlength: int) -> None:
    """Retrieves the string value of a console variable.

@param convar        Handle to the convar.
@param value         Buffer to store the value of the convar.
@param maxlength     Maximum length of string buffer.
@error               Invalid or corrupt Handle.     """
    pass
def SetConVarString(convar: Any, value: str, replicate: bool = ..., notify: bool = ...) -> None:
    """Sets the string value of a console variable.

Note: The replicate and notify params are only relevant for the original, Dark Messiah, and
Episode 1 engines. Newer engines automatically do these things when the convar value is changed.

@param convar        Handle to the convar.
@param value         New string value.
@param replicate     If set to true, the new convar value will be set on all clients.
                     This will only work if the convar has the FCVAR_REPLICATED flag
                     and actually exists on clients.
@param notify        If set to true, clients will be notified that the convar has changed.
                     This will only work if the convar has the FCVAR_NOTIFY flag.
@error               Invalid or corrupt Handle."""
    pass
def ResetConVar(convar: Any, replicate: bool = ..., notify: bool = ...) -> None:
    """Resets the console variable to its default value.

Note: The replicate and notify params are only relevant for the original, Dark Messiah, and
Episode 1 engines. Newer engines automatically do these things when the convar value is changed.

@param convar        Handle to the convar.
@param replicate     If set to true, the new convar value will be set on all clients.
                     This will only work if the convar has the FCVAR_REPLICATED flag
                     and actually exists on clients.
@param notify        If set to true, clients will be notified that the convar has changed.
                     This will only work if the convar has the FCVAR_NOTIFY flag.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarDefault(convar: Any, value: str, maxlength: int) -> int:
    """Retrieves the default string value of a console variable.

@param convar        Handle to the convar.
@param value         Buffer to store the default value of the convar.
@param maxlength     Maximum length of string buffer.
@return              Number of bytes written to the buffer (UTF-8 safe).
@error               Invalid or corrupt Handle."""
    pass
def GetConVarFlags(convar: Any) -> int:
    """Returns the bitstring of flags on a console variable.

@param convar        Handle to the convar.
@return              A bitstring containing the FCVAR_* flags that are enabled.
@error               Invalid or corrupt Handle."""
    pass
def SetConVarFlags(convar: Any, flags: int) -> None:
    """Sets the bitstring of flags on a console variable.

@param convar        Handle to the convar.
@param flags         A bitstring containing the FCVAR_* flags to enable.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarBounds(convar: Any, type: ConVarBounds, value: float) -> bool:
    """Retrieves the specified bound of a console variable.

@param convar        Handle to the convar.
@param type          Type of bound to retrieve, ConVarBound_Lower or ConVarBound_Upper.
@param value         By-reference cell to store the specified floating point bound value.
@return              True if the convar has the specified bound set, false otherwise.
@error               Invalid or corrupt Handle."""
    pass
def SetConVarBounds(convar: Any, type: ConVarBounds, set: bool, value: float = ...) -> None:
    """Sets the specified bound of a console variable.

@param convar        Handle to the convar.
@param type          Type of bound to set, ConVarBound_Lower or ConVarBound_Upper
@param set           If set to true, convar will use specified bound. If false, bound will be removed.
@param value         Floating point value to use as the specified bound.
@error               Invalid or corrupt Handle."""
    pass
def GetConVarName(convar: Any, name: str, maxlength: int) -> None:
    """Retrieves the name of a console variable.

@param convar        Handle to the convar.
@param name          Buffer to store the name of the convar.
@param maxlength     Maximum length of string buffer.
@error               Invalid or corrupt Handle.     """
    pass
def SendConVarValue(client: int, convar: Any, value: str) -> bool:
    """Replicates a convar value to a specific client. This does not change the actual convar value.

@param client        Client index
@param convar        ConVar handle
@param value         String value to send
@return              True on success, false on failure
@error               Invalid client index, client not in game, or client is fake"""
    pass
def QueryClientConVar(client: int, cvarName: str, callback: ConVarQueryFinished, value: Any = ...) -> QueryCookie:
    """Starts a query to retrieve the value of a client's console variable.

@param client        Player index.
@param cvarName      Name of client convar to query.
@param callback      A function to use as a callback when the query has finished.
@param value         Optional value to pass to the callback function.
@return              A cookie that uniquely identifies the query. 
                     Returns QUERYCOOKIE_FAILED on failure, such as when used on a bot."""
    pass
def IsValidConVarChar(c: int) -> bool:
    """Returns true if the supplied character is valid in a ConVar name.

@param c             Character to validate.
@return              True is valid for ConVars, false otherwise"""
    pass
ConVarChanged: Any = ...
hasMax: bool = ...