from typing import Any, list, Callable, Union
from .handles import *


class FileTimeMode:
    """File time modes."""
    FileTime_Created: int = ...
    FileTime_LastAccess: int = ...
    FileTime_LastChange: int = ...


class FileType:
    """File inode types."""
    FileType_Directory: int = ...
    FileType_File: int = ...
    FileType_Unknown: int = ...


class PathType:
    """Path types."""
    Path_SM: int = ...


def GetNext(buffer: str, maxlength: int, type: FileType = ...) -> bool:
    pass
def Close() -> None:
    pass
def Size() -> int:
    pass
def ReadLine(buffer: str, maxlength: int) -> bool:
    pass
def Read(items: list[Any], num_items: int, size: int) -> int:
    pass
def ReadString(buffer: str, max_size: int, read_count: int = ...) -> int:
    pass
def Write(items: list[Any], num_items: int, size: int) -> bool:
    pass
def WriteString(buffer: str, term: bool) -> bool:
    pass
def WriteLine(format: str, _0: Any, *args: Any) -> bool:
    pass
def ReadInt8(data: int) -> bool:
    pass
def ReadUint8(data: int) -> bool:
    pass
def ReadInt16(data: int) -> bool:
    pass
def ReadUint16(data: int) -> bool:
    pass
def ReadInt32(data: int) -> bool:
    pass
def WriteInt8(data: int) -> bool:
    pass
def WriteInt16(data: int) -> bool:
    pass
def WriteInt32(data: int) -> bool:
    pass
def EndOfFile() -> bool:
    pass
def Seek(position: int, where: int) -> bool:
    pass
def Flush() -> bool:
    pass
def get() -> Any:
    pass
def BuildPath(type: PathType, buffer: str, maxlength: int, fmt: str, _0: Any, *args: Any) -> int:
    """Builds a path relative to the SourceMod folder.  This should be used instead of
directly referencing addons/sourcemod, in case users change the name of their
folder layout.

@param type          Type of path to build as the base.
@param buffer        Buffer to store the path.
@param maxlength     Maximum length of buffer.
@param fmt           Format string.
@param ...           Format arguments.
@return              Number of bytes written to buffer (not including null terminator)."""
    pass
