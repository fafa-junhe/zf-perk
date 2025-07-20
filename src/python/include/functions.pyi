from typing import Any, list, Callable, Union
from .handles import *


class ExecType:
    """Defines how a forward iterates through plugin functions."""
    ET_Event: int = ...
    ET_Hook: int = ...
    ET_Ignore: int = ...
    ET_Single: int = ...


class ParamType:
    """Describes the various ways to pass parameters to functions or forwards."""
    Param_Any: int = ...
    Param_Array: int = ...
    Param_Cell: int = ...
    Param_CellByRef: int = ...
    Param_Float: int = ...
    Param_FloatByRef: int = ...
    Param_String: int = ...
    Param_VarArgs: int = ...


"""Accepts one of the following function signatures:
- Defines a native function.

It is not necessary to validate the parameter count

@param plugin        Handle of the calling plugin.
@param numParams     Number of parameters passed to the native.
@return              Value for the native call to return.
- Defines a native function.

It is not necessary to validate the parameter count

@param plugin         Handle of the calling plugin.
@param numParams      Number of parameters passed to the native.
@return               Value for the native call to return.
- Defines a native function.

It is not necessary to validate the parameter count

@param plugin         Handle of the calling plugin.
@param numParams      Number of parameters passed to the native."""
NativeCall = Union[
    Callable[[Any, int], int],
    Callable[[Any, int], Any],
    Callable[[Any, int], None]
]
"""Defines a RequestFrame Callback.

@param data          Data passed to the RequestFrame native."""
RequestFrameCallback = Union[
    Callable[[], None],
    Callable[[Any], None]
]
def GlobalForward(name: str, type: ExecType, _0: ParamType, *args: Any) -> Any:
    pass
def get() -> Any:
    pass
def PrivateForward(type: ExecType, _0: ParamType, *args: Any) -> Any:
    pass
def AddFunction(plugin: Any, func: Callable) -> bool:
    pass
def RemoveFunction(plugin: Any, func: Callable) -> bool:
    pass
def RemoveAllFunctions(plugin: Any) -> int:
    pass
def GetFunctionByName(plugin: Any, name: str) -> Callable:
    """Gets a function id from a function name.

@param plugin        Handle of the plugin that contains the function.
                     Pass INVALID_HANDLE to search in the calling plugin.
@param name          Name of the function.
@return              Function id or INVALID_FUNCTION if not found.
@error               Invalid or corrupt plugin handle."""
    pass
def CreateGlobalForward(name: str, type: ExecType, _0: ParamType, *args: Any) -> Any:
    """Creates a global forward.

@note The name used to create the forward is used as its public function in all target plugins.
@note This is ideal for global, static forwards that are never changed.
@note Global forwards cannot be cloned.
@note Use CloseHandle() to destroy these.

@param name          Name of public function to use in forward.
@param type          Execution type to be used.
@param ...           Variable number of parameter types (up to 32).
@return              Handle to new global forward.
@error               More than 32 parameter types passed."""
    pass
def CreateForward(type: ExecType, _0: ParamType, *args: Any) -> Any:
    """Creates a private forward.

@note No functions are automatically added. Use AddToForward() to do this.
@note Private forwards can be cloned.
@note Use CloseHandle() to destroy these.

@param type          Execution type to be used.
@param ...           Variable number of parameter types (up to 32).
@return              Handle to new private forward.
@error               More than 32 parameter types passed."""
    pass
def GetForwardFunctionCount(fwd: Any) -> int:
    """Returns the number of functions in a global or private forward's call list.

@param fwd           Handle to global or private forward.
@return              Number of functions in forward.
@error               Invalid or corrupt forward handle."""
    pass
def AddToForward(fwd: Any, plugin: Any, func: Callable) -> bool:
    """Adds a function to a private forward's call list.

@note Cannot be used during an incomplete call.

@param fwd           Handle to private forward.
@param plugin        Handle of the plugin that contains the function.
                     Pass INVALID_HANDLE to specify the calling plugin.
@param func          Function to add to forward.
@return              True on success, false otherwise.
@error               Invalid or corrupt private forward handle, invalid or corrupt plugin handle, or invalid function."""
    pass
