#include "Anthill.h"
#include "Ant.h"

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





