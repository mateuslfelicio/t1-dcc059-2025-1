#include <iostream>
#include <fstream>
#include "Gerenciador.h"
#include <vector>
#include <string>
#include <algorithm>

string remove_espacos(const string& s) {
    string res = s;
    size_t pos = res.find(' ');
    while (pos != string::npos) {
        res.replace(pos, 1, "");
        pos = res.find(' ');
    }
    return res;
}

void ler_arquivo( const string& nome_arquivo, bool& direcionado,
     bool& ponderado_aresta, bool& ponderado_vertice,
    vector<string>& vertices, vector<string>& arestas){
    ifstream arq(nome_arquivo);
    if(!arq.is_open()){
        cerr<< "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        exit(1);
        }
        int dir, pond_arest, pond_vert;
        int n_vertices;
        arq >> dir >> pond_arest >> pond_vert;
        direcionado = dir;
        ponderado_aresta = pond_arest;
        ponderado_vertice = pond_vert;
        arq >> n_vertices;
        arq.ignore();

        string linha;
        for (int i = 0; i < n_vertices; i++){
            getline(arq, linha);
            linha = remove_espacos(linha);
            if(!linha.empty()){
                vertices.push_back(linha);
            }
        }

        while(getline(arq,linha)){
            linha = remove_espacos(linha);
            if(!linha.empty()){
                arestas.push_back(linha);
            }
        }
        arq.close();
}

using namespace std;
int main(int argc, char *argv[])
{
   /* vector<string> vertices = {"A", "B", "C", "D"};
    vector<string> arestas = {"AB1", "AC2", "BD3", "CD4"};    
    Grafo* grafo = new Grafo(0,1,0, vertices, arestas);*/
    if(argc < 2){
        cout << "Uso: " << argv[0] << " <arquivo_instancia>" << endl;
        return 1;
    }
    bool direcionado, ponderado_aresta, ponderado_vertice;
    vector<string> vertices, arestas;
    ler_arquivo(argv[1], direcionado, ponderado_aresta, ponderado_vertice, vertices, arestas);

    Grafo* grafo = new Grafo(direcionado, ponderado_aresta, ponderado_vertice, vertices, arestas);
    grafo->gravar("../saida/grafo.txt");

    Gerenciador::comandos(grafo);

    return 0;
}
