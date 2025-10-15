#pragma once
#include "MovableEntity.h"

enum class AttackMode { Melee, Ranged };

class Player : public MovableEntity {
private:
    unsigned int experience;
    AttackMode mode;
public:
    Player(int health, int attackPower)
        : MovableEntity(health, attackPower),
        experience(0),
        mode(AttackMode::Melee) {
    }

    void takeTurn() override;
    void takeDamage(int dmg) override;

    void addExperience(unsigned int exp) { experience += exp; }
    unsigned int getExperience() const { return experience; }

    void switchMode() {
        mode = (mode == AttackMode::Melee) ? AttackMode::Ranged : AttackMode::Melee;
    }
    AttackMode getAttackMode() const { return mode; }
    int getAttackRange() const {
        return (mode == AttackMode::Melee) ? 1 : 3;
    }


};
