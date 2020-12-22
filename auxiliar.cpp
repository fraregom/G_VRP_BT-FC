#include <string>
#include <vector>
#include <cmath>
#include "auxiliar.h"
#include "Node.h"

using namespace std;

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

double get_radians(double degree)
{
    const double halfC = M_PI / 180;
    return degree * halfC;
}

double haversine_distance(TNode &A, TNode &B){

    double difLatitud = get_radians(B.Latitude - A.Latitude);
    double difLongitud = get_radians(B.Longitude - A.Longitude);

    double a = pow(sin(difLatitud/2),2) +
            cos(get_radians(A.Latitude)) *
            cos(get_radians(B.Latitude)) *
            pow(sin(difLongitud/2),2);

    double distance = 2 * EarthRadiusKm * asin(sqrt(a));

    return distance;
}
