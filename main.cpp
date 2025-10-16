#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game(10, 10);
        game.init();
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
//TODO: Перенести логику хода в Game, сделать удаление игроков, исправить direction на enums