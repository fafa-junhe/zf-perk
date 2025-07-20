from typing import Any, list, Callable, Union
from .handles import *


def DataPack() -> Any:
    pass
def WriteCell(cell: Any, insert: bool = ...) -> None:
    pass
def WriteFloat(val: float, insert: bool = ...) -> None:
    pass
def WriteString(str: str, insert: bool = ...) -> None:
    pass
def WriteFunction(fktptr: Callable, insert: bool = ...) -> None:
    pass
def WriteCellArray(array: list[Any], count: int, insert: bool = ...) -> None:
    pass
def WriteFloatArray(array: list[float], count: int, insert: bool = ...) -> None:
    pass
def ReadCell() -> Any:
    pass
def ReadFloat() -> float:
    pass
def ReadString(buffer: str, maxlen: int) -> None:
    pass
def ReadFunction() -> Callable:
    pass
def ReadCellArray(buffer: list[Any], count: int) -> None:
    pass
def ReadFloatArray(buffer: list[float], count: int) -> None:
    pass
def Reset(clear: bool = ...) -> None:
    pass
def IsReadable(unused: int = ...) -> bool:
    pass
def get() -> Any:
    pass
def set(pos: Any) -> Any:
    pass
def CreateDataPack() -> Any:
    """Creates a new data pack.

@return              A Handle to the data pack.  Must be closed with CloseHandle()."""
    pass
def WritePackCell(pack: Any, cell: Any) -> None:
    """Packs a normal cell into a data pack.

@param pack          Handle to the data pack.
@param cell          Cell to add.
@error               Invalid handle."""
    pass
def WritePackFloat(pack: Any, val: float) -> None:
    """Packs a float into a data pack.

@param pack          Handle to the data pack.
@param val           Float to add.
@error               Invalid handle."""
    pass
def WritePackString(pack: Any, str: str) -> None:
    """Packs a string into a data pack.

@param pack          Handle to the data pack.
@param str           String to add.
@error               Invalid handle."""
    pass
def WritePackFunction(pack: Any, fktptr: Callable) -> None:
    """Packs a function pointer into a data pack.

@param pack          Handle to the data pack.
@param fktptr        Function pointer to add.
@error               Invalid handle."""
    pass
def ReadPackCell(pack: Any) -> Any:
    """Reads a cell from a data pack.

@param pack          Handle to the data pack.
@return              Cell value.
@error               Invalid handle, or bounds error."""
    pass
def ReadPackFloat(pack: Any) -> float:
    """Reads a float from a data pack.

@param pack          Handle to the data pack.
@return              Float value.
@error               Invalid handle, or bounds error."""
    pass
def ReadPackString(pack: Any, buffer: str, maxlen: int) -> None:
    """Reads a string from a data pack.

@param pack          Handle to the data pack.
@param buffer        Destination string buffer.
@param maxlen        Maximum length of output string buffer.
@error               Invalid handle, or bounds error."""
    pass
def ReadPackFunction(pack: Any) -> Callable:
    """Reads a function pointer from a data pack.

@param pack          Handle to the data pack.
@return              Function pointer.
@error               Invalid handle, or bounds error."""
    pass
def ResetPack(pack: Any, clear: bool = ...) -> None:
    """Resets the position in a data pack.

@param pack          Handle to the data pack.
@param clear         If true, clears the contained data.
@error               Invalid handle."""
    pass
def GetPackPosition(pack: Any) -> Any:
    """Returns the read or write position in a data pack.

@param pack          Handle to the data pack.
@return              Position in the data pack, only usable with calls to SetPackPosition.
@error               Invalid handle."""
    pass
def SetPackPosition(pack: Any, position: Any) -> None:
    """Sets the read/write position in a data pack.

@param pack          Handle to the data pack.
@param position      New position to set. Must have been previously retrieved from a call to GetPackPosition.
@error               Invalid handle, or position is beyond the pack bounds."""
    pass
def IsPackReadable(pack: Any, bytes: int) -> bool:
    """Returns whether or not a specified number of bytes from the data pack
position to the end can be read.

@param pack          Handle to the data pack.
@param bytes         Number of bytes to simulate reading.
@return              True if can be read, false otherwise.
@error               Invalid handle."""
    pass