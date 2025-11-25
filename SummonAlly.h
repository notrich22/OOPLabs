#pragma once
#include "ISummonSpell.h"

class SummonAlly final : public ISummonSpell {
    int health_ = 30;
    int damage_ = 10;
public:
    SummonAlly() = default;
    SummonAlly(int hp, int dmg) : health_(hp), damage_(dmg) {}

    std::string name() const override { return "SummonAlly"; }
    int summonHealth() const noexcept override { return health_; }
    int summonDamage() const noexcept override { return damage_; }

    bool canCast(const CastContext& ctx) const override;
    bool cast(CastContext& ctx) override;

    void upgrade() override {
        health_ += 10;
        damage_ += 2;
    }
    std::string getUpgradeInfo() const override {
        return "Ally stats +10HP/+2DMG (Current: " + std::to_string(health_) + "/" + std::to_string(damage_) + ")";
    }
};