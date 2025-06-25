#include "Grafo.h"

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
    arquivo << "Direcionado: "<< in_direcionado << endl
            << "Ponderado nos vertices:" << in_ponderado_aresta << endl
            << "Ponderado nas arestas" << in_ponderado_vertice << endl;
    arquivo << "Ordem: " << ordem << endl;
    arquivo << "Lista de adjacencia:" << endl;
    arquivo << "No(peso) -> Arestas(peso)" << endl;
    for(No *no : lista_adj) {
        arquivo << no->id;
        if(in_ponderado_vertice) {
            arquivo << "(" << no->peso << ")";
        }
        for(Aresta *aresta : no->arestas) {
            arquivo << " -> ";
            arquivo << aresta->id_no_alvo;
            if(in_ponderado_aresta) {
                arquivo << "(" << aresta->peso << ")";
            }
        }
        arquivo << endl;
    }
    arquivo.close();
}

/***
 * @brief Insere um novo nó (vértice) no grafo
 * @param id_no Identificador do nó a ser inserido
 * @param peso Peso do nó, se o grafo for ponderado
 *  @note Se o nó já existir, não será inserido novamente
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
 * @note se ambos os nós não existirem, a aresta não será inserida
 * @note se a aresta já existir, não será inserida novamente
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

void Grafo::temporaria(int &raio, int &diametro, vector<char> &centro, vector<char> &periferia) {
    Grafo grafo = new Grafo(in_direcionado, 1, 0);
    for(No *no : lista_adj) {
        grafo.insereNo(no->id, no->peso);
    }
    for(No *no : lista_adj) {
        for(Aresta *aresta : no->arestas) {
            grafo.insereAresta(no->id, aresta->id_no_alvo, 1);
        }
    }
    vector<vector<int>> distancias(grafo.ordem, vector<int>(grafo.ordem, INF));
    vector<vector<char>> antecessores(grafo.ordem, vector<char>(grafo.ordem, '\0'));
    grafo.floyd(distancias, antecessores);

    for(int i = 0; i < grafo.ordem; i++) {
        for(int j = 0; j < grafo.ordem; j++) {
            if(distancias[i][j] != INF) {
                if(i == j) continue;
                if(distancias[i][j] > diametro) {
                    diametro = distancias[i][j];
                }
                if(distancias[i][j] < raio) {
                    raio = distancias[i][j];
                }
            }
        }
    }

    for(int i = 0; i < grafo.ordem; i++) {
        for(int j = 0; j < grafo.ordem; j++) {
            if(distancias[i][j] == raio) {
                centro.push_back(grafo.lista_adj[i]->id);
            }
            if(distancias[i][j] == diametro) {
                periferia.push_back(grafo.lista_adj[i]->id);
            }
        }
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
    vector<vector<int>> distancias(ordem, vector<int>(ordem, INF));
    vector<vector<char>> antecessores(ordem, vector<char>(ordem, '\0'));
    floyd(distancias, antecessores);

    // Mapeia id para índice e vice-versa
    map<char, int> id_to_index;
    map<int, char> index_to_id;
    int i = 0, c1 = -1, c2 = -1;
    for(No *no : lista_adj) {
        id_to_index[no->id] = i;
        index_to_id[i] = no->id;
        if(no->id == id_no)
            c1 = i;
        if(no->id == id_no_b)
            c2 = i;
        i++;
    }
    if(c1 == -1 || c2 == -1 || distancias[c1][c2] == INF)
        return {};

    // Reconstrução do caminho de trás para frente
    vector<char> caminho;
    int atual = c2;
    while(atual != c1) {
        caminho.push_back(index_to_id[atual]);
        char ant = antecessores[c1][atual];
        if(ant == '\0') // Não há caminho
            return {};
        atual = id_to_index[ant];
    }
    caminho.push_back(index_to_id[c1]);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

void Grafo::floyd(vector<vector<int>> &distancias, vector<vector<char>> &antecessores) {
    int n = ordem;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                distancias[i][j] = 0;
                antecessores[i][j] = '\0';
            } else {
                distancias[i][j] = INF;
                antecessores[i][j] = '\0';
            }
        }
    }

    for(int i = 0; i < n; i++) {
        No *no = lista_adj[i];
        for(Aresta *aresta : no->arestas) {
            int j = 0;
            while(j < n && lista_adj[j]->id != aresta->id_no_alvo) j++;
            if(j < n) {
                distancias[i][j] = aresta->peso;
                antecessores[i][j] = no->id;
            }
        }
    }

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(distancias[i][j] > distancias[i][k] + distancias[k][j]) {
                    distancias[i][j] = distancias[i][k] + distancias[k][j];
                    antecessores[i][j] = antecessores[k][j];
                }
            }
        }
    }

}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
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
