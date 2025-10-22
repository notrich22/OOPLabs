#pragma once
#include "MovableEntity.h"
#include <iostream>
#include "Direction.h"

class Board;                 // <-- форвард-декларация, чтобы видеть тип

class Player : public MovableEntity {
private:
    unsigned int experience = 0;
	unsigned int attackRange = 2;
public:
    Player(int health = 100, int melee = 20, int ranged = 10)
        : MovableEntity(health, melee, ranged) {
    }

    void takeTurn();

    void takeDamage(int dmg) override;
    void addExperience(unsigned int exp);
    char symbol() const noexcept override { return 'P'; }

    unsigned int getExperience() const { return experience; }
	unsigned int getAttackRange() const { return attackRange; }
    void switchMode();
    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Player>(*this);
    }

};
