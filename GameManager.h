#pragma once
#include "Game.h"
#include "Command.h"

template <typename InputT, typename VisualizerT>
class GameManager {
private:
    Game& game;
    InputT input;           
    VisualizerT& visualizer;

public:
    GameManager(Game& g, VisualizerT& v)
        : game(g), visualizer(v) {

    }

    void run() {
        game.setRunning(true);
        game.logEvent(input.getStatusMessage());
        while (game.isGameRunning()) {
            visualizer.render(game);

            if (game.getPlayer()) {
                Command cmd = input.getCommand(*game.getPlayer());
                bool turnMade = game.executeCommand(cmd);
                if (turnMade) {
                    game.updateWorld();
                }
            }
        }

        if (game.getPlayer() && !game.getPlayer()->isAlive()) {
            visualizer.showGameOver();
        }
        else if (game.isVictory()) {
            visualizer.showWin();
        }
        std::cout << "\nPress Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

};