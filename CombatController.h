#pragma once
#include "Board.h"
#include "Relationships.h"
#include "ICombatEntity.h"
#include "Player.h"
#include <memory>

class CombatController {
    Board& board;
public:
    CombatController(Board& b)
        : board(b){
    }

    // ѕоиск первой цели в направлении dir в пределах range
    std::shared_ptr<Entity> findTargetInDirection(
        const std::shared_ptr<ICombatEntity>& source,
        Direction dir,
        int range) const;

    // ќбработка бо€ (атака и уничтожение)
    bool handleCombat(
        const std::shared_ptr<ICombatEntity>& attacker,
        const std::shared_ptr<ICombatEntity>& defender);

};
