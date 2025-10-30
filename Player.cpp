#include "Player.h"
#include <iostream>
#include "Game.h"
#include "Direction.h"

void Player::takeDamage(int dmg){
	ICombatEntity::takeDamage(dmg);
    std::cout << "Player took " << dmg << " damage! HP left: " << getHealth() << "\n";
}

void Player::addExperience(unsigned int exp) {
    experience_ += exp;
    std::cout << "Player gained " << exp << " XP! Total: " << experience_ << "\n";
}

void Player::takeTurn() {
    std::cout << "Player turn.\n";
}

void Player::switchMode() {
    if (attackMode_ == AttackMode::Melee) {
        attackMode_ = AttackMode::Ranged;
    }
    else {
        attackMode_ = AttackMode::Melee;
    }
}

std::shared_ptr<Entity> Player::clone() const {
    // создаём нового игрока с теми же параметрами
    auto copy = std::make_shared<Player>(getHealth(), getMeleeAttackPower(), getRangedAttackPower());

    // копируем простые поля
    copy->experience_ = this->experience_;
    copy->health_ = this->health_;
    copy->attackRange_ = this->attackRange_;
    copy->mana_ = this->mana_;
	copy->enhancementState_ = this->enhancementState_;
    // копируем все спеллы (глубоко)
    auto& sourceHand = this->spellHand_;
    auto& targetHand = copy->spellHand_;

    for (std::size_t i = 0; i < sourceHand.size(); ++i) {
        auto spell = sourceHand.getSpell(i);
        if (spell) {
            // если у спелла есть clone() — клонируем его, иначе копируем shared_ptr
            // (в базовом интерфейсе ISpell можно добавить виртуальный метод clone)
            targetHand.addSpell(spell);
        }
    }

    // сохраняем позицию на поле
    copy->setPosition(getX(), getY());

    return copy;
}
