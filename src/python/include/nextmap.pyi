from typing import Any, list, Callable, Union


def SetNextMap(map: str) -> bool:
    """Sets SourceMod's internal nextmap.
Equivalent to changing sm_nextmap but with an added validity check.

@param map           Next map to set.
@return              True if the nextmap was set, false if map was invalid."""
    pass
def GetNextMap(map: str, maxlen: int) -> bool:
    """Returns SourceMod's internal nextmap.

@param map           Buffer to store the nextmap name.
@param maxlen        Maximum length of the map buffer.
@return              True if a Map was found and copied, false if no nextmap is set (map will be unchanged)."""
    pass
def ForceChangeLevel(map: str, reason: str) -> None:
    """Changes the current map and records the reason for the change with maphistory

@param map           Map to change to.
@param reason        Reason for change."""
    pass
def GetMapHistorySize() -> int:
    """Gets the current number of maps in the map history

@return              Number of maps."""
    pass
def GetMapHistory(item: int, map: str, mapLen: int, reason: str, reasonLen: int, startTime: int) -> None:
    """Retrieves a map from the map history list.

@param item          Item number. Must be 0 or greater and less than GetMapHistorySize().
@param map           Buffer to store the map name.
@param mapLen        Length of map buffer.
@param reason        Buffer to store the change reason.
@param reasonLen     Length of the reason buffer.
@param startTime     Time the map started.
@error               Invalid item number."""
    pass