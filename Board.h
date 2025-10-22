#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Cell.h"
#include "MovableEntity.h"
#include "Player.h"
#include <random>

// ����� Board ������������ ������� ���� � ��������� ��������� �� ��
class Board {
private:
    int width;   // ������ ����
    int height;  // ������ ����
    std::vector<std::vector<Cell>> grid;  // ����� ������

public:
    // ������������
    Board(int width, int height, std::mt19937& rng);
    Board(const Board& other);            // ����������� (��������)
    Board(Board&& other) noexcept;        // �����������
    Board& operator=(const Board& other);
    Board& operator=(Board&& other) noexcept;
    ~Board() = default;

    // �������
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // ������ � ��������
    bool isInside(int x, int y) const;    // ��������, ������ �� ����������
    Cell& getCell(int x, int y);          // �������� ������
    const Cell& getCell(int x, int y) const;
    std::pair<int, int> getRandomFreeCell(std::mt19937& rng) const;
	bool hasFreeNeighbor(const std::pair<int, int>& pos) const; // �������� ������������ ������

    // ������ � ���������
    void placeEntity(std::shared_ptr<Entity> entity, int x, int y);  // ���������� ��������
};
