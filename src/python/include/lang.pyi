from typing import Any, list, Callable, Union


def LoadTranslations(file: str) -> None:
    """Loads a translation file for the plugin calling this native.
If no extension is specified, .txt is assumed.

@param file          Translation file."""
    pass
def SetGlobalTransTarget(client: int) -> None:
    """Sets the global language target.  This is useful for creating functions
that will be compatible with the %t format specifier.  Note that invalid
indexes can be specified but the error will occur during translation,
not during this function call.

@param client        Client index or LANG_SERVER."""
    pass
def GetClientLanguage(client: int) -> int:
    """Retrieves the language number of a client.

@param client        Client index.
@return              Language number client is using.
@error               Invalid client index or client not connected."""
    pass
def GetServerLanguage() -> int:
    """Retrieves the server's language.

@return              Language number server is using."""
    pass
def GetLanguageCount() -> int:
    """Returns the number of languages known in languages.cfg.

@return              Language count."""
    pass
def GetLanguageInfo(language: int, code: str = ..., codeLen: int = ..., name: str = ..., nameLen: int = ...) -> None:
    """Retrieves info about a given language number.

@param language      Language number.
@param code          Language code buffer (2-3 characters usually).
@param codeLen       Maximum length of the language code buffer.
@param name          Language name buffer.
@param nameLen       Maximum length of the language name buffer.
@error               Invalid language number."""
    pass
def SetClientLanguage(client: int, language: int) -> None:
    """Sets the language number of a client.

@param client        Client index.
@param language      Language number.
@error               Invalid client index or client not connected."""
    pass
def GetClientOriginalLanguage(client: int) -> int:
    """Retrieves the language number a client had when they connected.

@param client        Client index.
@return              Language number client originally had.
@error               Invalid client index or client not connected."""
    pass
def GetLanguageByCode(code: str) -> int:
    """Retrieves the language number from a language code.

@param code          Language code (2-3 characters usually).
@return              Language number. -1 if not found."""
    pass
def GetLanguageByName(name: str) -> int:
    """Retrieves the language number from a language name.

@param name          Language name (case insensitive).
@return              Language number. -1 if not found."""
    pass
def TranslationPhraseExists(phrase: str) -> bool:
    """Determines if the specified phrase exists within the plugin's
translation cache.

@param phrase        Phrase to look for.
@return              True if phrase exists."""
    pass
def IsTranslatedForLanguage(phrase: str, language: int) -> bool:
    """Determines if there is a translation for the specified language.

@param phrase        Phrase to check.
@param language      Language number.
@return              True if translation exists."""
    pass
LANG_SERVER: Any = ...  # 0      /**< Translate using the server's language */