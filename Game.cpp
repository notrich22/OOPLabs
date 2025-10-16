#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <random>

static std::pair<int, int> validateSize(int width, int height) {
    if (width < 10 || height < 10 || width > 25 || height > 25)
        throw std::invalid_argument("Board size must be between 10 and 25");
    return { width, height };
}

Game::Game(int width, int height)
    : board(validateSize(width, height).first, validateSize(width, height).second)
{
}

void Game::init() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    MAX_ENEMIES = (board.getHeight() * board.getWidth()) / 20;
    player = std::make_shared<Player>();
    auto freePos = board.getRandomFreeCell();
    int x = freePos.first;
    int y = freePos.second;
    board.placeEntity(player, x, y);


    for (int i = 0; i < MAX_ENEMIES/2; ++i) {
        auto enemy = std::make_shared<Enemy>(40, 10);
        enemies.push_back(enemy);
        freePos = board.getRandomFreeCell();
        x = freePos.first;
        y = freePos.second;
        board.placeEntity(enemy, x, y);
    }


    auto spawner = std::make_shared<EnemySpawner>(100, 4);
    spawners.push_back(spawner);
    freePos = board.getRandomFreeCell();
    x = freePos.first;
    y = freePos.second;    
    board.placeEntity(spawner, x, y);

    isRunning = true;
}

void Game::run() {
    while (isRunning && !checkGameOver()) {
        render();

        processPlayerTurn();
        processEnemies();
        processSpawners();

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        turnCounter++;
    }
    std::cout << "Game over.\n";
}

void Game::processPlayerTurn() {
    std::cout << "Player HP: " << player->getHealth() << "\n";
    player->getAttackMode() == AttackMode::Melee?std::cout << "Enter command (w/a/s/d to move, m to switch weapon, q to quit): ": std::cout << "Enter command (w/a/s/d to move, m to switch weapon, f to shoot, q to quit): ";
    
    if (player->shouldSkipMove()) {
		player->setSkipNextMove(false);
		std::cout << "You skip this turn!\n";
		return;
    }
    char cmd;
    std::cin >> cmd;
    int attack_range = player->getAttackRange();

    int dx = 0, dy = 0;
    switch (cmd) {
    case 'w': dy = -1; break;
    case 's': dy = 1;  break;
    case 'a': dx = -1; break;
    case 'd': dx = 1;  break;
    case 'f': {
        if (player->getAttackMode() == AttackMode::Melee)break;
        std::cout << "Attack direction (w/a/s/d): ";
        char dir;
        std::cin >> dir;
        switch (dir) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        default:
            std::cout << "Invalid direction\n";
            return;
        }

        // вызываем новую функцию для поиска цели
        board.rangedAttack(*player, dx, dy, attack_range);
        return;
    }
    case 'm':
        player->switchMode();
        std::cout << "Attack mode switched to "
            << (player->getAttackMode() == AttackMode::Melee ? "MELEE\n" : "RANGED\n");
        player->setSkipNextMove(true); // пропускаем ход после смены режима
        return;
    case 'q':
        isRunning = false;
        return;
    default:
        std::cout << "Unknown command\n";
        return;
    }

    moveEntity(*player, dx, dy);
}

void Game::processEnemies() {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        if (enemy->shouldSkipMove()) {
            enemy->setSkipNextMove(false);
            std::cout << "Enemy skips this turn!\n";
            continue;
        }

        const int dirs[4][2] = {
            {0, -1},  // вверх
            {0, 1},   // вниз
            {-1, 0},  // влево
            {1, 0}    // вправо
        };

        std::vector<int> indices = { 0, 1, 2, 3 };
        std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));

        for (int i : indices) {
            int dx = dirs[i][0];
            int dy = dirs[i][1];
            if (moveEntity(*enemy, dx, dy)) {
                break;
            }
        }
    }

    // Удаляем всех мёртвых врагов из списка
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

        // Проверяем лимит по текущему количеству врагов
        if (static_cast<int>(enemies.size()) >= MAX_ENEMIES) {
            continue;
        }

        if (spawner->readyToSpawn()) {
            int sx = -1, sy = -1;
            for (int y = 0; y < board.getHeight(); ++y) {
                for (int x = 0; x < board.getWidth(); ++x) {
                    if (board.getCell(x, y).getEntity().get() == spawner.get()) {
                        sx = x; sy = y;
                        break;
                    }
                }
                if (sx != -1) break;
            }

            const int dirs[8][2] = {
                {-1, -1}, {0, -1}, {1, -1},
                {-1, 0},          {1, 0},
                {-1, 1},  {0, 1}, {1, 1}
            };

            for (auto [dx, dy] : dirs) {
                int nx = sx + dx;
                int ny = sy + dy;
                if (board.isInside(nx, ny) &&
                    !board.getCell(nx, ny).isOccupied() &&
                    board.getCell(nx, ny).getType() != CellType::Wall)
                {
                    auto newEnemy = std::make_shared<Enemy>(40, 10);
                    enemies.push_back(newEnemy);
                    board.placeEntity(newEnemy, nx, ny);
                    std::cout << "Spawner created enemy at (" << nx << ", " << ny << ")\n";
                    spawner->resetCounter();
                    break;
                }
            }
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
                auto entity = cell.getEntity();
                if (std::dynamic_pointer_cast<Player>(entity))
                    symbol = 'P';
                else if (std::dynamic_pointer_cast<Enemy>(entity))
                    symbol = 'E';
                else if (std::dynamic_pointer_cast<EnemySpawner>(entity))
                    symbol = 'S';
            }

            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
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

            // Переместить атакующего на её место
            target.setEntity(attacker);
            current.clearEntity();
            entity.setPosition(newX, newY);
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
    if (!attacker || !defender) return false;
    if (attacker->getAttackMode() == AttackMode::Melee) {
        defender->takeDamage(attacker->getMeleeAttackPower());
        std::cout << "Player hits enemy for " << attacker->getMeleeAttackPower() << " damage!\n";
        if (!defender->isAlive() && (dynamic_pointer_cast<Enemy>(defender) || dynamic_pointer_cast<EnemySpawner>(defender))) {
            if (auto player = std::dynamic_pointer_cast<Player>(attacker)) {
                player->addExperience(10);
                std::cout << "Player gained 10 experience points!\n";
            }
        }
        return true;
    }
    return false;
}
