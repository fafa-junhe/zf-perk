from typing import Any, list, Callable, Union
from .handles import *


class SortOrder:
    """Contains sorting orders."""
    Sort_Ascending: int = ...
    Sort_Descending: int = ...
    Sort_Random: int = ...


class SortType:
    """Data types for ADT Array Sorts"""
    Sort_Float: int = ...
    Sort_Integer: int = ...
    Sort_String: int = ...


"""Sort comparison function for 2D array elements (sub-arrays).
@note You may need to use explicit tags in order to use data properly.

@param elem1         First array to compare.
@param elem2         Second array to compare.
@param array         Array that is being sorted (order is undefined).
@param hndl          Handle optionally passed in while sorting.
@return              -1 if first should go before second
                     0 if first is equal to second
                     1 if first should go after second"""
SortFunc2D = Union[
    Callable[[list[int], list[int], list[Any], Any], int],
    Callable[[str, str, list[Any], Any], int]
]
def SortIntegers(array: list[int], array_size: int, order: SortOrder = ...) -> None:
    """Sorts an array of integers.

@param array         Array of integers to sort in-place.
@param array_size    Size of the array.
@param order         Sorting order to use."""
    pass
def SortFloats(array: list[float], array_size: int, order: SortOrder = ...) -> None:
    """Sorts an array of float point numbers.

@param array         Array of floating point numbers to sort in-place.
@param array_size    Size of the array.
@param order         Sorting order to use."""
    pass
def SortStrings(array: list[Any], array_size: int, order: SortOrder = ...) -> None:
    """Sorts an array of strings.

@param array         Array of strings to sort in-place.
@param array_size    Size of the array.
@param order         Sorting order to use."""
    pass
def SortCustom1D(array: list[int], array_size: int, sortfunc: Any, hndl: Any = ...) -> None:
    """Sorts a custom 1D array.  You must pass in a comparison function.

@param array         Array to sort.
@param array_size    Size of the array to sort.
@param sortfunc      Sort function.
@param hndl          Optional Handle to pass through the comparison calls."""
    pass
def SortCustom2D(array: list[Any], array_size: int, sortfunc: SortFunc2D, hndl: Any = ...) -> None:
    """Sorts a custom 2D array.  You must pass in a comparison function.

@param array         Array to sort.
@param array_size    Size of the major array to sort (first index, outermost).
@param sortfunc      Sort comparison function to use.
@param hndl          Optional Handle to pass through the comparison calls."""
    pass
def SortADTArray(array: Any, order: SortOrder, type: SortType) -> None:
    """Sort an ADT Array. Specify the type as Integer, Float, or String.

@param array         Array Handle to sort
@param order         Sort order to use, same as other sorts.
@param type          Data type stored in the ADT Array"""
    pass
def SortADTArrayCustom(array: Any, sortfunc: Any, hndl: Any = ...) -> None:
    """Custom sorts an ADT Array. You must pass in a comparison function.

@param array         Array Handle to sort
@param sortfunc      Sort comparison function to use
@param hndl          Optional Handle to pass through the comparison calls."""
    pass
SortFunc1D: Any = ...
SortFuncADTArray: Any = ...