#pragma once
#include "ISpell.h"

class ISummonSpell : public ISpell {
public:
    virtual int alliesCount() const = 0;
};