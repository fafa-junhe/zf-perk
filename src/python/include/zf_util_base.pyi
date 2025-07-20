from typing import Any, list, Callable, Union
from .sdkhooks import *
from .sdktools import *
from .sourcemod import *
from .tf2 import *
from .tf2_stocks import *
from .weapondata import *


class ZFRoundState:
    RoundActive: int = ...
    RoundGrace: int = ...
    RoundInit1: int = ...
    RoundInit2: int = ...
    RoundPost: int = ...


def max(a: int, b: int) -> int:
    pass
def min(a: int, b: int) -> int:
    pass
def fMax(a: float, b: float) -> float:
    pass
def fMin(a: float, b: float) -> float:
    pass
def zomTeam() -> int:
    pass
def surTeam() -> int:
    pass
def setZomTeam(_0: Any) -> None:
    pass
def setSurTeam(_0: Any) -> None:
    pass
def isZom(client: int) -> int:
    pass
def isSur(client: int) -> int:
    pass
def validClient(client: int) -> bool:
    pass
def validSur(client: int) -> bool:
    pass
def validZom(client: int) -> bool:
    pass
def validLivingClient(client: int) -> bool:
    pass
def validLivingSur(client: int) -> bool:
    pass
def validLivingZom(client: int) -> bool:
    pass
def validZombie(___class: int) -> bool:
    pass
def validSurvivor(___class: int) -> bool:
    pass
def randomZombie() -> int:
    pass
def randomSurvivor() -> int:
    pass
def isEngineer(client: int) -> bool:
    pass
def isHeavy(client: int) -> bool:
    pass
def isMedic(client: int) -> bool:
    pass
def isPyro(client: int) -> bool:
    pass
def isScout(client: int) -> bool:
    pass
def isSpy(client: int) -> bool:
    pass
def mapIsZF() -> bool:
    pass
def mapIsPL() -> bool:
    pass
def mapIsCP() -> bool:
    pass
def setRoundState(_state: ZFRoundState) -> None:
    pass
def roundState() -> ZFRoundState:
    pass
def endRound(winningTeam: int) -> None:
    pass
def activeWeapon(client: int) -> int:
    pass
def activeWeaponId(client: int) -> int:
    pass
def slotWeaponId(client: int, slot: int) -> int:
    pass
def activeWeaponSlot(client: int) -> int:
    pass
def isEquipped(client: int, weaponId: int) -> bool:
    pass
def isWielding(client: int, weaponId: int) -> bool:
    pass
def isWieldingMelee(client: int) -> bool:
    pass
def isWieldingAuto(client: int) -> bool:
    pass
def isWieldingBullet(client: int) -> bool:
    pass
def switchToSlot(client: int, slot: int) -> None:
    pass
def stripWeapons(client: int, keepPri: bool = ...) -> None:
    pass
def stripWeaponSlot(client: int, slot: int) -> None:
    pass
def setWeaponRof(weapon: int, rateScale: int) -> None:
    pass
def attackWasBackstab(attacker: int, inflictor: int, damagetype: int) -> bool:
    pass
def attackWasMelee(attacker: int, inflictor: int, damagetype: int) -> bool:
    pass
def attackWasBullet(attacker: int, inflictor: int) -> bool:
    pass
def attackWasProjectile(attacker: int, inflictor: int) -> bool:
    pass
def attackWasSelfFall(inflictor: int, damagetype: int) -> bool:
    pass
def attackWasBleed(_0: Any) -> bool:
    pass
def attackWasBludgeon(_0: Any) -> bool:
    pass
def attackWasEnvExplosion(_0: Any) -> bool:
    pass
def attackWasExplosive(_0: Any) -> bool:
    pass
def attackWasFall(_0: Any) -> bool:
    pass
def attackWasFire(_0: Any) -> bool:
    pass
def addCondKritz(client: int, duration: float) -> None:
    pass
def remCondKritz(client: int) -> None:
    pass
def isSlowed(client: int) -> bool:
    pass
def isKritzed(client: int) -> bool:
    pass
def isBonked(client: int) -> bool:
    pass
def isDazed(client: int) -> bool:
    pass
def isCharging(client: int) -> bool:
    pass
def isBeingHealed(client: int) -> bool:
    pass
def isCloaked(client: int) -> bool:
    pass
def isUbered(client: int) -> bool:
    pass
def isOnFire(client: int) -> bool:
    pass
def addFlagNoTarget(client: int) -> None:
    pass
def remFlagNoTarget(client: int) -> None:
    pass
def isGrounded(client: int) -> bool:
    pass
