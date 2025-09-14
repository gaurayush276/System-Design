#pragma once
#include "Message.h"
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace  std ;
class MessageQueue {
public:
    void push(const Message& msg) {
        std::unique_lock<mutex> lock(mtx);
        q.push(msg);
        cv.notify_one(); // notify waiting consumers
    }

    Message pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this](){ return !q.empty(); }); // wait until not empty
        Message msg = q.front();
        q.pop();
        return msg;
    }

    bool empty() {
        unique_lock<mutex> lock(mtx);
        return q.empty();
    }

private:
    queue<Message> q;
    mutex mtx;
    condition_variable cv;
};
