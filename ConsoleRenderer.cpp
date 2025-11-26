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
                                                                                
 .d8888b. 88888888888 8888888888 8888888b.  888       888        d8888 8888888b.   .d8888b.  
d88P  Y88b    888     888        888   Y88b 888   o   888       d88888 888   Y88b d88P  Y88b 
Y88b.         888     888        888    888 888  d8b  888      d88P888 888    888 Y88b.      
 "Y888b.      888     8888888    888   d88P 888 d888b 888     d88P 888 888   d88P  "Y888b.   
    "Y88b.    888     888        8888888P"  888d88888b888    d88P  888 8888888P"      "Y88b. 
      "888    888     888        888        88888P Y88888   d88P   888 888 T88b         "888 
Y88b  d88P    888     888        888        8888P   Y8888  d8888888888 888  T88b  Y88b  d88P 
 "Y8888P"     888     8888888888 888        888P     Y888 d88P     888 888   T88b  "Y8888P"  
                                                                                             
                                                   
)" << '\n';
}


void ConsoleRenderer::printSeed(unsigned seed) const {
    std::cout << "Game Seed: " << seed << "\n";
}

void ConsoleRenderer::printWin() const {
    clearScreen();
    std::cout << R"(
                                                                                
Y88b   d88P  .d88888b.  888     888      888       888 8888888 888b    888 888 
 Y88b d88P  d88P" "Y88b 888     888      888   o   888   888   8888b   888 888 
  Y88o88P   888     888 888     888      888  d8b  888   888   88888b  888 888 
   Y888P    888     888 888     888      888 d888b 888   888   888Y88b 888 888 
    888     888     888 888     888      888d88888b888   888   888 Y88b888 888 
    888     888     888 888     888      88888P Y88888   888   888  Y88888 Y8P 
    888     Y88b. .d88P Y88b. .d88P      8888P   Y8888   888   888   Y8888  "  
    888      "Y88888P"   "Y88888P"       888P     Y888 8888888 888    Y888 888                                                                
)" << '\n';
}

void ConsoleRenderer::printGameOver() const {
    clearScreen();
    std::cout << R"(
                                                                                
Y88b   d88P  .d88888b.  888     888      888      .d88888b.   .d8888b.  8888888888 888 
 Y88b d88P  d88P" "Y88b 888     888      888     d88P" "Y88b d88P  Y88b 888        888 
  Y88o88P   888     888 888     888      888     888     888 Y88b.      888        888 
   Y888P    888     888 888     888      888     888     888  "Y888b.   8888888    888 
    888     888     888 888     888      888     888     888     "Y88b. 888        888 
    888     888     888 888     888      888     888     888       "888 888        Y8P 
    888     Y88b. .d88P Y88b. .d88P      888     Y88b. .d88P Y88b  d88P 888         "  
    888      "Y88888P"   "Y88888P"       88888888 "Y88888P"   "Y8888P"  8888888888 888                                                                                                                                                  
)" << '\n';
}
