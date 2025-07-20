from typing import Any, list, Callable, Union
from .handles import *


class KvDataTypes:
    """KeyValue data value types"""
    KvData_Color: int = ...
    KvData_Float: int = ...
    KvData_Int: int = ...
    KvData_NUMTYPES: int = ...
    KvData_None: int = ...
    KvData_Ptr: int = ...
    KvData_String: int = ...
    KvData_UInt64: int = ...
    KvData_WString: int = ...


def KeyValues(name: str, firstKey: str = ..., firstValue: str = ...) -> Any:
    pass
def ExportToFile(file: str) -> bool:
    pass
def ExportToString(buffer: str, maxlength: int) -> int:
    pass
def get() -> Any:
    pass
def ImportFromFile(file: str) -> bool:
    pass
def ImportFromString(buffer: str, resourceName: str = ...) -> bool:
    pass
def Import(other: Any) -> None:
    pass
def SetString(key: str, value: str) -> None:
    pass
def SetNum(key: str, value: int) -> None:
    pass
def SetUInt64(key: str, value: list[int]) -> None:
    pass
def SetFloat(key: str, value: float) -> None:
    pass
def SetColor(key: str, r: int, g: int, b: int, a: int = ...) -> None:
    pass
def SetColor4(key: str, color: list[int]) -> None:
    pass
def SetVector(key: str, vec: list[float]) -> None:
    pass
def GetString(key: str, value: str, maxlength: int, defvalue: str = ...) -> None:
    pass
def GetNum(key: str, defvalue: int = ...) -> int:
    pass
def GetFloat(key: str, defvalue: float = ...) -> float:
    pass
def GetColor(key: str, r: int, g: int, b: int, a: int) -> None:
    pass
def GetColor4(key: str, color: list[int]) -> None:
    pass
def GetUInt64(key: str, value: list[int], defvalue: list[int] = ...) -> None:
    pass
def GetVector(key: str, vec: list[float], defvalue: list[float] = ...) -> None:
    pass
def JumpToKey(key: str, create: bool = ...) -> bool:
    pass
def JumpToKeySymbol(id: int) -> bool:
    pass
def GotoFirstSubKey(keyOnly: bool = ...) -> bool:
    pass
def GotoNextKey(keyOnly: bool = ...) -> bool:
    pass
def SavePosition() -> bool:
    pass
def GoBack() -> bool:
    pass
def DeleteKey(key: str) -> bool:
    pass
def DeleteThis() -> int:
    pass
def Rewind(clearHistory: bool = ...) -> None:
    pass
def GetSectionName(section: str, maxlength: int) -> bool:
    pass
def SetSectionName(section: str) -> None:
    pass
def GetDataType(key: str) -> KvDataTypes:
    pass
def SetEscapeSequences(useEscapes: bool) -> None:
    pass
def NodesInStack() -> int:
    pass
def FindKeyById(id: int, name: str, maxlength: int) -> bool:
    pass
def GetNameSymbol(key: str, id: int) -> bool:
    pass
def GetSectionSymbol(id: int) -> bool:
    pass
def CreateKeyValues(name: str, firstKey: str = ..., firstValue: str = ...) -> Any:
    """Creates a new KeyValues structure.  The Handle must always be closed.

@param name          Name of the root section.
@param firstKey      If non-empty, specifies the first key value.
@param firstValue    If firstKey is non-empty, specifies the first key's value.
@return              A Handle to a new KeyValues structure."""
    pass
def KvSetString(kv: Any, key: str, value: str) -> None:
    """Sets a string value of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         String value.
@error               Invalid Handle."""
    pass
def KvSetNum(kv: Any, key: str, value: int) -> None:
    """Sets an integer value of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         Value number.
@error               Invalid Handle."""
    pass
def KvSetUInt64(kv: Any, key: str, value: list[int]) -> None:
    """Sets a large integer value of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         Large integer value (0=High bits, 1=Low bits)
@error               Invalid Handle."""
    pass
def KvSetFloat(kv: Any, key: str, value: float) -> None:
    """Sets a floating point value of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         Floating point value.
@error               Invalid Handle."""
    pass
def KvSetColor(kv: Any, key: str, r: int, g: int, b: int, a: int = ...) -> None:
    """Sets a set of color values of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param r             Red value.
@param g             Green value.
@param b             Blue value.
@param a             Alpha value.
@error               Invalid Handle."""
    pass
