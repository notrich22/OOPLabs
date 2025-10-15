#pragma once
#include <utility>
#include <memory>

// ������� ����� Entity - �������� � ������ ����������������
class Entity {
protected:
    int health;             // ��������
    int meleeAttackPower;   // ���� ������� �����
    int rangedAttackPower;  // ���� ������� �����

public:
    // ����������� � ��������� ��������
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

    // ������������ ������� (��� �������� �����)
    virtual std::shared_ptr<Entity> clone() const = 0;

	// ����������� ������ ��������� ��������
    virtual void takeTurn() = 0;
    virtual void takeDamage(int dmg) = 0;

    // �������
    int getHealth() const { return health; }
    int getMeleeAttackPower() const { return meleeAttackPower; }
    int getRangedAttackPower() const { return rangedAttackPower; }

    // �������
    void setHealth(int h) { health = (h > 0 ? h : 0); }
    void setMeleeAttackPower(int power) { meleeAttackPower = (power > 0 ? power : 0); }
    void setRangedAttackPower(int power) { rangedAttackPower = (power > 0 ? power : 0); }

    // �������� ���������
    bool isAlive() const { return health > 0; }
};
