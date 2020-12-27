#include "functions.h"

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

double to_radians(double degree)
{
    return(degree / 57.29578);
}

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

vector<vector<double>> distance_matrix(vector<TNode> &nodes){

    vector<vector<double>> matrix ( nodes.size(), vector<double>( nodes.size()));

    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = 0; j < nodes.size(); j++)
        {
            matrix[i][j] = haversine_distance(nodes[i],nodes[j]);
        }
    }

    return matrix;
}
