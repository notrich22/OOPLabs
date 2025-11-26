#pragma once
#include "Command.h"
#include "Direction.h"
#include "Player.h" 
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h> 

class InputHandler {
private:
    std::map<char, CommandType> keyMap;
    std::string statusMessage;

    CommandType stringToCommand(const std::string& str) {
        if (str == "Move") return CommandType::Move;
        if (str == "AttackMelee") return CommandType::AttackMelee;
        if (str == "AttackRanged") return CommandType::AttackRanged;
        if (str == "CastSpell") return CommandType::CastSpell;
        if (str == "SwitchWeapon") return CommandType::SwitchWeapon;
        if (str == "SaveGame") return CommandType::SaveGame;
        if (str == "LoadGame") return CommandType::LoadGame;
        if (str == "Quit") return CommandType::Quit;
        return CommandType::None;
    }

    std::vector<CommandType> getRequiredCommands() const {
        return {
            CommandType::Move,
            CommandType::AttackMelee,
            CommandType::AttackRanged,
            CommandType::CastSpell,
            CommandType::SwitchWeapon,
            CommandType::SaveGame,
            CommandType::LoadGame,
            CommandType::Quit
        };
    }

public:
    InputHandler(const std::string& configFile = "controls.cfg") {
        setDefaultControls();
        loadConfig(configFile);
    }

    std::string getStatusMessage() const { return statusMessage; }

    void setDefaultControls() {
        keyMap.clear();
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
        statusMessage = "Controls set to DEFAULT.";
    }

    void loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

        std::map<char, CommandType> tempMap;
        std::set<CommandType> assignedCommands;

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
            if (type == CommandType::None) continue;

            if (tempMap.count(key)) {
                setDefaultControls();
                statusMessage = "[Config Error] Duplicate key assignment.";
                return;
            }

            if (type != CommandType::Move && assignedCommands.count(type)) {
                setDefaultControls();
                statusMessage = "[Config Error] Duplicate command assignment.";
                return;
            }

            tempMap[key] = type;
            assignedCommands.insert(type);
        }

        for (const auto& req : getRequiredCommands()) {
            if (assignedCommands.find(req) == assignedCommands.end()) {
                setDefaultControls();
                statusMessage = "[Config Error] Missing required command.";
                return;
            }
        }

        keyMap = tempMap;
        statusMessage = "Custom controls loaded from " + filename;
    }

    Command getCommand(const Player& player) {
        int key = _getch();

        if (key == 0 || key == 224) {
            _getch();
            return Command{};
        }

        char input = static_cast<char>(std::tolower(key));

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
            return cmd;
        }

        if (cmd.type == CommandType::AttackRanged) {
            std::cout << "\n[AIM] Direction (w/a/s/d): ";
            int dKey = _getch();
            char d = static_cast<char>(std::tolower(dKey));

            auto dirOpt = keyToDir(d);
            if (dirOpt) cmd.dir = *dirOpt;
            else {
                std::cout << "Invalid direction.\n";
                cmd.type = CommandType::None;
            }
        }
        else if (cmd.type == CommandType::CastSpell) {
            auto names = player.getSpellHand().getSpellNames();
            if (names.empty()) {
                std::cout << "\n[!] No spells available!\n";
                cmd.type = CommandType::None;
                return cmd;
            }

            std::cout << "\nAvailable Spells:\n";
            for (size_t i = 0; i < names.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << names[i] << "\n";
            }

            std::cout << "Spell index (1-" << names.size() << "): ";
            int idx;
            if (std::cin >> idx && idx > 0 && idx <= names.size()) {
                cmd.spellIndex = idx - 1;

                std::cout << "Target X Y (if needed, else 0 0): ";
                int x, y;
                std::cin >> x >> y;
                cmd.target = { x, y };

                std::cout << "Direction w/a/s/d (if needed): ";
                int dKey = _getch();
                char d = static_cast<char>(dKey);

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