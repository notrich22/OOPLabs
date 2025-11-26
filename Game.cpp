#include "Game.h"
#include "SaveManager.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>

// Валидация размеров поля
static std::pair<int, int> validateSize(int width, int height) {
    if (width < Config::MIN_BOARD_SIZE || height < Config::MIN_BOARD_SIZE ||
        width > Config::MAX_BOARD_SIZE || height > Config::MAX_BOARD_SIZE)
        throw std::invalid_argument("Board size must be between 10 and 25");
    return { width, height };
}

// Конструктор
Game::Game(int w, int h, unsigned seed)
    : seed(seed),
    rng(seed),
    board(Board(validateSize(w, h).first, validateSize(w, h).second, rng)),
    combat(board),
    movement(board, combat),
    towerController(board),
    currentLevel(1) {
}

// Инициализация уровня
void Game::init(std::shared_ptr<Player> existingPlayer) {
    try {
        board = Board(board.getWidth(), board.getHeight(), rng);

        enemies.clear();
        spawners.clear();
        towerController.clear();
        allies.clear();

        double hpMult = std::pow(Config::LEVEL_HP_MULTIPLIER, currentLevel - 1);
        int enemyHP = static_cast<int>(Config::ENEMY_DEFAULT_HEALTH * hpMult);
        MAX_ENEMIES = static_cast<int>(board.getHeight() * board.getWidth() * Config::ENEMY_DENSITY_FACTOR);

        if (existingPlayer) {
            player = existingPlayer;
        }
        else {
            player = std::make_shared<Player>();
            player->getSpellHand().addRandomSpell();
        }
        spawnEntity(player);

        std::uniform_int_distribution<int> distX(0, board.getWidth() - 1);
        std::uniform_int_distribution<int> distY(0, board.getHeight() - 1);

        for (int i = 0; i < MAX_ENEMIES; ++i) {
            auto enemy = std::make_shared<Enemy>(enemyHP, Config::ENEMY_MELEE_DAMAGE);
            for (int attempt = 0; attempt < 50; ++attempt) {
                int x = distX(rng);
                int y = distY(rng);
                Cell& cell = board.getCell(x, y);
                if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                    board.placeEntity(enemy, x, y);
                    enemies.push_back(enemy);
                    break;
                }
            }
        }

        int numSpawners = std::max(1, static_cast<int>(board.getWidth() * board.getHeight() * Config::SPAWNER_DENSITY_FACTOR));
        numSpawners += (currentLevel / 3);

        for (int i = 0; i < numSpawners; ++i) {
            auto spawner = std::make_shared<EnemySpawner>();
            for (int attempt = 0; attempt < 50; ++attempt) {
                int x = distX(rng);
                int y = distY(rng);
                Cell& cell = board.getCell(x, y);
                if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                    board.placeEntity(spawner, x, y);
                    spawners.push_back(spawner);
                    break;
                }
            }
        }

        int numTowers = std::max(1, static_cast<int>(board.getWidth() * board.getHeight() * Config::ENEMYTOWER_DENSITY_FACTOR));
        numTowers += (currentLevel / 2);

        for (int i = 0; i < numTowers; ++i) {
            int spellDmg = 5 + (currentLevel * 2);
            auto tower = std::make_shared<EnemyTower>(
                std::make_shared<Firebolt>(spellDmg, 3),
                3
            );
            for (int attempt = 0; attempt < 50; ++attempt) {
                int x = distX(rng);
                int y = distY(rng);
                Cell& cell = board.getCell(x, y);
                if (!cell.hasEntity() && cell.getType() == CellType::Normal) {
                    towerController.addTower(tower, x, y);
                    break;
                }
            }
        }

        isRunning = true;
        turnCounter = 1;
        notify("Level " + std::to_string(currentLevel) + " initialized.");
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Init failed: ") + e.what());
    }
}

// Переход на следующий уровень
void Game::nextLevel() {
    notify("=== LEVEL " + std::to_string(currentLevel) + " COMPLETE ===");

    processLevelUp(); // Меню

    currentLevel++;
    player->setHealth(Config::PLAYER_DEFAULT_HEALTH);
    notify("HP Restored to full.");

    player->getSpellHand().removeRandomHalf();
    notify("Half of spells lost in transition.");

    int newSize = board.getWidth() + Config::LEVEL_SIZE_INCREMENT;
    if (newSize > Config::MAX_LEVEL_SIZE) {
        newSize = Config::MAX_LEVEL_SIZE;
    }

    seed = std::random_device{}();
    rng.seed(seed);
    board = Board(newSize, newSize, rng);

    init(player);
}

