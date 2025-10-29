#include "PrintController.h"
#include <iostream>

#ifdef _WIN32
#include <cstdlib>
#endif

void PrintController::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void PrintController::renderBoard(const Board& board, int turnCounter, unsigned seed) const {
    clearScreen();
    printTitle();
    printSeed(seed);
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

            if (cell.isOccupied())
                symbol = cell.getEntity()->symbol();

            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

void PrintController::printTitle() const {
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


void PrintController::printSeed(unsigned seed) const {
    std::cout << "Game Seed: " << seed << "\n";
}

void PrintController::printWin() const {
    clearScreen();
    std::cout << R"(
                                                                                
██    ██  ██████  ██    ██     ██     ██ ██ ███    ██ 
 ██  ██  ██    ██ ██    ██     ██     ██ ██ ████   ██ 
  ████   ██    ██ ██    ██     ██  █  ██ ██ ██ ██  ██ 
   ██    ██    ██ ██    ██     ██ ███ ██ ██ ██  ██ ██ 
   ██     ██████   ██████       ███ ███  ██ ██   ████ 
                                                      
                                                      
                                                                                
)" << '\n';
}

void PrintController::printGameOver() const {
    clearScreen();
    std::cout << "You died! Game over.\n";
}

void PrintController::printMessage(const std::string& msg) const {
    std::cout << msg << "\n";
}
