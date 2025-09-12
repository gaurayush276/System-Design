#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <stdexcept>
using namespace std ; 
class Node {
public:
    string id;
    int weight;

    Node(string id_, int weight_ = 1) : id(id_), weight(weight_) {}
    // Node ( string id , int weight = 1 ) {
    //     this->id = id ;
    //     this->wight = weight ; 
    // }
};

class Request {
public:
    string id;

    Request(string id_) : id(id_) {}
};

class ConsistentHashing {
private:
// nodePositions → stores all “virtual points” for each node.
    map<Node*, vector<long>> nodePositions;
    // nodeMappings → ordered mapping of each point to its corresponding node.
    map<long, Node*> nodeMappings;
    // hashFunction → converts a string (request ID or virtual point) into a numeric hash.
    function<long(const string&)> hashFunction;
    int pointMultiplier;

public:
    ConsistentHashing(function<long(const string&)> hashFunc, int multiplier)
        : hashFunction(hashFunc), pointMultiplier(multiplier) {
        if (pointMultiplier == 0) throw invalid_argument("pointMultiplier cannot be 0");
    }

    void addNode(Node* node) {
        nodePositions[node] = vector<long>();
        for (int i = 0; i < pointMultiplier; i++) {
            for (int j = 0; j < node->weight; j++) {
                long point = hashFunction(to_string(i * pointMultiplier + j) + node->id);
                nodePositions[node].push_back(point);
                nodeMappings[point] = node;
            }
        }
    }

    void removeNode(Node* node) {
        auto it = nodePositions.find(node);
        if (it != nodePositions.end()) {
            for (long point : it->second) {
                nodeMappings.erase(point);
            }
            nodePositions.erase(it);
        }
    }

    Node* getAssignedNode(Request* request) {
        long key = hashFunction(request->id);
        auto it = nodeMappings.upper_bound(key); // higherEntry equivalent
        if (it == nodeMappings.end()) {
            return nodeMappings.begin()->second;
        } else {
            return it->second;
        }
    }
};

// Example hash function

// 31 (or another larger prime, like 131) = better for production-grade hashing.
long simpleHash(const string& s) {
    long hash = 0;
    for (char c : s) hash = hash * 31 + c;
    return hash;
}

// Example usage
int main() {
    ConsistentHashing ch(simpleHash, 3);
    
    Node n1("Node1", 1);
    Node n2("Node2", 1);

    ch.addNode(&n1);
    ch.addNode(&n2);

    Request r1("RequestA");
    Request r2("RequestB");

    cout << "RequestA assigned to: " << ch.getAssignedNode(&r1)->id << endl;
    cout << "RequestB assigned to: " << ch.getAssignedNode(&r2)->id << endl;

    return 0;
}


// 🔹 What you said (and the tweaks)

// Nodes & Requests

// Node = server (with an ID and weight).

// Request = some incoming job (with an ID).
// ✅ Correct.

// nodePositions

// map<Node*, vector<long>> nodePositions;


// For each real node, we keep a list of its virtual positions on the ring.

// Example: Node1 might live at positions [123, 456, 789].
// ✅ Correct.

// nodeMappings

// map<long, Node*> nodeMappings;


// A sorted map of every virtual point → real node.

// Used to search clockwise with upper_bound().
// ✅ Correct.

// hashFunction

// function<long(const string&)> hashFunction;


// Takes a string ("RequestA", "Node1", etc.), turns it into a number, places it on the ring.
// ✅ Correct.

// When you call

// ConsistentHashing ch(simpleHash, 3);


// This does not yet build the ring.

// It just sets up:

// hashFunction = simpleHash

// pointMultiplier = 3

// 👉 The ring is actually populated only when you call addNode().

// Example:

// ch.addNode(&n1);

// places n1 at 3 * weight virtual points on the ring.

// 🔑 So:

// ConsistentHashing ch(simpleHash, 3); = “create a system that knows how to hash strings, and will give each node 3 virtual points.”

// addNode() = “actually put the node(s) on the ring at those positions.”

// getAssignedNode(request) = “given a request, hash it, find next clockwise node, and return that server.”

// ⚡ So you’re 95% correct — the only small correction:
// 👉 The constructor (ConsistentHashing ch(simpleHash, 3)) does not yet make the ring. It just sets the rules.
// 👉 The ring is built when you call addNode() for each node.