from typing import Any, list, Callable, Union
from .core import *
from .handles import *


class UserMessageType:
    """UserMsg message serialization formats"""
    UM_BitBuf: int = ...
    UM_Protobuf: int = ...


class UserMsg:
    """UserMsg helper values."""
    INVALID_MESSAGE_ID: int = ...


"""Hook function types for user messages.

Accepts one of the following function signatures:
- Called when a bit buffer based usermessage is hooked

@param msg_id        Message index.
@param msg           Handle to the input bit buffer.
@param players       Array containing player indexes.
@param playersNum    Number of players in the array.
@param reliable      True if message is reliable, false otherwise.
@param init          True if message is an initmsg, false otherwise.
@return              Ignored for normal hooks.  For intercept hooks, Plugin_Handled
                     blocks the message from being sent, and Plugin_Continue
                     resumes normal functionality.
- Called when a protobuf based usermessage is hooked

@param msg_id        Message index.
@param msg           Handle to the input protobuf.
@param players       Array containing player indexes.
@param playersNum    Number of players in the array.
@param reliable      True if message is reliable, false otherwise.
@param init          True if message is an initmsg, false otherwise.
@return              Ignored for normal hooks.  For intercept hooks, Plugin_Handled
                     blocks the message from being sent, and Plugin_Continue
                     resumes normal functionality."""
MsgHook = Union[
    Callable[[UserMsg, Any, list[int], int, bool, bool], Any],
    Callable[[UserMsg, Any, list[int], int, bool, bool], Any]
]
def GetUserMessageType() -> UserMessageType:
    """Returns usermessage serialization type used for the current engine

@return              The supported usermessage type."""
    pass
def UserMessageToProtobuf(msg: Any) -> Any:
    pass
def UserMessageToBfWrite(msg: Any) -> Any:
    pass
def UserMessageToBfRead(msg: Any) -> Any:
    pass
def GetUserMessageId(msg: str) -> UserMsg:
    """Returns the ID of a given message, or -1 on failure.

@param msg           String containing message name (case sensitive).
@return              A message index, or INVALID_MESSAGE_ID on failure."""
    pass
def GetUserMessageName(msg_id: UserMsg, msg: str, maxlength: int) -> bool:
    """Retrieves the name of a message by ID.

@param msg_id        Message index.
@param msg           Buffer to store the name of the message.
@param maxlength     Maximum length of string buffer.
@return              True if message index is valid, false otherwise."""
    pass
def StartMessage(msgname: str, clients: list[int], numClients: int, flags: int = ...) -> Any:
    """Starts a usermessage (network message).

@note Only one message can be active at a time.
@note It is illegal to send any message while a non-intercept hook is in progress.

@param msgname       Message name to start.
@param clients       Array containing player indexes to broadcast to.
@param numClients    Number of players in the array.
@param flags         Optional flags to set.
@return              A handle to a bf_write bit packing structure, or
                     INVALID_HANDLE on failure.
@error               Invalid message name, unable to start a message, invalid client,
                     or client not connected."""
    pass
def StartMessageEx(msg: UserMsg, clients: list[int], numClients: int, flags: int = ...) -> Any:
    """Starts a usermessage (network message).

@note Only one message can be active at a time.
@note It is illegal to send any message while a non-intercept hook is in progress.

@param msg           Message index to start.
@param clients       Array containing player indexes to broadcast to.
@param numClients    Number of players in the array.
@param flags         Optional flags to set.
@return              A handle to a bf_write bit packing structure, or
                     INVALID_HANDLE on failure.
@error               Invalid message name, unable to start a message, invalid client,
                     or client not connected."""
    pass
def EndMessage() -> None:
    """Ends a previously started user message (network message)."""
    pass
def HookUserMessage(msg_id: UserMsg, hook: MsgHook, intercept: bool = ..., post: Any = ...) -> None:
    """Hooks a user message.

@param msg_id        Message index.
@param hook          Function to use as a hook.
@param intercept     If intercept is true, message will be fully intercepted,
                     allowing the user to block the message.  Otherwise,
                     the hook is normal and ignores the return value.
@param post          Notification function.
@error               Invalid message index."""
    pass
def UnhookUserMessage(msg_id: UserMsg, hook: MsgHook, intercept: bool = ...) -> None:
    """Removes one usermessage hook.

@param msg_id        Message index.
@param hook          Function used for the hook.
@param intercept     Specifies whether the hook was an intercept hook or not.
@error               Invalid message index."""
    pass
def StartMessageAll(msgname: str, flags: int = ...) -> Any:
    """Starts a usermessage (network message) that broadcasts to all clients.

@note See StartMessage or StartMessageEx().

@param msgname       Message name to start.
@param flags         Optional flags to set.
@return              A handle to a bf_write bit packing structure, or
                     INVALID_HANDLE on failure."""
    pass
def StartMessageOne(msgname: str, client: int, flags: int = ...) -> Any:
    """Starts a simpler usermessage (network message) for one client.

@note See StartMessage or StartMessageEx().

@param msgname       Message name to start.
@param client        Client to send to.
@param flags         Optional flags to set.
@return              A handle to a bf_write bit packing structure, or
                     INVALID_HANDLE on failure."""
    pass
USERMSG_RELIABLE: Any = ...  # (1<<2)    /**< Message will be set on the reliable stream */
USERMSG_INITMSG: Any = ...  # (1<<3)    /**< Message will be considered to be an initmsg */
USERMSG_BLOCKHOOKS: Any = ...  # (1<<7)    /**< Prevents the message from triggering SourceMod and Metamod hooks */
MsgPostHook: Any = ...
total: int = ...