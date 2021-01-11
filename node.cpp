#include "node.h"

/*!
 * Constructor de un nodo, este guardara la información referente a un nodo de la instancia.
 * @param id: Id del nodo (nombre)
 * @param type: Tipo del nodo (c o f o d)
 * @param lgt: Longitud donde se encuentra el nodo.
 * @param lat: Latitud donde se encuentra el nodo.
 */
TNode::TNode(string id, string type, double lgt, double lat) { // Constructor
    ID = std::move(id);
    Type = std::move(type);
    Longitude = lgt;
    Latitude = lat;
}


