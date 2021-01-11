#include "CSP_Algorithm.h"

/*!
 * Función encargada de calcular el coste (tiempo, combustible) asociado a una distancia.
 * @param info: estructura que registra y almacena la información referente al vehículo.
 * @param distance: variable con la distancia a calcular.
 * @return tuple<double, double>: Una tupla con los resultados (fuelUsed, timeUsed).
 */
tuple<double, double> costArc(struct IFWProblem info, double distance){

    double fuelUsed = info.r * distance;
    double timeUsed = distance / info.v;

    return make_tuple(fuelUsed, timeUsed);
}

/*!
 * Función encargada de aplicar "cargos" (restar combustible, tiempo) al vehículo según una distancia dada.
 * @param info: estructura que registra y almacena la información referente al vehículo.
 * @param distance: variable con la distancia para aplicar cargos.
 */
void applyCharges (struct IFWProblem &info, double distance){

    tuple<double, double> costs = costArc(info, distance);
    info.Q -= get<0>(costs);
    info.TL -= get<1>(costs);

}

/*!
 * Función encargada de revisar si un viaje es realizable con las actuales condiciones del vehículo.
 * (Estanque de combustible, tiempo restante).
 * @param info: estructura que registra y almacena la información referente al vehículo.
 * @param distance: variable con la distancia a válidar.
 * @return bool: Retorna verdadero si es posible hacer el viaje o viceversa.
 */
bool validateTravel(struct IFWProblem &info, double distance){

    tuple<double, double> costs = costArc(info, distance);

    if (info.Q > get<0>(costs) && info.TL > get<1>(costs)) return true;

    return false;

}

/*!
 * La función evaluatorFunction, se encargara de filtrar los nodos que se pueden utilizar (candidatesNodes) según una
 * instancia dada. En caso que los nodos no se puedan utilizar debido a que no se pueden visitar debido a que no hay
 * suficiente combustible se agregan a AFSRequiredNodes y por último si al utilizar estos nodos la solución empeora o
 * no alcalza el tiempo se agregan a worstNodes.
 * @param info: estructura que registra y almacena la información referente al vehículo.
 * @param distanceMatrix: Vector 2D con las distancias entre nodos (Arcos).
 * @param clients: vector con los indices de los clientes a analizar (que están disponibles/sin visitar en ese momento)
 * @param best_tour_cost: variable que almacena el coste del mejor tour hasta ese momento.
 * @param partial_tour_cost: variable que almacena el coste que esta tomando el tour hasta ese momento.
 * @param currPos: Indice del nodo/cliente instanciado en ese momento el cual revisará a sus vecinos.
 * @return tuple<vector<int>, vector<int>, vector<int>>: Cada arreglo contendra los nodos/clientes correspondientes.
 */
tuple<vector<int>, vector<int>, vector<int>> evaluatorFunction (struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
        vector<int> &clients, double best_tour_cost, double partial_tour_cost, int currPos){

    vector<int> worstNodes;
    vector<int> candidatesNodes;
    vector<int> AFSRequiredNodes;

    /*Según el nodo instanciado (cliente) se revisan los vecinos (que no han sido visitados) y se agregan a la
     * categoría que corresponda.
     */
    for(int client: clients){

        double distanceEval = distanceMatrix[currPos][client];
        tuple<double, double> costs = costArc(info, distanceEval);

        if(((distanceEval + partial_tour_cost) > best_tour_cost) && best_tour_cost != 0) worstNodes.push_back(client);

        else if (info.Q > get<0>(costs) && info.TL > get<1>(costs)) candidatesNodes.push_back(client);

        else if (info.TL <= get<1>(costs)) worstNodes.push_back(client);

        else AFSRequiredNodes.push_back(client);

    }

    return make_tuple(candidatesNodes, AFSRequiredNodes, worstNodes);
}


/*!
 * Función encargada de encontrar la estación de servicio más cercana, y en caso de encontrarlo recarga el combustible
 * como también permite agregar el nodo AFS al tour parcial.
 * @param info: estructura que registra y almacena la información referente al vehículo.
 * @param distanceMatrix: Vector 2D con las distancias entre nodos (Arcos).
 * @param AFS_nodes: Vector que contiene la información referente a la posición (indices) de las estaciones de servicio.
 * @param currPos: Indice del nodo cliente que tiene problemas y necesita ir a una estación de servicio.
 * @return tuple <int, double>: Retorna el indice de la estación de servicio encontrado como también la distancia que
 * se de recorrio para llegar allí.
 */
