from typing import Any, list, Callable, Union
from .core import *
from .handles import *


class EventHookMode:
    """Event hook modes determining how hooking should be handled"""
    EventHookMode_Post: int = ...
    EventHookMode_PostNoCopy: int = ...
    EventHookMode_Pre: int = ...


"""Hook function types for events."""
EventHook = Union[
    Callable[[Any, str, bool], Any],
    Callable[[Any, str, bool], None]
]
def Fire(dontBroadcast: bool = ...) -> None:
    pass
def FireToClient(client: int) -> None:
    pass
def Cancel() -> None:
    pass
def GetBool(key: str, defValue: bool = ...) -> bool:
    pass
def SetBool(key: str, value: bool) -> None:
    pass
def GetInt(key: str, defValue: int = ...) -> int:
    pass
def SetInt(key: str, value: int) -> None:
    pass
def GetFloat(key: str, defValue: float = ...) -> float:
    pass
def SetFloat(key: str, value: float) -> None:
    pass
def GetString(key: str, value: str, maxlength: int, defvalue: str = ...) -> None:
    pass
def SetString(key: str, value: str) -> None:
    pass
def GetName(name: str, maxlength: int) -> None:
    pass
def set(dontBroadcast: bool) -> Any:
    pass
def get() -> Any:
    pass
def HookEvent(name: str, callback: EventHook, mode: EventHookMode = ...) -> None:
    """Creates a hook for when a game event is fired.

@param name          Name of event.
@param callback      An EventHook function pointer.
@param mode          Optional EventHookMode determining the type of hook.
@error               Invalid event name or invalid callback function."""
    pass
def HookEventEx(name: str, callback: EventHook, mode: EventHookMode = ...) -> bool:
    """Creates a hook for when a game event is fired.

@param name          Name of event.
@param callback      An EventHook function pointer.
@param mode          Optional EventHookMode determining the type of hook.
@return              True if event exists and was hooked successfully, false otherwise.
@error               Invalid callback function."""
    pass
def UnhookEvent(name: str, callback: EventHook, mode: EventHookMode = ...) -> None:
    """Removes a hook for when a game event is fired.

@param name          Name of event.
@param callback      An EventHook function pointer.
@param mode          Optional EventHookMode determining the type of hook.
@error               Invalid callback function or no active hook for specified event."""
    pass
def CreateEvent(name: str, force: bool = ...) -> Any:
    """Creates a game event to be fired later.

The Handle should not be closed via CloseHandle().  It must be closed via 
event.Fire() or event.Cancel().

@param name          Name of event.
@param force         If set to true, this forces the event to be created even if it's not being hooked.
                     Note that this will not force it if the event doesn't exist at all.
@return              Handle to event. INVALID_HANDLE is returned if the event doesn't exist or isn't 
                     being hooked (unless force is true)."""
    pass
def FireEvent(event: Any, dontBroadcast: bool = ...) -> None:
    """Fires a game event.

This function closes the event Handle after completing.

@param event         Handle to the event.
@param dontBroadcast Optional boolean that determines if event should be broadcast to clients.
@error               Invalid or corrupt Handle."""
    pass
def CancelCreatedEvent(event: Any) -> None:
    """Cancels a previously created game event that has not been fired.

@param event         Handled to the event.
@error               Invalid or corrupt Handle."""
    pass
def GetEventBool(event: Any, key: str, defValue: bool = ...) -> bool:
    """Returns the boolean value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param defValue      Optional default value to use if the key is not found.
@return              The boolean value of the specified event key.
@error               Invalid or corrupt Handle."""
    pass
def SetEventBool(event: Any, key: str, value: bool) -> None:
    """Sets the boolean value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param value         New boolean value.
@error               Invalid or corrupt Handle."""
    pass
def GetEventInt(event: Any, key: str, defValue: int = ...) -> int:
    """Returns the integer value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param defValue      Optional default value to use if the key is not found.
@return              The integer value of the specified event key.
@error               Invalid or corrupt Handle."""
    pass
def SetEventInt(event: Any, key: str, value: int) -> None:
    """Sets the integer value of a game event's key.

Integer value refers to anything that can be reduced to an integer.
The various size specifiers, such as "byte" and "short" are still 
integers, and only refer to how much data will actually be sent 
over the network (if applicable).

@param event         Handle to the event.
@param key           Name of event key.
@param value         New integer value.
@error               Invalid or corrupt Handle."""
    pass
def GetEventFloat(event: Any, key: str, defValue: float = ...) -> float:
    """Returns the floating point value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param defValue      Optional default value to use if the key is not found.
@return              The floating point value of the specified event key.
@error               Invalid or corrupt Handle."""
    pass
def SetEventFloat(event: Any, key: str, value: float) -> None:
    """Sets the floating point value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param value         New floating point value.
@error               Invalid or corrupt Handle."""
    pass
def GetEventString(event: Any, key: str, value: str, maxlength: int, defvalue: str = ...) -> None:
    """Retrieves the string value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param value         Buffer to store the value of the specified event key.
@param maxlength     Maximum length of string buffer.
@param defValue      Optional default value to use if the key is not found.
@error               Invalid or corrupt Handle."""
    pass
def SetEventString(event: Any, key: str, value: str) -> None:
    """Sets the string value of a game event's key.

@param event         Handle to the event.
@param key           Name of event key.
@param value         New string value.
@error               Invalid or corrupt Handle."""
    pass
def GetEventName(event: Any, name: str, maxlength: int) -> None:
    """Retrieves the name of a game event.

@param event         Handle to the event.
@param name          Buffer to store the name of the event.
@param maxlength     Maximum length of string buffer.
@error               Invalid or corrupt Handle.     """
    pass
def SetEventBroadcast(event: Any, dontBroadcast: bool) -> None:
    """Sets whether an event's broadcasting will be disabled or not.

This has no effect on events Handles that are not from HookEvent
or HookEventEx callbacks.

@param event         Handle to an event from an event hook.
@param dontBroadcast True to disable broadcasting, false otherwise.
@error               Invalid Handle."""
    pass