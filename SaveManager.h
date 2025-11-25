#pragma once
#include <string>
#include "Game.h"

class SaveManager {
public:
    static void saveGame(const Game& game, const std::string& filename);
    static void loadGame(Game& game, const std::string& filename);
};
