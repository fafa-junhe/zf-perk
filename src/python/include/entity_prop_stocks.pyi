from typing import Any, list, Callable, Union


class MoveType:
    MOVETYPE_CUSTOM: int = ...
    MOVETYPE_FLY: int = ...
    MOVETYPE_FLYGRAVITY: int = ...
    MOVETYPE_ISOMETRIC: int = ...
    MOVETYPE_LADDER: int = ...
    MOVETYPE_NOCLIP: int = ...
    MOVETYPE_NONE: int = ...
    MOVETYPE_OBSERVER: int = ...
    MOVETYPE_PUSH: int = ...
    MOVETYPE_STEP: int = ...
    MOVETYPE_VPHYSICS: int = ...
    MOVETYPE_WALK: int = ...


class RenderFx:
    RENDERFX_CLAMP_MIN_SCALE: int = ...
    RENDERFX_DISTORT: int = ...
    RENDERFX_ENV_RAIN: int = ...
    RENDERFX_ENV_SNOW: int = ...
    RENDERFX_EXPLODE: int = ...
    RENDERFX_FADE_FAST: int = ...
    RENDERFX_FADE_SLOW: int = ...
    RENDERFX_FLICKER_FAST: int = ...
    RENDERFX_FLICKER_SLOW: int = ...
    RENDERFX_GLOWSHELL: int = ...
    RENDERFX_HOLOGRAM: int = ...
    RENDERFX_MAX: int = ...
    RENDERFX_NONE: int = ...
    RENDERFX_NO_DISSIPATION: int = ...
    RENDERFX_PULSE_FAST: int = ...
    RENDERFX_PULSE_FAST_WIDE: int = ...
    RENDERFX_PULSE_FAST_WIDER: int = ...
    RENDERFX_PULSE_SLOW: int = ...
    RENDERFX_PULSE_SLOW_WIDE: int = ...
    RENDERFX_RAGDOLL: int = ...
    RENDERFX_SOLID_FAST: int = ...
    RENDERFX_SOLID_SLOW: int = ...
    RENDERFX_SPOTLIGHT: int = ...
    RENDERFX_STROBE_FAST: int = ...
    RENDERFX_STROBE_FASTER: int = ...
    RENDERFX_STROBE_SLOW: int = ...


class RenderMode:
    RENDER_ENVIRONMENTAL: int = ...
    RENDER_GLOW: int = ...
    RENDER_NONE: int = ...
    RENDER_NORMAL: int = ...
    RENDER_TRANSADD: int = ...
    RENDER_TRANSADDFRAMEBLEND: int = ...
    RENDER_TRANSALPHA: int = ...
    RENDER_TRANSALPHAADD: int = ...
    RENDER_TRANSCOLOR: int = ...
    RENDER_TRANSTEXTURE: int = ...
    RENDER_WORLDGLOW: int = ...


def GetEntityFlags(entity: int) -> int:
    """Get an entity's flags.

@note The game's actual flags are internally translated by SM
      to match the entity flags defined above as the actual values
      can differ per engine.

@param entity        Entity index.
@return              Entity's flags, see entity flag defines above.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityFlags(entity: int, flags: int) -> None:
    """Sets an entity's flags.

@note The entity flags as defined above are internally translated by SM
      to match the current game's expected value for the flags as
      the actual values can differ per engine.

@param entity        Entity index.
@param flags         Entity flags, see entity flag defines above.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetEntityMoveType(entity: int) -> MoveType:
    """Gets an entity's movetype.

@param entity        Entity index.
@return              Movetype, see enum above.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityMoveType(entity: int, mt: MoveType) -> None:
    """Sets an entity's movetype.

@param entity        Entity index.
@param mt            Movetype, see enum above.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetEntityRenderMode(entity: int) -> RenderMode:
    """Gets an entity's render mode.

@param entity        Entity index.
@return              RenderMode value.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityRenderMode(entity: int, mode: RenderMode) -> None:
    """Sets an entity's render mode.

@param entity        Entity index.
@param mode          RenderMode value.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetEntityRenderFx(entity: int) -> RenderFx:
    """Gets an entity's render Fx.

@param entity        Entity index.
@return              RenderFx value.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityRenderFx(entity: int, fx: RenderFx) -> None:
    """Sets an entity's render Fx.

@param entity        Entity index.
@param fx            RenderFx value.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetEntityRenderColor(entity: int, r: int, g: int, b: int, a: int) -> None:
    """Gets an entity's color.

@param entity        Entity index.
@param r             Amount of red (0-255)
@param g             Amount of green (0-255)
@param b             Amount of blue (0-255)
@param a             Amount of alpha (0-255)
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityRenderColor(entity: int, r: int = ..., g: int = ..., b: int = ..., a: int = ...) -> None:
    """Sets an entity's color.

@param entity        Entity index
@param r             Amount of red (0-255)
@param g             Amount of green (0-255)
@param b             Amount of blue (0-255)
@param a             Amount of alpha (0-255)
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetEntityGravity(entity: int) -> float:
    """Gets an entity's gravity.

@param entity 	Entity index.
@return              Entity's m_flGravity value.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityGravity(entity: int, amount: float) -> None:
    """Sets an entity's gravity.

@param entity        Entity index.
@param amount        Gravity to set (default = 1.0, half = 0.5, double = 2.0).
@error               Invalid entity index, or lack of mod compliance."""
    pass
def SetEntityHealth(entity: int, amount: int) -> None:
    """Sets an entity's health

@param entity        Entity index.
@param amount        Health amount.
@error               Invalid entity index, or lack of mod compliance."""
    pass
def GetClientButtons(client: int) -> int:
    """Get's a users current pressed buttons

