from typing import Any, list, Callable, Union
from .handles import *


def WriteBool(bit: bool) -> None:
    pass
def WriteByte(byte: int) -> None:
    pass
def WriteChar(chr: int) -> None:
    pass
def WriteShort(num: int) -> None:
    pass
def WriteWord(num: int) -> None:
    pass
def WriteNum(num: int) -> None:
    pass
def WriteFloat(num: float) -> None:
    pass
def WriteString(string: str) -> None:
    pass
def WriteEntity(ent: int) -> None:
    pass
def WriteAngle(angle: float, numBits: int = ...) -> None:
    pass
def WriteCoord(coord: float) -> None:
    pass
def WriteVecCoord(coord: list[float]) -> None:
    pass
def WriteVecNormal(vec: list[float]) -> None:
    pass
def WriteAngles(angles: list[float]) -> None:
    pass
def ReadBool() -> bool:
    pass
def ReadByte() -> int:
    pass
def ReadChar() -> int:
    pass
def ReadShort() -> int:
    pass
def ReadWord() -> int:
    pass
def ReadNum() -> int:
    pass
def ReadFloat() -> float:
    pass
def ReadString(buffer: str, maxlength: int, line: bool = ...) -> int:
    pass
def ReadEntity() -> int:
    pass
def ReadAngle(numBits: int = ...) -> float:
    pass
def ReadCoord() -> float:
    pass
def ReadVecCoord(coord: list[float]) -> None:
    pass
def ReadVecNormal(vec: list[float]) -> None:
    pass
def ReadAngles(angles: list[float]) -> None:
    pass
def get() -> Any:
    pass
def BfWriteBool(bf: Any, bit: bool) -> None:
    """Writes a single bit to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param bit           Bit to write (true for 1, false for 0).
@error               Invalid or incorrect Handle."""
    pass
def BfWriteByte(bf: Any, byte: int) -> None:
    """Writes a byte to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param byte          Byte to write (value will be written as 8bit).
@error               Invalid or incorrect Handle."""
    pass
def BfWriteChar(bf: Any, chr: int) -> None:
    """Writes a byte to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param chr           Character to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteShort(bf: Any, num: int) -> None:
    """Writes a 16bit integer to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param num           Integer to write (value will be written as 16bit).
@error               Invalid or incorrect Handle."""
    pass
def BfWriteWord(bf: Any, num: int) -> None:
    """Writes a 16bit unsigned integer to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param num           Integer to write (value will be written as 16bit).
@error               Invalid or incorrect Handle."""
    pass
def BfWriteNum(bf: Any, num: int) -> None:
    """Writes a normal integer to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param num           Integer to write (value will be written as 32bit).
@error               Invalid or incorrect Handle."""
    pass
def BfWriteFloat(bf: Any, num: float) -> None:
    """Writes a floating point number to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param num           Number to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteString(bf: Any, string: str) -> None:
    """Writes a string to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param string        Text string to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteEntity(bf: Any, ent: int) -> None:
    """Writes an entity to a writable bitbuffer (bf_write).
@note This is a wrapper around BfWriteShort().

@param bf            bf_write handle to write to.
@param ent           Entity index to write.
@error               Invalid or incorrect Handle, or invalid entity."""
    pass
def BfWriteAngle(bf: Any, angle: float, numBits: int = ...) -> None:
    """Writes a bit angle to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param angle         Angle to write.
@param numBits       Optional number of bits to use.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteCoord(bf: Any, coord: float) -> None:
    """Writes a coordinate to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param coord         Coordinate to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteVecCoord(bf: Any, coord: list[float]) -> None:
    """Writes a 3D vector of coordinates to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param coord         Coordinate array to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteVecNormal(bf: Any, vec: list[float]) -> None:
    """Writes a 3D normal vector to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param vec           Vector to write.
@error               Invalid or incorrect Handle."""
    pass
def BfWriteAngles(bf: Any, angles: list[float]) -> None:
    """Writes a 3D angle vector to a writable bitbuffer (bf_write).

@param bf            bf_write handle to write to.
@param angles        Angle vector to write.
@error               Invalid or incorrect Handle."""
    pass
def BfReadBool(bf: Any) -> bool:
    """Reads a single bit from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Bit value read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadByte(bf: Any) -> int:
    """Reads a byte from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Byte value read (read as 8bit).
@error               Invalid or incorrect Handle."""
    pass
def BfReadChar(bf: Any) -> int:
    """Reads a character from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Character value read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadShort(bf: Any) -> int:
    """Reads a 16bit integer from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Integer value read (read as 16bit).
@error               Invalid or incorrect Handle."""
    pass
def BfReadWord(bf: Any) -> int:
    """Reads a 16bit unsigned integer from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Integer value read (read as 16bit).
@error               Invalid or incorrect Handle."""
    pass
def BfReadNum(bf: Any) -> int:
    """Reads a normal integer to a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Integer value read (read as 32bit).
@error               Invalid or incorrect Handle."""
    pass
def BfReadFloat(bf: Any) -> float:
    """Reads a floating point number from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Floating point value read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadString(bf: Any, buffer: str, maxlength: int, line: bool = ...) -> int:
    """Reads a string from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer.
@param line          If true the buffer will be copied until it reaches a '\n' or a null terminator.
@return              Number of bytes written to the buffer.  If the bitbuffer stream overflowed, 
                     that is, had no terminator before the end of the stream, then a negative 
                     number will be returned equal to the number of characters written to the 
                     buffer minus 1.  The buffer will be null terminated regardless of the 
                     return value.
@error               Invalid or incorrect Handle."""
    pass
def BfReadEntity(bf: Any) -> int:
    """Reads an entity from a readable bitbuffer (bf_read).
@note This is a wrapper around BfReadShort().

@param bf            bf_read handle to read from.
@return              Entity index read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadAngle(bf: Any, numBits: int = ...) -> float:
    """Reads a bit angle from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@param numBits       Optional number of bits to use.
@return              Angle read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadCoord(bf: Any) -> float:
    """Reads a coordinate from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Coordinate read.
@error               Invalid or incorrect Handle."""
    pass
def BfReadVecCoord(bf: Any, coord: list[float]) -> None:
    """Reads a 3D vector of coordinates from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@param coord         Destination coordinate array.
@error               Invalid or incorrect Handle."""
    pass
def BfReadVecNormal(bf: Any, vec: list[float]) -> None:
    """Reads a 3D normal vector from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@param vec           Destination vector array.
@error               Invalid or incorrect Handle."""
    pass
def BfReadAngles(bf: Any, angles: list[float]) -> None:
    """Reads a 3D angle vector from a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@param angles        Destination angle vector.
@error               Invalid or incorrect Handle."""
    pass
def BfGetNumBytesLeft(bf: Any) -> int:
    """Returns the number of bytes left in a readable bitbuffer (bf_read).

@param bf            bf_read handle to read from.
@return              Number of bytes left unread.
@error               Invalid or incorrect Handle."""
    pass