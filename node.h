#ifndef NODE_H
#define NODE_H

#include <string>

using namespace std;

class TNode {

    public:
        // Propiedades de un nodo
        string ID;
        string Type;
        double Longitude;
        double Latitude;

        // Constructor del nodo
        TNode(string id, string type, double lgt, double lat);
};

#endif //NODE_H
