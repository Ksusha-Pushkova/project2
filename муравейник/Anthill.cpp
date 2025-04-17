#include <functional>
#include "Anthill.h"
#include "Ant.h"
#include "Enemy.h"
#include <algorithm>
#include <random>
#include <iostream>
Anthill::Anthill(int initialSize)
    : size(initialSize), maxAnts(initialSize * 10), foodAmount(200), buildingMaterials(50), isUnderAttack(false) 
{
    
    const std::vector<std::function<std::unique_ptr<Role>()>> roleFactories = { //cоздаЄм функции дл€ создани€ ролей
        []() { return std::make_unique<Nurse>(); },
        []() { return std::make_unique<Soldier>(); },
        []() { return std::make_unique<Shepherd>(); },
        []() { return std::make_unique<Forager>(); },
        []() { return std::make_unique<Builder>(); },
        []() { return std::make_unique<Cleaner>(); }
    };
    for (int i = 0; i < 2; ++i) {  // создаЄм двух врагов
        auto enemyAnt = std::make_unique<Ant>( //создаЄм новоого муравь€
            /*age*/ 10,     
            /*health*/ 80,   
            /*informant*/ nullptr,
            /*anthill*/ this,
            /*isEnemy*/ true 
        );
        enemyAnt->ChangeRole(std::make_unique<EnemyRole>()); //назначаем роль врага
        AddAnt(std::move(enemyAnt));  
    }

    for (int i = 0; i < std::min(20, maxAnts); ++i) { //создаЄм муравьЄв
        auto ant = std::make_unique<Ant>(
            /*age*/ 0,
            /*health*/ 100,
            /*informant*/ nullptr,
            /*anthill*/ this
        );

        auto role = roleFactories[i % roleFactories.size()](); //выбор роли дл€ муравь€
        ant->ChangeRole(std::move(role)); //назначаем муравью эту роль

        AddAnt(std::move(ant));
    }
    std::srand(static_cast<unsigned>(time(nullptr)));
    
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
    static bool initialized = false;
    if (!initialized) {
        std::srand(static_cast<unsigned>(time(nullptr)));
        initialized = true;
    }

    // —оздание новых муравьЄв (с проверками)
    if (!roleFactories.empty() && ants.size() < maxAnts && foodAmount > 50) {
        auto ant = std::make_unique<Ant>(0, 100, nullptr, this);
        size_t roleIndex = rand() % roleFactories.size();
        ant->ChangeRole(roleFactories[roleIndex]());
        AddAnt(std::move(ant));
        foodAmount -= 10;

        std::cout << "New ant created! Total: " << ants.size()
            << ", Food left: " << foodAmount << std::endl;
    }
    if (buildingMaterials > 150) { //расширение муравейника
        Expand();
    }
    else if (buildingMaterials < 30) { //уменьшение
        Decay();
    }
    if (foodAmount <= 0) {
        for (auto& ant : ants) {
            ant->TakeDamage(1); // тер€ют здоровье без еды
        }
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
                    std::cout << "¬раг уничтожен!" << std::endl;
                    break; 
                }
            }
        }

    }
    //удаление мЄртвых
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

Enemy* Anthill::GetNearestEnemy() {
    if (!enemies.empty()) {
        return &enemies.front(); 
    }
    return nullptr;
}

void Anthill::RemoveDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies.end()
    );
}

bool Anthill::TakeFood(int amount) {
    if (foodAmount >= amount) {
        foodAmount -= amount;
        return true;
    }
    return false;
}
void Anthill::ProtectLarvae() {
    for (auto& ant : ants) {
        if (ant->GetRoleName() == "Nurse") {
            ant->Heal(5);
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

void Anthill::HandleAttack() {
    
}








