from typing import Any, list, Callable, Union
from .handles import *


class RayType:
    """@endsection"""
    RayType_EndPoint: int = ...
    RayType_Infinite: int = ...


class TraceType:
    TRACE_ENTITIES_ONLY: int = ...
    TRACE_EVERYTHING: int = ...
    TRACE_EVERYTHING_FILTER_PROPS: int = ...
    TRACE_WORLD_ONLY: int = ...


"""Accepts one of the following function signatures:
- Called on entity filtering.

@param entity        Entity index.
@param contentsMask  Contents Mask.
@return              True to allow the current entity to be hit, otherwise false.
- Called on entity filtering.

@param entity        Entity index.
@param contentsMask  Contents Mask.
@param data          Data value, if used.
@return              True to allow the current entity to be hit, otherwise false."""
TraceEntityFilter = Union[
    Callable[[int, int], bool],
    Callable[[int, int, Any], bool]
]
def TR_GetPointContents(pos: list[float], entindex: int = ...) -> int:
    """Get the contents mask and the entity index at the given position.

@param pos           World position to test.
@param entindex      Entity index found at the given position (by reference).
@return              Contents mask."""
    pass
def TR_GetPointContentsEnt(entindex: int, pos: list[float]) -> int:
    """Get the point contents testing only the given entity index.

@param entindex      Entity index to test.
@param pos           World position.
@return              Contents mask.
@error               Invalid entity."""
    pass
def TR_TraceRay(pos: list[float], vec: list[float], flags: int, rtype: RayType) -> None:
    """Starts up a new trace ray using a global trace result.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the
                     ending point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction."""
    pass
def TR_TraceHull(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int) -> None:
    """Starts up a new trace hull using a global trace result.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags."""
    pass
def TR_EnumerateEntities(pos: list[float], vec: list[float], mask: int, rtype: RayType, enumerator: TraceEntityEnumerator, data: Any = ...) -> None:
    """Enumerates over entities along a ray. This may find entities that are
close to the ray but do not actually intersect it. Use TR_Clip*RayToEntity
with TR_DidHit to check if the ray actually intersects the entity.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param mask          Mask to use for the trace. See PARTITION_* flags.
@param rtype         Method to calculate the ray direction.
@param enumerator    Function to use as enumerator. For each entity found
                     along the ray, this function is called.
@param data          Arbitrary data value to pass through to the enumerator."""
    pass
def TR_EnumerateEntitiesHull(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], mask: int, enumerator: TraceEntityEnumerator, data: Any = ...) -> None:
    """Enumerates over entities along a ray hull. This may find entities that are
close to the ray but do not actually intersect it. Use TR_Clip*RayToEntity
with TR_DidHit to check if the ray actually intersects the entity.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param mask          Mask to use for the trace. See PARTITION_* flags.
@param enumerator    Function to use as enumerator. For each entity found
                     along the ray, this function is called.
@param data          Arbitrary data value to pass through to the enumerator."""
    pass
def TR_EnumerateEntitiesSphere(pos: list[float], radius: float, mask: int, enumerator: TraceEntityEnumerator, data: Any = ...) -> None:
    """Enumerates over entities in a sphere.

@param pos           Starting position of the ray.
@param radius        Radius of the ray.
@param mask          Mask to use for the trace. See PARTITION_* flags.
@param enumerator    Function to use as enumerator. For each entity found
                     along the ray, this function is called.
@param data          Arbitrary data value to pass through to the enumerator."""
    pass
def TR_EnumerateEntitiesBox(mins: list[float], maxs: list[float], mask: int, enumerator: TraceEntityEnumerator, data: Any = ...) -> None:
    """Enumerates over entities in a box.

@param mins          Box minimum size.
@param maxs          Box maximum size.
@param mask          Mask to use for the trace. See PARTITION_* flags.
@param enumerator    Function to use as enumerator. For each entity found
                     along the box, this function is called.
@param data          Arbitrary data value to pass through to the enumerator."""
    pass
