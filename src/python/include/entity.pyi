from typing import Any, list, Callable, Union
from .sourcemod import *


class PropFieldType:
    PropField_Entity: int = ...
    PropField_Float: int = ...
    PropField_Integer: int = ...
    PropField_String: int = ...
    PropField_String_T: int = ...
    PropField_Unsupported: int = ...
    PropField_Variant: int = ...
    PropField_Vector: int = ...


class PropType:
    """Property types for entities."""
    Prop_Data: int = ...
    Prop_Send: int = ...


def GetMaxEntities() -> int:
    """Returns the maximum number of networked entities.

Note: For legacy reasons, this only returns the maximum
networked entities (maximum edicts), rather than total
maximum entities.

@return              Maximum number of networked entities."""
    pass
def GetEntityCount() -> int:
    """Returns the number of networked entities in the server.

Note: For legacy reasons, this only returns the current count
of networked entities (current edicts), rather than total
count of current entities.

@return              Number of entities in the server."""
    pass
def IsValidEntity(entity: int) -> bool:
    """Returns whether or not an entity is valid.  Returns false
if there is no matching CBaseEntity for this entity index.

@param entity        Index of the entity.
@return              True if valid, false otherwise."""
    pass
def IsValidEdict(edict: int) -> bool:
    """Returns whether or not an edict index is valid.

@param edict         Index of the edict.
@return              True if valid, false otherwise."""
    pass
def IsEntNetworkable(entity: int) -> bool:
    """Returns whether or not an entity has a valid networkable edict.

@param entity        Index of the entity.
@return              True if networkable, false if invalid or not networkable."""
    pass
def CreateEdict() -> int:
    """Creates a new edict (the basis of a networkable entity)

@return              Index of the edict, 0 on failure."""
    pass
def RemoveEdict(edict: int) -> None:
    """Removes an edict from the world.

@param edict         Index of the edict.
@error               Invalid edict index."""
    pass
def RemoveEntity(entity: int) -> None:
    """Marks an entity for deletion.

@param entity        Index of the entity.
@error               Invalid entity index."""
    pass
def GetEdictFlags(edict: int) -> int:
    """Returns the flags on an edict.  These are not the same as entity flags.

@param edict         Index of the entity.
@return              Edict flags.
@error               Invalid edict index."""
    pass
def SetEdictFlags(edict: int, flags: int) -> None:
    """Sets the flags on an edict.  These are not the same as entity flags.

@param edict         Index of the entity.
@param flags         Flags to set.
@error               Invalid edict index."""
    pass
def GetEdictClassname(edict: int, clsname: str, maxlength: int) -> bool:
    """Retrieves an edict classname.

@param edict         Index of the entity.
@param clsname       Buffer to store the classname.
@param maxlength     Maximum length of the buffer.
@return              True on success, false if there is no classname set."""
    pass
def GetEntityNetClass(edict: int, clsname: str, maxlength: int) -> bool:
    """Retrieves an entity's networkable serverclass name.
This is not the same as the classname and is used for networkable state changes.

@param edict         Index of the entity.
@param clsname       Buffer to store the serverclass name.
@param maxlength     Maximum length of the buffer.
@return              True on success, false if the edict is not networkable.
@error               Invalid edict index."""
    pass
def ChangeEdictState(edict: int, offset: int = ...) -> None:
    """Marks an entity as state changed.  This can be useful if you set an offset
and wish for it to be immediately changed over the network.  By default this
is not done for offset setting functions.

@param edict         Index to the edict.
@param offset        Offset to mark as changed.  If 0,
                     the entire edict is marked as changed.
@error               Invalid entity or offset out of bounds."""
    pass
def GetEntData(entity: int, offset: int, size: int = ...) -> int:
    """Peeks into an entity's object data and retrieves the integer value at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@param size          Number of bytes to read (valid values are 1, 2, or 4).
@return              Value at the given memory location.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntData(entity: int, offset: int, value: Any, size: int = ..., changeState: bool = ...) -> None:
    """Peeks into an entity's object data and sets the integer value at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@param value         Value to set.
