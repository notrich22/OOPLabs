#pragma once
#include <string>
#include <memory>
#include <optional>
#include "CastContext.h"

// Базовый интерфейс заклинания
class ISpell {
public:
    virtual ~ISpell() = default;

    virtual std::string name() const = 0;

    // Можно ли кастовать в текущем контексте
    virtual bool canCast(const CastContext& ctx) const = 0;

    // Выполняет заклинание
    virtual bool cast(CastContext& ctx) = 0;

    // По умолчанию карточка одноразовая
    virtual bool consumable() const { return true; }

    // Применяет улучшение к заклинанию 
    virtual void upgrade() = 0;

    // Возвращает текст описания улучшения
    virtual std::string getUpgradeInfo() const = 0;
};