tuple <int, double> fuelRecharge(struct IFWProblem &info, vector<vector<double>> &distanceMatrix,
                    vector<int> &AFS_nodes, int currPos){

    //Se almacena el indice de la estación encontrada como también la distancia.
    int indexAFS = 0;
    double nearestStation = DBL_MAX;

    //Si no hay estaciones de servicio disponibles se devuelve (0,0) indicando que no hay forma de recargar.
    if(AFS_nodes.empty()){
        return make_tuple(0, 0);
    }

    //Se revisan todas las estaciones de servicio disponible (según su distancia) al nodo en problemas.
    for(int AFS_node : AFS_nodes){
        double distanceTemp = distanceMatrix[currPos][AFS_node];

        if( distanceTemp < nearestStation){
            nearestStation = distanceTemp;
            indexAFS = AFS_node;
        }
    }

    tuple<double,double> costs = costArc(info, nearestStation);

    //Se revisa que sea factible recargar el combustible en dicha estación.
    if( info.Q > get<0>(costs) && info.TL > get<1>(costs)) {

        info.Q = 50;
        info.TL -= get<1>(costs);

        AFS_nodes.erase(remove(AFS_nodes.begin(), AFS_nodes.end(), indexAFS),
                                AFS_nodes.end());

        return make_tuple(indexAFS, nearestStation);
    }

    return make_tuple(0, 0);
}


/*!
 * Función recursiva (Backtracking) que se encarga de instanciar nodos, esto lo hace sacando el último valor de la
 * solución parcial (parte en 0 (depot)) para luego usando la evaluatorFunction (Aplicamos Forward Checking), elegir cual
 * nodo será parte de la solución parcial. Cuando ya no hayan más vecinos que instanciar, se da por entender que el algoritmo
 * termino y ya estaria en condiciones de entregar una solución. (Ya sea peor o mejor, siempre se elige la mejor).
 *
 * @param adjMatrix: Vector 2D con las distancias entre nodos (Arcos).
 * @param info: Estructura que registra y almacena la información referente al vehículo.
 * @param clients: Vector con los indices de los clientes a instanciar (que están disponibles/sin visitar en un momento dado.)
 * @param AFS_nodes: Vector con las indices de los estaciones de servicio (que están disponibles/sin visitar en un momento dado.)
 * @param best_tour: El vector con los indices (clientes y/o AFS) correspondientes a la mejor solución que se tiene en un momento dado.
 * @param best_tour_cost: El coste del mejor tour que se tiene en un momento dado.
 * @param partial_tour: El vector con los indices (clientes y/o AFS) correspondientes a solución parcial.
 * @param partial_tour_cost: El coste que esta tomando el tour en un momento dado.
 * @param count: Contador que lleva el número de llamadas recursivas que esta llevando el algoritmo.
 * @param numIteracion: Variable que almacena el número de llamadas recursivas que tomo una solución en particular.
 */
