#pragma once
#include "IObservable.h"
#include <fstream>
#include <iostream>
#include <ctime>

class FileLogger : public IGameObserver {
private:
    std::ofstream file;
public:
    FileLogger(const std::string& filename) {
        file.open(filename, std::ios::app);
        if (file.is_open()) {
            file << "\n=== NEW SESSION ===\n";
        }
    }

    void onGameEvent(const std::string& message) override {
        if (file.is_open()) {
            std::time_t t = std::time(nullptr);
            char buffer[100];

            struct tm timeInfo;
            localtime_s(&timeInfo, &t);
            std::strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", &timeInfo);
            file << buffer << message << std::endl;
        }
    }
};
