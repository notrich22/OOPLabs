#pragma once
#include "Entity.h"
#include <memory>

// ����� MovableEntity ��������� ��������, ��������� ������������ �� ����
class MovableEntity : public Entity {
protected:
    bool skipNextMove_ = false;  // ������� ���������� ����

public:
    // ����������� �������������� ��������� �������� � �����
    MovableEntity(int health, int meleeAttackPower, int rangedAttackPower)
        : Entity(health, meleeAttackPower, rangedAttackPower) {
    }

    MovableEntity(const MovableEntity&) = default;
    MovableEntity(MovableEntity&&) noexcept = default;
    MovableEntity& operator=(const MovableEntity&) = default;
    MovableEntity& operator=(MovableEntity&&) noexcept = default;
    virtual ~MovableEntity() = default;

    // �������� ����� �������
    virtual std::shared_ptr<Entity> clone() const = 0;

    // ���������� ���������� �������� ����
    bool shouldSkipMove() const { return skipNextMove_; }
    void setSkipNextMove(bool v) { skipNextMove_ = v; }
};
