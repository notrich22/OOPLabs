#pragma once
#include "Board.h"
#include "Player.h"
#include <memory>
#include <string>

// Контроллер отвечает за вывод игры на экран.
class PrintController {
public:
    void clearScreen() const;

    // Рисует текущее состояние поля
    void renderBoard(const Board& board, int turnCounter, unsigned seed) const;

    // Отдельные экраны
    void printTitle() const;
    void printSeed(unsigned seed) const;
    void printWin() const;
    void printGameOver() const;
	void printMessage(const std::string& message) const;
};
