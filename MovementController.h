#pragma once
#include "Board.h"
#include "MovableEntity.h"
#include "CombatController.h"

// ���������� �������� �� �������� ��������� �� ����.
// �� �� ������, ��� � ����� ��������� � ������ ��� ������ ���������� ���.
class MovementController {
private:
    Board& board;
    CombatController& combat;

public:
    MovementController(Board& b, CombatController& c);

    // ������� ����������� �������� �� dx, dy.
    // ���������� true, ���� �������� (��� ���) ����������.
    bool move(MovableEntity& entity, int dx, int dy);

    // ��������, ����� �� ����� � ������
    bool canMoveTo(int x, int y) const;
};
