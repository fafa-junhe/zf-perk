from typing import Any, list, Callable, Union


def GetVectorLength(vec: list[float], squared: bool = ...) -> float:
    """Calculates a vector's length.

@param vec           Vector.
@param squared       If true, the result will be squared (for optimization).
@return              Vector length (magnitude)."""
    pass
def GetVectorDistance(vec1: list[float], vec2: list[float], squared: bool = ...) -> float:
    """Calculates the distance between two vectors.

@param vec1          First vector.
@param vec2          Second vector.
@param squared       If true, the result will be squared (for optimization).
@return              Vector distance."""
    pass
def GetVectorDotProduct(vec1: list[float], vec2: list[float]) -> float:
    """Calculates the dot product of two vectors.

@param vec1          First vector.
@param vec2          Second vector.
@return              Dot product of the two vectors."""
    pass
def GetVectorCrossProduct(vec1: list[float], vec2: list[float], result: list[float]) -> None:
    """Computes the cross product of two vectors.  Any input array can be the same
as the output array.

@param vec1          First vector.
@param vec2          Second vector.
@param result        Resultant vector."""
    pass
def NormalizeVector(vec: list[float], result: list[float]) -> float:
    """Normalizes a vector.  The input array can be the same as the output array.

@param vec           Vector.
@param result        Resultant vector.
@return              Vector length."""
    pass
def GetAngleVectors(angle: list[float], fwd: list[float], right: list[float], up: list[float]) -> None:
    """Returns vectors in the direction of an angle.

@param angle         Angle.
@param fwd           Forward vector buffer or NULL_VECTOR.
@param right         Right vector buffer or NULL_VECTOR.
@param up            Up vector buffer or NULL_VECTOR."""
    pass
def GetVectorAngles(vec: list[float], angle: list[float]) -> None:
    """Returns angles from a vector.

@param vec           Vector.
@param angle         Angle buffer."""
    pass
def GetVectorVectors(vec: list[float], right: list[float], up: list[float]) -> None:
    """Returns direction vectors from a vector.

@param vec           Vector.
@param right         Right vector buffer or NULL_VECTOR.
@param up            Up vector buffer or NULL_VECTOR."""
    pass
def AddVectors(vec1: list[float], vec2: list[float], result: list[float]) -> None:
    """Adds two vectors.  It is safe to use either input buffer as an output
buffer.

@param vec1          First vector.
@param vec2          Second vector.
@param result        Result buffer."""
    pass
def SubtractVectors(vec1: list[float], vec2: list[float], result: list[float]) -> None:
    """Subtracts a vector from another vector.  It is safe to use either input
buffer as an output buffer.

@param vec1          First vector.
@param vec2          Second vector to subtract from first.
@param result        Result buffer."""
    pass
def ScaleVector(vec: list[float], scale: float) -> None:
    """Scales a vector.

@param vec           Vector.
@param scale         Scale value."""
    pass
def NegateVector(vec: list[float]) -> None:
    """Negatives a vector.

@param vec           Vector."""
    pass
def MakeVectorFromPoints(pt1: list[float], pt2: list[float], output: list[float]) -> None:
    """Builds a vector from two points by subtracting the points.

@param pt1           First point (to be subtracted from the second).
@param pt2           Second point.
@param output        Output vector buffer."""
    pass