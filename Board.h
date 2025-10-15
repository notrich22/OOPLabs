#pragma once
#include <vector>
#include <memory>
#include "Cell.h"
#include "MovableEntity.h"
#include <functional>
#include <string>
#include "Player.h"

class Board {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;
public:
    Board(int width, int height);

    //  онструкторы копировани€ и перемещени€ (глубокие)
    Board(const Board& other);
    Board(Board&& other) noexcept;
    Board& operator=(const Board& other);
    Board& operator=(Board&& other) noexcept;

    ~Board() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::pair<int, int> getPosition(const Entity& e) const;

    void setPosition(Entity& e, std::pair<int, int> pos);

    bool isInside(int x, int y) const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    bool rangedAttack(Player& player, int dx, int dy, int range);

    bool moveEntity(MovableEntity& entity, int dx, int dy);
    void placeEntity(std::shared_ptr<Entity> entity, int x, int y);
};
