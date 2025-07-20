from typing import Any, list, Callable, Union
from .zf_perk import *


def getParam(key: str) -> int:
    pass
def setParam(key: str, value: int) -> None:
    pass
def getName() -> str:
    pass
def getShortdesc() -> str:
    pass
def getDesc() -> str:
    pass
def onPlayerRunCmd(buttons: int, impulse: int, vel: list[float], angles: list[float], weapon: int) -> None:
    pass
def onAmmoPickup(pickup: int) -> None:
    pass
def onCalcIsAttackCritical() -> None:
    pass
def onCallForMedic() -> None:
    pass
def onGameFrame() -> None:
    pass
def onGraceEnd() -> None:
    pass
def onMedPickup(pickup: int) -> None:
    pass
def onPeriodic() -> None:
    pass
def onPlayerDeath(victim: int, killer: int, assist: int, inflictor: int, damagetype: int) -> None:
    pass
def onPlayerSpawn() -> None:
    pass
def onRemove() -> None:
    pass
def onSetTransmit(entity: int, client: int) -> None:
    pass
def onTakeDamage(victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> None:
    pass
def onTakeDamagePost(victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> None:
    pass
def onTouch(toucher: int, touchee: int) -> None:
    pass
def updateClientPermStats() -> None:
    pass
def updateCondStats() -> None:
    pass
def doItemThrow(model: str, force: float, color: list[int]) -> None:
    pass
BASE_PERK_NAME: Any = ...  # "Unselected"
BASE_PERK_SHORTDESC: Any = ...  # "Unselected"
BASE_PERK_DESC: Any = ...  # "Please select one perk to check info"
sm: Any = ...