@param client        Client index
@return              Bitsum of buttons
@error               Invalid client index, client not in game,
                     or lack of mod compliance."""
    pass
IN_ATTACK: Any = ...  # (1 << 0)
IN_JUMP: Any = ...  # (1 << 1)
IN_DUCK: Any = ...  # (1 << 2)
IN_FORWARD: Any = ...  # (1 << 3)
IN_BACK: Any = ...  # (1 << 4)
IN_USE: Any = ...  # (1 << 5)
IN_CANCEL: Any = ...  # (1 << 6)
IN_LEFT: Any = ...  # (1 << 7)
IN_RIGHT: Any = ...  # (1 << 8)
IN_MOVELEFT: Any = ...  # (1 << 9)
IN_MOVERIGHT: Any = ...  # (1 << 10)
IN_ATTACK2: Any = ...  # (1 << 11)
IN_RUN: Any = ...  # (1 << 12)
IN_RELOAD: Any = ...  # (1 << 13)
IN_ALT1: Any = ...  # (1 << 14)
IN_ALT2: Any = ...  # (1 << 15)
IN_SCORE: Any = ...  # (1 << 16)   /**< Used by client.dll for when scoreboard is held down */
IN_SPEED: Any = ...  # (1 << 17)   /**< Player is holding the speed key */
IN_WALK: Any = ...  # (1 << 18)   /**< Player holding walk key */
IN_ZOOM: Any = ...  # (1 << 19)   /**< Zoom key for HUD zoom */
IN_WEAPON1: Any = ...  # (1 << 20)   /**< weapon defines these bits */
IN_WEAPON2: Any = ...  # (1 << 21)   /**< weapon defines these bits */
IN_BULLRUSH: Any = ...  # (1 << 22)
IN_GRENADE1: Any = ...  # (1 << 23)   /**< grenade 1 */
IN_GRENADE2: Any = ...  # (1 << 24)   /**< grenade 2 */
IN_ATTACK3: Any = ...  # (1 << 25)
FL_ONGROUND: Any = ...  # (1 << 0)   /**< At rest / on the ground */
FL_DUCKING: Any = ...  # (1 << 1)   /**< Player flag -- Player is fully crouched */
FL_WATERJUMP: Any = ...  # (1 << 2)   /**< player jumping out of water */
FL_ONTRAIN: Any = ...  # (1 << 3)   /**< Player is _controlling_ a train, so movement commands should be ignored on client during prediction. */
FL_INRAIN: Any = ...  # (1 << 4)   /**< Indicates the entity is standing in rain */
FL_FROZEN: Any = ...  # (1 << 5)   /**< Player is frozen for 3rd person camera */
FL_ATCONTROLS: Any = ...  # (1 << 6)   /**< Player can't move, but keeps key inputs for controlling another entity */
FL_CLIENT: Any = ...  # (1 << 7)   /**< Is a player */
FL_FAKECLIENT: Any = ...  # (1 << 8)   /**< Fake client, simulated server side; don't send network messages to them */
PLAYER_FLAG_BITS: Any = ...  # 9
FL_INWATER: Any = ...  # (1 << 9)   /**< In water */
FL_FLY: Any = ...  # (1 << 10)  /**< Changes the SV_Movestep() behavior to not need to be on ground */
FL_SWIM: Any = ...  # (1 << 11)  /**< Changes the SV_Movestep() behavior to not need to be on ground (but stay in water) */
FL_CONVEYOR: Any = ...  # (1 << 12)
FL_NPC: Any = ...  # (1 << 13)
FL_GODMODE: Any = ...  # (1 << 14)
FL_NOTARGET: Any = ...  # (1 << 15)
FL_AIMTARGET: Any = ...  # (1 << 16)  /**< set if the crosshair needs to aim onto the entity */
FL_PARTIALGROUND: Any = ...  # (1 << 17)  /**< not all corners are valid */
FL_STATICPROP: Any = ...  # (1 << 18)  /**< Eetsa static prop!  */
FL_GRAPHED: Any = ...  # (1 << 19)  /**< worldgraph has this ent listed as something that blocks a connection */
FL_GRENADE: Any = ...  # (1 << 20)
FL_STEPMOVEMENT: Any = ...  # (1 << 21)  /**< Changes the SV_Movestep() behavior to not do any processing */
FL_DONTTOUCH: Any = ...  # (1 << 22)  /**< Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set */
FL_BASEVELOCITY: Any = ...  # (1 << 23)  /**< Base velocity has been applied this frame (used to convert base velocity into momentum) */
FL_WORLDBRUSH: Any = ...  # (1 << 24)  /**< Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something) */
FL_OBJECT: Any = ...  # (1 << 25)  /**< Terrible name. This is an object that NPCs should see. Missiles, for example. */
FL_KILLME: Any = ...  # (1 << 26)  /**< This entity is marked for death -- will be freed by game DLL */
FL_ONFIRE: Any = ...  # (1 << 27)  /**< You know... */
FL_DISSOLVING: Any = ...  # (1 << 28)  /**< We're dissolving! */
FL_TRANSRAGDOLL: Any = ...  # (1 << 29)  /**< In the process of turning into a client side ragdoll. */
FL_UNBLOCKABLE_BY_PLAYER: Any = ...  # (1 << 30)  /**< pusher that can't be blocked by the player */
FL_FREEZING: Any = ...  # (1 << 31)  /**< We're becoming frozen! */
FL_EP2V_UNKNOWN1: Any = ...  # (1 << 31)  /**< Unknown */
gc: Any = ...
exists: bool = ...
offset: int = ...