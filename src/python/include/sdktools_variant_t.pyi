from typing import Any, list, Callable, Union


def SetVariantBool(val: bool) -> None:
    """Sets a bool value in the global variant object.

@param val           Input value."""
    pass
def SetVariantString(str: str) -> None:
    """Sets a string in the global variant object.

@param str           Input string."""
    pass
def SetVariantInt(val: int) -> None:
    """Sets an integer value in the global variant object.

@param val           Input value."""
    pass
def SetVariantFloat(val: float) -> None:
    """Sets a floating point value in the global variant object.

@param val           Input value."""
    pass
def SetVariantVector3D(vec: list[float]) -> None:
    """Sets a 3D vector in the global variant object.

@param vec           Input vector."""
    pass
def SetVariantPosVector3D(vec: list[float]) -> None:
    """Sets a 3D position vector in the global variant object.

@param vec           Input position vector."""
    pass
def SetVariantColor(color: list[int]) -> None:
    """Sets a color in the global variant object.

@param color         Input color."""
    pass
def SetVariantEntity(entity: int) -> None:
    """Sets an entity in the global variant object.

@param entity        Entity index.
@error               Invalid entity index."""
    pass