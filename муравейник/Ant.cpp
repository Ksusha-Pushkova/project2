#include "Ant.h"
#include "Role.h"
#include "Informant.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

Ant::Ant(int initialAge, int initialHealth, Informant* informant, Anthill* anthill, bool isEnemy)
    : age(initialAge), health(initialHealth), informant(informant),
    isAlive(true), homeAnthill(anthill) {

    if (isEnemy) { //если муравей - враг, то создаём роль врага
        role = std::make_unique<EnemyRole>();
        std::cout << "New enemy ant created!" << std::endl;
        return;
    }

    static bool srand_called = false;
    if (!srand_called) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        srand_called = true;
    }

    if (age < 5) {
        role = std::make_unique<NoRole>();
    }
    else if (age < 15) {
        role = std::make_unique<Nurse>();
    }
    else if (age < 30) {
        if (health < 50) {
            role = std::make_unique<Shepherd>();
        }
        else {
            role = std::make_unique<Soldier>();
        }
    }
    else if (age < 50) {
        if (rand() % 2 == 0) { //с вероятностью 50% роль собирателя
            role = std::make_unique<Forager>();
        }
        else {
            role = std::make_unique<Builder>();//с такой же вероятностью роль строителя
        }
    }
    else {
        role = std::make_unique<Cleaner>();
    }

    if (role && informant) { //если у муравья есть роль, он подписывается на события
        informant->Subscribe(role->GetSubscribedEventType(),
            [this](EventType e) { OnEvent(e); });
    }
    std::cout << "New ant created! Age: " << age << ", Health: " << health << ", Role: " << GetRoleName() << std::endl;
}

void Ant::Update() {
    if (!isAlive) return; //если муравей мёртв, завершаем работу

    std::cout << "Ant " << this << " (Age: " << age << ", Health: " << health << ", Role: " << GetRoleName() << "): Updating" << std::endl;

    age++;
    health = std::min(100, health - 1); //гарантирует, что здоровье не превысит 100

    if (age == 5 || age == 15 || age == 30 || age == 50) { //меняет роль при достижении переходного возраста
        std::unique_ptr<Role> newRole;

        if (age == 5) {
            newRole = std::make_unique<Nurse>();
            std::cout << "Ant " << this << ": Role change - NoRole to Nurse" << std::endl;
        }
        else if (age == 15) {
            if (health < 50) {
                newRole = std::make_unique<Shepherd>();
                std::cout << "Ant " << this << ": Role change - to Shepherd" << std::endl;
            }
            else {
                newRole = std::make_unique<Soldier>();
                std::cout << "Ant " << this << ": Role change - to Soldier" << std::endl;
            }
        }
        else if (age == 30) {
            if (std::rand() % 2 == 0) {
                newRole = std::make_unique<Forager>();
                std::cout << "Ant " << this << ": Role change - to Forager" << std::endl;
            }
            else {
                newRole = std::make_unique<Builder>();
                std::cout << "Ant " << this << ": Role change - to Builder" << std::endl;
            }
        }
        else if (age == 50) {
            newRole = std::make_unique<Cleaner>();
            std::cout << "Ant " << this << ": Role change - to Cleaner" << std::endl;
        }

        if (newRole) {
            ChangeRole(std::move(newRole));
        }
    }

    if (health <= 0) { //если здоровье меньше 0, муравей умират, функция завершает работу
        isAlive = false;
        std::cout << "Ant " << this << ": Died" << std::endl;
        return;
    }

    Work(); //если муравей жив и роль не поменял, то выполняется работа, характерная текущей роли муравья.
}


void Ant::OnEvent(EventType event) { //передает событие текущей роли муравья для обработки
    if (role) {
        role->HandleEvent(*this, event); //обрабатывает событие в соответствии с поведением, определенным для данной роли
    }
}
void Ant::Work() {
    if (role) {
        role->Work(*this);
    }
}

void Ant::ChangeRole(std::unique_ptr<Role> newRole) {
    if (!newRole) return;

    if (role && informant) {
        informant->Unsubscribe(role->GetSubscribedEventType(), //если у муравья есть роль , муравей отписывается от текущих событий, связанных с его старой ролью
            [this](EventType e) { OnEvent(e); });
    }

    role = std::move(newRole); //старая роль уничтожается

    if (role && informant) {
        informant->Subscribe(role->GetSubscribedEventType(), //муравей подписывается на события, связанные с его новой ролью
            [this](EventType e) { OnEvent(e); });
    }
}

void Ant::TakeDamage(int damage) { //уменьшает здоровье муравья от урона
    health = std::max(0, health - damage);
    if (health <= 0) {
        isAlive = false;
    }
}

void Ant::Heal(int amount) {
    health = std::min(100, health + amount);
}