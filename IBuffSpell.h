#pragma once
#include "ISpell.h"

class IBuffSpell: public ISpell {
public:
    virtual int stacksGranted() const = 0;
};