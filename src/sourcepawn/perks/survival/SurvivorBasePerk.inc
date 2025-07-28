#if defined __SurvivorBasePerk_included
#endinput
#endif
#define __SurvivorBasePerk_included

#include "../BasePerk.inc"
#include <datapack>

#define VTABLE_ON_CHARITABLE_GIFT_TOUCHED (PERK_DATA_START_INDEX + 0)

methodmap SurvivorBasePerk < BasePerk {
    public SurvivorBasePerk(int client) {
        BasePerk sm_base = new BasePerk(client);
        SurvivorBasePerk sm = view_as<SurvivorBasePerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SurvivorBasePerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CHARITABLE_GIFT_TOUCHED, SurvivorBasePerkFonCharitableGiftTouched);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, SurvivorBasePerkFonDealDamagePost);

        return sm;
    }

    public void onCharitableGiftTouched(int entity, int other) {
        PERK_IMPL_EVENT_CELL_CELL(VTABLE_ON_CHARITABLE_GIFT_TOUCHED, entity, other);
    }
}

stock BasePerk SurvivorBasePerkFnew(int client) {
    return new SurvivorBasePerk(client);
}


FUNCTION(SurvivorBasePerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Unselected");
}


FUNCTION(SurvivorBasePerk, onCharitableGiftTouched), int entity, int other) {
    /**
     * 当慈善家扔出的礼物被触摸时触发。
     */
}

FUNCTION(SurvivorBasePerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    /**
     * 当玩家造成伤害时触发。
     */
}
