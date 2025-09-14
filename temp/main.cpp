#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cmath>
#include<map>
#include"EventBus.h"
using namespace std ;

int main () {

     EventBus bus ; 
     bus.subscribe([](const Event & e){
        cout << "user123 "  ;
        e.print() ; 
     }) ;
     bus.subscribe([](const Event & e){
        cout << "user1243 " ; 
        e.print() ; 
     }) ;
     Event e1 ( "user123" , "The match is started " , "Notification") ; 
     Event e2 ( "user1243" , "The water level above the danger line" , "Warning") ; 
     bus.publish(e1) ; 
     bus.publish(e2) ; 

return 0 ;
}