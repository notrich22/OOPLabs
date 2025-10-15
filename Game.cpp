#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <stdexcept>

static std::pair<int, int> validateSize(int width, int height) {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Размеры поля должны быть положительными");
    if (width > 100 || height > 100)
        throw std::invalid_argument("Размеры поля слишком большие");
    return { width, height };
}

Game::Game(int width, int height)
    : board(validateSize(width, height).first, validateSize(width, height).second)
{
}

void Game::init() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    player = std::make_shared<Player>(100, 20);
    board.placeEntity(player, 1, 1);

    for (int i = 0; i < 3; ++i) {
        auto enemy = std::make_shared<Enemy>(40, 10);
        enemies.push_back(enemy);
        int x = 5 + i;
        int y = 5;
        board.placeEntity(enemy, x, y);
    }

    auto spawner = std::make_shared<EnemySpawner>(100, 0, 3);
    spawners.push_back(spawner);
    board.placeEntity(spawner, 8, 8);

    isRunning = true;
}

void Game::run() {
    while (isRunning && !checkGameOver()) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
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
        int dx = 0, dy = 0;
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

    board.moveEntity(*player, dx, dy);
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

        auto [dx, dy] = dirs[std::rand() % 4];
        board.moveEntity(*enemy, dx, dy);

    }
}

void Game::processSpawners() {

    for (auto& spawner : spawners) {
        if (!spawner->isAlive())
            continue;
        spawner->takeTurn();
        if (static_cast<int>(enemies.size()) >= MAX_ENEMIES) {
            spawner->resetCounter();
            return;
        }
        if (spawner->readyToSpawn()) {
            // ищем координаты спавнера
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

            // ищем свободное соседнее место
            const int dirs[8][2] = {
                {-1, -1}, {0, -1}, {1, -1},
                {-1, 0},          {1, 0},
                {-1, 1},  {0, 1}, {1, 1}
            };

            for (auto [dx, dy] : dirs) {
                int nx = sx + dx;
                int ny = sy + dy;
                if (board.isInside(nx, ny) && !board.getCell(nx, ny).isOccupied() && board.getCell(nx,ny).getType() != CellType::Wall) {
                    auto newEnemy = std::make_shared<Enemy>(40, 10);
                    enemies.push_back(newEnemy);
                    board.placeEntity(newEnemy, nx, ny);
                    std::cout << "Spawner created enemy at (" << nx << ", " << ny << ")\n";
                    spawner->resetCounter();
                    goto NEXT; // выходим из обоих циклов
                }
            }
        NEXT:;
        }
    }
}

void Game::render() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
        std::cout << "All enemies defeated! You win!\n";
        return true;
    }

    return false;
}
