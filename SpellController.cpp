#include "SpellController.h"
#include "Cell.h"
#include <iostream>

bool SpellController::castSpell(
    std::size_t spellIndex,
    std::optional<std::pair<int, int>> cell,
    std::shared_ptr<ICombatEntity> target)
{
    if (!player) {
        printer.printMessage("Player reference invalid.");
        return false;
    }

    auto& hand = player->getSpellHand();
    auto spell = hand.getSpell(spellIndex);
    if (!spell) {
        printer.printMessage("Invalid spell index.");
        return false;
    }

    // --- ������ �������� ---
    CastContext ctx{
        *player,          // ��� �������
        board,            // ����
        std::weak_ptr<ICombatEntity>(target), // ���� (���� ����)
        cell,             // ������ (���� ����)
        enhancement       // ��������
    };

    // --- �������� ---
    if (!spell->canCast(ctx)) {
        printer.printMessage("You cannot cast this spell right now.");
        return false;
    }

    // --- ���� ---
    bool success = spell->cast(ctx);

    if (success) {
        printer.printMessage(spell->name() + " cast successfully!");

        // ���� ����� ����������� � �������
        if (spell->consumable())
            hand.removeSpell(spellIndex);
    }
    else {
        printer.printMessage(spell->name() + " failed!");
    }

    return success;
}
