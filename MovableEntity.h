#pragma once
#include "Entity.h"

// Любая подвижная сущность
class MovableEntity : public Entity {
protected:
    bool skipNextMove_ = false;  // пропуск следующего хода

public:
    MovableEntity() = default;
    virtual ~MovableEntity() override = default;

    // Управление пропуском хода
    bool shouldSkipMove() const noexcept { return skipNextMove_; }
    void setSkipNextMove(bool v) noexcept { skipNextMove_ = v; }
};
