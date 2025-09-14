#include "EventBus.h"

// so event bus is like a platform and subscriber are the users when any event publishes then it get brodcasted to each user
int main() {
    EventBus bus;

    // Subscriber 1
    
      // Defining a lambda
    // auto res = [](int x) {
    //     return x + x;
    // };
    // cout << res(5);
    bus.subscribe([](const Event& e){
        std::cout << "Subscriber 1 received: ";
        e.print();
    });

    // Subscriber 2
    bus.subscribe([](const Event& e){
        std::cout << "Subscriber 2 received: ";
        e.print();
    });

    // Publish some events
    Event e1("SensorA", EventType::INFO, "Temperature normal");
    Event e2("SensorB", EventType::WARNING, "Temperature high");

    bus.publish(e1);
    bus.publish(e2);
    
//  ^ this make an event collection in which when is collection's blueprint is made the array of function that takes string and return the void get built then we insert the event in that collection 
    return 0;
}

