#pragma once
#include <functional>
#include <vector>
#include <map>
#include "EventType.h"

class Informant {
private:
    std::map<EventType, std::vector<std::function<void(EventType)>>> subscribers;

public:
    void Subscribe(EventType type, const std::function<void(EventType)>& callback);
    void Unsubscribe(EventType type, const std::function<void(EventType)>& callback);
    void Notify(EventType event) const;
};
