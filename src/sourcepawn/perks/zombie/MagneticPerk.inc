#if defined __MagneticPerk_included
#endinput
#endif
#define __MagneticPerk_included

#include "../../../../include/clients.inc"
#include "../../../../include/core.inc"
#include "../../../../include/entity.inc"
#include "../../../../include/entity_prop_stocks.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../../../include/tf2.inc"
#include "../../../../include/tf2_stocks.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "ZombieBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

// Defines from all_perks.md
#define ZF_MAGNETIC_RADIUSSQ (500.0 * 500.0)

methodmap MagneticPerk < ZombieBasePerk {
    public MagneticPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        MagneticPerk sm = view_as<MagneticPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, MagneticPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, MagneticPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, MagneticPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PERIODIC, MagneticPerkFonPeriodic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, MagneticPerkFonRemove);

        return sm;
    }
}

stock BasePerk MagneticPerkFnew(int client) {
    return new MagneticPerk(client);
}

FUNCTION(MagneticPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Magnetic");
}

FUNCTION(MagneticPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MagneticPerk_shortdesc");
}

FUNCTION(MagneticPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MagneticPerk_desc");
}

FUNCTION(MagneticPerk, onPeriodic)) {
    int client = _inst.client;
    float zom_pos[3];
    GetClientAbsOrigin(client, zom_pos);

    for (int i = MaxClients + 1; i <= GetMaxEntities(); i++) {
        if (!IsValidEntity(i)) {
            continue;
        }

        char classname[64];
        GetEdictClassname(i, classname, sizeof(classname));

        bool isBuilding = StrEqual(classname, "obj_sentrygun") || StrEqual(classname, "obj_dispenser") || StrEqual(classname, "obj_teleporter");

        if (isBuilding) {
            if (GetEntProp(i, Prop_Data, "m_iTeamNum") == zomTeam()) {
                continue;
            }

            float build_pos[3];
            GetEntPropVector(i, Prop_Send, "m_vecOrigin", build_pos);

            if (GetVectorDistance(zom_pos, build_pos, true) <= ZF_MAGNETIC_RADIUSSQ) {
                AcceptEntityInput(i, "SetDisabled");
                if (GetRandomInt(0, 5) == 0) {
                    fxSpark(i);
                }
            } else {
                AcceptEntityInput(i, "SetEnabled");
            }
        }
    }
}

FUNCTION(MagneticPerk, onRemove)) {
    for (int i = MaxClients + 1; i <= GetMaxEntities(); i++) {
        if (!IsValidEntity(i)) {
            continue;
        }

        char classname[64];
        GetEdictClassname(i, classname, sizeof(classname));
        
        bool isBuilding = StrEqual(classname, "obj_sentrygun") || StrEqual(classname, "obj_dispenser") || StrEqual(classname, "obj_teleporter");

        if (isBuilding) {
            if (GetEntProp(i, Prop_Data, "m_iTeamNum") != zomTeam()) {
                AcceptEntityInput(i, "SetEnabled");
            }
        }
    }
}
