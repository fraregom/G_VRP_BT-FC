#include "CSP_Algorithm.h"

tuple<double, double> costArc(struct IFWProblem info, double distance){

    double fuelUsed = info.r * distance;
    double timeUsed = distance / info.v;

    return make_tuple(fuelUsed, timeUsed);
}

void applyCharges (struct IFWProblem &info, double distance){

    tuple<double, double> costs = costArc(info, distance);
    info.Q -= get<0>(costs);
    info.TL -= get<1>(costs);
}

tuple<vector<int>, vector<int>, vector<int>> evaluatorFunction (struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
        vector<int> &clients, double best_tour_cost, int currPos){

    vector<int> worstNodes;
    vector<int> candidatesNodes;
    vector<int> AFSRequiredNodes;

    for(int client: clients){

        double distanceEval = distanceMatrix[currPos][client];
        tuple<double, double> costs = costArc(info, distanceEval);

        if(distanceEval < best_tour_cost) worstNodes.push_back(client);

        else if (info.Q > get<0>(costs) && info.TL > get<1>(costs)) candidatesNodes.push_back(client);

        else if (info.TL < get<1>(costs)) worstNodes.push_back(client);

        else AFSRequiredNodes.push_back(client);

    }

    return make_tuple(candidatesNodes, AFSRequiredNodes, worstNodes);
}

tuple <int, double> fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
                    vector<int> &AFS_nodes, int currPos, int finalPos){

    int indexAFS = 0;
    double nearestStation = DBL_MAX;

    if(AFS_nodes.empty()){
        return make_tuple(0, 0);
    }

    for(int AFS_node : AFS_nodes){
        double distanceTemp = distanceMatrix[currPos][AFS_node];

        if( distanceTemp < nearestStation){
            nearestStation = distanceTemp;
            indexAFS = AFS_node;
        }
    }

    double distanceTotal = nearestStation + distanceMatrix[currPos][finalPos];
    tuple<double,double> costs = costArc(info, distanceTotal);

    if( info.Q > get<0>(costs) && info.TL > get<1>(costs)) {

        AFS_nodes.erase(remove(AFS_nodes.begin(), AFS_nodes.end(), indexAFS),
                                AFS_nodes.end());

        return make_tuple(indexAFS, distanceTotal);
    }

    return make_tuple(0, 0);
}

/*void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}*/

/*vector<int> insertAFS(vector<int> &partial_tour, int index, int AFSIndex){

    vector<int> split_lo(partial_tour.begin(), partial_tour.begin() + index);
    split_lo.push_back(AFSIndex);

    vector<int> split_hi(partial_tour.begin() + index, partial_tour.end());

    vector<int> partial_tour2;
    split_lo.insert(split_lo.end(), split_hi.begin(), split_hi.end());

    partial_tour2 = split_lo;

    split_lo.clear();
    split_hi.clear();

    return partial_tour2;
}*/

