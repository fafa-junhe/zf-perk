from typing import Any, list, Callable, Union
from .core import *


def OnBanClient(client: int, time: int, flags: int, reason: str, kick_message: str, command: str, source: Any) -> Any:
    """Called for calls to BanClient() with a non-empty command.

@param client        Client being banned.
@param time          Time the client is being banned for (0 = permanent).
@param flags         One if AUTHID or IP will be enabled.  If AUTO is also 
                     enabled, it means Core autodetected which to use.
@param reason        Reason passed via BanClient().
@param kick_message  Kick message passed via BanClient().
@param command       Command string to identify the ban source.
@param source        Source value passed via BanClient().
@return              Plugin_Handled to block the actual server banning.
                     Kicking will still occur."""
    pass
def OnBanIdentity(identity: str, time: int, flags: int, reason: str, command: str, source: Any) -> Any:
    """Called for calls to BanIdentity() with a non-empty command.

@param identity      Identity string being banned (authstring or ip).
@param time          Time the client is being banned for (0 = permanent).
@param flags         Ban flags (only IP or AUTHID are valid here).
@param reason        Reason passed via BanIdentity().
@param command       Command string to identify the ban source.
@param source        Source value passed via BanIdentity().
@return              Plugin_Handled to block the actual server banning."""
    pass
def OnRemoveBan(identity: str, flags: int, command: str, source: Any) -> Any:
    """Called for calls to RemoveBan() with a non-empty command.

@param identity      Identity string being banned (authstring or ip).
@param flags         Ban flags (only IP or AUTHID are valid here).
@param command       Command string to identify the ban source.
@param source        Source value passed via BanIdentity().
@return              Plugin_Handled to block the actual unbanning."""
    pass
def BanClient(client: int, time: int, flags: int, reason: str, kick_message: str = ..., command: str = ..., source: Any = ...) -> bool:
    """Bans a client.

@param client        Client being banned.
@param time          Time (in minutes) to ban (0 = permanent).
@param flags         Flags for controlling the ban mechanism.  If AUTHID 
                     is set and no AUTHID is available, the ban will fail 
                     unless AUTO is also flagged.
@param reason        Reason to ban the client for.
@param kick_message  Message to display to the user when kicking.
@param command       Command string to identify the source.  If this is left 
                     empty, then the OnBanClient forward will not be called.
@param source        A source value that could be interpreted as a player 
                     index of any sort (not actually checked by Core).
@return              True on success, false on failure.
@error               Invalid client index or client not in game."""
    pass
def BanIdentity(identity: str, time: int, flags: int, reason: str, command: str = ..., source: Any = ...) -> bool:
    """Bans an identity (either an IP address or auth string).

@param identity      String to ban (ip or authstring).
@param time          Time to ban for (0 = permanent).
@param flags         Flags (only IP and AUTHID are valid flags here).
@param reason        Ban reason string.
@param command       Command string to identify the source.  If this is left 
                     empty, then the OnBanIdentity forward will not be called.
@param source        A source value that could be interpreted as a player
                     index of any sort (not actually checked by Core).
@return              True on success, false on failure."""
    pass
def RemoveBan(identity: str, flags: int, command: str = ..., source: Any = ...) -> bool:
    """Removes a ban that was written to the server (either in memory or on disk).

@param identity      String to unban (ip or authstring).
@param flags         Flags (only IP and AUTHID are valid flags here).
@param command       Command string to identify the source.  If this is left 
                     empty, then OnRemoveBan will not be called.
@param source        A source value that could be interpreted as a player 
                     index of any sort (not actually checked by Core).
@return              True on success, false on failure."""
    pass
BANFLAG_AUTO: Any = ...  # (1<<0)  /**< Auto-detects whether to ban by steamid or IP */
BANFLAG_IP: Any = ...  # (1<<1)  /**< Always ban by IP address */
BANFLAG_AUTHID: Any = ...  # (1<<2)  /**< Always ban by authstring (for BanIdentity) if possible */
BANFLAG_NOKICK: Any = ...  # (1<<3)  /**< Does not kick the client */
source: Any = ...