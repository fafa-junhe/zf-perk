from typing import Any, list, Callable, Union
from .core import *
from .datapack import *
from .handles import *


"""Any of the following prototypes will work for a timed function.

Accepts one of the following function signatures:
- Called when the timer interval has elapsed.

@param timer         Handle to the timer object.
@param data          Handle or value passed to CreateTimer() when timer was created.
@return              Plugin_Stop to stop a repeating timer, any other value for
                     default behavior.
                     Ignored for non-repeating timers (use void return type).
- Called when the timer interval has elapsed.
For repeating timers, use the callback with return value.

@param timer         Handle to the timer object.
@param data          Handle or value passed to CreateTimer() when timer was created.
- Called when the timer interval has elapsed.

@param timer         Handle to the timer object.
@return              Plugin_Stop to stop a repeating timer, any other value for
                     default behavior.
                     Ignored for non-repeating timers (use void return type).
- Called when the timer interval has elapsed.
For repeating timers, use the callback with return value.

@param timer         Handle to the timer object."""
Timer = Union[
    Callable[[Any, Any], Any],
    Callable[[Any, Any], None],
    Callable[[Any], Any],
    Callable[[Any], None]
]
def CreateTimer(interval: float, func: Any, data: Any = ..., flags: int = ...) -> Any:
    """Creates a basic timer.  Calling CloseHandle() on a timer will end the timer.

@param interval      Interval from the current game time to execute the given function.
@param func          Function to execute once the given interval has elapsed.
@param data          Handle or value to pass through to the timer callback function.
@param flags         Flags to set (such as repeatability or auto-Handle closing).
@return              Handle to the timer object.  You do not need to call CloseHandle().
                     If the timer could not be created, INVALID_HANDLE will be returned."""
    pass
def KillTimer(timer: Any, autoClose: bool = ...) -> None:
    """Kills a timer.  Use this instead of CloseHandle() if you need more options.

@param timer         Timer Handle to kill.
@param autoClose     If autoClose is true, the data that was passed to CreateTimer() will
                     be closed as a handle if TIMER_DATA_HNDL_CLOSE was not specified.
@error               Invalid timer handle."""
    pass
def TriggerTimer(timer: Any, reset: bool = ...) -> None:
    """Manually triggers a timer so its function will be called.

@param timer         Timer Handle to trigger.
@param reset         If reset is true, the elapsed time counter is reset
                     so the full interval must pass again.
@error               Invalid timer handle."""
    pass
def GetTickedTime() -> float:
    """Returns the simulated game time.

This time is internally maintained by SourceMod and is based on the game
tick count and tick rate.  Unlike GetGameTime(), it will increment past
map changes and while no players are connected.  Unlike GetEngineTime(),
it will not increment based on the system clock (i.e. it is still bound
to the ticking process).

@return              Time based on the game tick count."""
    pass
def GetMapTimeLeft(timeleft: int) -> bool:
    """Returns an estimate of the time left before the map ends.  If the server
has not processed any frames yet (i.e. no players have joined the map yet),
then the time left returned will always be infinite.

@param timeleft      Variable to store the time, in seconds.  If the
                     value is less than 0, the time limit is infinite.
@return              True if the operation is supported, false otherwise."""
    pass
def GetMapTimeLimit(time: int) -> bool:
    """Retrieves the current map time limit.  If the server has not processed any
frames yet (i.e. no players have joined the map yet), then the time limit
returned will always be 0.

@param time          Set to the number of total seconds in the map time
                     limit, or 0 if there is no time limit set.
@return              True on success, false if operation is not supported."""
    pass
def ExtendMapTimeLimit(time: int) -> bool:
    """Extends the map time limit in a way that will notify all plugins.

@param time          Number of seconds to extend map time limit by.
                     The number can be negative to decrease the time limit.
                     If 0, the map will be set to have no time limit.
@return              True on success, false if operation is not supported."""
    pass
def GetTickInterval() -> float:
    """Returns the number of seconds in between game server ticks.

Note: A tick, in this context, is a frame.

@return              Number of seconds in between ticks."""
    pass
def OnMapTimeLeftChanged() -> None:
    """Notification that the map's time left has changed via a change in the time
limit or a change in the game rules (such as mp_restartgame).  This is useful
for plugins trying to create timers based on the time left in the map.

Calling ExtendMapTimeLimit() from here, without proper precaution, will
cause infinite recursion.

If the operation is not supported, this will never be called.

If the server has not yet processed any frames (i.e. no players have joined
the map yet), then this will be called once the server begins ticking, even
if there is no time limit set."""
    pass
def IsServerProcessing() -> bool:
    """Returns whether or not the server is processing frames or not.

The server does not process frames until at least one client joins the game.
If server hibernation is disabled, once the first player has joined, even if that player
leaves, the server's timers and entities will continue to work.

@return              True if the server is ticking, false otherwise."""
    pass
def CreateDataTimer(interval: float, func: Any, datapack: Any, flags: int = ...) -> Any:
    """Creates a timer associated with a new datapack, and returns the datapack.
@note The datapack is automatically freed when the timer ends.
@note The position of the datapack is not reset or changed for the timer function.

@param interval      Interval from the current game time to execute the given function.
@param func          Function to execute once the given interval has elapsed.
@param datapack      The newly created datapack is passed though this by-reference
                     parameter to the timer callback function.
@param flags         Timer flags.
@return              Handle to the timer object.  You do not need to call CloseHandle()."""
    pass
TIMER_REPEAT: Any = ...  # (1<<0)      /**< Timer will repeat until it returns Plugin_Stop */
TIMER_FLAG_NO_MAPCHANGE: Any = ...  # (1<<1)      /**< Timer will not carry over mapchanges */
TIMER_HNDL_CLOSE: Any = ...  # (1<<9)      /**< Deprecated define, replaced by below */
TIMER_DATA_HNDL_CLOSE: Any = ...  # (1<<9)      /**< Timer will automatically call CloseHandle() on its data when finished */