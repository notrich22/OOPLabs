#pragma once
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include "ISpell.h"
#include <random>
#include <functional> 
#include "SpellFactory.cpp"

// ����� SpellHand � "����" ������, � ������� �������� ��������� ����������
class SpellHand {
private:
    std::size_t capacity_;
    std::vector<std::shared_ptr<ISpell>> spells_;
public:
    explicit SpellHand(std::size_t capacity = 3)
        : capacity_(capacity) {
    }

    // �������� �����, ���� ���� �����. ���������� true, ���� �������
    bool addSpell(std::shared_ptr<ISpell> spell) {
        if (!spell) return false;
        if (spells_.size() >= capacity_) return false;
        spells_.push_back(std::move(spell));
        return true;
    }

    // �������� ����� �� ������� (��� ��������)
    std::shared_ptr<ISpell> getSpell(std::size_t index) const {
        if (index >= spells_.size()) return nullptr;
        return spells_[index];
    }

    // ������� ����� �� ������� (��������, ���� �����������)
    void removeSpell(std::size_t index) {
        if (index < spells_.size())
            spells_.erase(spells_.begin() + static_cast<std::ptrdiff_t>(index));
    }

    // ������� ���������� ����������
    std::size_t size() const noexcept { return spells_.size(); }

    // ������������ �����������
    std::size_t capacity() const noexcept { return capacity_; }

    void addRandomSpell() {
        if (spells_.size() >= capacity_) {
            std::cout << "Cannot add: hand is full.\n";
            return;
        }
        addSpell(SpellFactory::createRandom());
    }

    // ���������, ������ �� ����
    bool empty() const noexcept { return spells_.empty(); }

    // ���������� ����� ���� ���������� (��� ����������� � ����������)
    std::vector<std::string> getSpellNames() const {
        std::vector<std::string> names;
        names.reserve(spells_.size());
        for (const auto& s : spells_) {
            names.push_back(s ? s->name() : "<empty>");
        }
        return names;
    }
};
