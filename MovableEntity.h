#pragma once
#include "Entity.h"
#include <iostream>

enum class AttackMode { Melee, Ranged };
// ����� ��������� �������� (�����, ���� � �.�.)
class MovableEntity : public Entity {
protected:
    int meleeAttackPower;
    int rangedAttackPower;
    AttackMode attackMode = AttackMode::Melee;
    bool skipNextMove_ = false;

public:
    MovableEntity(int health, int meleePower, int rangedPower = 0)
        : Entity(health),
        meleeAttackPower(std::max(0, meleePower)),
        rangedAttackPower(std::max(0, rangedPower)) {
    }

    virtual ~MovableEntity() = default;

    // ��������� �����
    void setAttackMode(AttackMode m) { attackMode = m; }
    AttackMode getAttackMode() const { return attackMode; }

    int getMeleeAttackPower() const { return meleeAttackPower; }
    int getRangedAttackPower() const { return rangedAttackPower; }

    // ������� ������� �����
    virtual void attack(Entity& target) {
        if (!isAlive()) return;
        target.takeDamage(attackMode == AttackMode::Melee? getMeleeAttackPower() : getRangedAttackPower());
    }
    
    // ���������� �����
    bool shouldSkipMove() const { return skipNextMove_; }
    void setSkipNextMove(bool v) { skipNextMove_ = v; }
};
