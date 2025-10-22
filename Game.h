#pragma once
#include <vector>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include <random>


// Класс Game управляет основным игровым циклом и взаимодействием объектов
class Game {
private:
    std::mt19937 rng; // генератор псевдослучайных чисел с фиксируемым seed
    unsigned seed = 0;
    Board board;                                      // игровое поле
    std::shared_ptr<Player> player;                   // игрок
    std::vector<std::shared_ptr<Enemy>> enemies;      // список врагов
    std::vector<std::shared_ptr<EnemySpawner>> spawners; // спавнеры врагов

    int turnCounter = 1;   // счётчик ходов
    bool isRunning = false; // флаг состояния игры
    int MAX_ENEMIES = 0;    // максимальное число врагов на поле

	//Физика выстрела в заданном направлении
    std::shared_ptr<Entity> findTargetInDirection(const std::shared_ptr<Entity>& source, Direction dir, int range) const;

    //Логика боя между двумя сущностями
    bool handleCombat(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender);

    // Ход игрока
    void processPlayerTurn();

    // Обработка действий врагов
    void processEnemies();

    // Работа спавнеров (создание новых врагов)
    void processSpawners();

    //Логика перемещения сущности
    bool moveEntity(MovableEntity& entity, int dx, int dy);

public:
    // Конструктор инициализирует игру с заданным размером поля
    Game(int width, int height, unsigned seed = std::random_device{}());

    // Инициализация объектов
    void init();

	// Спавн сущности на свободной и достижимой клетке
    void spawnEntity(const std::shared_ptr<Entity>& entity);

    // Главный игровой цикл
    void run();

    // Очистка консоли
    void clearCmd() const;

    // Вывод заголовка игры
    void printTitle() const;

	//Вывод сида игры
	void printSeed() const;

    // Проверка завершения игры
    bool checkGameOver() const;

    // Вывод экрана победы
    void printWin() const;

    // Отрисовка игрового состояния
    void render() const;
};
