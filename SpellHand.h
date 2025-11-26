#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ISpell.h"
#include "SpellFactory.cpp"
#include <algorithm>
#include <random>
// Класс SpellHand — "рука" игрока, в которой хранятся доступные заклинания
class SpellHand {
private:
    std::size_t capacity_;
    std::vector<std::shared_ptr<ISpell>> spells_;
public:
    explicit SpellHand(std::size_t capacity = 3)
        : capacity_(capacity) {
    }

    // Добавить спелл, если есть место. Возвращает true, если успешно
    bool addSpell(std::shared_ptr<ISpell> spell) {
        if (!spell) return false;
        if (spells_.size() >= capacity_) return false;
        spells_.push_back(std::move(spell));
        return true;
    }

    // Получить спелл по индексу (без удаления)
    std::shared_ptr<ISpell> getSpell(std::size_t index) const {
        if (index >= spells_.size()) return nullptr;
        return spells_[index];
    }
    // Удаление половины карт с руки
    void removeRandomHalf() {
        if (spells_.empty()) return;

        size_t keepCount = spells_.size() / 2;

        if (spells_.size() == 1) return;

        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(spells_.begin(), spells_.end(), g);

        spells_.resize(keepCount);
    }
    // Удалить спелл по индексу
    void removeSpell(std::size_t index) {
        if (index < spells_.size())
            spells_.erase(spells_.begin() + static_cast<std::ptrdiff_t>(index));
    }

    // Текущее количество заклинаний
    std::size_t size() const noexcept { return spells_.size(); }

    // Максимальная вместимость
    std::size_t capacity() const noexcept { return capacity_; }

    std::string addRandomSpell() {
        if (spells_.size() >= capacity_) {
            return ""; // Рука полна
        }
        auto spell = SpellFactory::createRandom();
        std::string name = spell->name();
        addSpell(spell);
        return name;
    }

    // Проверяет, пустая ли рука
    bool empty() const noexcept { return spells_.empty(); }

    // Возвращает имена всех заклинаний (для отображения в интерфейсе)
    std::vector<std::string> getSpellNames() const {
        std::vector<std::string> names;
        names.reserve(spells_.size());
        for (const auto& s : spells_) {
            names.push_back(s ? s->name() : "<empty>");
        }
        return names;
    }
    // Получить весь список заклинаний
    const std::vector<std::shared_ptr<ISpell>>& getSpells() const noexcept {
        return spells_;
    }

    // Очистить все заклинания
    void clear() noexcept {
        spells_.clear();
    }

};
