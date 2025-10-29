#pragma once
struct EnhancementState {
    int stacks = 0;
    bool empty() const { return stacks == 0; }
    void clear() { stacks = 0; }
    int rangeBonus()  const { return stacks; }      // +1 � �������/��������� �� ����
    int areaBonus()   const { return stacks; }      // +1 � ������� �������� �� ����
    int trapBonus()   const { return 5 * stacks; }  // +���� �������
    int summonBonus() const { return stacks; }      // +���������
};