def KvSetVector(kv: Any, key: str, vec: list[float]) -> None:
    """Sets a vector value of a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param vec           Vector value.
@error               Invalid Handle."""
    pass
def KvGetString(kv: Any, key: str, value: str, maxlength: int, defvalue: str = ...) -> None:
    """Retrieves a string value from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         Buffer to store key value in.
@param maxlength     Maximum length of the value buffer.
@param defvalue      Optional default value to use if the key is not found.
@error               Invalid Handle."""
    pass
def KvGetNum(kv: Any, key: str, defvalue: int = ...) -> int:
    """Retrieves an integer value from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param defvalue      Optional default value to use if the key is not found.
@return              Integer value of the key.
@error               Invalid Handle."""
    pass
def KvGetFloat(kv: Any, key: str, defvalue: float = ...) -> float:
    """Retrieves a floating point value from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param defvalue      Optional default value to use if the key is not found.
@return              Floating point value of the key.
@error               Invalid Handle."""
    pass
def KvGetColor(kv: Any, key: str, r: int, g: int, b: int, a: int) -> None:
    """Retrieves a set of color values from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param r             Red value, set by reference.
@param g             Green value, set by reference.
@param b             Blue value, set by reference.
@param a             Alpha value, set by reference.
@error               Invalid Handle."""
    pass
def KvGetUInt64(kv: Any, key: str, value: list[int], defvalue: list[int] = ...) -> None:
    """Retrieves a large integer value from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param value         Array to represent the large integer.
@param defvalue      Optional default value to use if the key is not found.
@error               Invalid Handle."""
    pass
def KvGetVector(kv: Any, key: str, vec: list[float], defvalue: list[float] = ...) -> None:
    """Retrieves a vector value from a KeyValues key.

@param kv            KeyValues Handle.
@param key           Name of the key, or NULL_STRING.
@param vec           Destination vector to store the value in.
@param defvalue      Optional default value to use if the key is not found.
@error               Invalid Handle."""
    pass
def KvJumpToKey(kv: Any, key: str, create: bool = ...) -> bool:
    """Sets the current position in the KeyValues tree to the given key.

@param kv            KeyValues Handle.
@param key           Name of the key.
@param create        If true, and the key does not exist, it will be created.
@return              True if the key exists, false if it does not and was not created."""
    pass
def KvJumpToKeySymbol(kv: Any, id: int) -> bool:
    """Sets the current position in the KeyValues tree to the given key.

@param kv            KeyValues Handle.
@param id            KeyValues id.
@return              True if the key exists, false if it does not."""
    pass
def KvGotoFirstSubKey(kv: Any, keyOnly: bool = ...) -> bool:
    """Sets the current position in the KeyValues tree to the first sub key.
This native adds to the internal traversal stack.

@param kv            KeyValues Handle.
@param keyOnly       If false, non-keys will be traversed (values).
@return              True on success, false if there was no first sub key.
@error               Invalid Handle."""
    pass
def KvGotoNextKey(kv: Any, keyOnly: bool = ...) -> bool:
    """Sets the current position in the KeyValues tree to the next sub key.
This native does NOT add to the internal traversal stack, and thus
KvGoBack() is not needed for each successive call to this function.

@param kv            KeyValues Handle.
@param keyOnly       If false, non-keys will be traversed (values).
@return              True on success, false if there was no next sub key.
@error               Invalid Handle."""
    pass
def KvSavePosition(kv: Any) -> bool:
    """Saves the current position in the traversal stack onto the traversal
stack.  This can be useful if you wish to use KvGotoNextKey() and
have the previous key saved for backwards traversal.

@param kv            KeyValues Handle.
@return              True on success, false if there is no higher node.
@error               Invalid Handle."""
    pass
def KvDeleteKey(kv: Any, key: str) -> bool:
    """Removes the given key from the current position.

@param kv            KeyValues Handle.
@param key           Name of the key.
@return              True on success, false if key did not exist.
@error               Invalid Handle."""
    pass
def KvDeleteThis(kv: Any) -> int:
    """Removes the current sub-key and attempts to set the position
to the sub-key after the removed one.  If no such sub-key exists,
the position will be the parent key in the traversal stack.
Given the sub-key having position "N" in the traversal stack, the
removal will always take place from position "N-1."

@param kv            KeyValues Handle.
@return              1 if removal succeeded and there was another key.
                     0 if the current node was not contained in the
                       previous node, or no previous node exists.
                    -1 if removal succeeded and there were no more keys,
                       thus the state is as if KvGoBack() was called.
@error               Invalid Handle."""
    pass
