#pragma once
#include <memory>
#include "Entity.h"

// Тип клетки: обычная, стена или замедляющая
enum class CellType { Normal, Wall, Slow };

// Класс Cell описывает отдельную клетку игрового поля
class Cell {
private:
    CellType type;                        // тип клетки
    std::shared_ptr<Entity> entity;       // указатель на сущность (nullptr, если клетка пуста)

public:
    // Конструктор с типом по умолчанию
    explicit Cell(CellType type = CellType::Normal)
        : type(type), entity(nullptr) {
    }

    // Конструктор копирования (глубокое копирование сущности)
    Cell(const Cell& other)
        : type(other.type)
    {
        if (other.entity)
            entity = other.entity->clone();
    }

    // Конструктор и оператор перемещения по умолчанию
    Cell(Cell&&) noexcept = default;
    Cell& operator=(Cell&&) noexcept = default;

    // Оператор копирующего присваивания (глубокая копия)
    Cell& operator=(const Cell& other) {
        if (this != &other) {
            type = other.type;
            entity = other.entity ? other.entity->clone() : nullptr;
        }
        return *this;
    }

    // Работа с типом клетки
    CellType getType() const noexcept { return type; }
    void setType(CellType newType) noexcept { type = newType; }

    // Работа с сущностью
    bool hasEntity() const noexcept { return entity != nullptr; }
    bool isOccupied() const noexcept { return entity && entity->isBlocking(); }
    std::shared_ptr<Entity> getEntity() const noexcept { return entity; }  // получить сущность
    void setEntity(std::shared_ptr<Entity> e) { entity = std::move(e); } // поместить сущность
    void clearEntity() noexcept { entity.reset(); }                        // очистить клетку
};
