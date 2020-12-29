#include <iostream>
#include <fstream>
#include "node.h"
#include "functions.h"
#include "CSP_Algorithm.h"

using namespace std;

int main(int argc, char* argv[]) {

    string filename;
    IFWProblem ifwProblem {};
    NodeList nodeList;
    DistanceMatrix distanceMatrix;

    if(argc > 0){
        filename = argv[1];
    }

    ifstream file (filename);

    if (file.is_open())
    {
        string textLine;
        vector <float> vectorAux;

        getline (file, textLine);
        while(getline (file, textLine))
        {
            vector <string> parseResult = split_string(textLine, "\t");
            int vectorLong = parseResult.size();

            if(vectorLong > 1) {

                auto Node = new TNode(parseResult[0], parseResult[1],
                                      stod(parseResult[2]), stod(parseResult[3]));
                nodeList.push_back(*Node);

            } else if (textLine != "\r") {
                vector <string> parseResult2 = split_string(textLine, "/");
                vectorAux.push_back(stof(parseResult2[1]));
            }
        }

        file.close();

        ifwProblem.Q = vectorAux[0];
        ifwProblem.r = vectorAux[1];
        ifwProblem.TL = vectorAux[2];
        ifwProblem.v = vectorAux[3];
        ifwProblem.m = vectorAux[4];

        distanceMatrix = distance_matrix(nodeList);
        int size = distanceMatrix.size();

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cout << distanceMatrix[i][j] << " ";
            }
            cout << endl;
        }

        cout << "---" << endl;

        tuple<double,vector<int>> result = shortest_path(ifwProblem,nodeList,distanceMatrix);
        cout << get<0>(result) << endl;

        cout << "---" << endl;

        vector<int> trip = get<1>(result);
        for (int i : trip) {
            cout << nodeList[i].ID;
            cout << " ";
        }
        cout << endl;

        cout << "---" << endl;

    }

    else printf("Existe un problema al abrir dicha instancia..");
    return 0;
}