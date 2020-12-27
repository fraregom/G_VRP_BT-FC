#include "node.h"

TNode::TNode(string id, string type, double lgt, double lat) { // Constructor
    ID = std::move(id);
    Type = std::move(type);
    Longitude = lgt;
    Latitude = lat;
}

string TNode::Mostrar (){ // Métodos
    return ID;
};


