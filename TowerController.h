#pragma once
#include "Board.h"
#include "Direction.h"
#include "EnemyTower.h"
#include <memory>
#include <vector>

class TowerController {
    Board& board_;
    std::vector<std::shared_ptr<EnemyTower>> towers_;

public:
    explicit TowerController(Board& board);

    void addTower(const std::shared_ptr<EnemyTower>& tower, int x, int y);
    void update();
    const std::vector<std::shared_ptr<EnemyTower>>& getTowers() const { return towers_; }
    void clear();

};
