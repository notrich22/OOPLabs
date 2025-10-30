#pragma once

#include "IBuffSpell.h"
#include <iostream>

class EnhanceSpell final : public IBuffSpell {
public:
    std::string name() const override { return "EnhanceSpell"; }

    int stacksGranted() const noexcept override { return 1; }

    void applyBuff(EnhancementState& state) const override {
        state.stacks += stacksGranted();

        std::cout << "[EnhanceSpell] Magic stacks increased to "
            << state.stacks << "!\n";
    }

    bool canCast(const CastContext&) const override { return true; }

    bool cast(CastContext& ctx) override {
        applyBuff(ctx.enhancement);
        std::cout << "You focus your energy... Magic grows stronger!\n";
        return true;
    }
};
