from typing import Any, list, Callable, Union
from .handles import *


def FormatUserLogText(client: int, buffer: str, maxlength: int) -> None:
    pass
def FindPluginByFile(filename: str) -> Any:
    """Returns plugin handle from plugin filename.

@param filename      Filename of the plugin to search for.
@return              Handle to plugin if found, INVALID_HANDLE otherwise."""
    pass
def SearchForClients(pattern: str, clients: list[int], maxClients: int) -> int:
    pass
def FindTarget(client: int, target: str, nobots: bool = ..., immunity: bool = ...) -> int:
    """Wraps ProcessTargetString() and handles producing error messages for
bad targets.

Note that you should use LoadTranslations("common.phrases") in OnPluginStart(). 
"common.phrases" contains all of the translatable phrases that FindTarget() will
reply with in the event a target is not found (error).

@param client        Client who issued command
@param target        Client's target argument
@param nobots        Optional. Set to true if bots should NOT be targetted
@param immunity      Optional. Set to false to ignore target immunity.
@return              Index of target client, or -1 on error."""
    pass
def LoadMaps(array: Any, fileTime: int = ..., fileCvar: Any = ...) -> int:
    pass
iter: Any = ...
total: int = ...
input: int = ...
client: int = ...
flags: int = ...
fileFound: bool = ...
mapCycleFile: Any = ...
newTime: int = ...
file: Any = ...