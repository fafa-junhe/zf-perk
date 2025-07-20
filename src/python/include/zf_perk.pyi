from typing import Any, list, Callable, Union
from .BasePerk import *
from .core import *
from .handles import *
from .menus import *
from .perk_structs import *
from .sdkhooks import *
from .sdktools import *
from .sourcemod import *
from .tf2 import *
from .tf2_stocks import *
from .zf_util_base import *
from .zf_util_fx import *
from .zf_util_pref import *


def GetTotalSurPerks() -> int:
    pass
def GetTotalZomPerks() -> int:
    pass
def GetPerkInfoString(registry: Any, typeList: Any, index: int, key: str, buffer: str, maxLen: int) -> None:
    pass
def GetSurPerkName(index: int, buffer: str, maxLen: int) -> None:
    pass
def GetSurPerkShortDesc(index: int, buffer: str, maxLen: int) -> None:
    pass
def GetSurPerkLongDesc(index: int, buffer: str, maxLen: int) -> None:
    pass
def GetZomPerkName(index: int, buffer: str, maxLen: int) -> None:
    pass
def GetZomPerkShortDesc(index: int, buffer: str, maxLen: int) -> None:
    pass
def GetZomPerkLongDesc(index: int, buffer: str, maxLen: int) -> None:
    pass
def RegisterSurvivorPerk(bp: Any) -> int:
    pass
def RegisterZombiePerk(bp: Any) -> int:
    """Registers a new zombie perk.
This should be called during perkInit().

@param bp    The BasePerk object to register.
@return      The registered index of the perk, or -1 on failure."""
    pass
def perkInit() -> None:
    pass
def command_zfPerkSetMode(client: int, args: int) -> Any:
    pass
def command_zfPerkSetTeamSurPerk(client: int, args: int) -> Any:
    pass
def command_zfPerkSetTeamZomPerk(client: int, args: int) -> Any:
    pass
def command_zfPerkList(client: int, args: int) -> Any:
    pass
def command_zfPerkEnable(client: int, args: int) -> Any:
    pass
def command_zfPerkDisable(client: int, args: int) -> Any:
    pass
def command_perkUpdate(client: int, args: int, doEnable: bool) -> Any:
    pass
def command_zfPerkLimit(client: int, args: int) -> Any:
    pass
def hook_zfSelectPerk(client: int, command: str, argc: int) -> Any:
    pass
def getStat(client: int, stat: ZFStat) -> int:
    pass
def getStatType(client: int, stat: ZFStat, type: ZFStatType) -> int:
    pass
def addStat(client: int, stat: ZFStat, type: ZFStatType, val: int) -> None:
    pass
def addStatTempStack(client: int, stat: ZFStat, newStr: int, newDur: int) -> None:
    pass
def addStatTempExtend(client: int, stat: ZFStat, newStr: int, newDur: int) -> None:
    pass
def scaleStatTempPct(client: int, stat: ZFStat, strPct: float, durPct: float = ...) -> None:
    pass
def getCond(client: int, cond: ZFCond) -> bool:
    pass
def addCond(client: int, cond: ZFCond, val: int) -> None:
    pass
def subCond(client: int, cond: ZFCond, val: int) -> None:
    pass
def resetAllClients() -> None:
    pass
def resetClient(client: int) -> None:
    pass
def resetClientStats(client: int) -> None:
    pass
def resetStatType(type: ZFStatType) -> None:
    pass
def resetClientStatType(client: int, type: ZFStatType) -> None:
    pass
def resetClientConds(client: int) -> None:
    pass
def surPerkEnabled(perk: int) -> bool:
    pass
def zomPerkEnabled(perk: int) -> bool:
    pass
def usingSurPerk(client: int, perk: int) -> bool:
    pass
def usingZomPerk(client: int, perk: int) -> bool:
    pass
def selectSurPerk(client: int, perk: int) -> None:
    pass
def selectZomPerk(client: int, perk: int) -> None:
    pass
def surPerkAtLimit(client: int, perk: int) -> bool:
    pass
def zomPerkAtLimit(client: int, perk: int) -> bool:
    pass
def perk_menuSurPerkList(menu: Any, action: MenuAction, param1: int, param2: int) -> None:
    pass
def perk_menuZomPerkList(menu: Any, action: MenuAction, param1: int, param2: int) -> None:
    pass
