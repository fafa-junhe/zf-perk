from typing import Any, list, Callable, Union


class AdmAccessMode:
    """Methods of computing access permissions."""
    Access_Effective: int = ...
    Access_Real: int = ...


class AdminCachePart:
    """Represents the various cache regions."""
    AdminCache_Admins: int = ...
    AdminCache_Groups: int = ...
    AdminCache_Overrides: int = ...


class AdminFlag:
    """Access levels (flags) for admins."""
    Admin_Ban: int = ...
    Admin_Changemap: int = ...
    Admin_Chat: int = ...
    Admin_Cheats: int = ...
    Admin_Config: int = ...
    Admin_Convars: int = ...
    Admin_Custom1: int = ...
    Admin_Custom2: int = ...
    Admin_Custom3: int = ...
    Admin_Custom4: int = ...
    Admin_Custom5: int = ...
    Admin_Custom6: int = ...
    Admin_Generic: int = ...
    Admin_Kick: int = ...
    Admin_Password: int = ...
    Admin_RCON: int = ...
    Admin_Reservation: int = ...
    Admin_Root: int = ...
    Admin_Slay: int = ...
    Admin_Unban: int = ...
    Admin_Vote: int = ...


class AdminId:
    """Identifies a unique entry in the admin permissions cache.  These are not Handles."""
    INVALID_ADMIN_ID: int = ...


class GroupId:
    """Identifies a unique entry in the group permissions cache.  These are not Handles."""
    INVALID_GROUP_ID: int = ...


class ImmunityType:
    """DEPRECATED, do not use."""
    Immunity_Default: int = ...
    Immunity_Global: int = ...


class OverrideRule:
    """Access override rules."""
    Command_Allow: int = ...
    Command_Deny: int = ...


class OverrideType:
    """Access override types."""
    Override_Command: int = ...
    Override_CommandGroup: int = ...


def GetUsername(name: str, maxlength: int) -> None:
    pass
def BindIdentity(authMethod: str, ident: str) -> bool:
    pass
def SetFlag(flag: AdminFlag, enabled: bool) -> None:
    pass
def SetBitFlags(flags: int, enabled: bool) -> None:
    pass
def HasFlag(flag: AdminFlag, mode: AdmAccessMode = ...) -> bool:
    pass
def GetFlags(mode: AdmAccessMode) -> int:
    pass
def InheritGroup(gid: GroupId) -> bool:
    pass
def GetGroup(index: int, name: str, maxlength: int) -> GroupId:
    pass
def SetPassword(password: str) -> None:
    pass
def GetPassword(buffer: str = ..., maxlength: int = ...) -> bool:
    pass
def CanTarget(other: AdminId) -> bool:
    pass
def get() -> Any:
    pass
def set(level: int) -> Any:
    pass
def GetGroupImmunity(index: int) -> GroupId:
    pass
def AddGroupImmunity(other: GroupId) -> None:
    pass
def GetCommandOverride(name: str, type: OverrideType, rule: OverrideRule) -> bool:
    pass
def AddCommandOverride(name: str, type: OverrideType, rule: OverrideRule) -> None:
    pass
def OnRebuildAdminCache(part: AdminCachePart) -> None:
    """Called when part of the cache needs to be rebuilt.

@param part          Part of the admin cache to rebuild."""
    pass
def DumpAdminCache(part: AdminCachePart, rebuild: bool) -> None:
    """Tells the admin system to dump a portion of the cache.

@param part          Part of the cache to dump.  Specifying groups also dumps admins.
@param rebuild       If true, the rebuild forwards will fire."""
    pass
def UnsetCommandOverride(cmd: str, type: OverrideType) -> None:
    """Unsets a command override.

@param cmd           String containing command name (case sensitive).
@param type          Override type (specific command or group)."""
    pass
def CreateAdmGroup(group_name: str) -> GroupId:
    """Adds a new group.  Name must be unique.

@param group_name    String containing the group name.
@return              A new group id, INVALID_GROUP_ID if it already exists."""
    pass
def FindAdmGroup(group_name: str) -> GroupId:
    """Finds a group by name.

@param group_name    String containing the group name.
@return              A group id, or INVALID_GROUP_ID if not found."""
    pass
def SetAdmGroupAddFlag(id: GroupId, flag: AdminFlag, enabled: bool) -> None:
    """Adds or removes a flag from a group's flag set.
@note These are called "add flags" because they add to a user's flags.

@param id            Group id.
@param flag          Admin flag to toggle.
@param enabled       True to set the flag, false to unset/disable."""
    pass
def GetAdmGroupAddFlag(id: GroupId, flag: AdminFlag) -> bool:
    """Gets the set value of an add flag on a group's flag set.
@note These are called "add flags" because they add to a user's flags.

@param id            Group id.
@param flag          Admin flag to retrieve.
@return              True if enabled, false otherwise,"""
    pass
