#ifndef CSP_ALGORITHM_H
#define CSP_ALGORITHM_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "functions.h"
#include "cfloat"
#include "tuple"

using namespace std;

/*
 * Funciones utilizadas en CSP_Algorithm.
 */
tuple<double, double> costArc(struct IFWProblem info, double distance);

bool validateTravel(struct IFWProblem &info, double distance);

tuple<vector<int>, vector<int>, vector<int>> evaluatorFunction (struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
                   vector<int> &clients, double best_tour_cost, double partial_tour_cost, int currPos);

tuple <int, double> fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
                                 vector<int> &AFS_nodes, int currPos);

void G_VRP_BT_FC(vector<vector<double>> &adjMatrix, IFWProblem &info, vector<int> &clients, vector<int> &AFS_nodes,
                 vector<int> &best_tour, double &best_tour_cost, vector<int> &partial_tour, double &partial_tour_cost,
                 unsigned int &count, unsigned int &numIteracion);

double G_VRP(vector<vector<double>> &adjMatrix, vector<TNode> &nodes, struct IFWProblem &info,
                          vector<int> &best_tour, unsigned int &count, unsigned int &numIteracion);


#endif //CSP_ALGORITHM_H
