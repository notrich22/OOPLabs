#pragma once
#include "MovableEntity.h"
#include "ICombatEntity.h"
#include "Config.h"
#include <memory>

//  ласс Enemy Ч враждебна€ сущность
class Enemy : public MovableEntity, public ICombatEntity {
public:
    Enemy(int health = Config::ENEMY_DEFAULT_HEALTH,
        int meleeAttackPower = Config::ENEMY_MELEE_DAMAGE,
        int rangedAttackPower = 0)
        : MovableEntity() {
        health_ = health;
        meleeAttackPower_ = meleeAttackPower;
        rangedAttackPower_ = rangedAttackPower;
    }

    void takeTurn() override;
    char symbol() const noexcept override { return 'E'; }
    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Enemy>(*this);
    }

    std::pair<int, int> getPosition() const override {
        return Entity::getPosition();
    }

    Faction faction() const override { return Faction::Enemy; }

    void takeDamage(int amount) override;
};

