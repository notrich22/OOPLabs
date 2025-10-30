#include "Game.h"

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
    printer(),
    towerController(board){}



void Game::init() {
    MAX_ENEMIES = static_cast<int>(board.getHeight() * board.getWidth() * Config::ENEMY_DENSITY_FACTOR);

    // === Игрок ===
    player = std::make_shared<Player>();
    spawnEntity(player);

    // стартовые заклинания
    auto& hand = player->getSpellHand();
	hand.addRandomSpell();

    // === Настройка генератора случайных позиций ===
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distX(0, board.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, board.getHeight() - 1);

    // === Враги ===
    for (int i = 0; i < MAX_ENEMIES / 2; ++i) {
        auto enemy = std::make_shared<Enemy>();

        // ищем пустую клетку для спавна
        for (int attempt = 0; attempt < 30; ++attempt) {
            int x = distX(rng);
            int y = distY(rng);
            auto& cell = board.getCell(x, y);
            if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                board.placeEntity(enemy, x, y);
                enemies.push_back(enemy);
                break;
            }
        }
    }

    // === Спавнеры ===
    int numSpawners = std::max(1, static_cast<int>(board.getWidth() * board.getHeight() * Config::SPAWNER_DENSITY_FACTOR));
    for (int i = 0; i < numSpawners; ++i) {
        auto spawner = std::make_shared<EnemySpawner>();
        for (int attempt = 0; attempt < 30; ++attempt) {
            int x = distX(rng);
            int y = distY(rng);
            auto& cell = board.getCell(x, y);
            if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                board.placeEntity(spawner, x, y);
                spawners.push_back(spawner);
                std::cout << "Spawner placed at (" << x << "," << y << ")\n";
                break;
            }
        }
    }

    // === Башни ===
    int numTowers = std::max(1, static_cast<int>(board.getWidth() * board.getHeight() * Config::ENEMYTOWER_DENSITY_FACTOR));
    for (int i = 0; i < numTowers; ++i) {
        auto tower = std::make_shared<EnemyTower>(
            std::make_shared<Firebolt>(5, 3), // слабая версия фаерболта
            3                                 // радиус атаки
        );

        for (int attempt = 0; attempt < 30; ++attempt) {
            int x = distX(rng);
            int y = distY(rng);
            auto& cell = board.getCell(x, y);
            if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                towerController.addTower(tower, x, y);
                std::cout << "Enemy tower placed at (" << x << "," << y << ")\n";
                break;
            }
        }
    }

    // === Финал инициализации ===
    isRunning = true;
    turnCounter = 1;

    std::cout << "Game initialized: "
        << enemies.size() << " enemies, "
        << spawners.size() << " spawners, "
        << numTowers << " towers.\n";
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
        processAllies();
        processEnemies();
        processSpawners();
        processTowers();
        system("pause");
        turnCounter++;
    }
}
void Game::processTowers() {
    if (towerController.getTowers().empty())
        return;

    towerController.update();
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
    std::cin >> cmd;  // нормальный ввод без get()
    cmd = std::tolower(cmd);

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

        auto spellNames = hand.getSpellNames();
        std::cout << "\nAvailable spells:\n";
        for (std::size_t i = 0; i < spellNames.size(); ++i)
            std::cout << i + 1 << ") " << spellNames[i] << '\n';

        std::cout << "Choose spell number: ";
        std::size_t index;
        if (!(std::cin >> index)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            return;
        }

        if (index == 0 || index > spellNames.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }

        auto spell = hand.getSpell(index - 1);
        if (!spell) return;

        std::string spellName = spell->name();

        // === FIREBOLT ===
        if (spellName == "Firebolt") {
            std::cout << "Direction (w/a/s/d): ";
            char key;
            std::cin >> key;
            auto direction = keyToDir(std::tolower(key));

            if (!direction) {
                std::cout << "Invalid direction.\n";
                return;
            }

            int maxRange = 3 + player->getEnhancementState().rangeBonus();
            auto target = combat.findTargetInDirection(player, *direction, maxRange);

            if (!target) {
                std::cout << "No enemy in that direction!\n";
                return;
            }

            auto combatTarget = std::dynamic_pointer_cast<ICombatEntity>(target);
            CastContext ctx{
                *player,
                board,
                combatTarget,
                combatTarget->getPosition(),
                player->getEnhancementState()
            };

            if (spell->canCast(ctx)) spell->cast(ctx);
            else std::cout << "Cannot cast " << spellName << " right now.\n";
        }

        // === EXPLOSION ===
        else if (spellName == "Explosion") {
            int x, y;
            std::cout << "Enter center (x y): ";
            if (!(std::cin >> x >> y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid coordinates.\n";
                return;
            }

            CastContext ctx{
                *player, board, {}, std::make_pair(x, y),
                player->getEnhancementState()
            };

            if (spell->canCast(ctx)) {
                spell->cast(ctx);
                std::cout << "Explosion cast complete!\n";
            }
            else {
                std::cout << "Cannot cast Explosion at (" << x << ", " << y << ").\n";
            }
        }

        // === TRAP ===
        else if (spellName == "TrapSpell") {
            int x, y;
            std::cout << "Enter trap position (x y): ";
            if (!(std::cin >> x >> y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid coordinates.\n";
                return;
            }

            CastContext ctx{
                *player, board, {}, std::make_pair(x, y),
                player->getEnhancementState()
            };

            if (spell->canCast(ctx) && spell->cast(ctx))
                std::cout << "Trap placed at (" << x << ", " << y << ") successfully!\n";
            else
                std::cout << "Failed to place trap at (" << x << ", " << y << ").\n";
        }

        // === SUMMON ===
        else if (spellName == "SummonAlly") {
            CastContext ctx{
                *player, board, {}, player->getPosition(),
                player->getEnhancementState()
            };

            if (spell->canCast(ctx) && spell->cast(ctx)) {
                std::cout << "Ally summoned successfully!\n";
                auto [px, py] = player->getPosition();
                for (Direction dir : cardinalDirs) {
                    auto [dx, dy] = delta(dir);
                    int nx = px + dx, ny = py + dy;
                    if (!board.isInside(nx, ny)) continue;
                    auto entity = board.getCell(nx, ny).getEntity();
                    if (entity && entity->symbol() == 'A') {
                        if (auto ally = std::dynamic_pointer_cast<Ally>(entity))
                            allies.push_back(ally);
                    }
                }
            }
            else {
                std::cout << "Cannot cast SummonAlly right now.\n";
            }
        }

        // === ENHANCE ===
        else if (spellName == "EnhanceSpell") {
            CastContext ctx{
                *player, board, {}, player->getPosition(),
                player->getEnhancementState()
            };

            if (spell->canCast(ctx)) {
                spell->cast(ctx);
                std::cout << "Enhancement applied!\n";
            }
            else {
                std::cout << "Cannot cast EnhanceSpell right now.\n";
            }
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
        auto dirOpt = keyToDir(std::tolower(dirKey));

        if (!dirOpt) {
            std::cout << "Invalid direction\n";
            return;
        }

        auto target = combat.findTargetInDirection(player, *dirOpt, player->getAttackRange());
        if (target)
            combat.handleCombat(player, std::dynamic_pointer_cast<ICombatEntity>(target));
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


void Game::processAllies() {
    if (allies.empty() || enemies.empty()) return;

    for (auto& ally : allies) {
        if (!ally || !ally->isAlive()) continue;

        if (ally->shouldSkipMove()) {
            ally->setSkipNextMove(false);
            std::cout << "Ally skips this turn!\n";
            continue;
        }

        // === ищем ближайшего врага ===
        std::shared_ptr<Enemy> nearestEnemy = nullptr;
        int minDist = std::numeric_limits<int>::max();

        auto [ax, ay] = ally->getPosition();

        for (auto& enemy : enemies) {
            if (!enemy || !enemy->isAlive()) continue;

            auto [ex, ey] = enemy->getPosition();
            int dist = std::abs(ax - ex) + std::abs(ay - ey);
            if (dist < minDist) {
                minDist = dist;
                nearestEnemy = enemy;
            }
        }

        bool moved = false;

        // === если враг найден и он близко — идём к нему ===
        if (nearestEnemy && minDist <= 5) {
            auto [ex, ey] = nearestEnemy->getPosition();
            int dx = ex - ax;
            int dy = ey - ay;

            if (std::abs(dx) > std::abs(dy))
                dx = (dx > 0) ? 1 : -1, dy = 0;
            else
                dy = (dy > 0) ? 1 : -1, dx = 0;

            moved = movement.move(*ally, dx, dy);

            // если не смог сдвинуться (препятствие) — пробуем случайно
            if (!moved) {
                std::array<Direction, 4> shuffled = cardinalDirs;
                std::shuffle(shuffled.begin(), shuffled.end(), rng);
                for (Direction dir : shuffled) {
                    auto [adx, ady] = delta(dir);
                    if (movement.move(*ally, adx, ady)) {
                        moved = true;
                        break;
                    }
                }
            }
        }
        else {
            // === иначе — двигаемся случайно ===
            std::array<Direction, 4> shuffled = cardinalDirs;
            std::shuffle(shuffled.begin(), shuffled.end(), rng);
            for (Direction dir : shuffled) {
                auto [adx, ady] = delta(dir);
                if (movement.move(*ally, adx, ady)) {
                    moved = true;
                    break;
                }
            }
        }
    }

    // удаляем мёртвых союзников
    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [](auto& a) { return !a->isAlive(); }),
        allies.end());
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
