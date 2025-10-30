#pragma once
#include <memory>
#include "Entity.h"

// ��� ������: �������, ����� ��� �����������
enum class CellType { Normal, Wall, Slow };

// ����� Cell ��������� ��������� ������ �������� ����
class Cell {
private:
    CellType type;                        // ��� ������
    std::shared_ptr<Entity> entity;       // ��������� �� �������� (nullptr, ���� ������ �����)

public:
    // ����������� � ����� �� ���������
    explicit Cell(CellType type = CellType::Normal)
        : type(type), entity(nullptr) {
    }

    // ����������� ����������� (�������� ����������� ��������)
    Cell(const Cell& other)
        : type(other.type)
    {
        if (other.entity)
            entity = other.entity->clone();
    }

    // ����������� � �������� ����������� �� ���������
    Cell(Cell&&) noexcept = default;
    Cell& operator=(Cell&&) noexcept = default;

    // �������� ����������� ������������ (�������� �����)
    Cell& operator=(const Cell& other) {
        if (this != &other) {
            type = other.type;
            entity = other.entity ? other.entity->clone() : nullptr;
        }
        return *this;
    }

    // ������ � ����� ������
    CellType getType() const noexcept { return type; }
    void setType(CellType newType) noexcept { type = newType; }

    // ������ � ���������
    bool hasEntity() const noexcept { return entity != nullptr; }
    bool isOccupied() const noexcept { return entity && entity->isBlocking(); }
    std::shared_ptr<Entity> getEntity() const noexcept { return entity; }  // �������� ��������
    void setEntity(std::shared_ptr<Entity> e) { entity = std::move(e); } // ��������� ��������
    void clearEntity() noexcept { entity.reset(); }                        // �������� ������
};
