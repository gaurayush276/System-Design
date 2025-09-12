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
};

class Request {
public:
    string id;

    Request(string id_) : id(id_) {}
};

class ConsistentHashing {
private:
    map<Node*, vector<long>> nodePositions;
    map<long, Node*> nodeMappings;
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
