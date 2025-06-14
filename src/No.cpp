#include "No.h"

No::No(char id, int peso) {
    this->id = id;
    this->peso = peso;
}

No::~No() {
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
    arestas.clear();
}