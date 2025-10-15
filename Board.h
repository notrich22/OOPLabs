#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Cell.h"
#include "MovableEntity.h"
#include "Player.h"

// ����� Board ������������ ������� ���� � ��������� ��������� �� ��
class Board {
private:
    int width;   // ������ ����
    int height;  // ������ ����
    std::vector<std::vector<Cell>> grid;  // ����� ������

public:
    // ������������
    Board(int width, int height);
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

    // ������ � ���������
    void placeEntity(std::shared_ptr<Entity> entity, int x, int y);  // ���������� ��������
    bool moveEntity(MovableEntity& entity, int dx, int dy);          // ����������� ��������
    bool rangedAttack(Player& player, int dx, int dy, int range);    // ������� ����� ������
};
