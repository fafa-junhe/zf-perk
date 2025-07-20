// Generated from D:\workspace\code\tf2\zf\src\python\perks\Registraion.py

#if defined __Registraion_included
#endinput
#endif
#define __Registraion_included

#include "../zf_perk.inc"
#include "../zf_util_base.inc"
#include "survival/AthleticPerk.inc"
#include "survival/SurvivorBasePerk.inc"
#include "zombie/ZombieBasePerk.inc"
#include <adt_trie>


stock void registerSurvivorPerks() {
    RegisterSurvivorPerk(SurvivorBasePerk);
    RegisterSurvivorPerk(AthleticPerk);
}

stock void registerZombiePerks() {
    RegisterZombiePerk(ZombieBasePerk);
}

