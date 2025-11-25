#pragma once
#include "IBuffSpell.h"
#include <iostream>

class EnhanceSpell final : public IBuffSpell {
    int power_ = 1;
public:
    std::string name() const override { return "EnhanceSpell"; }

    int stacksGranted() const noexcept override { return power_; }

    void applyBuff(EnhancementState& state) const override {
        state.stacks += stacksGranted();
        std::cout << "[EnhanceSpell] Magic stacks increased by " << power_ << "!\n";
    }

    bool canCast(const CastContext&) const override { return true; }

    bool cast(CastContext& ctx) override {
        applyBuff(ctx.enhancement);
        std::cout << "You focus your energy... Magic grows stronger!\n";
        return true;
    }

    void upgrade() override {
        power_++;
    }
    std::string getUpgradeInfo() const override {
        return "Increase stacks generation by 1 (Current: " + std::to_string(power_) + ")";
    }
};