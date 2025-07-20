from typing import Any, list, Callable, Union


def strlen(str: str) -> int:
    """Calculates the length of a string.

@param str           String to check.
@return              Number of valid character bytes in the string."""
    pass
def StrContains(str: str, substr: str, caseSensitive: bool = ...) -> int:
    """Tests whether a string is found inside another string.

@param str           String to search in.
@param substr        Substring to find inside the original string.
@param caseSensitive If true (default), search is case sensitive.
                     If false, search is case insensitive.
@return              -1 on failure (no match found). Any other value
                     indicates a position in the string where the match starts."""
    pass
def strcmp(str1: str, str2: str, caseSensitive: bool = ...) -> int:
    """Compares two strings lexographically.

@param str1          First string (left).
@param str2          Second string (right).
@param caseSensitive If true (default), comparison is case sensitive.
                     If false, comparison is case insensitive.
@return              -1 if str1 < str2
                     0 if str1 == str2
                     1 if str1 > str2"""
    pass
def strncmp(str1: str, str2: str, num: int, caseSensitive: bool = ...) -> int:
    """Compares two strings parts lexographically.

@param str1          First string (left).
@param str2          Second string (right).
@param num           Number of characters to compare.
@param caseSensitive If true (default), comparison is case sensitive.
                     If false, comparison is case insensitive.
@return              -1 if str1 < str2
                     0 if str1 == str2
                     1 if str1 > str2"""
    pass
def StrCompare(str1: str, str2: str, caseSensitive: bool = ...) -> int:
    pass
def StrEqual(str1: str, str2: str, caseSensitive: bool = ...) -> bool:
    """Returns whether two strings are equal.

@param str1          First string (left).
@param str2          Second string (right).
@param caseSensitive If true (default), comparison is case sensitive.
                     If false, comparison is case insensitive.
@return              True if equal, false otherwise."""
    pass
def strcopy(dest: str, destLen: int, source: str) -> int:
    """Copies one string to another string.
@note If the destination buffer is too small to hold the source string, the 
      destination will be truncated.

@param dest          Destination string buffer to copy to.
@param destLen       Destination buffer length (includes null terminator).
@param source        Source string buffer to copy from.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def StrCopy(dest: str, destLen: int, source: str) -> int:
    pass
def Format(buffer: str, maxlength: int, format: str, _0: Any, *args: Any) -> int:
    """Formats a string according to the SourceMod format rules (see documentation).

@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer,
                     including the null terminator.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def FormatEx(buffer: str, maxlength: int, format: str, _0: Any, *args: Any) -> int:
    """Formats a string according to the SourceMod format rules (see documentation).
@note This is the same as Format(), except none of the input buffers can 
      overlap the same memory as the output buffer.  Since this security 
      check is removed, it is slightly faster.

@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer,
                     including the null terminator.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def VFormat(buffer: str, maxlength: int, format: str, varpos: int) -> int:
    """Formats a string according to the SourceMod format rules (see documentation).
@note This is the same as Format(), except it grabs parameters from a 
      parent parameter stack, rather than a local.  This is useful for 
      implementing your own variable argument functions.

@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer,
                     including the null terminator.
@param format        Formatting rules.
@param varpos        Argument number which contains the '...' symbol.
                     Note: Arguments start at 1.
@return              Number of bytes written.
@error               Invalid argument index."""
    pass
def StringToInt(str: str, nBase: int = ...) -> int:
    """Converts a string to an integer.

@param str           String to convert.
@param nBase         Numerical base to use.  10 is default.
@return              Integer conversion of string, or 0 on failure."""
    pass
def StringToIntEx(str: str, result: int, nBase: int = ...) -> int:
    """Converts a string to an integer with some more options.

@param str           String to convert.
@param result        Variable to store the result in.
@param nBase         Numerical base to use.  10 is default.
@return              Number of characters consumed."""
    pass
def StringToInt64(str: str, result: list[int], nBase: int = ...) -> int:
    """Converts a string to a 64-bit integer.