// Размещение сущности
void Game::spawnEntity(const std::shared_ptr<Entity>& entity) {
    std::pair<int, int> pos;
    for (int attempts = 0; attempts < 1000; ++attempts) {
        pos = board.getRandomFreeCell(rng);
        if (board.hasFreeNeighbor(pos)) break;
    }
    board.placeEntity(entity, pos.first, pos.second);
}

// Обработка команд игрока с детальным логированием
bool Game::executeCommand(const Command& cmd) {
    if (!player->isAlive()) return false;

    // Снимок состояния для логирования
    int xpBefore = player->getExperience();

    if (player->shouldSkipMove()) {
        player->setSkipNextMove(false);
        notify("Player is slowed and skips turn!");
        return true;
    }

    bool turnUsed = false;

    switch (cmd.type) {
    case CommandType::Move:
        if (cmd.dir != Direction::None) {
            auto [dx, dy] = delta(cmd.dir);

            auto [oldX, oldY] = player->getPosition();
            int hpBefore = player->getHealth();

            // Предварительная проверка цели для ближнего боя
            int targetX = oldX + dx;
            int targetY = oldY + dy;
            std::shared_ptr<ICombatEntity> meleeTarget = nullptr;
            if (board.isInside(targetX, targetY)) {
                auto& cell = board.getCell(targetX, targetY);
                if (cell.isOccupied()) {
                    meleeTarget = std::dynamic_pointer_cast<ICombatEntity>(cell.getEntity());
                }
            }
            int targetHpBefore = (meleeTarget) ? meleeTarget->getHealth() : 0;

            // Выполнение движения
            if (movement.move(*player, dx, dy)) {
                turnUsed = true;
                auto [newX, newY] = player->getPosition();

                if (newX != oldX || newY != oldY) {
                    notify("Player moved to (" + std::to_string(newX) + ", " + std::to_string(newY) + ")");

                    int dmgTaken = hpBefore - player->getHealth();
                    if (dmgTaken > 0) {
                        notify(" > Stepped on TRAP! Took " + std::to_string(dmgTaken) + " damage.");
                    }
                }
                else if (meleeTarget) {
                    int dmgDealt = targetHpBefore - meleeTarget->getHealth();
                    notify("Player hit Enemy for " + std::to_string(dmgDealt) + " damage!");
                    if (!meleeTarget->isAlive()) {
                        notify(" > Enemy destroyed!");
                    }
                }
            }
            else {
                notify("Way blocked.");
            }
        }
        break;

    case CommandType::AttackMelee:
        player->setAttackMode(AttackMode::Melee);
        notify("Switched to Melee mode.");
        turnUsed = true;
        break;

    case CommandType::SwitchWeapon:
        player->switchMode();
        notify("Switched weapon mode.");
        player->setSkipNextMove(true);
        turnUsed = true;
        break;

    case CommandType::AttackRanged:
        if (player->getAttackMode() == AttackMode::Melee) {
            notify("Cannot shoot in melee mode!");
        }
        else if (cmd.dir != Direction::None) {
            auto target = combat.findTargetInDirection(player, cmd.dir, player->getAttackRange());
            auto combatTarget = std::dynamic_pointer_cast<ICombatEntity>(target);

            if (combatTarget) {
                int hpBefore = combatTarget->getHealth();
                combat.handleCombat(player, combatTarget);
                int dmg = hpBefore - combatTarget->getHealth();

                notify("Player shot target for " + std::to_string(dmg) + " damage!");
                if (!combatTarget->isAlive()) {
                    notify(" > Target eliminated!");
                }
            }
            else {
                notify("Player fired a shot but missed!");
            }
            turnUsed = true;
        }
        break;

    case CommandType::CastSpell: {
        auto& hand = player->getSpellHand();
        auto spell = hand.getSpell(cmd.spellIndex);

        if (spell) {
            std::shared_ptr<ICombatEntity> targetEntity = nullptr;
            // Определение цели для направленных заклинаний
            if (cmd.dir != Direction::None) {
                int maxRange = 3 + player->getEnhancementState().rangeBonus();
                auto t = combat.findTargetInDirection(player, cmd.dir, maxRange);
                targetEntity = std::dynamic_pointer_cast<ICombatEntity>(t);
            }

            CastContext ctx{
                *player, board, targetEntity,
                cmd.target,
                player->getEnhancementState()
            };

            if (spell->canCast(ctx)) {
                if (spell->cast(ctx)) {
                    // 1. Удаляем карту
                    hand.removeSpell(cmd.spellIndex);

                    // 2. Логируем
                    std::string sName = spell->name();
                    notify("Player cast " + sName + "!");
                    turnUsed = true;

                    if (sName == "SummonAlly") {
                        int addedCount = 0;
                        for (int y = 0; y < board.getHeight(); ++y) {
                            for (int x = 0; x < board.getWidth(); ++x) {
                                Cell& c = board.getCell(x, y);
                                if (c.hasEntity() && c.getEntity()->symbol() == 'A') {
                                    bool known = false;
                                    for (auto& existing : allies) {
                                        if (existing.get() == c.getEntity().get()) {
                                            known = true;
                                            break;
                                        }
                                    }
                                    if (!known) {
                                        if (auto newAlly = std::dynamic_pointer_cast<Ally>(c.getEntity())) {
                                            allies.push_back(newAlly);
                                            addedCount++;
                                        }
                                    }
                                }
                            }
                        }
                        if (addedCount > 0) {
                            notify("Synced " + std::to_string(addedCount) + " new Ally(ies) to squad.");
                        }
                        else {
                        }
                    }
                }
            }
            else {
                notify("Failed to cast " + spell->name());
            }
        }
        break;
    }

    case CommandType::SaveGame:
        try {
            SaveManager::saveGame(*this, "save.json");
            notify("Game saved successfully.");
        }
        catch (const std::exception& e) {
            notify(std::string("Save failed: ") + e.what());
        }
        break;

    case CommandType::LoadGame:
        try {
            SaveManager::loadGame(*this, "save.json");
            notify("Game loaded successfully.");
        }
        catch (const std::exception& e) {
            notify(std::string("Load failed: ") + e.what());
        }
        break;

    case CommandType::Quit:
        isRunning = false;
        break;
    }

    // Логирование полученного опыта
    int xpGained = player->getExperience() - xpBefore;
    if (xpGained > 0) {
        notify(" > Gained " + std::to_string(xpGained) + " XP! (Total: " + std::to_string(player->getExperience()) + ")");
        if (player->getExperience() % Config::XP_FOR_SPELL == 0) {
            std::string spellName = player->getSpellHand().addRandomSpell();
            if (!spellName.empty()) {
                notify("!!! NEW SPELL OBTAINED: " + spellName + " !!!");
            }
            else {
                notify("Inventory full. Spell discaded.");
            }
        }
    }
    return turnUsed;
}

