#pragma once
#include "MovableEntity.h"
#include "ICombatEntity.h"
#include "Config.h"
#include "Board.h"
#include <iostream>

class Ally : public MovableEntity, public ICombatEntity {
public:
    Ally(int hp = 30, int melee = 10, int ranged = 0)
        : MovableEntity(), ICombatEntity() {
        health_ = hp;
        meleeAttackPower_ = melee;
        rangedAttackPower_ = ranged;
    }

    char symbol() const noexcept override { return 'A'; }
    Faction faction() const override { return Faction::Player; }

    std::pair<int, int> getPosition() const override { return Entity::getPosition(); }

    void takeDamage(int amount) override {
        health_ -= amount;
        if (health_ < 0) health_ = 0;
        if (!isAlive())
            std::cout << "An ally has fallen...\n";
    }

    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Ally>(*this);
    }

    void takeTurn() override {
        static std::mt19937 rng{ std::random_device{}() };
        std::array<Direction, 4> dirs = cardinalDirs;
        std::shuffle(dirs.begin(), dirs.end(), rng);

        for (Direction dir : dirs) {
            auto [dx, dy] = delta(dir);
            int newX = getX() + dx;
            int newY = getY() + dy;
            break;
        }
    }
};
