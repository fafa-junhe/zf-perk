from include.zf_perk import addStat
from include.perk_structs import ZFStat, ZFStatType
from python.perks.survival import SurvivorBasePerk

ZF_ATHLETIC_ATTACK = -40          # 改为 -40 ，降低该职业的输出能力
ZF_ATHLETIC_CRIT = -100
ZF_ATHLETIC_ROF = 100      
ZF_ATHLETIC_SPEED = 100      

class AthleticPerk(SurvivorBasePerk):
    __NAME__ = "Athletic"
    __SHORTDESC__ = "Faster movement and ROF"
    __DESC__ = "Faster movement and ROF!!"
    def __init__(self, client: int):
        super().__init__(client)

    def updateClientPermStats(self) -> None:
        addStat(self.client, ZFStat.ZFStatAtt, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_ATTACK)
        addStat(self.client, ZFStat.ZFStatCrit, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_CRIT)
        addStat(self.client, ZFStat.ZFStatRof, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_ROF)
        addStat(self.client, ZFStat.ZFStatSpeed, ZFStatType.ZFStatTypePerm, ZF_ATHLETIC_SPEED)