#include "EnemySpawner.h"
#include <iostream>

void EnemySpawner::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}


void EnemySpawner::takeTurn() {
    if (currentCounter > 0)
        --currentCounter;
}

std::shared_ptr<Entity> EnemySpawner::clone() const {
    return std::make_shared<EnemySpawner>(*this);
}
