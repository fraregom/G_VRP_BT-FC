#include <iostream>
#include <fstream>
#include "node.h"
#include "functions.h"

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

        ifwProblem.Q = vectorAux[0];
        ifwProblem.r = vectorAux[1];
        ifwProblem.TL = vectorAux[2];
        ifwProblem.v = vectorAux[3];
        ifwProblem.m = vectorAux[4];

        distanceMatrix = distance_matrix(nodeList);
        file.close();
    }

    else printf("Existe un problema al abrir dicha instancia..");
    return 0;
}