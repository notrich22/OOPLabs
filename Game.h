#pragma once
#include "IObservable.h" 
#include "Command.h"
#include <vector>
#include <memory>
#include <random>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "CombatController.h"
#include "MovementController.h"
#include "ConsoleRenderer.h"
#include "TowerController.h"
#include "Ally.h"
class SaveManager;
// Класс Game управляет общим циклом игры
class Game : public IObservable{
private:
    // Базовые компоненты игры
    std::mt19937 rng;      // генератор случайных чисел
    unsigned seed = 0;
    Board board;
	EnhancementState enhancement;
    friend class SaveManager;
    // Основные сущности
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<EnemySpawner>> spawners;
	std::vector<std::shared_ptr<Ally>> allies;
    // Контроллеры
    CombatController combat;
    MovementController movement;
    TowerController towerController;
    // Состояние игры
    int turnCounter = 1;
    bool isRunning = false;
    int MAX_ENEMIES = 0;

    // Ходы и фазы игры
    void processLevelUp();
    void processPlayerTurn();
    void processAllies();
    void processEnemies();
    void processSpawners();
	void processTowers();
    // Вспомогательные методы
    bool checkGameOver() const;
    char lastCommand = '\0';

	int currentLevel = 1;

public:
    bool isGameRunning() const { return isRunning; }
    void setRunning(bool v) { isRunning = v; }
    bool executeCommand(const Command& cmd);
    void updateWorld();

    // Конструктор
    Game(int width, int height, unsigned seed = std::random_device{}());
    bool isPlayerTurn() const;
    // Инициализация объектов
    void init(std::shared_ptr<Player> existingPlayer = nullptr);

    // Главный игровой цикл
    void run();

	// Переход на следующий уровень
    void nextLevel();

    // Доступ к основным объектам
    std::shared_ptr<Player> getPlayer() const { return player; }
    void setPlayer(const std::shared_ptr<Player>& p) { player = p; }

    // Размещение сущностей на карте
    void spawnEntity(const std::shared_ptr<Entity>& entity);

    // Враги
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
    void addEnemy(const std::shared_ptr<Enemy>& e) { enemies.push_back(e); }

    // Союзники
    const std::vector<std::shared_ptr<Ally>>& getAllies() const { return allies; }
    void addAlly(const std::shared_ptr<Ally>& a) { allies.push_back(a); }

    // Спавнеры
    const std::vector<std::shared_ptr<EnemySpawner>>& getSpawners() const { return spawners; }
    void addSpawner(const std::shared_ptr<EnemySpawner>& s) { spawners.push_back(s); }

    // Башни
    const std::vector<std::shared_ptr<EnemyTower>>& getTowers() const {
        return towerController.getTowers();
    }
    void addTower(const std::shared_ptr<EnemyTower>& t) {
        auto [x, y] = t->getPosition();
        towerController.addTower(t, x, y);
    }
    void placeEntityAt(const std::shared_ptr<Entity>& entity, int x, int y) {
        board.placeEntity(entity, x, y);
    }

    // Turn / Seed
    int getTurnCounter() const { return turnCounter; }
    void setTurnCounter(int t) { turnCounter = t; }
	int getLevel() const { return currentLevel; }
	int setLevel(int lvl) { return currentLevel = lvl; }
    unsigned getSeed() const { return seed; }

    void setSeed(unsigned s) {
        seed = s;
        rng = std::mt19937(seed);
    }
    void generateBoard() {
        board = Board(board.getWidth(), board.getHeight(), rng);
    }

    Board& getBoard() { return board; }
    const Board& getBoard() const { return board; }

    // Очистка всех сущностей (для загрузки)
    void clearEntities() {
        // очистка всего
        enemies.clear();
        allies.clear();
        spawners.clear();
        towerController.clear();
        player.reset();

        // очистка доски
        board.clearEntities();
    }

};
