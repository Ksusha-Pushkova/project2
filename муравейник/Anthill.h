#pragma once
#include <vector>
#include <memory>
#include "Ant.h"
#include "Enemy.h"

class Anthill {
private:

    int size;
    int maxAnts;
    int foodAmount;
    int buildingMaterials;
    bool isUnderAttack;

    std::vector<std::unique_ptr<Ant>> ants;  // Определение вектора муравьев
    std::vector<Enemy> enemies;

public:
    explicit Anthill(int initialSize = 5);

    // Методы управления муравьями
    void AddAnt(std::unique_ptr<Ant> ant);
    size_t GetAntCount() const { return ants.size(); }

    // Методы работы с ресурсами
    void AddFood(int amount);
    bool TakeFood(int amount);
    void AddBuildingMaterials(int amount);

    // Методы работы с врагами
    void AddEnemy(const Enemy& enemy);
    Enemy* GetNearestEnemy();
    void RemoveDeadEnemies();

    // Методы обновления состояния
    void Update();
    void Expand();
    void Decay();
    void HandleAttack();

    // Метод защиты детей
    void ProtectLarvae();
    void TryStealFood(Ant& ant);
    const std::vector<std::unique_ptr<Ant>>& GetAnts() const {
        return ants;
    }
    const std::vector<Enemy>& GetEnemies() const { return enemies; }

    // Геттеры
    int GetSize() const { return size; }
    int GetMaxAnts() const { return maxAnts; }
    int GetFoodAmount() const { return foodAmount; }
    int GetBuildingMaterials() const { return buildingMaterials; }
    bool IsUnderAttack() const { return isUnderAttack; }

    Anthill(const Anthill&) = delete;
    Anthill& operator=(const Anthill&) = delete;
};
