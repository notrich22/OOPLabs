#include "Player.h"
#include <iostream>

void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

void Player::takeTurn() {
    std::cout << "Player turn.\n";
}
