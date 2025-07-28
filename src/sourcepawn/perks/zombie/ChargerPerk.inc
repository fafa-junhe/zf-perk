#if defined __ChargerPerk_included
#endinput
#endif
#define __ChargerPerk_included

#include "ZombieBasePerk.inc"
#include "../../perk_vtable.inc"
#include "../../perk_macros.inc"
#include "../../zf_perk.inc"
#include "../../zf_util_base.inc"
#include "../../zf_util_fx.inc"
#include <sdktools_trace>
#include <sdkhooks>

// --- Defines ---
#define CHARGER_DEFEND 50       // 防御力偏高
#define CHARGER_ATTACK -10      // 伤害略低
#define CHARGER_SPEED -20       // 速度慢
#define CHARGER_COOLDOWN 12.0   // 冷却时间
#define CHARGER_CHARGE_DURATION 4.0 // 冲锋持续时间
#define CHARGER_CHARGE_SPEED 1000.0 // 冲锋速度
#define CHARGER_IMPACT_DAMAGE 150.0 // 撞墙伤害
#define CHARGER_STUN_DURATION 2.0   // 眩晕时间

// --- Data Slots ---
#define CHARGER_DATA_STATE          (PERK_DATA_START_INDEX + 0) // 0: ready, 1: charging, 2: grabbing, 3: cooldown
#define CHARGER_DATA_TIMER          (PERK_DATA_START_INDEX + 1) // Cooldown / duration timer
#define CHARGER_DATA_GRABBED_VICTIM (PERK_DATA_START_INDEX + 2)
#define CHARGER_DATA_CHARGE_VEC     (PERK_DATA_START_INDEX + 3) // Size: 3

methodmap ChargerPerk < ZombieBasePerk {
    public ChargerPerk(int client) {
        ZombieBasePerk sm_base = new ZombieBasePerk(client);
        ChargerPerk sm = view_as<ChargerPerk>(sm_base);

        PERK_REGISTER_VTABLE(sm, VTABLE_GET_NAME, ChargerPerkFgetName);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_SHORT_DESC, ChargerPerkFgetShortdesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_GET_LONG_DESC, ChargerPerkFgetDesc);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_CLIENT_PERM_STATS, ChargerPerkFupdateClientPermStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_UPDATE_COND_STATS, ChargerPerkFupdateCondStats);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_CALL_FOR_MEDIC, ChargerPerkFonCallForMedic);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_GAME_FRAME, ChargerPerkFonGameFrame);
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_DEATH, ChargerPerkFonDeath);

        sm.state = 0;
        sm.timer = 0.0;
        sm.grabbed_victim = 0;
        float zero_vec[3] = {0.0, 0.0, 0.0};
        sm.setChargeVec(zero_vec);

        return sm;
    }

    property int state {
        public get() { 
            this.Position = view_as<DataPackPos>(CHARGER_DATA_STATE);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_STATE);
            this.WriteCell(value);
        }
    }

    property float timer {
        public get() {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_TIMER);
            return this.ReadFloat();
        }
        public set(float value) {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_TIMER);
            this.WriteFloat(value);
        }
    }

    property int grabbed_victim {
        public get() {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_GRABBED_VICTIM);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = view_as<DataPackPos>(CHARGER_DATA_GRABBED_VICTIM);
            this.WriteCell(value);
        }
    }

    public void getChargeVec(float buffer[3]) {
        this.Position = view_as<DataPackPos>(CHARGER_DATA_CHARGE_VEC);
        this.ReadCellArray(buffer, 3);
    }
    public void setChargeVec(const float value[3]) {
        this.Position = view_as<DataPackPos>(CHARGER_DATA_CHARGE_VEC);
        this.WriteCellArray(value, 3);
    }
}

stock BasePerk ChargerPerkFnew(int client) {
    return new ChargerPerk(client);
}

FUNCTION(ChargerPerk, getName), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "Charger");
}

FUNCTION(ChargerPerk, getShortdesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ChargerPerk_shortdesc");
}

FUNCTION(ChargerPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "ChargerPerk_desc");
}

FUNCTION(ChargerPerk, updateClientPermStats)) {
    addStat(_inst.client, ZFStatDef, ZFStatTypePerm, CHARGER_DEFEND);
    addStat(_inst.client, ZFStatAtt, ZFStatTypePerm, CHARGER_ATTACK);
    addStat(_inst.client, ZFStatSpeed, ZFStatTypePerm, CHARGER_SPEED);
}

FUNCTION(ChargerPerk, onCallForMedic)) {
    if (_inst.state != 0) return; // Not ready

    int client = _inst.client;
    _inst.state = 1; // charging
    _inst.timer = GetGameTime() + CHARGER_CHARGE_DURATION;

    float angles[3];
    GetClientEyeAngles(client, angles);
    float charge_vec[3];
    GetAngleVectors(angles, charge_vec, NULL_VECTOR, NULL_VECTOR);
    _inst.setChargeVec(charge_vec);

    // Apply initial velocity
    float velocity[3];
    velocity = charge_vec;
    ScaleVector(velocity, CHARGER_CHARGE_SPEED);
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, velocity);

    PrintHintText(client, "冲锋！");
    fxCreateSoundToAll("ambient/explosions/explode_8.wav", client); // Placeholder sound
}

