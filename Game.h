#pragma once
#include <vector>
#include <memory>
#include <random>

#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"

#include "CombatController.h"
#include "MovementController.h"
#include "PrintController.h"
#include "TowerController.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include "Direction.h"
#include "Firebolt.h"
#include "Explosion.h"
#include "TrapSpell.h"
#include "SummonAlly.h"
#include "EnhanceSpell.h"
#include "Ally.h"

// ����� Game ��������� ����� ������ ����
class Game {
private:
    // ������� ���������� ����
    std::mt19937 rng;      // ��������� ��������� �����
    unsigned seed = 0;
    Board board;
	EnhancementState enhancement;
    // �������� ��������
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<EnemySpawner>> spawners;
	std::vector<std::shared_ptr<Ally>> allies;
    // �����������
    CombatController combat;
    MovementController movement;
    PrintController printer;
    TowerController towerController;
    // ��������� ����
    int turnCounter = 1;
    bool isRunning = false;
    int MAX_ENEMIES = 0;

    // ���� � ���� ����
    void processPlayerTurn();
    void processAllies();
    void processEnemies();
    void processSpawners();
	void processTowers();
    // ��������������� ������
    bool checkGameOver() const;

    // ���������� ��������� �� �����
    void spawnEntity(const std::shared_ptr<Entity>& entity);

public:
    // �����������
    Game(int width, int height, unsigned seed = std::random_device{}());

    // ������������� ��������
    void init();

    // ������� ������� ����
    void run();
};