def RemoveFromForward(fwd: Any, plugin: Any, func: Callable) -> bool:
    """Removes a function from a private forward's call list.

@note Only removes one instance.
@note Functions will be removed automatically if their parent plugin is unloaded.

@param fwd           Handle to private forward.
@param plugin        Handle of the plugin that contains the function.
                     Pass INVALID_HANDLE to specify the calling plugin.
@param func          Function to remove from forward.
@return              True on success, false otherwise.
@error               Invalid or corrupt private forward handle, invalid or corrupt plugin handle, or invalid function."""
    pass
def RemoveAllFromForward(fwd: Any, plugin: Any) -> int:
    """Removes all instances of a plugin from a private forward's call list.

@note Functions will be removed automatically if their parent plugin is unloaded.

@param fwd           Handle to private forward.
@param plugin        Handle of the plugin to remove instances of.
                     Pass INVALID_HANDLE to specify the calling plugin.
@return              Number of functions removed from forward.
@error               Invalid or corrupt private forward handle or invalid or corrupt plugin handle."""
    pass
def Call_StartForward(fwd: Any) -> None:
    """Starts a call to functions in a forward's call list.

@note Cannot be used during an incomplete call.

@param fwd           Handle to global or private forward.
@error               Invalid or corrupt forward handle or called before another call has completed."""
    pass
def Call_StartFunction(plugin: Any, func: Callable) -> None:
    """Starts a call to a function.

@note Cannot be used during an incomplete call.

@param plugin        Handle of the plugin that contains the function.
                     Pass INVALID_HANDLE to specify the calling plugin.
@param func          Function to call.
@error               Invalid or corrupt plugin handle, invalid function, or called before another call has completed."""
    pass
def Call_PushCell(value: Any) -> None:
    """Pushes a cell onto the current call.

@note Cannot be used before a call has been started.

@param value         Cell value to push.
@error               Called before a call has been started."""
    pass
def Call_PushCellRef(value: Any) -> None:
    """Pushes a cell by reference onto the current call.

@note Cannot be used before a call has been started.

@param value         Cell reference to push.
@error               Called before a call has been started."""
    pass
def Call_PushFloat(value: float) -> None:
    """Pushes a float onto the current call.

@note Cannot be used before a call has been started.

@param value         Floating point value to push.
@error               Called before a call has been started."""
    pass
def Call_PushFloatRef(value: float) -> None:
    """Pushes a float by reference onto the current call.

@note Cannot be used before a call has been started.

@param value         Floating point reference to push.
@error               Called before a call has been started."""
    pass
def Call_PushArray(value: list[Any], size: int) -> None:
    """Pushes an array onto the current call.

@note Changes to array are not copied back to caller. Use PushArrayEx() to do this.
@note Cannot be used before a call has been started.

@param value         Array to push.
@param size          Size of array.
@error               Called before a call has been started."""
    pass
def Call_PushArrayEx(value: list[Any], size: int, cpflags: int) -> None:
    """Pushes an array onto the current call.

@note Cannot be used before a call has been started.

@param value         Array to push.
@param size          Size of array.
@param cpflags       Whether or not changes should be copied back to the input array.
                     See SM_PARAM_* constants for details.
@error               Called before a call has been started."""
    pass
def Call_PushNullVector() -> None:
    """Pushes the NULL_VECTOR onto the current call.
@see IsNullVector

@note Cannot be used before a call has been started.

@error               Called before a call has been started."""
    pass
def Call_PushString(value: str) -> None:
    """Pushes a string onto the current call.

@note Changes to string are not copied back to caller. Use PushStringEx() to do this.
@note Cannot be used before a call has been started.

@param value         String to push.
@error               Called before a call has been started."""
    pass
def Call_PushStringEx(value: str, length: int, szflags: int, cpflags: int) -> None:
    """Pushes a string onto the current call.

@note Cannot be used before a call has been started.

@param value         String to push.
@param length        Length of string buffer.
@param szflags       Flags determining how string should be handled.
                     See SM_PARAM_STRING_* constants for details.
                     The default (0) is to push ASCII.
@param cpflags       Whether or not changes should be copied back to the input array.
                     See SM_PARAM_* constants for details.
@error               Called before a call has been started."""
    pass
def Call_PushNullString() -> None:
    """Pushes the NULL_STRING onto the current call.
@see IsNullString

@note Cannot be used before a call has been started.

@error               Called before a call has been started."""
    pass