def TR_EnumerateEntitiesPoint(pos: list[float], mask: int, enumerator: TraceEntityEnumerator, data: Any = ...) -> None:
    """Enumerates over entities at point.

@param pos           Position of the point.
@param mask          Mask to use for the trace. See PARTITION_* flags.
@param enumerator    Function to use as enumerator. For each entity found
                     along the point, this function is called.
@param data          Arbitrary data value to pass through to the enumerator."""
    pass
def TR_TraceRayFilter(pos: list[float], vec: list[float], flags: int, rtype: RayType, filter: TraceEntityFilter, data: Any = ..., traceType: TraceType = ...) -> None:
    """Starts up a new trace ray using a global trace result and a customized
trace ray filter.

Calling TR_Trace*Filter or TR_Trace*FilterEx from inside a filter
function is currently not allowed and may not work.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction.
@param filter        Function to use as a filter.
@param data          Arbitrary data value to pass through to the filter
                     function.
@param traceType     Trace type."""
    pass
def TR_TraceHullFilter(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int, filter: TraceEntityFilter, data: Any = ..., traceType: TraceType = ...) -> None:
    """Starts up a new trace hull using a global trace result and a customized
trace ray filter.

Calling TR_Trace*Filter or TR_Trace*FilterEx from inside a filter
function is currently not allowed and may not work.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags.
@param filter        Function to use as a filter.
@param data          Arbitrary data value to pass through to the filter
                     function.
@param traceType     Trace type."""
    pass
def TR_ClipRayToEntity(pos: list[float], vec: list[float], flags: int, rtype: RayType, entity: int) -> None:
    """Clips a ray to a particular entity.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction.
@param entity        Entity to clip to.
@error               Invalid entity."""
    pass
def TR_ClipRayHullToEntity(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int, entity: int) -> None:
    """Clips a ray hull to a particular entity.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags.
@param entity        Entity to clip to.
@error               Invalid entity."""
    pass
def TR_ClipCurrentRayToEntity(flags: int, entity: int) -> None:
    """Clips the current global ray (or hull) to a particular entity.

@param flags         Trace flags.
@param entity        Entity to clip to.
@error               Invalid entity."""
    pass
def TR_TraceRayEx(pos: list[float], vec: list[float], flags: int, rtype: RayType) -> Any:
    """Starts up a new trace ray using a new trace result.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction.
@return              Ray trace handle, which must be closed via CloseHandle()."""
    pass
def TR_TraceHullEx(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int) -> Any:
    """Starts up a new trace hull using a new trace result.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags.
@return              Ray trace handle, which must be closed via CloseHandle()."""
    pass
def TR_TraceRayFilterEx(pos: list[float], vec: list[float], flags: int, rtype: RayType, filter: TraceEntityFilter, data: Any = ..., traceType: TraceType = ...) -> Any:
    """Starts up a new trace ray using a new trace result and a customized
trace ray filter.

Calling TR_Trace*Filter or TR_TraceRay*Ex from inside a filter
function is currently not allowed and may not work.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction.
@param filter        Function to use as a filter.
@param data          Arbitrary data value to pass through to the filter function.
@param traceType     Trace type.
@return              Ray trace handle, which must be closed via CloseHandle()."""
    pass
def TR_TraceHullFilterEx(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int, filter: TraceEntityFilter, data: Any = ..., traceType: TraceType = ...) -> Any:
    """Starts up a new trace hull using a new trace result and a customized
trace ray filter.

Calling TR_Trace*Filter or TR_Trace*FilterEx from inside a filter
function is currently not allowed and may not work.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags.
@param filter        Function to use as a filter.
@param data          Arbitrary data value to pass through to the filter function.
@param traceType     Trace type.
@return              Ray trace handle, which must be closed via CloseHandle()."""
    pass
def TR_ClipRayToEntityEx(pos: list[float], vec: list[float], flags: int, rtype: RayType, entity: int) -> Any:
    """Clips a ray to a particular entity.

@param pos           Starting position of the ray.
@param vec           Depending on RayType, it will be used as the ending
                     point, or the direction angle.
@param flags         Trace flags.
@param rtype         Method to calculate the ray direction.
@param entity        Entity to clip to.
@return              Ray trace handle, which must be closed via CloseHandle().
@error               Invalid entity."""
    pass