def GetAdmGroupAddFlags(id: GroupId) -> int:
    """Returns the flag set that is added to a user from their group.
@note These are called "add flags" because they add to a user's flags.

@param id            GroupId of the group.
@return              Bitstring containing the flags enabled."""
    pass
def SetAdmGroupImmunity(id: GroupId, type: ImmunityType, enabled: bool) -> None:
    pass
def GetAdmGroupImmunity(id: GroupId, type: ImmunityType) -> bool:
    pass
def SetAdmGroupImmuneFrom(id: GroupId, other_id: GroupId) -> None:
    """Adds immunity to a specific group.

@param id            Group id.
@param other_id      Group id to receive immunity to."""
    pass
def GetAdmGroupImmuneCount(id: GroupId) -> int:
    """Returns the number of specific group immunities.

@param id            Group id.
@return              Number of group immunities."""
    pass
def GetAdmGroupImmuneFrom(id: GroupId, number: int) -> GroupId:
    """Returns a group that this group is immune to given an index.

@param id            Group id.
@param number        Index from 0 to N-1, from GetAdmGroupImmuneCount().
@return              GroupId that this group is immune to, or INVALID_GROUP_ID on failure."""
    pass
def AddAdmGroupCmdOverride(id: GroupId, name: str, type: OverrideType, rule: OverrideRule) -> None:
    """Adds a group-specific override type.

@param id            Group id.
@param name          String containing command name (case sensitive).
@param type          Override type (specific command or group).
@param rule          Override allow/deny setting."""
    pass
def GetAdmGroupCmdOverride(id: GroupId, name: str, type: OverrideType, rule: OverrideRule) -> bool:
    """Retrieves a group-specific command override.

@param id            Group id.
@param name          String containing command name (case sensitive).
@param type          Override type (specific command or group).
@param rule          Optional pointer to store allow/deny setting.
@return              True if an override exists, false otherwise."""
    pass
def RegisterAuthIdentType(name: str) -> None:
    """Registers an authentication identity type.  You normally never need to call this except for
very specific systems.

@param name          Codename to use for your authentication type."""
    pass
def CreateAdmin(name: str = ...) -> AdminId:
    """Creates a new admin entry in the permissions cache and returns the generated AdminId index.

@param name          Name for this entry (does not have to be unique).
                     Specify an empty string for an anonymous admin.
@return              New AdminId index or INVALID_ADMIN_ID if name is empty"""
    pass
def GetAdminUsername(id: AdminId, name: str, maxlength: int) -> int:
    """Retrieves an admin's user name as made with CreateAdmin().

@note This function can return UTF-8 strings, and will safely chop UTF-8 strings.

@param id            AdminId of the admin.
@param name          String buffer to store name.
@param maxlength     Maximum size of string buffer.
@return              Number of bytes written."""
    pass
def BindAdminIdentity(id: AdminId, auth: str, ident: str) -> bool:
    """Binds an admin to an identity for fast lookup later on.  The bind must be unique.

@param id            AdminId of the admin.
@param auth          Auth method to use, predefined or from RegisterAuthIdentType().
@param ident         String containing the arbitrary, unique identity.
@return              True on success, false if the auth method was not found,
                     ident was already taken, or ident invalid for auth method."""
    pass
def SetAdminFlag(id: AdminId, flag: AdminFlag, enabled: bool) -> None:
    """Sets whether or not a flag is enabled on an admin.

@param id            AdminId index of the admin.
@param flag          Admin flag to use.
@param enabled       True to enable, false to disable."""
    pass
def GetAdminFlag(id: AdminId, flag: AdminFlag, mode: AdmAccessMode = ...) -> bool:
    """Returns whether or not a flag is enabled on an admin.

@param id            AdminId index of the admin.
@param flag          Admin flag to use.
@param mode          Access mode to check.
@return              True if enabled, false otherwise."""
    pass
def GetAdminFlags(id: AdminId, mode: AdmAccessMode) -> int:
    """Returns the bitstring of access flags on an admin.

@param id            AdminId index of the admin.
@param mode          Access mode to use.
@return              A bitstring containing which flags are enabled."""
    pass
def AdminInheritGroup(id: AdminId, gid: GroupId) -> bool:
    """Adds a group to an admin's inherited group list.  Any flags the group has
will be added to the admin's effective flags.

@param id            AdminId index of the admin.
@param gid           GroupId index of the group.
@return              True on success, false on invalid input or duplicate membership."""
    pass
def GetAdminGroupCount(id: AdminId) -> int:
    """Returns the number of groups this admin is a member of.

@param id            AdminId index of the admin.
@return              Number of groups this admin is a member of."""
    pass
