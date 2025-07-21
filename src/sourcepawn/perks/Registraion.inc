// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/Registraion.py

#if defined __Registraion_included
#endinput
#endif
#define __Registraion_included

#include "../zf_perk.inc"
#include "survival/AthleticPerk.inc"
#include "survival/SurvivorBasePerk.inc"
#include "zombie/ZombieBasePerk.inc"
#include <datapack>


public void registerSurvivorPerks() {
    RegisterSurvivorPerk(SurvivorBasePerk);
    RegisterSurvivorPerk(AthleticPerk);
}

public void registerZombiePerks() {
    RegisterZombiePerk(ZombieBasePerk);
}

