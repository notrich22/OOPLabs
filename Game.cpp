#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <random>
#include "Direction.h"
#include "DirectDamageSpell.h"

static std::pair<int, int> validateSize(int width, int height) {
    if (width < Config::MIN_BOARD_SIZE || height < Config::MIN_BOARD_SIZE ||
        width > Config::MAX_BOARD_SIZE || height > Config::MAX_BOARD_SIZE)
        throw std::invalid_argument("Board size must be between 10 and 25");
    return { width, height };
}

Game::Game(int w, int h, unsigned seed)
    : seed(seed),
    rng(seed),
    board(Board(validateSize(w, h).first, validateSize(w, h).second, rng)),
    combat(board),
    movement(board, combat),
    printer() {
}

void Game::init() {
    MAX_ENEMIES = static_cast<int>(board.getHeight() * board.getWidth() * Config::ENEMY_DENSITY_FACTOR);

    player = std::make_shared<Player>();
    spawnEntity(player);
    player->getSpellHand().addSpell(std::make_shared<DirectDamageSpell>());

    for (int i = 0; i < MAX_ENEMIES / 2; ++i) {
        auto enemy = std::make_shared<Enemy>();
        enemies.push_back(enemy);
        spawnEntity(enemy);
    }

    auto spawner = std::make_shared<EnemySpawner>();
    spawners.push_back(spawner);
    spawnEntity(spawner);

    isRunning = true;
    turnCounter = 1;
}

void Game::spawnEntity(const std::shared_ptr<Entity>& entity) {
    std::pair<int, int> pos;
    for (int attempts = 0; attempts < 1000; ++attempts) {
        pos = board.getRandomFreeCell(rng);
        if (board.hasFreeNeighbor(pos)) break;
    }
    board.placeEntity(entity, pos.first, pos.second);
}

void Game::run() {
    while (isRunning && !checkGameOver()) {
        printer.renderBoard(board, turnCounter, seed);

        processPlayerTurn();
        processEnemies();
        processSpawners();

        system("pause");
        turnCounter++;
    }
}

void Game::processPlayerTurn() {
    std::cout << "Player HP: " << player->getHealth() << "\n";

    std::cout << "Enter command (w/a/s/d to move, m to switch weapon, "
        << "f to shoot, c to cast spell, q to quit): ";

    // Пропуск хода после замедления
    if (player->shouldSkipMove()) {
        player->setSkipNextMove(false);
        std::cout << "You skip this turn!\n";
        return;
    }

    char cmd;
    std::cin >> cmd;

    // --- Смена оружия ---
    if (cmd == 'm') {
        player->switchMode();
        std::cout << "Attack mode switched to "
            << (player->getAttackMode() == AttackMode::Melee ? "MELEE\n" : "RANGED\n");
        player->setSkipNextMove(true);
        return;
    }

    // --- Выход ---
    if (cmd == 'q') {
        isRunning = false;
        return;
    }

    // --- Каст заклинания ---
    if (cmd == 'c') {
        auto& hand = player->getSpellHand();
        if (hand.empty()) {
            std::cout << "You have no spells!\n";
            return;
        }

        // показать список
        auto spellNames = hand.getSpellNames();
        std::cout << "\nAvailable spells:\n";
        for (std::size_t i = 0; i < spellNames.size(); ++i)
            std::cout << i + 1 << ") " << spellNames[i] << '\n';

        std::cout << "Choose spell number: ";
        std::size_t index = 0;
        std::cin >> index;

        if (index == 0 || index > spellNames.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }

        // параметры для контроллера
        std::optional<Direction> direction;
        std::optional<std::pair<int, int>> position;

        // получаем указатель на спелл
        auto spell = hand.getSpell(index - 1);
        if (!spell) return;

        // для разных типов спеллов — разные параметры
        std::string spellName = spell->name();
        if (spellName == "Firebolt") {
            std::cout << "Direction (w/a/s/d): ";
            char key; std::cin >> key;
            direction = keyToDir(key);
        }
        else if (spellName == "Explosion") {
            int x, y;
            std::cout << "Enter center (x y): ";
            std::cin >> x >> y;
            position = std::make_pair(x, y);
        }
        else if (spellName == "Trap") {
            int x, y;
            std::cout << "Enter trap position (x y): ";
            std::cin >> x >> y;
            position = std::make_pair(x, y);
        }

        return;
    }

    // --- Дальняя атака ---
    if (cmd == 'f') {
        if (player->getAttackMode() == AttackMode::Melee) {
            std::cout << "You can't shoot in melee mode!\n";
            return;
        }

        std::cout << "Attack direction (w/a/s/d): ";
        char dirKey;
        std::cin >> dirKey;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto dirOpt = keyToDir(dirKey);
        if (!dirOpt) {
            std::cout << "Invalid direction\n";
            return;
        }

        auto target = combat.findTargetInDirection(player, *dirOpt, player->getAttackRange());
        if (target)
            combat.handleCombat(player, target);
        else
            std::cout << "You missed!\n";
        return;
    }

    // --- Движение ---
    if (auto dirOpt = keyToDir(cmd)) {
        auto [dx, dy] = delta(*dirOpt);
        movement.move(*player, dx, dy);
    }
    else {
        std::cout << "Unknown command\n";
    }
}


