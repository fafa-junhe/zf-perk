from typing import Any, list, Callable, Union


def SetClientViewEntity(client: int, entity: int) -> None:
    """Sets a client's "viewing entity."

@param client        Client index.
@param entity        Entity index.
@error               Invalid client or entity, lack of mod support, or client not in 
                     game."""
    pass
def SetLightStyle(style: int, value: str) -> None:
    """Sets a light style.

@param style         Light style (from 0 to MAX_LIGHTSTYLES-1)
@param value         Light value string (see world.cpp/light.cpp in dlls)
@error               Light style index is out of range."""
    pass
def GetClientEyePosition(client: int, pos: list[float]) -> None:
    """Returns the client's eye position.

@param client        Player's index.
@param pos           Destination vector to store the client's eye position.
@error               Invalid client index, client not in game, or no mod support."""
    pass
MAX_LIGHTSTYLES: Any = ...  # 64