def isCrouching(client: int) -> bool:
    pass
def addInvincibility(client: int) -> None:
    pass
def remInvincibility(client: int) -> None:
    pass
def isInWater(client: int) -> bool:
    pass
def isNotMoving(client: int) -> bool:
    pass
def getEntityPos(entity: int, pos: list[float]) -> None:
    pass
def getHealthPct(client: int) -> float:
    pass
def setHealthPct(client: int, healthPct: float) -> None:
    pass
def clientMaxHealth(client: int) -> int:
    pass
def setClientSpeed(client: int, speed: float) -> None:
    pass
def clientBaseSpeed(client: int) -> float:
    pass
def clientBonusSpeed(client: int) -> float:
    pass
def entClassnameContains(ent: int, strRefClassname: str) -> bool:
    pass
def setGlow(client: int, glowEnabled: bool) -> None:
    pass
def entIsSentry(_0: Any) -> bool:
    pass
def getCloak(client: int) -> float:
    pass
def setCloak(client: int, cloakPct: float) -> None:
    pass
def addUber(client: int, uberPct: float) -> None:
    pass
def subUber(client: int, uberPct: float) -> None:
    pass
def addMetalPct(client: int, metalPct: float, metalLimitPct: float = ...) -> None:
    pass
def subMetalPct(client: int, metalPct: float) -> None:
    pass
def addMetal(client: int, metal: int) -> None:
    pass
def subMetal(client: int, metal: int) -> None:
    pass
def getMetal(client: int) -> int:
    pass
def setMetal(client: int, metal: int) -> None:
    pass
def addClipAmmoPct(client: int, slot: int, ammoPct: float, ammoLimitPct: float = ...) -> None:
    pass
def subClipAmmoPct(client: int, slot: int, ammoPct: float) -> None:
    pass
def addResAmmoPct(client: int, slot: int, ammoPct: float, ammoLimitPct: float = ...) -> None:
    pass
def subResAmmoPct(client: int, slot: int, ammoPct: float) -> None:
    pass
def addClipAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def subClipAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def addResAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def subResAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def getClipAmmoPct(client: int, slot: int) -> float:
    pass
def getResAmmoPct(client: int, slot: int) -> float:
    pass
def getClipAmmo(client: int, slot: int) -> int:
    pass
def setClipAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def getResAmmo(client: int, slot: int) -> int:
    pass
def setResAmmo(client: int, slot: int, ammo: int) -> None:
    pass
def maxClipAmmo(client: int, slot: int) -> int:
    pass
def maxResAmmo(client: int, slot: int) -> int:
    pass
def spawnClient(client: int, nextClientTeam: int) -> None:
    pass
def applyDamageRadialAtClient(_0: Any, _1: Any, _2: Any, doFx: bool = ...) -> None:
    pass
def applyDamageRadial(_0: Any, _1: Any, pos: list[float], _2: Any, doFx: bool = ...) -> None:
    pass
