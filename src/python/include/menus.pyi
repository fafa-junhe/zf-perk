from typing import Any, list, Callable, Union
from .handles import *


class MenuAction:
    """Different actions for the menu "pump" callback"""
    MenuAction_Cancel: int = ...
    MenuAction_Display: int = ...
    MenuAction_DisplayItem: int = ...
    MenuAction_DrawItem: int = ...
    MenuAction_End: int = ...
    MenuAction_Select: int = ...
    MenuAction_Start: int = ...
    MenuAction_VoteCancel: int = ...
    MenuAction_VoteEnd: int = ...
    MenuAction_VoteStart: int = ...


class MenuSource:
    """Describes a menu's source"""
    MenuSource_External: int = ...
    MenuSource_None: int = ...
    MenuSource_Normal: int = ...
    MenuSource_RawPanel: int = ...


class MenuStyle:
    """Low-level drawing style of the menu."""
    MenuStyle_Default: int = ...
    MenuStyle_Radio: int = ...
    MenuStyle_Valve: int = ...


MenuCancel_Disconnected: int = ...
MenuCancel_Interrupted: int = ...
MenuCancel_Exit: int = ...
MenuCancel_NoDisplay: int = ...
MenuCancel_Timeout: int = ...
MenuCancel_ExitBack: int = ...
VoteCancel_Generic: int = ...
VoteCancel_NoVotes: int = ...
MenuEnd_Selected: int = ...
MenuEnd_VotingDone: int = ...
MenuEnd_VotingCancelled: int = ...
MenuEnd_Cancelled: int = ...
MenuEnd_Exit: int = ...
MenuEnd_ExitBack: int = ...
"""Called when a menu action is completed.

@param menu              The menu being acted upon.
@param action            The action of the menu.
@param param1            First action parameter (usually the client).
@param param2            Second action parameter (usually the item).

Use void-typed prototype if you don't plan to handle MenuAction_DrawItem
and MenuAction_DisplayItem actions."""
MenuHandler = Union[
    Callable[[Any, MenuAction, int, int], int],
    Callable[[Any, MenuAction, int, int], None]
]
def Panel(hStyle: Any = ...) -> Any:
    pass
def SetTitle(text: str, onlyIfEmpty: bool = ...) -> None:
    pass
def DrawItem(text: str, style: int = ...) -> int:
    pass
def DrawText(text: str) -> bool:
    pass
def CanDrawFlags(style: int) -> bool:
    pass
def SetKeys(keys: int) -> bool:
    pass
def Send(client: int, handler: MenuHandler, time: int) -> bool:
    pass
def get() -> Any:
    pass
def set(key: int) -> Any:
    pass
def Menu(handler: MenuHandler, actions: MenuAction = ...) -> Any:
    pass
def Display(client: int, time: int) -> bool:
    pass
def DisplayAt(client: int, first_item: int, time: int) -> bool:
    pass
def AddItem(info: str, display: str, style: int = ...) -> bool:
    pass
def InsertItem(position: int, info: str, display: str, style: int = ...) -> bool:
    pass
def RemoveItem(position: int) -> bool:
    pass
def RemoveAllItems() -> None:
    pass
def GetItem(position: int, infoBuf: str, infoBufLen: int, style: int = ..., dispBuf: str = ..., dispBufLen: int = ..., client: int = ...) -> bool:
    pass
def ShufflePerClient(start: int = ..., stop: int = ...) -> None:
    pass
def SetClientMapping(client: int, array: list[int], length: int) -> None:
    pass
def GetTitle(buffer: str, maxlength: int) -> None:
    pass
def ToPanel() -> Any:
    pass
def Cancel() -> None:
    pass
def DisplayVote(clients: list[int], numClients: int, time: int, flags: int = ...) -> bool:
    pass
def DisplayVoteToAll(time: int, flags: int = ...) -> bool:
    pass
def CreateMenu(handler: MenuHandler, actions: MenuAction = ...) -> Any:
    """Creates a new, empty menu using the default style.

@param handler       Function which will receive menu actions.
@param actions       Optionally set which actions to receive.  Select,
                     Cancel, and End will always be received regardless
                     of whether they are set or not.  They are also
                     the only default actions.
@return              A new menu Handle."""
    pass