def GetAdminGroup(id: AdminId, index: int, name: str, maxlength: int) -> GroupId:
    """Returns group information from an admin.

@param id            AdminId index of the admin.
@param index         Group number to retrieve, from 0 to N-1, where N
                     is the value of GetAdminGroupCount(id).
@param name          Buffer to store the group's name.
                     Note: This will safely chop UTF-8 strings.
@param maxlength     Maximum size of the output name buffer.
@return              A GroupId index and a name pointer, or
                     INVALID_GROUP_ID and NULL if an error occurred."""
    pass
def SetAdminPassword(id: AdminId, password: str) -> None:
    """Sets a password on an admin.

@param id            AdminId index of the admin.
@param password      String containing the password."""
    pass
def GetAdminPassword(id: AdminId, buffer: str = ..., maxlength: int = ...) -> bool:
    """Gets an admin's password.

@param id            AdminId index of the admin.
@param buffer        Optional buffer to store the admin's password.
@param maxlength     Maximum size of the output name buffer.
                     Note: This will safely chop UTF-8 strings.
@return              True if there was a password set, false otherwise."""
    pass
def FindAdminByIdentity(auth: str, identity: str) -> AdminId:
    """Attempts to find an admin by an auth method and an identity.

@param auth          Auth method to try.
@param identity      Identity string to look up.
@return              An AdminId index if found, INVALID_ADMIN_ID otherwise."""
    pass
def RemoveAdmin(id: AdminId) -> bool:
    """Removes an admin entry from the cache.

@note This will remove any bindings to a specific user.

@param id            AdminId index to remove/invalidate.
@return              True on success, false otherwise."""
    pass
def FlagBitsToBitArray(bits: int, array: list[bool], maxSize: int) -> int:
    """Converts a flag bit string to a bit array.

@param bits          Bit string containing the flags.
@param array         Array to write the flags to.  Enabled flags will be 'true'.
@param maxSize       Maximum number of flags the array can store.
@return              Number of flags written."""
    pass
def FlagBitArrayToBits(array: list[bool], maxSize: int) -> int:
    """Converts a flag array to a bit string.

@param array         Array containing true or false for each AdminFlag.
@param maxSize       Maximum size of the flag array.
@return              A bit string composed of the array bits."""
    pass
def FlagArrayToBits(array: list[AdminFlag], numFlags: int) -> int:
    """Converts an array of flags to bits.

@param array         Array containing flags that are enabled.
@param numFlags      Number of flags in the array.
@return              A bit string composed of the array flags."""
    pass
def FlagBitsToArray(bits: int, array: list[AdminFlag], maxSize: int) -> int:
    """Converts a bit string to an array of flags.

@param bits          Bit string containing the flags.
@param array         Output array to write flags.
@param maxSize       Maximum size of the flag array.
@return              Number of flags written."""
    pass
def FindFlagByName(name: str, flag: AdminFlag) -> bool:
    """Finds a flag by its string name.

@param name          Flag name (like "kick"), case sensitive.
@param flag          Variable to store flag in.
@return              True on success, false if not found."""
    pass
def FindFlagByChar(c: int, flag: AdminFlag) -> bool:
    """Finds a flag by a given character.

@param c             Flag ASCII character/token.
@param flag          Variable to store flag in.
@return              True on success, false if not found."""
    pass
def FindFlagChar(flag: AdminFlag, c: int) -> bool:
    """Finds the flag char for a given admin flag.

@param flag          Flag to look up.
@param c             Variable to store flag char.
@return              True on success, false if not found."""
    pass
def ReadFlagString(flags: str, numchars: int = ...) -> int:
    """Converts a string of flag characters to a bit string.

@param flags         Flag ASCII string.
@param numchars      Optional variable to store the number of bytes read.
@return              Bit string of ADMFLAG values."""
    pass
def CanAdminTarget(admin: AdminId, target: AdminId) -> bool:
    """Tests whether one admin can target another.

The heuristics for this check are as follows:
0. If the targeting AdminId is INVALID_ADMIN_ID, targeting fails.
1. If the targeted AdminId is INVALID_ADMIN_ID, targeting succeeds.
2. If the targeted AdminId is the same as the targeting AdminId,
   (self) targeting succeeds.
3. If the targeting admin is root, targeting succeeds.
4. If the targeted admin has access higher (as interpreted by
   (sm_immunity_mode) than the targeting admin, then targeting fails.
5. If the targeted admin has specific immunity from the
   targeting admin via group immunities, targeting fails.
6. Targeting succeeds.

@param admin         Admin doing the targetting (may be INVALID_ADMIN_ID).
@param target        Target admin (may be INVALID_ADMIN_ID).
@return              True if targetable, false if immune."""
    pass
def CreateAuthMethod(method: str) -> bool:
    """Creates an admin auth method.  This does not need to be called more than once
per method, ever.

@param method        Name of the authentication method.
@return              True on success, false on failure."""
    pass
