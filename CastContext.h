#pragma once
#include <memory>
#include <optional>
#include <utility>
#include "EnhancementState.h"
struct EnhancementState;
class Board;
class ICombatEntity;

struct CastContext {
    ICombatEntity& caster;
    Board& board;

    std::weak_ptr<ICombatEntity> target;                // ���� (��� ������� �����)
    std::optional<std::pair<int, int>> cell;      // ������ (��� AOE/�������/�������)

    EnhancementState& enhancement;               // ����� ���������
};