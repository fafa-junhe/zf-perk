#if defined __SelflessPerk_included
#endinput
#endif
#define __SelflessPerk_included

#include "../../perk_structs.inc"
#include "../../zf_perk.inc"
#include "SurvivorBasePerk.inc"
#include <datapack>
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"

#define ZF_SELFLESS_DAMAGE 10000
#define ZF_SELFLESS_RADIUS 5000

methodmap SelflessPerk < SurvivorBasePerk {
    public SelflessPerk(int client) {
        SurvivorBasePerk sm_base = new SurvivorBasePerk(client);
        SelflessPerk sm = view_as<SelflessPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, SelflessPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, SelflessPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, SelflessPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_DEATH, SelflessPerkFonPlayerDeath);

        return sm;
    }
}

stock SurvivorBasePerk SelflessPerkFnew(int client) {
    return new SelflessPerk(client);
}


FUNCTION(SelflessPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Selfless");
}

FUNCTION(SelflessPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Explode on death");
}

FUNCTION(SelflessPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "You explode on death, dealing massive damage to nearby zombies.");
}

FUNCTION(SelflessPerk, onPlayerDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (victim == _inst.client && validZom(killer))
    {
        // Create explosion effect
        fxExplosionBig(victim);

        // Damage zombies in radius
        applyDamageRadialAtClient(victim, ZF_SELFLESS_DAMAGE, ZF_SELFLESS_RADIUS, false);
    }
}