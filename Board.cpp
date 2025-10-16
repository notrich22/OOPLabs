#include "Board.h"
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include <cstdlib>

Board::Board(int width, int height)
    : width(width), height(height), grid(height, std::vector<Cell>(width))
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            int r = std::rand() % 100;

            if (r < 10)
                grid[y][x].setType(CellType::Wall);      // 10 % стен
            else if (r < 15)
                grid[y][x].setType(CellType::Slow);      // 5 % замедляющих
            else
                grid[y][x].setType(CellType::Normal);    // остальное — обычные
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

void Board::placeEntity(std::shared_ptr<Entity> entity, int x, int y) {
    if (!isInside(x, y))
        throw std::out_of_range("Invalid position for entity");

    Cell& cell = getCell(x, y);

    if (cell.isOccupied())
        throw std::logic_error("Cell is already occupied");

    cell.setEntity(entity);
    entity->setPosition(x, y);
}


std::pair<int, int> Board::getRandomFreeCell() const {
    int x, y;
    do {
        x = std::rand() % width;
        y = std::rand() % height;
    } while (grid[y][x].isOccupied() || grid[y][x].getType() == CellType::Wall);
    return { x, y };
}

bool Board::rangedAttack(Player& player, int dx, int dy, int range) {
	if (player.getAttackMode() != AttackMode::Ranged) return false;
    // находим позицию игрока
    auto pos = player.getPosition();
    // проверяем клетки по направлению dx/dy
    for (int step = 1; step <= range; ++step) {
        int nx = pos.first + dx * step;
        int ny = pos.second + dy * step;
        if (!isInside(nx, ny)) break;

        Cell& target = getCell(nx, ny);
        if (target.getType() == CellType::Wall) break; // снаряд врезался в стену

        if (target.isOccupied()) {
            auto targetEntity = target.getEntity();
            if (auto enemy = std::dynamic_pointer_cast<Enemy>(targetEntity)) {
                enemy->takeDamage(player.getRangedAttackPower());
                std::cout << "Ranged attack hits enemy for "
                    << player.getRangedAttackPower() << " damage!\n";
                if (!enemy->isAlive()) {
                    std::cout << "Enemy killed by ranged attack!\n";
                    target.clearEntity();
                    player.addExperience(10);
                }
                return true;
            }
            if (auto spawner = std::dynamic_pointer_cast<EnemySpawner>(targetEntity)) {
                spawner->takeDamage(player.getRangedAttackPower());
                std::cout << "Ranged attack hits spawner for "
                    << player.getRangedAttackPower() << " damage!\n";
                if (!spawner->isAlive()) {
                    std::cout << "Spawner destroyed by ranged attack!\n";
                    target.clearEntity();
                }
                return true;
            }
        }
    }

    std::cout << "Ranged attack missed.\n";
    return false;
}