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
};
