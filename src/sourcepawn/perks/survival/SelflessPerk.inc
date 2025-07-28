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
        PERK_REGISTER_VTABLE(sm, VTABLE_ON_TAKE_DAMAGE, SelflessPerkFonTakeDamage);

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
    strcopy(buffer, maxlen, "SelflessPerk_shortdesc");
}

FUNCTION(SelflessPerk, getDesc), char[] buffer, int maxlen) {
    strcopy(buffer, maxlen, "SelflessPerk_desc");
}

// 使用 onTakeDamage 来触发爆炸
FUNCTION_FLOAT(SelflessPerk, onTakeDamage), int victim, int attacker, int inflictor, float damage, int damagetype)
{
    int client = _inst.client;

    // 1. 确保是 Selfless 玩家自己受到伤害
    if (victim != client) {
        return damage;
    }

    if (isUbered(victim)){
        return 0.0;
    }

    // 2. 检查是否是来自僵尸的致命一击
    if (validZom(attacker)) {
        int currentHealth = GetClientHealth(client);
        if (damage >= currentHealth) {
            // 这是致命一击！在伤害应用前触发爆炸。
            // 此时，`client` 仍然是活着的，`validClient(client)` 会返回 true。
            
            // a. 创建特效
            fxExplosionBig(client);

            // b. 调用范围伤害函数
            // 此时 client 仍然是有效的 attacker
            applyDamageRadialAtClient(client, ZF_SELFLESS_DAMAGE, ZF_SELFLESS_RADIUS, false);
        }
    }

    // 3. 返回原始伤害，让玩家被正常杀死
    return damage;
}
