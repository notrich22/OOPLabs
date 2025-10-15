#pragma once
#include "StaticEntity.h"
#include "Enemy.h"
#include <memory>

// Класс EnemySpawner создаёт врагов через определённые интервалы времени
class EnemySpawner : public StaticEntity {
private:
    int spawnCooldown;    // период между спавнами
    int currentCounter;   // счётчик до следующего спавна

public:
    // Конструктор с параметрами здоровья и времени перезарядки
    EnemySpawner(int health, int cooldown, int meleeAttackPower = 0, int rangedAttackPower = 0)
        : StaticEntity(health, meleeAttackPower, rangedAttackPower),
        spawnCooldown(cooldown),
        currentCounter(cooldown) {
    }

    EnemySpawner(const EnemySpawner&) noexcept = default;

    // Поведение спавнера
    void takeTurn() override;            // ход спавнера (уменьшение счётчика, создание врага)
    void takeDamage(int dmg) override;   // получение урона
    std::shared_ptr<Entity> clone() const override; // копирование объекта

    // Проверка готовности и сброс таймера
    bool readyToSpawn() const { return currentCounter <= 0; }
    void resetCounter() { currentCounter = spawnCooldown; }
};
