#pragma once
#include "Entity.h"
#include <memory>

// Класс MovableEntity описывает сущность, способную перемещаться по полю
class MovableEntity : public Entity {
protected:
    bool skipNextMove_ = false;  // пропуск следующего хода

public:
    // Конструктор инициализирует параметры здоровья и урона
    MovableEntity(int health, int meleeAttackPower, int rangedAttackPower)
        : Entity(health, meleeAttackPower, rangedAttackPower) {
    }

    MovableEntity(const MovableEntity&) = default;
    MovableEntity(MovableEntity&&) noexcept = default;
    MovableEntity& operator=(const MovableEntity&) = default;
    MovableEntity& operator=(MovableEntity&&) noexcept = default;
    virtual ~MovableEntity() = default;

    // Создание копии объекта
    virtual std::shared_ptr<Entity> clone() const = 0;

    // Управление состоянием пропуска хода
    bool shouldSkipMove() const { return skipNextMove_; }
    void setSkipNextMove(bool v) { skipNextMove_ = v; }
};
