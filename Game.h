#pragma once
#include <vector>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"

class Game {
private:
    Board board;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<EnemySpawner>> spawners;
    int turnCounter = 0;
    bool isRunning = false;
    static constexpr int MAX_ENEMIES = 7;
    std::vector<std::string> logMessages;
public:
    void addLog(const std::string& msg) { logMessages.push_back(msg); }
    void clearLog() { logMessages.clear(); }
    Game(int width, int height);

    void init();
    void run();
    void processPlayerTurn();
    void processEnemies();
    void processSpawners();
    void printTitle() const;
    bool checkGameOver() const;

    void render() const;
};
