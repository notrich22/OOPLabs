#pragma once
#include "IObservable.h"
#include "Board.h"
#include <string>
#include <vector>

class ConsoleRenderer : public IGameObserver {
private:
    mutable std::vector<std::string> messageBuffer;

public:
    void clearScreen() const;
    void renderBoard(const Board& board, const Player& player, int turnCounter, unsigned seed) const;    void printWin() const;
    void printGameOver() const;
    void printMessage(const std::string& msg) const;

    void addMessage(const std::string& msg) const {
        messageBuffer.push_back(msg);
        if (messageBuffer.size() > 5) {
            messageBuffer.erase(messageBuffer.begin());
        }
    }

    void onGameEvent(const std::string& message) override {
        addMessage(message);
    }

private:
    void printTitle() const;
    void printSeed(unsigned seed) const;
};