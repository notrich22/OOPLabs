#include "Enemy.h"
#include <iostream>
#include <cstdlib>

void Enemy::takeDamage(int dmg) {
    Entity::takeDamage(dmg);
    std::cout << "Enemy takes " << dmg << " damage! HP: " << getHealth() << "\n";
}

void Enemy::takeTurn() {
    std::cout << "Enemy takes its turn.\n";

    int dx = (std::rand() % 3) - 1; // -1, 0, 1
    int dy = (std::rand() % 3) - 1;
    std::cout << "Enemy tries to move (" << dx << ", " << dy << ")\n";
}
