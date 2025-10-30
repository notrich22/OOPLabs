#include "TowerController.h"
#include "CastContext.h"
#include "EnhancementState.h"
#include <iostream>

TowerController::TowerController(Board& board)
    : board_(board) {
}

void TowerController::addTower(const std::shared_ptr<EnemyTower>& tower, int x, int y) {
    board_.placeEntity(tower, x, y);
    towers_.push_back(tower);
}

void TowerController::update() {
    for (auto& tower : towers_) {
        if (!tower || !tower->isAlive())
            continue;

        tower->tickCooldown();
        if (!tower->isReady())
            continue;

        auto [tx, ty] = tower->getPosition();
        bool attacked = false;

        for (Direction dir : cardinalDirs) {
            auto [dx, dy] = delta(dir);

            for (int step = 1; step <= tower->getAttackRange(); ++step) {
                int x = tx + dx * step;
                int y = ty + dy * step;

                if (!board_.isInside(x, y)) break;

                const auto& cell = board_.getCell(x, y);
                if (cell.getType() == CellType::Wall)
                    break;

                if (cell.isOccupied()) {
                    auto target = std::dynamic_pointer_cast<ICombatEntity>(cell.getEntity());
                    if (target && target->faction() == Faction::Player) {
                        std::cout << "Tower at (" << tx << "," << ty << ") attacks player!\n";

                        EnhancementState none;
                        CastContext ctx{
                            *tower, board_, target,
                            target->getPosition(), none
                        };

                        tower->getSpell()->cast(ctx);
                        tower->resetCooldown();
                        attacked = true;
                    }
                    break;
                }
            }
            if (attacked) break;
        }
    }
}
