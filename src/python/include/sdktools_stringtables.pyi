from typing import Any, list, Callable, Union


def FindStringTable(name: str) -> int:
    """Searches for a string table.

@param name          Name of string table to find.
@return              A string table index number if found, INVALID_STRING_TABLE otherwise."""
    pass
def GetNumStringTables() -> int:
    """Returns the number of string tables that currently exist.

@return              Number of string tables that currently exist."""
    pass
def GetStringTableNumStrings(tableidx: int) -> int:
    """Returns the number of strings that currently exist in a given string table.

@param tableidx      A string table index.
@return              Number of strings that currently exist.
@error               Invalid string table index."""
    pass
def GetStringTableMaxStrings(tableidx: int) -> int:
    """Returns the maximum number of strings that are allowed in a given string table.

@param tableidx      A string table index.
@return              Maximum number of strings allowed.
@error               Invalid string table index."""
    pass
def GetStringTableName(tableidx: int, name: str, maxlength: int) -> int:
    """Retrieves the name of a string table.

@param tableidx      A string table index.
@param name          Buffer to store the name of the string table.
@param maxlength     Maximum length of string buffer.
@return              Number of bytes written to the buffer (UTF-8 safe).
@error               Invalid string table index."""
    pass
def FindStringIndex(tableidx: int, str: str) -> int:
    """Searches for the index of a given string in a string table.

@param tableidx      A string table index.
@param str           String to find.
@return              String index if found, INVALID_STRING_INDEX otherwise.
@error               Invalid string table index."""
    pass
def ReadStringTable(tableidx: int, stringidx: int, str: str, maxlength: int) -> int:
    """Retrieves the string at a given index of a string table.

@param tableidx      A string table index.
@param stringidx     A string index.
@param str           Buffer to store the string value.
@param maxlength     Maximum length of string buffer.
@return              Number of bytes written to the buffer (UTF-8 safe).
@error               Invalid string table index or string index."""
    pass
def GetStringTableDataLength(tableidx: int, stringidx: int) -> int:
    """Returns the length of the user data associated with a given string index.

@param tableidx      A string table index.
@param stringidx     A string index.
@return              Length of user data. This will be 0 if there is no user data.
@error               Invalid string table index or string index."""
    pass
def GetStringTableData(tableidx: int, stringidx: int, userdata: str, maxlength: int) -> int:
    """Retrieves the user data associated with a given string index.

@param tableidx      A string table index.
@param stringidx     A string index.
@param userdata      Buffer to store the user data. This will be set to "" if there is no user data
@param maxlength     Maximum length of string buffer.
@return              Number of bytes written to the buffer (binary safe, includes the null terminator).
@error               Invalid string table index or string index."""
    pass
def SetStringTableData(tableidx: int, stringidx: int, userdata: str, length: int) -> None:
    """Sets the user data associated with a given string index.

@param tableidx      A string table index.
@param stringidx     A string index.
@param userdata      User data string that will be set.
@param length        Length of user data string. This should include the null terminator.
@error               Invalid string table index or string index."""
    pass
def AddToStringTable(tableidx: int, str: str, userdata: str = ..., length: int = ...) -> None:
    """Adds a string to a given string table.

@param tableidx      A string table index.
@param str           String to add.
@param userdata      An optional user data string.
@param length        Length of user data string. This should include the null terminator.
                     If set to -1, then user data will be not be altered if the specified string
                     already exists in the string table.
@error               Invalid string table index."""
    pass
def LockStringTables(lock: bool) -> bool:
    """Locks or unlocks the network string tables.

@param lock          Determines whether network string tables should be locked.
                     True means the tables should be locked for writing; false means unlocked.
@return              Previous lock state."""
    pass
def AddFileToDownloadsTable(filename: str) -> None:
    """Adds a file to the downloadables network string table.
This forces a client to download the file if they do not already have it.

@param filename      File that will be added to downloadables table."""
    pass
INVALID_STRING_TABLE: Any = ...  # -1     /**< An invalid string table index */
INVALID_STRING_INDEX: Any = ...  # -1     /**< An invalid string index in a table */
save: bool = ...