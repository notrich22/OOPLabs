#pragma once
#include "ITrapSpell.h"
#include "CastContext.h"
#include <string>

class TrapSpell final : public ITrapSpell {
    int damage_;
public:
    explicit TrapSpell(int damage = 20)
        : damage_(damage) {
    }

    std::string name() const override { return "TrapSpell"; }
    int trapDamage() const override { return damage_; }

    bool canCast(const CastContext& ctx) const override;
    bool cast(CastContext& ctx) override;
};
