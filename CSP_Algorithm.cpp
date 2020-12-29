#include "CSP_Algorithm.h"

vector<double> costArc(struct IFWProblem info, double distance){

    vector<double> result;

    double fuelUsed = info.r * distance;
    double timeUsed = distance / info.v;

    result.push_back(fuelUsed);
    result.push_back(timeUsed);

    return result;
}

enum STATE evaluator(struct IFWProblem &info, double distance){

    vector<double> costs = costArc(info, distance);

    if( info.Q > costs[0] && info.TL > costs[1]){
        info.Q -= costs[0];
        info.TL -= costs[1];
        return SUCCESS;
    } else {
        if(info.TL < costs[1]) return TIME_OUT;

        return NO_GAS;
    }
}

double fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
        vector<int> &gasStations, int currPos){

    double nearestStation = DBL_MAX;
    for(int gasStation : gasStations){
        nearestStation = min (nearestStation, distanceMatrix[currPos][gasStation]);
    }

    vector<double> costs = costArc(info, nearestStation);

    if( info.Q > costs[0] && info.TL > costs[1]) {
        info.Q = 60;
        info.TL -= costs[1];
        return nearestStation;
    }

    return 0;
}

tuple<double,vector<int>> shortest_path(IFWProblem info,vector<TNode> &nodesArray,
                                        vector<vector <double>> &distanceMatrix)
{
    /// Picking a source city
    int source = 0;

    double shortest_path = DBL_MAX;
    vector<int> bestTour;
    vector<int> clients;
    vector<int> gasStations;

    /// pushing the rest num_nodes-1 cities into a bundle
    for(int i=0;i<nodesArray.size();i++)
    {
        if(i != source && nodesArray[i].Type != "f")
        {
            clients.push_back(i);
        } else {
            gasStations.push_back(i);
        }
    }

    /// generating permutations and tracking the minimum cost
    while(next_permutation(clients.begin(),clients.end()))
    {
        double path_weight = 0;
        int j = source;

        IFWProblem aux = info;
        bool error = false;

        for (int client : clients)
        {
            double actual_distance = distanceMatrix[j][client];
            enum STATE state = evaluator(aux,actual_distance);

            switch(state){
                case NO_GAS:
                    {
                        cout << "NO GAS" << endl;
                        for (int client2: clients){
                            cout << nodesArray[client2].ID;
                        }
                        cout << endl;
                        double distanceToRecharge = fuelRecharge(aux, distanceMatrix, gasStations, j);
                        if (distanceToRecharge > 0){
                            path_weight += actual_distance + distanceToRecharge;
                            j = client;
                        } else {
                            break;
                        }
                    }
                case TIME_OUT:
                    cout << "Time OUT" << endl;
                    for (int client2: clients){
                        cout << nodesArray[client2].ID;
                    }
                    cout << endl;
                    error = true;
                    break;
                case SUCCESS:
                    path_weight += actual_distance;
                    j = client;
            }

        }

        if(!error) {
            path_weight += distanceMatrix[j][source];
            if (path_weight < shortest_path) {
                bestTour = clients;
                shortest_path = path_weight;
            }
        }
    }
    return make_tuple(shortest_path, bestTour);
}