void G_VRP_BT_FC(vector<vector<double>> &adjMatrix, IFWProblem &info, vector<int> &clients, vector<int> &AFS_nodes,
                 vector<int> &best_tour, double &best_tour_cost, vector<int> &partial_tour, double &partial_tour_cost,
                 unsigned int &count, unsigned int &numIteracion)
{

    //Si el vector de clientes esta vacio, quiere decir que no hay más nodos que instanciar y por ende ya se tiene una solución.
    if (clients.empty()) {

        double tour_cost = partial_tour_cost +
                           + adjMatrix[partial_tour.back()][0];

        /*Se válida que se pueda realizar un último viaje al nodo inicial (depot) y si la solución es mejor
         * (y se guardan los resultados correspondientes).
         */
        if (best_tour_cost == 0 || (tour_cost < best_tour_cost && validateTravel(info, tour_cost))) {

            applyCharges(info, tour_cost);
            best_tour = partial_tour;
            best_tour_cost = tour_cost;
            numIteracion = count;
        }

        return;
    }

    else {

        //Instancianciamos el último nodo de la solución partial y revisamos su vecindad.
        int instance_node = partial_tour.back();

        tuple<vector<int>, vector<int>, vector<int>> results = evaluatorFunction(info, adjMatrix,
                                                                                 clients, best_tour_cost,
                                                                                 partial_tour_cost,
                                                                                 instance_node);

        /*En caso de existir nodos viables entramos aqui (revisamos todos). Calculamos la distancia que nos toma viajar hacia alli y los
         * costes asociados. Una vez realizado esto, agregamos este candidato a la solución parcial y lo eliminamos como posible
         * elección en un futuro.
         */
        for(int candidatesNode : get<0>(results)){

            double distanceTemp = adjMatrix[instance_node][candidatesNode];

            vector<int> clients_temp = clients;
            struct IFWProblem info_temp = info;

            //Aplicamos los cargos de combustible y tiempo correspondientes.
            applyCharges(info, distanceTemp);

            /*Agregamos la distancia que nos tomo al partial_tour_cost como también el nodo cliente que
            * corresponda.
            */
            partial_tour_cost += distanceTemp;
            partial_tour.push_back(candidatesNode);
            clients.erase(remove(clients.begin(), clients.end(), candidatesNode),
                          clients.end());
            ++count;

            G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                        partial_tour, partial_tour_cost, count, numIteracion);

            // Restauramos los valores para poder explorar más nodos.
            partial_tour_cost -= distanceTemp;
            partial_tour.pop_back();
            clients = clients_temp;
            info = info_temp;
        }

        /*En caso de existir nodos que necesitan una estación de servicio para alcanzarlos entramos aqui (revisamos todos).
         * Buscamos la estación de servicio más cercana, recargamos y válidamos que el viaje es posible (A->F->C).
         * Una vez realizado esto, agregamos el nodo AFS y el nodo cliente de destino a la solución parcial y los eliminamos
         * como posible elección en un futuro.
         */

        for(int AFSRequiredNode : get<1>(results)){

            struct IFWProblem info_temp = info;
            vector<int> AFSNodes_temp = AFS_nodes;

            //Llamamos a la función encargada de la recarga de combustible.
            tuple<int,double> resultsTemp = fuelRecharge(info,adjMatrix,
                                                     AFS_nodes, instance_node);

            int AFS_index = get<0>(resultsTemp);
            double cost_temp = get<1>(resultsTemp);
            double distanceTemp = adjMatrix[AFS_index][AFSRequiredNode];

            /*Válidamos efectivamente se puede recargar el combustible como también si es posible realizar un viaje al
             *nodo cliente final.
             */
            if(AFS_index > 0 && cost_temp > 0 && validateTravel(info, distanceTemp)) {

                vector<int> clients_temp = clients;
                vector<int> partial_tour_temp = partial_tour;

                //Aplicamos los cargos de combustible y tiempo correspondientes.
                applyCharges(info, distanceTemp);

                /*Agregamos la distancia que nos tomo al partial_tour_cost como también los nodos (cliente y AFS) que
                * correspondan
                */
                partial_tour_cost += (cost_temp + distanceTemp);
                partial_tour.push_back(AFS_index);
                partial_tour.push_back(AFSRequiredNode);
                clients.erase(remove(clients.begin(), clients.end(), AFSRequiredNode),
                              clients.end());
                ++count;

                G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                            partial_tour, partial_tour_cost, count, numIteracion);

                // Restauramos los valores para poder explorar más nodos.
                partial_tour_cost -= (cost_temp + distanceTemp);
                partial_tour = partial_tour_temp;
                clients = clients_temp;
                info = info_temp;
                AFS_nodes = AFSNodes_temp;
            }
        }
    }
}

/*!
 *Función encargada de filtrar los nodos según su tipo (cliente o estación de servicio). Inicializa una solución parcial
 * y entrega todos los datos referentes al problema a la función recursiva (G_VRP_BT_FC).
 *
 * @param adjMatrix: Vector 2D con las distancias entre nodos (Arcos).
 * @param nodes: vector con todos los nodos de la instacia (clientes, depot, afs)
 * @param info: Estructura que registra y almacena la información referente al vehículo.
 * @param best_tour: El vector con los indices (clientes y/o AFS) correspondientes a la mejor solución que se tiene en un momento dado.
 * @param count: Contador que lleva el número de llamadas recursivas que esta llevando el algoritmo.
 * @return double, retorna el coste total del mejor tour.
 */
double G_VRP(vector<vector<double>> &adjMatrix, vector<TNode> &nodes, struct IFWProblem &info,
                          vector<int> &best_tour, unsigned int &count, unsigned int &numIteracion)
{
    double best_tour_cost = 0;
    double partial_tour_cost = 0;
    vector<int> clients;
    vector<int> AFS_nodes;
    vector<int> partial_tour;

    partial_tour.push_back(0); //Comenzamos en el nodo DEPOT (index 0)

    for (unsigned int i = 1; i < nodes.size(); i++) {
        if(nodes[i].Type != "f"){
            clients.push_back(i);
        } else {
            AFS_nodes.push_back(i);
        }
    }


    G_VRP_BT_FC(adjMatrix, info, clients, AFS_nodes, best_tour, best_tour_cost,
                partial_tour, partial_tour_cost, count, numIteracion);

    clients.clear();
    AFS_nodes.clear();
    partial_tour.clear();

    return best_tour_cost;
}
