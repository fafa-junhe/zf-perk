from include.zf_perk import RegisterSurvivorPerk, RegisterZombiePerk
from perks.survival import AthleticPerk
from python.perks.survival import SurvivorBasePerk
from python.perks.zombie import ZombieBasePerk

def registerSurvivorPerks():
    RegisterSurvivorPerk(SurvivorBasePerk(-1)) # P:RegisterSurvivorPerk(SurvivorBasePerk);
    RegisterSurvivorPerk(AthleticPerk(-1)) # P:RegisterSurvivorPerk(AthleticPerk);

def registerZombiePerks():
    RegisterZombiePerk(ZombieBasePerk(-1)) # P:RegisterZombiePerk(ZombieBasePerk);
