#pragma once
#include <random>

class Ant;
class Anthill;

class Enemy {
private:
    int strength;
    int stolenFood;
    bool isAlive;
    std::mt19937 rng;

public:
    Enemy(int strength);

    void Attack(Ant& ant);
    bool TryStealFood(Anthill& anthill, int amount);
    void FightSoldier(Ant& soldier);

    bool IsAlive() const { return isAlive; }
    int GetStolenFood() const { return stolenFood; }
    int GetStrength() const { return strength; }
    void TakeDamage(int damage);

};