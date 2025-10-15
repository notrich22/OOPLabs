#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Cell.h"
#include "MovableEntity.h"
#include "Player.h"

// Класс Board представляет игровое поле и управляет объектами на нём
class Board {
private:
    int width;   // ширина поля
    int height;  // высота поля
    std::vector<std::vector<Cell>> grid;  // сетка клеток

public:
    // Конструкторы
    Board(int width, int height);
    Board(const Board& other);            // копирование (глубокое)
    Board(Board&& other) noexcept;        // перемещение
    Board& operator=(const Board& other);
    Board& operator=(Board&& other) noexcept;
    ~Board() = default;

    // Геттеры
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Работа с клетками
    bool isInside(int x, int y) const;    // проверка, внутри ли координаты
    Cell& getCell(int x, int y);          // получить клетку
    const Cell& getCell(int x, int y) const;

    // Работа с объектами
    void placeEntity(std::shared_ptr<Entity> entity, int x, int y);  // разместить сущность
    bool moveEntity(MovableEntity& entity, int dx, int dy);          // переместить сущность
    bool rangedAttack(Player& player, int dx, int dy, int range);    // дальняя атака игрока
};
