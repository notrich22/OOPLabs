#pragma once
#include "Command.h"
#include "Direction.h"
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

class InputHandler {
private:
    std::map<char, CommandType> keyMap;

    CommandType stringToCommand(const std::string& str) {
        if (str == "Move") return CommandType::Move;
        if (str == "AttackMelee") return CommandType::AttackMelee;
        if (str == "AttackRanged") return CommandType::AttackRanged;
        if (str == "CastSpell") return CommandType::CastSpell;
        if (str == "SwitchWeapon") return CommandType::SwitchWeapon;
        if (str == "SaveGame") return CommandType::SaveGame;
        if (str == "LoadGame") return CommandType::LoadGame;
        if (str == "Quit") return CommandType::Quit;
        if (str == "SkipTurn") return CommandType::SkipTurn;
        return CommandType::None;
    }

public:
    InputHandler(const std::string& configFile = "controls.cfg") {
        loadConfig(configFile);
    }

    void loadConfig(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Config file not found. Using defaults.\n";
            setDefaultControls();
            return;
        }

        keyMap.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue;

            char key = std::tolower(line[0]);
            std::string commandStr = line.substr(delimiterPos + 1);

            if (!commandStr.empty() && commandStr.back() == '\r')
                commandStr.pop_back();

            CommandType type = stringToCommand(commandStr);

            if (type != CommandType::None) {
                keyMap[key] = type;
            }
        }
        std::cout << "Controls loaded from " << filename << "\n";
    }

    void setDefaultControls() {
        keyMap['w'] = CommandType::Move;
        keyMap['a'] = CommandType::Move;
        keyMap['s'] = CommandType::Move;
        keyMap['d'] = CommandType::Move;
        keyMap['e'] = CommandType::AttackMelee;
        keyMap['f'] = CommandType::AttackRanged;
        keyMap['m'] = CommandType::SwitchWeapon;
        keyMap['c'] = CommandType::CastSpell;
        keyMap['k'] = CommandType::SaveGame;
        keyMap['o'] = CommandType::LoadGame;
        keyMap['q'] = CommandType::Quit;
    }

    Command getCommand(const Player& player) {
        std::cout << "> ";
        char input;
        std::cin >> input;
        input = std::tolower(input);

        Command cmd;

        if (keyMap.find(input) != keyMap.end()) {
            cmd.type = keyMap[input];

            if (cmd.type == CommandType::Move) {
                if (input == 'w') cmd.dir = Direction::Up;
                else if (input == 'a') cmd.dir = Direction::Left;
                else if (input == 's') cmd.dir = Direction::Down;
                else if (input == 'd') cmd.dir = Direction::Right;
            }
        }
        else {
            std::cout << "Unknown key.\n";
            return cmd;
        }

        if (cmd.type == CommandType::AttackRanged) {
            std::cout << "Direction (w/a/s/d): ";
            char d; std::cin >> d;
            auto dirOpt = keyToDir(std::tolower(d));
            if (dirOpt) cmd.dir = *dirOpt;
            else cmd.type = CommandType::None;
        }
        // === ИСПРАВЛЕНИЕ ДЛЯ СПЕЛЛОВ ===
        if (cmd.type == CommandType::CastSpell) {
            // 1. Выводим список доступных заклинаний
            auto names = player.getSpellHand().getSpellNames();
            if (names.empty()) {
                std::cout << "[!] No spells available!\n";
                cmd.type = CommandType::None;
                return cmd;
            }

            std::cout << "Available Spells:\n";
            for (size_t i = 0; i < names.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << names[i] << "\n";
            }

            // 2. Просим ввод
            std::cout << "Spell index (1-" << names.size() << "): ";
            int idx;

            if (std::cin >> idx && idx > 0 && idx <= names.size()) {
                cmd.spellIndex = idx - 1;

                // Доп. параметры (цель/направление)
                std::cout << "Target X Y (if needed, else 0 0): ";
                int x, y; std::cin >> x >> y;
                cmd.target = { x, y };

                std::cout << "Direction w/a/s/d (if needed, else w): ";
                char d; std::cin >> d;
                auto dirOpt = keyToDir(d);
                if (dirOpt) cmd.dir = *dirOpt;
            }
            else {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Invalid index.\n";
                cmd.type = CommandType::None;
            }
        }

        return cmd;
    }
};