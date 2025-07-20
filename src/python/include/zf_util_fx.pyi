from typing import Any, list, Callable, Union
from .zf_util_base import *


class ZFParticleAttachStyle:
    AttachBack: int = ...
    AttachBase: int = ...
    AttachHead: int = ...
    AttachNone: int = ...


def utilFxInit() -> None:
    pass
def utilFxPrecache() -> None:
    pass
def fxSetClientColor(client: int, r: int, g: int, b: int, a: int, colorWeapons: bool = ..., colorWearables: bool = ...) -> None:
    pass
def fxBloodBurst(client: int) -> None:
    pass
def fxBloodSpray(client: int) -> None:
    pass
def fxDeathScream(client: int) -> None:
    pass
def fxEvilLaughToAll(client: int) -> None:
    pass
def fxEvilLaughToClient(evilClient: int, otherClient: int) -> None:
    pass
def fxHealthGained(client: int) -> None:
    pass
def fxHealthLost(client: int) -> None:
    pass
def fxHealthMist(client: int) -> None:
    pass
def fxKritzStart(client: int) -> None:
    pass
def fxKritzStop(client: int) -> None:
    pass
def fxPain(client: int) -> None:
    pass
def fxPowerup(client: int) -> None:
    pass
def fxTeleportTrail(client: int, duration: float) -> None:
    pass
def fxYikes(client: int) -> None:
    pass
def fxJump(client: int, force: float, doVert: bool = ...) -> None:
    pass
def fxKnockback(targetClient: int, sourceEntity: int, force: float) -> None:
    pass
def fxApplyForce(entity: int, direction: list[float], force: float, forceMinVert: bool = ...) -> None:
    pass
def fxBits(entity: int) -> None:
    pass
def fxExplosionBig(entity: int) -> None:
    pass
def fxExplosionTiny(entity: int) -> None:
    pass
def fxExplosionParty(entity: int) -> None:
    pass
def fxPuffBig(entity: int) -> None:
    pass
def fxPuffSmall(entity: int) -> None:
    pass
def fxSmoke(entity: int) -> None:
    pass
def fxSpark(entity: int) -> None:
    pass
def fxTrailConfetti(entity: int, duration: float) -> None:
    pass
def fxCreateModelThrown(strModel: str, client: int, pos: list[float] = ..., ang: list[float] = ..., force: float = ..., color: list[int] = ...) -> int:
    pass
def fxCreateModelStatic(strModel: str, client: int, isOwned: bool = ..., isSolid: bool = ..., pos: list[float] = ..., ang: list[float] = ..., color: list[int] = ...) -> int:
    pass
def fxDeleteModel(ent: int) -> None:
    pass
def fxIsModelValid(ent: int) -> bool:
    pass
def fxCreateParticle(strPart: str, target: int, attachStyle: ZFParticleAttachStyle, duration: float = ..., posOffset: list[float] = ...) -> int:
    pass
def fxDeleteParticle(ent: int) -> None:
    pass
def fxStartParticle(ent: int) -> None:
    pass
def fxStopParticle(ent: int) -> None:
    pass
def fxIsParticleValid(ent: int) -> bool:
    pass
def fxCreateSprite(strSprite: str, target: int, entSpr: int, entAnc: int) -> None:
    pass
def fxDeleteSprite(entSpr: int, entAnc: int) -> None:
    pass
def fxShowSprite(entSpr: int, entAnc: int) -> None:
    pass
def fxHideSprite(entSpr: int, entAnc: int) -> None:
    pass
def fxIsSpriteValid(entSpr: int, entAnc: int) -> bool:
    pass
def fxCreateSoundToAll(strSound: str, entity: int) -> None:
    pass
def fxCreateSoundToClient(strSound: str, client: int) -> None:
    pass
weapon: int = ...
index: int = ...
clientClass: int = ...
evilClientClass: int = ...
offset: float = ...
pos: float = ...
ang: float = ...
color: int = ...
ent: int = ...
posOffset: float = ...