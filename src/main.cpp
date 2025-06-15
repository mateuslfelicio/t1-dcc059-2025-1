#include <iostream>
#include "Gerenciador.h"
#include <vector>
#include <string>

using namespace std;
int main(int argc, char *argv[])
{
    vector<string> vertices = {"A", "B", "C", "D"};
    vector<string> arestas = {"AB1", "AC2", "BD3", "CD4"};    
    Grafo* grafo = new Grafo(0,1,0, vertices, arestas);
    grafo->print();

    Gerenciador::comandos(grafo);

    return 0;
}
