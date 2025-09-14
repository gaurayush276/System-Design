#pragma once
#include "Event.h"
#include <vector>
#include <functional>

class EventBus {
public:
    using Callback = std::function<void(const Event&)>;

    void subscribe(Callback cb) {
        subscribers_.push_back(cb);
    }

    void publish(const Event& event) {
        for (auto& cb : subscribers_) {
            cb(event);
        }
    }

private:
    std::vector<Callback> subscribers_;
};
