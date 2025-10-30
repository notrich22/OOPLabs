#include "MovementController.h"
#include <iostream>
#include "TrapSpell.h"
#include "TrapEntity.h"

MovementController::MovementController(Board& b, CombatController& c)
    : board(b), combat(c) {
}

bool MovementController::canMoveTo(int x, int y) const {
    if (!board.isInside(x, y))
        return false;

    const Cell& target = board.getCell(x, y);
    return target.getType() != CellType::Wall;
}
bool MovementController::move(MovableEntity& entity, int dx, int dy) {
    auto [x, y] = entity.getPosition();
    int newX = x + dx;
    int newY = y + dy;

    if (!canMoveTo(newX, newY))
        return false;

    Cell& current = board.getCell(x, y);
    Cell& target = board.getCell(newX, newY);

    // === ѕровер€ем ловушку ===
    if (target.hasEntity()) { // не isOccupied()!
        auto trap = std::dynamic_pointer_cast<TrapEntity>(target.getEntity());
        if (trap) {
            // существо, которое двигаетс€, Ч это и есть "жертва"
            auto victim = std::dynamic_pointer_cast<ICombatEntity>(current.getEntity());
            if (victim) {
                trap->trigger(*victim, board);
            }
            if(!victim->isAlive()) {
                current.clearEntity();
                return true; // существо погибло на месте
			}
        }
    }

    // === ѕроверка на боевое столкновение ===
    if (target.isOccupied()) {
        auto attacker = std::dynamic_pointer_cast<ICombatEntity>(current.getEntity());
        auto defender = std::dynamic_pointer_cast<ICombatEntity>(target.getEntity());

        if (!attacker || !defender)
            return false;

        combat.handleCombat(attacker, defender);
        return true;
    }

    // === ѕеремещаем сущность ===
    target.setEntity(current.getEntity());
    current.clearEntity();
    entity.setPosition(newX, newY);

    if (target.getType() == CellType::Slow)
        entity.setSkipNextMove(true);

    return true;
}