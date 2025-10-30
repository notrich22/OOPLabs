#pragma once
#include <utility>
#include <memory>

// Базовый класс сущности на поле
class Entity {
protected:
    std::pair<int, int> position{};  // координаты сущности на поле

public:
    Entity() = default;
    virtual ~Entity() = default;

    // Общий интерфейс
    virtual void takeTurn() = 0;                     // поведение сущности
    virtual std::shared_ptr<Entity> clone() const = 0; // клонирование
    virtual char symbol() const noexcept = 0;          // символ на поле
    virtual bool isBlocking() const noexcept { return true; }

    // Доступ к данным
    std::pair<int, int> getPosition() const noexcept { return position; }
    int getX() const noexcept { return position.first; }
    int getY() const noexcept { return position.second; }

    void setPosition(int x, int y) noexcept { position = { x, y }; }
};