@param size          Number of bytes to write (valid values are 1, 2, or 4).
@param changeState   If true, change will be sent over the network.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntDataFloat(entity: int, offset: int) -> float:
    """Peeks into an entity's object data and retrieves the float value at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@return              Value at the given memory location.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntDataFloat(entity: int, offset: int, value: float, changeState: bool = ...) -> None:
    """Peeks into an entity's object data and sets the float value at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@param value         Value to set.
@param changeState   If true, change will be sent over the network.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntDataEnt(entity: int, offset: int) -> int:
    pass
def SetEntDataEnt(entity: int, offset: int, other: int, changeState: bool = ...) -> None:
    pass
def GetEntDataEnt2(entity: int, offset: int) -> int:
    """Peeks into an entity's object data and retrieves the entity index
at the given offset.

Note: This will only work on offsets that are stored as "entity
handles" (which usually looks like m_h* in properties).  These
are not SourceMod Handles, but internal Source structures.

@param entity        Edict index.
@param offset        Offset to use.
@return              Entity index at the given location.  If there is no entity,
                     or the stored entity is invalid, then -1 is returned.
@error               Invalid input entity, or offset out of reasonable bounds."""
    pass
def SetEntDataEnt2(entity: int, offset: int, other: int, changeState: bool = ...) -> None:
    """Peeks into an entity's object data and sets the entity index at the
given offset.

Note: This will only work on offsets that are stored as "entity
handles" (which usually looks like m_h* in properties).  These
are not SourceMod Handles, but internal Source structures.

@param entity        Edict index.
@param offset        Offset to use.
@param other         Entity index to set, or -1 to clear.
@param changeState   If true, change will be sent over the network.
@error               Invalid input entity, or offset out of reasonable bounds."""
    pass
def GetEntDataVector(entity: int, offset: int, vec: list[float]) -> None:
    """Peeks into an entity's object data and retrieves the vector at the
given offset.
@note Both a Vector and a QAngle are three floats.  This is a
      convenience function and will work with both types.

@param entity        Edict index.
@param offset        Offset to use.
@param vec           Vector buffer to store data in.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntDataVector(entity: int, offset: int, vec: list[float], changeState: bool = ...) -> None:
    """Peeks into an entity's object data and sets the vector at the given
offset.
@note Both a Vector and a QAngle are three floats.  This is a
      convenience function and will work with both types.

@param entity        Edict index.
@param offset        Offset to use.
@param vec           Vector to set.
@param changeState   If true, change will be sent over the network.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntDataString(entity: int, offset: int, buffer: str, maxlen: int) -> int:
    """Peeks into an entity's object data and retrieves the string at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@param buffer        Destination string buffer.
@param maxlen        Maximum length of output string buffer.
@return              Number of non-null bytes written.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntDataString(entity: int, offset: int, buffer: str, maxlen: int, changeState: bool = ...) -> int:
    """Peeks into an entity's object data and sets the string at
the given offset.

@param entity        Edict index.
@param offset        Offset to use.
@param buffer        String to set.
@param maxlen        Maximum length of bytes to write.
@param changeState   If true, change will be sent over the network.
@return              Number of non-null bytes written.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def FindSendPropOffs(cls: str, prop: str) -> int:
    pass
def FindSendPropInfo(cls: str, prop: str, type: PropFieldType = ..., num_bits: int = ..., local_offset: int = ..., array_size: int = ...) -> int:
    """Given a ServerClass name, finds a networkable send property offset.
This information is cached for future calls.

@param cls           Classname.
@param prop          Property name.
@param type          Optional parameter to store the type.
@param num_bits      Optional parameter to store the number of bits the field
                     uses, if applicable (otherwise 0 is stored).  The number
                     of bits varies for integers and floats, and is always 0
                     for strings.
@param local_offset  Optional parameter to store the local offset, as
                     FindSendPropOffs() would return.
@param array_size    Optional parameter to store array size, 0 if not an array.
@return              On success, returns an absolutely computed offset.
                     If no offset is available, 0 is returned.
                     If the property is not found, -1 is returned."""
    pass
def FindDataMapOffs(entity: int, prop: str, type: PropFieldType = ..., num_bits: int = ...) -> int:
    pass
