#pragma once
#include "Entity.h"

// Класс StaticEntity описывает статическую сущность, имеющую здоровье и урон, но не способную перемещаться
class StaticEntity : public Entity {
public:
    // Конструктор инициализирует базовые параметры сущности
    StaticEntity(int health, int meleeAttackPower, int rangedAttackPower)
        : Entity(health, meleeAttackPower, rangedAttackPower) {
    }

    virtual ~StaticEntity() = default;

    // Абстрактный метод: статическая сущность должна реализовать собственное поведение
    void takeTurn() override = 0;
};
