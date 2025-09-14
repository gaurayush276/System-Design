#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
using namespace std ; 

// Scenario

// Chef thread makes pizzas and puts them in a queue.

// Customer thread(s) wait for pizza.

// Mutex ensures safe access to the queue.

// Condition variable ensures customers wait efficiently until pizza is available.

mutex mtx;                    // protects pizza_queue
condition_variable cv;        // for waiting customers
queue<int> pizza_queue;

bool finished = false;             // flag to indicate chef is done

// Chef function
void chef() {
    for (int i = 1; i <= 5; ++i) {
        this_thread::sleep_for(chrono::seconds(1)); // making pizza
        {
            lock_guard<mutex> lock(mtx); // lock the queue
            pizza_queue.push(i);
            cout << "Chef made pizza " << i << "\n";
        }
        cv.notify_one(); // notify a waiting customer
    }
    // Indicate chef is done
    {
        lock_guard<mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all(); // wake all customers to finish
}

// Customer function
void customer(int id) {
    while (true) {
        unique_lock<mutex> lock(mtx);
        // Wait until there is pizza or chef finished
        // temporarily unlock so that chef can add pizza 
        cv.wait(lock, [](){ return !pizza_queue.empty() || finished; });

        if (!pizza_queue.empty()) {
            int pizza = pizza_queue.front();
            pizza_queue.pop();
            lock.unlock(); // unlock while eating
            cout << "Customer " << id << " got pizza " << pizza << "\n";
        } else if (finished) {
            // No more pizza and chef finished
            break;
        }
    }
    cout << "Customer " << id << " is done.\n";
}

int main() {
    thread chef_thread(chef);
    thread customer1(customer, 1);
    thread customer2(customer, 2);

    chef_thread.join();
    customer1.join();
    customer2.join();

    // if customer 2 get the pizza first ,it's okay it is all based on the os 
}
