from typing import Any, list, Callable, Union
from .halflife import *


def AddTempEntHook(te_name: str, hook: Any) -> None:
    """Hooks a temp entity.

@param te_name       TE name to hook.
@param hook          Function to use as a hook.
@error               Temp Entity name not available or invalid function hook."""
    pass
def RemoveTempEntHook(te_name: str, hook: Any) -> None:
    """Removes a temp entity hook.

@param te_name       TE name to unhook.
@param hook          Function used for the hook.
@error               Temp Entity name not available or invalid function hook."""
    pass
def TE_Start(te_name: str) -> None:
    """Starts a temp entity transmission.

@param te_name       TE name.
@error               Temp Entity name not available."""
    pass
def TE_IsValidProp(prop: str) -> bool:
    """Checks if a certain TE property exists.

@param prop          Property to use.
@return              True if the property exists, otherwise false."""
    pass
def TE_WriteNum(prop: str, value: int) -> None:
    """Sets an integer value in the current temp entity.

@param prop          Property to use.
@param value         Integer value to set.
@error               Property not found."""
    pass
def TE_ReadNum(prop: str) -> int:
    """Reads an integer value in the current temp entity.

@param prop          Property to use.
@return              Property value.
@error               Property not found."""
    pass
def TE_WriteEnt(prop: str, value: int) -> None:
    """Sets an entity value in the current temp entity.

@param prop          Property to use.
@param value         Entity reference or index value to set.
@error               Property not found."""
    pass
def TE_ReadEnt(prop: str) -> int:
    """Reads an entity value in the current temp entity.

@param prop          Property to use.
@return              Property value as backwards compatible entity reference.
@error               Property not found."""
    pass
def TE_WriteFloat(prop: str, value: float) -> None:
    """Sets a floating point number in the current temp entity.

@param prop          Property to use.
@param value         Floating point number to set.
@error               Property not found."""
    pass
def TE_ReadFloat(prop: str) -> float:
    """Reads a floating point number in the current temp entity.

@param prop          Property to use.
@return              Property value.
@error               Property not found."""
    pass
def TE_WriteVector(prop: str, vector: list[float]) -> None:
    """Sets a vector in the current temp entity.

@param prop          Property to use.
@param vector        Vector to set.
@error               Property not found."""
    pass
def TE_ReadVector(prop: str, vector: list[float]) -> None:
    """Reads a vector in the current temp entity.

@param prop          Property to use.
@param vector        Vector to read.
@error               Property not found."""
    pass
def TE_WriteAngles(prop: str, angles: list[float]) -> None:
    """Sets a QAngle in the current temp entity.

@param prop          Property to use.
@param angles        Angles to set.
@error               Property not found."""
    pass
def TE_WriteFloatArray(prop: str, array: list[float], arraySize: int) -> None:
    """Sets an array of floats in the current temp entity.

@param prop          Property to use.
@param array         Array of values to copy.
@param arraySize     Number of values to copy.
@error               Property not found."""
    pass
def TE_Send(clients: list[int], numClients: int, delay: float = ...) -> None:
    """Sends the current temp entity to one or more clients.

@param clients       Array containing player indexes to broadcast to.
@param numClients    Number of players in the array.
@param delay         Delay in seconds to send the TE.
@error               Invalid client index or client not in game."""
    pass
def TE_WriteEncodedEnt(prop: str, value: int) -> None:
    """Sets an encoded entity index in the current temp entity.
(This is usually used for m_nStartEntity and m_nEndEntity).

@param prop          Property to use.
@param value         Value to set.
@error               Property not found."""
    pass
def TE_SendToAll(delay: float = ...) -> None:
    """Broadcasts the current temp entity to all clients.
@note See TE_Start().

@param delay         Delay in seconds to send the TE."""
    pass
def TE_SendToClient(client: int, delay: float = ...) -> None:
    """Sends the current TE to only a client.
@note See TE_Start().

@param client        Client to send to.
@param delay         Delay in seconds to send the TE.
@error               Invalid client index or client not in game."""
    pass
def TE_SendToAllInRange(origin: list[float], rangeType: ClientRangeType, delay: float = ...) -> None:
    """Sends the current TE to all clients that are in
visible or audible range of the origin.
@note See TE_Start().
@note See GetClientsInRange()

@param origin        Coordinates from which to test range.
@param rangeType     Range type to use for filtering clients.
@param delay         Delay in seconds to send the TE."""
    pass
TEHook: Any = ...
encvalue: int = ...
total: int = ...