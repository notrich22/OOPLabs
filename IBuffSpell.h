#pragma once
#include "ISpell.h"
#include "EnhancementState.h"

class IBuffSpell : public ISpell {
public:
    virtual int stacksGranted() const noexcept = 0;

    virtual void applyBuff(EnhancementState& state) const = 0;
};
