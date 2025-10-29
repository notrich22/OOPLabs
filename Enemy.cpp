#include "Enemy.h"
#include <iostream>
#include <cstdlib>

void Enemy::takeDamage(int dmg) {
    ICombatEntity::takeDamage(dmg);
    std::cout << "Enemy takes " << dmg << " damage! HP: " << getHealth() << "\n";
}

void Enemy::takeTurn() {}
