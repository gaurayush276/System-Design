#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
#include<string>
 #include"Event.h"
using namespace std ;
 

class EventBus{
        public : 
        // so it the list of function by which the Event can pass by
        vector < function<void(const Event &)> > subscribers ; 
        void subscribe(function<void(const Event &)> Callback ){
            subscribers.push_back(Callback) ;
        }
        void publish(const Event & event){
            for ( auto i : subscribers)
            i(event) ; 
        }
 
} ; 