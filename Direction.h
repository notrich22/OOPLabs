#pragma once
#include <array>
#include <optional>

enum class Direction { Up, Down, Left, Right, None };

struct Offset { int dx; int dy; };

constexpr Offset delta(Direction dir) noexcept {
    switch (dir) {
    case Direction::Up:    return { 0, -1 };
    case Direction::Down:  return { 0,  1 };
    case Direction::Left:  return { -1, 0 };
    case Direction::Right: return { 1,  0 };
    default:               return { 0,  0 };
    }
}

inline std::optional<Direction> keyToDir(char key) noexcept {
    switch (key) {
    case 'w': return Direction::Up;
    case 's': return Direction::Down;
    case 'a': return Direction::Left;
    case 'd': return Direction::Right;
    default:  return std::nullopt;
    }
}

// === Добавь это ===
constexpr std::array<Direction, 4> cardinalDirs = {
    Direction::Up, Direction::Down, Direction::Left, Direction::Right
};
