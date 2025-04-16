#pragma once
#include <string>
#include "EventType.h"
#include <random>

class Ant;
class Anthill;
class Enemy;
class Role {
public:
    virtual ~Role() = default;
    virtual void Work(Ant& ant) = 0;
    virtual EventType GetSubscribedEventType() const = 0;
    virtual std::string GetName() const = 0;
    virtual void HandleEvent(Ant& ant, EventType event) = 0;
};

class NoRole : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;

};

class Nurse : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class Soldier : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class Shepherd : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class Forager : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class Builder : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class Cleaner : public Role {
public:
    void Work(Ant& ant) override;
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override;
    void HandleEvent(Ant& ant, EventType event) override;
};

class EnemyRole : public Role {
public:
    void Work(Ant& ant) override; 
    EventType GetSubscribedEventType() const override;
    std::string GetName() const override { return "Enemy"; }
    void HandleEvent(Ant& ant, EventType event) override;
};