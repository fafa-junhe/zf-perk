from include.zf_perk import RegisterSurvivorPerk
from perks.survival import AthleticPerk

def registerSurvivorPerks():
    RegisterSurvivorPerk(AthleticPerk(-1))

def registerZombiePerks():
    pass