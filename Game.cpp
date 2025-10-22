#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <random>
#include "Direction.h"

static std::pair<int, int> validateSize(int width, int height) {
    if (width < 10 || height < 10 || width > 25 || height > 25)
        throw std::invalid_argument("Board size must be between 10 and 25");
    return { width, height };
}

Game::Game(int w, int h, unsigned seed)
    : seed(seed),
    rng(seed),
    board(Board(validateSize(w, h).first, validateSize(w, h).second, rng)){}

void Game::init() {
    MAX_ENEMIES = (board.getHeight() * board.getWidth()) / 20;

    player = std::make_shared<Player>();
    spawnEntity(player);

    for (int i = 0; i < MAX_ENEMIES / 2; ++i) {
        auto enemy = std::make_shared<Enemy>(40, 10);
        enemies.push_back(enemy);
        spawnEntity(enemy);
    }

    auto spawner = std::make_shared<EnemySpawner>(100, 4);
    spawners.push_back(spawner);
    spawnEntity(spawner);

    isRunning = true;
    turnCounter = 1;
}

void Game::spawnEntity(const std::shared_ptr<Entity>& entity) {
    std::pair<int, int> pos;
    do {
        pos = board.getRandomFreeCell(rng);
    } while (!board.hasFreeNeighbor(pos));
    board.placeEntity(entity, pos.first, pos.second);
}

void Game::run() {
    while (isRunning && !checkGameOver()) {
        render();

        processPlayerTurn();
        processEnemies();
        processSpawners();

        //std::cout << "\nPress Enter to continue...";
		system("pause");

        turnCounter++;
    }
    std::cout << "Game over.\n";
}

std::shared_ptr<Entity> Game::findTargetInDirection(const std::shared_ptr<Entity>& source, Direction dir, int range) const
{
    auto [x, y] = source->getPosition();
    auto [dx, dy] = delta(dir);

    for (int step = 1; step <= range; ++step) {
        int nx = x + dx * step;
        int ny = y + dy * step;
        if (!board.isInside(nx, ny)) break;

        const Cell& cell = board.getCell(nx, ny);
        if (cell.getType() == CellType::Wall) break;
        if (cell.isOccupied()) return cell.getEntity();
    }

    return nullptr;
}

void Game::processPlayerTurn() {
    std::cout << "Player HP: " << player->getHealth() << "\n";

    if (player->getAttackMode() == AttackMode::Melee)
        std::cout << "Enter command (w/a/s/d to move, m to switch weapon, q to quit): ";
    else
        std::cout << "Enter command (w/a/s/d to move, m to switch weapon, f to shoot, q to quit): ";

    // Пропуск хода после замедления/события
    if (player->shouldSkipMove()) {
        player->setSkipNextMove(false);
        std::cout << "You skip this turn!\n";
        return;
    }

    char cmd;
    std::cin >> cmd;


    // ----- Смена оружия -----
    if (cmd == 'm') {
        player->switchMode();
        std::cout << "Attack mode switched to "
            << (player->getAttackMode() == AttackMode::Melee ? "MELEE\n" : "RANGED\n");
        player->setSkipNextMove(true);
        return;
    }

    // ----- Выход -----
    if (cmd == 'q') {
        isRunning = false;
        return;
    }

    // ----- Дальняя атака -----
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

        auto target = findTargetInDirection(player, *dirOpt, player->getAttackRange());

        if (target) {
            handleCombat(player, target);
        }
        else {
			std::cout << "You missed!\n";
        }
        return;
    }

    // ----- Движение -----
    if (auto dirOpt = keyToDir(cmd)) {
        auto [dx, dy] = delta(*dirOpt);
        moveEntity(*player, dx, dy);
    }
    else {
        std::cout << "Unknown command\n";
    }
}

void Game::processEnemies() {
    auto& dirs = cardinalDirs;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        if (enemy->shouldSkipMove()) {
            enemy->setSkipNextMove(false);
            std::cout << "Enemy skips this turn!\n";
            continue;
        }

        std::array<Direction, 4> shuffled = cardinalDirs;
        std::shuffle(shuffled.begin(), shuffled.end(), rng);

        for (Direction dir : shuffled) {
            auto [dx, dy] = delta(dir);
            if (moveEntity(*enemy, dx, dy)) {
                break;
            }
        }
    }
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::shared_ptr<Enemy>& e) { return !e->isAlive(); }),
        enemies.end()
    );
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

