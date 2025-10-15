#pragma once
#include "StaticEntity.h"
#include "Enemy.h"
#include <memory>

class EnemySpawner : public StaticEntity {
private:
    int spawnCooldown;
    int currentCounter;

public:
    EnemySpawner(int health, int attackPower, int cooldown)
        : StaticEntity(health, attackPower),
        spawnCooldown(cooldown),
        currentCounter(cooldown) {
    }

    void takeTurn() override;
    void takeDamage(int dmg) override;

    bool readyToSpawn() const { return currentCounter <= 0; }
    void resetCounter() { currentCounter = spawnCooldown; }
};
