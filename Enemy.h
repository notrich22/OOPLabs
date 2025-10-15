#pragma once
#include "MovableEntity.h"

class Enemy : public MovableEntity {
public:
    Enemy(int health, int attackPower)
        : MovableEntity(health, attackPower) {
    }
    bool shouldSkipMove() const { return skipNextMove_; }
    void setSkipNextMove(bool v) { skipNextMove_ = v; }
    void takeTurn() override;
    void takeDamage(int dmg) override;
};
