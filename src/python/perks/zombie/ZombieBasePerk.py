from python.perks import BasePerk


class ZombieBasePerk(BasePerk):
    __NAME__ = "ZombieUnselected"
    __SHORTDESC__ = "Unselected"
    __DESC__ = "Please select one perk to check info"
    
    def __init__(self, client: int):
        super().__init__(client)
        self.test = "zooooooo"

