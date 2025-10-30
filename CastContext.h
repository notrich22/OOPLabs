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

    std::weak_ptr<ICombatEntity> target;                // цель (для прямого урона)
    std::optional<std::pair<int, int>> cell;      // клетка (для AOE/ловушки/призыва)

    EnhancementState& enhancement;               // стеки улучшения
};