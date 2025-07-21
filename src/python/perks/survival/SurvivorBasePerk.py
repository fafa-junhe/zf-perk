from python.perks import BasePerk


class SurvivorBasePerk(BasePerk):
    __NAME__ = "Unselected"
    __SHORTDESC__ = "Without perks"
    __DESC__ = "Please select one perk to check info"
    
    def __init__(self, client: int):
        super().__init__(client)


    def onCharitableGiftTouched(self, entity: int, other: int) -> None:
        """
        当慈善家扔出的礼物被触摸时触发。
        """
        pass
