from typing import Any, list, Callable, Union


def RemovePlayerItem(client: int, item: int) -> bool:
    """Removes a player's item.

@param client        Client index.
@param item          CBaseCombatWeapon entity index.
@return              True on success, false otherwise.
@error               Invalid client or entity, lack of mod support, or client not in
                     game."""
    pass
def GivePlayerItem(client: int, item: str, iSubType: int = ...) -> int:
    """Gives a named item to a player.

@param client        Client index.
@param item          Item classname (such as weapon_ak47).
@param iSubType      Unknown.
@return              Entity index on success, or -1 on failure.
@error               Invalid client or client not in game, or lack of mod support."""
    pass
def GetPlayerWeaponSlot(client: int, slot: int) -> int:
    """Returns the weapon in a player's slot.

@param client        Client index.
@param slot          Slot index (mod specific).
@return              Entity index on success, -1 if no weapon existed.
@error               Invalid client or client not in game, or lack of mod support."""
    pass
def IgniteEntity(entity: int, time: float, npc: bool = ..., size: float = ..., level: bool = ...) -> None:
    """Ignites an entity on fire.

@param entity        Entity index.
@param time          Number of seconds to set on fire.
@param npc           True to only affect NPCs.
@param size          Unknown.
@param level         Unknown.
@error               Invalid entity or client not in game, or lack of mod support."""
    pass
def ExtinguishEntity(entity: int) -> None:
    """Extinguishes an entity that is on fire.

@param entity        Entity index.
@error               Invalid entity or client not in game, or lack of mod support."""
    pass
def TeleportEntity(entity: int, origin: list[float] = ..., angles: list[float] = ..., velocity: list[float] = ...) -> None:
    """Teleports an entity.

@param entity        Client index.
@param origin        New origin, or NULL_VECTOR for no change.
@param angles        New angles, or NULL_VECTOR for no change.
@param velocity      New velocity, or NULL_VECTOR for no change.
@error               Invalid entity or client not in game, or lack of mod support."""
    pass
def ForcePlayerSuicide(client: int, explode: bool = ...) -> None:
    """Forces a player to commit suicide.

@param client        Client index.
@param explode       If true, explode the player.
@error               Invalid client or client not in game, or lack of mod support."""
    pass
def SlapPlayer(client: int, health: int = ..., sound: bool = ...) -> None:
    """Slaps a player in a random direction.

@param client        Client index.
@param health        Health to subtract.
@param sound         False to disable the sound effects.
@error               Invalid client or client not in game, or lack of mod support."""
    pass
def FindEntityByClassname(startEnt: int, classname: str) -> int:
    """Searches for an entity by classname.

@param startEnt      A valid entity's index after which to begin searching from.
                     Use -1 to start from the first entity.
@param classname     Classname of the entity to find.
@return              Entity index >= 0 if found, -1 otherwise.
@error               Invalid start entity or lack of mod support."""
    pass
def GetClientEyeAngles(client: int, ang: list[float]) -> bool:
    """Returns the client's eye angles.

@param client        Player's index.
@param ang           Destination vector to store the client's eye angles.
@return              True on success, false on failure.
@error               Invalid client index, client not in game, or lack of mod support."""
    pass
def CreateEntityByName(classname: str, ForceEdictIndex: int = ...) -> int:
    """Creates an entity by string name, but does not spawn it (see DispatchSpawn).
If ForceEdictIndex is not -1, then it will use the edict by that index. If the index is
 invalid or there is already an edict using that index, it will error out.

@param classname         Entity classname.
@param ForceEdictIndex   Edict index used by the created entity (ignored on Orangebox and above).
@return                  Entity index on success, or -1 on failure.
@error                   Invalid edict index, no map is running, or lack of mod support."""
    pass
def DispatchSpawn(entity: int) -> bool:
    """Spawns an entity into the game.

@param entity        Entity index of the created entity.
@return              True on success, false otherwise.
@error               Invalid entity index or lack of mod support."""
    pass
def DispatchKeyValue(entity: int, keyName: str, value: str) -> bool:
    """Dispatches a KeyValue into given entity using a string value.

@param entity        Destination entity index.
@param keyName       Name of the key.
@param value         String value.
@return              True on success, false otherwise.
@error               Invalid entity index or lack of mod support."""
    pass
def DispatchKeyValueInt(entity: int, keyName: str, value: int) -> bool:
    """Dispatches a KeyValue into given entity using an integer value.

@param entity        Destination entity index.
@param keyName       Name of the key.
@param value         Integer value.
@return              True on success, false otherwise.
@error               Invalid entity index or lack of mod support."""
    pass
def DispatchKeyValueFloat(entity: int, keyName: str, value: float) -> bool:
    """Dispatches a KeyValue into given entity using a floating point value.

@param entity        Destination entity index.
@param keyName       Name of the key.
@param value         Floating point value.
@return              True on success, false otherwise.
@error               Invalid entity index or lack of mod support."""
    pass
def DispatchKeyValueVector(entity: int, keyName: str, vec: list[float]) -> bool:
    """Dispatches a KeyValue into given entity using a vector value.

@param entity        Destination entity index.
@param keyName       Name of the key.
@param vec           Vector value.
@return              True on success, false otherwise.
@error               Invalid entity index or lack of mod support."""
    pass
def GetClientAimTarget(client: int, only_clients: bool = ...) -> int:
    """Returns the entity a client is aiming at.

@param client        Client performing the aiming.
@param only_clients  True to exclude all entities but clients.
@return              Entity index being aimed at.
                     -1 if no entity is being aimed at.
                     -2 if the function is not supported.
@error               Invalid client index or client not in game."""
    pass
