#pragma once
#include "Game.h"
#include "Command.h"

template <typename InputT, typename RenderT>
class GameManager {
private:
    Game& game;
    InputT& input;
    RenderT& renderer;

public:
    GameManager(Game& g, InputT& i, RenderT& r)
        : game(g), input(i), renderer(r) {
    }

    void run() {
        game.setRunning(true);

        while (game.isGameRunning()) {
            const Player& playerRef = *game.getPlayer();

            renderer.renderBoard(game.getBoard(), playerRef, game.getTurnCounter(), game.getSeed());

            Command cmd = input.getCommand(playerRef);

            bool turnMade = game.executeCommand(cmd);

            if (turnMade) {
                game.updateWorld();
            }
        }
    }
};