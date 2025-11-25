#include "Game.h"
#include "InputHandler.h"
#include "ConsoleRenderer.h"
#include "GameManager.h"
#include "Logger.h"
#include "SaveManager.h"
#include "Exceptions.h"

#include <iostream>
#include <limits>
#include <random>

int main() {
    try {
        unsigned seed = std::random_device{}();
        Game game(10, 10, seed);


        FileLogger fileLogger("game_log.txt");
        game.addObserver(&fileLogger);

        ConsoleRenderer renderer;
        game.addObserver(&renderer);

        InputHandler input("controls.cfg");

        std::cout << "=== WELCOME TO THE STEPWARS ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "Choose option: ";

        char choice;
        std::cin >> choice;

        if (choice == '2') {
            try {
                SaveManager::loadGame(game, "save.json");
                std::cout << "Save loaded successfully.\n";

                std::cout << "Press Enter to start...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
            catch (const std::exception& e) {
                std::cerr << "\n[ERROR] Load failed: " << e.what() << "\n";
                std::cout << "Starting NEW GAME instead.\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();

                game.init();
            }
        }
        else {
            std::cout << "Enter seed (0 for random): ";
            unsigned userSeed;
            std::cin >> userSeed;

            if (userSeed != 0) {
                game.setSeed(userSeed);
            }

            game.init();
        }

        GameManager<InputHandler, ConsoleRenderer> manager(game, input, renderer);

        manager.run();

    }
    catch (const std::exception& e) {
        std::cerr << "\nFATAL ERROR: " << e.what() << "\n";
        std::cout << "Press Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }

    return 0;
}