// Обновление мира (AI)
void Game::updateWorld() {
    processAllies();
    processEnemies();
    processSpawners();
    processTowers();

    turnCounter++;

    if (player->isDead()) {
        notify("Player died.");
        isRunning = false;
    }
    else if (enemies.empty() && spawners.empty()) {
        if (currentLevel >= Config::MAX_LEVEL) {
            notify("YOU HAVE CONQUERED ALL LEVELS!");
            isRunning = false;
        }
        else {
            notify("Victory! Level cleared.");
            nextLevel();
        }
    }
}

// Логика союзников
void Game::processAllies() {
    if (allies.empty()) return;

    for (auto& ally : allies) {
        if (!ally || !ally->isAlive()) continue;

        if (ally->shouldSkipMove()) {
            ally->setSkipNextMove(false);
            auto [ax, ay] = ally->getPosition();
            notify("Ally at (" + std::to_string(ax) + "," + std::to_string(ay) + ") is slowed.");
            continue;
        }

        auto [oldX, oldY] = ally->getPosition();

        std::shared_ptr<Enemy> nearestEnemy = nullptr;
        int minDist = 10000;

        if (!enemies.empty()) {
            for (auto& enemy : enemies) {
                if (!enemy || !enemy->isAlive()) continue;
                auto [ex, ey] = enemy->getPosition();
                int dist = std::abs(oldX - ex) + std::abs(oldY - ey);
                if (dist < minDist) {
                    minDist = dist;
                    nearestEnemy = enemy;
                }
            }
        }

        bool moveSuccess = false;

        if (nearestEnemy && minDist <= 5) {
            auto [ex, ey] = nearestEnemy->getPosition();
            int dx = ex - oldX;
            int dy = ey - oldY;

            if (std::abs(dx) > std::abs(dy)) { dx = (dx > 0) ? 1 : -1; dy = 0; }
            else { dy = (dy > 0) ? 1 : -1; dx = 0; }

            moveSuccess = movement.move(*ally, dx, dy);

            if (!moveSuccess) {
                std::array<Direction, 4> shuf = cardinalDirs;
                std::shuffle(shuf.begin(), shuf.end(), rng);
                for (Direction dir : shuf) {
                    auto [adx, ady] = delta(dir);
                    if (movement.move(*ally, adx, ady)) {
                        moveSuccess = true;
                        break;
                    }
                }
            }
        }
        else {
            std::array<Direction, 4> shuf = cardinalDirs;
            std::shuffle(shuf.begin(), shuf.end(), rng);
            for (Direction dir : shuf) {
                auto [adx, ady] = delta(dir);
                if (movement.move(*ally, adx, ady)) {
                    moveSuccess = true;
                    break;
                }
            }
        }

        if (moveSuccess) {
            auto [newX, newY] = ally->getPosition();

            if (newX != oldX || newY != oldY) {
                notify("Ally moved from (" + std::to_string(oldX) + "," + std::to_string(oldY) +
                    ") to (" + std::to_string(newX) + "," + std::to_string(newY) + ")");
            }
            else {
                notify("Ally at (" + std::to_string(oldX) + "," + std::to_string(oldY) + ") attacked Enemy!");
            }
        }
    }
    allies.erase(std::remove_if(allies.begin(), allies.end(),
        [](auto& a) { return !a->isAlive(); }), allies.end());
}

