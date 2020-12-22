#ifndef PROYECTO_GVRP_NODE_H
#define PROYECTO_GVRP_NODE_H

#include <string>
using namespace std;

class TNode {

    public:
        string ID;         // Propiedades
        string Type;
        double Longitude;
        double Latitude;

        TNode(string id, string type, double lgt, double lat); // Constructor
        string Mostrar(); //Metodos
};

#endif //PROYECTO_GVRP_NODE_H