def KvGoBack(kv: Any) -> bool:
    """Jumps back to the previous position.  Returns false if there are no
previous positions (i.e., at the root node with an empty traversal stack).
This should be called once for each successful Jump call, in order to return
to the top node.  This function pops one node off the internal traversal stack.

@param kv            KeyValues Handle.
@return              True on success, false if there is no higher node.
@error               Invalid Handle."""
    pass
def KvRewind(kv: Any) -> None:
    """Sets the position back to the top node, emptying the entire node
traversal history.  This can be used instead of looping KvGoBack()
if recursive iteration is not important.

@param kv            KeyValues Handle.
@error               Invalid Handle."""
    pass
def KvGetSectionName(kv: Any, section: str, maxlength: int) -> bool:
    """Retrieves the current section name.

@param kv            KeyValues Handle.
@param section       Buffer to store the section name.
@param maxlength     Maximum length of the name buffer.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass
def KvSetSectionName(kv: Any, section: str) -> None:
    """Sets the current section name.

@param kv            KeyValues Handle.
@param section       Section name.
@error               Invalid Handle."""
    pass
def KvGetDataType(kv: Any, key: str) -> KvDataTypes:
    """Returns the data type at a key.

@param kv            KeyValues Handle.
@param key           Key name.
@return              KvDataType value of the key.
@error               Invalid Handle."""
    pass
def KeyValuesToFile(kv: Any, file: str) -> bool:
    """Converts a KeyValues tree to a file.  The tree is dumped
from the current position.

@param kv            KeyValues Handle.
@param file          File to dump write to.
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def FileToKeyValues(kv: Any, file: str) -> bool:
    """Converts a file to a KeyValues tree.  The file is read into
the current position of the tree.

@param kv            KeyValues Handle.
@param file          File to read from.
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def StringToKeyValues(kv: Any, buffer: str, resourceName: str = ...) -> bool:
    """Converts a given string to a KeyValues tree.  The string is read into
the current postion of the tree.

@param kv            KeyValues Handle.
@param buffer        String buffer to load into the KeyValues.
@param resourceName  The resource name of the KeyValues, used for error tracking purposes.
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def KvSetEscapeSequences(kv: Any, useEscapes: bool) -> None:
    """Sets whether or not the KeyValues parser will read escape sequences.
For example, \n would be read as a literal newline.  This defaults
to false for new KeyValues structures.

@param kv            KeyValues Handle.
@param useEscapes    Whether or not to read escape sequences.
@error               Invalid Handle."""
    pass
def KvNodesInStack(kv: Any) -> int:
    """Returns the position in the jump stack; I.e. the number of calls
required for KvGoBack to return to the root node.  If at the root node,
0 is returned.

@param kv            KeyValues Handle.
@return              Number of non-root nodes in the jump stack.
@error               Invalid Handle."""
    pass
def KvCopySubkeys(origin: Any, dest: Any) -> None:
    """Makes a new copy of all subkeys in the origin KeyValues to
the destination KeyValues.
NOTE: All KeyValues are processed from the current location not the root one.

@param origin        Origin KeyValues Handle.
@param dest          Destination KeyValues Handle.
@error               Invalid Handle."""
    pass
def KvFindKeyById(kv: Any, id: int, name: str, maxlength: int) -> bool:
    """Finds a KeyValues name by id.

@param kv            KeyValues Handle.
@param id            KeyValues id.
@param name          Buffer to store the name.
@param maxlength     Maximum length of the value buffer.
@return              True on success, false if id not found.
@error               Invalid Handle."""
    pass
def KvGetNameSymbol(kv: Any, key: str, id: int) -> bool:
    """Finds a KeyValues id inside a KeyValues tree.

@param kv            KeyValues Handle.
@param key           Key name.
@param id            Id of the found KeyValue.
@return              True on success, false if key not found.
@error               Invalid Handle."""
    pass
def KvGetSectionSymbol(kv: Any, id: int) -> bool:
    """Retrieves the current section id.

@param kv            KeyValues Handle.
@param id            Id of the current section.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass