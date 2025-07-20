from typing import Any, list, Callable, Union


def TE_SetupSparks(pos: list[float], dir: list[float], Magnitude: int, TrailLength: int) -> None:
    """Sets up a sparks effect.

@param pos           Position of the sparks.
@param dir           Direction of the sparks.
@param Magnitude     Sparks size.
@param TrailLength   Trail lenght of the sparks."""
    pass
def TE_SetupSmoke(pos: list[float], Model: int, Scale: float, FrameRate: int) -> None:
    """Sets up a smoke effect.

@param pos           Position of the smoke.
@param Model         Precached model index.
@param Scale         Scale of the smoke.
@param FrameRate     Frame rate of the smoke."""
    pass
def TE_SetupDust(pos: list[float], dir: list[float], Size: float, Speed: float) -> None:
    """Sets up a dust cloud effect.

@param pos           Position of the dust.
@param dir           Direction of the dust.
@param Size          Dust cloud size.
@param Speed         Dust cloud speed."""
    pass
def TE_SetupMuzzleFlash(pos: list[float], angles: list[float], Scale: float, Type: int) -> None:
    """Sets up a muzzle flash effect.

@param pos           Position of the muzzle flash.
@param angles        Rotation angles of the muzzle flash.
@param Scale         Scale of the muzzle flash.
@param Type          Muzzle flash type to render (Mod specific)."""
    pass
def TE_SetupMetalSparks(pos: list[float], dir: list[float]) -> None:
    """Sets up a metal sparks effect.

@param pos           Position of the metal sparks.
@param dir           Direction of the metal sparks."""
    pass
def TE_SetupEnergySplash(pos: list[float], dir: list[float], Explosive: bool) -> None:
    """Sets up an energy splash effect.

@param pos           Position of the energy splash.
@param dir           Direction of the energy splash.
@param Explosive     Makes the effect explosive."""
    pass
def TE_SetupArmorRicochet(pos: list[float], dir: list[float]) -> None:
    """Sets up an armor ricochet effect.

@param pos           Position of the armor ricochet.
@param dir           Direction of the armor ricochet."""
    pass
def TE_SetupGlowSprite(pos: list[float], Model: int, Life: float, Size: float, Brightness: int) -> None:
    """Sets up a glowing sprite effect.

@param pos           Position of the sprite.
@param Model         Precached model index.
@param Life          Time duration of the sprite.
@param Size          Sprite size.
@param Brightness    Sprite brightness."""
    pass
def TE_SetupExplosion(pos: list[float], Model: int, Scale: float, Framerate: int, Flags: int, Radius: int, Magnitude: int, normal: list[float] = ..., MaterialType: int = ...) -> None:
    """Sets up a explosion effect.

@param pos           Explosion position.
@param Model         Precached model index.
@param Scale         Explosion scale.
@param Framerate     Explosion frame rate.
@param Flags         Explosion flags.
@param Radius        Explosion radius.
@param Magnitude     Explosion size.
@param normal        Normal vector to the explosion.
@param MaterialType  Exploded material type."""
    pass
def TE_SetupBloodSprite(pos: list[float], dir: list[float], color: list[int], Size: int, SprayModel: int, BloodDropModel: int) -> None:
    """Sets up a blood sprite effect.

@param pos             Position of the sprite.
@param dir             Sprite direction.
@param color           Color array (r, g, b, a).
@param Size            Sprite size.
@param SprayModel      Precached model index.
@param BloodDropModel  Precached model index."""
    pass
def TE_SetupBeamRingPoint(center: list[float], Start_Radius: float, End_Radius: float, ModelIndex: int, HaloIndex: int, StartFrame: int, FrameRate: int, Life: float, Width: float, Amplitude: float, Color: list[int], Speed: int, Flags: int) -> None:
    """Sets up a beam ring point effect.

@param center        Center position of the ring.
@param Start_Radius  Initial ring radius.
@param End_Radius    Final ring radius.
@param ModelIndex    Precached model index.
@param HaloIndex     Precached model index.
@param StartFrame    Initial frame to render.
@param FrameRate     Ring frame rate.
@param Life          Time duration of the ring.
@param Width         Beam width.
@param Amplitude     Beam amplitude.
@param Color         Color array (r, g, b, a).
@param Speed         Speed of the beam.
@param Flags         Beam flags."""
    pass
def TE_SetupBeamPoints(start: list[float], end: list[float], ModelIndex: int, HaloIndex: int, StartFrame: int, FrameRate: int, Life: float, Width: float, EndWidth: float, FadeLength: int, Amplitude: float, Color: list[int], Speed: int) -> None:
    """Sets up a point to point beam effect.

@param start         Start position of the beam.
@param end           End position of the beam.
@param ModelIndex    Precached model index.
@param HaloIndex     Precached model index.
@param StartFrame    Initial frame to render.
@param FrameRate     Beam frame rate.
@param Life          Time duration of the beam.
@param Width         Initial beam width.
@param EndWidth      Final beam width.
@param FadeLength    Beam fade time duration.
@param Amplitude     Beam amplitude.
@param Color         Color array (r, g, b, a).
@param Speed         Speed of the beam."""
    pass
