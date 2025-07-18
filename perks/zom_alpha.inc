#pragma newdecls required

#if defined _ZF_ZOM_ALPHA_PERK_INC
  #endinput
#endif
#define _ZF_ZOM_ALPHA_PERK_INC

#include "perks/base_perk.inc"
#include "perks/perk_structs.inc"

/**
 * Alpha Zombie Perk.
 * Summons zombie minions.
 */
methodmap AlphaPerk < base_perk
{
    /**
     * Constructor.
     */
    public AlphaPerk(int owner)
    {
        return view_as<AlphaPerk>(base_perk(owner));
    }

    /**
     * Called when the player spawns.
     */
    public void onSpawn()
    {
        // Create the aura effect for this perk
        createAura(this.getOwner(), ZFPART_AURAINBLU, AttachBase);
    }

    /**
     * Called when the perk is removed.
     */
    public void onRemove()
    {
        removeAura(this.getOwner());
    }

    /**
     * Called when the player calls for medic.
     */
    public Action onCallForMedic()
    {
        int owner = this.getOwner();
        int timer;
        this.GetValue("perk_timer", timer);

        if (timer == 0)
        {
            if (!isGrounded(owner))
            {
                PrintHintText(owner, "你必须站在地上才能召唤你的随从们.");
                ClientCommand(owner, "voicemenu 2 5");
            }
            else
            {
                int summonCount = this.doAlphaSummon();
                if (summonCount > 0)
                {
                    this.SetValue("perk_timer", ZF_ALPHA_TIMER_MINION * summonCount);
                    ClientCommand(owner, "voicemenu 2 1");
                }
                else
                {
                    PrintHintText(owner, "你没有随从可以召唤!");
                    ClientCommand(owner, "voicemenu 2 5");
                }
            }
        }
        else
        {
            PrintHintText(owner, "召唤技能没准备好.");
            ClientCommand(owner, "voicemenu 2 5");
        }
        return Plugin_Handled;
    }

    /**
     * Periodic update called every second.
     */
    public void onPeriodicUpdate()
    {
        // First, call the base class update for temp stats
        base_perk.onPeriodicUpdate();

        int owner = this.getOwner();
        int timer;
        this.GetValue("perk_timer", timer);

        if (timer > 0)
        {
            timer--;
            this.SetValue("perk_timer", timer);
            if (timer == 0)
            {
                PrintHintText(owner, "阿尔法准备就绪!");
            }
        }

        // Handle bonuses from nearby zombies
        int attBonus = 0;
        int regBonus = 0;
        
        // This part requires access to the global player perk list to check other zombies.
        // We will implement this logic in the main plugin file for now.
        // Later, we can pass the necessary context to the perk object.

        char key[64];
        Format(key, sizeof(key), "stat_%d_cond", ZFStatAtt);
        this.SetValue(key, attBonus);
        
        addHealth(owner, regBonus);

        // Handle HUD string
        char hudStr[32];
        Format(hudStr, sizeof(hudStr), (timer == 0 ? "(随从传送准备就绪)" : ""));
        this.SetString("hud_str", hudStr);
    }

    public int doAlphaSummon()
    {
        // This logic also needs access to other players.
        // It will be implemented in the main plugin file and called from here.
        return 0; 
    }
}