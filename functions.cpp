#include "functions.h"

/*!
 *
 * @param str
 * @param delimiter
 * @return
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
 *
 * @param degree
 * @return
 */
double to_radians(double degree)
{
    return(degree / 57.29578);
}

/*!
 *
 * @param sNode
 * @param eNode
 * @return
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

double simple_distance(TNode &sNode, TNode &eNode)
{
    double x1 = sNode.Longitude;
    double y1 = sNode.Latitude;
    double x2 = eNode.Longitude;
    double y2 = eNode.Latitude;

    // Calculating distance
    return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);
}

/*!
 *
 * @param nodes
 * @return
 */
vector<vector<double>> distance_matrix(vector<TNode> &nodes){

    vector<vector<double>> matrix ( nodes.size(), vector<double>( nodes.size()));

    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes.size(); j++)
        {
            //matrix[i][j] = haversine_distance(nodes[i],nodes[j]);
            matrix[i][j] = simple_distance(nodes[i],nodes[j]);
        }
    }

    return matrix;
}