def Call_Finish(result: Any = ...) -> int:
    """Completes a call to a function or forward's call list.

@note Cannot be used before a call has been started.

@param result        Return value of function or forward's call list.
@return              SP_ERROR_NONE on success, any other integer on failure.
@error               Called before a call has been started."""
    pass
def Call_Cancel() -> None:
    """Cancels a call to a function or forward's call list.

@note Cannot be used before a call has been started.

@error               Called before a call has been started."""
    pass
def CreateNative(name: str, func: NativeCall) -> None:
    """Creates a dynamic native.  This should only be called in AskPluginLoad(), or
else you risk not having your native shared with other plugins.

@param name          Name of the dynamic native; must be unique among
                     all other registered dynamic natives.
@param func          Function to use as the dynamic native."""
    pass
def ThrowNativeError(error: int, fmt: str, _0: Any, *args: Any) -> int:
    """Throws an error in the calling plugin of a native, instead of your own plugin.

@param error         Error code to use.
@param fmt           Error message format.
@param ...           Format arguments.
@noreturn
@error               Always!"""
    pass
def GetNativeStringLength(param: int, length: int) -> int:
    """Retrieves the string length from a native parameter string.  This is useful for
fetching the entire string using dynamic arrays.

@note If this function succeeds, Get/SetNativeString will also succeed.

@param param         Parameter number, starting from 1.
@param length        Stores the length of the string.
@return              SP_ERROR_NONE on success, any other integer on failure.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def GetNativeString(param: int, buffer: str, maxlength: int, bytes: int = ...) -> int:
    """Retrieves a string from a native parameter.

@note Output conditions are undefined on failure.

@param param         Parameter number, starting from 1.
@param buffer        Buffer to store the string in.
@param maxlength     Maximum length of the buffer.
@param bytes         Optionally store the number of bytes written.
@return              SP_ERROR_NONE on success, any other integer on failure.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def SetNativeString(param: int, source: str, maxlength: int, utf8: bool = ..., bytes: int = ...) -> int:
    """Sets a string in a native parameter.

@note Output conditions are undefined on failure.

@param param         Parameter number, starting from 1.
@param source        Source string to use.
@param maxlength     Maximum number of bytes to write.
@param utf8          If false, string will not be written
                     with UTF8 safety.
@param bytes         Optionally store the number of bytes written.
@return              SP_ERROR_NONE on success, any other integer on failure.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def GetNativeCell(param: int) -> Any:
    """Gets a cell from a native parameter.

@param param         Parameter number, starting from 1.
@return              Cell value at the parameter number.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def GetNativeFunction(param: int) -> Callable:
    """Gets a function pointer from a native parameter.

@param param             Parameter number, starting from 1.
@return                  Function pointer at the given parameter number.
@error                   Invalid parameter number, or calling from a non-native function."""
    pass
def GetNativeCellRef(param: int) -> Any:
    """Gets a cell from a native parameter, by reference.

@param param         Parameter number, starting from 1.
@return              Cell value at the parameter number.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def SetNativeCellRef(param: int, value: Any) -> None:
    """Sets a cell from a native parameter, by reference.

@param param         Parameter number, starting from 1.
@param value         Cell value at the parameter number to set by reference.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def GetNativeArray(param: int, local: list[Any], size: int) -> int:
    """Gets an array from a native parameter (always by reference).

@param param         Parameter number, starting from 1.
@param local         Local array to copy into.
@param size          Maximum size of local array.
@return              SP_ERROR_NONE on success, anything else on failure.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def SetNativeArray(param: int, local: list[Any], size: int) -> int:
    """Copies a local array into a native parameter array (always by reference).

@param param         Parameter number, starting from 1.
@param local         Local array to copy from.
@param size          Size of the local array to copy.
@return              SP_ERROR_NONE on success, anything else on failure.
@error               Invalid parameter number or calling from a non-native function."""
    pass
def IsNativeParamNullVector(param: int) -> bool:
    """Check if the native parameter is the NULL_VECTOR.

@param param         Parameter number, starting from 1.
@return              True if NULL_VECTOR, false otherwise."""
    pass
def IsNativeParamNullString(param: int) -> bool:
    """Check if the native parameter is the NULL_STRING.

@param param         Parameter number, starting from 1.
@return              True if NULL_STRING, false otherwise."""
    pass
def FormatNativeString(out_param: int, fmt_param: int, vararg_param: int, out_len: int, written: int = ..., out_string: str = ..., fmt_string: str = ...) -> int:
    """Formats a string using parameters from a native.