void Game::clearCmd() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Game::render() const {
    clearCmd();
    printTitle();
    printSeed();
    std::cout << "===== TURN " << turnCounter << " =====\n";
    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            const Cell& cell = board.getCell(x, y);
            char symbol = '.';

            switch (cell.getType()) {
            case CellType::Wall: symbol = '#'; break;
            case CellType::Slow: symbol = '~'; break;
            case CellType::Normal: symbol = '.'; break;
            }

            if (cell.isOccupied()) {
                symbol = cell.getEntity()->symbol();
            }
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

void Game::printSeed() const {
    std::cout << "Game Seed: " << seed << "\n";
}

void Game::printTitle() const{
    std::cout << R"(
                                                                                
   mmmm                                 mm      mm                              
 m#""""#     ##                         ##      ##                              
 ##m       #######    m####m   ##m###m  "#m ## m#"  m#####m   ##m####  mm#####m 
  "####m     ##      ##mmmm##  ##"  "##  ## ## ##   " mmm##   ##"      ##mmmm " 
      "##    ##      ##""""""  ##    ##  ###""###  m##"""##   ##        """"##m 
 #mmmmm#"    ##mmm   "##mmmm#  ###mm##"  ###  ###  ##mmm###   ##       #mmmmm## 
  """""       """"     """""   ## """    """  """   """" ""   ""        """"""  
                               ##                                               
                                                                                
)" << '\n';
}

bool Game::checkGameOver() const {
    if (!player->isAlive()) {
        std::cout << "You died!\n";
        return true;
    }

    bool anyEnemiesAlive = false;
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            anyEnemiesAlive = true;
            break;
        }
    }

    if (!anyEnemiesAlive) {
        printWin();
        return true;
    }

    return false;
}

void Game::printWin() const {
    clearCmd();
    std::cout << R"(
                                                                                
██    ██  ██████  ██    ██     ██     ██ ██ ███    ██ 
 ██  ██  ██    ██ ██    ██     ██     ██ ██ ████   ██ 
  ████   ██    ██ ██    ██     ██  █  ██ ██ ██ ██  ██ 
   ██    ██    ██ ██    ██     ██ ███ ██ ██ ██  ██ ██ 
   ██     ██████   ██████       ███ ███  ██ ██   ████ 
                                                      
                                                      
                                                                                
)" << '\n';
}

bool Game::moveEntity(MovableEntity& entity, int dx, int dy) {
    auto [x, y] = entity.getPosition();
    int newX = x + dx;
    int newY = y + dy;

    if (!board.isInside(newX, newY))
        return false;

    Cell& current = board.getCell(x, y);
    Cell& target = board.getCell(newX, newY);

    // Стена — нельзя пройти
    if (target.getType() == CellType::Wall)
        return false;

    // Если в клетке кто-то есть — бой
    if (target.isOccupied()) {
        auto attacker = current.getEntity();
        auto defender = target.getEntity();

        bool combatHappened = handleCombat(attacker, defender);

        if (!defender->isAlive()) {
            std::cout << "Target destroyed!\n";

            // Очистить клетку врага
            target.clearEntity();
        }

        return combatHappened;
    }

    // Если клетка пуста — обычное перемещение
    target.setEntity(current.getEntity());
    current.clearEntity();
    entity.setPosition(newX, newY);

    if (target.getType() == CellType::Slow)
        entity.setSkipNextMove(true);

    return true;
}

bool Game::handleCombat(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender) {

    if (!attacker || !defender)
        return false;
    auto enemyAttacker = std::dynamic_pointer_cast<Enemy>(attacker);
    auto enemyDefender = std::dynamic_pointer_cast<Enemy>(defender);
    auto spawnerDefender = std::dynamic_pointer_cast<EnemySpawner>(defender);
    if (enemyAttacker && (enemyDefender || spawnerDefender))
        return false;

    // атаковать могут только движущиеся сущности
    auto movable = std::dynamic_pointer_cast<MovableEntity>(attacker);
    if (!movable)
        return false;

    // ближняя атака
    if (movable->getAttackMode() == AttackMode::Melee) {
        movable->attack(*defender);
    }

    // дальняя атака (обрабатывается отдельно в Game)
    else if (movable->getAttackMode() == AttackMode::Ranged) {
        movable->attack(*defender); // можно просто использовать тот же метод
    }

    // после атаки проверяем смерть

    if (!defender->isAlive()) {
        auto [x, y] = defender->getPosition();
        board.getCell(x, y).clearEntity();
        if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
            player->addExperience(10);
        }
    }

    return true;
}
