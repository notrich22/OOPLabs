#pragma once
#include <memory>
#include <optional>
#include <utility>
#include "EnhancementState.h"

class Player;
class Board;
class ICombatEntity;

inline int manhattan(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

struct CastContext {
    Player& caster;
    Board& board;

    std::weak_ptr<ICombatEntity> target;                // ���� (��� ������� �����)
    std::optional<std::pair<int, int>> cell;      // ������ (��� AOE/�������/�������)

    EnhancementState& enhancement;               // ����� ���������
};
