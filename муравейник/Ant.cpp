#include "Ant.h"

#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

Ant::Ant(int initialAge, int initialHealth, Anthill* anthill)
    : age(initialAge), health(initialHealth), 
    isAlive(true) {

    

    static bool srand_called = false;
    if (!srand_called) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        srand_called = true;
    }

    
    std::cout << "New ant created! Age: " << age << ", Health: " << health << ", Role: " <<  std::endl;
}

void Ant::Update() {
    if (!isAlive) return;

    std::cout << "Ant " << this << " (Age: " << age << ", Health: " << health << ", Role: "  << "): Updating" << std::endl;

    age++;
    health = std::min(100, health - 1); 
    




void Ant::Heal(int amount) {
    health = std::min(100, health + amount);
}