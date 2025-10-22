#pragma once
#include "MovableEntity.h"
#include <iostream>

class Enemy : public MovableEntity {
public:
    Enemy(int health, int meleeAttackPower, int rangedAttackPower = 0)
        : MovableEntity(health, meleeAttackPower, rangedAttackPower) {
    }

    void takeTurn() override;

    void takeDamage(int dmg) override;
    char symbol() const noexcept override { return 'E'; }

    std::shared_ptr<Entity> clone() const override { return std::make_shared<Enemy>(*this); }
};
