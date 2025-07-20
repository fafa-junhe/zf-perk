// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/zombie/ZombieBasePerk.py

#if defined __ZombieBasePerk_included
#endinput
#endif
#define __ZombieBasePerk_included

#define DP(%1) view_as<DataPackPos>(%1)
#include "../BasePerk.inc"
#include <datapack>

methodmap ZombieBasePerk < BasePerk {
    public ZombieBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        ZombieBasePerk sm = view_as<ZombieBasePerk>(sm_base);

        sm.Position = DP(1);
        sm.WriteString("zooooooo");

        return sm;
    }
}

