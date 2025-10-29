#pragma once
struct EnhancementState {
    int stacks = 0;
    bool empty() const { return stacks == 0; }
    void clear() { stacks = 0; }
    int rangeBonus()  const { return stacks; }      // +1 к радиусу/дальности за стак
    int areaBonus()   const { return stacks; }      // +1 к стороне квадрата за стак
    int trapBonus()   const { return 5 * stacks; }  // +урон ловушки
    int summonBonus() const { return stacks; }      // +союзников
};
