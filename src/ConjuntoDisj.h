#ifndef CONJUNTODISJ_H
#define CONJUNTODISJ_H

#include <unordered_map>
#include "No.h"

class ConjuntoDisj {
    std::unordered_map<char, char> parent;
public:
    void make_set(const std::vector<No*>& nos);
    char find(char x);
    void unite(char x, char y);
};

#endif