def TR_ClipRayHullToEntityEx(pos: list[float], vec: list[float], mins: list[float], maxs: list[float], flags: int, entity: int) -> Any:
    """Clips a ray hull to a particular entity.

@param pos           Starting position of the ray.
@param vec           Ending position of the ray.
@param mins          Hull minimum size.
@param maxs          Hull maximum size.
@param flags         Trace flags.
@param entity        Entity to clip to.
@return              Ray trace handle, which must be closed via CloseHandle().
@error               Invalid entity."""
    pass
def TR_ClipCurrentRayToEntityEx(flags: int, entity: int) -> Any:
    """Clips the current global ray (or hull) to a particular entity.

@param flags         Trace flags.
@param entity        Entity to clip to.
@return              Ray trace handle, which must be closed via CloseHandle().
@error               Invalid entity."""
    pass
def TR_GetFraction(hndl: Any = ...) -> float:
    """Returns the time fraction from a trace result (1.0 means no collision).

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Time fraction value of the trace.
@error               Invalid Handle."""
    pass
def TR_GetFractionLeftSolid(hndl: Any = ...) -> float:
    """Returns the time fraction from a trace result when it left a solid.
Only valid if trace started in solid

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Time fraction left solid value of the trace.
@error               Invalid Handle."""
    pass
def TR_GetStartPosition(hndl: Any, pos: list[float]) -> None:
    """Returns the starting position of a trace.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@param pos           Vector buffer to store data in.
@error               Invalid Handle."""
    pass
def TR_GetEndPosition(pos: list[float], hndl: Any = ...) -> None:
    """Returns the collision position of a trace result.

@param pos           Vector buffer to store data in.
@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@error               Invalid Handle."""
    pass
def TR_GetEntityIndex(hndl: Any = ...) -> int:
    """Returns the entity index that collided with the trace.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Entity index or -1 for no collision.
@error               Invalid Handle."""
    pass
def TR_GetDisplacementFlags(hndl: Any = ...) -> int:
    """Returns the displacement flags for the surface that was hit. See DISPSURF_FLAG_*.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Displacement flags.
@error               Invalid Handle."""
    pass
def TR_GetSurfaceName(hndl: Any, buffer: str, maxlen: int) -> None:
    """Returns the name of the surface that was hit.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@param buffer        Buffer to store surface name in
@param maxlen        Maximum length of output buffer
@error               Invalid Handle."""
    pass
def TR_GetSurfaceProps(hndl: Any = ...) -> int:
    """Returns the surface properties index of the surface that was hit.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Surface props.
@error               Invalid Handle."""
    pass
def TR_GetSurfaceFlags(hndl: Any = ...) -> int:
    """Returns the surface flags. See SURF_*.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Surface flags.
@error               Invalid Handle."""
    pass
def TR_GetPhysicsBone(hndl: Any = ...) -> int:
    """Returns the index of the physics bone that was hit.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Physics bone index.
@error               Invalid Handle."""
    pass
def TR_AllSolid(hndl: Any = ...) -> bool:
    """Returns whether the entire trace was in a solid area.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              True if entire trace was in a solid area, otherwise false.
@error               Invalid Handle."""
    pass
def TR_StartSolid(hndl: Any = ...) -> bool:
    """Returns whether the initial point was in a solid area.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              True if initial point was in a solid area, otherwise false.
@error               Invalid Handle."""
    pass
def TR_DidHit(hndl: Any = ...) -> bool:
    """Returns if there was any kind of collision along the trace ray.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              True if any collision found, otherwise false.
@error               Invalid Handle."""
    pass
def TR_GetHitGroup(hndl: Any = ...) -> int:
    """Returns in which body hit group the trace collided if any.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Body hit group.
@error               Invalid Handle."""
    pass
