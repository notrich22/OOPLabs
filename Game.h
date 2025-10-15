#pragma once
#include <vector>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"

// Класс Game управляет основным игровым циклом и взаимодействием объектов
class Game {
private:
    Board board;                                      // игровое поле
    std::shared_ptr<Player> player;                   // игрок
    std::vector<std::shared_ptr<Enemy>> enemies;      // список врагов
    std::vector<std::shared_ptr<EnemySpawner>> spawners; // спавнеры врагов

    int turnCounter = 1;   // счётчик ходов
    bool isRunning = false; // флаг состояния игры
    int MAX_ENEMIES = 0;    // максимальное число врагов на поле

public:
    // Конструктор инициализирует игру с заданным размером поля
    Game(int width, int height);

    // Инициализация объектов
    void init();

    // Главный игровой цикл
    void run();

    // Ход игрока
    void processPlayerTurn();

    // Обработка действий врагов
    void processEnemies();

    // Работа спавнеров (создание новых врагов)
    void processSpawners();

    // Очистка консоли
    void clearCmd() const;

    // Вывод заголовка игры
    void printTitle() const;

    // Проверка завершения игры
    bool checkGameOver() const;

    // Вывод экрана победы
    void printWin() const;

    // Отрисовка игрового состояния
    void render() const;
};
