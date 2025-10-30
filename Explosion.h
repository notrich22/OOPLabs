#pragma once
#include "IAOESpell.h"
#include "CastContext.h"
#include <string>

class Explosion final : public IAOESpell {
    int damage_;
    int area_; // сторона области (например, 2)
public:
    Explosion(int damage = 15, int area = 2)
        : damage_(damage), area_(area) {
    }

    std::string name() const override { return "Explosion"; }
    int areaSize() const override { return area_; }
    int damagePerCell() const override { return damage_; }

    bool canCast(const CastContext& ctx) const override;
    bool cast(CastContext& ctx) override;
};