def DisplayMenu(menu: Any, client: int, time: int) -> bool:
    """Displays a menu to a client.

@param menu          Menu Handle.
@param client        Client index.
@param time          Maximum time to leave menu on the screen.
@return              True on success, false on failure.
@error               Invalid Handle or client not in game."""
    pass
def DisplayMenuAtItem(menu: Any, client: int, first_item: int, time: int) -> bool:
    """Displays a menu to a client, starting from the given item.

@param menu          Menu Handle.
@param client        Client index.
@param first_item    First item to begin drawing from.
@param time          Maximum time to leave menu on the screen.
@return              True on success, false on failure.
@error               Invalid Handle or client not in game."""
    pass
def AddMenuItem(menu: Any, info: str, display: str, style: int = ...) -> bool:
    """Appends a new item to the end of a menu.

@param menu          Menu Handle.
@param info          Item information string.
@param display       Default item display string.
@param style         Drawing style flags.  Anything other than DEFAULT or
                     DISABLED will be completely ignored when paginating.
@return              True on success, false on failure.
@error               Invalid Handle or item limit reached."""
    pass
def InsertMenuItem(menu: Any, position: int, info: str, display: str, style: int = ...) -> bool:
    """Inserts an item into the menu before a certain position; the new item will
be at the given position and all next items pushed forward.

@param menu          Menu Handle.
@param position      Position, starting from 0.
@param info          Item information string.
@param display       Default item display string.
@param style         Drawing style flags.  Anything other than DEFAULT or
                     DISABLED will be completely ignored when paginating.
@return              True on success, false on failure.
@error               Invalid Handle or menu position."""
    pass
def RemoveMenuItem(menu: Any, position: int) -> bool:
    """Removes an item from the menu.

@param menu          Menu Handle.
@param position      Position, starting from 0.
@return              True on success, false on failure.
@error               Invalid Handle or menu position."""
    pass
def RemoveAllMenuItems(menu: Any) -> None:
    """Removes all items from a menu.

@param menu          Menu Handle.
@error               Invalid Handle or menu position."""
    pass
def GetMenuItem(menu: Any, position: int, infoBuf: str, infoBufLen: int, style: int = ..., dispBuf: str = ..., dispBufLen: int = ..., client: int = ...) -> bool:
    """Retrieves information about a menu item.

@param menu          Menu Handle.
@param position      Position, starting from 0.
@param infoBuf       Info buffer.
@param infoBufLen    Maximum length of the info buffer.
@param style         By-reference variable to store drawing flags.
@param dispBuf       Display buffer.
@param dispBufLen    Maximum length of the display buffer.
@param client		Client index. Must be specified if menu is per-client random shuffled, -1 to ignore.
@return              True on success, false if position is invalid.
@error               Invalid Handle."""
    pass
def MenuShufflePerClient(menu: Any, start: int = ..., stop: int = ...) -> None:
    """Generates a per-client random mapping for the current vote options.

@param menu          Menu Handle.
@param start         Menu item index to start randomizing from.
@param stop          Menu item index to stop randomizing at. -1 = infinite"""
    pass
def MenuSetClientMapping(menu: Any, client: int, array: list[int], length: int) -> None:
    pass
def GetMenuSelectionPosition() -> int:
    """Returns the first item on the page of a currently selected menu.

This is only valid inside a MenuAction_Select callback.

@return              First item number on the page the client was viewing
                     before selecting the item in the callback.  This can
                     be used to re-display the menu from the original
                     position.
@error               Not called from inside a MenuAction_Select callback."""
    pass
def GetMenuItemCount(menu: Any) -> int:
    """Returns the number of items in a menu.

@param menu          Menu Handle.
@return              Number of items in the menu.
@error               Invalid Handle."""
    pass
def SetMenuPagination(menu: Any, itemsPerPage: int) -> bool:
    """Sets whether the menu should be paginated or not.

If itemsPerPage is MENU_NO_PAGINATION, and the exit button flag is set,
then the exit button flag is removed.  It can be re-applied if desired.

@param menu          Handle to the menu.
@param itemsPerPage  Number of items per page, or MENU_NO_PAGINATION.
@return              True on success, false if pagination is too high or
                     low.
@error               Invalid Handle."""
    pass