def TE_SetupBeamLaser(StartEntity: int, EndEntity: int, ModelIndex: int, HaloIndex: int, StartFrame: int, FrameRate: int, Life: float, Width: float, EndWidth: float, FadeLength: int, Amplitude: float, Color: list[int], Speed: int) -> None:
    """Sets up an entity to entity laser effect.

@param StartEntity   Entity index from where the beam starts.
@param EndEntity     Entity index from where the beam ends.
@param ModelIndex    Precached model index.
@param HaloIndex     Precached model index.
@param StartFrame    Initial frame to render.
@param FrameRate     Beam frame rate.
@param Life          Time duration of the beam.
@param Width         Initial beam width.
@param EndWidth      Final beam width.
@param FadeLength    Beam fade time duration.
@param Amplitude     Beam amplitude.
@param Color         Color array (r, g, b, a).
@param Speed         Speed of the beam."""
    pass
def TE_SetupBeamRing(StartEntity: int, EndEntity: int, ModelIndex: int, HaloIndex: int, StartFrame: int, FrameRate: int, Life: float, Width: float, Amplitude: float, Color: list[int], Speed: int, Flags: int) -> None:
    """Sets up a beam ring effect.

@param StartEntity   Entity index from where the ring starts.
@param EndEntity     Entity index from where the ring ends.
@param ModelIndex    Precached model index.
@param HaloIndex     Precached model index.
@param StartFrame    Initial frame to render.
@param FrameRate     Ring frame rate.
@param Life          Time duration of the ring.
@param Width         Beam width.
@param Amplitude     Beam amplitude.
@param Color         Color array (r, g, b, a).
@param Speed         Speed of the beam.
@param Flags         Beam flags."""
    pass
def TE_SetupBeamFollow(EntIndex: int, ModelIndex: int, HaloIndex: int, Life: float, Width: float, EndWidth: float, FadeLength: int, Color: list[int]) -> None:
    """Sets up a follow beam effect.

@param EntIndex      Entity index from where the beam starts.
@param ModelIndex    Precached model index.
@param HaloIndex     Precached model index.
@param Life          Time duration of the beam.
@param Width         Initial beam width.
@param EndWidth      Final beam width.
@param FadeLength    Beam fade time duration.
@param Color         Color array (r, g, b, a)."""
    pass
TE_EXPLFLAG_NONE: Any = ...  # 0x0   /**< all flags clear makes default Half-Life explosion */
TE_EXPLFLAG_NOADDITIVE: Any = ...  # 0x1   /**< sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite) */
TE_EXPLFLAG_NODLIGHTS: Any = ...  # 0x2   /**< do not render dynamic lights */
TE_EXPLFLAG_NOSOUND: Any = ...  # 0x4   /**< do not play client explosion sound */
TE_EXPLFLAG_NOPARTICLES: Any = ...  # 0x8   /**< do not draw particles */
TE_EXPLFLAG_DRAWALPHA: Any = ...  # 0x10  /**< sprite will be drawn alpha */
TE_EXPLFLAG_ROTATE: Any = ...  # 0x20  /**< rotate the sprite randomly */
TE_EXPLFLAG_NOFIREBALL: Any = ...  # 0x40  /**< do not draw a fireball */
TE_EXPLFLAG_NOFIREBALLSMOKE: Any = ...  # 0x80  /**< do not draw smoke with the fireball */
FBEAM_STARTENTITY: Any = ...  # 0x00000001
FBEAM_ENDENTITY: Any = ...  # 0x00000002
FBEAM_FADEIN: Any = ...  # 0x00000004
FBEAM_FADEOUT: Any = ...  # 0x00000008
FBEAM_SINENOISE: Any = ...  # 0x00000010
FBEAM_SOLID: Any = ...  # 0x00000020
FBEAM_SHADEIN: Any = ...  # 0x00000040
FBEAM_SHADEOUT: Any = ...  # 0x00000080
FBEAM_ONLYNOISEONCE: Any = ...  # 0x00000100  /**< Only calculate our noise once */
FBEAM_NOTILE: Any = ...  # 0x00000200
FBEAM_USE_HITBOXES: Any = ...  # 0x00000400  /**< Attachment indices represent hitbox indices instead when this is set. */
FBEAM_STARTVISIBLE: Any = ...  # 0x00000800  /**< Has this client actually seen this beam's start entity yet? */
FBEAM_ENDVISIBLE: Any = ...  # 0x00001000  /**< Has this client actually seen this beam's end entity yet? */
FBEAM_ISACTIVE: Any = ...  # 0x00002000
FBEAM_FOREVER: Any = ...  # 0x00004000
FBEAM_HALOBEAM: Any = ...  # 0x00008000  /**< When drawing a beam with a halo, don't ignore the segments and endwidth */