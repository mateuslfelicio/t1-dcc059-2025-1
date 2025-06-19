#include "Grafo.h"
#include <unordered_map>
#include <queue>
#include <tuple>
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

        // Adiciona nó destino à AGM se ainda não estiver
        if (!mapa_agm.count(destino)) {
            No* novo_no = new No(destino);
            ArvoreGer->lista_adj.push_back(novo_no);
            mapa_agm[destino] = novo_no;
        }

        // Adiciona aresta à AGM
        No* no_origem_agm = mapa_agm[origem];
        No* no_destino_agm = mapa_agm[destino];
        if (no_origem_agm && no_destino_agm) {
            no_origem_agm->arestas.push_back(new Aresta(destino, peso));
            if (!in_direcionado) {
                no_destino_agm->arestas.push_back(new Aresta(origem, peso));
            }
        }

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

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
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
