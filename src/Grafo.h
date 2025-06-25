//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <fstream>

#define INF 1000000000

using namespace std;
class Grafo {
public:
    Grafo(bool direcionado = false, bool ponderadoAresta = false, bool ponderadoVertice = false, vector<string> vertices = {}, vector<string> arestas = {});
    ~Grafo();

    void print();
    void insereNo(char id_no, int peso = 0);
    void insereAresta(char id_no_origem, char id_no_destino, int peso = 0);
    void gravar(string nome_arquivo);
    void floyd(vector<vector<int>> &distancias, vector<vector<char>> &antecessores);
    void temporaria(int &raio, int &diametro, vector<char> &centro, vector<char> &periferia);

    vector<char> fecho_transitivo_direto(char id_no); // a
    vector<char> fecho_transitivo_indireto(char id_no); // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i


    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
};



#endif //GRAFO_H
