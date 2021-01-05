//
// Created by Francisco Reyes Gómez on 29-12-20.
//

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
    SUCCESS
};

vector<double> costArc(struct IFWProblem info, double distance);
enum STATE evaluator(struct IFWProblem &info, double distance);
double fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix, vector<int> &gasStations, int currPos);
/*tuple<double,vector<int>> shortest_path(IFWProblem info, vector<TNode> &nodeArray,
                                        vector<vector <double>> &distanceMatrix);*/

void swap(int *a, int *b);
void G_VRP_Backtracking(vector<vector<double>> &adjMatrix, int order,
                        vector<int> &best_tour, double *best_tour_cost, vector<int> &partial_tour,
                        double *partial_tour_cost, int level);
double G_VRP(vector<vector<double>> &adjMatrix, int order,
                          vector<int> &best_tour, int &count);


#endif //PROYECTO_GVRP_CSP_ALGORITHM_H
