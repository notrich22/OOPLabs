#pragma once
#include <utility>
#include "Entity.h"
enum class Faction { Player, Enemy, Neutral };
enum class AttackMode { Melee, Ranged };

class ICombatEntity {
protected:
    int meleeAttackPower_ = 0;
    int rangedAttackPower_ = 0;
	int health_;
    AttackMode attackMode_ = AttackMode::Melee;
    
public:
    virtual ~ICombatEntity() = default;

    // ===== Позиция и фракция =====    
    virtual std::pair<int, int> getPosition() const = 0;

    virtual Faction faction() const = 0;

    // ===== Настройки атаки =====
    void setAttackMode(AttackMode m) noexcept { attackMode_ = m; }
    AttackMode getAttackMode() const noexcept { return attackMode_; }

    int getMeleeAttackPower() const noexcept { return meleeAttackPower_; }
    int getRangedAttackPower() const noexcept { return rangedAttackPower_; }

    void setAttackPowers(int melee, int ranged = 0) noexcept {
        meleeAttackPower_ = melee;
        rangedAttackPower_ = ranged;
    }

    // ===== Универсальная атака =====
    virtual void attack(ICombatEntity& target) const {
        if (!isAlive()) return;
        int damage = (attackMode_ == AttackMode::Melee)
            ? meleeAttackPower_
            : rangedAttackPower_;
        target.takeDamage(damage);
    }
    virtual bool canAttack(const ICombatEntity& target) const {
        return isAlive() && target.isAlive() && target.faction() != faction();
	}
    // ===== ХП =====
    virtual int getHealth() const noexcept { return health_; }
    virtual bool isAlive() const noexcept { return health_ > 0; };
    virtual void takeDamage(int amount) {
        if (amount <= 0 || !isAlive()) return;
        health_ -= amount;
        if (health_ < 0) health_ = 0;
    }
};