@param str           String to convert.
@param result        Array to store the upper and lower
                     32-bits of the 64-bit integer.
@param nBase         Numerical base to use.  10 is default.
@return              Number of characters consumed."""
    pass
def IntToString(num: int, str: str, maxlength: int) -> int:
    """Converts an integer to a string.

@param num           Integer to convert.
@param str           Buffer to store string in.
@param maxlength     Maximum length of string buffer.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def Int64ToString(num: list[int], str: str, maxlength: int) -> int:
    """Converts a 64-bit integer to a string.

@param num           Array containing the upper and lower
                     32-bits of a 64-bit integer.
@param str           Buffer to store string in.
@param maxlength     Maximum length of string buffer.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def StringToFloat(str: str) -> float:
    """Converts a string to a floating point number.

@param str           String to convert to a float.
@return              Floating point result, or 0.0 on error."""
    pass
def StringToFloatEx(str: str, result: float) -> int:
    """Converts a string to a floating point number with some more options.

@param str           String to convert to a float.
@param result        Variable to store result in.
@return              Number of characters consumed."""
    pass
def FloatToString(num: float, str: str, maxlength: int) -> int:
    """Converts a floating point number to a string.

@param num           Floating point number to convert.
@param str           Buffer to store string in.
@param maxlength     Maximum length of string buffer.
@return              Number of characters written to the buffer,
                     not including the null terminator."""
    pass
def BreakString(source: str, arg: str, argLen: int) -> int:
    """Finds the first "argument" in a string; either a set of space
terminated characters, or a fully quoted string.  After the 
argument is found, whitespace is read until the next portion
of the string is reached.  If nothing remains, -1 is returned.
Otherwise, the index to the first character is returned.

@param source        Source input string.
@param arg           Stores argument read from string.
@param argLen        Maximum length of argument buffer.
@return              Index to next piece of string, or -1 if none."""
    pass
def StrBreak(source: str, arg: str, argLen: int) -> int:
    pass
def TrimString(str: str) -> int:
    """Removes whitespace characters from the beginning and end of a string.

@param str           The string to trim.
@return              Number of bytes written (UTF-8 safe)."""
    pass
def SplitString(source: str, split: str, part: str, partLen: int) -> int:
    """Returns text in a string up until a certain character sequence is reached.

@param source        Source input string.
@param split         A string which specifies a search point to break at.
@param part          Buffer to store string part.
@param partLen       Maximum length of the string part buffer.
@return              -1 if no match was found; otherwise, an index into source
                     marking the first index after the searched text.  The
                     index is always relative to the start of the input string."""
    pass
def ReplaceString(text: str, maxlength: int, search: str, replace: str, caseSensitive: bool = ...) -> int:
    """Given a string, replaces all occurrences of a search string with a 
replacement string.

@param text          String to perform search and replacements on.
@param maxlength     Maximum length of the string buffer.
@param search        String to search for.
@param replace       String to replace the search string with.
@param caseSensitive If true (default), search is case sensitive.
@return              Number of replacements that were performed.
@error               'search' parameter is empty."""
    pass
def ReplaceStringEx(text: str, maxlength: int, search: str, replace: str, searchLen: int = ..., replaceLen: int = ..., caseSensitive: bool = ...) -> int:
    """Given a string, replaces the first occurrence of a search string with a 
replacement string.

@param text          String to perform search and replacements on.
@param maxlength     Maximum length of the string buffer.
@param search        String to search for.
@param replace       String to replace the search string with.
@param searchLen     If higher than -1, its value will be used instead of
                     a strlen() call on the search parameter.
@param replaceLen    If higher than -1, its value will be used instead of
                     a strlen() call on the replace parameter.
@param caseSensitive If true (default), search is case sensitive.
@return              Index into the buffer (relative to the start) from where
                     the last replacement ended, or -1 if no replacements were
                     made.
@error               'search' parameter is empty."""
    pass
def GetCharBytes(source: str) -> int:
    """Returns the number of bytes a character is using.  This is
for multi-byte characters (UTF-8).  For normal ASCII characters,
this will return 1.

