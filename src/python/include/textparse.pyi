from typing import Any, list, Callable, Union
from .handles import *


class SMCError:
    """Parse error codes."""
    SMCError_Custom: int = ...
    SMCError_InvalidProperty1: int = ...
    SMCError_InvalidSection1: int = ...
    SMCError_InvalidSection2: int = ...
    SMCError_InvalidSection3: int = ...
    SMCError_InvalidSection4: int = ...
    SMCError_InvalidSection5: int = ...
    SMCError_InvalidTokens: int = ...
    SMCError_Okay: int = ...
    SMCError_StreamError: int = ...
    SMCError_StreamOpen: int = ...
    SMCError_TokenOverflow: int = ...


class SMCResult:
    """Parse result directive."""
    SMCParse_Continue: int = ...
    SMCParse_Halt: int = ...
    SMCParse_HaltFail: int = ...


def SMCParser() -> Any:
    pass
def ParseFile(file: str, line: int = ..., col: int = ...) -> SMCError:
    pass
def ParseString(string: str, line: int = ..., col: int = ...) -> SMCError:
    pass
def set(func: Any) -> Any:
    pass
def GetErrorString(error: SMCError, buffer: str, buf_max: int) -> None:
    pass
def SMC_CreateParser() -> Any:
    """Creates a new SMC file format parser.  This is used to set parse hooks.

@return              A new Handle to an SMC Parse structure."""
    pass
def SMC_ParseFile(smc: Any, file: str, line: int = ..., col: int = ...) -> SMCError:
    """Parses an SMC file.

@param smc           A Handle to an SMC Parse structure.
@param file          A string containing the file path.
@param line          An optional by reference cell to store the last line number read.
@param col           An optional by reference cell to store the last column number read.
@return              An SMCParseError result.
@error               Invalid or corrupt Handle."""
    pass
def SMC_GetErrorString(error: SMCError, buffer: str, buf_max: int) -> bool:
    """Gets an error string for an SMCError code.

@note SMCError_Okay returns false.
@note SMCError_Custom (which is thrown on SMCParse_HaltFail) returns false.

@param error         The SMCParseError code.
@param buffer        A string buffer for the error (contents undefined on failure).
@param buf_max       The maximum size of the buffer.
@return              True on success, false otherwise."""
    pass
def SMC_SetParseStart(smc: Any, func: Any) -> None:
    """Sets the SMC_ParseStart function of a parse Handle.

@param smc           Handle to an SMC Parse.
@param func          SMC_ParseStart function.
@error               Invalid or corrupt Handle."""
    pass
def SMC_SetParseEnd(smc: Any, func: Any) -> None:
    """Sets the SMC_ParseEnd of a parse handle.

@param smc           Handle to an SMC Parse.
@param func          SMC_ParseEnd function.
@error               Invalid or corrupt Handle."""
    pass
def SMC_SetReaders(smc: Any, ns: Any, kv: Any, es: Any) -> None:
    """Sets the three main reader functions.

@param smc           An SMC parse Handle.
@param ns            An SMC_NewSection function pointer.
@param kv            An SMC_KeyValue function pointer.
@param es            An SMC_EndSection function pointer."""
    pass
def SMC_SetRawLine(smc: Any, func: Any) -> None:
    """Sets a raw line reader on an SMC parser Handle.

@param smc           Handle to an SMC Parse.
@param func          SMC_RawLine function."""
    pass
SMC_ParseStart: Any = ...
SMC_NewSection: Any = ...
SMC_KeyValue: Any = ...
SMC_EndSection: Any = ...
SMC_ParseEnd: Any = ...
SMC_RawLine: Any = ...