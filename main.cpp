#include <iostream>
#include <fstream>
#include "node.h"
#include "functions.h"
#include "CSP_Algorithm.h"

/*!
 * Función principal del G_VRP-FC
 * @param argv Se ingresa por terminal el nombre del archivo a utilizar en G-VRP.
 * @return No retorna nada, solo imprime en pantalla los resultados.
 */
int main(int argc, char* argv[]) {

    /*Se definen las estructuras a utilizar:
     * filename: Nombre del archivo.
     * ifwProblem: Guarda los datos referentes al problema (combustible, ratio de consumo, etc).
     * nodeList: vector (arreglo) que almacena los nodos con las coordenadas correspondientes (entre otros datos).
     * distanceMatrix: vector 2-D que almacena los costes (distancia) entre arcos, es decir, A->B.
    */
    string filename;
    IFWProblem ifwProblem {};
    NodeList nodeList;
    DistanceMatrix distanceMatrix;
    time_t start, end;

    if(argc > 0){
        filename = argv[1];
    }

    ifstream file (filename);

    /* Abrimos el archivo y extraemos los datos de la instancia, guardamos las coordenadas en los nodos
     * como también creamos la matriz de distancia.
     */
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
        ifwProblem.TL = 24;//vectorAux[2];
        ifwProblem.v = vectorAux[3];
        ifwProblem.m = vectorAux[4];

        file.close();
        vectorAux.clear();

        distanceMatrix = distance_matrix(nodeList);

        time(&start);
        printf("G-VRP usando BC-FC (Con Restricción de tiempo: 24 h)..\n");

        /* Llamamos a la función G_VRP, esta se encargara de encontrar la ruta optima, le pasamos los datos
         * referentes al problema, un vector best_tour encargado de almacenar la solución final como también
         * cout que guardará cuántas llamadas recursivas se han realizado a lo largo de la ejecución.
         */
        vector<int> best_tour;
        unsigned int numIteracion = 0;
        unsigned int count = 0;
        double cost = G_VRP(distanceMatrix, nodeList,ifwProblem, best_tour, count, numIteracion);
        time(&end);

        //Imprimir los resultados de la ejecución.

        printf("Resultados de la instancia '%s':\n", filename.c_str());
        printf("Costo del mejor tour: %f\n", cost);

        printf("Tour:\n");
        for(int i : best_tour){
            printf("%s -> ", nodeList[i].ID.c_str());
        }
        printf("D\n");

        printf("Solución encontrada en la iteración: %u \n", numIteracion);
        printf("Num. llamadas recursivas: %u \n", count);

        auto time_taken = double(end - start);
        cout << "Tiempo de ejecución: " << fixed
             << time_taken << setprecision(5);
        cout << " sec " << endl;

        best_tour.clear();
        nodeList.clear();
        distanceMatrix.clear();
    }

    else printf("Existe un problema al abrir dicha instancia, revise que este correctamente escrita..");
    return 0;
}
