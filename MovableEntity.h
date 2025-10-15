#pragma once
#include "Entity.h"

class MovableEntity : public Entity {
protected:
    bool skipNextMove_ = false;

public:
    MovableEntity(int health, int attackPower)
        : Entity(health, attackPower) {
    }

    virtual ~MovableEntity() = default;

    bool shouldSkipMove() const { return skipNextMove_; }
    void setSkipNextMove(bool v) { skipNextMove_ = v; }
};
