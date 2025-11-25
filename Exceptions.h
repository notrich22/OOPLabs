#pragma once
#include <stdexcept>
#include <string>

// Базовый класс для всех ошибок игры
class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

// Ошибка: файл не найден или недоступен
class FileException : public GameException {
public:
    explicit FileException(const std::string& filename, const std::string& details)
        : GameException("File error [" + filename + "]: " + details) {
    }
};

// Ошибка: некорректная структура данных
class SaveDataException : public GameException {
public:
    explicit SaveDataException(const std::string& details)
        : GameException("Data error: " + details) {
    }
};

// Ошибка: несовпадение хэша
class CorruptedSaveException : public GameException {
public:
    explicit CorruptedSaveException(const std::string& details)
        : GameException("Integrity check failed: " + details) {
    }
};