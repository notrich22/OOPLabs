#include "TrapEntity.h"

void TrapEntity::trigger(ICombatEntity& victim, Board& board) {
    std::cout << "Trap triggered at ("
        << getPosition().first << ", "
        << getPosition().second << ")! Dealing "
        << damage_ << " damage!\n";

    victim.takeDamage(damage_);
    auto [x, y] = getPosition();
    board.getCell(x, y).clearEntity();
}