def panel_PrintSurPerkSelect(client: int, perk: int) -> None:
    pass
def panel_HandleSurPerkSelect(menu: Any, action: MenuAction, param1: int, param2: int) -> None:
    pass
def panel_PrintZomPerkSelect(client: int, perk: int) -> None:
    pass
def panel_HandleZomPerkSelect(menu: Any, action: MenuAction, param1: int, param2: int) -> None:
    pass
def createAura(client: int, strPart: str, attachStyle: ZFParticleAttachStyle, offset: list[float] = ...) -> None:
    pass
def removeAura(client: int) -> None:
    pass
def validAura(client: int) -> bool:
    pass
def showAura(client: int) -> None:
    pass
def hideAura(client: int) -> None:
    pass
def createIcon(ownerClient: int, targetClient: int, strSprite: str) -> None:
    pass
def removeIcon(client: int) -> None:
    pass
def validIcon(client: int) -> bool:
    pass
def showIcon(client: int) -> None:
    pass
def hideIcon(client: int) -> None:
    pass
def removeItem(client: int, item: int) -> None:
    pass
def removeItems(client: int) -> None:
    pass
def validItem(client: int, itemIndex: int) -> bool:
    pass
def getItemMetadata(item: int) -> int:
    pass
def setItemMetadata(item: int, value: int) -> None:
    pass
def getFreeItemIndex(client: int, maxItems: int) -> int:
    pass
def addHealth(client: int, health: int, doOverheal: bool = ...) -> None:
    pass
def doItemCollide(ent: int, prevPos: list[float], hitPos: list[float], hitVec: list[float]) -> bool:
    pass
def TraceFilter(ent: int, contentMask: int) -> bool:
    pass
def doItemImpact(client: int, hitPos: list[float], hitVec: list[float], color: list[int]) -> int:
    pass
def doItemPlace(client: int, strModel: str) -> int:
    pass
def doItemThrow(client: int, strModel: str, force: float, color: list[int] = ...) -> int:
    pass
def doCarpenterBuild(client: int, physPos: list[float]) -> int:
    pass
def doFriendSelect(client: int, desiredFriend: int = ...) -> None:
    pass
def doMarkedSelect(client: int) -> None:
    pass
def doNinjaDecoyPlace(client: int) -> None:
    pass
def doNinjaDecoyPoof(client: int) -> None:
    pass
def doThievingLimit(client: int) -> None:
    pass
def doThievingSteal(attacker: int, victim: int, slot: int) -> None:
    pass
def updateClientPermStats(client: int) -> None:
    pass
def updateClientPermEffects(client: int) -> None:
    pass
def updateCondStats() -> None:
    pass
def updateTempStats() -> None:
    pass
def updateConds() -> None:
    pass
def perk_OnPeriodic() -> None:
    pass
def perk_OnGameFrame() -> None:
    pass
def perk_OnMapStart() -> None:
    pass
def perk_OnMapEnd() -> None:
    pass
def perk_OnClientConnect(client: int) -> None:
    pass
def perk_OnClientDisconnect(client: int) -> None:
    pass
def perk_OnRoundStart() -> None:
    pass
def perk_OnGraceEnd() -> None:
    pass
def perk_OnRoundEnd() -> None:
    pass
def perk_OnEntityCreated(entity: int, classname: str) -> None:
    pass
def perk_OnEntitySpawn(entity: int) -> None:
    pass
def perk_OnSetTransmit(entity: int, client: int) -> Any:
    pass
def perk_OnCharitableGiftTouched(entity: int, other: int) -> None:
    pass
def OnPlayerRunCmd(client: int, buttons: int, impulse: int, vel: list[float], angles: list[float], weapon: int) -> Any:
    pass
def perk_OnCalcIsAttackCritical(client: int) -> None:
    pass
