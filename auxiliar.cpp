#include <string>
#include <vector>
#include "auxiliar.h"
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
