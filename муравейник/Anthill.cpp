#include <functional>
#include "Anthill.h"
#include "Ant.h"
#include "Enemy.h"
#include <algorithm>
#include <random>
#include <iostream>
Anthill::Anthill(int initialSize)
    : size(initialSize), maxAnts(initialSize * 10), foodAmount(100), buildingMaterials(50), isUnderAttack(false) 
{
    
    const std::vector<std::function<std::unique_ptr<Role>()>> roleFactories = {
        []() { return std::make_unique<Nurse>(); },
        []() { return std::make_unique<Soldier>(); },
        []() { return std::make_unique<Shepherd>(); },
        []() { return std::make_unique<Forager>(); },
        []() { return std::make_unique<Builder>(); },
        []() { return std::make_unique<Cleaner>(); }
    };
    for (int i = 0; i < 2; ++i) {  // создаём двух врагов
        auto enemyAnt = std::make_unique<Ant>(
            /*age*/ 10,      // Age of the enemy
            /*health*/ 80,   // Health of the enemy
            /*informant*/ nullptr,
            /*anthill*/ this,
            /*isEnemy*/ true 
        );
        enemyAnt->ChangeRole(std::make_unique<EnemyRole>());
        AddAnt(std::move(enemyAnt));  
    }

    for (int i = 0; i < std::min(10, maxAnts); ++i) {
        auto ant = std::make_unique<Ant>(
            /*age*/ 0,
            /*health*/ 100,
            /*informant*/ nullptr,
            /*anthill*/ this
        );

        auto role = roleFactories[i % roleFactories.size()]();
        ant->ChangeRole(std::move(role));

        AddAnt(std::move(ant));
    }
    
    
}

void Anthill::AddAnt(std::unique_ptr<Ant> ant) {
    if (ants.size() < maxAnts) {
        ants.push_back(std::move(ant));
    }
}


void Anthill::AddFood(int amount) {
    foodAmount = std::max(0, foodAmount + amount);
}

void Anthill::AddBuildingMaterials(int amount) {
    buildingMaterials = std::max(0, buildingMaterials + amount);
}



void Anthill::AddEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
    isUnderAttack = true;
}

void Anthill::Update() {
    if (buildingMaterials > 100) {
        Expand();
    }
    else if (buildingMaterials < 10) {
        Decay();
    }

    for (auto& ant : ants) {
        ant->Update();
    }

    for (auto& ant : ants) {
        if (ant->GetRoleName() == "Enemy") {
            TryStealFood(*ant);
        }
    }

    for (auto& enemy : enemies) {
        for (auto& ant : ants) {
            if (ant->GetRoleName() == "Soldier") {
                enemy.FightSoldier(*ant);
                if (!enemy.IsAlive()) {
                    std::cout << "Враг уничтожен!" << std::endl;
                    break; 
                }
            }
        }
    }

    ants.erase(std::remove_if(ants.begin(), ants.end(),
        [](const auto& ant) { return !ant->IsAlive() || ant->GetRoleName() == "Enemy"; }),
        ants.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return !e.IsAlive(); }),
        enemies.end());
    if (isUnderAttack) {
        HandleAttack();
        isUnderAttack = false;
    }
}
void Anthill::TryStealFood(Ant& ant) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 100);
    if (dist(rng) < 20) {
        int foodStolen = 5 + dist(rng) % 10; 
        if (foodAmount >= foodStolen) {
            foodAmount -= foodStolen;
            std::cout << "Enemy stole " << foodStolen << " food from anthill!" << std::endl;
        }
    }
}

    
    

    
  

    
    


void Anthill::Expand() {
    size++;
    maxAnts = size * 10;
    buildingMaterials -= 100;
}

void Anthill::Decay() {
    size = std::max(1, size - 1);
    maxAnts = size * 10;
}






