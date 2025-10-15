#pragma once
#include <vector>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"

// ����� Game ��������� �������� ������� ������ � ��������������� ��������
class Game {
private:
    Board board;                                      // ������� ����
    std::shared_ptr<Player> player;                   // �����
    std::vector<std::shared_ptr<Enemy>> enemies;      // ������ ������
    std::vector<std::shared_ptr<EnemySpawner>> spawners; // �������� ������

    int turnCounter = 1;   // ������� �����
    bool isRunning = false; // ���� ��������� ����
    int MAX_ENEMIES = 0;    // ������������ ����� ������ �� ����

public:
    // ����������� �������������� ���� � �������� �������� ����
    Game(int width, int height);

    // ������������� ��������
    void init();

    // ������� ������� ����
    void run();

    // ��� ������
    void processPlayerTurn();

    // ��������� �������� ������
    void processEnemies();

    // ������ ��������� (�������� ����� ������)
    void processSpawners();

    // ������� �������
    void clearCmd() const;

    // ����� ��������� ����
    void printTitle() const;

    // �������� ���������� ����
    bool checkGameOver() const;

    // ����� ������ ������
    void printWin() const;

    // ��������� �������� ���������
    void render() const;
};
