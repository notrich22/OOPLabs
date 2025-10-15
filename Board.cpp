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
    if (!isInside(x, y)) return;
    grid[y][x].setEntity(std::move(entity));
}

bool Board::moveEntity(MovableEntity& entity, int dx, int dy) {
    std::pair<int, int> pos{ -1, -1 };

    // найти координаты сущности
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto cellEntity = grid[y][x].getEntity();
            if (cellEntity && cellEntity.get() == &entity) {
                pos = { x, y };
                break;
            }
        }
        if (pos.first != -1) break;
    }

    if (pos.first == -1) return false; // не найдено
    int newX = pos.first + dx;
    int newY = pos.second + dy;
    if (!isInside(newX, newY)) return false;

    Cell& current = getCell(pos.first, pos.second);
    Cell& target = getCell(newX, newY);

    // если в клетке есть враг/игрок → бой
    if (target.isOccupied()) {
        auto attacker = current.getEntity();
        auto defender = target.getEntity();

        // игрок атакует врага
        if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
            if(player->getAttackMode() == AttackMode::Melee)
                if (auto enemy = std::dynamic_pointer_cast<Enemy>(defender)) {
                    enemy->takeDamage(player->getMeleeAttackPower());
                    std::cout << "Player hits enemy for " << player->getMeleeAttackPower() << " damage!\n";
                    if (!enemy->isAlive()) {
                        std::cout << "Enemy defeated!\n";
                        target.clearEntity();
                        player->addExperience(10);
                    }
                    return true;
                }
        }

        // враг атакует игрока
        if (auto enemy = std::dynamic_pointer_cast<Enemy>(attacker)) {
            if (auto player = std::dynamic_pointer_cast<Player>(defender)) {
                player->takeDamage(enemy->getMeleeAttackPower());
                std::cout << "Enemy hits player for " << enemy->getMeleeAttackPower() << " damage!\n";
                if (!player->isAlive())
                    std::cout << "Player died!\n";
                return true;
            }
        }

        // Игрок атакует спавнер
        if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
            if (auto spawner = std::dynamic_pointer_cast<EnemySpawner>(defender)) {
                spawner->takeDamage(player->getMeleeAttackPower());
                std::cout << "Player hits spawner for "
                    << player->getMeleeAttackPower() << " damage!\n";
                if (!spawner->isAlive()) {
                    std::cout << "Spawner destroyed!\n";
                    target.clearEntity();
                }
                return true;
            }
        }

        return false; // остальные комбинации не взаимодействуют
    }

    // перемещение
    if (target.getType() == CellType::Wall) return false;

    target.setEntity(current.getEntity());
    current.clearEntity();

    // эффект замедляющей клетки
    if (target.getType() == CellType::Slow) {
        entity.setSkipNextMove(true);
    }

    return true;
}

bool Board::rangedAttack(Player& player, int dx, int dy, int range) {
	if (player.getAttackMode() != AttackMode::Ranged) return false;
    // находим позицию игрока
    std::pair<int, int> pos{ -1, -1 };
    for (int y = 0; y < this->getHeight(); ++y) {
        for (int x = 0; x < this->getWidth(); ++x) {
            if (grid[y][x].getEntity().get() == &player) {
                pos = { x, y };
                break;
            }
        }
        if (pos.first != -1) break;
    }
    if (pos.first == -1) return false;

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