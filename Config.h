#pragma once

// ����� ��������� ����
namespace Config {

    // ----- ������ ���� -----
    constexpr int MIN_BOARD_SIZE = 10;    // ����������� ������ ����
    constexpr int MAX_BOARD_SIZE = 25;    // ������������ ������ ����

    // ----- ����� -----
    constexpr int PLAYER_DEFAULT_HEALTH = 100;
    constexpr int PLAYER_MELEE_DAMAGE = 20;
    constexpr int PLAYER_RANGED_DAMAGE = 10;
    constexpr int PLAYER_ATTACK_RANGE = 2;
    constexpr int PLAYER_SPELLS_COUNT = 5;
    // ----- ����� -----
    constexpr int ENEMY_DEFAULT_HEALTH = 40;
    constexpr int ENEMY_MELEE_DAMAGE = 10;

    // ----- ������� -----
    constexpr int SPAWNER_HEALTH = 100;
    constexpr int SPAWN_COOLDOWN = 4;   // ����� ������� ����� ���������� ����
    // ----- ����� -----
    constexpr int TOWER_DEFAULT_HEALTH = 50;
    constexpr int TOWER_DEFAULT_COOLDOWN = 3;

    // ----- ��������� ������ -----
    constexpr double ENEMY_DENSITY_FACTOR = 0.05; // ��������� ������ = 5% �� ������� ����
    constexpr double ENEMYTOWER_DENSITY_FACTOR = 0.01; // ��������� ����� = 1% �� ������� ����
    constexpr double SPAWNER_DENSITY_FACTOR = 0.02; // ��������� ������ = 2% �� ������� ����

    // ----- ������� ������ -----
    constexpr double WALL_PROBABILITY = 0.1;  // ����������� ����� ��� ���������
    constexpr double SLOW_PROBABILITY = 0.1;  // ����������� ����������� ������

    // ----- ������ -----
    constexpr int DAMAGE_EXPERIENCE_REWARD = 10;  // ���� �� �������� �����
}
