#include "Grafo.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <tuple>
#include <algorithm>
#include "ConjuntoDisj.h"
using namespace std;


/***
 * @brief Construtor da classe Grafo
 * @param direcionado Indica se o grafo é direcionado
 * @param ponderadoAresta Indica se as arestas são ponderadas
 * @param ponderadoVertice Indica se os vértices são ponderados
 * @param vertices Vetor de strings representando os vértices, onde cada string é composta por um caractere seguido de um número (se ponderado)
 * @param arestas Vetor de strings representando as arestas, onde cada string é composta por dois caracteres (vértices) e um número (se ponderado)
 */
Grafo::Grafo(bool direcionado, bool ponderadoAresta, bool ponderadoVertice, vector<string> vertices, vector<string> arestas) {
    int ordem = 0;
    in_direcionado = direcionado;
    in_ponderado_aresta = ponderadoAresta;
    in_ponderado_vertice = ponderadoVertice;
    for(string vertice : vertices) {
        No *no = new No(vertice[0], (in_ponderado_vertice ? stoi(vertice.substr(1)) : 0));
        lista_adj.push_back(no);
        ordem++;
    }

    for(string aresta: arestas){
        char id_no_origem = aresta[0];
        char id_no_destino = aresta[1];
        int peso = (in_ponderado_aresta ? stoi(aresta.substr(2)) : 0);

        No *no_origem = nullptr;
        No *no_destino = nullptr;

        for(No *no : lista_adj) {
            if(no->id == id_no_origem) {
                no_origem = no;
            } else if(no->id == id_no_destino) {
                no_destino = no;
            }
        }

        if(no_origem && no_destino) {
            Aresta *aresta_obj = new Aresta(id_no_destino, peso);
            no_origem->arestas.push_back(aresta_obj);
            if(!direcionado) {
                Aresta *aresta_obj_inversa = new Aresta(id_no_origem, peso);
                no_destino->arestas.push_back(aresta_obj_inversa);
            }
        }
    }
}

/***
 * @brief Destrutor da classe Grafo
 */
Grafo::~Grafo() {
    for(No *no : lista_adj) {
        delete no;
    }
    lista_adj.clear();
    ordem = 0;
    in_direcionado = false;
    in_ponderado_aresta = false;
    in_ponderado_vertice = false;   
}

/***
 * @brief Imprime o grafo na forma de sua lista de adjacência
 */
void Grafo::print(){
    if(lista_adj.empty()) {
        cout << "Grafo vazio." << endl;
        return;
    }
    for(No *no : lista_adj) {
        cout << no->id;
        if(in_ponderado_vertice) {
            cout << "(" << no->peso << ")";
        }
        for(Aresta *aresta : no->arestas) {
            cout << " -> ";
            cout << aresta->id_no_alvo;
            if(in_ponderado_aresta) {
                cout << "(" << aresta->peso << ")";
            }
        }
        cout << endl;
    }
}

/***
 * @brief Insere um novo nó (vértice) no grafo
 * @param id_no Identificador do nó a ser inserido
 * @param peso Peso do nó, se o grafo for ponderado
 */
void Grafo::insereNo(char id_no, int peso) {
    for(No *no : lista_adj) {
        if(no->id == id_no) {
            return;
        }
    }
    No *novo_no = new No(id_no, peso);
    lista_adj.push_back(novo_no);
    ordem++;
}

/***
 * @brief Insere uma aresta entre dois nós no grafo
 * @param id_no_origem Identificador do nó de origem
 * @param id_no_destino Identificador do nó de destino
 * @param peso Peso da aresta, se o grafo for ponderado
 */
