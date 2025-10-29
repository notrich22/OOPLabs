#include "CombatController.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "Config.h"
#include <iostream>
#include <algorithm>

std::shared_ptr<ICombatEntity> CombatController::findTargetInDirection(
    const std::shared_ptr<ICombatEntity>& source,
    Direction dir,
    int range) const
{
    auto [x, y] = source->getPosition();
    auto [dx, dy] = delta(dir);

    for (int step = 1; step <= range; ++step) {
        int nx = x + dx * step;
        int ny = y + dy * step;

        if (!board.isInside(nx, ny)) break;

        const Cell& cell = board.getCell(nx, ny);
        if (cell.getType() == CellType::Wall)
            break;
        if (cell.isOccupied())
            return std::dynamic_pointer_cast<ICombatEntity>(cell.getEntity());
    }

    return nullptr;
}

bool CombatController::handleCombat(
    const std::shared_ptr<ICombatEntity>& attacker,
    const std::shared_ptr<ICombatEntity>& defender)
{
    if (!attacker || !defender) return false;
    if (!attacker->isAlive() || !defender->isAlive()) return false;

    // Проверяем правила враждебности
    //if (!rules.canDealDamage(attacker->faction(), defender->faction()))
        //return false;

    // Совершаем атаку
    attacker->attack(*defender);

    if (!defender->isAlive()) {
        auto [x, y] = defender->getPosition();
        board.getCell(x, y).clearEntity();
        std::cout << "Target destroyed!\n";

        // Награда игроку за убийство
        if (attacker->faction() == Faction::Player) {
            if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
                player->addExperience(Config::DAMAGE_EXPERIENCE_REWARD);
            }
        }
    }

    return true;
}