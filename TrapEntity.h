#pragma once
#include "StaticEntity.h"
#include "ICombatEntity.h"
#include "Board.h"
#include <iostream>

class TrapEntity : public StaticEntity {
    int damage_;
public:
    TrapEntity(int x, int y, int damage)
        : damage_(damage) {
        setPosition(x, y);
    }
	bool isBlocking() const noexcept override { return false; }

    char symbol() const noexcept override { return '^'; }

    void takeTurn() override {} // ловушка не активна сама по себе

    int damage() const noexcept { return damage_; }

    // Когда кто-то наступает на ловушку
    void trigger(ICombatEntity& victim, Board& board);
    std::shared_ptr<Entity> clone() const override {
        return std::make_shared<TrapEntity>(*this);
    }

};
