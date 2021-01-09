#include "CSP_Algorithm.h"

vector<double> costArc(struct IFWProblem info, double distance){

    vector<double> result;

    double fuelUsed = info.r * distance;
    double timeUsed = distance / info.v;

    result.push_back(fuelUsed);
    result.push_back(timeUsed);

    return result;
}

enum STATE evaluatorFunction(struct IFWProblem &info, double best_tour_cost, double distanceEval){

    vector<double> costs = costArc(info, distanceEval);

    if(distanceEval < best_tour_cost) return WORST_ROUTE;

    if( info.Q > costs[0] && info.TL > costs[1]){

        info.Q -= costs[0];
        info.TL -= costs[1];

        return BETTER_ROUTE;

    } else if (info.TL < costs[1]) return TIME_OUT;

    return NO_GAS;
}

tuple<int,double> fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
                    vector<int> &AFS_nodes, int currPos){

    int indexAFS = 0;
    double nearestStation = DBL_MAX;
    for(int gasStation : AFS_nodes){
        int distanceTemp = distanceMatrix[currPos][gasStation];

        if( distanceTemp < nearestStation){
            nearestStation = distanceTemp;
            indexAFS = gasStation;
        }
    }

    vector<double> costs = costArc(info, nearestStation);

    if( info.Q > costs[0] && info.TL > costs[1]) {
        info.Q = 60;
        info.TL -= costs[1];

        return make_tuple(indexAFS, nearestStation);
    }

    return make_tuple(0, 0);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

vector<int> insertAFS(vector<int> &partial_tour, int index, int AFSIndex){

    vector<int> split_lo(partial_tour.begin(), partial_tour.begin() + index);
    split_lo.push_back(AFSIndex);

    vector<int> split_hi(partial_tour.begin() + index, partial_tour.end());

    vector<int> partial_tour2;
    split_lo.insert(split_lo.end(), split_hi.begin(), split_hi.end());

    partial_tour2 = split_lo;

    split_lo.clear();
    split_hi.clear();

    return partial_tour2;
}

void G_VRP_BT_FC(vector<vector<double>> &adjMatrix, IFWProblem info, int order,
                 vector<int> &best_tour, double *best_tour_cost, vector<int> &partial_tour,
                 vector<int> &AFS_nodes, double *partial_tour_cost, int level, int &count)
{
    if (level == order-1) {
        double tour_cost = *partial_tour_cost
                           + adjMatrix[partial_tour[order - 2]][partial_tour[order - 1]]
                           + adjMatrix[partial_tour[order - 1]][0];

        if (*best_tour_cost == 0 || tour_cost < *best_tour_cost) {
            best_tour = partial_tour;
            *best_tour_cost = tour_cost;
        }
    }

    else {

        for (int i = level; i < order; i++) {

            double distanceTemp = *partial_tour_cost + adjMatrix[partial_tour[level - 1]][partial_tour[i]];
            enum STATE status = evaluatorFunction(info, *best_tour_cost, distanceTemp);

            if(*best_tour_cost == 0 || status == BETTER_ROUTE)
            {

                swap(&partial_tour[level], &partial_tour[i]);

                double cost = adjMatrix[partial_tour[level - 1]][partial_tour[level]];
                *partial_tour_cost += cost;
                ++count;

                G_VRP_BT_FC(adjMatrix, info, order, best_tour, best_tour_cost,
                            partial_tour, AFS_nodes, partial_tour_cost, level + 1, count);

                *partial_tour_cost -= cost;
                swap(&partial_tour[level], &partial_tour[i]);
            }
            else if (status == NO_GAS){

                swap(&partial_tour[level], &partial_tour[i]);

                tuple<int,double> results = fuelRecharge(info,adjMatrix,AFS_nodes, partial_tour[level - 1]);

                double cost =  get<1>(results);
                *partial_tour_cost += cost;
                ++count;

                vector<int> partial_tour2 = insertAFS(partial_tour, partial_tour[level - 1], get<0>(results));

                G_VRP_BT_FC(adjMatrix, info, order + 1, best_tour, best_tour_cost,
                            partial_tour2, AFS_nodes,partial_tour_cost, level + 1, count);

                *partial_tour_cost -= cost;
                swap(&partial_tour[level], &partial_tour[i]);

            }
        }
    }
}

double G_VRP(vector<vector<double>> &adjMatrix, vector<TNode> &nodes, struct IFWProblem &info, int order,
                          vector<int> &best_tour, int &count)
{
    double best_tour_cost = 0;
    double partial_tour_cost = 0;
    vector<int> clients;
    vector<int> AFS_nodes;

    for (int i = 0; i < order; i++) {
        if(nodes[i].Type != "f"){
            clients.push_back(i);
        } else {
            AFS_nodes.push_back(i);
        }
    }

    /*clients.push_back(0);
    clients.push_back(1);
    clients.push_back(2);
    clients.push_back(3);
    clients.push_back(4);
    clients.push_back(5);
    clients.push_back(6);

    AFS_nodes.push_back(7);
    AFS_nodes.push_back(8);
    AFS_nodes.push_back(9);*/

    G_VRP_BT_FC(adjMatrix, info, clients.size(), best_tour, &best_tour_cost, clients, AFS_nodes,
                &partial_tour_cost, 1, count);

    clients.clear();
    AFS_nodes.clear();

    return best_tour_cost;
}