#pragma once

// Общие параметры игры
namespace Config {

    // ----- Размер поля -----
    constexpr int MIN_BOARD_SIZE = 10;    // минимальный размер поля
    constexpr int MAX_BOARD_SIZE = 25;    // максимальный размер поля

    // ----- Игрок -----
    constexpr int PLAYER_DEFAULT_HEALTH = 100;
    constexpr int PLAYER_MELEE_DAMAGE = 20;
    constexpr int PLAYER_RANGED_DAMAGE = 10;
    constexpr int PLAYER_ATTACK_RANGE = 2;
    constexpr int PLAYER_SPELLS_COUNT = 3;
    // ----- Враги -----
    constexpr int ENEMY_DEFAULT_HEALTH = 40;
    constexpr int ENEMY_MELEE_DAMAGE = 10;

    // ----- Спавнер -----
    constexpr int SPAWNER_HEALTH = 100;
    constexpr int SPAWN_COOLDOWN = 4;   // через сколько ходов появляется враг

    // ----- Генерация врагов -----
    constexpr double ENEMY_DENSITY_FACTOR = 0.05; // плотность врагов = 5% от площади поля

    // ----- Эффекты клеток -----
    constexpr double WALL_PROBABILITY = 0.1;  // вероятность стены при генерации
    constexpr double SLOW_PROBABILITY = 0.1;  // вероятность замедляющей клетки

    // ----- Прочее -----
    constexpr int DAMAGE_EXPERIENCE_REWARD = 10;  // опыт за убийство врага
}