def FindDataMapInfo(entity: int, prop: str, type: PropFieldType = ..., num_bits: int = ..., local_offset: int = ...) -> int:
    """Given an entity, finds a nested datamap property offset.
This information is cached for future calls.

@param entity        Entity index.
@param prop          Property name.
@param type          Optional parameter to store the type.
@param num_bits      Optional parameter to store the number of bits the field
                     uses.  The bit count will either be 1 (for boolean) or
                     divisible by 8 (including 0 if unknown).
@param local_offset  Optional parameter to store the local offset, as
                     FindDataMapOffs() would return.
@return              An offset, or -1 on failure."""
    pass
def GetEntSendPropOffs(ent: int, prop: str, actual: bool = ...) -> int:
    """Wrapper function for finding a send property for a particular entity.

@param ent           Entity index.
@param prop          Property name.
@param actual        Defaults to false for backwards compatibility.
                     If true, the newer FindSendPropInfo() function
                     is used instead.
@return              An offset, or -1 on failure."""
    pass
def HasEntProp(entity: int, type: PropType, prop: str) -> bool:
    """Checks if an entity property exists on an entity.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@return              Whether the property exists on the entity.
@error               Invalid entity."""
    pass
def GetEntProp(entity: int, type: PropType, prop: str, size: int = ..., element: int = ...) -> int:
    """Retrieves an integer value from an entity's property.

This function is considered safer and more robust over GetEntData,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param size          Number of bytes to write (valid values are 1, 2, or 4).
                     This value is auto-detected, and the size parameter is
                     only used as a fallback in case detection fails.
@param element       Element # (starting from 0) if property is an array.
@return              Value at the given property offset.
@error               Invalid entity or property not found."""
    pass
def SetEntProp(entity: int, type: PropType, prop: str, value: Any, size: int = ..., element: int = ...) -> None:
    """Sets an integer value in an entity's property.

This function is considered safer and more robust over SetEntData,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param value         Value to set.
@param size          Number of bytes to write (valid values are 1, 2, or 4).
                     This value is auto-detected, and the size parameter is
                     only used as a fallback in case detection fails.
@param element       Element # (starting from 0) if property is an array.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntPropFloat(entity: int, type: PropType, prop: str, element: int = ...) -> float:
    """Retrieves a float value from an entity's property.

This function is considered safer and more robust over GetEntDataFloat,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param element       Element # (starting from 0) if property is an array.
@return              Value at the given property offset.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntPropFloat(entity: int, type: PropType, prop: str, value: float, element: int = ...) -> None:
    """Sets a float value in an entity's property.

This function is considered safer and more robust over SetEntDataFloat,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param value         Value to set.
@param element       Element # (starting from 0) if property is an array.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntPropEnt(entity: int, type: PropType, prop: str, element: int = ...) -> int:
    """Retrieves an entity index from an entity's property.

This function is considered safer and more robust over GetEntDataEnt*,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param element       Element # (starting from 0) if property is an array.
@return              Entity index at the given property.
                     If there is no entity, or the entity is not valid,
                     then -1 is returned.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntPropEnt(entity: int, type: PropType, prop: str, other: int, element: int = ...) -> None:
    """Sets an entity index in an entity's property.

This function is considered safer and more robust over SetEntDataEnt*,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param other         Entity index to set, or -1 to unset.
@param element       Element # (starting from 0) if property is an array.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntPropVector(entity: int, type: PropType, prop: str, vec: list[float], element: int = ...) -> None:
    """Retrieves a vector of floats from an entity, given a named network property.

This function is considered safer and more robust over GetEntDataVector,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param vec           Vector buffer to store data in.
@param element       Element # (starting from 0) if property is an array.
@error               Invalid entity, property not found, or property not
                     actually a vector data type."""
    pass
def SetEntPropVector(entity: int, type: PropType, prop: str, vec: list[float], element: int = ...) -> None:
    """Sets a vector of floats in an entity, given a named network property.

This function is considered safer and more robust over SetEntDataVector,
because it performs strict offset checking and typing rules.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@param vec           Vector to set.
@param element       Element # (starting from 0) if property is an array.
@error               Invalid entity, property not found, or property not
                     actually a vector data type."""
    pass
def GetEntPropString(entity: int, type: PropType, prop: str, buffer: str, maxlen: int, element: int = ...) -> int:
    """Gets a network property as a string.

