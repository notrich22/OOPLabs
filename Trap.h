#pragma once
#include "StaticEntity.h"
#include <iostream>

class Trap : public StaticEntity {
private:
    int damage_;
    bool armed_ = true;

public:
    Trap(int damage)
        : StaticEntity(),
        damage_(damage)
    {
    }

    // ������� ������ �� ������ ���� �� ����
    void takeTurn() override {}

    // ������ �� ����
    char symbol() const noexcept override {
        return armed_ ? 'T' : 't'; // ��������� t = ���������
    }

    void trigger(ICombatEntity& victim) {
        if (!armed_) return;
        victim.takeDamage(damage_);
        armed_ = false;
    }

    bool isArmed() const noexcept { return armed_; }

    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Trap>(*this);
    }
};
