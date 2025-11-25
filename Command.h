#pragma once
#include "Direction.h"
#include <utility>

enum class CommandType {
    None,
    Move,
    AttackMelee,
    AttackRanged,
    CastSpell,
    SwitchWeapon,
    SaveGame,
    LoadGame,
    Quit,
    SkipTurn,
    Help
};

struct Command {
    CommandType type = CommandType::None;
    Direction dir = Direction::None;         // Для движения/атаки
    int spellIndex = -1;                     // Для выбора заклинания
    std::pair<int, int> target = { -1, -1 }; // Координаты (для телепорта/AOE)
};