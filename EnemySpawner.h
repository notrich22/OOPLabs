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
    // Конструктор с параметрами здоровья и времени перезарядки
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

    // Поведение спавнера
    void takeTurn() override;            // ход спавнера (уменьшение счётчика, создание врага)
    void takeDamage(int dmg) override;   // получение урона
    std::shared_ptr<Entity> clone() const override; // копирование объекта
	char symbol() const noexcept override { return 'S'; } // символ спавнера на поле
	std::pair<int, int> getPosition() const override { return Entity::getPosition(); }
	Faction faction() const override { return Faction::Enemy; } // фракция спавнера
    bool readyToSpawn() const noexcept { return currentCounter <= 0; } // Проверка готовности 
    void resetCounter() noexcept { currentCounter = spawnCooldown; } // Cброс таймера

};
