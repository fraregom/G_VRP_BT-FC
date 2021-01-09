#ifndef PROYECTO_GVRP_CSP_ALGORITHM_H
#define PROYECTO_GVRP_CSP_ALGORITHM_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "functions.h"
#include "cfloat"
#include "tuple"

using namespace std;

enum STATE{
    NO_GAS,
    TIME_OUT,
    BETTER_ROUTE,
    WORST_ROUTE
};

vector<double> costArc(struct IFWProblem info, double distance);
enum STATE evaluatorFunction(struct IFWProblem &info, double best_tour_cost, double distanceEval);
tuple<int,double>  fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix, vector<int> &AFS_nodes, int currPos);

void swap(int *a, int *b);
vector<int> insertAFS(vector<int> &partial_tour, int index, int AFSIndex);

void G_VRP_BT_FC(vector<vector<double>> &adjMatrix, struct IFWProblem &info, int order,
                        vector<int> &best_tour, double *best_tour_cost, vector<int> &partial_tour, vector<int> &AFS_nodes,
                        double *partial_tour_cost, int level);
double G_VRP(vector<vector<double>> &adjMatrix, vector<TNode> &nodes, struct IFWProblem &info, int order,
                          vector<int> &best_tour, int &count);


#endif //PROYECTO_GVRP_CSP_ALGORITHM_H
