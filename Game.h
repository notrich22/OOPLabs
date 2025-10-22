#pragma once
#include <vector>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include <random>


// ����� Game ��������� �������� ������� ������ � ��������������� ��������
class Game {
private:
    std::mt19937 rng; // ��������� ��������������� ����� � ����������� seed
    unsigned seed = 0;
    Board board;                                      // ������� ����
    std::shared_ptr<Player> player;                   // �����
    std::vector<std::shared_ptr<Enemy>> enemies;      // ������ ������
    std::vector<std::shared_ptr<EnemySpawner>> spawners; // �������� ������

    int turnCounter = 1;   // ������� �����
    bool isRunning = false; // ���� ��������� ����
    int MAX_ENEMIES = 0;    // ������������ ����� ������ �� ����

	//������ �������� � �������� �����������
    std::shared_ptr<Entity> findTargetInDirection(const std::shared_ptr<Entity>& source, Direction dir, int range) const;

    //������ ��� ����� ����� ����������
    bool handleCombat(std::shared_ptr<Entity> attacker, std::shared_ptr<Entity> defender);

    // ��� ������
    void processPlayerTurn();

    // ��������� �������� ������
    void processEnemies();

    // ������ ��������� (�������� ����� ������)
    void processSpawners();

    //������ ����������� ��������
    bool moveEntity(MovableEntity& entity, int dx, int dy);

public:
    // ����������� �������������� ���� � �������� �������� ����
    Game(int width, int height, unsigned seed = std::random_device{}());

    // ������������� ��������
    void init();

	// ����� �������� �� ��������� � ���������� ������
    void spawnEntity(const std::shared_ptr<Entity>& entity);

    // ������� ������� ����
    void run();

    // ������� �������
    void clearCmd() const;

    // ����� ��������� ����
    void printTitle() const;

	//����� ���� ����
	void printSeed() const;

    // �������� ���������� ����
    bool checkGameOver() const;

    // ����� ������ ������
    void printWin() const;

    // ��������� �������� ���������
    void render() const;
};
