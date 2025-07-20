from typing import Any, list, Callable, Union


class RoundState:
    RoundState_BetweenRounds: int = ...
    RoundState_Bonus: int = ...
    RoundState_GameOver: int = ...
    RoundState_Init: int = ...
    RoundState_Pregame: int = ...
    RoundState_Preround: int = ...
    RoundState_Restart: int = ...
    RoundState_RoundRunning: int = ...
    RoundState_Stalemate: int = ...
    RoundState_StartGame: int = ...
    RoundState_TeamWin: int = ...


def GameRules_GetProp(prop: str, size: int = ..., element: int = ...) -> int:
    """Retrieves an integer value from a property of the gamerules entity.

@param prop          Property name.
@param size          Number of bytes to read (valid values are 1, 2, or 4).
                     This value is auto-detected, and the size parameter is
                     only used as a fallback in case detection fails.
@param element       Element # (starting from 0) if property is an array.
@return              Value at the given property offset.
@error               Prop type is not an integer, or lack of mod support."""
    pass
def GameRules_SetProp(prop: str, value: Any, size: int = ..., element: int = ..., changeState: bool = ...) -> None:
    """Sets an integer value for a property of the gamerules entity.

@param prop          Property name.
@param value         Value to set.
@param size          Number of bytes to write (valid values are 1, 2, or 4).
                     This value is auto-detected, and the size parameter is
                     only used as a fallback in case detection fails.
@param element       Element # (starting from 0) if property is an array.
@param changeState   This parameter is ignored.
@error               Prop type is not an integer, or lack of mod support."""
    pass
def GameRules_GetPropFloat(prop: str, element: int = ...) -> float:
    """Retrieves a float value from a property of the gamerules entity.

@param prop          Property name.
@param element       Element # (starting from 0) if property is an array.
@return              Value at the given property offset.
@error               Prop type is not a float, or lack of mod support."""
    pass
def GameRules_SetPropFloat(prop: str, value: float, element: int = ..., changeState: bool = ...) -> None:
    """Sets a float value for a property of the gamerules entity.

@param prop          Property name.
@param value         Value to set.
@param element       Element # (starting from 0) if property is an array.
@param changeState   This parameter is ignored.
@error               Prop type is not a float, or lack of mod support."""
    pass
def GameRules_GetPropEnt(prop: str, element: int = ...) -> int:
    """Retrieves a entity index from a property of the gamerules entity.

@param prop          Property name.
@param element       Element # (starting from 0) if property is an array.
@return              Entity index at the given property.
                     If there is no entity, or the entity is not valid,
                     then -1 is returned.
@error               Prop type is not an entity, or lack of mod support."""
    pass
def GameRules_SetPropEnt(prop: str, other: int, element: int = ..., changeState: bool = ...) -> None:
    """Sets an entity index for a property of the gamerules entity.

@param prop          Property name.
@param other         Entity index to set, or -1 to unset.
@param element       Element # (starting from 0) if property is an array.
@param changeState   This parameter is ignored.
@error               Prop type is not an entity, invalid entity, or lack of mod support."""
    pass
def GameRules_GetPropVector(prop: str, vec: list[float], element: int = ...) -> None:
    """Retrieves a vector of floats from the gamerules entity, given a named network property.

@param prop          Property name.
@param vec           Vector buffer to store data in.
@param element       Element # (starting from 0) if property is an array.
@error               Prop type is not a vector, or lack of mod support."""
    pass
def GameRules_SetPropVector(prop: str, vec: list[float], element: int = ..., changeState: bool = ...) -> None:
    """Sets a vector of floats in the gamerules entity, given a named network property.

@param prop          Property name.
@param vec           Vector to set.
@param element       Element # (starting from 0) if property is an array.
@param changeState   This parameter is ignored.
@error               Prop type is not a vector, or lack of mod support."""
    pass
def GameRules_GetPropString(prop: str, buffer: str, maxlen: int, element: int = ...) -> int:
    """Gets a gamerules property as a string.

@param prop          Property to use.
@param buffer        Destination string buffer.
@param maxlen        Maximum length of output string buffer.
@param element       Element # (starting from 0) if property is an array.
@return              Number of non-null bytes written.
@error               Prop type is not a string, or lack of mod support."""
    pass
def GameRules_SetPropString(prop: str, buffer: str, changeState: bool = ..., element: int = ...) -> int:
    """Sets a gamerules property as a string.

@param prop          Property to use.
@param buffer        String to set.
@param changeState   This parameter is ignored.
@param element       Element # (starting from 0) if property is an array.
@return              Number of non-null bytes written.
@error               Prop type is not a string, or lack of mod support."""
    pass
def GameRules_GetRoundState() -> RoundState:
    """Gets the current round state.

@return              Round state.
@error               Game doesn't support round state."""
    pass