void Game::processEnemies() {
    auto [px, py] = player->getPosition();

    for (auto& enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        if (enemy->shouldSkipMove()) {
            enemy->setSkipNextMove(false);
            std::cout << "Enemy skips this turn!\n";
            continue;
        }

        auto [ex, ey] = enemy->getPosition();
        int dx = px - ex;
        int dy = py - ey;
        int dist = std::abs(dx) + std::abs(dy);

        bool moved = false;

        // --- если игрок близко, идём к нему ---
        if (dist <= 3) {
            // выбираем ось, где смещение больше
            if (std::abs(dx) > std::abs(dy))
                dx = (dx > 0) ? 1 : -1, dy = 0;
            else
                dy = (dy > 0) ? 1 : -1, dx = 0;

            moved = movement.move(*enemy, dx, dy);

            // если не смог сдвинуться (препятствие) — попробуем другой вариант
            if (!moved) {
                std::array<Direction, 4> shuffled = cardinalDirs;
                std::shuffle(shuffled.begin(), shuffled.end(), rng);
                for (Direction dir : shuffled) {
                    auto [adx, ady] = delta(dir);
                    if (movement.move(*enemy, adx, ady)) {
                        moved = true;
                        break;
                    }
                }
            }
        }
        // --- иначе — старое поведение (рандом) ---
        else {
            std::array<Direction, 4> shuffled = cardinalDirs;
            std::shuffle(shuffled.begin(), shuffled.end(), rng);
            for (Direction dir : shuffled) {
                auto [adx, ady] = delta(dir);
                if (movement.move(*enemy, adx, ady)) {
                    moved = true;
                    break;
                }
            }
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::shared_ptr<Enemy>& e) { return !e->isAlive(); }),
        enemies.end());
}


void Game::processSpawners() {
    for (auto& spawner : spawners) {
        if (!spawner->isAlive())
            continue;

        spawner->takeTurn();

        if (static_cast<int>(enemies.size()) >= MAX_ENEMIES)
            continue;

        if (!spawner->readyToSpawn())
            continue;

        auto [sx, sy] = spawner->getPosition();
        std::array<Direction, 4> shuffled = cardinalDirs;
        std::shuffle(shuffled.begin(), shuffled.end(), rng);

        for (Direction dir : shuffled) {
            auto [dx, dy] = delta(dir);
            int nx = sx + dx;
            int ny = sy + dy;

            if (!board.isInside(nx, ny))
                continue;

            Cell& cell = board.getCell(nx, ny);
            if (cell.isOccupied() || cell.getType() == CellType::Wall)
                continue;

            auto newEnemy = std::make_shared<Enemy>(40, 10);
            enemies.push_back(newEnemy);
            board.placeEntity(newEnemy, nx, ny);
            std::cout << "Spawner created enemy at (" << nx << ", " << ny << ")\n";

            spawner->resetCounter();
            break;
        }
    }
}

bool Game::checkGameOver() const {
    if (!player->isAlive()) {
        printer.printGameOver();
        return true;
    }

    bool anyEnemiesAlive = std::any_of(enemies.begin(), enemies.end(),
        [](const auto& e) { return e->isAlive(); });

    if (!anyEnemiesAlive) {
        printer.printWin();
        return true;
    }

    return false;
}
