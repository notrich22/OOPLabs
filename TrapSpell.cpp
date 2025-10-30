#include "TrapSpell.h"
#include "Board.h"
#include "TrapEntity.h"
#include <iostream>

bool TrapSpell::canCast(const CastContext& ctx) const {
    auto [x, y] = ctx.cell.value_or(std::make_pair(-1, -1));
    if (!ctx.board.isInside(x, y)) return false;

    const auto& cell = ctx.board.getCell(x, y);
    if (cell.isOccupied() || cell.getType() == CellType::Wall) {
        std::cout << "Cell already occupied!\n";
        return false;
    }
    return true;
}

bool TrapSpell::cast(CastContext& ctx) {
    if (!canCast(ctx)) return false;

    auto [x, y] = ctx.cell.value();

    int totalDamage = trapDamage() + ctx.enhancement.trapBonus();

    auto trap = std::make_shared<TrapEntity>(x, y, totalDamage);
    ctx.board.placeEntity(trap, x, y);

    std::cout << "Trap placed at (" << x << ", " << y
        << ") with damage " << totalDamage << "!\n";

    ctx.enhancement.clear();
    return true;
}
