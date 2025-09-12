#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<functional>
using namespace std ;

class Node {
    public :
    string id ;
    int weight ;
    Node (string _id , int _weight  ) : id(_id) , weight ( _weight ) {}
} ; 

class Request {
    public :
    string id ; 
    Request ( string _id) : id (_id) {}
} ;
class ConsistentHashing {
    public :
    // map the nodes to their virtual server positions 
    map< Node * , vector <long > > nodePositions ;
    // when the request is resolved by getting mapped with one of the server 
    map < long , Node * > requestToServer ; 
    function <long (const string & ) > hashFuction ; 
    int multiplier ;

    ConsistentHashing ( function<long(const string &)> hashFunction_ ,  int mul)  : hashFuction(hashFunction_) ,multiplier (mul) {}


    void addServer ( Node * server ){
        // because we need to make 3 virtual servers 
        nodePositions[server] = vector< long > () ; 
        for ( int i = 0 ; i < 3 ; i++ ) {
            for ( int j = 0 ;j< server->weight ;j++ ){
                long createHash = hashFuction(to_string(i * multiplier + j ) + server->id) ; 
                nodePositions[server].push_back(createHash) ;
                requestToServer[createHash] = server ; 
            }
        }
    }

    Node * getAssignedServer ( Request * req ) {
        long positon_of_req = hashFuction(req->id) ;
        auto it = requestToServer.upper_bound(positon_of_req) ;
        if ( it == requestToServer.end())
        return requestToServer.begin()->second ; 
        else 
        return it->second ; 
    }

    
    
} ;

// long hashMaker ( string str )  {
//     long hash = 0 ; 
//     for ( auto i : str) 
//         hash  = hash * 31 + i ; 
    
//     return hash ;
// }
int main () {
    // making the body ; 

    hash< string > hashMaker ; 
    ConsistentHashing ch (hashMaker , 6 ) ; 
    Node n1("user100" , 1) ;
    Node n2( "user234" , 1) ;
    Node n3( "user355" , 1) ;
    Node n4( "user466" , 1) ;

    ch.addServer ( &n1 ) ;
    ch.addServer ( &n2 ) ;
    ch.addServer ( &n3 ) ;
    ch.addServer ( &n4 ) ;

    Request r1 ("ayush") ;
    Request r2 ("peter") ;

    cout << "R1 is assigned to :" << ch.getAssignedServer(&r1)->id <<endl ;
    cout << "R2 is assigned to :" << ch.getAssignedServer(&r2)->id ;

return 0 ;
}