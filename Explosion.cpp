#include "Explosion.h"
#include "Board.h"
#include "ICombatEntity.h"
#include <iostream>
#include <cmath>

bool Explosion::canCast(const CastContext& ctx) const {
    if (!ctx.cell.has_value())
        return false;

    auto [targetX, targetY] = ctx.cell.value();
    if (!ctx.board.isInside(targetX, targetY))
        return false;

    auto casterPos = ctx.caster.getPosition();
    int dist = std::abs(casterPos.first - targetX) + std::abs(casterPos.second - targetY);
    int maxRange = 5 + ctx.enhancement.areaBonus();

    if (dist > maxRange) {
        std::cout << "Target cell too far (" << dist << " > " << maxRange << ").\n";
        return false;
    }

    return true;
}

bool Explosion::cast(CastContext& ctx) {
    if (!canCast(ctx)) {
        std::cout << "Invalid explosion position.\n";
        return false;
    }

    auto [centerX, centerY] = ctx.cell.value();

    int area = areaSize() + ctx.enhancement.areaBonus();
    int damage = damagePerCell() + 2 * ctx.enhancement.stacks;

    std::cout << "Explosion at (" << centerX << ", " << centerY
        << "), area " << area << "×" << area
        << ", damage per cell: " << damage << "\n";

    int halfFloor = area / 2;
    int halfCeil = area - halfFloor - 1;

    for (int dy = -halfFloor; dy <= halfCeil; ++dy) {
        for (int dx = -halfFloor; dx <= halfCeil; ++dx) {
            int x = centerX + dx;
            int y = centerY + dy;

            if (!ctx.board.isInside(x, y))
                continue;

            auto entity = ctx.board.getCell(x, y).getEntity();
            if (!entity) continue;

            auto combat = std::dynamic_pointer_cast<ICombatEntity>(entity);
            if (combat && combat->faction() != ctx.caster.faction()) {
                combat->takeDamage(damage);
                std::cout << "Explosion hits " << entity->symbol()
                    << " at (" << x << "," << y << ") for "
                    << damage << " damage!\n";
            }
        }
    }

    ctx.enhancement.clear();
    return true;
}
