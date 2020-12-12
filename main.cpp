#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

int main() {


    ifstream myfile ("/Users/havok/Documents/Workspace.nosync/Proyecto_GVRP/Instancias/20c3sU1.txt");
    string delimiter = "   ";

    if (myfile.is_open())
    {
        string line;
        int count = 0;
        while ( getline (myfile, line) )
        {
            if(count >= 1){
                for (char &c : line) {
                    if (c != ' ') {
                        if (isupper(c)){
                            cout << "------ Un nodo: ----" << endl;
                        }
                        cout << "One character: " << c << "\n";

                    }
                }
            }
            count += 1;
        }
        myfile.close();
    }



    else printf("Unable to open file");
    return 0;
}