def GetMenuPagination(menu: Any) -> int:
    """Returns a menu's pagination setting.

@param menu          Handle to the menu.
@return              Pagination setting.
@error               Invalid Handle."""
    pass
def GetMenuStyle(menu: Any) -> Any:
    """Returns a menu's MenuStyle Handle.  The Handle
is global and cannot be freed.

@param menu          Handle to the menu.
@return              Handle to the menu's draw style.
@error               Invalid Handle."""
    pass
def SetMenuTitle(menu: Any, fmt: str, _0: Any, *args: Any) -> None:
    """Sets the menu's default title/instruction message.

@param menu          Menu Handle.
@param fmt           Message string format
@param ...           Message string arguments.
@error               Invalid Handle."""
    pass
def GetMenuTitle(menu: Any, buffer: str, maxlength: int) -> int:
    """Returns the text of a menu's title.

@param menu          Menu Handle.
@param buffer        Buffer to store title.
@param maxlength     Maximum length of the buffer.
@return              Number of bytes written.
@error               Invalid Handle/"""
    pass
def CreatePanelFromMenu(menu: Any) -> Any:
    """Creates a raw MenuPanel based off the menu's style.
The Handle must be freed with CloseHandle().

@param menu          Menu Handle.
@return              A new MenuPanel Handle.
@error               Invalid Handle."""
    pass
def GetMenuExitButton(menu: Any) -> bool:
    """Returns whether or not the menu has an exit button.
By default, menus have an exit button.

@param menu          Menu Handle.
@return              True if the menu has an exit button; false otherwise.
@error               Invalid Handle."""
    pass
def SetMenuExitButton(menu: Any, button: bool) -> bool:
    """Sets whether or not the menu has an exit button.  By default, paginated menus
have an exit button.

If a menu's pagination is changed to MENU_NO_PAGINATION, and the pagination
was previously a different value, then the Exit button status is changed to
false.  It must be explicitly re-enabled afterwards.

If a non-paginated menu has an exit button, then at most 9 items will be
displayed.

@param menu          Menu Handle.
@param button        True to enable the button, false to remove it.
@return              True if allowed; false on failure.
@error               Invalid Handle."""
    pass
def GetMenuExitBackButton(menu: Any) -> bool:
    """Returns whether or not the menu has an "exit back" button.  By default,
menus do not have an exit back button.

Exit Back buttons appear as "Back" on page 1 of paginated menus and have
functionality defined by the user in MenuEnd_ExitBack.

@param menu          Menu Handle.
@return              True if the menu has an exit back button; false otherwise.
@error               Invalid Handle."""
    pass
def SetMenuExitBackButton(menu: Any, button: bool) -> None:
    """Sets whether or not the menu has an "exit back" button. By default, menus
do not have an exit back button.

Exit Back buttons appear as "Back" on page 1 of paginated menus and have
functionality defined by the user in MenuEnd_ExitBack.

@param menu          Menu Handle.
@param button        True to enable the button, false to remove it.
@error               Invalid Handle."""
    pass
def SetMenuNoVoteButton(menu: Any, button: bool) -> bool:
    """Sets whether or not the menu has a "no vote" button in slot 1.
By default, menus do not have a no vote button.

@param menu          Menu Handle.
@param button        True to enable the button, false to remove it.
@return              True if allowed; false on failure.
@error               Invalid Handle."""
    pass
def CancelMenu(menu: Any) -> None:
    """Cancels a menu from displaying on all clients.  While the
cancellation is in progress, this menu cannot be re-displayed
to any clients.

The menu may still exist on the client's screen after this command.
This simply verifies that the menu is not being used anywhere.

If any vote is in progress on a menu, it will be cancelled.

@param menu          Menu Handle.
@error               Invalid Handle."""
    pass
def GetMenuOptionFlags(menu: Any) -> int:
    """Retrieves a menu's option flags.

@param menu          Menu Handle.
@return              A bitstring of MENUFLAG bits.
@error               Invalid Handle."""
    pass
def SetMenuOptionFlags(menu: Any, flags: int) -> None:
    """Sets a menu's option flags.

If a certain bit is not supported, it will be stripped before being set.
See SetMenuExitButton() for information on Exit buttons.
See SetMenuExitBackButton() for information on Exit Back buttons.

@param menu          Menu Handle.
@param flags         A new bitstring of MENUFLAG bits.
@error               Invalid Handle."""
    pass
