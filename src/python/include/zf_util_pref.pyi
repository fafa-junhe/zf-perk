from typing import Any, list, Callable, Union


class ZFPref:
    JoinState: int = ...
    PerkSelectMode: int = ...
    SurPendPerk: int = ...
    SurPerk: int = ...
    TeamPref: int = ...
    ZomPendPerk: int = ...
    ZomPerk: int = ...


def pref_OnClientConnect(client: int) -> None:
    pass
def pref_OnClientDisconnect(client: int) -> None:
    pass
def prefGet(client: int, pref: ZFPref) -> int:
    pass
def prefSet(client: int, pref: ZFPref, value: int) -> None:
    pass
MAX_PREF_TRIE_SIZE: Any = ...  # 512
MAX_PREFS: Any = ...  # 7
ZF_JOINSTATE_SUR: Any = ...  # 0x1
ZF_JOINSTATE_ZOM: Any = ...  # 0x2
ZF_TEAMPREF_SUR: Any = ...  # 0
ZF_TEAMPREF_NONE: Any = ...  # 1
ZF_TEAMPREF_ZOM: Any = ...  # 2
DEFAULT_PREFS: int = ...