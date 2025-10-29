#include "DirectDamageSpell.h"
#include "Relationships.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <cmath>

bool DirectDamageSpell::canCast(const CastContext& ctx) const {
    // Нет цели — нельзя кастовать
    auto target = ctx.target.lock();
    if (!target) return false;

    // Цель должна быть жива и враждебна
    if (!target->isAlive()) return false;
    if (!ctx.caster.canAttack(*target))
        return false;

    // Проверка радиуса действия
    auto casterPos = ctx.caster.getPosition();
    auto targetPos = target->getPosition();

    int dist = std::abs(casterPos.first - targetPos.first)
        + std::abs(casterPos.second - targetPos.second);

    int maxRange = range() + ctx.enhancement.rangeBonus(); // учитываем усиления
    return dist <= maxRange;
}

bool DirectDamageSpell::cast(CastContext& ctx) {
    if (!canCast(ctx)) {
        std::cout << "No valid target in range.\n";
        return false;
    }

    auto target = ctx.target.lock();
    if (!target) return false;

    int totalDamage = baseDamage();

    target->takeDamage(totalDamage);

    std::cout << "Dealt " << totalDamage
        << " damage to target at "
        << target->getPosition().first << ", "
        << target->getPosition().second << "!\n";

    // Сжечь накопленные усиления (по ТЗ п.8е)
    ctx.enhancement.clear();

    return true;
}