def IsVoteInProgress(menu: Any = ...) -> bool:
    """Returns whether a vote is in progress.

@param menu          Deprecated; no longer used.
@return              True if a vote is in progress, false otherwise."""
    pass
def CancelVote() -> None:
    """Cancels the vote in progress.

@error               If no vote is in progress."""
    pass
def VoteMenu(menu: Any, clients: list[int], numClients: int, time: int, flags: int = ...) -> bool:
    """Broadcasts a menu to a list of clients.  The most selected item will be
returned through MenuAction_End.  On a tie, a random item will be returned
from a list of the tied items.

Note that MenuAction_VoteEnd and MenuAction_VoteStart are both
default callbacks and do not need to be enabled.

@param menu          Menu Handle.
@param clients       Array of clients to broadcast to.
@param numClients    Number of clients in the array.
@param time          Maximum time to leave menu on the screen.
@param flags         Optional voting flags.
@return              True on success, false if this menu already has a vote session
                     in progress.
@error               Invalid Handle, or a vote is already in progress."""
    pass
def VoteMenuToAll(menu: Any, time: int, flags: int = ...) -> bool:
    """Sends a vote menu to all clients.  See VoteMenu() for more information.

@param menu          Menu Handle.
@param time          Maximum time to leave menu on the screen.
@param flags         Optional voting flags.
@return              True on success, false if this menu already has a vote session
                     in progress.
@error               Invalid Handle."""
    pass
def SetVoteResultCallback(menu: Any, callback: VoteHandler) -> None:
    """Sets an advanced vote handling callback.  If this callback is set,
MenuAction_VoteEnd will not be called.

@param menu          Menu Handle.
@param callback      Callback function.
@error               Invalid Handle or callback."""
    pass
def CheckVoteDelay() -> int:
    """Returns the number of seconds you should "wait" before displaying
a publicly invocable menu.  This number is the time remaining until
(last_vote + sm_vote_delay).

@return              Number of seconds to wait, or 0 for none."""
    pass
def IsClientInVotePool(client: int) -> bool:
    """Returns whether a client is in the pool of clients allowed
to participate in the current vote.  This is determined by
the client list passed to VoteMenu().

@param client        Client index.
@return              True if client is allowed to vote, false otherwise.
@error               If no vote is in progress or client index is invalid."""
    pass
def RedrawClientVoteMenu(client: int, revotes: bool = ...) -> bool:
    """Redraws the current vote menu to a client in the voting pool.

@param client        Client index.
@param revotes       True to allow revotes, false otherwise.
@return              True on success, false if the client is in the vote pool
                     but cannot vote again.
@error               No vote in progress, int client is not in the voting pool,
                     or client index is invalid."""
    pass
def GetMenuStyleHandle(style: MenuStyle) -> Any:
    """Returns a style's global Handle.

@param style         Menu Style.
@return              A Handle, or INVALID_HANDLE if not found or unusable."""
    pass
def CreatePanel(hStyle: Any = ...) -> Any:
    """Creates a MenuPanel from a MenuStyle.  Panels are used for drawing raw
menus without any extra helper functions.  The Handle must be closed
with CloseHandle().

@param hStyle        MenuStyle Handle, or INVALID_HANDLE to use the default style.
@return              A new MenuPanel Handle.
@error               Invalid Handle other than INVALID_HANDLE."""
    pass
def CreateMenuEx(hStyle: Any = ..., handler: MenuHandler = ..., actions: MenuAction = ...) -> Any:
    """Creates a Menu from a MenuStyle.  The Handle must be closed with
CloseHandle().

@param hStyle        MenuStyle Handle, or INVALID_HANDLE to use the default style.
@param handler       Function which will receive menu actions.
@param actions       Optionally set which actions to receive.  Select,
                     Cancel, and End will always be received regardless
                     of whether they are set or not.  They are also
                     the only default actions.
@return              A new menu Handle.
@error               Invalid Handle other than INVALID_HANDLE."""
    pass
def GetClientMenu(client: int, hStyle: Any = ...) -> MenuSource:
    """Returns whether a client is viewing a menu.

@param client        Client index.
@param hStyle        MenuStyle Handle, or INVALID_HANDLE to use the default style.
@return              A MenuSource value.
@error               Invalid Handle other than null."""
    pass