void Grafo::insereAresta(char id_no_origem, char id_no_destino, int peso) {
    No *no_origem = nullptr;
    No *no_destino = nullptr;

    for(No *no : lista_adj) {
        if(no->id == id_no_origem) {
            no_origem = no;
        } else if(no->id == id_no_destino) {
            no_destino = no;
        }
    }

    if(!no_origem || !no_destino)
        return;

    for(Aresta *aresta : no_origem->arestas)
        if(aresta->id_no_alvo == id_no_destino)
            return;

    Aresta *nova_aresta = new Aresta(id_no_destino, peso);
    no_origem->arestas.push_back(nova_aresta);

    if(!in_direcionado) {
        Aresta *aresta_inversa = new Aresta(id_no_origem, peso);
        no_destino->arestas.push_back(aresta_inversa);
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

No* Grafo::buscar_no(char id) {
    for (auto* no : lista_adj) {
        if (no->id == id) return no;
    }
    return nullptr;
}

/**
 * @brief Gera a Árvore Geradora Mínima (AGM) usando o algoritmo de Prim.
 * @param ids_nos Vetor com os identificadores dos nós do subconjunto a ser considerado.
 * @return Ponteiro para um novo objeto Grafo representando a AGM.
 */
Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    unordered_map<char, bool> visitado;
    for(auto* no : lista_adj){
        visitado[no->id] = false;
    }

    using ArestaInfo = tuple<int, char, char>;
    priority_queue<ArestaInfo, vector<ArestaInfo>, greater<ArestaInfo>> fila;

    Grafo* ArvoreGer = new Grafo(false, true, false);

    char id_inicial = ids_nos[0];
    visitado[id_inicial] = true;

    unordered_map<char, No*> mapa_agm;

    No* novo_no = new No(id_inicial);
    ArvoreGer->lista_adj.push_back(novo_no);
    mapa_agm[id_inicial] = novo_no;

    No* no_atual = buscar_no(id_inicial);


  if(no_atual){
    for (Aresta* aresta : no_atual->arestas) {
        fila.push(make_tuple(aresta->peso, no_atual->id, aresta->id_no_alvo));
    
    }
}

    while (!fila.empty()) {
        auto aresta = fila.top();
        fila.pop();
        int peso = get<0>(aresta);
        char origem = get<1>(aresta);
        char destino = get<2>(aresta);

        if (visitado[destino]) continue;
        visitado[destino] = true;

if (!mapa_agm.count(destino)) {
    ArvoreGer->insereNo(destino, 0); // ou use o peso correto se necessário
    mapa_agm[destino] = ArvoreGer->buscar_no(destino);
}
ArvoreGer->insereAresta(origem, destino, peso);

        // Busca ponteiro para o nó destino no grafo original
        No* no_destino = buscar_no(destino);

        // Adiciona as arestas do novo nó à fila
        for (Aresta* aresta : no_destino->arestas) {
            if (!visitado[aresta->id_no_alvo]) {
                fila.push(make_tuple(aresta->peso, destino, aresta->id_no_alvo));
            }
        }
    }

    return ArvoreGer;
}

/**
 * @brief Gera a Árvore Geradora Mínima (AGM) usando o algoritmo de Kruskal.
 * @param ids_nos Vetor com os identificadores dos nós do subconjunto a ser considerado.
 * @return Ponteiro para um novo objeto Grafo representando a AGM.
 */
Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    struct Edge { int peso; char u, v; };
    vector<Edge> arestas;
    unordered_set<string> inseridas;
    for (auto* no : lista_adj) {
        for (auto* aresta : no->arestas) {
            string chave = string() + min(no->id, aresta->id_no_alvo) + max(no->id, aresta->id_no_alvo);
            if (!inseridas.count(chave)) {
                arestas.push_back({aresta->peso, no->id, aresta->id_no_alvo});
                inseridas.insert(chave);
            }
        }
    }

    sort(arestas.begin(), arestas.end(), [](const Edge& a, const Edge& b) {
        return a.peso < b.peso;
    });

    ConjuntoDisj uf;
    uf.make_set(lista_adj);

    Grafo* agm = new Grafo(false, true, false);
    unordered_map<char, No*> mapa_agm;
    for (auto* no : lista_adj) {
        No* novo_no = new No(no->id, no->peso);
        agm->lista_adj.push_back(novo_no);
        mapa_agm[no->id] = novo_no;
    }

    int arestas_agm = 0;
    for (const auto& edge : arestas) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            mapa_agm[edge.u]->arestas.push_back(new Aresta(edge.v, edge.peso));
            if (!in_direcionado)
                mapa_agm[edge.v]->arestas.push_back(new Aresta(edge.u, edge.peso));
            arestas_agm++;
            if (arestas_agm == (int)lista_adj.size() - 1) break;
        }
    }

    return agm;

}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