def SetAdmGroupImmunityLevel(gid: GroupId, level: int) -> int:
    """Sets a group's immunity level.

@param gid           Group Id.
@param level         Immunity level value.
@return              Old immunity level value."""
    pass
def GetAdmGroupImmunityLevel(gid: GroupId) -> int:
    """Gets a group's immunity level (defaults to 0).

@param gid           Group Id.
@return              Immunity level value."""
    pass
def SetAdminImmunityLevel(id: AdminId, level: int) -> int:
    """Sets an admin's immunity level.

@param id            Admin Id.
@param level         Immunity level value.
@return              Old immunity level value."""
    pass
def GetAdminImmunityLevel(id: AdminId) -> int:
    """Gets an admin's immunity level.

@param id            Admin Id.
@return              Immunity level value."""
    pass
def FlagToBit(flag: AdminFlag) -> int:
    """Converts a flag to its single bit.

@param flag          Flag to convert.
@return              Bit representation of the flag."""
    pass
def BitToFlag(bit: int, flag: AdminFlag) -> bool:
    """Converts a bit to an AdminFlag.

@param bit           Bit to convert.
@param flag          Stores the converted flag by reference.
@return              True on success, false otherwise."""
    pass
def FlagBitsToString(bits: int, flags: str, maxSize: int) -> int:
    """Converts a bit string to a string of flag characters.

@param bits			Bit string containing the flags.
@param flags			Output array to write a string of flag characters.
@param maxSize		Maximum size of the string array.
@return				Number of flag characters written."""
    pass
ADMFLAG_RESERVATION: Any = ...  # (1<<0)      /**< Convenience macro for Admin_Reservation as a FlagBit */
ADMFLAG_GENERIC: Any = ...  # (1<<1)      /**< Convenience macro for Admin_Generic as a FlagBit */
ADMFLAG_KICK: Any = ...  # (1<<2)      /**< Convenience macro for Admin_Kick as a FlagBit */
ADMFLAG_BAN: Any = ...  # (1<<3)      /**< Convenience macro for Admin_Ban as a FlagBit */
ADMFLAG_UNBAN: Any = ...  # (1<<4)      /**< Convenience macro for Admin_Unban as a FlagBit */
ADMFLAG_SLAY: Any = ...  # (1<<5)      /**< Convenience macro for Admin_Slay as a FlagBit */
ADMFLAG_CHANGEMAP: Any = ...  # (1<<6)      /**< Convenience macro for Admin_Changemap as a FlagBit */
ADMFLAG_CONVARS: Any = ...  # (1<<7)      /**< Convenience macro for Admin_Convars as a FlagBit */
ADMFLAG_CONFIG: Any = ...  # (1<<8)      /**< Convenience macro for Admin_Config as a FlagBit */
ADMFLAG_CHAT: Any = ...  # (1<<9)      /**< Convenience macro for Admin_Chat as a FlagBit */
ADMFLAG_VOTE: Any = ...  # (1<<10)     /**< Convenience macro for Admin_Vote as a FlagBit */
ADMFLAG_PASSWORD: Any = ...  # (1<<11)     /**< Convenience macro for Admin_Password as a FlagBit */
ADMFLAG_RCON: Any = ...  # (1<<12)     /**< Convenience macro for Admin_RCON as a FlagBit */
ADMFLAG_CHEATS: Any = ...  # (1<<13)     /**< Convenience macro for Admin_Cheats as a FlagBit */
ADMFLAG_ROOT: Any = ...  # (1<<14)     /**< Convenience macro for Admin_Root as a FlagBit */
ADMFLAG_CUSTOM1: Any = ...  # (1<<15)     /**< Convenience macro for Admin_Custom1 as a FlagBit */
ADMFLAG_CUSTOM2: Any = ...  # (1<<16)     /**< Convenience macro for Admin_Custom2 as a FlagBit */
ADMFLAG_CUSTOM3: Any = ...  # (1<<17)     /**< Convenience macro for Admin_Custom3 as a FlagBit */
ADMFLAG_CUSTOM4: Any = ...  # (1<<18)     /**< Convenience macro for Admin_Custom4 as a FlagBit */
ADMFLAG_CUSTOM5: Any = ...  # (1<<19)     /**< Convenience macro for Admin_Custom5 as a FlagBit */
ADMFLAG_CUSTOM6: Any = ...  # (1<<20)     /**< Convenience macro for Admin_Custom6 as a FlagBit */
AUTHMETHOD_STEAM: Any = ...  # "steam"     /**< SteamID based authentication */
AUTHMETHOD_IP: Any = ...  # "ip"        /**< IP based authentication */
AUTHMETHOD_NAME: Any = ...  # "name"      /**< Name based authentication */
num: int = ...
numFlags: int = ...