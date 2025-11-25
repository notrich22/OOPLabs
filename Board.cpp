#include "Board.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include <cstdlib>
#include <queue>
#include <set>
#include "Config.h"

Board::Board(int width, int height, std::mt19937& rng)
    : width(width), height(height), grid(height, std::vector<Cell>(width))
{
    std::uniform_real_distribution<double> dist(0.0, 1.0); // от 0 до 1

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double r = dist(rng);

            if (r < Config::WALL_PROBABILITY)
                grid[y][x].setType(CellType::Wall);
            else if (r < Config::WALL_PROBABILITY + Config::SLOW_PROBABILITY)
                grid[y][x].setType(CellType::Slow);
            else
                grid[y][x].setType(CellType::Normal);
        }
    }
}

Board::Board(const Board& other)
    : width(other.width), height(other.height), grid(other.grid) {
}

Board::Board(Board&& other) noexcept
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

Board& Board::operator=(Board&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool Board::isInside(int x, int y) const {
    return x >= 0 && y >= 0 && x < width && y < height;
}

Cell& Board::getCell(int x, int y) {
    return grid[y][x];
}

const Cell& Board::getCell(int x, int y) const {
    return grid[y][x];
}

Cell& Board::getCell(std::pair<int, int> xy) {
    return grid[xy.second][xy.first];
}

const Cell& Board::getCell(std::pair<int, int> xy) const{
    return grid[xy.second][xy.first];
}

void Board::placeEntity(std::shared_ptr<Entity> entity, int x, int y) {
    if (!isInside(x, y))
        throw std::out_of_range("Invalid position for entity");

    Cell& cell = getCell(x, y);

    if (cell.isOccupied())
        throw std::logic_error("Cell is already occupied");

    cell.setEntity(entity);
    entity->setPosition(x, y);
}

bool Board::hasFreeNeighbor(const std::pair<int, int>& pos) const {
    if (!isInside(pos.first, pos.second) || grid[pos.second][pos.first].isOccupied())
        return false;

    const std::vector<std::pair<int, int>> dirs = { {1,0},{-1,0},{0,1},{0,-1} };
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;

    q.push(pos);
    visited.insert(pos);

    int steps = 0;
    while (!q.empty() && steps < 8) {
        auto [x, y] = q.front();
        q.pop();
        ++steps;

        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (!isInside(nx, ny) || visited.count({ nx, ny })) continue;
            if (!grid[nx][ny].isOccupied()) return true;
            visited.insert({ nx, ny });
        }
    }
    return false;
}

std::pair<int, int> Board::getRandomFreeCell(std::mt19937& rng) const {
    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    while (true) {
        int x = distX(rng);
        int y = distY(rng);
        if (!grid[y][x].isOccupied() && grid[y][x].getType() != CellType::Wall)
            return { x, y };
    }
}

void Board::clearEntities() {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            grid[y][x].clearEntity();
}