void Game::processEnemies() {
    auto [px, py] = player->getPosition();

    for (auto& enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        if (enemy->shouldSkipMove()) {
            enemy->setSkipNextMove(false);
            auto [ex, ey] = enemy->getPosition();
            notify("Enemy at (" + std::to_string(ex) + "," + std::to_string(ey) + ") is slowed and skips turn.");
            continue;
        }

        auto [ex, ey] = enemy->getPosition();
        int dx = px - ex;
        int dy = py - ey;
        int dist = std::abs(dx) + std::abs(dy);

        auto [oldX, oldY] = enemy->getPosition();
        int playerHpBefore = player->getHealth();

        bool moved = false;
        if (dist <= 3) {
            if (std::abs(dx) > std::abs(dy)) dx = (dx > 0) ? 1 : -1, dy = 0;
            else dy = (dy > 0) ? 1 : -1, dx = 0;

            moved = movement.move(*enemy, dx, dy);
            if (!moved) {
                std::array<Direction, 4> shuf = cardinalDirs;
                std::shuffle(shuf.begin(), shuf.end(), rng);
                for (Direction dir : shuf) {
                    auto [adx, ady] = delta(dir);
                    if (movement.move(*enemy, adx, ady)) break;
                }
            }
        }
        else {
            std::array<Direction, 4> shuf = cardinalDirs;
            std::shuffle(shuf.begin(), shuf.end(), rng);
            for (Direction dir : shuf) {
                auto [adx, ady] = delta(dir);
                if (movement.move(*enemy, adx, ady)) break;
            }
        }


        int dmgDealt = playerHpBefore - player->getHealth();
        if (dmgDealt > 0) {
            notify("Enemy attacked Player for " + std::to_string(dmgDealt) + " damage!");
        }
        else {
            auto [newX, newY] = enemy->getPosition();
            if (newX != oldX || newY != oldY) {
                notify("Enemy moved from (" + std::to_string(oldX) + "," + std::to_string(oldY) +
                    ") to (" + std::to_string(newX) + "," + std::to_string(newY) + ")");
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::shared_ptr<Enemy>& e) { return !e->isAlive(); }), enemies.end());
}

