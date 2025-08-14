#if defined __VindictivePerk_included
#endinput
#endif
#define __VindictivePerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include <sdktools_functions>

// Defines from all_perks.md
// --- Constants ---
// Caps
#define ZF_VINDICTIVE_DAMAGE_CAP 50
#define ZF_VINDICTIVE_UTILITY_CAP 25
// Kill bonuses
#define ZF_VINDICTIVE_DAMAGE_ON_KILL 10
#define ZF_VINDICTIVE_UTILITY_ON_KILL 5
// Assist bonuses
#define ZF_VINDICTIVE_DAMAGE_ON_ASSIST 5
#define ZF_VINDICTIVE_UTILITY_ON_ASSIST 2 // 2.5 is not an integer, using 2

// --- DataPack Slots ---
#define VINDICTIVE_DATA_DAMAGE_BONUS (PERK_DATA_START_INDEX + 0)
#define VINDICTIVE_DATA_UTILITY_BONUS (PERK_DATA_START_INDEX + 1)

methodmap VindictivePerk < ZombieBasePerk {
    public VindictivePerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        VindictivePerk sm = view_as<VindictivePerk>(sm_base);

        PERK_REGISTER_BASIC_INFO(sm, VindictivePerk);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_KILL, VindictivePerkFonKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_ASSIST_KILL, VindictivePerkFonAssistKill);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, VindictivePerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, VindictivePerkFupdateCondStats);

        // Initialize data pack
        sm.Position = view_as<DataPackPos>(VINDICTIVE_DATA_DAMAGE_BONUS);
        sm.WriteCell(0);
        sm.Position = view_as<DataPackPos>(VINDICTIVE_DATA_UTILITY_BONUS);
        sm.WriteCell(0);

        return sm;
    }

    property int damage_bonus {
        public get() {
            this.Position = view_as<DataPackPos>(VINDICTIVE_DATA_DAMAGE_BONUS);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(VINDICTIVE_DATA_DAMAGE_BONUS);
            this.WriteCell(value);
        }
    }

    property int utility_bonus {
        public get() {
            this.Position = view_as<DataPackPos>(VINDICTIVE_DATA_UTILITY_BONUS);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(VINDICTIVE_DATA_UTILITY_BONUS);
            this.WriteCell(value);
        }
    }
}

stock BasePerk VindictivePerkFnew(int client) {
    return new VindictivePerk(client);
}

FUNCTION(VindictivePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Vindictive");
}

FUNCTION(VindictivePerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VindictivePerk_shortdesc");
}

FUNCTION(VindictivePerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "VindictivePerk_desc");
}


FUNCTION_INT(VindictivePerk, getCategory))
{
    return 1;
}


FUNCTION(VindictivePerk, updateClientPermStats)) {
    // No longer used for permanent stats, handled by conditional stats now.
}

FUNCTION(VindictivePerk, onKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    // Add kill bonus, respecting the cap
    _inst.damage_bonus = min(_inst.damage_bonus + ZF_VINDICTIVE_DAMAGE_ON_KILL, ZF_VINDICTIVE_DAMAGE_CAP);
    _inst.utility_bonus = min(_inst.utility_bonus + ZF_VINDICTIVE_UTILITY_ON_KILL, ZF_VINDICTIVE_UTILITY_CAP);
}

FUNCTION(VindictivePerk, onAssistKill), int victim, int killer, int assist, int inflictor, int damagetype) {
    // Add assist bonus, respecting the cap
    _inst.damage_bonus = min(_inst.damage_bonus + ZF_VINDICTIVE_DAMAGE_ON_ASSIST, ZF_VINDICTIVE_DAMAGE_CAP);
    _inst.utility_bonus = min(_inst.utility_bonus + ZF_VINDICTIVE_UTILITY_ON_ASSIST, ZF_VINDICTIVE_UTILITY_CAP);
}

FUNCTION(VindictivePerk, updateCondStats)) {
    // Apply the stored bonuses as conditional stats
    int client = _inst.client;
    addStat(client, ZFStatAtt, ZFStatTypeCond, _inst.damage_bonus);
    addStat(client, ZFStatDef, ZFStatTypeCond, _inst.damage_bonus);
    addStat(client, ZFStatSpeed, ZFStatTypeCond, _inst.utility_bonus);
    addStat(client, ZFStatCrit, ZFStatTypeCond, _inst.utility_bonus);
}
