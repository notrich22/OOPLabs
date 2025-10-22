#pragma once
#include <utility>
#include <memory>
#include <algorithm>

// Базовый класс сущности на поле
class Entity {
protected:
    int health;                    // здоровье
    std::pair<int, int> position;  // координаты сущности на поле
public:
    Entity(int health)
        : health(std::max(0, health)) {}

    virtual ~Entity() = default;

    // Поведение, общее для всех
    virtual void takeTurn() = 0;         // ход сущности (игровая логика)
    virtual void takeDamage(int dmg) {   // получение урона
        health = std::max(0, health - dmg);
    }

    virtual std::shared_ptr<Entity> clone() const = 0;

    // Доступ к данным
    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }
    virtual char symbol() const noexcept = 0;

    std::pair<int,int> getPosition() const noexcept { return position; }
    int getX() const noexcept { return position.first; }
    int getY() const noexcept { return position.second; }

    void setPosition(int x, int y) noexcept { position = {x, y}; }
};