void G_VRP_BT_FC(vector<vector<double>> &adjMatrix, IFWProblem info, vector<int> &clients, vector<int> &AFS_nodes,
                 vector<int> &best_tour, double &best_tour_cost, vector<int> &partial_tour, double &partial_tour_cost,
                 int level, int order, int &count)
{
    if (level == order-1) {
        double tour_cost = partial_tour_cost +
                           //+ adjMatrix[partial_tour[order - 2]][partial_tour[order - 1]]
                           + adjMatrix[partial_tour.back()][0]; //Revisar denuevo si es factible?

        if (best_tour_cost == 0 || tour_cost < best_tour_cost) {
            best_tour = partial_tour;
            best_tour_cost = tour_cost;
        }
    }

    else {

        //Instanciar un nodo y sacar los nodos vecinos al cual puede viajar
        int instance_node = partial_tour.back();

        tuple<vector<int>, vector<int>, vector<int>> results = evaluatorFunction(info, adjMatrix,
                                                                                 clients, best_tour_cost,
                                                                                 instance_node);

        for(int candidatesNode : get<0>(results)){

            double distanceTemp = adjMatrix[instance_node][candidatesNode];

            vector<int> clients_temp = clients;
            struct IFWProblem info_temp = info;

            applyCharges(info, distanceTemp);
            partial_tour_cost += distanceTemp;
            partial_tour.push_back(candidatesNode);
            clients.erase(remove(clients.begin(), clients.end(), candidatesNode),
                          clients.end());
            ++count;

            G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                        partial_tour, partial_tour_cost, level + 1, order, count);

            partial_tour_cost -= distanceTemp;
            partial_tour.pop_back();
            clients = clients_temp;
            info = info_temp;

        }

        for(int AFSRequiredNode : get<1>(results)){

            tuple<int,double> resultsTemp = fuelRecharge(info,adjMatrix,
                                                     AFS_nodes, instance_node, AFSRequiredNode);
            int AFS_index = get<0>(resultsTemp);
            double cost_temp = get<1>(resultsTemp);

            if(AFS_index > 0 && cost_temp > 0) {

                vector<int> clients_temp = clients;
                vector<int> partial_tour_temp = partial_tour;
                struct IFWProblem info_temp = info;

                applyCharges(info, cost_temp);

                partial_tour_cost += cost_temp;
                partial_tour.push_back(AFS_index);
                partial_tour.push_back(AFSRequiredNode);
                clients.erase(remove(clients.begin(), clients.end(), AFSRequiredNode),
                              clients.end());
                ++count;

                G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                            partial_tour, partial_tour_cost, level + 1, order + 1, count);

                partial_tour_cost -= cost_temp;
                partial_tour = partial_tour_temp;
                clients = clients_temp;
                info = info_temp;
            }

        }

        //Probar cada uno de los nodos que estan disponibles en clients con instance_node
        /*for(int client_index: clients){

            double distanceTemp = adjMatrix[instance_node][client_index];

            enum STATE status = evaluatorFunction(info, best_tour_cost, distanceTemp);

            switch (status) {

                case NO_GAS: {


                    tuple<int,double> results = fuelRecharge(info,adjMatrix,
                                                             AFS_nodes, instance_node, client_index);
                    int AFS_index = get<0>(results);
                    double cost_temp = get<1>(results);

                    if(AFS_index > 0 && cost_temp > 0){

                        vector<int> clients_temp = clients;
                        vector<int> partial_tour_temp = partial_tour;

                        partial_tour_cost += cost_temp;
                        partial_tour.push_back(AFS_index);
                        partial_tour.push_back(client_index);
                        clients.erase(remove(clients.begin(), clients.end(), client_index),
                                      clients.end());
                        ++count;

                        G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                                    partial_tour, partial_tour_cost, level + 1, order + 1, count);

                        partial_tour_cost -= cost_temp;
                        partial_tour = partial_tour_temp;
                        clients = clients_temp;

                    } else  continue;
                }

                case TIME_OUT:
                    continue;

                case BETTER_ROUTE: {

                            vector<int> clients_temp = clients;

                            partial_tour_cost += distanceTemp;
                            partial_tour.push_back(client_index);
                            clients.erase(remove(clients.begin(), clients.end(), client_index),
                            clients.end());
                            ++count;

                            G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                                        partial_tour, partial_tour_cost, level + 1, order, count);

                            partial_tour_cost -= distanceTemp;
                            partial_tour.pop_back();
                            clients = clients_temp;

                    }

                case WORST_ROUTE:
                    continue;
            }
        }*/
    }
}

double G_VRP(vector<vector<double>> &adjMatrix, vector<TNode> &nodes, struct IFWProblem &info, int order,
                          vector<int> &best_tour, int &count)
{
    double best_tour_cost = 0;
    double partial_tour_cost = 0;
    vector<int> clients;
    vector<int> AFS_nodes;
    vector<int> partial_tour;

    partial_tour.push_back(0); //Comenzamos en el nodo DEPOT (index 0)

    for (int i = 1; i < order; i++) {
        if(nodes[i].Type != "f"){
            clients.push_back(i);
        } else {
            AFS_nodes.push_back(i);
        }
    }

    /*clients.push_back(1);
    clients.push_back(2);
    clients.push_back(3);
    clients.push_back(4);
    clients.push_back(5);
    clients.push_back(6);

    AFS_nodes.push_back(7);
    AFS_nodes.push_back(8);
    AFS_nodes.push_back(9);*/

    G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                partial_tour, partial_tour_cost, 1, order, count);

    clients.clear();
    AFS_nodes.clear();
    partial_tour.clear();

    return best_tour_cost;
}


