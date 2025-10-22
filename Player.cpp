#include "Player.h"
#include <iostream>
#include "Game.h"
#include "Direction.h"


void Player::takeDamage(int dmg){
    Entity::takeDamage(dmg);
    std::cout << "Player took " << dmg << " damage! HP left: " << getHealth() << "\n";
}

void Player::addExperience(unsigned int exp) {
    experience += exp;
    std::cout << "Player gained " << exp << " XP! Total: " << experience << "\n";
}

void Player::takeTurn() {
    std::cout << "Player turn.\n";
}

void Player::switchMode() {
    if (attackMode == AttackMode::Melee) {
        attackMode = AttackMode::Ranged;
    }
    else {
        attackMode = AttackMode::Melee;
    }
}

