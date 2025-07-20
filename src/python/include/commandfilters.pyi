from typing import Any, list, Callable, Union
from .handles import *


"""Adds clients to a multi-target filter.

@param pattern       Pattern name.
@param clients       Array to fill with unique, valid client indexes.
@param client        Client that triggered this filter.
@return              True if pattern was recognized, false otherwise.

@note To see if the client param is available, use FeatureType_Capability and FEATURECAP_MULTITARGETFILTER_CLIENTPARAM."""
MultiTargetFilter = Union[
    Callable[[str, Any], bool],
    Callable[[str, Any], bool],
    Callable[[str, Any, int], bool],
    Callable[[str, Any, int], bool]
]
def ProcessTargetString(pattern: str, admin: int, targets: list[int], max_targets: int, filter_flags: int, target_name: str, tn_maxlength: int, tn_is_ml: bool) -> int:
    """Processes a generic command target string, and resolves it to a list 
of clients or one client, based on filtering rules and a pattern.

Note that you should use LoadTranslations("common.phrases") in OnPluginStart(), 
as that file is guaranteed to contain all of the translatable phrases that 
ProcessTargetString() will return.

@param pattern       Pattern to find clients against.
@param admin         Admin performing the action, or 0 if the server.
@param targets       Array to hold targets.
@param max_targets   Maximum size of the targets array.
@param filter_flags  Filter flags.
@param target_name   Buffer to store the target name.
@param tn_maxlength  Maximum length of the target name buffer.
@param tn_is_ml      OUTPUT: Will be true if the target name buffer is an ML phrase,
                     false if it is a normal string.
@return              If a multi-target pattern was used, the number of clients found 
                     is returned.  If a single-target pattern was used, 1 is returned 
                     if one valid client is found.  Otherwise, a COMMAND_TARGET reason 
                     for failure is returned."""
    pass
def ReplyToTargetError(client: int, reason: int) -> None:
    """Replies to a client with a given message describing a targetting 
failure reason.

Note: The translation phrases are found in common.phrases.txt.

@param client        Client index, or 0 for server.
@param reason        COMMAND_TARGET reason."""
    pass
def AddMultiTargetFilter(pattern: str, filter: MultiTargetFilter, phrase: str, phraseIsML: bool) -> None:
    """Adds a multi-target filter function for ProcessTargetString().

@param pattern       Pattern to match (case sensitive).
@param filter        Filter function.
@param phrase        Descriptive phrase to display on successful match.
@param phraseIsML    True if phrase is multi-lingual, false otherwise."""
    pass
def RemoveMultiTargetFilter(pattern: str, filter: MultiTargetFilter) -> None:
    """Removes a multi-target filter function from ProcessTargetString().

@param pattern       Pattern to match (case sensitive).
@param filter        Filter function."""
    pass
MAX_TARGET_LENGTH: Any = ...  # 64
COMMAND_FILTER_ALIVE: Any = ...  # (1<<0)      /**< Only allow alive players */
COMMAND_FILTER_DEAD: Any = ...  # (1<<1)      /**< Only filter dead players */
COMMAND_FILTER_CONNECTED: Any = ...  # (1<<2)      /**< Allow players not fully in-game */
COMMAND_FILTER_NO_IMMUNITY: Any = ...  # (1<<3)      /**< Ignore immunity rules */
COMMAND_FILTER_NO_MULTI: Any = ...  # (1<<4)      /**< Do not allow multiple target patterns */
COMMAND_FILTER_NO_BOTS: Any = ...  # (1<<5)      /**< Do not allow bots to be targetted */
COMMAND_TARGET_NONE: Any = ...  # 0          /**< No target was found */
COMMAND_TARGET_NOT_ALIVE: Any = ...  # -1          /**< Single client is not alive */
COMMAND_TARGET_NOT_DEAD: Any = ...  # -2          /**< Single client is not dead */
COMMAND_TARGET_NOT_IN_GAME: Any = ...  # -3          /**< Single client is not in game */
COMMAND_TARGET_IMMUNE: Any = ...  # -4          /**< Single client is immune */
COMMAND_TARGET_EMPTY_FILTER: Any = ...  # -5          /**< A multi-filter (such as @all) had no targets */
COMMAND_TARGET_NOT_HUMAN: Any = ...  # -6          /**< Target was not human */
COMMAND_TARGET_AMBIGUOUS: Any = ...  # -7          /**< Partial name had too many targets */
FEATURECAP_MULTITARGETFILTER_CLIENTPARAM: Any = ...  # "SourceMod MultiTargetFilter ClientParam"