@note All parameter indexes start at 1.
@note If the input and output buffers overlap, the contents
      of the output buffer at the end is undefined.

@param out_param     Output parameter number to write to.  If 0, out_string is used.
@param fmt_param     Format parameter number.  If 0, fmt_string is used.
@param vararg_param  First variable parameter number.
@param out_len       Output string buffer maximum length (always required).
@param written       Optionally stores the number of bytes written.
@param out_string    Output string buffer to use if out_param is not used.
@param fmt_string    Format string to use if fmt_param is not used.
@return              SP_ERROR_NONE on success, anything else on failure."""
    pass
def RequestFrame(Function: RequestFrameCallback, data: Any = ...) -> None:
    """Creates a single use Next Frame hook.

@param Function      Function to call on the next frame.
@param data          Value to be passed on the invocation of the Function."""
    pass
fmt_string: str = ...  # "")
SP_PARAMFLAG_BYREF: Any = ...  # (1<<0)  /**< Internal use only. */
SM_PARAM_COPYBACK: Any = ...  # (1<<0)      /**< Copy an array/reference back after call */
SM_PARAM_STRING_UTF8: Any = ...  # (1<<0)      /**< String should be UTF-8 handled */
SM_PARAM_STRING_COPY: Any = ...  # (1<<1)      /**< String should be copied into the plugin */
SM_PARAM_STRING_BINARY: Any = ...  # (1<<2)      /**< Treat the string as a binary string */
SP_ERROR_NONE: Any = ...  # 0   /**< No error occurred */
SP_ERROR_FILE_FORMAT: Any = ...  # 1   /**< File format unrecognized */
SP_ERROR_DECOMPRESSOR: Any = ...  # 2   /**< A decompressor was not found */
SP_ERROR_HEAPLOW: Any = ...  # 3   /**< Not enough space left on the heap */
SP_ERROR_PARAM: Any = ...  # 4   /**< Invalid parameter or parameter type */
SP_ERROR_INVALID_ADDRESS: Any = ...  # 5   /**< A memory address was not valid */
SP_ERROR_NOT_FOUND: Any = ...  # 6   /**< The object in question was not found */
SP_ERROR_INDEX: Any = ...  # 7   /**< Invalid index parameter */
SP_ERROR_STACKLOW: Any = ...  # 8   /**< Not enough space left on the stack */
SP_ERROR_NOTDEBUGGING: Any = ...  # 9   /**< Debug mode was not on or debug section not found */
SP_ERROR_INVALID_INSTRUCTION: Any = ...  # 10  /**< Invalid instruction was encountered */
SP_ERROR_MEMACCESS: Any = ...  # 11  /**< Invalid memory access */
SP_ERROR_STACKMIN: Any = ...  # 12  /**< Stack went beyond its minimum value */
SP_ERROR_HEAPMIN: Any = ...  # 13  /**< Heap went beyond its minimum value */
SP_ERROR_DIVIDE_BY_ZERO: Any = ...  # 14  /**< Division by zero */
SP_ERROR_ARRAY_BOUNDS: Any = ...  # 15  /**< Array index is out of bounds */
SP_ERROR_INSTRUCTION_PARAM: Any = ...  # 16  /**< Instruction had an invalid parameter */
SP_ERROR_STACKLEAK: Any = ...  # 17  /**< A native leaked an item on the stack */
SP_ERROR_HEAPLEAK: Any = ...  # 18  /**< A native leaked an item on the heap */
SP_ERROR_ARRAY_TOO_BIG: Any = ...  # 19  /**< A dynamic array is too big */
SP_ERROR_TRACKER_BOUNDS: Any = ...  # 20  /**< Tracker stack is out of bounds */
SP_ERROR_INVALID_NATIVE: Any = ...  # 21  /**< Native was pending or invalid */
SP_ERROR_PARAMS_MAX: Any = ...  # 22  /**< Maximum number of parameters reached */
SP_ERROR_NATIVE: Any = ...  # 23  /**< Error originates from a native */
SP_ERROR_NOT_RUNNABLE: Any = ...  # 24  /**< Function or plugin is not runnable */
SP_ERROR_ABORTED: Any = ...  # 25  /**< Function call was aborted */