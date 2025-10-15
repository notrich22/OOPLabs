#pragma once
#include "MovableEntity.h"
#include <memory>

// Класс Enemy - сущность врага, способная перемещаться и атаковать
class Enemy : public MovableEntity {
public:
    // Конструктор инициализирует параметры здоровья и урона
    Enemy(int health, int meleeAttackPower, int rangedAttackPower = 0)
        : MovableEntity(health, meleeAttackPower, rangedAttackPower) {
    }

    Enemy(const Enemy&) noexcept = default;

    // Логика поведения врага
    void takeTurn() override;            // действие врага на своём ходу
    void takeDamage(int dmg) override;   // получение урона

    // Создание копии объекта
    std::shared_ptr<Entity> clone() const override;
};
