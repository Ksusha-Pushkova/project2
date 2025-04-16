#include "Enemy.h"
#include "Ant.h"
#include "Anthill.h"  
#include <ctime>
#include <iostream> 
#include <random> 

Enemy::Enemy(int strength)
    : strength(strength), stolenFood(0), isAlive(true), rng(std::random_device{}()) { 
}

void Enemy::Attack(Ant& ant) {
    if (ant.GetRoleName() == "Soldier") {
        FightSoldier(ant);
    }
    else {
        ant.TakeDamage(strength);
    }
}

bool Enemy::TryStealFood(Anthill& anthill, int amount) {
    if (anthill.GetFoodAmount() >= amount) {
        anthill.AddFood(-amount);
        stolenFood += amount;
        return true;
    }
    return false;
}

void Enemy::FightSoldier(Ant& soldier) {
    std::uniform_int_distribution<int> dist(0, 100);
    int soldierAttackChance = dist(rng);
    int enemyAttackChance = dist(rng);

    // Вероятность отпугивания (при высоком здоровье солдата)
    if (soldier.GetHealth() > 75 && soldierAttackChance > 60) {
        // Отпугивание
        std::cout << "Солдат отпугнул врага!" << std::endl;
        isAlive = false; // Враг убегает
        return;
    }

    // Атака солдата (убийство врага)
    if (soldierAttackChance > strength + 20) { // Шанс на успех солдата больше
        std::cout << "Солдат убил врага!" << std::endl;
        isAlive = false; // Враг убит
        return;
    }

    // Атака врага (убийство солдата)
    if (enemyAttackChance > 70) { // Шанс врага на успех выше
        std::cout << "Враг убил солдата!" << std::endl;
        soldier.TakeDamage(strength);
        return;
    }

    // Взаимные повреждения
    soldier.TakeDamage(strength / 2); // Солдат получает половину урона от врага
    TakeDamage(soldier.GetHealth() / 2); // Враг получает половину урона от здоровья солдата
    std::cout << "Враги обменялись ударами!" << std::endl;
}

void Enemy::TakeDamage(int damage) {
    strength -= damage;
    if (strength <= 0) {
        isAlive = false;
    }
}