#pragma once
#include <memory>
#include "Role.h"
class Anthill;
class Ant {
private:
    int age;
    int health;
    std::unique_ptr<Role> role;
    bool isAlive;
    Anthill* homeAnthill;

public:
    Ant(int age, int health, Anthill* anthill, bool isEnemy = false);


    void Update();
    void Work();
    void ChangeRole(std::unique_ptr<Role> newRole);
    void OnEvent(EventType event);
    void TakeDamage(int damage);
    
    void Heal(int amount);

    int GetAge() const { return age; }
    int GetHealth() const { return health; }
    bool IsAlive() const { return isAlive; }
    const Role* GetRole() const { return role.get(); }
    std::string GetRoleName() const { return role ? role->GetName() : "None"; }
    Anthill* GetAnthill() const { return homeAnthill; }
};