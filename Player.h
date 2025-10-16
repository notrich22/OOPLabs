#pragma once
#include "MovableEntity.h"


// Класс Player представляет управляемого игроком персонажа
class Player : public MovableEntity {
private:
    unsigned int experience;  // накопленный опыт
    AttackMode mode;          // текущий режим атаки

public:
    // Конструктор с начальными параметрами здоровья и урона
    Player(int health = 100, int meleeAttackPower = 20, int rangedAttackPower = 10)
        : MovableEntity(health, meleeAttackPower, rangedAttackPower),
        experience(0),
        mode(AttackMode::Melee) {
    }

    Player(const Player&) = default;

    // Игровое поведение
    void takeTurn() override;            // ход игрока
    void takeDamage(int dmg) override;   // получение урона

    // Работа с опытом
    void addExperience(unsigned int exp) { experience += exp; }
    unsigned int getExperience() const { return experience; }

    // Переключение между ближней и дальней атакой
    void switchMode() {
        mode = (mode == AttackMode::Melee) ? AttackMode::Ranged : AttackMode::Melee;
    }

    // Получить текущий режим и дальность атаки
    AttackMode getAttackMode() const { return mode; }
    int getAttackRange() const {
        return (mode == AttackMode::Melee) ? 1 : 3;
    }

    // Клонирование объекта (для копирования сущностей на поле)
    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<Player>(*this);
    }
};
