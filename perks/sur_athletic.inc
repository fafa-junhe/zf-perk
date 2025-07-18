#pragma newdecls required

#if defined _ZF_SUR_ATHLETIC_PERK_INC
  #endinput
#endif
#define _ZF_SUR_ATHLETIC_PERK_INC

#include "perks/base_perk.inc"
#include "perks/perk_structs.inc"

/**
 * Athletic Perk.
 * High mobility.
 */
methodmap AthleticPerk < base_perk
{
    /**
     * Constructor.
     */
    public AthleticPerk(int owner)
    {
        return view_as<AthleticPerk>(base_perk(owner));
    }

    /**
     * Called when the player spawns.
     * We set the permanent stats here.
     */
    public void onSpawn()
    {
        // Set permanent stats for Athletic perk
        char key[64];

        Format(key, sizeof(key), "stat_%d_perm", ZFStatAtt);
        this.SetValue(key, ZF_ATHLETIC_ATTACK);

        Format(key, sizeof(key), "stat_%d_perm", ZFStatCrit);
        this.SetValue(key, ZF_ATHLETIC_CRIT);

        Format(key, sizeof(key), "stat_%d_perm", ZFStatRof);
        this.SetValue(key, ZF_ATHLETIC_ROF);

        Format(key, sizeof(key), "stat_%d_perm", ZFStatSpeed);
        this.SetValue(key, ZF_ATHLETIC_SPEED);
    }
}