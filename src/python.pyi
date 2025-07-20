from typing import Any, list, Callable, Union
from .handles import *


def ReadInt(field: str, index: int = ...) -> int:
    pass
def ReadInt64(field: str, value: list[int], index: int = ...) -> None:
    pass
def ReadFloat(field: str, index: int = ...) -> float:
    pass
def ReadBool(field: str, index: int = ...) -> bool:
    pass
def ReadString(field: str, buffer: str, maxlength: int, index: int = ...) -> None:
    pass
def ReadColor(field: str, buffer: list[int], index: int = ...) -> None:
    pass
def ReadAngle(field: str, buffer: list[float], index: int = ...) -> None:
    pass
def ReadVector(field: str, buffer: list[float], index: int = ...) -> None:
    pass
def ReadVector2D(field: str, buffer: list[float], index: int = ...) -> None:
    pass
def GetRepeatedFieldCount(field: str) -> int:
    pass
def HasField(field: str) -> bool:
    pass
def SetInt(field: str, value: int, index: int = ...) -> None:
    pass
def SetInt64(field: str, value: list[int], index: int = ...) -> None:
    pass
def SetFloat(field: str, value: float, index: int = ...) -> None:
    pass
def SetBool(field: str, value: bool, index: int = ...) -> None:
    pass
def SetString(field: str, value: str, index: int = ...) -> None:
    pass
def SetColor(field: str, color: list[int], index: int = ...) -> None:
    pass
def SetAngle(field: str, angle: list[float], index: int = ...) -> None:
    pass
def SetVector(field: str, vec: list[float], index: int = ...) -> None:
    pass
def SetVector2D(field: str, vec: list[float], index: int = ...) -> None:
    pass
def AddInt(field: str, value: int) -> None:
    pass
def AddInt64(field: str, value: list[int]) -> None:
    pass
def AddFloat(field: str, value: float) -> None:
    pass
def AddBool(field: str, value: bool) -> None:
    pass
def AddString(field: str, value: str) -> None:
    pass
def AddColor(field: str, color: list[int]) -> None:
    pass
def AddAngle(field: str, angle: list[float]) -> None:
    pass
def AddVector(field: str, vec: list[float]) -> None:
    pass
def AddVector2D(field: str, vec: list[float]) -> None:
    pass
def RemoveRepeatedFieldValue(field: str, index: int) -> None:
    pass
def ReadMessage(field: str) -> Any:
    pass
def ReadRepeatedMessage(field: str, index: int) -> Any:
    pass
def AddMessage(field: str) -> Any:
    pass
def PbReadInt(pb: Any, field: str, index: int = ...) -> int:
    """Reads an int32, uint32, sint32, fixed32, sfixed32, or enum value from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param index         Index into repeated field.
@return              Integer value read.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadFloat(pb: Any, field: str, index: int = ...) -> float:
    """Reads a float or downcasted double from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param index         Index into repeated field.
@return              Float value read.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadBool(pb: Any, field: str, index: int = ...) -> bool:
    """Reads a bool from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param index         Index into repeated field.
@return              Boolean value read.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadString(pb: Any, field: str, buffer: str, maxlength: int, index: int = ...) -> None:
    """Reads a string from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadColor(pb: Any, field: str, buffer: list[int], index: int = ...) -> None:
    """Reads an RGBA color value from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param buffer        Destination color buffer.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadAngle(pb: Any, field: str, buffer: list[float], index: int = ...) -> None:
    """Reads an XYZ angle value from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param buffer        Destination angle buffer.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadVector(pb: Any, field: str, buffer: list[float], index: int = ...) -> None:
    """Reads an XYZ vector value from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param buffer        Destination vector buffer.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadVector2D(pb: Any, field: str, buffer: list[float], index: int = ...) -> None:
    """Reads an XY vector value from a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param buffer        Destination vector buffer.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbGetRepeatedFieldCount(pb: Any, field: str) -> int:
    """Gets the number of elements in a repeated field of a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@return              Number of elements in the field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetInt(pb: Any, field: str, value: int, index: int = ...) -> None:
    """Sets an int32, uint32, sint32, fixed32, sfixed32, or enum value on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param value         Integer value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetFloat(pb: Any, field: str, value: float, index: int = ...) -> None:
    """Sets a float or double on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param value         Float value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetBool(pb: Any, field: str, value: bool, index: int = ...) -> None:
    """Sets a bool on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param value         Boolean value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetString(pb: Any, field: str, value: str, index: int = ...) -> None:
    """Sets a string on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param value         String value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetColor(pb: Any, field: str, color: list[int], index: int = ...) -> None:
    """Sets an RGBA color on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param color         Color value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetAngle(pb: Any, field: str, angle: list[float], index: int = ...) -> None:
    """Sets an XYZ angle on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param angle         Angle value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetVector(pb: Any, field: str, vec: list[float], index: int = ...) -> None:
    """Sets an XYZ vector on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param vec           Vector value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbSetVector2D(pb: Any, field: str, vec: list[float], index: int = ...) -> None:
    """Sets an XY vector on a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@param vec           Vector value to set.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddInt(pb: Any, field: str, value: int) -> None:
    """Add an int32, uint32, sint32, fixed32, sfixed32, or enum value to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param value         Integer value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddFloat(pb: Any, field: str, value: float) -> None:
    """Add a float or double to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param value         Float value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddBool(pb: Any, field: str, value: bool) -> None:
    """Add a bool to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param value         Boolean value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddString(pb: Any, field: str, value: str) -> None:
    """Add a string to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param value         String value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddColor(pb: Any, field: str, color: list[int]) -> None:
    """Add an RGBA color to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param color         Color value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddAngle(pb: Any, field: str, angle: list[float]) -> None:
    """Add an XYZ angle to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param angle         Angle value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddVector(pb: Any, field: str, vec: list[float]) -> None:
    """Add an XYZ vector to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param vec           Vector value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddVector2D(pb: Any, field: str, vec: list[float]) -> None:
    """Add an XY vector to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param vec           Vector value to add.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbRemoveRepeatedFieldValue(pb: Any, field: str, index: int) -> None:
    """Removes a value by index from a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param index         Index into repeated field.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadMessage(pb: Any, field: str) -> Any:
    """Retrieve a handle to an embedded protobuf message in a protobuf message.

@param pb            protobuf handle.
@param field         Field name.
@return              protobuf handle to embedded message.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbReadRepeatedMessage(pb: Any, field: str, index: int) -> Any:
    """Retrieve a handle to an embedded protobuf message in a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@param index         Index in the repeated field.
@return              protobuf handle to embedded message.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
def PbAddMessage(pb: Any, field: str) -> Any:
    """Adds an embedded protobuf message to a protobuf message repeated field.

@param pb            protobuf handle.
@param field         Field name.
@return              protobuf handle to added, embedded message.
@error               Invalid or incorrect Handle, non-existent field, or incorrect field type."""
    pass
PB_FIELD_NOT_REPEATED: Any = ...  # -1