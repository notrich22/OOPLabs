#pragma once
#include "StaticEntity.h"
#include "ICombatEntity.h"
#include "IDirectDamageSpell.h"
#include <memory>
#include <iostream>
#include "Config.h"
#include "Config.h"
class EnemyTower final : public StaticEntity, public ICombatEntity {
private:
    int attackRange_;
    int cooldown_ ;
    std::shared_ptr<IDirectDamageSpell> spell_;
public:
    EnemyTower(std::shared_ptr<IDirectDamageSpell> spell, int range = 3, int cooldown = Config::TOWER_DEFAULT_COOLDOWN);

    char symbol() const noexcept override { return 'T'; }
    void takeTurn() override {}
    std::shared_ptr<Entity> clone() const override;

    Faction faction() const override { return Faction::Enemy; }
    void takeDamage(int amount) override;
    bool isAlive() const noexcept override { return health_ > 0; }

    std::pair<int, int> getPosition() const override { return Entity::getPosition(); }

    bool isReady() const noexcept { return cooldown_ == 0; }
    void resetCooldown() noexcept { cooldown_ = Config::TOWER_DEFAULT_COOLDOWN; }
    void tickCooldown() noexcept { if (cooldown_ > 0) --cooldown_; }


    int getAttackRange() const noexcept { return attackRange_; }

    std::shared_ptr<IDirectDamageSpell> getSpell() const noexcept { return spell_; }
};
