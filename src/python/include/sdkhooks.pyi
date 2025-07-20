from typing import Any, list, Callable, Union
from .core import *


class SDKHookType:
    SDKHook_Blocked: int = ...
    SDKHook_BlockedPost: int = ...
    SDKHook_CanBeAutobalanced: int = ...
    SDKHook_EndTouch: int = ...
    SDKHook_EndTouchPost: int = ...
    SDKHook_FireBulletsPost: int = ...
    SDKHook_GetMaxHealth: int = ...
    SDKHook_GroundEntChangedPost: int = ...
    SDKHook_OnTakeDamage: int = ...
    SDKHook_OnTakeDamageAlive: int = ...
    SDKHook_OnTakeDamageAlivePost: int = ...
    SDKHook_OnTakeDamagePost: int = ...
    SDKHook_PostThink: int = ...
    SDKHook_PostThinkPost: int = ...
    SDKHook_PreThink: int = ...
    SDKHook_PreThinkPost: int = ...
    SDKHook_Reload: int = ...
    SDKHook_ReloadPost: int = ...
    SDKHook_SetTransmit: int = ...
    SDKHook_ShouldCollide: int = ...
    SDKHook_Spawn: int = ...
    SDKHook_SpawnPost: int = ...
    SDKHook_StartTouch: int = ...
    SDKHook_StartTouchPost: int = ...
    SDKHook_Think: int = ...
    SDKHook_ThinkPost: int = ...
    SDKHook_Touch: int = ...
    SDKHook_TouchPost: int = ...
    SDKHook_TraceAttack: int = ...
    SDKHook_TraceAttackPost: int = ...
    SDKHook_Use: int = ...
    SDKHook_UsePost: int = ...
    SDKHook_VPhysicsUpdate: int = ...
    SDKHook_VPhysicsUpdatePost: int = ...
    SDKHook_WeaponCanSwitchTo: int = ...
    SDKHook_WeaponCanSwitchToPost: int = ...
    SDKHook_WeaponCanUse: int = ...
    SDKHook_WeaponCanUsePost: int = ...
    SDKHook_WeaponDrop: int = ...
    SDKHook_WeaponDropPost: int = ...
    SDKHook_WeaponEquip: int = ...
    SDKHook_WeaponEquipPost: int = ...
    SDKHook_WeaponSwitch: int = ...
    SDKHook_WeaponSwitchPost: int = ...


class UseType:
    Use_Off: int = ...
    Use_On: int = ...
    Use_Set: int = ...
    Use_Toggle: int = ...


SDKHookCB = Union[
    Callable[[int], None],
    Callable[[int], Any],
    Callable[[int], None],
    Callable[[int, int], Any],
    Callable[[int, int], None],
    Callable[[int, int], Any],
    Callable[[int, int], Any],
    Callable[[int, int], None],
    Callable[[int, int], Any],
    Callable[[int, int, int, float, int], Any],
    Callable[[int, int, int, float, int, int, list[float], list[float]], Any],
    Callable[[int, int, int, float, int], None],
    Callable[[int, int, int, float, int, int, list[float], list[float]], None],
    Callable[[int, int, str], None],
    Callable[[int, int, int, float, int, int, int, int], Any],
    Callable[[int, int, int, float, int, int, int, int], None],
    Callable[[int, int, int, bool], bool],
    Callable[[int, int, int, UseType, float], Any],
    Callable[[int, int, int, UseType, float], None],
    Callable[[int], Any],
    Callable[[int, bool], None],
    Callable[[int, bool], bool]
]
def OnEntityCreated(entity: int, classname: str) -> None:
    """When an entity is created

@param entity        Entity index
@param classname     Class name"""
    pass
def OnEntityDestroyed(entity: int) -> None:
    """When an entity is destroyed

@param entity        Entity index or edict reference."""
    pass
def OnGetGameDescription(gameDesc: str) -> Any:
    """When the game description is retrieved

@note Not supported on ep2v.

@param gameDesc      Game description
@return              Plugin_Changed if gameDesc has been edited, else no change."""
    pass
def OnLevelInit(mapName: str, mapEntities: str) -> Any:
    pass
def SDKHook(entity: int, type: SDKHookType, callback: SDKHookCB) -> None:
    """Hooks an entity

Unhooked automatically upon destruction/removal of the entity

@param entity        Entity index
@param type          Type of function to hook
@param callback      Function to call when hook is called"""
    pass
def SDKHookEx(entity: int, type: SDKHookType, callback: SDKHookCB) -> bool:
    """Hooks an entity

Unhooked automatically upon destruction/removal of the entity

@param entity        Entity index
@param type          Type of function to hook
@param callback      Function to call when hook is called
@return              Hook Successful"""
    pass
def SDKUnhook(entity: int, type: SDKHookType, callback: SDKHookCB) -> None:
    """Unhooks an entity

@param entity   Entity index
@param type     Type of function to unhook
@param callback Callback function to unhook"""
    pass
