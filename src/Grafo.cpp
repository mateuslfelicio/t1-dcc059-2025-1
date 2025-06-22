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
    ordem = 0;
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
 * @brief Grava o grafo em um arquivo
 * @param nome_arquivo Nome do arquivo onde o grafo será gravado
 */
void Grafo::gravar(string nome_arquivo) {
    fstream arquivo;
    arquivo.open(nome_arquivo, ios::out);
    if(!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }
    arquivo << in_direcionado << " "
            << in_ponderado_aresta << " "
            << in_ponderado_vertice << endl;
    arquivo << ordem;
    for(No *no : lista_adj) {
        arquivo << endl << no->id;
        if(in_ponderado_vertice) {
            arquivo << " " << no->peso;
        }
    }
    for(No *no : lista_adj) {
        for(Aresta *aresta : no->arestas) {
            if(!in_direcionado && (no->id > aresta->id_no_alvo))
                continue;
            arquivo << endl << no->id << " " << aresta->id_no_alvo;
            if(in_ponderado_aresta) {
                arquivo << " " << aresta->peso;
            }
        }
    }
    arquivo.close();
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


/***
 * @brief Calcula o caminho mínimo entre dois nós usando o algoritmo de Dijkstra
 * @param id_no_a Identificador do nó de origem
 * @param id_no_b Identificador do nó de destino
 * @return Vetor de caracteres representando o caminho mínimo
 */
vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    
    map<char, No*> lista_aux;
    map<char, bool> visitados;
    map<char, int> distancias;
    map<char,char> antecessores;
    
    priority_queue<pair<int, char>, vector<pair<int,char>>, greater<pair<int,char>>> fila;
    
    for(No *no : lista_adj) {
        lista_aux[no->id] = no;
        fila.push({(no->id == id_no_a) ? 0 : INF, no->id});
        visitados[no->id] = false;
        distancias[no->id] = INF;
        antecessores[no->id] = '\0';
    }

    distancias[id_no_a] = 0;
    
    while(!fila.empty()) {
        pair<int, char> atual = fila.top();
        visitados[atual.second] = true;
        fila.pop();

        if(distancias[atual.second] < atual.first)
            continue;

        vector<Aresta*> fecho_direto(lista_aux[atual.second]->arestas);
        for(Aresta *aresta : fecho_direto) {
            if(visitados[aresta->id_no_alvo])
                continue;
            
            int nova_distancia = distancias[atual.second] + aresta->peso;
            if(nova_distancia < distancias[aresta->id_no_alvo]) {
                distancias[aresta->id_no_alvo] = nova_distancia;
                antecessores[aresta->id_no_alvo] = atual.second;
                fila.push({nova_distancia, aresta->id_no_alvo});
            }
        }

    }

    vector<char> caminho;
    char atual = id_no_b;
    if(antecessores[atual] == '\0')
        return {};
    while(atual != id_no_a) {
        if (antecessores[atual] == '\0')
            return {};
        caminho.push_back(atual);
        atual = antecessores[atual];
    }
    caminho.push_back(id_no_a);
    reverse(caminho.begin(), caminho.end());

    return caminho;
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
