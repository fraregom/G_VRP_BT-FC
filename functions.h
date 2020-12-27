#ifndef PROYECTO_GVRP_AUXILIAR_H
#define PROYECTO_GVRP_AUXILIAR_H

#include <string>
#include <vector>
#include <cmath>
#include "node.h"

using namespace std;

const double EarthRadius = 4182.44949;
typedef vector <TNode> NodeList;
typedef vector <vector<double>> DistanceMatrix;

struct IFWProblem
{
    float Q;
    float r;
    float TL;
    float v;
    float m;
};

vector<string> split_string(const string& str,
                                 const string& delimiter);

double to_radians(double degree);
double haversine_distance(TNode &sNode, TNode &eNode);
vector<vector<double>> distance_matrix(vector<TNode> &nodes);

#endif //PROYECTO_GVRP_AUXILIAR_H
