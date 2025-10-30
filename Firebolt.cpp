#include "Firebolt.h"
#include "Relationships.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <cmath>

bool Firebolt::canCast(const CastContext& ctx) const {
    // ��� ���� � ������ ���������
    auto target = ctx.target.lock();
    if (!target) return false;

    // ���� ������ ���� ���� � ���������
    if (!target->isAlive()) return false;

    if (!ctx.caster.canAttack(*target))
        return false;

    // �������� ������� ��������
    auto casterPos = ctx.caster.getPosition();
    auto targetPos = target->getPosition();

    int dist = std::abs(casterPos.first - targetPos.first)
        + std::abs(casterPos.second - targetPos.second);

    int maxRange = range() + ctx.enhancement.rangeBonus(); // ��������� ��������
    return dist <= maxRange;
}

bool Firebolt::cast(CastContext& ctx) {
    if (!canCast(ctx)) {
        //std::cout << "No valid target in range.\n";
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
    ctx.enhancement.clear();

    return true;
}
