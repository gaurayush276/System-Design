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
#include<unordered_map>
#include<thread>
using namespace std ;
void func() {
    cout <<"HEllo" <<endl ;
}
int main () {

    thread t1(func) ;
    t1.join()  ; 
    thread t2(func) ; 
    t2.join() ; 

return 0 ;
}