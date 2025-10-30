#pragma once
#include <utility>
#include <memory>

// ������� ����� �������� �� ����
class Entity {
protected:
    std::pair<int, int> position{};  // ���������� �������� �� ����

public:
    Entity() = default;
    virtual ~Entity() = default;

    // ����� ���������
    virtual void takeTurn() = 0;                     // ��������� ��������
    virtual std::shared_ptr<Entity> clone() const = 0; // ������������
    virtual char symbol() const noexcept = 0;          // ������ �� ����
    virtual bool isBlocking() const noexcept { return true; }

    // ������ � ������
    std::pair<int, int> getPosition() const noexcept { return position; }
    int getX() const noexcept { return position.first; }
    int getY() const noexcept { return position.second; }

    void setPosition(int x, int y) noexcept { position = { x, y }; }
};
