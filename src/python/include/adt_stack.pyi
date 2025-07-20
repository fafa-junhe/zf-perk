from typing import Any, list, Callable, Union
from .handles import *


def ArrayStack(blocksize: int = ...) -> Any:
    pass
def Clear() -> None:
    pass
def Clone() -> Any:
    pass
def Push(value: Any) -> None:
    pass
def PushString(value: str) -> None:
    pass
def PushArray(values: list[Any], size: int = ...) -> None:
    pass
def Pop(block: int = ..., asChar: bool = ...) -> Any:
    pass
def Top(block: int = ..., asChar: bool = ...) -> Any:
    pass
def PopString(buffer: str, maxlength: int, written: int = ...) -> None:
    pass
def TopString(buffer: str, maxlength: int, written: int = ...) -> None:
    pass
def PopArray(buffer: list[Any], size: int = ...) -> None:
    pass
def TopArray(buffer: list[Any], size: int = ...) -> None:
    pass
def get() -> Any:
    pass
def CreateStack(blocksize: int = ...) -> Any:
    """Creates a stack structure.  A stack is a LIFO (last in, first out) 
vector (array) of items.  It has O(1) insertion and O(1) removal.

Stacks have two operations: Push (adding an item) and Pop (removes 
items in reverse-push order).

The contents of the stack are uniform; i.e. storing a string and then 
retrieving it as an integer is NOT the same as StringToInt()!

The "blocksize" determines how many cells each slot has; it cannot
be changed after creation.

@param blocksize     The number of cells each entry in the stack can 
                     hold.  For example, 32 cells is equivalent to:
                     new Array[X][32]
@return              New stack Handle."""
    pass
def PushStackCell(stack: Any, value: Any) -> None:
    """Pushes a value onto the end of the stack, adding a new index.

This may safely be used even if the stack has a blocksize
greater than 1.

@param stack         Stack Handle.
@param value         Value to push.
@error               Invalid Handle or out of memory."""
    pass
def PushStackString(stack: Any, value: str) -> None:
    """Pushes a copy of a string onto the end of a stack, truncating it if it is 
too big.

@param stack         Stack Handle.
@param value         String to push.
@error               Invalid Handle or out of memory."""
    pass
def PushStackArray(stack: Any, values: list[Any], size: int = ...) -> None:
    """Pushes a copy of an array of cells onto the end of a stack.  The cells
are pushed as a block (i.e. the entire array takes up one stack slot),
rather than pushing each cell individually.

@param stack         Stack Handle.
@param values        Block of values to copy.
@param size          If not set, the number of elements copied from the array
                     will be equal to the blocksize.  If set higher than the 
                     blocksize, the operation will be truncated.
@error               Invalid Handle or out of memory."""
    pass
def PopStackCell(stack: Any, value: Any, block: int = ..., asChar: bool = ...) -> bool:
    """Pops a cell value from a stack.

@param stack         Stack Handle.
@param value         Variable to store the value.
@param block         Optionally specify which block to read from
                     (useful if the blocksize > 0).
@param asChar        Optionally read as a byte instead of a cell.
@return              True on success, false if the stack is empty.
@error               Invalid Handle."""
    pass
def PopStackString(stack: Any, buffer: str, maxlength: int, written: int = ...) -> bool:
    """Pops a string value from a stack.

@param stack         Stack Handle.
@param buffer        Buffer to store string.
@param maxlength     Maximum size of the buffer.
@return              True on success, false if the stack is empty.
@error               Invalid Handle."""
    pass
def PopStackArray(stack: Any, buffer: list[Any], size: int = ...) -> bool:
    """Pops an array of cells from a stack.

@param stack         Stack Handle.
@param buffer        Buffer to store the array in.
@param size          If not set, assumes the buffer size is equal to the
                     blocksize.  Otherwise, the size passed is used.
@return              True on success, false if the stack is empty.
@error               Invalid Handle."""
    pass
def IsStackEmpty(stack: Any) -> bool:
    """Checks if a stack is empty.

@param stack         Stack Handle.
@return              True if empty, false if not empty.
@error               Invalid Handle."""
    pass
def PopStack(stack: Any) -> bool:
    """Pops a value off a stack, ignoring it completely.

@param stack         Stack Handle.
@return              True if something was popped, false otherwise.
@error               Invalid Handle."""
    pass
def GetStackBlockSize(stack: Any) -> int:
    """Returns the blocksize the stack was created with.

@param stack         Stack Handle.
@return              The blocksize of the stack.
@error               Invalid Handle"""
    pass