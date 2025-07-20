from typing import Any, list, Callable, Union


class ListenOverride:
    """@endsection"""
    Listen_Default: int = ...
    Listen_No: int = ...
    Listen_Yes: int = ...


def OnClientSpeaking(client: int) -> None:
    """Called when a client is speaking.

@param client        The client index"""
    pass
def OnClientSpeakingEnd(client: int) -> None:
    """Called once a client speaking end.

@param client        The client index"""
    pass
def SetClientListeningFlags(client: int, flags: int) -> None:
    """Set the client listening flags.

@param client        The client index
@param flags         The voice flags
@error               Invalid client index or client not connected."""
    pass
def GetClientListeningFlags(client: int) -> int:
    """Retrieve the client current listening flags.

@param client        The client index
@return              The current voice flags
@error               Invalid client index or client not connected."""
    pass
def SetClientListening(iReceiver: int, iSender: int, bListen: bool) -> bool:
    pass
def GetClientListening(iReceiver: int, iSender: int) -> bool:
    pass
def SetListenOverride(iReceiver: int, iSender: int, override: ListenOverride) -> bool:
    """Override the receiver's ability to listen to the sender.

@param iReceiver     The listener index.
@param iSender       The sender index.
@param override      The override of the receiver's ability to listen to the sender.
@return              True if successful otherwise false.
@error               Listener or sender client index is invalid or not connected."""
    pass
def GetListenOverride(iReceiver: int, iSender: int) -> ListenOverride:
    """Retrieves the override of the receiver's ability to listen to the sender.

@param iReceiver     The listener index.
@param iSender       The sender index.
@return              The override value.
@error               Listener or sender client index is invalid or not connected."""
    pass
def IsClientMuted(iMuter: int, iMutee: int) -> bool:
    """Retrieves if the muter has muted the mutee.

@param iMuter        The muter index.
@param iMutee        The mutee index.
@return              True if muter has muted mutee, false otherwise.
@error               Muter or mutee client index is invalid or not connected."""
    pass
VOICE_NORMAL: Any = ...  # 0   /**< Allow the client to listen and speak normally. */
VOICE_MUTED: Any = ...  # 1   /**< Mutes the client from speaking to everyone. */
VOICE_SPEAKALL: Any = ...  # 2   /**< Allow the client to speak to everyone. */
VOICE_LISTENALL: Any = ...  # 4   /**< Allow the client to listen to everyone. */
VOICE_TEAM: Any = ...  # 8   /**< Allow the client to always speak to team, even when dead. */
VOICE_LISTENTEAM: Any = ...  # 16  /**< Allow the client to always hear teammates, including dead ones. */