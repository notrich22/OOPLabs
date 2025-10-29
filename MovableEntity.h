#pragma once
#include "Entity.h"

// ����� ��������� ��������
class MovableEntity : public Entity {
protected:
    bool skipNextMove_ = false;  // ������� ���������� ����

public:
    MovableEntity() = default;
    virtual ~MovableEntity() override = default;

    // ���������� ��������� ����
    bool shouldSkipMove() const noexcept { return skipNextMove_; }
    void setSkipNextMove(bool v) noexcept { skipNextMove_ = v; }
};
