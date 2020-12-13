#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"

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
        string ID,Type;
        float lgt,lat;

        getline (file, line);
        while ( file >> ID >> Type >> lgt >> lat)
        {
            TNode *Node = new TNode(ID, Type, lgt, lat);
            node_vector.push_back(*Node);
        }
        file.close();
    }

    else printf("Existe un problema al abrir dicha instancia..");

    for(TNode node : node_vector)
        cout << node.Mostrar() << endl;

    return 0;
}