from typing import Any, list, Callable, Union
from .handles import *


def StringMap() -> Any:
    pass
def Clone() -> Any:
    pass
def SetValue(key: str, value: Any, replace: bool = ...) -> bool:
    pass
def SetArray(key: str, array: list[Any], num_items: int, replace: bool = ...) -> bool:
    pass
def SetString(key: str, value: str, replace: bool = ...) -> bool:
    pass
def GetValue(key: str, value: Any) -> bool:
    pass
def GetArray(key: str, array: list[Any], max_size: int, size: int = ...) -> bool:
    pass
def GetString(key: str, value: str, max_size: int, size: int = ...) -> bool:
    pass
def ContainsKey(key: str) -> bool:
    pass
def Remove(key: str) -> bool:
    pass
def Clear() -> None:
    pass
def Snapshot() -> Any:
    pass
def get() -> Any:
    pass
def KeyBufferSize(index: int) -> int:
    pass
def GetKey(index: int, buffer: str, maxlength: int) -> int:
    pass
def IntMap() -> Any:
    pass
def CreateTrie() -> Any:
    """Creates a hash map. A hash map is a container that can map strings (called
"keys") to arbitrary values (cells, arrays, or strings). Keys in a hash map
are unique. That is, there is at most one entry in the map for a given key.

Insertion, deletion, and lookup in a hash map are all considered to be fast
operations, amortized to O(1), or constant time.

The word "Trie" in this API is historical. As of SourceMod 1.6, tries have
been internally replaced with hash tables, which have O(1) insertion time
instead of O(n).

@return              New Map Handle, which must be freed via CloseHandle()."""
    pass
def SetTrieValue(map: Any, key: str, value: Any, replace: bool = ...) -> bool:
    """Sets a value in a hash map, either inserting a new entry or replacing an old one.

@param map           Map Handle.
@param key           Key string.
@param value         Value to store at this key.
@param replace       If false, operation will fail if the key is already set.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass
def SetTrieArray(map: Any, key: str, array: list[Any], num_items: int, replace: bool = ...) -> bool:
    """Sets an array value in a Map, either inserting a new entry or replacing an old one.

@param map           Map Handle.
@param key           Key string.
@param array         Array to store.
@param num_items     Number of items in the array.
@param replace       If false, operation will fail if the key is already set.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass
def SetTrieString(map: Any, key: str, value: str, replace: bool = ...) -> bool:
    """Sets a string value in a Map, either inserting a new entry or replacing an old one.

@param map           Map Handle.
@param key           Key string.
@param value         String to store.
@param replace       If false, operation will fail if the key is already set.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass
def GetTrieValue(map: Any, key: str, value: Any) -> bool:
    """Retrieves a value in a Map.

@param map           Map Handle.
@param key           Key string.
@param value         Variable to store value.
@return              True on success.  False if the key is not set, or the key is set 
                     as an array or string (not a value).
@error               Invalid Handle."""
    pass
def GetTrieArray(map: Any, key: str, array: list[Any], max_size: int, size: int = ...) -> bool:
    """Retrieves an array in a Map.

@param map           Map Handle.
@param key           Key string.
@param array         Buffer to store array.
@param max_size      Maximum size of array buffer.
@param size          Optional parameter to store the number of elements written to the buffer.
@return              True on success.  False if the key is not set, or the key is set 
                     as a value or string (not an array).
@error               Invalid Handle."""
    pass
def GetTrieString(map: Any, key: str, value: str, max_size: int, size: int = ...) -> bool:
    """Retrieves a string in a Map.

@param map           Map Handle.
@param key           Key string.
@param value         Buffer to store value.
@param max_size      Maximum size of string buffer.
@param size          Optional parameter to store the number of bytes written to the buffer.
@return              True on success.  False if the key is not set, or the key is set 
                     as a value or array (not a string).
@error               Invalid Handle."""
    pass
def RemoveFromTrie(map: Any, key: str) -> bool:
    """Removes a key entry from a Map.

@param map           Map Handle.
@param key           Key string.
@return              True on success, false if the value was never set.
@error               Invalid Handle."""
    pass
def ClearTrie(map: Any) -> None:
    """Clears all entries from a Map.

@param map           Map Handle.
@error               Invalid Handle."""
    pass
def GetTrieSize(map: Any) -> int:
    """Retrieves the number of elements in a map.

@param map           Map Handle.
@return              Number of elements in the trie.
@error               Invalid Handle."""
    pass
def CreateTrieSnapshot(map: Any) -> Any:
    """Creates a snapshot of all keys in the map. If the map is changed after this
call, the changes are not reflected in the snapshot. Keys are not sorted.

@param map           Map Handle.
@return              New Map Snapshot Handle, which must be closed via CloseHandle().
@error               Invalid Handle."""
    pass
def TrieSnapshotLength(snapshot: Any) -> int:
    """Returns the number of keys in a map snapshot. Note that this may be
different from the size of the map, since the map can change after the
snapshot of its keys was taken.

@param snapshot      Map snapshot.
@return              Number of keys.
@error               Invalid Handle."""
    pass
def TrieSnapshotKeyBufferSize(snapshot: Any, index: int) -> int:
    """Returns the buffer size required to store a given key. That is, it returns
the length of the key plus one.

@param snapshot      Map snapshot.
@param index         Key index (starting from 0).
@return              Buffer size required to store the key string.
@error               Invalid Handle or index out of range."""
    pass
def GetTrieSnapshotKey(snapshot: Any, index: int, buffer: str, maxlength: int) -> int:
    """Retrieves the key string of a given key in a map snapshot.

@param snapshot      Map snapshot.
@param index         Key index (starting from 0).
@param buffer        String buffer.
@param maxlength     Maximum buffer length.
@return              Number of bytes written to the buffer.
@error               Invalid Handle or index out of range."""
    pass