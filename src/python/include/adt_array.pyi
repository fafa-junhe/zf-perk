from typing import Any, list, Callable, Union
from .handles import *
from .sorting import *


def ByteCountToCells(size: int) -> int:
    """Given a maximum string size (including the null terminator),
returns the number of cells required to fit that string.

@param size          Number of bytes.
@return              Minimum number of cells required to fit the byte count."""
    pass
def ArrayList(blocksize: int = ..., startsize: int = ...) -> Any:
    pass
def Clear() -> None:
    pass
def Clone() -> Any:
    pass
def Resize(newsize: int) -> None:
    pass
def Push(value: Any) -> int:
    pass
def PushString(value: str) -> int:
    pass
def PushArray(values: list[Any], size: int = ...) -> int:
    pass
def Get(index: int, block: int = ..., asChar: bool = ...) -> Any:
    pass
def GetString(index: int, buffer: str, maxlength: int, block: int = ...) -> int:
    pass
def GetArray(index: int, buffer: list[Any], size: int = ..., block: int = ...) -> int:
    pass
def Set(index: int, value: Any, block: int = ..., asChar: bool = ...) -> None:
    pass
def SetString(index: int, value: str, size: int = ..., block: int = ...) -> int:
    pass
def SetArray(index: int, values: list[Any], size: int = ..., block: int = ...) -> int:
    pass
def ShiftUp(index: int) -> None:
    pass
def Erase(index: int) -> None:
    pass
def SwapAt(index1: int, index2: int) -> None:
    pass
def FindString(item: str, block: int = ...) -> int:
    pass
def FindValue(item: Any, block: int = ...) -> int:
    pass
def Sort(order: SortOrder, type: SortType) -> None:
    pass
def SortCustom(sortfunc: Any, hndl: Any = ...) -> None:
    pass
def get() -> Any:
    pass
def CreateArray(blocksize: int = ..., startsize: int = ...) -> Any:
    """Creates a dynamic global cell array.  While slower than a normal array,
it can be used globally AND dynamically, which is otherwise impossible.

The contents of the array are uniform; i.e. storing a string at index X
and then retrieving it as an integer is NOT the same as StringToInt()!
The "blocksize" determines how many cells each array slot has; it cannot
be changed after creation.

@param blocksize     The number of cells each member of the array can
                     hold.  For example, 32 cells is equivalent to:
                     new Array[X][32]
@param startsize     Initial size of the array.  Note that data will
                     NOT be auto-initialized.
@return              New Handle to the array object."""
    pass
def ClearArray(array: Any) -> None:
    """Clears an array of all entries.  This is the same as ResizeArray(0).

@param array         Array Handle.
@error               Invalid Handle."""
    pass
def CloneArray(array: Any) -> Any:
    """Clones an array, returning a new handle with the same size and data. This should NOT
be confused with CloneHandle. This is a completely new handle with the same data but
no relation to the original. You MUST close it.

@param array         Array handle to be cloned
@return              New handle to the cloned array object
@error               Invalid Handle"""
    pass
def ResizeArray(array: Any, newsize: int) -> None:
    """Resizes an array.  If the size is smaller than the current size,
the array is truncated.  If the size is larger than the current size,
the data at the additional indexes will not be initialized.

@param array         Array Handle.
@param newsize       New size.
@error               Invalid Handle or out of memory."""
    pass
def GetArraySize(array: Any) -> int:
    """Returns the array size.

@param array         Array Handle.
@return              Number of elements in the array.
@error               Invalid Handle."""
    pass
def PushArrayCell(array: Any, value: Any) -> int:
    """Pushes a value onto the end of an array, adding a new index.

This may safely be used even if the array has a blocksize
greater than 1.

@param array         Array Handle.
@param value         Value to push.
@return              Index of the new entry.
@error               Invalid Handle or out of memory."""
    pass
def PushArrayString(array: Any, value: str) -> int:
    """Pushes a string onto the end of an array, truncating it
if it is too big.

@param array         Array Handle.
@param value         String to push.
@return              Index of the new entry.
@error               Invalid Handle or out of memory."""
    pass
def PushArrayArray(array: Any, values: list[Any], size: int = ...) -> int:
    """Pushes an array of cells onto the end of an array.  The cells
are pushed as a block (i.e. the entire array sits at the index),
rather than pushing each cell individually.

@param array         Array Handle.
@param values        Block of values to copy.
@param size          If not set, the number of elements copied from the array
                     will be equal to the blocksize.  If set higher than the
                     blocksize, the operation will be truncated.
@return              Index of the new entry.
@error               Invalid Handle or out of memory."""
    pass