@param entity        Edict index.
@param type          Property type.
@param prop          Property to use.
@param buffer        Destination string buffer.
@param maxlen        Maximum length of output string buffer.
@param element       Element # (starting from 0) if property is an array.
@return              Number of non-null bytes written.
@error               Invalid entity, offset out of reasonable bounds, or property is not a valid string."""
    pass
def SetEntPropString(entity: int, type: PropType, prop: str, buffer: str, element: int = ...) -> int:
    """Sets a network property as a string.

@param entity        Edict index.
@param type          Property type.
@param prop          Property to use.
@param buffer        String to set.
@param element       Element # (starting from 0) if property is an array.
@return              Number of non-null bytes written.
@error               Invalid entity, offset out of reasonable bounds, or property is not a valid string."""
    pass
def GetEntPropArraySize(entity: int, type: PropType, prop: str) -> int:
    """Retrieves the count of values that an entity property's array can store.

@param entity        Entity/edict index.
@param type          Property type.
@param prop          Property name.
@return              Size of array (in elements) or 0 if property is not an array.
@error               Invalid entity or property not found."""
    pass
def GetEntDataArray(entity: int, offset: int, array: list[Any], arraySize: int, dataSize: int = ...) -> None:
    """Copies an array of cells from an entity at a given offset.

@param entity        Entity index.
@param offset        Offset to use.
@param array         Array to read into.
@param arraySize     Number of values to read.
@param dataSize      Size of each value in bytes (1, 2, or 4).
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def SetEntDataArray(entity: int, offset: int, array: list[Any], arraySize: int, dataSize: int = ..., changeState: bool = ...) -> None:
    """Copies an array of cells to an entity at a given offset.

@param entity        Entity index.
@param offset        Offset to use.
@param array         Array of values to copy.
@param arraySize     Number of values to copy.
@param dataSize      Size of each value in bytes (1, 2, or 4).
@param changeState   True to set the network state as changed; false otherwise.
@error               Invalid entity or offset out of reasonable bounds."""
    pass
def GetEntityAddress(entity: int) -> Address:
    """Gets the memory address of an entity.

@param entity        Entity index.
@return              Address of the entity.
@error               Invalid entity."""
    pass
def GetEntityClassname(entity: int, clsname: str, maxlength: int) -> bool:
    """Retrieves the classname of an entity.
This is like GetEdictClassname(), except it works for ALL
entities, not just edicts.

@param entity        Index of the entity.
@param clsname       Buffer to store the classname.
@param maxlength     Maximum length of the buffer.
@return              True on success, false if there is no classname set."""
    pass
def LoadEntityFromHandleAddress(addr: Address) -> int:
    """Interprets the address as an entity handle and returns the associated entity.

@param addr          Address to a memory location.
@return              Entity index at the given location.  If there is no entity, or the stored entity is invalid, then -1 is returned."""
    pass
def StoreEntityToHandleAddress(addr: Address, entity: int) -> None:
    """Interprets the address as an entity handle and sets the entity.

@param addr          Address to a memory location.
@param entity        Entity index to set, or -1 to clear."""
    pass
FL_EDICT_CHANGED: Any = ...  # (1<<0)  /**< Game DLL sets this when the entity state changes
FL_EDICT_FREE: Any = ...  # (1<<1)  /**< this edict if free for reuse */
FL_EDICT_FULL: Any = ...  # (1<<2)  /**< this is a full server entity */
FL_EDICT_FULLCHECK: Any = ...  # (0<<0)  /**< call ShouldTransmit() each time, this is a fake flag */
FL_EDICT_ALWAYS: Any = ...  # (1<<3)  /**< always transmit this entity */
FL_EDICT_DONTSEND: Any = ...  # (1<<4)  /**< don't transmit this entity */
FL_EDICT_PVSCHECK: Any = ...  # (1<<5)  /**< always transmit entity, but cull against PVS */
FL_EDICT_PENDING_DORMANT_CHECK: Any = ...  # (1<<6)
FL_EDICT_DIRTY_PVS_INFORMATION: Any = ...  # (1<<7)
FL_FULL_EDICT_CHANGED: Any = ...  # (1<<8)
local: int = ...
offset: int = ...