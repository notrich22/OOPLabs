#pragma once

struct EnhancementState {
    int stacks = 0;

    bool empty() const noexcept { return stacks == 0; }
    void clear() noexcept { stacks = 0; }

    // ������
    int rangeBonus()  const noexcept { return stacks; }       // +� �������/���������
    int areaBonus()   const noexcept { return stacks; }       // +� �������
    int trapBonus()   const noexcept { return 5 * stacks; }   // +���� �������
    int summonBonus() const noexcept { return stacks; }       // +����� ���������
};
