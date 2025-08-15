#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No(char id, int peso = 0);
    ~No();

    char id;
    int peso;
    vector<Aresta*> arestas;
    bool dominado = false;
};



#endif //NO_H
