// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/zombie/ZombieBasePerk.py

#if defined __ZombieBasePerk_included
#endinput
#endif
#define __ZombieBasePerk_included

#include "../BasePerk.inc"
#include <datapack>

methodmap ZombieBasePerk < BasePerk {
    public ZombieBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        ZombieBasePerk sm = view_as<ZombieBasePerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteFunction(ZombieBasePerk_getName);
        sm.Position = DP(2);
        sm.WriteFunction(ZombieBasePerk_getShortdesc);
        sm.Position = DP(3);
        sm.WriteFunction(ZombieBasePerk_getDesc);

        return sm;
    }
}

stock BasePerk ZombieBasePerk_new(int client) {
    return new ZombieBasePerk(client);
}


public void ZombieBasePerk_getName(ZombieBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}

public void ZombieBasePerk_getShortdesc(ZombieBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Without perks");
}

public void ZombieBasePerk_getDesc(ZombieBasePerk _inst, char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Please select one perk to check info");
}

