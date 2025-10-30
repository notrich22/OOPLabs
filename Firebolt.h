#pragma once
#include "IDirectDamageSpell.h"
#include "CastContext.h"
#include <string>

class Firebolt final : public IDirectDamageSpell {
    int damage_;
    int range_;
public:
    Firebolt(int damage = 10, int range = 3)
        : damage_(damage), range_(range) {
    }

    std::string name() const override { return "Firebolt"; }
    int baseDamage() const override { return damage_; }
    int range() const override { return range_; }

    bool canCast(const CastContext& ctx) const override;
    bool cast(CastContext& ctx) override;
};
