#pragma once
#include <memory>
#include "Entity.h"

enum class CellType { Normal, Wall, Slow };

class Cell {
private:
    CellType type;
    std::shared_ptr<Entity> entity; // nullptr, если клетка пуста

public:
    explicit Cell(CellType type = CellType::Normal)
        : type(type), entity(nullptr) {
    }

    CellType getType() const { return type; }
    void setType(CellType newType) { type = newType; }

    bool isOccupied() const { return entity != nullptr; }
    std::shared_ptr<Entity> getEntity() const { return entity; }
    void setEntity(std::shared_ptr<Entity> e) { entity = std::move(e); }
    void clearEntity() { entity.reset(); }
};
