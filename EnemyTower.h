#pragma once
#include "StaticEntity.h"
class EnemyTower : public StaticEntity {
    int attackDamage_;
    int attackRange_;
    bool onCooldown_ = false;
public:
    EnemyTower(int dmg, int range) : attackDamage_(dmg), attackRange_(range) {}
    void takeTurn(Board& board, Player& player) {
        if (onCooldown_) { onCooldown_ = false; return; }
        if (distance(position, player.getPosition()) <= attackRange_) {
            player.takeDamage(attackDamage_);
            onCooldown_ = true;
        }
    }
};