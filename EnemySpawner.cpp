#include "EnemySpawner.h"
#include <iostream>

void EnemySpawner::takeDamage(int dmg) {
	ICombatEntity::takeDamage(dmg);
    std::cout << "EnemySpawner took " << dmg << " damage! HP left: " << getHealth() << "\n";
}


void EnemySpawner::takeTurn() {
    if (currentCounter > 0)
        --currentCounter;
}

std::shared_ptr<Entity> EnemySpawner::clone() const {
    return std::make_shared<EnemySpawner>(*this);
}
