#include <iostream>
#include <thread>
using namespace std;

void increment() {
    for (int i = 0; i < 5; i++) {
        cout << "Running in thread: " << i << endl;
    }
}

int main() {
    thread t1(increment);
    thread t2(increment);

    t1.join();
    t2.join();
}

// cl /EHSc Main.cpp
// ./Main.exe