#pragma once
#include "StaticEntity.h"
#include <memory>
#include "Config.h"
#include "ICombatEntity.h"
class EnemySpawner : public StaticEntity, public ICombatEntity {
private:
    int spawnCooldown;
    int currentCounter;

public:
    // ����������� � ����������� �������� � ������� �����������
    EnemySpawner(int health = Config::SPAWNER_HEALTH, int cooldown = Config::SPAWN_COOLDOWN)
        : StaticEntity(),
        ICombatEntity(),
        spawnCooldown(cooldown),
        currentCounter(cooldown)
    {
        health_ = health;
        meleeAttackPower_ = 0;
        rangedAttackPower_ = 0;
    }


    EnemySpawner(const EnemySpawner&) noexcept = default;

    // ��������� ��������
    void takeTurn() override;            // ��� �������� (���������� ��������, �������� �����)
    void takeDamage(int dmg) override;   // ��������� �����
    std::shared_ptr<Entity> clone() const override; // ����������� �������
	char symbol() const noexcept override { return 'S'; } // ������ �������� �� ����
	std::pair<int, int> getPosition() const override { return Entity::getPosition(); }
	Faction faction() const override { return Faction::Enemy; } // ������� ��������
    bool readyToSpawn() const noexcept { return currentCounter <= 0; } // �������� ���������� 
    void resetCounter() noexcept { currentCounter = spawnCooldown; } // C���� �������

};
