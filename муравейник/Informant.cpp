#include "Informant.h"
#include <algorithm>

void Informant::Subscribe(EventType type, const std::function<void(EventType)>& callback) {
    subscribers[type].push_back(callback);
}

void Informant::Unsubscribe(EventType type, const std::function<void(EventType)>& callback) {
    auto& callbacks = subscribers[type];
    callbacks.erase(
        std::remove_if(callbacks.begin(), callbacks.end(),
            [&callback](const auto& cb) {
                return cb.target_type() == callback.target_type();
            }),
        callbacks.end()
    );
}

void Informant::Notify(EventType event) const {
    if (subscribers.count(event)) {
        for (const auto& callback : subscribers.at(event)) {
            callback(event);
        }
    }
}
