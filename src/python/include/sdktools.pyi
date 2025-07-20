from typing import Any, list, Callable, Union
from .core import *
from .handles import *
from .sdktools_client import *
from .sdktools_engine import *
from .sdktools_entinput import *
from .sdktools_entoutput import *
from .sdktools_functions import *
from .sdktools_gamerules import *
from .sdktools_hooks import *
from .sdktools_sound import *
from .sdktools_stocks import *
from .sdktools_stringtables import *
from .sdktools_tempents import *
from .sdktools_tempents_stocks import *
from .sdktools_trace import *
from .sdktools_variant_t import *
from .sdktools_voice import *
from .sourcemod import *


class SDKCallType:
    SDKCall_Engine: int = ...
    SDKCall_Entity: int = ...
    SDKCall_EntityList: int = ...
    SDKCall_GameRules: int = ...
    SDKCall_Player: int = ...
    SDKCall_Raw: int = ...
    SDKCall_Server: int = ...
    SDKCall_Static: int = ...


class SDKFuncConfSource:
    SDKConf_Address: int = ...
    SDKConf_Signature: int = ...
    SDKConf_Virtual: int = ...


class SDKLibrary:
    SDKLibrary_Engine: int = ...
    SDKLibrary_Server: int = ...


class SDKPassMethod:
    SDKPass_ByRef: int = ...
    SDKPass_ByValue: int = ...
    SDKPass_Plain: int = ...
    SDKPass_Pointer: int = ...


class SDKType:
    SDKType_Bool: int = ...
    SDKType_CBaseEntity: int = ...
    SDKType_CBasePlayer: int = ...
    SDKType_Edict: int = ...
    SDKType_Float: int = ...
    SDKType_PlainOldData: int = ...
    SDKType_QAngle: int = ...
    SDKType_String: int = ...
    SDKType_Vector: int = ...


def StartPrepSDKCall(type: SDKCallType) -> None:
    """Starts the preparation of an SDK call.

@param type          Type of function call this will be."""
    pass
def PrepSDKCall_SetVirtual(vtblidx: int) -> None:
    """Sets the virtual index of the SDK call if it is virtual.

@param vtblidx       Virtual table index."""
    pass
def PrepSDKCall_SetSignature(lib: SDKLibrary, signature: str, bytes: int) -> bool:
    """Finds an address in a library and sets it as the address to use for the SDK call.

@param lib           Library to use.
@param signature     Binary data to search for in the library.  If it starts with '@',
                     the bytes parameter is ignored and the signature is interpreted
                     as a symbol lookup in the library.
@param bytes         Number of bytes in the binary search string.
@return              True on success, false if nothing was found."""
    pass
def PrepSDKCall_SetAddress(addr: Address) -> bool:
    """Uses the given function address for the SDK call.

@param addr          Address of function to use.
@return              True on success, false on failure."""
    pass
def PrepSDKCall_SetFromConf(gameconf: Any, source: SDKFuncConfSource, name: str) -> bool:
    """Finds an address or virtual function index in a GameConfig file and sets it as
the calling information for the SDK call.

@param gameconf      GameConfig Handle, or INVALID_HANDLE to use sdktools.games.txt.
@param source        Whether to look in Offsets or Signatures.
@param name          Name of the property to find.
@return              True on success, false if nothing was found.
@error               Invalid game config Handle."""
    pass
def PrepSDKCall_SetReturnInfo(type: SDKType, ___pass: SDKPassMethod, decflags: int = ..., encflags: int = ...) -> None:
    """Sets the return information of an SDK call.  Do not call this if there is no return data.
This must be called if there is a return value (i.e. it is not necessarily safe to ignore
the data).

@param type          Data type to convert to/from.
@param pass          How the data is passed in C++.
@param decflags      Flags on decoding from the plugin to C++.
@param encflags      Flags on encoding from C++ to the plugin."""
    pass
def PrepSDKCall_AddParameter(type: SDKType, ___pass: SDKPassMethod, decflags: int = ..., encflags: int = ...) -> None:
    """Adds a parameter to the calling convention.  This should be called in normal ascending order.

@param type          Data type to convert to/from.
@param pass          How the data is passed in C++.
@param decflags      Flags on decoding from the plugin to C++.
@param encflags      Flags on encoding from C++ to the plugin.
@error               Parameter limit for SDK calls reached."""
    pass
def EndPrepSDKCall() -> Any:
    """Finalizes an SDK call preparation and returns the resultant Handle.

@return              A new SDKCall Handle on success, or INVALID_HANDLE on failure."""
    pass
def SDKCall(call: Any, _0: Any, *args: Any) -> Any:
    """Calls an SDK function with the given parameters.

If the call type is Entity or Player, the index MUST ALWAYS be the FIRST parameter passed.
If the call type is GameRules, then nothing special needs to be passed.
If the return value is a Vector or QAngles, the SECOND parameter must be a Float[3].
If the return value is a string, the THIRD parameter must be a String buffer, and the
 FOURTH parameter must be the maximum length.
All parameters must be passed after the above is followed.  Failure to follow these
 rules will result in crashes or wildly unexpected behavior!

If the return value is a float or integer, the return value will be this value.
If the return value is a string, the value returned by the function will be the number of bytes written, or -1 for NULL.
If the return value is a CBaseEntity, CBasePlayer, or edict, the return value will
 always be the entity index, or -1 for NULL.

@param call          SDKCall Handle.
@param ...           Call Parameters.
@return              Simple return value, if any.
@error               Invalid Handle or internal decoding error."""
    pass
def GetPlayerResourceEntity() -> int:
    """Returns the entity index of the player resource/manager entity.

@return              Index of resource entity or -1 if not found."""
    pass
VDECODE_FLAG_ALLOWNULL: Any = ...  # (1<<0)    /**< Allow NULL for pointers */
VDECODE_FLAG_ALLOWNOTINGAME: Any = ...  # (1<<1)    /**< Allow players not in game */
VDECODE_FLAG_ALLOWWORLD: Any = ...  # (1<<2)    /**< Allow World entity */
VDECODE_FLAG_BYREF: Any = ...  # (1<<3)    /**< Floats/ints by reference */
VENCODE_FLAG_COPYBACK: Any = ...  # (1<<0)    /**< Copy back data once done */
__ext_sdktools: Extension = ...