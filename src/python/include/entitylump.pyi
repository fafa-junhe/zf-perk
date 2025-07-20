from typing import Any, list, Callable, Union


def Get(index: int, keybuf: str = ..., keylen: int = ..., valbuf: str = ..., vallen: int = ...) -> None:
    """Copies the key / value at the given index into buffers.

@param index     Position, starting from 0.
@param keybuf    Key name buffer.
@param keylen    Maximum length of the key name buffer.
@param valbuf    Value buffer.
@param vallen    Maximum length of the value buffer.
@error           Index is out of bounds."""
    pass
def Update(index: int, key: str = ..., value: str = ...) -> None:
    """Updates the key / value pair at the given index.

@param index    Position, starting from 0.
@param key      New key name, or NULL_STRING to preserve the existing key name.
@param value    New value, or NULL_STRING to preserve the existing value.
@error          Index is out of bounds or entity lump is read-only."""
    pass
def Insert(index: int, key: str, value: str) -> None:
    """Inserts a new key / value pair at the given index, shifting the pair at that index and beyond up.
If EntityLumpEntry.Length is passed in, this is an append operation.

@param index    Position, starting from 0.
@param key      New key name.
@param value    New value.
@error          Index is out of bounds or entity lump is read-only."""
    pass
def Erase(index: int) -> None:
    """Removes the key / value pair at the given index, shifting all entries past it down.

@param index    Position, starting from 0.
@error          Index is out of bounds or entity lump is read-only."""
    pass
def Append(key: str, value: str) -> None:
    """Inserts a new key / value pair at the end of the entry's list.

@param key      New key name.
@param value    New value.
@error          Index is out of bounds or entity lump is read-only."""
    pass
def FindKey(key: str, start: int = ...) -> int:
    """Searches the entry list for an index matching a key starting from a position.

@param key      Key name to search.
@param start    A position after which to begin searching from.  Use -1 to start from the
                first entry.
@return         Position after start with an entry matching the given key, or -1 if no
                match was found.
@error          Invalid start position; must be a value between -1 and one less than the
                length of the entry."""
    pass
def GetNextKey(key: str, buffer: str, maxlen: int, start: int = ...) -> int:
    """Searches the entry list for an index matching a key starting from a position.
This also copies the value from that index into the given buffer.

This can be used to find the first / only value matching a key, or to iterate over all
the values that match said key.

@param key       Key name to search.
@param buffer    Value buffer.  This will contain the result of the next match, or empty
                 if no match was found.
@param maxlen    Maximum length of the value buffer.
@param start     An index after which to begin searching from.  Use -1 to start from the
                 first entry.
@return          Position after start with an entry matching the given key, or -1 if no
                 match was found.
@error           Invalid start position; must be a value between -1 and one less than the
                 length of the entry."""
    pass
def get() -> Any:
    pass
result: int = ...