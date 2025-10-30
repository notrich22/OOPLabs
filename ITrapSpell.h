#pragma once
#include "ISpell.h"

class ITrapSpell : public ISpell {
public:
    virtual int trapDamage() const = 0;
};