def GetArrayCell(array: Any, index: int, block: int = ..., asChar: bool = ...) -> Any:
    """Retrieves a cell value from an array.

@param array         Array Handle.
@param index         Index in the array.
@param block         Optionally specify which block to read from
                     (useful if the blocksize > 0).
@param asChar        Optionally read as a byte instead of a cell.
@return              Value read.
@error               Invalid Handle, invalid index, or invalid block."""
    pass
def GetArrayString(array: Any, index: int, buffer: str, maxlength: int, block: int = ...) -> int:
    """Retrieves a string value from an array.

@param array         Array Handle.
@param index         Index in the array.
@param buffer        Buffer to copy to.
@param maxlength     Maximum size of the buffer.
@param block         Optionally specify which block to read from
                     (useful if the blocksize > 0).
@return              Number of characters copied.
@error               Invalid Handle or invalid index."""
    pass
def GetArrayArray(array: Any, index: int, buffer: list[Any], size: int = ..., block: int = ...) -> int:
    """Retrieves an array of cells from an array.

@param array         Array Handle.
@param index         Index in the array.
@param buffer        Buffer to store the array in.
@param size          If not set, assumes the buffer size is equal to the
                     blocksize.  Otherwise, the size passed is used.
@param block         Optionally specify which block to read from
                     (useful if the blocksize > 0).
@return              Number of cells copied.
@error               Invalid Handle or invalid index."""
    pass
def SetArrayCell(array: Any, index: int, value: Any, block: int = ..., asChar: bool = ...) -> None:
    """Sets a cell value in an array.

@param array         Array Handle.
@param index         Index in the array.
@param value         Cell value to set.
@param block         Optionally specify which block to write to
                     (useful if the blocksize > 0).
@param asChar        Optionally set as a byte instead of a cell.
@error               Invalid Handle, invalid index, or invalid block."""
    pass
def SetArrayString(array: Any, index: int, value: str, size: int = ..., block: int = ...) -> int:
    """Sets a string value in an array.

@param array         Array Handle.
@param index         Index in the array.
@param value         String value to set.
@param size          If not set, assumes the buffer size is equal to the
                     blocksize.  Otherwise, the size passed is used.
@param block         Optionally specify which block to write to
                     (useful if the blocksize > 0).
@return              Number of characters copied.
@error               Invalid Handle or invalid index."""
    pass
def SetArrayArray(array: Any, index: int, values: list[Any], size: int = ..., block: int = ...) -> int:
    """Sets an array of cells in an array.

@param array         Array Handle.
@param index         Index in the array.
@param values        Array to copy.
@param size          If not set, assumes the buffer size is equal to the
                     blocksize.  Otherwise, the size passed is used.
@param block         Optionally specify which block to write to
                     (useful if the blocksize > 0).
@return              Number of cells copied.
@error               Invalid Handle or invalid index."""
    pass
def ShiftArrayUp(array: Any, index: int) -> None:
    """Shifts an array up.  All array contents after and including the given
index are shifted up by one, and the given index is then "free."
After shifting, the contents of the given index is undefined.

@param array         Array Handle.
@param index         Index in the array to shift up from.
@error               Invalid Handle or invalid index."""
    pass
def RemoveFromArray(array: Any, index: int) -> None:
    """Removes an array index, shifting the entire array down from that position
on.  For example, if item 8 of 10 is removed, the last 3 items will then be
(6,7,8) instead of (7,8,9), and all indexes before 8 will remain unchanged.

@param array         Array Handle.
@param index         Index in the array to remove at.
@error               Invalid Handle or invalid index."""
    pass
def SwapArrayItems(array: Any, index1: int, index2: int) -> None:
    """Swaps two items in the array.

@param array         Array Handle.
@param index1        First index.
@param index2        Second index.
@error               Invalid Handle or invalid index."""
    pass
def FindStringInArray(array: Any, item: str, block: int = ...) -> int:
    """Returns the index for the first occurrence of the provided string. If the string
cannot be located, -1 will be returned.

@param array         Array Handle.
@param item          String to search for
@param block         Optionally which block to search in
@return              Array index, or -1 on failure
@error               Invalid Handle"""
    pass
def FindValueInArray(array: Any, item: Any, block: int = ...) -> int:
    """Returns the index for the first occurrence of the provided value. If the value
cannot be located, -1 will be returned.

@param array         Array Handle.
@param item          Value to search for
@param block         Optionally which block to search in
@return              Array index, or -1 on failure
@error               Invalid Handle or invalid block"""
    pass
def GetArrayBlockSize(array: Any) -> int:
    """Returns the blocksize the array was created with.

@param array         Array Handle.
@return              The blocksize of the array.
@error               Invalid Handle"""
    pass