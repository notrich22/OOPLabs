#pragma once
#include <utility>

class Entity {
protected:
    int health;
    int attackPower;

public:
    Entity(int health, int attackPower)
        : health(health > 0 ? health : 0),
        attackPower(attackPower > 0 ? attackPower : 0) {
    }

    Entity(const Entity&) = default;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) noexcept = default;
    virtual ~Entity() = default;

    virtual void takeTurn() = 0;
    virtual void takeDamage(int dmg) = 0;

    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }

    void setHealth(int h) { health = (h > 0 ? h : 0); }
    void setAttackPower(int power) { attackPower = (power > 0 ? power : 0); }

    bool isAlive() const { return health > 0; }
};
