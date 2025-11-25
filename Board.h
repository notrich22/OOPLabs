#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Cell.h"
#include "MovableEntity.h"
#include "Player.h"
#include <random>

// Класс Board представляет игровое поле и управляет объектами на нём
class Board {
private:
    int width;   // ширина поля
    int height;  // высота поля
    std::vector<std::vector<Cell>> grid;  // сетка клеток

public:
    // Конструкторы
    Board(int width, int height, std::mt19937& rng);
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
    Cell& getCell(std::pair<int, int> xy);          // получить клетку
    const Cell& getCell(std::pair<int, int> xy) const;          // получить клетку
    const Cell& getCell(int x, int y) const;
    std::pair<int, int> getRandomFreeCell(std::mt19937& rng) const;
	bool hasFreeNeighbor(const std::pair<int, int>& pos) const; // проверка достижимости клетки

    // Работа с объектами
    void placeEntity(std::shared_ptr<Entity> entity, int x, int y);  // разместить сущность
    void clearEntities();
};
