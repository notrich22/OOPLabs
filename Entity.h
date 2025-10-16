#pragma once
#include <utility>
#include <memory>

// Режим атаки
enum class AttackMode { Melee, Ranged };

// Базовый класс Entity - сущность с общими характеристиками
class Entity {
protected:
    int health;             // здоровье
    int meleeAttackPower;   // сила ближней атаки
    int rangedAttackPower;  // сила дальней атаки
	AttackMode attackMode = AttackMode::Melee; // текущий режим атаки
	std::pair<int, int> position; // позиция на игровом поле (x, y)
public:
    // Конструктор с проверкой значений
    Entity(int health, int meleeAttackPower, int rangedAttackPower)
        : health(health > 0 ? health : 0),
        meleeAttackPower(meleeAttackPower > 0 ? meleeAttackPower : 0),
        rangedAttackPower(rangedAttackPower > 0 ? rangedAttackPower : 0) {
    }

    Entity(const Entity&) = default;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) noexcept = default;
    virtual ~Entity() = default;

    // Клонирование объекта (для глубоких копий)
    virtual std::shared_ptr<Entity> clone() const = 0;

	// Абстрактные методы поведения сущности
    virtual void takeTurn() = 0;
    virtual void takeDamage(int dmg) = 0;

    // Геттеры
    int getHealth() const { return health; }
    int getMeleeAttackPower() const { return meleeAttackPower; }
    int getRangedAttackPower() const { return rangedAttackPower; }
    std::pair<int, int> getPosition() const noexcept {return position;}
    int getX() const noexcept { return position.first; }
    int getY() const noexcept { return position.second; }
	AttackMode getAttackMode() const { return attackMode; }

    // Сеттеры
    void setHealth(int h) { health = (h > 0 ? h : 0); }
    void setMeleeAttackPower(int power) { meleeAttackPower = (power > 0 ? power : 0); }
    void setRangedAttackPower(int power) { rangedAttackPower = (power > 0 ? power : 0); }
	void setPosition(int x, int y) { position = { x, y }; }
	void setAttackMode(AttackMode mode) { attackMode = mode; }

    // Проверка состояния
    bool isAlive() const { return health > 0; }
};