@param source        Source input string.
@return              Number of bytes the current character uses."""
    pass
def IsCharAlpha(chr: int) -> bool:
    """Returns whether a character is an ASCII alphabet character.

@note Multi-byte characters will always return false.

@param chr           Character to test.
@return              True if character is alphabetical, otherwise false."""
    pass
def IsCharNumeric(chr: int) -> bool:
    """Returns whether a character is numeric.

@note Multi-byte characters will always return false.

@param chr           Character to test.
@return              True if character is numeric, otherwise false."""
    pass
def IsCharSpace(chr: int) -> bool:
    """Returns whether a character is whitespace.

@note Multi-byte characters will always return false.

@param chr           Character to test.
@return              True if character is whitespace, otherwise false."""
    pass
def IsCharMB(chr: int) -> int:
    """Returns if a character is multi-byte or not.

@param chr           Character to test.
@return              0 for a normal 7-bit ASCII character,
                     otherwise number of bytes in multi-byte character."""
    pass
def IsCharUpper(chr: int) -> bool:
    """Returns whether an alphabetic character is uppercase.

@note Multi-byte characters will always return false.

@param chr           Character to test.
@return              True if character is uppercase, otherwise false."""
    pass
def IsCharLower(chr: int) -> bool:
    """Returns whether an alphabetic character is lowercase.

@note Multi-byte characters will always return false.

@param chr           Character to test.
@return              True if character is lowercase, otherwise false."""
    pass
def StripQuotes(text: str) -> bool:
    """Strips a quote pair off a string if it exists.  That is, the following 
replace rule is applied once:  ^"(.*)"$ -> ^\1$

Note that the leading and trailing quotes will only be removed if both 
exist.  Otherwise, the string is left unmodified.  This function should 
be considered O(k) (all characters get shifted down).

@param text          String to modify (in place).
@return              True if string was modified, false if there was no 
                     set of quotes."""
    pass
def CharToUpper(chr: int) -> int:
    """Converts a lowercase character to its uppercase counterpart.

@param chr           Character to convert.
@return              Uppercase character on success, 
                     no change on failure."""
    pass
def CharToLower(chr: int) -> int:
    """Converts an uppercase character to its lowercase counterpart.

@param chr           Character to convert.
@return              Lowercase character on success, 
                     no change on failure."""
    pass
def FindCharInString(str: str, c: str, reverse: bool = ...) -> int:
    """Finds the first occurrence of a character in a string.

@param str           String.
@param c             Character to search for.
@param reverse       False (default) to search forward, true to search 
                     backward.
@return              The index of the first occurrence of the character 
                     in the string, or -1 if the character was not found."""
    pass
def StrCat(buffer: str, maxlength: int, source: str) -> int:
    """Concatenates one string onto another.

@param buffer        String to append to.
@param maxlength     Maximum length of entire buffer.
@param source        Source string to concatenate.
@return              Number of bytes written."""
    pass
def ExplodeString(text: str, split: str, buffers: list[Any], maxStrings: int, maxStringLength: int, copyRemainder: bool = ...) -> int:
    """Breaks a string into pieces and stores each piece into an array of buffers.

@param text              The string to split.
@param split             The string to use as a split delimiter.
@param buffers           An array of string buffers (2D array).
@param maxStrings        Number of string buffers (first dimension size).
@param maxStringLength   Maximum length of each string buffer.
@param copyRemainder     False (default) discard excess pieces, true to ignore
                         delimiters after last piece.
@return                  Number of strings retrieved."""
    pass
def ImplodeStrings(strings: list[Any], numStrings: int, join: str, buffer: str, maxLength: int) -> int:
    """Joins an array of strings into one string, with a "join" string inserted in
between each given string.  This function complements ExplodeString.

@param strings       An array of strings.
@param numStrings    Number of strings in the array.
@param join          The join string to insert between each string.
@param buffer        Output buffer to write the joined string to.
@param maxLength     Maximum length of the output buffer.
@return              Number of bytes written to the output buffer."""
    pass
len: int = ...
join_length: int = ...