#include "ConjuntoDisj.h"

void ConjuntoDisj::make_set(const std::vector<No*>& nos) {
    for (auto* no : nos) parent[no->id] = no->id;
}
char ConjuntoDisj::find(char x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}
void ConjuntoDisj::unite(char x, char y) {
    parent[find(x)] = find(y);
}