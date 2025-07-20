from typing import Any, list, Callable, Union


def GetCommandLine(commandLine: str, maxlen: int) -> bool:
    """Gets the full command line the server was launched with.

@param commandLine   Buffer to store the command line in.
@param maxlen        Maximum length of the command line buffer.
@return              True if the command line is valid; otherwise, false.
@error               No command line available, or no mod support."""
    pass
def GetCommandLineParam(param: str, value: str, maxlen: int, defValue: str = ...) -> None:
    """Gets the value of a command line parameter the server was launched with.

@param param         The command line parameter to get the value of.
@param value         Buffer to store the parameter value in.
@param maxlen        Maximum length of the value buffer.
@param defValue      The default value to return if the parameter wasn't specified.
@error               No command line available, or no mod support."""
    pass
def GetCommandLineParamInt(param: str, defValue: int = ...) -> int:
    """Gets the value of a command line parameter the server was launched with.

@param param         The command line parameter to get the value of.
@param defValue      The default value to return if the parameter wasn't specified.
@return              The integer value of the command line parameter value.
@error               No command line available, or no mod support."""
    pass
def GetCommandLineParamFloat(param: str, defValue: float = ...) -> float:
    """Gets the value of a command line parameter the server was launched with.

@param param         The command line parameter to get the value of.
@param defValue      The default value to return if the parameter wasn't specified.
@return              The floating point value of the command line parameter value.
@error               No command line available, or no mod support."""
    pass
def FindCommandLineParam(param: str) -> bool:
    """Determines if a specific command line parameter is present.

@param param         The command line parameter to test.
@return              True if the command line parameter is specified; otherwise, false.
@error               No command line available, or no mod support."""
    pass