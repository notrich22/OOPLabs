#include "EnemyTower.h"

EnemyTower::EnemyTower(std::shared_ptr<IDirectDamageSpell> spell, int range, int cooldown)
    : attackRange_(range), spell_(std::move(spell)) {
    health_ = Config::TOWER_DEFAULT_HEALTH;
	cooldown_ = cooldown;
}

std::shared_ptr<Entity> EnemyTower::clone() const {
    return std::make_shared<EnemyTower>(spell_, attackRange_);
}

void EnemyTower::takeDamage(int amount) {
	ICombatEntity::takeDamage(amount);
    std::cout << "Enemy Tower at (" << getX() << ", " << getY() << ") takes " 
		<< amount << " damage. HP: " << health_ << ".\n";
}
