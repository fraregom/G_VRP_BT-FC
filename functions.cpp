#include "functions.h"

/*!
 * Función encargada de "dividir" un string según delimitador
 *
 * @param str: String que se desea dividir
 * @param delimiter: Delimitador que se quiere utilizar.
 * @return vector<string>: Vector string con el resultado de la división.
 */
vector<string> split_string(const string& str,
                                 const string& delimiter)
{
    vector<string> strings;

    string::size_type pos;
    string::size_type prev = 0;

    while ((pos = str.find(delimiter, prev)) != string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    strings.push_back(str.substr(prev));

    return strings;
}

/*!
 * Función encargada de convertir grados en radianes.
 * @param degree: grados a convertir.
 * @return double, resultado de la conversión.
 */
double to_radians(double degree)
{
    return(degree / 57.29578);
}

/*!
 * Función encargada de encontrar la distancia entre dos puntos distantes según la curvatura de la tierra.
 * @param sNode: Nodo inicial
 * @param eNode: Nodo final.
 * @return double, retorna la distancia entre los nodos (arco).
 */
double haversine_distance(TNode &sNode, TNode &eNode){

    double dLat = to_radians(eNode.Latitude - sNode.Latitude);
    double dLon = to_radians(eNode.Longitude - sNode.Longitude);

    double aux = pow(sin(dLat / 2), 2) +
            cos(to_radians(sNode.Latitude)) *
            cos(to_radians(eNode.Latitude)) *
            pow(sin(dLon / 2), 2);

    double distance = 2 * EarthRadius * asin(sqrt(aux));

    return distance;
}

/*!
 * Función encargad de crear y poblar una matrix con los arcos (distancia) resultantes entre dos nodos.
 * @param nodes: Vector con todos los nodos que se requiere calcular sus distancias.
 * @return vector<vector<double>>: Retorna un vector 2D que contiene los arcos entre nodos de la instancia.
 */
vector<vector<double>> distance_matrix(vector<TNode> &nodes){

    vector<vector<double>> matrix ( nodes.size(), vector<double>( nodes.size()));

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        for (unsigned int j = 0; j < nodes.size(); j++)
        {
            matrix[i][j] = haversine_distance(nodes[i],nodes[j]);
        }
    }

    return matrix;
}
