#pragma once

enum class EventType {
    EnemySpotted,       // Солдат заметил врага
    HeavyBranchFound,   // Найдена тяжелая ветка
    LargeFoodFound,     // Найдена большая порция еды
    AnthillAttacked,    // Нападение на муравейник
    StorageEmpty,       // Склады пусты
    NewLarvaeBorn       // Появились новые личинки
};