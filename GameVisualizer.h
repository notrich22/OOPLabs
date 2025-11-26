#pragma once
#include "IObservable.h"
#include "Game.h"


template <typename RenderT>
class GameVisualizer : public IGameObserver {
private:
    RenderT renderer;

public:
    GameVisualizer() = default;
    void onGameEvent(const std::string& message) override {
        renderer.addMessage(message);
    }
    void render(const Game& game) {
        if (game.getPlayer()) {
            renderer.renderBoard(game.getBoard(), *game.getPlayer(), game.getTurnCounter(), game.getSeed());
        }
    }
    void setLogs(const std::vector<std::string>& logs) {
        renderer.setBuffer(logs);
    }
    void showGameOver() { renderer.printGameOver(); }
    void showWin() { renderer.printWin(); }
};