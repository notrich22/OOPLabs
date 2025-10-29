#pragma once
#include "Board.h"
#include "Player.h"
#include <memory>
#include <string>

// ���������� �������� �� ����� ���� �� �����.
class PrintController {
public:
    void clearScreen() const;

    // ������ ������� ��������� ����
    void renderBoard(const Board& board, int turnCounter, unsigned seed) const;

    // ��������� ������
    void printTitle() const;
    void printSeed(unsigned seed) const;
    void printWin() const;
    void printGameOver() const;
	void printMessage(const std::string& message) const;
};
