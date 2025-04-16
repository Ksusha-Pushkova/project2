// Role.cpp
#include "Role.h"
#include "Ant.h"
#include "Anthill.h"
#include <random>
#include <ctime>
#include <iostream> // Add this line

// ===== NoRole (???????) =====
void NoRole::Work(Ant& ant) {
    ant.Heal(1); 
}

EventType NoRole::GetSubscribedEventType() const {
    return EventType::StorageEmpty; // ?? ????????? ?? ???????
}

std::string NoRole::GetName() const {
    return "NoRole";
}

void NoRole::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::AnthillAttacked) {
        ant.TakeDamage(10); 
    }
}

// ===== Nurse (??????) =====
void Nurse::Work(Ant& ant) {
    if (auto anthill = ant.GetAnthill()) {
        anthill->AddFood(-1); // ?????????? ??? ??? ??????
        ant.Heal(2); 
    }
}

EventType Nurse::GetSubscribedEventType() const {
    return EventType::AnthillAttacked; // ????????? ?? ?????
}

std::string Nurse::GetName() const {
    return "Nurse";
}

void Nurse::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::AnthillAttacked) {
        if (auto anthill = ant.GetAnthill()) {
            anthill->ProtectLarvae(); 
        }
    }
}

// ===== Soldier (???????) =====
void Soldier::Work(Ant& ant) {
    if (auto anthill = ant.GetAnthill()) {
        anthill->AddFood(-2); 
    }
}

EventType Soldier::GetSubscribedEventType() const {
    return EventType::EnemySpotted; // ????????? ?? ??????
}

std::string Soldier::GetName() const {
    return "Soldier";
}

void Soldier::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::EnemySpotted) {
        ant.TakeDamage(5); // ???????? ???? ??? ??????
        std::cout << "Ant " << &ant << " (Soldier): Responding to enemy spotted, taking damage" << std::endl;
        if (auto anthill = ant.GetAnthill()) {
            if (auto enemy = anthill->GetNearestEnemy()) {
                enemy->FightSoldier(ant); 
                
            }
        }
    }
}

// ===== Shepherd (???????) =====
void Shepherd::Work(Ant& ant) {
    if (auto anthill = ant.GetAnthill()) {
        anthill->AddFood(-1);
        ant.Heal(3); 
    }
}

EventType Shepherd::GetSubscribedEventType() const {
    return EventType::StorageEmpty; 
}

std::string Shepherd::GetName() const {
    return "Shepherd";
}

void Shepherd::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::StorageEmpty) {
        ant.TakeDamage(2); 
    }
}

// ===== Forager (??????????) =====
void Forager::Work(Ant& ant) {
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 100);

    if (dist(rng) < 20) { // 20% ???? ????? ???
        if (auto anthill = ant.GetAnthill()) {
            int foodFound = 15 + dist(rng) % 10;
            anthill->AddFood(foodFound);
            
        }
    }
}

EventType Forager::GetSubscribedEventType() const {
    return EventType::LargeFoodFound; 
}

std::string Forager::GetName() const {
    return "Forager";
}

void Forager::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::LargeFoodFound) {
        ant.Heal(5); 
    }
}

// ===== Builder (?????????) =====
void Builder::Work(Ant& ant) {
    static std::mt19937 rng((static_cast<unsigned int>(std::time(nullptr))));
    std::uniform_int_distribution<int> dist(0, 100);

    if (dist(rng) < 15) { // 15% ???? ????? ?????????
        if (auto anthill = ant.GetAnthill()) {
            int materialsFound = 10 + dist(rng) % 5;
            anthill->AddBuildingMaterials(materialsFound);
            
        }
    }
}

EventType Builder::GetSubscribedEventType() const {
    return EventType::HeavyBranchFound; 
}

std::string Builder::GetName() const {
    return "Builder";
}

void Builder::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::HeavyBranchFound) {
        ant.Heal(3); 
    }
}

// ===== Cleaner (????????) =====
void Cleaner::Work(Ant& ant) {
    if (auto anthill = ant.GetAnthill()) {
        anthill->AddFood(-1);
        ant.Heal(1); 
    }
}

EventType Cleaner::GetSubscribedEventType() const {
    return EventType::AnthillAttacked; 
}

std::string Cleaner::GetName() const {
    return "Cleaner";
}

void Cleaner::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::AnthillAttacked) {
        ant.TakeDamage(3); 
    }
}

void EnemyRole::Work(Ant& ant) {
   
}

EventType EnemyRole::GetSubscribedEventType() const {
    return EventType::AnthillAttacked;
}



void EnemyRole::HandleEvent(Ant& ant, EventType event) {
    if (event == EventType::AnthillAttacked) {
        // Логика атаки муравейника
        std::cout << "Enemy attacking" << std::endl;
        // Враг атакует, можно добавить логику, например, чтобы атаковать случайного муравья
    }
}