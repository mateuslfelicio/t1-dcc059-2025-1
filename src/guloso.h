#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Guloso {
    public:
    static vector<char> guloso(Grafo* grafo);
    static vector<char> guloso_randomizado(Grafo* grafo);
    static vector<char> guloso_randomizado_reativo(Grafo* grafo);

    static bool verifica();
    static vector<char> heuristics();
};

#endif 