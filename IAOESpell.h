#pragma once
#include "ISpell.h"

class IAOESpell: public ISpell {
public:
    virtual int areaSize() const = 0;
    virtual int damagePerCell() const = 0;
};