def perk_OnFenceTakeDamage(victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> Any:
    pass
def perk_OnTakeDamage(victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> Any:
    pass
def perk_OnTakeDamagePost(victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> None:
    pass
def perk_OnTouch(toucher: int, touchee: int) -> None:
    pass
def perk_OnPlayerSpawn(client: int) -> None:
    pass
def perk_OnPlayerDeath(victim: int, killer: int, assist: int, inflictor: int, damagetype: int) -> None:
    pass
def perk_OnCallForMedic(client: int) -> Any:
    pass
def perk_OnAmmoPickup(client: int, pickup: int) -> None:
    pass
def perk_OnMedPickup(client: int, pickup: int) -> None:
    pass
def perk_tSpawnClient(timer: Any, client: Any) -> None:
    pass
def perk_tNinjaDecoyPoof(Timer: Any, client: Any) -> None:
    pass
def perk_tSickSpit(timer: Any, dataPack: Any) -> None:
    pass
def perk_tTarredSpit(timer: Any, dataPack: Any) -> None:
    pass
def perk_tZenlikeAttack(timer: Any, client: Any) -> None:
    pass
ZF_PERK_NONE: Any = ...  # 0
ZF_PERK_ATHLETIC: Any = ...  # 1
ZF_PERK_CARPENTER: Any = ...  # 2
ZF_PERK_CHARITABLE: Any = ...  # 3
ZF_PERK_COWARDLY: Any = ...  # 4
ZF_PERK_FRIEND: Any = ...  # 5
ZF_PERK_HEROIC: Any = ...  # 6
ZF_PERK_HOLY: Any = ...  # 7
ZF_PERK_JUGGERNAUT: Any = ...  # 8
ZF_PERK_LEADER: Any = ...  # 9
ZF_PERK_NINJA: Any = ...  # 10
ZF_PERK_NONLETHAL: Any = ...  # 11
ZF_PERK_RESOURCEFUL: Any = ...  # 12
ZF_PERK_SELFLESS: Any = ...  # 13
ZF_PERK_STASH: Any = ...  # 14
ZF_PERK_STIRCRAZY: Any = ...  # 15
ZF_PERK_SUPPLIER: Any = ...  # 16
ZF_PERK_TANTRUM: Any = ...  # 17
ZF_PERK_TRAPPER: Any = ...  # 18
ZF_PERK_TURTLE: Any = ...  # 19
ZF_PERK_WISE: Any = ...  # 20
ZF_PERK_ZENLIKE: Any = ...  # 21
ZF_PERK_ALPHA: Any = ...  # 1
ZF_PERK_COMBUSTIBLE: Any = ...  # 2
ZF_PERK_HORRIFYING: Any = ...  # 3
ZF_PERK_HUNTER: Any = ...  # 4
ZF_PERK_LEAP: Any = ...  # 5
ZF_PERK_MAGNETIC: Any = ...  # 6
ZF_PERK_MARKED: Any = ...  # 7
ZF_PERK_RAGE: Any = ...  # 8
ZF_PERK_ROAR: Any = ...  # 9
ZF_PERK_SCORCHING: Any = ...  # 10
ZF_PERK_SICK: Any = ...  # 11
ZF_PERK_SWARMING: Any = ...  # 12
ZF_PERK_TARRED: Any = ...  # 13
ZF_PERK_THIEVING: Any = ...  # 14
ZF_PERK_TOXIC: Any = ...  # 15
ZF_PERK_VAMPIRIC: Any = ...  # 16
ZF_PERK_VINDICTIVE: Any = ...  # 17
ICON_SPR: Any = ...  # 0
ICON_ANC: Any = ...  # 1
classname: str = ...
perkInfo: Any = ...
listSur: bool = ...
listZom: bool = ...
setSur: bool = ...
setZom: bool = ...
limit: int = ...
oldStr: int = ...
oldDur: int = ...
total: int = ...
finalDur: int = ...
finalStr: int = ...
killer: int = ...
spectate: int = ...
menu: Any = ...
panel: Any = ...
firstItem: int = ...
cur: int = ...
lim: int = ...
didHit: bool = ...
TraceEx: Any = ...
color: int = ...
off: float = ...
selectedFriend: int = ...
health: int = ...
ammo: int = ...
headOffset: float = ...
thisZom: int = ...
validSurPerkCount: int = ...
validZomPerkCount: int = ...
giftOwner: int = ...
giftIndex: int = ...
randStat: int = ...
randBonus: int = ...
prevButtons: int = ...
fenceOwner: int = ...
fenceIndex: int = ...
js: int = ...
nextPerk: int = ...
perkChange: bool = ...
teamChange: bool = ...
client: int = ...
entIdx: int = ...