def CancelClientMenu(client: int, autoIgnore: bool = ..., hStyle: Any = ...) -> bool:
    """Cancels a menu on a client.  This will only affect non-external menus.

@param client        Client index.
@param autoIgnore    If true, no menus can be re-drawn on the client during
                     the cancellation process.
@param hStyle        MenuStyle Handle, or INVALID_HANDLE to use the default style.
@return              True if a menu was cancelled, false otherwise."""
    pass
def GetMaxPageItems(hStyle: Any = ...) -> int:
    """Returns a style's maximum items per page.

@param hStyle        MenuStyle Handle, or INVALID_HANDLE to use the default style.
@return              Maximum items per page.
@error               Invalid Handle other than INVALID_HANDLE."""
    pass
def GetPanelStyle(panel: Any) -> Any:
    """Returns a MenuPanel's parent style.

@param panel         A MenuPanel Handle.
@return              The MenuStyle Handle that created the panel.
@error               Invalid Handle."""
    pass
def SetPanelTitle(panel: Any, text: str, onlyIfEmpty: bool = ...) -> None:
    """Sets the panel's title.

@param panel         A MenuPanel Handle.
@param text          Text to set as the title.
@param onlyIfEmpty   If true, the title will only be set if no title is set.
@error               Invalid Handle."""
    pass
def DrawPanelItem(panel: Any, text: str, style: int = ...) -> int:
    """Draws an item on a panel.  If the item takes up a slot, the position
is returned.

@param panel         A MenuPanel Handle.
@param text          Display text to use.  If not a raw line,
                     the style may automatically add color markup.
                     No numbering or newlines are needed.
@param style         ITEMDRAW style flags.
@return              A slot position, or 0 if item was a rawline or could not be drawn.
@error               Invalid Handle."""
    pass
def DrawPanelText(panel: Any, text: str) -> bool:
    """Draws a raw line of text on a panel, without any markup other than a newline.

@param panel         A MenuPanel Handle, or INVALID_HANDLE if inside a
                     MenuAction_DisplayItem callback.
@param text          Display text to use.
@return              True on success, false if raw lines are not supported.
@error               Invalid Handle."""
    pass
def CanPanelDrawFlags(panel: Any, style: int) -> bool:
    """Returns whether or not the given drawing flags are supported by
the menu style.

@param panel         A MenuPanel Handle.
@param style         ITEMDRAW style flags.
@return              True if item is drawable, false otherwise.
@error               Invalid Handle."""
    pass
def SetPanelKeys(panel: Any, keys: int) -> bool:
    """Sets the selectable key map of a panel.  This is not supported by
all styles (only by Radio, as of this writing).

@param panel         A MenuPanel Handle.
@param keys          An integer where each bit N allows key
                     N+1 to be selected.  If no keys are selectable,
                     then key 0 (bit 9) is automatically set.
@return              True if supported, false otherwise."""
    pass
def SendPanelToClient(panel: Any, client: int, handler: MenuHandler, time: int) -> bool:
    """Sends a panel to a client.  Unlike full menus, the handler
function will only receive the following actions, both of
which will have INVALID_HANDLE for a menu, and the client
as param1.

MenuAction_Select (param2 will be the key pressed)
MenuAction_Cancel (param2 will be the reason)

Also, if the menu fails to display, no callbacks will be called.

@param panel         A MenuPanel Handle.
@param client        A client to draw to.
@param handler       The MenuHandler function to catch actions with.
@param time          Time to hold the menu for.
@return              True on success, false on failure.
@error               Invalid Handle."""
    pass
def GetPanelTextRemaining(panel: Any) -> int:
    """@brief Returns the amount of text the menu can still hold.  If this is
limit is reached or overflowed, the text is silently truncated.

Radio menus: Currently 511 characters (512 bytes).
Valve menus: Currently -1 (no meaning).

@param panel         A MenuPanel Handle.
@return              Number of characters that the menu can still hold,
                     or -1 if there is no known limit.
@error               Invalid Handle."""
    pass
def GetPanelCurrentKey(panel: Any) -> int:
    """@brief Returns the current key position.

@param panel         A MenuPanel Handle.
@return              Current key position starting at 1.
@error               Invalid Handle."""
    pass
