# 幸存者技能代码与文档吻合度审阅报告

本文档详细记录了 `src/sourcepawn/perks/survival/` 目录下的技能代码与 `docs/all_perks.md` 设计文档之间的差异。

## 总结

审阅发现，多个技能的实现与文档存在出入，部分属于严重不匹配，需要重写；另一部分为功能缺失或数值错误，需要修正。

---

## 严重不匹配的技能

以下技能的代码实现与文档描述存在根本性差异，建议根据文档逻辑进行重写。

### 1. Heroic (英雄)

*   **问题**: 代码实现与文档描述完全不同。代码是一个简单的“最后幸存者获得固定加成”的技能，而文档描述的是一个复杂的、通过击杀/助攻累积暴击时间的系统。
*   **文件**: [`src/sourcepawn/perks/survival/HeroicPerk.inc`](src/sourcepawn/perks/survival/HeroicPerk.inc:1)
*   **建议**: 根据文档完全重写该技能的逻辑，包括暴击时间累积、激活机制和永久属性加成。

### 2. Stash (仓鼠)

*   **问题**: 代码实现与文档描述完全不同。代码实现了一个简单的补给站，而文档描述的是一个带有预热、永久/临时攻击加成的复杂系统。参数定义也完全不一致。
*   **文件**: [`src/sourcepawn/perks/survival/StashPerk.inc`](src/sourcepawn/perks/survival/StashPerk.inc:1)
*   **建议**: 根据文档完全重写该技能的逻辑和参数。

### 3. Nonlethal (治安官)

*   **问题**:
    1.  子弹攻击力惩罚值不匹配 (代码 `-50` vs 文档 `-90`)。
    2.  完全没有实现攻击力惩罚的逻辑 (`onTakeDamage` 事件)，只实现了击退效果。
*   **文件**: [`src/sourcepawn/perks/survival/NonlethalPerk.inc`](src/sourcepawn/perks/survival/NonlethalPerk.inc:1)
*   **建议**:
    1.  将 [`ZF_NONLETHAL_ATTACK_BULLET`](src/sourcepawn/perks/survival/NonlethalPerk.inc:20) 的值从 `-50` 修改为 `-90`。
    2.  添加 `VTABLE_ON_TAKE_DAMAGE` 的实现，在伤害计算前应用攻击力惩罚。

### 4. Selfless (利他主义者)

*   **问题**: 死亡爆炸的伤害和范围远小于文档定义。
    *   伤害: 代码 `1000` vs 文档 `10000`。
    *   范围: 代码 `500` vs 文档 `5000`。
*   **文件**: [`src/sourcepawn/perks/survival/SelflessPerk.inc`](src/sourcepawn/perks/survival/SelflessPerk.inc:1)
*   **建议**: 将 [`ZF_SELFLESS_DAMAGE`](src/sourcepawn/perks/survival/SelflessPerk.inc:13) 和 [`ZF_SELFLESS_RADIUS`](src/sourcepawn/perks/survival/SelflessPerk.inc:14) 的值修改为与文档一致。

---

## 功能缺失或微小差异的技能

以下技能的实现基本正确，但存在功能缺失、数值错误或小的逻辑差异。

*   **Cowardly (懦夫)**:
    *   **问题**: 缺少了在技能准备就绪时显示 "(随时准备跑路)" 的HUD文本。
    *   **文件**: [`src/sourcepawn/perks/survival/CowardlyPerk.inc`](src/sourcepawn/perks/survival/CowardlyPerk.inc:1)
    *   **建议**: 在 [`updateCondStats`](src/sourcepawn/perks/survival/CowardlyPerk.inc:100) 中添加相应的HUD文本逻辑。

*   **Holy (牧师)**:
    *   **问题**: 缺少了文档中提到的光环特效 (`ZFPART_AURAGLOWBEAMS`)。
    *   **文件**: [`src/sourcepawn/perks/survival/HolyPerk.inc`](src/sourcepawn/perks/survival/HolyPerk.inc:1)
    *   **建议**: 添加 `VTABLE_UPDATE_CLIENT_PERM_EFFECTS` 的实现来创建光环。

*   **Leader (领袖)**:
    *   **问题**: 缺少了文档中提到的被动光环特效 (`ZFPART_AURAINRED`)。
    *   **文件**: [`src/sourcepawn/perks/survival/LeaderPerk.inc`](src/sourcepawn/perks/survival/LeaderPerk.inc:1)
    *   **建议**: 添加 `VTABLE_UPDATE_CLIENT_PERM_EFFECTS` 的实现来创建光环。

*   **Ninja (忍者)**:
    *   **问题**: 代码中包含大量用于调试的 `PrintToServer` 语句。
    *   **文件**: [`src/sourcepawn/perks/survival/NinjaPerk.inc`](src/sourcepawn/perks/survival/NinjaPerk.inc:1)
    *   **建议**: 移除这些调试信息。

*   **Supplier (供应商)**:
    *   **问题**: 在 [`updateClientPermStats`](src/sourcepawn/perks/survival/SupplierPerk.inc:62) 中应用了一个文档未提及的基于 `ZF_SUPPLIER_AMMOPCT_RESLIMIT` 的永久攻击加成。
    *   **文件**: [`src/sourcepawn/perks/survival/SupplierPerk.inc`](src/sourcepawn/perks/survival/SupplierPerk.inc:1)

*   **Trapper (陷阱大师)**:
    *   **问题**:
        1.  缺少对磁化僵尸 (Magnetic perk) 的特殊处理（地雷失效）。
        2.  缺少地雷待机时的心跳音效。
    *   **文件**: [`src/sourcepawn/perks/survival/TrapperPerk.inc`](src/sourcepawn/perks/survival/TrapperPerk.inc:1)
    *   **建议**: 在 [`updateCondStats`](src/sourcepawn/perks/survival/TrapperPerk.inc:57) 中添加对磁化僵尸的检查和相应的音效逻辑。

*   **Zenlike (禅师)**:
    *   **问题**: 缺少了治疗中毒 (`ZF_ZENLIKE_HEAL`) 的逻辑。
    *   **文件**: [`src/sourcepawn/perks/survival/ZenlikePerk.inc`](src/sourcepawn/perks/survival/ZenlikePerk.inc:1)
    *   **建议**: 在 [`updateCondStats`](src/sourcepawn/perks/survival/ZenlikePerk.inc:53) 中添加治疗中毒效果的逻辑。

---

## 文档缺失的技能

*   **Scavenger (拾荒者)**:
    *   **问题**: 代码文件 [`src/sourcepawn/perks/survival/ScavengerPerk.inc`](src/sourcepawn/perks/survival/ScavengerPerk.inc:1) 存在，但 `docs/all_perks.md` 中没有对应的技能描述。
    *   **建议**: 为 Scavenger 技能补充设计文档。

---

## 需要确认的外部依赖

*   **Carpenter (木工)**: 依赖 `perk_OnFenceTakeDamage` 函数。
*   **Charitable (慈善家)**: 依赖 `perk_OnCharitableGiftTouched` 函数。

这些函数未在各自的技能文件中定义，需要确认它们是否存在于全局或其他包含文件中，并被正确挂接。
