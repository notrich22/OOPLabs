#pragma once
#include "MovableEntity.h"
#include "ICombatEntity.h"
#include "Direction.h"
#include "Config.h"
#include "SpellHand.h"
#include <memory>

// ����� Player � ����������� ������������� ��������
class Player : public MovableEntity, public ICombatEntity {
private:
    unsigned int experience_ = 0;                     // ���� ������
    unsigned int attackRange_ = Config::PLAYER_ATTACK_RANGE;  // ��������� ��������
    unsigned int mana_ = 100;                         // ���� ������
    SpellHand spellHand_{ Config::PLAYER_SPELLS_COUNT }; // ���� �� ��������
public:
    Player(
        int health = Config::PLAYER_DEFAULT_HEALTH,
        int meleeDamage = Config::PLAYER_MELEE_DAMAGE,
        int rangedDamage = Config::PLAYER_RANGED_DAMAGE)
        : MovableEntity(), ICombatEntity() {
        health_ = health;
        meleeAttackPower_ = meleeDamage;
        rangedAttackPower_ = rangedDamage;
    }

    void takeTurn() override;
    char symbol() const noexcept override { return 'P'; }
    std::shared_ptr<Entity> clone() const override;

    std::pair<int, int> getPosition() const override { return Entity::getPosition(); }
    Faction faction() const override { return Faction::Player; }

    void takeDamage(int amount) override;

    unsigned int getExperience() const noexcept { return experience_; }
    void addExperience(unsigned int exp);

    unsigned int getAttackRange() const noexcept { return attackRange_; }

    unsigned int getMana() const noexcept { return mana_; }
    void setMana(unsigned int m) noexcept { mana_ = m; }

    // ������ �� ��������
    SpellHand& getSpellHand() noexcept { return spellHand_; }
    const SpellHand& getSpellHand() const noexcept { return spellHand_; }

    // ������������ ������ �����
    void switchMode();
};
