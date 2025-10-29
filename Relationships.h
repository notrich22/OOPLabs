#pragma once
#include "ICombatEntity.h"

struct RelationshipRules {
    // ������� ������� �source -> target�
    bool hostile[3][3]{}; // ���������� �� (int)Faction

    bool canDealDamage(Faction from, Faction to) const {
        return hostile[(int)from][(int)to];
    }

    static RelationshipRules Default() {
        RelationshipRules r{};
        r.hostile[(int)Faction::Player][(int)Faction::Enemy] = true;
        r.hostile[(int)Faction::Enemy][(int)Faction::Player] = true;
        r.hostile[(int)Faction::Enemy][(int)Faction::Neutral] = true;
        r.hostile[(int)Faction::Neutral][(int)Faction::Enemy] = true;
        // ��������� � false (neutral �� ���� � �.�.)
        return r;
    }
};