// Логика спавнеров
void Game::processSpawners() {
    for (auto& spawner : spawners) {
        if (!spawner->isAlive()) continue;

        spawner->takeTurn();

        if (static_cast<int>(enemies.size()) >= MAX_ENEMIES) continue;
        if (!spawner->readyToSpawn()) continue;

        auto [sx, sy] = spawner->getPosition();
        std::array<Direction, 4> shuf = cardinalDirs;
        std::shuffle(shuf.begin(), shuf.end(), rng);

        bool spawned = false;

        for (Direction dir : shuf) {
            auto [dx, dy] = delta(dir);
            int nx = sx + dx;
            int ny = sy + dy;

            if (!board.isInside(nx, ny)) continue;
            Cell& cell = board.getCell(nx, ny);
            if (cell.isOccupied() || cell.getType() == CellType::Wall) continue;

            double hpMult = std::pow(Config::LEVEL_HP_MULTIPLIER, currentLevel - 1);
            int enemyHP = static_cast<int>(Config::ENEMY_DEFAULT_HEALTH * hpMult);

            auto newEnemy = std::make_shared<Enemy>(enemyHP, Config::ENEMY_MELEE_DAMAGE);
            enemies.push_back(newEnemy);
            board.placeEntity(newEnemy, nx, ny);

            spawner->resetCounter();
            spawned = true;

            notify("Spawner at (" + std::to_string(sx) + "," + std::to_string(sy) +
                ") created new Enemy at (" + std::to_string(nx) + "," + std::to_string(ny) + ")!");
            break;
        }

        if (!spawned && spawner->readyToSpawn()) {
            notify("Spawner at (" + std::to_string(sx) + "," + std::to_string(sy) + ") is blocked!");
        }
    }

    spawners.erase(std::remove_if(spawners.begin(), spawners.end(),
        [](const std::shared_ptr<EnemySpawner>& s) { return !s->isAlive(); }),
        spawners.end());
}

// Логика башен
void Game::processTowers() {
    if (towerController.getTowers().empty()) return;

    int hpBefore = player->getHealth();

    towerController.update();

    int dmgTaken = hpBefore - player->getHealth();

    if (dmgTaken > 0) {
        notify("Towers attacked Player! Taken " + std::to_string(dmgTaken) + " damage.");
    }
}

// Проверка конца игры
bool Game::checkGameOver() const {
    if (!player->isAlive()) return true;
    return false;
}

// Меню прокачки
void Game::processLevelUp() {
    std::cout << "\n=== LEVEL COMPLETED! ===\n";
    std::cout << "Choose your reward:\n";
    std::cout << "1. Increase Player Melee Damage (+5)\n";
    std::cout << "2. Increase Player Ranged Damage (+3)\n";

    bool canUpgradeSpell = !player->getSpellHand().empty();
    if (canUpgradeSpell) {
        std::cout << "3. Upgrade an existing Spell card\n";
    }
    else {
        std::cout << "3. (Unavailable - No spells in hand)\n";
    }

    std::cout << "Enter choice: ";
    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Try again: ";
    }

    if (choice == 1) {
        int newDmg = player->getMeleeAttackPower() + 5;
        player->setAttackPowers(newDmg, player->getRangedAttackPower());
        std::cout << "Melee damage increased to " << newDmg << "!\n";
        notify("Level Up Reward: Melee Damage increased (+5).");
    }
    else if (choice == 2) {
        int newDmg = player->getRangedAttackPower() + 3;
        player->setAttackPowers(player->getMeleeAttackPower(), newDmg);
        std::cout << "Ranged damage increased to " << newDmg << "!\n";
        notify("Level Up Reward: Ranged Damage increased (+3).");
    }
    else if (choice == 3) {
        if (!canUpgradeSpell) {
            std::cout << "No spells to upgrade! You gain +10 HP instead.\n";
            player->setHealth(player->getHealth() + 10);
        }
        else {
            auto& hand = player->getSpellHand();
            auto spells = hand.getSpellNames();

            std::cout << "\nSelect spell to upgrade:\n";
            for (size_t i = 0; i < spells.size(); ++i) {
                auto spell = hand.getSpell(i);
                std::cout << (i + 1) << ". " << spells[i]
                    << " -> " << spell->getUpgradeInfo() << "\n";
            }

            int spellIndex;
            std::cout << "Number: ";
            while (!(std::cin >> spellIndex) || spellIndex < 1 || spellIndex > static_cast<int>(spells.size())) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid number. Try again: ";
            }

            auto selectedSpell = hand.getSpell(spellIndex - 1);
            selectedSpell->upgrade();
            std::cout << selectedSpell->name() << " has been upgraded!\n";
            notify("Level Up Reward: Spell upgraded/added.");
        }
    }

    std::cout << "Press Enter to continue to next level...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}