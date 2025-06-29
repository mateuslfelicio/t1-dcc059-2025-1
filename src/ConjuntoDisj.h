#ifndef CONJUNTODISJ_H
#define CONJUNTODISJ_H

#include <unordered_map>
#include "No.h"
using namespace std;

/**
 * @brief Estrutura de dados de conjuntos disjuntos (Union-Find).
 *
 * Permite agrupar elementos em conjuntos, verificar rapidamente se dois elementos
 * pertencem ao mesmo conjunto e unir conjuntos distintos.
 */
class ConjuntoDisj {
    unordered_map<char, char> parent;
public:
    void make_set(const vector<No*>& nos);
    char find(char x);
    void unite(char x, char y);
};

#endif