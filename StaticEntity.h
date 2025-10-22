#pragma once
#include "Entity.h"

// ����� StaticEntity ��������� ����������� ��������, ������� �������� � ����, �� �� ��������� ������������
class StaticEntity : public Entity {
public:
    // ����������� �������������� ������� ��������� ��������
    StaticEntity(int health)
        : Entity(health) {
    }

    virtual ~StaticEntity() = default;

    // ����������� �����: ����������� �������� ������ ����������� ����������� ���������
    void takeTurn() override = 0;
};