def SDKHooks_TakeDamage(entity: int, inflictor: int, attacker: int, damage: float, damageType: int = ..., weapon: int = ..., damageForce: list[float] = ..., damagePosition: list[float] = ..., bypassHooks: bool = ...) -> None:
    """Applies damage to an entity

@note Force application is dependent on game and damage type(s)

@param entity         Entity index taking damage
@param inflictor      Inflictor entity index
@param attacker       Attacker entity index
@param damage         Amount of damage
@param damageType     Bitfield of damage types
@param weapon         Weapon index (orangebox and later) or -1 for unspecified
@param damageForce    Velocity of damage force
@param damagePosition Origin of damage
@param bypassHooks    If true, bypass SDK hooks on OnTakeDamage
@error                Invalid entity, attacker, inflictor, or weapon entity."""
    pass
def SDKHooks_DropWeapon(client: int, weapon: int, vecTarget: list[float] = ..., vecVelocity: list[float] = ..., bypassHooks: bool = ...) -> None:
    """Forces a client to drop the specified weapon

@param client        Client index.
@param weapon        Weapon entity index.
@param vecTarget     Location to toss weapon to, or NULL_VECTOR for default.
@param vecVelocity   Velocity at which to toss weapon, or NULL_VECTOR for default.
@param bypassHooks   If true, bypass SDK hooks on Weapon Drop
@error               Invalid client or weapon entity, weapon not owned by client."""
    pass
DMG_GENERIC: Any = ...  # 0          /**< generic damage was done */
DMG_CRUSH: Any = ...  # (1 << 0)    /**< crushed by falling or moving object.
DMG_BULLET: Any = ...  # (1 << 1)    /**< shot */
DMG_SLASH: Any = ...  # (1 << 2)    /**< cut, clawed, stabbed */
DMG_BURN: Any = ...  # (1 << 3)    /**< heat burned */
DMG_VEHICLE: Any = ...  # (1 << 4)    /**< hit by a vehicle */
DMG_FALL: Any = ...  # (1 << 5)    /**< fell too far */
DMG_BLAST: Any = ...  # (1 << 6)    /**< explosive blast damage */
DMG_CLUB: Any = ...  # (1 << 7)    /**< crowbar, punch, headbutt */
DMG_SHOCK: Any = ...  # (1 << 8)    /**< electric shock */
DMG_SONIC: Any = ...  # (1 << 9)    /**< sound pulse shockwave */
DMG_ENERGYBEAM: Any = ...  # (1 << 10)   /**< laser or other high energy beam  */
DMG_PREVENT_PHYSICS_FORCE: Any = ...  # (1 << 11)   /**< Prevent a physics force  */
DMG_NEVERGIB: Any = ...  # (1 << 12)   /**< with this bit OR'd in, no damage type will be able to gib victims upon death */
DMG_ALWAYSGIB: Any = ...  # (1 << 13)   /**< with this bit OR'd in, any damage type can be made to gib victims upon death. */
DMG_DROWN: Any = ...  # (1 << 14)   /**< Drowning */
DMG_PARALYZE: Any = ...  # (1 << 15)   /**< slows affected creature down */
DMG_NERVEGAS: Any = ...  # (1 << 16)   /**< nerve toxins, very bad */
DMG_POISON: Any = ...  # (1 << 17)   /**< blood poisoning - heals over time like drowning damage */
DMG_RADIATION: Any = ...  # (1 << 18)   /**< radiation exposure */
DMG_DROWNRECOVER: Any = ...  # (1 << 19)   /**< drowning recovery */
DMG_ACID: Any = ...  # (1 << 20)   /**< toxic chemicals or acid burns */
DMG_SLOWBURN: Any = ...  # (1 << 21)   /**< in an oven */
DMG_REMOVENORAGDOLL: Any = ...  # (1 << 22)   /**< with this bit OR'd in, no ragdoll will be created, and the target will be quietly removed.
DMG_PHYSGUN: Any = ...  # (1 << 23)   /**< Hit by manipulator. Usually doesn't do any damage. */
DMG_PLASMA: Any = ...  # (1 << 24)   /**< Shot by Cremator */
DMG_AIRBOAT: Any = ...  # (1 << 25)   /**< Hit by the airboat's gun */
DMG_DISSOLVE: Any = ...  # (1 << 26)   /**< Dissolving! */
DMG_BLAST_SURFACE: Any = ...  # (1 << 27)   /**< A blast on the surface of water that cannot harm things underwater */
DMG_DIRECT: Any = ...  # (1 << 28)
DMG_BUCKSHOT: Any = ...  # (1 << 29)   /**< not quite a bullet. Little, rounder, different. */
DMG_CRIT: Any = ...  # DMG_ACID        /**< TF2 crits and minicrits */
DMG_RADIUS_MAX: Any = ...  # DMG_ENERGYBEAM  /**< No damage falloff */
DMG_NOCLOSEDISTANCEMOD: Any = ...  # DMG_POISON      /**< Don't do damage falloff too close */
DMG_HALF_FALLOFF: Any = ...  # DMG_RADIATION   /**< 50% damage falloff */
DMG_USEDISTANCEMOD: Any = ...  # DMG_SLOWBURN    /**< Do damage falloff */
DMG_IGNITE: Any = ...  # DMG_PLASMA      /**< Ignite victim */
DMG_USE_HITLOCATIONS: Any = ...  # DMG_AIRBOAT     /**< Do hit location damage (Like the sniperrifle and ambassador) */
bypassHooks: bool = ...
vecVelocity: float = ...
__ext_sdkhooks: Extension = ...