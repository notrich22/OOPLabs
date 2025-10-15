#pragma once
#include "Entity.h"

// ����� StaticEntity ��������� ����������� ��������, ������� �������� � ����, �� �� ��������� ������������
class StaticEntity : public Entity {
public:
    // ����������� �������������� ������� ��������� ��������
    StaticEntity(int health, int meleeAttackPower, int rangedAttackPower)
        : Entity(health, meleeAttackPower, rangedAttackPower) {
    }

    virtual ~StaticEntity() = default;

    // ����������� �����: ����������� �������� ������ ����������� ����������� ���������
    void takeTurn() override = 0;
};
