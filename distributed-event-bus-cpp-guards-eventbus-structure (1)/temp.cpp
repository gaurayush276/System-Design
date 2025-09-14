#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cmath>
#include<map>
#include<functional>
 
using namespace std ;

// server 
class Node {
    public :
    string id ; 
    int weight ;
    Node ( string _id , int _wt ) : id(_id) , weight(_wt) { }
} ; 
// Request 
class Request { 
     public :
    string id ; 
    Request ( string _id ) : id(_id) {}
} ; 

class ConsistentHashing {
    public :
    // make the virtual servers 
    map < Node * , vector < long > > virtualServers ; 
    map<long , Node * >  requestToServer ; 
    function<long(const string &)>hashFunction ;
    int multiplier ; 

    ConsistentHashing(function <long(const string &)>hashFunc , int mul ): hashFunction(hashFunc) , multiplier(mul) {}

    void addServers ( Node * server ) {
        virtualServers[server]= vector < long >() ;
        for ( int i =0;i < multiplier ;i++ ) {
            for ( int j = 0 ; j < server->weight ;j++ ) {
                long point = hashFunction(to_string(i*multiplier + j )+ server->id) ; 
                virtualServers[server].push_back(point) ;
                requestToServer[point] = server ; 
            }
        }
    }

    Node * getResponse (Request * req ) {
        long key = hashFunction( req->id ) ; 
         auto it = requestToServer.upper_bound(key) ; 
         if ( it ==  requestToServer.end())
         return requestToServer.begin()->second ; 
         else 
         return it->second ; 
    } 
} ; 

int main () {
    hash<string > hashMaker; 
    ConsistentHashing ch (hashMaker , 3  ) ;

    Node n1 ("user100" , 1) ;
    Node n2 ("user400" , 1) ;
    Node n3 ("f,dsns" , 1) ;
    Node n4 ("4544ete" , 1) ;

    ch.addServers(&n1) ; 
    ch.addServers(&n2) ;
    ch.addServers(&n3) ;
    ch.addServers(&n4) ;
    
    Request r1("ayush") ; 
    Request r2("peter") ; 

    cout << ch.getResponse(&r1)->id <<endl ;
    cout << ch.getResponse(&r2)->id <<endl ;


return 0 ;
}