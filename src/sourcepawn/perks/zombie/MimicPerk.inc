#if defined __MimicPerk_included
#endinput
#endif
#define __MimicPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include <tf2_stocks>

// --- Defines ---
#define MIMIC_ATTACK -75            // 攻击力大幅降低
#define MIMIC_COOLDOWN 40.0         // 冷却时间
#define MIMIC_DURATION 20.0         // 伪装持续时间
#define MIMIC_BONUS_DAMAGE 150.0    // 伪装后的额外伤害

// --- Data Slots ---
#define MIMIC_DATA_STATE (PERK_DATA_START_INDEX + 0) // 0: Ready, 1: Disguised, 2: Cooldown
#define MIMIC_DATA_TIMER (PERK_DATA_START_INDEX + 1)

methodmap MimicPerk < ZombieBasePerk {
    public MimicPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        MimicPerk sm = view_as<MimicPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, MimicPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, MimicPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, MimicPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, MimicPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, MimicPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, MimicPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEAL_DAMAGE_POST, MimicPerkFonDealDamagePost);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, MimicPerkFonDeath);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_REMOVE, MimicPerkFonRemove);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_PLAYER_SPAWN, MimicPerkFonPlayerSpawn);

        sm.state = 0;
        sm.timer = 0.0;

        return sm;
    }

    property int state {
        public get() {
            this.Position = view_as<DataPackPos>(MIMIC_DATA_STATE);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(MIMIC_DATA_STATE);
            this.WriteCell(value);
        }
    }

    property float timer {
        public get() {
            this.Position = view_as<DataPackPos>(MIMIC_DATA_TIMER);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(MIMIC_DATA_TIMER);
            this.WriteFloat(value);
        }
    }
}

stock BasePerk MimicPerkFnew(int client) {
    return new MimicPerk(client);
}

stock void removeMimicDisguise(int client, MimicPerk inst) {
    if (inst.state != 1) return;

    inst.state = 2; // Go to cooldown
    inst.timer = GetGameTime() + MIMIC_COOLDOWN;
    
    // Revert visual changes
    TF2_SetPlayerClass(client, TF2_GetPlayerClass(client), true, true); // Force model reload
    SetEntProp(client, Prop_Send, "m_bUseClassAnimations", 1);
    SetEntityRenderColor(client, 255, 255, 255, 255); // Reset color
    
    PrintHintText(client, "伪装已解除。");
}

FUNCTION(MimicPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Mimic");
}

FUNCTION(MimicPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MimicPerk_shortdesc");
}

FUNCTION(MimicPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "MimicPerk_desc");
}

FUNCTION(MimicPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, MIMIC_ATTACK);
}

FUNCTION(MimicPerk, onCallForMedic)) {
    if (_inst.state != 0) return;

    int client = _inst.client;
    _inst.state = 1; // Disguised
    _inst.timer = GetGameTime() + MIMIC_DURATION;

    // Apply disguise
    TF2_SetPlayerClass(client, view_as<TFClassType>(GetRandomInt(1, 9)), false, true);
    SetEntProp(client, Prop_Send, "m_bUseClassAnimations", 1);
    SetEntityRenderColor(client, 180, 180, 180, 255); // A slightly off color to not be perfect
    
    PrintHintText(client, "你已伪装！持续 %d 秒或直到你攻击。", RoundFloat(MIMIC_DURATION));
}

FUNCTION(MimicPerk, onDealDamagePost), int victim, int attacker, int inflictor, float damage, int damagetype) {
    if (_inst.client == attacker && _inst.state == 1) {
        if (validLivingSur(victim)) {
            SDKHooks_TakeDamage(victim, attacker, inflictor, MIMIC_BONUS_DAMAGE, DMG_CRIT);
            PrintHintText(attacker, "致命一击！");
        }
        removeMimicDisguise(attacker, _inst);
    }
}

FUNCTION(MimicPerk, onPlayerSpawn)) {
    // Ensure disguise is removed on spawn
    if (_inst.state == 1) {
        removeMimicDisguise(_inst.client, _inst);
    }
}

FUNCTION(MimicPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    if (_inst.state == 1) {
        removeMimicDisguise(_inst.client, _inst);
    }
    _inst.state = 0; // Reset on death
    _inst.timer = 0.0;
}

FUNCTION(MimicPerk, onRemove)) {
    if (_inst.state == 1) {
        removeMimicDisguise(_inst.client, _inst);
    }
}

FUNCTION(MimicPerk, updateCondStats), char[] buffer, int maxlen) {
    float time_left = _inst.timer - GetGameTime();

    switch (_inst.state) {
        case 0: {
            strcopy(buffer, maxlen, "伪装已就绪");
        }
        case 1: {
            if (time_left > 0) {
                Format(buffer, maxlen, "伪装中: %ds", RoundFloat(time_left));
            } else {
                // Time ran out
                removeMimicDisguise(_inst.client, _inst);
                Format(buffer, maxlen, "伪装时间结束");
            }
        }
        case 2: {
            if (time_left > 0) {
                Format(buffer, maxlen, "冷却中: %ds", RoundFloat(time_left));
            } else {
                _inst.state = 0;
                strcopy(buffer, maxlen, "伪装已就绪");
            }
        }
    }
}