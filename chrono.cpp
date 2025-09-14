#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void print(int delay) {
    this_thread::sleep_for(chrono::seconds(delay));
    cout << "Hello" << endl;
}

int main() {
    thread a(print , 1 );
    a.join();  // wait for a to finish completely

    thread b(print, 3 );
    b.join();  // then run b

    return 0;
} 