def OpenDirectory(path: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> Any:
    """Opens a directory/folder for contents enumeration.

@note Directories are closed with CloseHandle() or delete.
@note Directories Handles can be cloned.
@note OpenDirectory() supports the "file://" notation.

@param path          Path to open.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to find files existing in any of
                     the Valve search paths, rather than solely files
                     existing directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              A Handle to the directory, null on error."""
    pass
def ReadDirEntry(dir: Any, buffer: str, maxlength: int, type: FileType = ...) -> bool:
    """Reads the current directory entry as a local filename, then moves to the next file.

@note Contents of buffers are undefined when returning false.
@note Both the '.' and '..' automatic directory entries will be retrieved for Windows and Linux.

@param dir           Handle to a directory.
@param buffer        String buffer to hold directory name.
@param maxlength     Maximum size of string buffer.
@param type          Optional variable to store the file type.
@return              True on success, false if there are no more files to read.
@error               Invalid or corrupt Handle."""
    pass
def OpenFile(file: str, mode: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> Any:
    """Opens or creates a file, returning a File handle on success. File handles
should be closed with delete or CloseHandle().

The open mode may be one of the following strings:
  "r": Open an existing file for reading.
  "w": Create a file for writing, or truncate (delete the contents of) an
       existing file and then open it for writing.
  "a": Create a file for writing, or open an existing file such that writes
       will be appended to the end.
  "r+": Open an existing file for both reading and writing.
  "w+": Create a file for reading and writing, or truncate an existing file
        and then open it for reading and writing.
  "a+": Create a file for both reading and writing, or open an existing file
        such that writes will be appended to the end.

The open mode may also contain an additional character after "r", "w", or "a",
but before any "+" sign. This character may be "b" (indicating binary mode) or
"t" (indicating text mode). By default, "text" mode is implied. On Linux and
Mac, this has no distinction from binary mode. On Windows, it causes the '\n'
character (0xA) to be written as "\r\n" (0xD, 0xA).

Example: "rb" opens a binary file for reading; "at" opens a text file for
appending.

@param file          File to open.
@param mode          Open mode.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to find files existing in valve
                     search paths, rather than solely files existing directly
                     in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              A File handle, or null if the file could not be opened."""
    pass
def DeleteFile(path: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> bool:
    """Deletes a file.

@param path          Path of the file to delete.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to delete files existing in the Valve
                     search path, rather than solely files existing directly
                     in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              True on success, false on failure or if file not immediately removed."""
    pass
def ReadFileLine(hndl: Any, buffer: str, maxlength: int) -> bool:
    """Reads a line from a text file.

@param hndl          Handle to the file.
@param buffer        String buffer to hold the line.
@param maxlength     Maximum size of string buffer.
@return              True on success, false otherwise."""
    pass
def ReadFile(hndl: Any, items: list[Any], num_items: int, size: int) -> int:
    """Reads binary data from a file.

@param hndl          Handle to the file.
@param items         Array to store each item read.
@param num_items     Number of items to read into the array.
@param size          Size of each element, in bytes, to be read.
                     Valid sizes are 1, 2, or 4.
@return              Number of elements read, or -1 on error."""
    pass
def ReadFileString(hndl: Any, buffer: str, max_size: int, read_count: int = ...) -> int:
    """Reads a UTF8 or ANSI string from a file.

@param hndl          Handle to the file.
@param buffer        Buffer to store the string.
@param max_size      Maximum size of the string buffer.
@param read_count    If -1, reads until a null terminator is encountered in
                     the file.  Otherwise, read_count bytes are read
                     into the buffer provided.  In this case the buffer
                     is not explicitly null terminated, and the buffer
                     will contain any null terminators read from the file.
@return              Number of characters written to the buffer, or -1
                     if an error was encountered.
@error               Invalid Handle, or read_count > max_size."""
    pass
def WriteFile(hndl: Any, items: list[Any], num_items: int, size: int) -> bool:
    """Writes binary data to a file.

@param hndl          Handle to the file.
@param items         Array of items to write.  The data is read directly.
                     That is, in 1 or 2-byte mode, the lower byte(s) in
                     each cell are used directly, rather than performing
                     any casts from a 4-byte number to a smaller number.
@param num_items     Number of items in the array.
@param size          Size of each item in the array in bytes.
                     Valid sizes are 1, 2, or 4.
@return              True on success, false on error.
@error               Invalid Handle."""
    pass
def WriteFileString(hndl: Any, buffer: str, term: bool) -> bool:
    """Writes a binary string to a file.

@param hndl          Handle to the file.
@param buffer        String to write.
@param term          True to append NUL terminator, false otherwise.
@return              True on success, false on error.
@error               Invalid Handle."""
    pass
def WriteFileLine(hndl: Any, format: str, _0: Any, *args: Any) -> bool:
    """Writes a line of text to a text file.  A newline is automatically appended.

@param hndl          Handle to the file.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def ReadFileCell(hndl: Any, data: int, size: int) -> int:
    """Reads a single binary cell from a file.

@param hndl          Handle to the file.
@param data          Variable to store the data read.
@param size          Size of the data to read in bytes.  Valid
                     sizes are 1, 2, or 4 bytes.
@return              Number of elements read (max 1), or -1 on error.
@error               Invalid Handle."""
    pass
def WriteFileCell(hndl: Any, data: int, size: int) -> bool:
    """Writes a single binary cell to a file.

@param hndl          Handle to the file.
@param data          Cell to write to the file.
@param size          Size of the data to read in bytes.  Valid
                     sizes are 1, 2, or 4 bytes.  If the size
                     is less than 4 bytes, the data is truncated
                     rather than casted.  That is, only the lower
                     bits will be read.
@return              True on success, false on error.
@error               Invalid Handle."""
    pass
def IsEndOfFile(file: Any) -> bool:
    """Tests if the end of file has been reached.

@param file          Handle to the file.
@return              True if end of file has been reached, false otherwise.
@error               Invalid Handle."""
    pass
def FileSeek(file: Any, position: int, where: int) -> bool:
    """Sets the file position indicator.

@param file          Handle to the file.
@param position      Position relative to what is specified in whence.
@param where         SEEK_ constant value of where to see from.
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def FilePosition(file: Any) -> int:
    """Get current position in the file.

@param file          Handle to the file.
@return              Value for the file position indicator.
@error               Invalid Handle."""
    pass
def FileExists(path: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> bool:
    """Checks if a file exists.

@param path          Path to the file.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to find files existing in any of
                     the Valve search paths, rather than solely files
                     existing directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              True if the file exists, false otherwise."""
    pass
def RenameFile(newpath: str, oldpath: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> bool:
    """Renames a file.

@param newpath       New path to the file.
@param oldpath       Path to the existing file.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to rename files in the game's
                     Valve search paths, rather than directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              True on success or use_valve_fs specified, false otherwise."""
    pass
def DirExists(path: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> bool:
    """Checks if a directory exists.

@param path          Path to the directory.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to find files existing in any of
                     the Valve search paths, rather than solely files
                     existing directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              True if the directory exists, false otherwise."""
    pass
def FileSize(path: str, use_valve_fs: bool = ..., valve_path_id: str = ...) -> int:
    """Get the file size in bytes.

@param path          Path to the file.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to find files existing in any of
                     the Valve search paths, rather than solely files
                     existing directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for all search paths.
@return              File size in bytes, -1 if file not found."""
    pass
def FlushFile(file: Any) -> bool:
    """Flushes a file's buffered output; any buffered output
is immediately written to the file.

@param file          Handle to the file.
@return              True on success or use_valve_fs specified with OpenFile,
                     otherwise false on failure."""
    pass
def RemoveDir(path: str) -> bool:
    """Removes a directory.
@note On most Operating Systems you cannot remove a directory which has files inside it.

@param path          Path to the directory.
@return              True on success, false otherwise."""
    pass
def CreateDirectory(path: str, mode: int = ..., use_valve_fs: bool = ..., valve_path_id: str = ...) -> bool:
    """Creates a directory.

@param path          Path to create. Note that directories are not created recursively unless use_valve_fs is used.
@param mode          Permissions (default is o=rx,g=rx,u=rwx).  Note that folders must have
                     the execute bit set on Linux.  On Windows, the mode is ignored.
@param use_valve_fs  If true, the Valve file system will be used instead.
                     This can be used to create folders in the game's
                     Valve search paths, rather than directly in the gamedir.
@param valve_path_id If use_valve_fs, a search path from gameinfo or NULL_STRING for default.
                     In this case, mode is ignored.
@return              True on success, false otherwise."""
    pass
def SetFilePermissions(path: str, mode: int) -> bool:
    """Changes a file or directories permissions.

@param path          Path to the file.
@param mode          Permissions to set.
@return              True on success, false otherwise."""
    pass
def GetFilePermissions(path: str, mode: int) -> bool:
    """Retrieves a file or directories permissions.

@param path          Path to the file.
@param mode          Variable to store the permissions in.
@return              True on success, false otherwise."""
    pass
def GetFileTime(file: str, tmode: FileTimeMode) -> int:
    """Returns a file timestamp as a unix timestamp.

@param file          File name.
@param tmode         Time mode.
@return              Time value, or -1 on failure."""
    pass
def LogToOpenFile(hndl: Any, message: str, _0: Any, *args: Any) -> None:
    """Same as LogToFile(), except uses an open file Handle.  The file must
be opened in text appending mode.

@param hndl          Handle to the file.
@param message       Message format.
@param ...           Message format parameters.
@error               Invalid Handle."""
    pass
def LogToOpenFileEx(hndl: Any, message: str, _0: Any, *args: Any) -> None:
    """Same as LogToFileEx(), except uses an open file Handle.  The file must
be opened in text appending mode.

@param hndl          Handle to the file.
@param message       Message format.
@param ...           Message format parameters.
@error               Invalid Handle."""
    pass
PLATFORM_MAX_PATH: Any = ...  # 256   /**< Maximum path length. */
SEEK_SET: Any = ...  # 0              /**< Seek from start. */
SEEK_CUR: Any = ...  # 1              /**< Seek from current position. */
SEEK_END: Any = ...  # 2              /**< Seek from end position. */
FPERM_U_READ: Any = ...  # 0x0100
FPERM_U_WRITE: Any = ...  # 0x0080
FPERM_U_EXEC: Any = ...  # 0x0040
FPERM_G_READ: Any = ...  # 0x0020
FPERM_G_WRITE: Any = ...  # 0x0010
FPERM_G_EXEC: Any = ...  # 0x0008
FPERM_O_READ: Any = ...  # 0x0004
FPERM_O_WRITE: Any = ...  # 0x0002
FPERM_O_EXEC: Any = ...  # 0x0001