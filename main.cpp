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