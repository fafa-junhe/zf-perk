#if defined __SupplierPerk_included
#endinput
#endif
#define __SupplierPerk_included

#include "../../../../include/core.inc"
#include "../../../../include/helpers.inc"
#include "../../../../include/sourcemod.inc"
#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_SUPPLIER_MAX_ITEMS 2
#define ZF_SUPPLIER_TIMER 10
#define ZF_SUPPLIER_RADIUSSQ (75 * 75)
#define ZF_SUPPLIER_UPDATERATE 10
#define ZF_SUPPLIER_SELF_DEFEND 25
#define ZF_SUPPLIER_AMMOPCT_RESLIMIT 2.0
#define ZF_SUPPLIER_AMMOPCT_SELF 0.10
#define ZF_SUPPLIER_AMMOPCT_OTHER 0.25
#define ZF_SUPPLIER_RESUPPLY_COUNT 4
#define ZF_SUPPLIER_ATTACK 25
#define ZF_SUPPLIER_DURATION 10

methodmap SupplierPerk < SurvivorBasePerk {
    public SupplierPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        SupplierPerk sm = view_as<SupplierPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SupplierPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, SupplierPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, SupplierPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, SupplierPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, SupplierPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, SupplierPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, SupplierPerkFonPlayerDeath);

        return sm;
    }
}

stock SurvivorBasePerk SupplierPerkFnew(int client) {
    return new SupplierPerk(client);
}

FUNCTION(SupplierPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Supplier");
}

FUNCTION(SupplierPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Place ammo packs");
}

FUNCTION(SupplierPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "You periodically regenerate ammo and have double reserve ammo. Call for a medic to place a supply crate that other survivors can use to get ammo. 10s cooldown.");
}

FUNCTION(SupplierPerk, updateClientPermStats)) {
    int client = _inst.client;
    addStat(client, ZFStatDef, ZFStatTypePerm, ZF_SUPPLIER_SELF_DEFEND);
    addStat(client, ZFStatAtt, ZFStatTypePerm, RoundToFloor(ZF_SUPPLIER_AMMOPCT_RESLIMIT * 100.0));
}

FUNCTION(SupplierPerk, updateCondStats), char[] buffer, int maxlen) {
    int client = _inst.client;

    // Self-resupply
    if (zf_frameCounter % ZF_SUPPLIER_UPDATERATE == 0) {
        addResAmmoPct(client, 0, ZF_SUPPLIER_AMMOPCT_SELF);
        addResAmmoPct(client, 1, ZF_SUPPLIER_AMMOPCT_SELF);
    }

    // Supply crates
    for (int i = 0; i < ZF_SUPPLIER_MAX_ITEMS; i++) {
        int crate = zf_item[client][i];
        if (crate > 0 && IsValidEntity(crate)) {
            int uses = getItemMetadata(crate);
            if (uses <= 0) {
                removeItem(client, i);
                continue;
            }

            float crate_pos[3];
            GetEntPropVector(crate, Prop_Send, "m_vecOrigin", crate_pos);

            for (int j = 1; j <= MaxClients; j++) {
                if (validLivingSur(j) && j != client) {
                    float target_pos[3];
                    GetClientAbsOrigin(j, target_pos);
                    if (GetVectorDistance(crate_pos, target_pos, true) <= ZF_SUPPLIER_RADIUSSQ) {
                        addResAmmoPct(j, 0, ZF_SUPPLIER_AMMOPCT_OTHER);
                        addResAmmoPct(j, 1, ZF_SUPPLIER_AMMOPCT_OTHER);
                        setItemMetadata(crate, --uses);
                        if (uses <= 0) break;
                    }
                }
            }
        }
    }

    if (zf_perkTimer[client] > 0) {
        Format(buffer, maxlen, "(Cooldown: %ds)", zf_perkTimer[client]);
    } else {
        if (getFreeItemIndex(client, ZF_SUPPLIER_MAX_ITEMS) == -1) {
            Format(buffer, maxlen, "(Crates Full)");
        } else {
            Format(buffer, maxlen, "(Ready to Supply)");
        }
    }
}

FUNCTION(SupplierPerk, onCallForMedic)) {
    int client = _inst.client;

    if (zf_perkTimer[client] > 0) {
        return;
    }
    if (!isGrounded(client) || !isCrouching(client)) {
        return;
    }
    int itemIndex = getFreeItemIndex(client, ZF_SUPPLIER_MAX_ITEMS);
    if (itemIndex == -1) {
        return;
    }

    float pos[3];
    GetClientAbsOrigin(client, pos);

    int entity = fxCreateModelStatic(ZFMDL_SUPPLYCRATE, client, true, true, pos);
    if (entity != -1) {
        zf_item[client][itemIndex] = entity;
        setItemMetadata(entity, ZF_SUPPLIER_RESUPPLY_COUNT);
        zf_perkTimer[client] = ZF_SUPPLIER_TIMER;
    }
}

FUNCTION(SupplierPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client) {
        removeItems(_inst.client);
    }
}