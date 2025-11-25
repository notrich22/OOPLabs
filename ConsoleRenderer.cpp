#include "ConsoleRenderer.h"
#include <iostream>

#ifdef _WIN32
#include <cstdlib>
#endif

void ConsoleRenderer::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderer::renderBoard(const Board& board, const Player& player, int turnCounter, unsigned seed) const {
    clearScreen();
    printTitle();
    std::cout << "-------------------------------------------------\n";
    std::cout << "CONTROLS: [W/A/S/D] Move  [F] Shoot  [M] Switch Weapon\n";
    std::cout << "          [C] Spell     [K] Save   [O] Load   [Q] Quit\n";
    std::cout << "-------------------------------------------------\n";
    printSeed(seed);
    std::cout << "===== TURN " << turnCounter << " =====\n\n";

    std::cout << "   ";
    for (int x = 0; x < board.getWidth(); ++x)
        std::cout << x % 10 << ' ';
    std::cout << "\n";

    for (int y = 0; y < board.getHeight(); ++y) {
        std::cout << (y % 10) << "  ";

        for (int x = 0; x < board.getWidth(); ++x) {
            const Cell& cell = board.getCell(x, y);
            char symbol = '.';

            switch (cell.getType()) {
            case CellType::Wall: symbol = '#'; break;
            case CellType::Slow: symbol = '~'; break;
            case CellType::Normal: symbol = '.'; break;
            }

            if (cell.hasEntity())
                symbol = cell.getEntity()->symbol();

            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\nPLAYER STATUS:\n";
    std::cout << "HP: " << player.getHealth() << " | Mana: " << player.getMana() << "\n";
    std::cout << "Mode: " << (player.getAttackMode() == AttackMode::Melee ? "MELEE" : "RANGED") << "\n";
    std::cout << "Damage: " << player.getMeleeAttackPower() << " (Melee) / "
        << player.getRangedAttackPower() << " (Ranged)\n";
    std::cout << "\n--- EVENT LOG ---\n";
    if (messageBuffer.empty()) {
        std::cout << "(no events)\n";
    }
    else {
        for (const auto& msg : messageBuffer) {
            std::cout << " > " << msg << "\n";
        }
    }
    std::cout << "-----------------\n";
}

void ConsoleRenderer::printTitle() const {
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


void ConsoleRenderer::printSeed(unsigned seed) const {
    std::cout << "Game Seed: " << seed << "\n";
}

void ConsoleRenderer::printWin() const {
    clearScreen();
    std::cout << R"(
                                                                                
██    ██  ██████  ██    ██     ██     ██ ██ ███    ██ 
 ██  ██  ██    ██ ██    ██     ██     ██ ██ ████   ██ 
  ████   ██    ██ ██    ██     ██  █  ██ ██ ██ ██  ██ 
   ██    ██    ██ ██    ██     ██ ███ ██ ██ ██  ██ ██ 
   ██     ██████   ██████       ███ ███  ██ ██   ████ 
                                                      
                                                      
                                                                                
)" << '\n';
}

void ConsoleRenderer::printGameOver() const {
    clearScreen();
    std::cout << "You died! Game over.\n";
}
