#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
 
using namespace std ;

 class Event {
    public :
        string user ; 
        string description ;
        string type ; 

        Event(string _user , string _des , string _type) : user(_user) , description(_des) , type(_type) {}

        void print()const{
            cout <<description <<" " <<type << endl ;
        }

 };