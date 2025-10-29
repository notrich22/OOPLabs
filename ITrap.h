#pragma once
#include "ISpell.h"

class ITrap : public ISpell {
public:
    virtual int trapDamage() const = 0;
};
