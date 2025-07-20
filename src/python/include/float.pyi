from typing import Any, list, Callable, Union


def float(value: int) -> float:
    """Converts an integer into a floating point value.

@param value         Integer to convert.
@return              Floating point value."""
    pass
def FloatMul(oper1: float, oper2: float) -> float:
    pass
def FloatDiv(dividend: float, divisor: float) -> float:
    pass
def FloatAdd(oper1: float, oper2: float) -> float:
    pass
def FloatSub(oper1: float, oper2: float) -> float:
    pass
def FloatMod(oper1: float, oper2: float) -> float:
    pass
def FloatFraction(value: float) -> float:
    """Returns the decimal part of a float.

@param value         Input value.
@return              Decimal part."""
    pass
def RoundToZero(value: float) -> int:
    """Rounds a float to the closest integer to zero.

@param value         Input value to be rounded.
@return              Rounded value."""
    pass
def RoundToCeil(value: float) -> int:
    """Rounds a float to the next highest integer value.

@param value         Input value to be rounded.
@return              Rounded value."""
    pass
def RoundToFloor(value: float) -> int:
    """Rounds a float to the next lowest integer value.

@param value         Input value to be rounded.
@return              Rounded value."""
    pass
def RoundToNearest(value: float) -> int:
    """Standard IEEE rounding.

@param value         Input value to be rounded.
@return              Rounded value."""
    pass
def FloatCompare(fOne: float, fTwo: float) -> int:
    """Compares two floats.

@param fOne          First value.
@param fTwo          Second value.
@return              Returns 1 if the first argument is greater than the second argument.
                     Returns -1 if the first argument is smaller than the second argument.
                     Returns 0 if both arguments are equal."""
    pass
def SquareRoot(value: float) -> float:
    """Returns the square root of the input value, equivalent to floatpower(value, 0.5).

@param value         Input value.
@return              Square root of the value."""
    pass
def Pow(value: float, exponent: float) -> float:
    """Returns the value raised to the power of the exponent.

@param value         Value to be raised.
@param exponent      Value to raise the base.
@return              value^exponent."""
    pass
def Exponential(value: float) -> float:
    """Returns the value of raising the input by e.

@param value         Input value.
@return              exp(value)."""
    pass
def Logarithm(value: float, base: float = ...) -> float:
    """Returns the logarithm of any base specified.

@param value         Input value.
@param base          Logarithm base to use, default is 10.
@return              log(value)/log(base)."""
    pass
def Sine(value: float) -> float:
    """Returns the sine of the argument.

@param value         Input value in radians.
@return              sin(value)."""
    pass
def Cosine(value: float) -> float:
    """Returns the cosine of the argument.

@param value         Input value in radians.
@return              cos(value)."""
    pass
def Tangent(value: float) -> float:
    """Returns the tangent of the argument.

@param value         Input value in radians.
@return              tan(value)."""
    pass
def FloatAbs(value: float) -> float:
    """Returns an absolute value.

@param value         Input value.
@return              Absolute value of the input."""
    pass
def ArcTangent(angle: float) -> float:
    """Returns the arctangent of the input value.

@param angle         Input value.
@return              atan(value) in radians."""
    pass
def ArcCosine(angle: float) -> float:
    """Returns the arccosine of the input value.

@param angle         Input value.
@return              acos(value) in radians."""
    pass
def ArcSine(angle: float) -> float:
    """Returns the arcsine of the input value.

@param angle         Input value.
@return              asin(value) in radians."""
    pass
def ArcTangent2(y: float, x: float) -> float:
    """Returns the arctangent2 of the input values.

@param y             Vertical value.
@param x             Horizontal value.
@return              atan2(value) in radians."""
    pass
def RoundFloat(value: float) -> int:
    """Rounds a floating point number using the "round to nearest" algorithm.

@param value         Floating point value to round.
@return              The value rounded to the nearest integer."""
    pass
def __FLOAT_MUL__(a: float, b: float) -> float:
    pass
def __FLOAT_DIV__(a: float, b: float) -> float:
    pass
def __FLOAT_ADD__(a: float, b: float) -> float:
    pass
def __FLOAT_SUB__(a: float, b: float) -> float:
    pass
def __FLOAT_MOD__(a: float, b: float) -> float:
    pass
def __FLOAT_GT__(a: float, b: float) -> bool:
    pass
def __FLOAT_GE__(a: float, b: float) -> bool:
    pass
def __FLOAT_LT__(a: float, b: float) -> bool:
    pass
def __FLOAT_LE__(a: float, b: float) -> bool:
    pass
def __FLOAT_EQ__(a: float, b: float) -> bool:
    pass
def __FLOAT_NE__(a: float, b: float) -> bool:
    pass
def __FLOAT_NOT__(a: float) -> bool:
    pass
def DegToRad(angle: float) -> float:
    """Converts degrees to radians.

@param angle         Degrees.
@return              Radians."""
    pass
def RadToDeg(angle: float) -> float:
    """Converts radians to degrees.

@param angle         Radians.
@return              Degrees."""
    pass
def GetURandomInt() -> int:
    """Returns a random integer in the range [0, 2^31-1].

Note: Uniform random number streams are seeded automatically per-plugin.

@return              Random integer."""
    pass
def GetURandomFloat() -> float:
    """Returns a uniform random float in the range [0, 1).

Note: Uniform random number streams are seeded automatically per-plugin.

@return              Uniform random floating-point number."""
    pass
def SetURandomSeed(seeds: list[int], numSeeds: int) -> None:
    """Seeds a plugin's uniform random number stream. This is done automatically,
so normally it is totally unnecessary to call this.

@param seeds         Array of numbers to use as seeding data.
@param numSeeds      Number of seeds in the seeds array."""
    pass
def SetURandomSeedSimple(seed: int) -> None:
    """Seeds a plugin's uniform random number stream. This is done automatically,
so normally it is totally unnecessary to call this.

@param seed      Single seed value."""
    pass
FLOAT_PI: Any = ...  # 3.1415926535897932384626433832795