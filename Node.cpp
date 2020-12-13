#include "Node.h"

TNode::TNode(string id, string type, float lgt, float lat) { // Constructor
    ID = id;
    Type = type;
    Longitude = lgt;
    Latitude = lat;
}

string TNode::Mostrar (){ // Métodos
    return ID;
};


