#pragma once

// Общие параметры игры
namespace Config {

    // ----- Размер поля -----
    constexpr int MIN_BOARD_SIZE = 10;    // минимальный размер поля
    constexpr int MAX_BOARD_SIZE = 25;    // максимальный размер поля

    // ----- Игрок -----
    constexpr int PLAYER_DEFAULT_HEALTH = 1000;
    constexpr int PLAYER_MELEE_DAMAGE = 20;
    constexpr int PLAYER_RANGED_DAMAGE = 10;
    constexpr int PLAYER_ATTACK_RANGE = 2;
    constexpr int PLAYER_SPELLS_COUNT = 5;
    // ----- Враги -----
    constexpr int ENEMY_DEFAULT_HEALTH = 40;
    constexpr int ENEMY_MELEE_DAMAGE = 10;

    // ----- Спавнер -----
    constexpr int SPAWNER_HEALTH = 100;
    constexpr int SPAWN_COOLDOWN = 4;   // через сколько ходов появляется враг
    // ----- Башня -----
    constexpr int TOWER_DEFAULT_HEALTH = 50;
    constexpr int TOWER_DEFAULT_COOLDOWN = 3;

    // ----- Генерация врагов -----
    constexpr double ENEMY_DENSITY_FACTOR = 0.05; // плотность врагов = 5% от площади поля
    constexpr double ENEMYTOWER_DENSITY_FACTOR = 0.01; // плотность башен = 1% от площади поля
    constexpr double SPAWNER_DENSITY_FACTOR = 0.02; // плотность врагов = 2% от площади поля

    // ----- Эффекты клеток -----
    constexpr double WALL_PROBABILITY = 0.1;  // вероятность стены при генерации
    constexpr double SLOW_PROBABILITY = 0.1;  // вероятность замедляющей клетки

    // ----- Прочее -----
    constexpr int DAMAGE_EXPERIENCE_REWARD = 10;  // опыт за убийство врага

    // ----- Баланс уровней -----
    constexpr double LEVEL_HP_MULTIPLIER = 1.2; // +20% ХП за уровень
    constexpr int LEVEL_SIZE_INCREMENT = 2;     // +2 клетки к размеру поля за уровень
    constexpr int MAX_LEVEL_SIZE = 25;          // Ограничение размера
    
    // ----- КОНЕЦ ИГРЫ ------
    constexpr int MAX_LEVEL = 3; // После прохождения 3-го уровня — победа
}