def SetPanelCurrentKey(panel: Any, key: int) -> bool:
    """@brief Sets the next key position.  This cannot be used
to traverse backwards.

@param panel         A MenuPanel Handle.
@param key           Key that is greater or equal to
                     GetPanelCurrentKey().
@return              True on success, false otherwise.
@error               Invalid Handle."""
    pass
def RedrawMenuItem(text: str) -> int:
    """@brief Redraws menu text from inside a MenuAction_DisplayItem callback.

@param text          Menu text to draw.
@return              Item position; must be returned via the callback."""
    pass
def InternalShowMenu(client: int, str: str, time: int, keys: int = ..., handler: MenuHandler = ...) -> bool:
    """This function is provided for legacy code only.  Some older plugins may use
network messages instead of the panel API.  This function wraps the panel
API for eased portability into the SourceMod menu system.

This function is only usable with the Radio Menu style.  You do not need to
split up your menu into multiple packets; SourceMod will break the string
up internally.

@param client        Client index.
@param str           Full menu string as would be passed over the network.
@param time          Time to hold the menu for.
@param keys          Selectable key bitstring.
@param handler       Optional handler function, with the same rules as
                     SendPanelToClient().
@return              True on success, false on failure.
@error               Invalid client index, or radio menus not supported."""
    pass
def GetMenuVoteInfo(param2: int, winningVotes: int, totalVotes: int) -> None:
    """Retrieves voting information from MenuAction_VoteEnd.

@param param2        Second parameter of MenuAction_VoteEnd.
@param winningVotes  Number of votes received by the winning option.
@param totalVotes    Number of total votes received."""
    pass
def IsNewVoteAllowed() -> bool:
    """Quick stock to determine whether voting is allowed.  This doesn't let you
fine-tune a reason for not voting, so it's not recommended for lazily
telling clients that voting isn't allowed.

@return              True if voting is allowed, false if voting is in progress
                     or the cooldown is active."""
    pass
MENU_ACTIONS_DEFAULT: Any = ...  # MenuAction_Select|MenuAction_Cancel|MenuAction_End
MENU_ACTIONS_ALL: Any = ...  # view_as<MenuAction>(0xFFFFFFFF)
MENU_NO_PAGINATION: Any = ...  # 0           /**< Menu should not be paginated (10 items max) */
MENU_TIME_FOREVER: Any = ...  # 0           /**< Menu should be displayed as long as possible */
ITEMDRAW_DEFAULT: Any = ...  # (0)     /**< Item should be drawn normally */
ITEMDRAW_DISABLED: Any = ...  # (1<<0)  /**< Item is drawn but not selectable */
ITEMDRAW_RAWLINE: Any = ...  # (1<<1)  /**< Item should be a raw line, without a slot */
ITEMDRAW_NOTEXT: Any = ...  # (1<<2)  /**< No text should be drawn */
ITEMDRAW_SPACER: Any = ...  # (1<<3)  /**< Item should be drawn as a spacer, if possible */
ITEMDRAW_IGNORE: Any = ...  # ((1<<1)|(1<<2)) /**< Item should be completely ignored (rawline + notext) */
ITEMDRAW_CONTROL: Any = ...  # (1<<4)  /**< Item is control text (back/next/exit) */
MENUFLAG_BUTTON_EXIT: Any = ...  # (1<<0)  /**< Menu has an "exit" button (default if paginated) */
MENUFLAG_BUTTON_EXITBACK: Any = ...  # (1<<1)  /**< Menu has an "exit back" button */
MENUFLAG_NO_SOUND: Any = ...  # (1<<2)  /**< Menu will not have any select sounds */
MENUFLAG_BUTTON_NOVOTE: Any = ...  # (1<<3)  /**< Menu has a "No Vote" button at slot 1 */
VOTEINFO_CLIENT_INDEX: Any = ...  # 0       /**< Client index */
VOTEINFO_CLIENT_ITEM: Any = ...  # 1       /**< Item the client selected, or -1 for none */
VOTEINFO_ITEM_INDEX: Any = ...  # 0       /**< Item index */
VOTEINFO_ITEM_VOTES: Any = ...  # 1       /**< Number of votes for the item */
VOTEFLAG_NO_REVOTES: Any = ...  # (1<<0)  /**< Players cannot change their votes */
total: int = ...
style: int = ...
client: int = ...