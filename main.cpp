#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include "auxiliar.h"

using namespace std;
typedef vector <TNode> Node_list;

struct GeneralInfo
{
    float Q;
    float r;
    float TL;
    float v;
};

int main(int argc, char* argv[]) {

    string filename;
    GeneralInfo status{};
    Node_list node_vector;

    if(argc > 0){
        filename = argv[1];
    }

    ifstream file (filename);

    if (file.is_open())
    {
        string line;
        vector <float> aux;

        getline (file, line);
        while(getline (file, line))
        {
            vector <string> parse = split_string(line,"\t");
            int vectorLong = parse.size();

            if(vectorLong > 1) {

                auto Node = new TNode(parse[0], parse[1],
                                      stod(parse[2]), stod(parse[3]));
                node_vector.push_back(*Node);

            } else if (line != "\r") {
                vector <string> parse2 = split_string(line,"/");
                aux.push_back(stof(parse2[1]));
            }
        }

        status.Q = aux[0];
        status.r = aux[1];
        status.TL = aux[2];
        status.v = aux[3];

        file.close();
    }

    else printf("Existe un problema al abrir dicha instancia..");

    for(TNode node : node_vector)
        cout << node.Mostrar() << endl;

    cout << status.Q << endl;
    cout << status.r << endl;
    cout << status.TL << endl;
    cout << status.v << endl;

    return 0;
}