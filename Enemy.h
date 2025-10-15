#pragma once
#include "MovableEntity.h"
#include <memory>

// ����� Enemy - �������� �����, ��������� ������������ � ���������
class Enemy : public MovableEntity {
public:
    // ����������� �������������� ��������� �������� � �����
    Enemy(int health, int meleeAttackPower, int rangedAttackPower = 0)
        : MovableEntity(health, meleeAttackPower, rangedAttackPower) {
    }

    Enemy(const Enemy&) noexcept = default;

    // ������ ��������� �����
    void takeTurn() override;            // �������� ����� �� ���� ����
    void takeDamage(int dmg) override;   // ��������� �����

    // �������� ����� �������
    std::shared_ptr<Entity> clone() const override;
};
