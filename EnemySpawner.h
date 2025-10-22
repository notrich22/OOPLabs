#pragma once
#include "StaticEntity.h"
#include "Enemy.h"
#include <memory>

// ����� EnemySpawner ������ ������ ����� ����������� ��������� �������
class EnemySpawner : public StaticEntity {
private:
    int spawnCooldown;    // ������ ����� ��������
    int currentCounter;   // ������� �� ���������� ������

public:
    // ����������� � ����������� �������� � ������� �����������
    EnemySpawner(int health, int cooldown)
        : StaticEntity(health),
        spawnCooldown(cooldown),
        currentCounter(cooldown) {
    }

    EnemySpawner(const EnemySpawner&) noexcept = default;

    // ��������� ��������
    void takeTurn() override;            // ��� �������� (���������� ��������, �������� �����)
    void takeDamage(int dmg) override;   // ��������� �����
    std::shared_ptr<Entity> clone() const override; // ����������� �������
    char symbol() const noexcept override { return 'S'; }

    // �������� ���������� � ����� �������
    bool readyToSpawn() const { return currentCounter <= 0; }
    void resetCounter() { currentCounter = spawnCooldown; }
};
