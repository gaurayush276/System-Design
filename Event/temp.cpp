#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib> // for rand()

using namespace std;

// ---------------- Event ----------------
enum class EventType { PRIORITY, LOGGING, ERROR };

struct Event {
    string id;
    string publisher;
    EventType type;
    string description;
    long long creationTime;

    Event(const string& pub, EventType t, const string& desc)
        : publisher(pub), type(t), description(desc)
    {
        id = to_string(rand()); // simple unique id
        creationTime = chrono::steady_clock::now().time_since_epoch().count();
    }
};

// ---------------- Subscription ----------------
struct Subscription {
    function<bool(const Event&)> precondition;
    function<void(const Event&)> handler;
    int retries;
    size_t currentIndex;

    Subscription(function<bool(const Event&)> pre,
                 function<void(const Event&)> h, int r)
        : precondition(pre), handler(h), retries(r), currentIndex(0) {}
};

// ---------------- KeyedExecutor ----------------
class KeyedExecutor {
    mutex mtx;

public:
    void execute(function<void()> task) {
        lock_guard<mutex> lock(mtx);
        thread t(task);
        t.detach(); // run asynchronously
    }
};

// ---------------- EventBus ----------------
class EventBus {
    unordered_map<string, vector<Event>> topics;
    unordered_map<string, unordered_map<string, Subscription>> pushSubs;
    unordered_map<string, unordered_map<string, Subscription>> pullSubs;
    KeyedExecutor executor;

public:
    // Publish event
    void publish(const string& topic, const Event& event) {
        topics[topic].push_back(event);
        notifyPushSubscribers(topic, event);
    }

    // Subscribe for push
    void subscribeForPush(const string& topic, const string& subscriber,
                          function<bool(const Event&)> precondition,
                          function<void(const Event&)> handler,
                          int retries = 3)
    {
        pushSubs[topic].emplace(subscriber, Subscription(precondition, handler, retries));
    }

    // Subscribe for pull
    void subscribeForPull(const string& topic, const string& subscriber,
                          function<bool(const Event&)> precondition)
    {
        pullSubs[topic].emplace(subscriber, Subscription(precondition, nullptr, 0));
    }

    // Poll for pull subscriber
    Event poll(const string& topic, const string& subscriber) {
        auto topicIt = pullSubs.find(topic);
        if (topicIt == pullSubs.end()) 
            return Event("none", EventType::LOGGING, "NO_EVENT");

        auto subIt = topicIt->second.find(subscriber);
        if (subIt == topicIt->second.end()) 
            return Event("none", EventType::LOGGING, "NO_EVENT");

        Subscription& sub = subIt->second;

        for (; sub.currentIndex < topics[topic].size(); ++sub.currentIndex) {
            Event& e = topics[topic][sub.currentIndex];
            if (sub.precondition(e)) {
                sub.currentIndex++;
                return e;
            }
        }

        return Event("none", EventType::LOGGING, "NO_EVENT");
    }

private:
    void notifyPushSubscribers(const string& topic, const Event& event) {
        auto itTopic = pushSubs.find(topic);
        if (itTopic == pushSubs.end()) return;

        auto& subs = itTopic->second;
       for (auto it = subs.begin(); it != subs.end(); ++it) {
    Subscription subCopy = it->second;

    if (subCopy.precondition(event)) {
        executor.execute([event, subCopy]() mutable {
            int attempts = 0;
            while (attempts < subCopy.retries) {
                try {
                    if (subCopy.handler) subCopy.handler(event);
                    break;
                } catch (...) {
                    attempts++;
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }
        });
    }
}

};

};

// ---------------- Example Usage ----------------
int main() {
    EventBus bus;

    // Push subscribers
    bus.subscribeForPush("STOCK_UPDATES", "Logger",
        [](const Event&) { return true; },
        [](const Event& e) { cout << "Logger: " << e.description << "\n"; });

    bus.subscribeForPush("STOCK_UPDATES", "Trader",
        [](const Event& e) { return e.description.find("Apple") != string::npos; },
        [](const Event& e) { cout << "Trader executes: " << e.description << "\n"; });

    // Pull subscriber
    bus.subscribeForPull("STOCK_UPDATES", "Analytics",
        [](const Event&) { return true; });

    // Publish events
    Event apple("Feed", EventType::PRIORITY, "Apple stock price 180");
    Event google("Feed", EventType::PRIORITY, "Google stock price 120");

    bus.publish("STOCK_UPDATES", apple);
    bus.publish("STOCK_UPDATES", google);

    // Polling
    this_thread::sleep_for(chrono::milliseconds(200)); // wait for async tasks
    Event polled = bus.poll("STOCK_UPDATES", "Analytics");
    cout << "Analytics polled: " << polled.description << "\n";

    this_thread::sleep_for(chrono::milliseconds(200)); // keep main alive for async
    return 0;
}