FUNCTION(ChargerPerk, onGameFrame)) {
    if (_inst.state == 0) return;

    int client = _inst.client;

    // Cooldown state
    if (_inst.state == 3) {
        if (GetGameTime() >= _inst.timer) {
            _inst.state = 0;
            PrintHintText(client, "冲锋已准备就绪！");
        }
        return;
    }

    // Charge has expired
    if (GetGameTime() >= _inst.timer) {
        PrintHintText(client, "冲锋结束。");
        _inst.state = 3; // Cooldown
        _inst.timer = GetGameTime() + CHARGER_COOLDOWN;
        if (_inst.grabbed_victim > 0) {
            // If charge ends mid-air, just release the victim
            _inst.grabbed_victim = 0;
        }
        return;
    }

    // --- Charging Logic ---
    float charge_vec[3];
    _inst.getChargeVec(charge_vec);
    float velocity[3];
    velocity = charge_vec;
    ScaleVector(velocity, CHARGER_CHARGE_SPEED);
    TeleportEntity(client, NULL_VECTOR, NULL_VECTOR, velocity);

    float start_pos[3], end_pos[3];
    GetClientAbsOrigin(client, start_pos);
    
    float check_vec[3];
    check_vec = charge_vec;
    ScaleVector(check_vec, 20.0); // Check 20 units ahead
    AddVectors(start_pos, check_vec, end_pos);

    // Check for wall collision
    TR_TraceHull(start_pos, end_pos, {-16.0, -16.0, 0.0}, {16.0, 16.0, 72.0}, MASK_SOLID);
    if (TR_DidHit()) {
        int victim = _inst.grabbed_victim;
        if (victim > 0 && IsClientInGame(victim) && IsPlayerAlive(victim)) {
            PrintHintText(client, "你把 %N 撞到了墙上！", victim);
            PrintHintText(victim, "你被冲撞者撞到了墙上！");
            SDKHooks_TakeDamage(client, client, client, CHARGER_IMPACT_DAMAGE, DMG_CRUSH);
            SDKHooks_TakeDamage(victim, client, client, CHARGER_IMPACT_DAMAGE, DMG_CRUSH);
            
            // Stun both
            addStatTempStack(client, ZFStatSpeed, -200, RoundToNearest(CHARGER_STUN_DURATION));
            addStatTempStack(victim, ZFStatSpeed, -200, RoundToNearest(CHARGER_STUN_DURATION));
            
            _inst.grabbed_victim = 0;
        } else {
            PrintHintText(client, "你撞到了墙上！");
            SDKHooks_TakeDamage(client, client, client, CHARGER_IMPACT_DAMAGE / 2.0, DMG_CRUSH);
            addStatTempStack(client, ZFStatSpeed, -200, RoundToNearest(CHARGER_STUN_DURATION));
        }
        
        _inst.state = 3; // Cooldown
        _inst.timer = GetGameTime() + CHARGER_COOLDOWN;
        return;
    }

    // If not grabbing anyone, check for victims
    if (_inst.state == 1) {
        for (int i = 1; i <= MaxClients; i++) {
            if (validLivingSur(i)) {
                float sur_pos[3];
                GetClientAbsOrigin(i, sur_pos);
                if (GetVectorDistance(start_pos, sur_pos) < 64.0) {
                    _inst.grabbed_victim = i;
                    _inst.state = 2; // grabbing
                    PrintHintText(client, "你抓住了 %N！", i);
                    PrintHintText(i, "你被冲撞者抓住了！");
                    break;
                }
            }
        }
    }

    // If grabbing a victim, move them
    if (_inst.state == 2) {
        int victim = _inst.grabbed_victim;
        if (validLivingSur(victim)) {
            float victim_pos[3];
            victim_pos = start_pos;
            float offset[3];
            offset = charge_vec;
            ScaleVector(offset, 64.0);
            AddVectors(victim_pos, offset, victim_pos);
            TeleportEntity(victim, victim_pos, NULL_VECTOR, velocity);
        } else {
            // Victim died or disconnected
            _inst.grabbed_victim = 0;
            _inst.state = 1; // continue charging
        }
    }
}

FUNCTION(ChargerPerk, onDeath), int victim, int killer, int assist, int inflictor, int damagetype) {
    // Reset state on death
    _inst.state = 0;
    _inst.timer = 0.0;
    _inst.grabbed_victim = 0;
}

FUNCTION(ChargerPerk, updateCondStats), char[] buffer, int maxlen) {
    switch (_inst.state) {
        case 0: strcopy(buffer, maxlen, "冲锋已就绪");
        case 1: strcopy(buffer, maxlen, "冲锋中！");
        case 2: {
            int victim = _inst.grabbed_victim;
            if (validClient(victim)) {
                Format(buffer, maxlen, "正拖着 %N", victim);
            } else {
                strcopy(buffer, maxlen, "冲锋中！");
            }
        }
        case 3: {
            int time_left = RoundFloat(_inst.timer - GetGameTime());
            if (time_left > 0) {
                Format(buffer, maxlen, "冷却中: %ds", time_left);
            }
        }
    }
}