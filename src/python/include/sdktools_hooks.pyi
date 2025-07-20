from typing import Any, list, Callable, Union
from .core import *


def OnPlayerRunCmdPre(client: int, buttons: int, impulse: int, vel: list[float], angles: list[float], weapon: int, subtype: int, cmdnum: int, tickcount: int, seed: int, mouse: list[int]) -> None:
    """Called when a clients movement buttons are being processed (Read Only)

@param client        Index of the client.
@param buttons       Current commands (as bitflags - see entity_prop_stocks.inc).
@param impulse       Current impulse command.
@param vel           Players desired velocity.
@param angles        Players desired view angles.
@param weapon        Entity index of the new weapon if player switches weapon, 0 otherwise.
@param subtype       Weapon subtype when selected from a menu.
@param cmdnum        Command number. Increments from the first command sent.
@param tickcount     Tick count. A client's prediction based on the server's GetGameTickCount value.
@param seed          Random seed. Used to determine weapon recoil, spread, and other predicted elements.
@param mouse         Mouse direction (x, y)."""
    pass
def OnPlayerRunCmd(client: int, buttons: int, impulse: int, vel: list[float], angles: list[float], weapon: int, subtype: int, cmdnum: int, tickcount: int, seed: int, mouse: list[int]) -> Any:
    """Called when a clients movement buttons are being processed

@param client        Index of the client.
@param buttons       Copyback buffer containing the current commands (as bitflags - see entity_prop_stocks.inc).
@param impulse       Copyback buffer containing the current impulse command.
@param vel           Players desired velocity.
@param angles        Players desired view angles.
@param weapon        Entity index of the new weapon if player switches weapon, 0 otherwise.
@param subtype       Weapon subtype when selected from a menu.
@param cmdnum        Command number. Increments from the first command sent.
@param tickcount     Tick count. A client's prediction based on the server's GetGameTickCount value.
@param seed          Random seed. Used to determine weapon recoil, spread, and other predicted elements.
@param mouse         Mouse direction (x, y).
@return              Plugin_Handled to block the commands from being processed, Plugin_Continue otherwise.

@note To see if all 11 params are available, use FeatureType_Capability and FEATURECAP_PLAYERRUNCMD_11PARAMS."""
    pass
def OnPlayerRunCmdPost(client: int, buttons: int, impulse: int, vel: list[float], angles: list[float], weapon: int, subtype: int, cmdnum: int, tickcount: int, seed: int, mouse: list[int]) -> None:
    """Called after a clients movement buttons were processed.

@param client        Index of the client.
@param buttons       The current commands (as bitflags - see entity_prop_stocks.inc).
@param impulse       The current impulse command.
@param vel           Players desired velocity.
@param angles        Players desired view angles.
@param weapon        Entity index of the new weapon if player switches weapon, 0 otherwise.
@param subtype       Weapon subtype when selected from a menu.
@param cmdnum        Command number. Increments from the first command sent.
@param tickcount     Tick count. A client's prediction based on the server's GetGameTickCount value.
@param seed          Random seed. Used to determine weapon recoil, spread, and other predicted elements.
@param mouse         Mouse direction (x, y)."""
    pass
def OnFileSend(client: int, sFile: str) -> Any:
    """Called when a client requests a file from the server.

@param client        Client index.
@param sFile         Requested file path.

@return              Plugin_Handled to block the transfer, Plugin_Continue to let it proceed."""
    pass
def OnFileReceive(client: int, sFile: str) -> Any:
    """Called when a client sends a file to the server.

@param client        Client index.
@param sFile         Requested file path.

@return              Plugin_Handled to block the transfer, Plugin_Continue to let it proceed."""
    pass
FEATURECAP_PLAYERRUNCMD_11PARAMS: Any = ...  # "SDKTools PlayerRunCmd 11Params"