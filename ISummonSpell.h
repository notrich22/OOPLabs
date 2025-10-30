#pragma once
#include "ISpell.h"
#include "CastContext.h"

class ISummonSpell : public ISpell {
public:
    virtual int summonHealth() const noexcept = 0;
    virtual int summonDamage() const noexcept = 0;
};
