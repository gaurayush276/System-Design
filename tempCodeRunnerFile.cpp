#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cmath>
#include<map>
#include<stack>
#include<queue>
#include<set>
#include<unordered_set>
#include<thread>
#include<chrono>
using namespace std ;

void print() {
    this_thread::sleep_for(chrono::seconds(1)) ;
    cout << "Hello"  <<endl ;
}
int main () {

    thread a(print) ; 
    thread b(print) ; 


return 0 ;
}