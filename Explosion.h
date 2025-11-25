#pragma once
#include "IAOESpell.h"
#include "CastContext.h"
#include <string>

class Explosion final : public IAOESpell {
    int damage_;
    int area_;
public:
    Explosion(int damage = 15, int area = 2)
        : damage_(damage), area_(area) {
    }

    std::string name() const override { return "Explosion"; }
    int areaSize() const override { return area_; }
    int damagePerCell() const override { return damage_; }

    bool canCast(const CastContext& ctx) const override;
    bool cast(CastContext& ctx) override;

    void upgrade() override {
        damage_ += 5;
    }
    std::string getUpgradeInfo() const override {
        return "Increase Explosion Damage by 5 (Current: " + std::to_string(damage_) + ")";
    }
};