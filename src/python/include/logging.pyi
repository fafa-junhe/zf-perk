from typing import Any, list, Callable, Union
from .core import *
from .handles import *


def LogMessage(format: str, _0: Any, *args: Any) -> None:
    """Logs a plugin message to the SourceMod logs.  The log message will be
prefixed by the plugin's logtag (filename).

@param format        String format.
@param ...           Format arguments."""
    pass
def LogToFile(file: str, format: str, _0: Any, *args: Any) -> None:
    """Logs a message to any file.  The log message will be in the normal
SourceMod format, with the plugin logtag prepended.

@param file          File to write the log message in.
@param format        String format.
@param ...           Format arguments.
@error               File could not be opened/written."""
    pass
def LogToFileEx(file: str, format: str, _0: Any, *args: Any) -> None:
    """Same as LogToFile(), except no plugin logtag is prepended.

@param file          File to write the log message in.
@param format        String format.
@param ...           Format arguments.
@error               File could not be opened/written."""
    pass
def LogAction(client: int, target: int, message: str, _0: Any, *args: Any) -> None:
    """Logs an action from a command or event whereby interception and routing may
be important.  This is intended to be a logging version of ShowActivity().

@param client        Client performing the action, 0 for server, or -1 if not
                     applicable.
@param target        Client being targetted, or -1 if not applicable.
@param message       Message format.
@param ...           Message formatting parameters."""
    pass
def LogError(format: str, _0: Any, *args: Any) -> None:
    """Logs a plugin error message to the SourceMod logs.

@param format        String format.
@param ...           Format arguments."""
    pass
def OnLogAction(source: Any, ident: Identity, client: int, target: int, message: str) -> Any:
    """Called when an action is going to be logged.

@param source        Handle to the object logging the action, or INVALID_HANDLE
                     if Core is logging the action.
@param ident         Type of object logging the action (plugin, ext, or core).
@param client        Client the action is from; 0 for server, -1 if not applicable.
@param target        Client the action is targetting, or -1 if not applicable.
@param message       Message that is being logged.
@return              Plugin_Continue will perform the default logging behavior.
                     Plugin_Handled will stop Core from logging the message.
                     Plugin_Stop is the same as Handled, but prevents any other
                     plugins from handling the message."""
    pass
def AddGameLogHook(hook: Any) -> None:
    """Adds a game log hook.

@param hook          Hook function."""
    pass
def RemoveGameLogHook(hook: Any) -> None:
    """Removes a game log hook.

@param hook          Hook function."""
    pass
GameLogHook: Any = ...