def TR_GetHitBoxIndex(hndl: Any = ...) -> int:
    """Returns in which hitbox the trace collided if any. 

Note: if the entity that collided with the trace is the world entity, 
then this function doesn't return an hitbox index but a static prop index.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@return              Hitbox index (Or static prop index).
@error               Invalid Handle."""
    pass
def TR_GetPlaneNormal(hndl: Any, normal: list[float]) -> None:
    """Find the normal vector to the collision plane of a trace.

@param hndl          A trace Handle, or INVALID_HANDLE to use a global trace result.
@param normal        Vector buffer to store the vector normal to the collision plane
@error               Invalid Handle"""
    pass
def TR_PointOutsideWorld(pos: list[float]) -> bool:
    """Tests a point to see if it's outside any playable area

@param pos           Vector buffer to store data in.
@return              True if outside world, otherwise false."""
    pass
CONTENTS_EMPTY: Any = ...  # 0           /**< No contents. */
CONTENTS_SOLID: Any = ...  # 0x1         /**< an eye is never valid in a solid . */
CONTENTS_WINDOW: Any = ...  # 0x2         /**< translucent, but not watery (glass). */
CONTENTS_AUX: Any = ...  # 0x4
CONTENTS_GRATE: Any = ...  # 0x8         /**< alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't. */
CONTENTS_SLIME: Any = ...  # 0x10
CONTENTS_WATER: Any = ...  # 0x20
CONTENTS_MIST: Any = ...  # 0x40
CONTENTS_OPAQUE: Any = ...  # 0x80        /**< things that cannot be seen through (may be non-solid though). */
LAST_VISIBLE_CONTENTS: Any = ...  # 0x80
ALL_VISIBLE_CONTENTS: Any = ...  # (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
CONTENTS_TESTFOGVOLUME: Any = ...  # 0x100
CONTENTS_UNUSED5: Any = ...  # 0x200
CONTENTS_UNUSED6: Any = ...  # 0x4000
CONTENTS_TEAM1: Any = ...  # 0x800       /**< per team contents used to differentiate collisions. */
CONTENTS_TEAM2: Any = ...  # 0x1000      /**< between players and objects on different teams. */
CONTENTS_IGNORE_NODRAW_OPAQUE: Any = ...  # 0x2000      /**< ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW. */
CONTENTS_MOVEABLE: Any = ...  # 0x4000      /**< hits entities which are MOVETYPE_PUSH (doors, plats, etc) */
CONTENTS_AREAPORTAL: Any = ...  # 0x8000      /**< remaining contents are non-visible, and don't eat brushes. */
CONTENTS_PLAYERCLIP: Any = ...  # 0x10000
CONTENTS_MONSTERCLIP: Any = ...  # 0x20000
CONTENTS_CURRENT_0: Any = ...  # 0x40000
CONTENTS_CURRENT_90: Any = ...  # 0x80000
CONTENTS_CURRENT_180: Any = ...  # 0x100000
CONTENTS_CURRENT_270: Any = ...  # 0x200000
CONTENTS_CURRENT_UP: Any = ...  # 0x400000
CONTENTS_CURRENT_DOWN: Any = ...  # 0x800000
CONTENTS_ORIGIN: Any = ...  # 0x1000000   /**< removed before bsp-ing an entity. */
CONTENTS_MONSTER: Any = ...  # 0x2000000   /**< should never be on a brush, only in game. */
CONTENTS_DEBRIS: Any = ...  # 0x4000000
CONTENTS_DETAIL: Any = ...  # 0x8000000   /**< brushes to be added after vis leafs. */
CONTENTS_TRANSLUCENT: Any = ...  # 0x10000000  /**< auto set if any surface has trans. */
CONTENTS_LADDER: Any = ...  # 0x20000000
CONTENTS_HITBOX: Any = ...  # 0x40000000  /**< use accurate hitboxes on trace. */
MASK_ALL: Any = ...  # (0xFFFFFFFF)
MASK_SOLID: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)                      /**< everything that is normally solid */
MASK_PLAYERSOLID: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)  /**< everything that blocks player movement */
MASK_NPCSOLID: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
MASK_WATER: Any = ...  # (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)                                                       /**< water physics in these contents */
MASK_OPAQUE: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)                                                      /**< everything that blocks line of sight for AI, lighting, etc */
MASK_OPAQUE_AND_NPCS: Any = ...  # (MASK_OPAQUE|CONTENTS_MONSTER)                                                                          /**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
MASK_VISIBLE: Any = ...  # (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)                                                             /**< everything that blocks line of sight for players */
MASK_VISIBLE_AND_NPCS: Any = ...  # (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)                                                    /**< everything that blocks line of sight for players, but with monsters added. */
MASK_SHOT: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)     /**< bullets see these as solid */
MASK_SHOT_HULL: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)      /**< non-raycasted weapons see this as solid (includes grates) */
MASK_SHOT_PORTAL: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW)                                                      /**< hits solids (not grates) and passes through everything else */
MASK_SOLID_BRUSHONLY: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)                                       /**< everything normally solid, except monsters (world+brush only) */
MASK_PLAYERSOLID_BRUSHONLY: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)                   /**< everything normally solid for player movement, except monsters (world+brush only) */
MASK_NPCSOLID_BRUSHONLY: Any = ...  # (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)                  /**< everything normally solid for npc movement, except monsters (world+brush only) */
MASK_NPCWORLDSTATIC: Any = ...  # (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)                                    /**< just the world, used for route rebuilding */
MASK_SPLITAREAPORTAL: Any = ...  # (CONTENTS_WATER|CONTENTS_SLIME)                                                                         /**< These are things that can split areaportals */
SURF_LIGHT: Any = ...  # 0x0001      /**< value will hold the light strength */
SURF_SKY2D: Any = ...  # 0x0002      /**< don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox */
SURF_SKY: Any = ...  # 0x0004      /**< don't draw, but add to skybox */
SURF_WARP: Any = ...  # 0x0008      /**< turbulent water warp */
SURF_TRANS: Any = ...  # 0x0010
SURF_NOPORTAL: Any = ...  # 0x0020      /**< the surface can not have a portal placed on it */
SURF_TRIGGER: Any = ...  # 0x0040      /**< This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders */
SURF_NODRAW: Any = ...  # 0x0080      /**< don't bother referencing the texture */
SURF_HINT: Any = ...  # 0x0100      /**< make a primary bsp splitter */
SURF_SKIP: Any = ...  # 0x0200      /**< completely ignore, allowing non-closed brushes */
SURF_NOLIGHT: Any = ...  # 0x0400      /**< Don't calculate light */
SURF_BUMPLIGHT: Any = ...  # 0x0800      /**< calculate three lightmaps for the surface for bumpmapping */
SURF_NOSHADOWS: Any = ...  # 0x1000      /**< Don't receive shadows */
SURF_NODECALS: Any = ...  # 0x2000      /**< Don't receive decals */
SURF_NOCHOP: Any = ...  # 0x4000      /**< Don't subdivide patches on this surface */
SURF_HITBOX: Any = ...  # 0x8000      /**< surface is part of a hitbox */
PARTITION_SOLID_EDICTS: Any = ...  # (1 << 1) /**< every edict_t that isn't SOLID_TRIGGER or SOLID_NOT (and static props) */
PARTITION_TRIGGER_EDICTS: Any = ...  # (1 << 2) /**< every edict_t that IS SOLID_TRIGGER */
PARTITION_NON_STATIC_EDICTS: Any = ...  # (1 << 5) /**< everything in solid & trigger except the static props, includes SOLID_NOTs */
PARTITION_STATIC_PROPS: Any = ...  # (1 << 7)
DISPSURF_FLAG_SURFACE: Any = ...  # (1<<0)
DISPSURF_FLAG_WALKABLE: Any = ...  # (1<<1)
DISPSURF_FLAG_BUILDABLE: Any = ...  # (1<<2)
DISPSURF_FLAG_SURFPROP1: Any = ...  # (1<<3)
DISPSURF_FLAG_SURFPROP2: Any = ...  # (1<<4)
data: Any = ...
traceType: TraceType = ...