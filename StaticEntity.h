#pragma once
#include "Entity.h"

class StaticEntity : public Entity {
public:
    virtual ~StaticEntity() = default;
    virtual void takeTurn() override = 0;
};