TF2_DMGTYPE_BLEED: Any = ...  # 0x4
TF2_DMGTYPE_FALL: Any = ...  # 0x20
TF2_DMGTYPE_EXPLOSIVE: Any = ...  # 0x40
TF2_DMGTYPE_BLUDGEON: Any = ...  # 0x80
TF2_DMGTYPE_FIRE: Any = ...  # 0x800
TF2_DMGTYPE_CRIT: Any = ...  # 0x100000
ZF_DMGTYPE_POISON: Any = ...  # 0x8000_0000
ZF_DAMAGERADIUS_NAME: Any = ...  # "zfdmgrad"
M_MAXRESAMMO: Any = ...  # 0x000000FF
S_MAXRESAMMO: Any = ...  # 0
M_MAXCLIPAMMO: Any = ...  # 0x0000FF00
S_MAXCLIPAMMO: Any = ...  # 8
F_ISAUTO: Any = ...  # 0x00010000
F_ISBULLET: Any = ...  # 0x00020000
F_NOSTRIP: Any = ...  # 0x00040000
MAX_ZF_WEAPONS: Any = ...  # 512
ZFWEAP_SCATTERGUN: Any = ...  # 13
ZFWEAP_FORCEANATURE: Any = ...  # 45
ZFWEAP_SHORTSTOP: Any = ...  # 220
ZFWEAP_SCOUTPISTOL: Any = ...  # 23
ZFWEAP_BONK: Any = ...  # 46
ZFWEAP_LUGERMORPH: Any = ...  # 160
ZFWEAP_CRITACOLA: Any = ...  # 163
ZFWEAP_MADMILK: Any = ...  # 222
ZFWEAP_BAT: Any = ...  # 0
ZFWEAP_SANDMAN: Any = ...  # 44
ZFWEAP_HOLYMACKAREL: Any = ...  # 221
ZFWEAP_CANDYCANE: Any = ...  # 317
ZFWEAP_BOSTONBASHER: Any = ...  # 325
ZFWEAP_SUNONASTICK: Any = ...  # 349
ZFWEAP_FANOWAR: Any = ...  # 355
ZFWEAP_SCATTERGUN_UPGRADE: Any = ...  # 200
ZFWEAP_PISTOL_UPGRADE: Any = ...  # 209
ZFWEAP_BAT_UPGRADE: Any = ...  # 190
ZFWEAP_SNIPERRIFLE: Any = ...  # 14
ZFWEAP_HUNTSMAN: Any = ...  # 56
ZFWEAP_SYDNEYSLEEPER: Any = ...  # 230
ZFWEAP_SMG: Any = ...  # 16
ZFWEAP_RAZORBACK: Any = ...  # 57
ZFWEAP_JARATE: Any = ...  # 58
ZFWEAP_DANGERSHIELD: Any = ...  # 231
ZFWEAP_KUKRI: Any = ...  # 3
ZFWEAP_SHIV: Any = ...  # 171
ZFWEAP_BUSHWACKA: Any = ...  # 232
ZFWEAP_SNIPERRIFLE_UPGRADE: Any = ...  # 201
ZFWEAP_SMG_UPGRADE: Any = ...  # 203
ZFWEAP_KUKRI_UPGRADE: Any = ...  # 193
ZFWEAP_ROCKETLAUNCHER: Any = ...  # 18
ZFWEAP_DIRECTHIT: Any = ...  # 127
ZFWEAP_BLACKBOX: Any = ...  # 228
ZFWEAP_ROCKETJUMPER: Any = ...  # 237
ZFWEAP_SOLDIERSHOTGUN: Any = ...  # 10
ZFWEAP_BUFFBANNER: Any = ...  # 129
ZFWEAP_GUNBOATS: Any = ...  # 133
ZFWEAP_BATTALIONBACKUP: Any = ...  # 226
ZFWEAP_CONCHEROR: Any = ...  # 354
ZFWEAP_SHOVEL: Any = ...  # 6
ZFWEAP_EQUALIZER: Any = ...  # 128
ZFWEAP_PAINTRAIN: Any = ...  # 154
ZFWEAP_FRYINGPAN: Any = ...  # 264
ZFWEAP_HALFZATOICHI: Any = ...  # 357
ZFWEAP_ROCKETLAUNCHER_UPGRADE: Any = ...  # 205
ZFWEAP_SHOTGUN_UPGRADE: Any = ...  # 199
ZFWEAP_SHOVEL_UPGRADE: Any = ...  # 196
ZFWEAP_GRENADELAUNCHER: Any = ...  # 19
ZFWEAP_LOCHNLOAD: Any = ...  # 308
ZFWEAP_STICKYLAUNCHER: Any = ...  # 20
ZFWEAP_SCOTTISHRESISTANCE: Any = ...  # 130
ZFWEAP_CHARGINTARGE: Any = ...  # 131
ZFWEAP_STICKYJUMPER: Any = ...  # 265
ZFWEAP_BOTTLE: Any = ...  # 1
ZFWEAP_EYELANDER: Any = ...  # 132
ZFWEAP_SKULLCUTTER: Any = ...  # 172
ZFWEAP_HHHHEADTAKER: Any = ...  # 266
ZFWEAP_ULLAPOOLCABER: Any = ...  # 307
ZFWEAP_CLAIDHEAMOHMOR: Any = ...  # 327
ZFWEAP_GRENADELAUNCHER_UPGRADE: Any = ...  # 206
ZFWEAP_STICKYLAUNCHER_UPGRADE: Any = ...  # 207
ZFWEAP_BOTTLE_UPGRADE: Any = ...  # 191
ZFWEAP_SYRINGEGUN: Any = ...  # 17
ZFWEAP_BLUTSAUGER: Any = ...  # 36
ZFWEAP_CRUSADERSCROSSBOW: Any = ...  # 305
ZFWEAP_MEDIGUN: Any = ...  # 29
ZFWEAP_KRITZKRIEG: Any = ...  # 35
ZFWEAP_BONESAW: Any = ...  # 8
ZFWEAP_UBERSAW: Any = ...  # 37
ZFWEAP_VITASAW: Any = ...  # 173
ZFWEAP_AMPUTATOR: Any = ...  # 304
ZFWEAP_SYRINGEGUN_UPGRADE: Any = ...  # 204
ZFWEAP_MEDIGUN_UPGRADE: Any = ...  # 211
ZFWEAP_BONESAW_UPGRADE: Any = ...  # 198
ZFWEAP_SASHA: Any = ...  # 15
ZFWEAP_NATASCHA: Any = ...  # 41
ZFWEAP_IRONCURTAIN: Any = ...  # 298
ZFWEAP_BRASSBEAST: Any = ...  # 312
ZFWEAP_HEAVYSHOTGUN: Any = ...  # 11
ZFWEAP_SANDVICH: Any = ...  # 42
ZFWEAP_DALOKOHSBAR: Any = ...  # 159
ZFWEAP_BUFFALOSTEAK: Any = ...  # 311
ZFWEAP_FISTS: Any = ...  # 5
ZFWEAP_KGB: Any = ...  # 43
ZFWEAP_GRU: Any = ...  # 239
ZFWEAP_WARRIORSSPIRIT: Any = ...  # 310
ZFWEAP_FISTSOFSTEEL: Any = ...  # 331
ZFWEAP_SASHA_UPGRADE: Any = ...  # 202
ZFWEAP_FISTS_UPGRADE: Any = ...  # 195
ZFWEAP_FLAMETHROWER: Any = ...  # 21
ZFWEAP_BACKBURNER: Any = ...  # 40
ZFWEAP_DEGREASER: Any = ...  # 215
ZFWEAP_PYROSHOTGUN: Any = ...  # 12
ZFWEAP_FLAREGUN: Any = ...  # 39
ZFWEAP_FIREAXE: Any = ...  # 2
ZFWEAP_AXETINGUSIHER: Any = ...  # 38
ZFWEAP_HOMEWRECKER: Any = ...  # 153
ZFWEAP_POWERJACK: Any = ...  # 214
ZFWEAP_BACKSCRATCHER: Any = ...  # 326
ZFWEAP_VOLCANOFRAGMENT: Any = ...  # 348
ZFWEAP_FLAMETHROWER_UPGRADE: Any = ...  # 208
ZFWEAP_FIREAXE_UPGRADE: Any = ...  # 192
ZFWEAP_REVOLVER: Any = ...  # 24
ZFWEAP_AMBASSADOR: Any = ...  # 61
ZFWEAP_BIGKILL: Any = ...  # 161
ZFWEAP_LETRANGER: Any = ...  # 224
ZFWEAP_KNIFE: Any = ...  # 4
ZFWEAP_ETERNALREWARD: Any = ...  # 225
ZFWEAP_CONNIVERSKUNAI: Any = ...  # 356
ZFWEAP_INVISWATCH: Any = ...  # 30
ZFWEAP_DEADRINGER: Any = ...  # 59
ZFWEAP_CLOAKANDDAGGER: Any = ...  # 60
ZFWEAP_TTGWATCH: Any = ...  # 297
ZFWEAP_REVOLVER_UPGRADE: Any = ...  # 210
ZFWEAP_KNIFE_UPGRADE: Any = ...  # 194
ZFWEAP_INVISWATCH_UPGRADE: Any = ...  # 212
ZFWEAP_ENGINEERSHOTGUN: Any = ...  # 9
ZFWEAP_FRONTIERJUSTICE: Any = ...  # 141
ZFWEAP_ENGINEERPISTOL: Any = ...  # 22
ZFWEAP_WRANGLER: Any = ...  # 140
ZFWEAP_WRENCH: Any = ...  # 7
ZFWEAP_GUNSLINGER: Any = ...  # 142
ZFWEAP_SOUTHERNHOSPITALITY: Any = ...  # 155
ZFWEAP_GOLDENWRENCH: Any = ...  # 169
ZFWEAP_JAG: Any = ...  # 329
ZFWEAP_BUILDPDA: Any = ...  # 25
ZFWEAP_DESTROYPDA: Any = ...  # 26
ZFWEAP_WRENCH_UPGRADE: Any = ...  # 197
ZF_WEAPON_DATA: int = ...
zf_zomTeam: int = ...
zf_surTeam: int = ...
index: int = ...
weapon: int = ...
weaponId: int = ...
newHealth: int = ...
mh: int = ...
heads: int = ...
curH: int = ...
curPct: float = ...
curMetal: int = ...
maxMetal: int = ...
metal: int = ...
curAmmo: int = ...
maxAmmo: int = ...
ammo: int = ...
newAmmo: int = ...
nextClientClass: int = ...
ent: int = ...