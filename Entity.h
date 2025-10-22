#pragma once
#include <utility>
#include <memory>
#include <algorithm>

// ������� ����� �������� �� ����
class Entity {
protected:
    int health;                    // ��������
    std::pair<int, int> position;  // ���������� �������� �� ����
public:
    Entity(int health)
        : health(std::max(0, health)) {}

    virtual ~Entity() = default;

    // ���������, ����� ��� ����
    virtual void takeTurn() = 0;         // ��� �������� (������� ������)
    virtual void takeDamage(int dmg) {   // ��������� �����
        health = std::max(0, health - dmg);
    }

    virtual std::shared_ptr<Entity> clone() const = 0;

    // ������ � ������
    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }
    virtual char symbol() const noexcept = 0;

    std::pair<int,int> getPosition() const noexcept { return position; }
    int getX() const noexcept { return position.first; }
    int getY() const noexcept { return position.second; }

    void setPosition(int x, int y) noexcept { position = {x, y}; }
};