def GetTeamCount() -> int:
    """Returns the total number of teams in a game.
Note: This native should not be called before OnMapStart.

@return              Total number of teams."""
    pass
def GetTeamName(index: int, name: str, maxlength: int) -> None:
    """Retrieves the team name based on a team index.
Note: This native should not be called before OnMapStart.

@param index         Team index.
@param name          Buffer to store string in.
@param maxlength     Maximum length of string buffer.
@error               Invalid team index."""
    pass
def GetTeamScore(index: int) -> int:
    """Returns the score of a team based on a team index.
Note: This native should not be called before OnMapStart.

@param index         Team index.
@return              Score.
@error               Invalid team index."""
    pass
def SetTeamScore(index: int, value: int) -> None:
    """Sets the score of a team based on a team index.
Note: This native should not be called before OnMapStart.

@param index         Team index.
@param value         New score value.
@error               Invalid team index."""
    pass
def GetTeamClientCount(index: int) -> int:
    """Retrieves the number of players in a certain team.
Note: This native should not be called before OnMapStart.

@param index         Team index.
@return              Number of players in the team.
@error               Invalid team index."""
    pass
def GetTeamEntity(teamIndex: int) -> int:
    """Returns the entity index of a team.

@param teamIndex     Team index.
@return              Entity index of team.
@error               Invalid team index."""
    pass
def SetEntityModel(entity: int, model: str) -> None:
    """Sets the model to a given entity.

@param entity        Entity index.
@param model         Model name.
@error               Invalid entity index or lack of mod support."""
    pass
def GetPlayerDecalFile(client: int, hex: str, maxlength: int) -> bool:
    """Retrieves the decal file name associated with a given client.

@param client        Player's index.
@param hex           Buffer to store the logo filename.
@param maxlength     Maximum length of string buffer.
@return              True on success, otherwise false.
@error               Invalid client or client not in game."""
    pass
def GetPlayerJingleFile(client: int, hex: str, maxlength: int) -> bool:
    """Retrieves the jingle file name associated with a given client.

@param client        Player's index.
@param hex           Buffer to store the jingle filename.
@param maxlength     Maximum length of string buffer.
@return              True on success, otherwise false.
@error               Invalid client or client not in game."""
    pass
def GetServerNetStats(inAmount: float, outAmout: float) -> None:
    """Returns the average server network traffic in bytes/sec.

@param in            Buffer to store the input traffic velocity.
@param out           Buffer to store the output traffic velocity.
@error               Lack of mod support."""
    pass
def EquipPlayerWeapon(client: int, weapon: int) -> None:
    """Equip's a player's weapon.

@param client        Client index.
@param weapon        CBaseCombatWeapon entity index.
@error               Invalid client or entity, lack of mod support, or client not in
                     game."""
    pass
def ActivateEntity(entity: int) -> None:
    """Activates an entity (CBaseAnimating::Activate)

@param entity        Entity index.
@error               Invalid entity or lack of mod support."""
    pass
def SetClientInfo(client: int, key: str, value: str) -> None:
    """Sets values to client info buffer keys and notifies the engine of the change.
The change does not get propagated to mods until the next frame.

@param client        Player's index.
@param key           Key string.
@param value         Value string.
@error               Invalid client index, client not connected, or lack of mod support."""
    pass
def SetClientName(client: int, name: str) -> None:
    """Changes a client's name.

@param client        Player's index.
@param name          New name.
@error               Invalid client index, client not connected, or lack of mod support."""
    pass
def GivePlayerAmmo(client: int, amount: int, ammotype: int, suppressSound: bool = ...) -> int:
    """Gives ammo of a certain type to a player.
This natives obeys the maximum amount of ammo a player can carry per ammo type.

@param client        The client index.
@param amount        Amount of ammo to give. Is capped at ammotype's limit.
@param ammotype      Type of ammo to give to player.
@param suppressSound If true, don't play the ammo pickup sound.
@return              Amount of ammo actually given.
@error               Lack of mod support."""
    pass
def SetEntityCollisionGroup(entity: int, collisionGroup: int) -> None:
    """Changes an entity's collision group (CBaseEntity::SetCollisionGroup).

@param entity            The entity index.
@param collisionGroup    Collision group to use.
@error                   Invalid entity or lack of mod support."""
    pass
def EntityCollisionRulesChanged(entity: int) -> None:
    """Recaculates entity collision rules (CBaseEntity::CollisionRulesChanged).

@param entity            The entity index.
@error                   Invalid entity or lack of mod support."""
    pass
def SetEntityOwner(entity: int, owner: int = ...) -> None:
    """Sets an entity's owner (CBaseEntity::SetEntityOwner).

@param entity            The entity index.
@param owner             The owner entity index, can be invalid.
@error                   Invalid entity or lack of mod support."""
    pass
def LookupEntityAttachment(entity: int, name: str) -> int:
    """Returns the index number of a given named attachment.

@param entity        The entity index.
@param name          The attachment name.
@return              An attachment index, or 0 if the attachment name is invalid or unused.
@error               Invalid entity or lack of mod support."""
    pass
def GetEntityAttachment(entity: int, attachment: int, origin: list[float], angles: list[float]) -> bool:
    """Returns the world location and world angles of an attachment.

@param entity        The entity index.
@param attachment    The attachment index.
@param origin        Destination vector to store the attachment's origin vector.
@param angles        Destination vector to store the attachment's position angle.
@return              True on success, otherwise false.
@error               Invalid entity or lack of mod support."""
    pass