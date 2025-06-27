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
    atualizado = false;
    raio_ = INF;
    diametro_ = 0;
    centro_ = {};
    periferia_ = {};
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
void Grafo::print() {
    if(lista_adj.empty()) {
        cout << "Grafo vazio." << endl;
        return;
    }
    for(No *no : lista_adj) {
        cout << no->id << ":";
        bool primeiro = true;
        for(Aresta *aresta : no->arestas) {
            if (primeiro) {
                cout << " " << aresta->id_no_alvo;
                primeiro = false;
            } else {
                cout << " -> " << aresta->id_no_alvo;
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
        cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }
    arquivo << "Direcionado: "<< in_direcionado << endl
            << "Ponderado nos vertices:" << in_ponderado_aresta << endl
            << "Ponderado nas arestas" << in_ponderado_vertice << endl;
    arquivo << "Ordem: " << ordem << endl;
    arquivo << "Lista de adjacencia:" << endl;
    for(No *no : lista_adj) {
        arquivo << no->id << ":";
        bool primeiro = true;
        for(Aresta *aresta : no->arestas) {
            if (primeiro) {
                arquivo << " " << aresta->id_no_alvo;
                primeiro = false;
            } else {
                arquivo << " -> " << aresta->id_no_alvo;
            }
        }
        arquivo << endl;
    }
    cout << "Grafo gravado em " << nome_arquivo << endl;
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

    atualizado = false;
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

    atualizado = false;
}

/***
 * @brief Atualiza as informações do grafo, como raio, diâmetro, centro e periferia
 */
void Grafo::atualizaInfo() {

    raio_ = INF;
    diametro_ = 0;
    
    vector<vector<int>> distancias(ordem, vector<int>(ordem, INF));
    vector<vector<char>> antecessores(ordem, vector<char>(ordem, '\0'));
    
    floyd(distancias, antecessores);

    map<char, int> excentricidade;

    for(int i = 0; i < ordem; i++) {
        for(int j = 0; j < ordem; j++) {
            excentricidade[lista_adj[i]->id] = max(excentricidade[lista_adj[i]->id], distancias[i][j]);
            if(distancias[i][j] != INF) {
                if(i == j) continue;

                if(distancias[i][j] > diametro_) {
                    diametro_ = distancias[i][j];
                }
                if(distancias[i][j] < raio_) {
                    raio_ = distancias[i][j];
                }
            }
        }
    }

    if(raio_ == INF and diametro_ == 0){
        atualizado = true;
        return;
    }

    for(int i = 0; i < ordem; i++) {
        if(excentricidade[lista_adj[i]->id] == raio_) {
            centro_.push_back(lista_adj[i]->id);
        }
        if(excentricidade[lista_adj[i]->id] == diametro_) {
            periferia_.push_back(lista_adj[i]->id);
        }
    }

    atualizado = true;
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


/***
 * @brief Calcula o caminho mínimo entre dois nós usando o algoritmo de Floyd-Warshall
 * @param id_no Identificador do nó de origem
 * @param id_no_b Identificador do nó de destino
 * @return Vetor de caracteres representando o caminho mínimo
 * @note Se não houver caminho entre os nós, retorna um vetor vazio
 */
vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    vector<vector<int>> distancias(ordem, vector<int>(ordem, INF));
    vector<vector<char>> antecessores(ordem, vector<char>(ordem, '\0'));
    floyd(distancias, antecessores);

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

    vector<char> caminho;
    int atual = c2;
    while(atual != c1) {
        caminho.push_back(index_to_id[atual]);
        char ant = antecessores[c1][atual];
        if(ant == '\0')
            return {};
        atual = id_to_index[ant];
    }
    caminho.push_back(index_to_id[c1]);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}


/***
 * @brief Implementa o algoritmo de Floyd-Warshall para calcular as distâncias entre todos os pares de nós
 * @param distancias Matriz onde serão armazenadas as distâncias entre os nós
 * @param antecessores Matriz onde serão armazenados os antecessores de cada nó no caminho mínimo
 */
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

/***
 * @brief Retorna o raio do grafo
 * @note Caso o grafo seja vazio retorna INF
 * @note INF = 1000000000
 */
int Grafo::raio() {
    if(!atualizado)
        atualizaInfo();
    return raio_;
}

/***
 * @brief Retorna o diâmetro do grafo
 * @note Caso o grafo seja vazio retorna 0
 */
int Grafo::diametro() {
    if(!atualizado)
        atualizaInfo();
    return diametro_;
}

/***
 * @brief Retorna o centro do grafo
 * @note Caso o grafo seja vazio retorna um vetor vazio
 */
vector<char> Grafo::centro() {
    if(!atualizado)
        atualizaInfo();
    return centro_;
}

/***
 * @brief Retorna a periferia do grafo
 * @note Caso o grafo seja vazio retorna um vetor vazio
 */
vector<char> Grafo::periferia() {
    if(!atualizado)
        atualizaInfo();
    return periferia_;
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
