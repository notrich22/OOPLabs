#include "SummonAlly.h"
#include "Board.h"
#include "Config.h"
#include <iostream>
#include "Ally.h"

bool SummonAlly::canCast(const CastContext& ctx) const {
    auto [px, py] = ctx.caster.getPosition();
    for (Direction dir : cardinalDirs) {
        auto [dx, dy] = delta(dir);
        int nx = px + dx, ny = py + dy;
        if (ctx.board.isInside(nx, ny) && !ctx.board.getCell(nx, ny).isOccupied())
            return true;
    }
    return false;
}

bool SummonAlly::cast(CastContext& ctx) {
    if (!canCast(ctx)) {
        std::cout << "No space to summon ally!\n";
        return false;
    }

    auto [px, py] = ctx.caster.getPosition();

    int count = 1 + ctx.enhancement.summonBonus();

    int summoned = 0;

    for (int i = 0; i < count; ++i) {
        bool placed = false;

        for (Direction dir : cardinalDirs) {
            auto [dx, dy] = delta(dir);
            int nx = px + dx, ny = py + dy;
            if (!ctx.board.isInside(nx, ny)) continue;

            Cell& cell = ctx.board.getCell(nx, ny);
            if (cell.isOccupied()) continue;

            auto ally = std::make_shared<Ally>(summonHealth(), summonDamage());
            ctx.board.placeEntity(ally, nx, ny);

            std::cout << "Ally summoned at (" << nx << ", " << ny << ")!\n";
            placed = true;
            summoned++;
            break;
        }

        if (!placed) {
            std::cout << "No space for more allies!\n";
            break;
        }
    }

    std::cout << "Total allies summoned: " << summoned << "\n";
    ctx.enhancement.clear();
    return summoned > 0;
}
