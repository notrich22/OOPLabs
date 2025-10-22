#include "Game.h"
#include <iostream>

int main() {
    try {
        unsigned seed;
        std::cout << "Enter seed (0 for random): ";
        std::cin >> seed;
        if (seed == 0) seed = std::random_device{}();
        Game game(10, 10, seed);
        game.init();
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}