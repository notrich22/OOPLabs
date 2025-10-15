#pragma once
#include "Entity.h"

class StaticEntity : public Entity {
public:
    StaticEntity(int health, int attackPower)
        : Entity(health, attackPower) {
    }

    virtual ~StaticEntity() = default;

    void takeTurn() override = 0;
};
