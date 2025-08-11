#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

class Guloso {
    public:
    static vector<char> guloso(Grafo* grafo);
    static vector<char> guloso_randomizado(Grafo* grafo, double alpha, mt19937& rng, int iteracoes);
    static vector<char> guloso_randomizado_reativo(Grafo* grafo, const vector<double>& alphas, mt19937& rng, int iteracoes, int bloco);
    static bool verifica(Grafo* grafo, vector<char> solucao);
    static vector<char> heuristics(Grafo* grafo);

    static int arestas_livres(No* no, Grafo* grafo);
    static void limpar_dominados(Grafo* grafo);
};

#endif 