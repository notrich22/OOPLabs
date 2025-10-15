#include "Enemy.h"
#include <iostream>
#include <cstdlib>

void Enemy::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

void Enemy::takeTurn() {
    std::cout << "Enemy takes its turn.\n";

    int dx = (std::rand() % 3) - 1; // -1, 0, 1
    int dy = (std::rand() % 3) - 1;
    std::cout << "Enemy tries to move (" << dx << ", " << dy << ")\n";
}
