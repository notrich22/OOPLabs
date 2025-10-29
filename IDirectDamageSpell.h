#pragma once
#include "ISpell.h"

class IDirectDamageSpell : public ISpell {
public:
    virtual int baseDamage() const = 0;
    virtual int range() const = 0;
};
