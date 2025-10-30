#pragma once
#include "SummonAlly.h"

#include <memory>
#include <random>
#include "ISpell.h"
#include "Firebolt.h"
#include "Explosion.h"
#include "TrapSpell.h"
#include "EnhanceSpell.h"

class SpellFactory {
public:
    static std::shared_ptr<ISpell> createRandom() {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 4);
        switch (dist(rng)) {
        case 0: return std::make_shared<Firebolt>();
        case 1: return std::make_shared<Explosion>();
        case 2: return std::make_shared<TrapSpell>();
        case 3: return std::make_shared<SummonAlly>();
        default: return std::make_shared<EnhanceSpell>();
        }
    }
};
