#pragma once
#include "MovableEntity.h"


// ����� Player ������������ ������������ ������� ���������
class Player : public MovableEntity {
private:
    unsigned int experience;  // ����������� ����
    AttackMode mode;          // ������� ����� �����

public:
    // ����������� � ���������� ����������� �������� � �����
    Player(int health = 100, int meleeAttackPower = 20, int rangedAttackPower = 10)
        : MovableEntity(health, meleeAttackPower, rangedAttackPower),
        experience(0),
        mode(AttackMode::Melee) {
    }

    Player(const Player&) = default;

    // ������� ���������
    void takeTurn() override;            // ��� ������
    void takeDamage(int dmg) override;   // ��������� �����

    // ������ � ������
    void addExperience(unsigned int exp) { experience += exp; }
    unsigned int getExperience() const { return experience; }

    // ������������ ����� ������� � ������� ������
    void switchMode() {
        mode = (mode == AttackMode::Melee) ? AttackMode::Ranged : AttackMode::Melee;
    }

    // �������� ������� ����� � ��������� �����
    AttackMode getAttackMode() const { return mode; }
    int getAttackRange() const {
        return (mode == AttackMode::Melee) ? 1 : 3;
    }

    // ������������ ������� (��� ����������� ��������� �� ����)
    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Player>(*this);
    }
};
