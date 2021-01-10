#include <iostream>
#include <fstream>
#include "node.h"
#include "functions.h"
#include "CSP_Algorithm.h"

/*!
 *
 * @param argc
 * @param argv
 * @return
 */
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

        file.close();
        vectorAux.clear();

        distanceMatrix = distance_matrix(nodeList);

        /*distanceMatrix = {
                {0,  148,  55,  32,  70, 140,  73,  60,  45, 75},
                {148,  0,  93, 180,  99,  12,  72, 102,  80, 40},
                {55,  93,   0,  85,  20,  83,  28,  45,  75, 90},
                {32, 180,  85,   0, 100, 174,  99,  90,  50, 93},
                {70,  99,  20, 100,   0,  85,  49,  25,  60, 55},
                {140, 12,  83, 174,  85,   0,  73,  89,  55, 80},
                {73,  72,  28,  99,  49,  73,   0,  50,  85, 64},
                {60, 102,  45,  90,  25,  89,  50,   0, 100, 82},
                {45,  80,  75,  50,  60,  55,  85, 100,   0, 58},
                {75,  40,  90,  93,  55,  80,  64,  82,  82,  0}
        };*/

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

        vector<int> best_tour;
        int count = 0;
        double cost = G_VRP(distanceMatrix, nodeList,ifwProblem, best_tour, count);

        printf("Costo del mejor tour: %f\n", cost);

        printf("Vertices:\n");
        for(int i : best_tour){
            printf("%u ", i);
        }
        printf("D\n");

        printf("Coste por arco:\n");
        for (int i = 0; i < best_tour.size() - 1; i++) {
            printf("%f ", distanceMatrix[best_tour[i]][best_tour[i + 1]]);
        }
        printf("%f\n", distanceMatrix[best_tour[best_tour.size() - 1]][0]);

        printf("---- \nNum. llamadas recursivas: %u \n", count);

        best_tour.clear();
        nodeList.clear();
        distanceMatrix.clear();
    }

    else printf("Existe un problema al abrir dicha instancia, revise que este correctamente escrita..");
    return 0;
}