#include "node.h"

/*!
 *
 * @param id
 * @param type
 * @param lgt
 * @param lat
 */
TNode::TNode(string id, string type, double lgt, double lat) { // Constructor
    ID = std::move(id);
    Type = std::move(type);
    Longitude = lgt;
    Latitude = lat;
}


