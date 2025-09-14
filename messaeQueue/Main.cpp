#include <iostream>
#include <thread>
#include <chrono>
#include "MessageQueue.h"

using namespace std;

void producer(MessageQueue& mq, const string& sender) {
    for (int i = 1; i <= 5; ++i) {
        mq.push(Message(sender, "Message " + to_string(i)));
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void consumer(MessageQueue& mq, const string& name) {
    for (int i = 0; i < 5; ++i) {
        Message msg = mq.pop();
        cout << name << " consumed: ";
        msg.print();
    }
}

int main() {
    MessageQueue mq;

    thread p1(producer, ref(mq), "Producer1");
    thread p2(producer, ref(mq), "Producer2");

    thread c1(consumer, ref(mq), "Consumer1");
    thread c2(consumer, ref(mq), "Consumer2");

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}
