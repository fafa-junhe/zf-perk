from python.perks import BasePerk


class ZombieBasePerk(BasePerk):
    __NAME__ = "Unselected"
    __SHORTDESC__ = "Without perks"
    __DESC__ = "Please select one perk to check info"
    
    def __init__(self, client: int):
        super().__init__(client)

