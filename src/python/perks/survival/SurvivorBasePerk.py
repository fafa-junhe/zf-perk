from python.perks import BasePerk


class SurvivorBasePerk(BasePerk):
    __NAME__ = "SurvivorUnselected"
    __SHORTDESC__ = "Unselected"
    __DESC__ = "Please select one perk to check info"
    
    def __init__(self, client: int):
        super().__init__(client)
        self.test = "nooooo"


    def onCharitableGiftTouched(self, entity: int, other: int):
        """
        当慈善家扔出的礼物被触摸时触发。
        """
        pass
