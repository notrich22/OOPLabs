#pragma once

struct EnhancementState {
    int stacks = 0;

    bool empty() const noexcept { return stacks == 0; }
    void clear() noexcept { stacks = 0; }

    // бонусы
    int rangeBonus()  const noexcept { return stacks; }       // +к радиусу/дальности
    int areaBonus()   const noexcept { return stacks; }       // +к площади
    int trapBonus()   const noexcept { return 5 * stacks; }   // +урон ловушки
    int summonBonus() const noexcept { return stacks; }       // +число союзников
};
