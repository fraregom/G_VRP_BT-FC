#include <string>
#include <vector>
#include "Node.h"
using namespace std;

#ifndef PROYECTO_GVRP_AUXILIAR_H
#define PROYECTO_GVRP_AUXILIAR_H

const float EarthRadiusKm = 6378.0F;
vector<string> split_string(const string& str,
                                 const string& delimiter);

double get_radians(double degree);
double haversine_distance(TNode &A, TNode &B);

#endif //PROYECTO_GVRP_AUXILIAR_H
