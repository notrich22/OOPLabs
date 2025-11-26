#include "Game.h"
#include "InputHandler.h"
#include "ConsoleRenderer.h"
#include "GameVisualizer.h"
#include "GameManager.h"
#include "Logger.h"
#include "SaveManager.h"
#include "Exceptions.h"

#include <iostream>
#include <limits>
#include <random>
#include <cstring>

int main(int argc, char* argv[]) {
    try {
        bool logToFile = true;
        bool logToConsole = true;

        if (argc > 1) {
            if (std::strcmp(argv[1], "-f") == 0 || std::strcmp(argv[1], "--file") == 0) {
                logToConsole = false;
                std::cout << "Logging Mode: FILE ONLY\n";
            }
            else if (std::strcmp(argv[1], "-c") == 0 || std::strcmp(argv[1], "--console") == 0) {
                logToFile = false;
                std::cout << "Logging Mode: CONSOLE ONLY\n";
            }
        }

        unsigned seed = std::random_device{}();
        Game game(10, 10, seed);

        FileLogger fileLogger("game_log.txt");
        if (logToFile) {
            game.addObserver(&fileLogger);
        }

        GameVisualizer<ConsoleRenderer> visualizer;

        if (logToConsole) {
            game.addObserver(&visualizer);
        }

        std::cout << "=== WELCOME TO THE GAME ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "Choose option: ";

        char choice;
        std::cin >> choice;

        if (choice == '2') {
            try {
                SaveManager::loadGame(game, "save.json");
                game.logEvent("Save loaded successfully.");
                if (logToConsole) {
                    visualizer.setLogs(game.getLogs());
                }
                std::cout << "Press Enter to start...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();

                game.setRunning(true);
            }
            catch (const std::exception& e) {
                game.logEvent("[System Error] Load failed: " + std::string(e.what()));
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

        GameManager<InputHandler, GameVisualizer<ConsoleRenderer>> manager(game, visualizer);

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