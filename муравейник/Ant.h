#pragma once
#include <memory>

class Anthill;
class Ant {
private:
    int age;
    int health;
    bool isAlive;
    Anthill* homeAnthill;

public:
    Ant(int age, int health, Anthill* anthill);


    void Update();
    void Work();
    
    
    void Heal(int amount);

    int GetAge() const { return age; }
    int GetHealth() const { return health; }
    bool IsAlive() const { return isAlive; }
    Anthill* GetAnthill() const { return homeAnthill; }
};