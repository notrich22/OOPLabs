#pragma once
#include "Board.h"
#include "PrintController.h"
#include "CombatController.h"
#include "Player.h"
#include "EnhancementState.h"
#include "ISpell.h"
#include <memory>
#include <optional>
#include "Direction.h"

class SpellController {
    Board& board;
    CombatController& combat;
    PrintController& printer;
    std::shared_ptr<Player> player;
    EnhancementState& enhancement;

public:
    SpellController(Board& b,
        CombatController& c,
        PrintController& p,
        std::shared_ptr<Player> pl,
        EnhancementState& enh)
        : board(b), combat(c), printer(p),
        player(std::move(pl)), enhancement(enh) {
    }

    bool castSpell(std::size_t spellIndex,
        std::optional<std::pair<int, int>> cell = std::nullopt,
        std::shared_ptr<ICombatEntity> target = nullptr);
};
