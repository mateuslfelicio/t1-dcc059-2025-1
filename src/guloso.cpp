#include "guloso.h"
#include <chrono>
#include <iomanip>

using namespace std;

vector<char> Guloso::guloso(Grafo* grafo) {
    vector<char> solucao;
    vector<char> heuristica = heuristics(grafo);
    if(heuristica.empty()) {
        return {};
    }
    while(!heuristica.empty()) {
        char no_id = heuristica.back();
        heuristica.pop_back();
        No* no = grafo->buscar_no(no_id);
        if(!no || no->dominado) continue;

        solucao.push_back(no_id);
        no->dominado = true;

        //atualiza os vértices vizinhos como dominados
        for(Aresta* aresta: no->arestas) {
            No* alvo = grafo->buscar_no(aresta->id_no_alvo);
            if(alvo) {
                alvo->dominado = true;
            }
        }
        heuristica = heuristics(grafo);
    }
    //atuzalizar o estado de dominados sempre que inserir nó a solução
    // Deixar essa função como ultima coisa antes do return
    if(!verifica(grafo, solucao)){
        cout << "Solução Inválida" << endl;
        return {};
    }
    limpar_dominados(grafo);
    return solucao;
}

vector<char> Guloso::guloso_randomizado(Grafo* grafo, double alpha, mt19937& rng) {
    Guloso::limpar_dominados(grafo);
    vector<char> solucao;
    set<char> dominados;
    int n = grafo->lista_adj.size();

    while ((int)dominados.size() < n) {
        // Recebe os candidatos ordenados pela heurística
        vector<char> candidatos = Guloso::heuristics(grafo);

        // Filtra candidatos válidos (não dominados, não adjacentes à solução)
        vector<char> candidatos_validos;
        for (char id : candidatos) {
            if (dominados.count(id)) continue;
            No* no = grafo->buscar_no(id);
            bool adjacente = false;
            for (Aresta* a : no->arestas) {
                if (find(solucao.begin(), solucao.end(), a->id_no_alvo) != solucao.end()) {
                    adjacente = true;
                    break;
                }
            }
            if (!adjacente) {
                candidatos_validos.push_back(id);
            }
        }
        if (candidatos_validos.empty()) break;

        // Monta LCR (top alpha% dos melhores candidatos)
        int lcr_size = std::max(1, int(std::ceil(candidatos_validos.size() * alpha)));
        vector<char> LCR = candidatos_validos;
        shuffle(LCR.begin(), LCR.end(), rng);
        if ((int)LCR.size() > lcr_size) LCR.resize(lcr_size);

        // Escolhe aleatoriamente um da LCR
        uniform_int_distribution<int> dist(0, LCR.size() - 1);
        char escolhido_id = LCR[dist(rng)];
        solucao.push_back(escolhido_id);

        // Marca escolhido e vizinhos como dominados
        dominados.insert(escolhido_id);
        grafo->buscar_no(escolhido_id)->dominado = true;
        No* escolhido = grafo->buscar_no(escolhido_id);
        for (Aresta* a : escolhido->arestas) {
            dominados.insert(a->id_no_alvo);
            grafo->buscar_no(a->id_no_alvo)->dominado = true;
        }
    }

    if (!Guloso::verifica(grafo, solucao)) {
        cout << "Solução Inválida" << endl;
        Guloso::limpar_dominados(grafo);
        return {};
    }
    // Deixar essa função como ultima coisa antes do return
    Guloso::limpar_dominados(grafo);
    return solucao;
}

vector<char> Guloso::guloso_randomizado_reativo(Grafo* grafo, double alpha, mt19937& rng) {
    Guloso::limpar_dominados(grafo);
    vector<char> solucao;
    set<char> dominados;
    int n = grafo->lista_adj.size();

    while ((int)dominados.size() < n) {
        // Usa heuristics para obter candidatos ordenados
        vector<char> candidatos = Guloso::heuristics(grafo);

        // Filtra candidatos válidos (não dominados, não adjacentes à solução)
        vector<pair<char, int>> candidatos_cobertura;
        int melhorCobertura = -1;
        for (char v_id : candidatos) {
            if (dominados.count(v_id)) continue;
            if (find(solucao.begin(), solucao.end(), v_id) != solucao.end()) continue;

            No* v = grafo->buscar_no(v_id);
            bool vizinho_na_solucao = false;
            for (Aresta* a : v->arestas) {
                if (find(solucao.begin(), solucao.end(), a->id_no_alvo) != solucao.end()) {
                    vizinho_na_solucao = true;
                    break;
                }
            }
            if (vizinho_na_solucao) continue;

            // Conta cobertura: v + vizinhos ainda não dominados
            int cobertura = 0;
            if (!dominados.count(v->id)) cobertura++;
            for (Aresta* a : v->arestas) {
                if (!dominados.count(a->id_no_alvo)) cobertura++;
            }
            if (cobertura > melhorCobertura) melhorCobertura = cobertura;
            candidatos_cobertura.push_back({v_id, cobertura});
        }

        // Monta LCR (top alpha%)
        vector<char> LCR;
        for (auto& par : candidatos_cobertura) {
            if (par.second >= melhorCobertura - int(alpha * melhorCobertura)) {
                LCR.push_back(par.first);
            }
        }
        if (LCR.empty()) break;

        int lcr_size = std::max(1, int(std::ceil(LCR.size() * alpha)));
        if (LCR.size() > 1 && lcr_size < 2) lcr_size = 2;
        if (lcr_size > (int)LCR.size()) lcr_size = LCR.size();
        shuffle(LCR.begin(), LCR.end(), rng);

        // Escolhe aleatoriamente um da LCR
        uniform_int_distribution<int> dist(0, LCR.size() - 1);
        char escolhido_id = LCR[dist(rng)];
        solucao.push_back(escolhido_id);

        // Marca escolhido e vizinhos como dominados
        dominados.insert(escolhido_id);
        grafo->buscar_no(escolhido_id)->dominado = true;
        No* escolhido = grafo->buscar_no(escolhido_id);
        for (Aresta* a : escolhido->arestas) {
            dominados.insert(a->id_no_alvo);
            grafo->buscar_no(a->id_no_alvo)->dominado = true;
        }
    }

    if (!Guloso::verifica(grafo, solucao)) {
        cout << "Solução Inválida" << endl;
        Guloso::limpar_dominados(grafo);
        return {};
    }
    Guloso::limpar_dominados(grafo);
    return solucao;
}

/***
 * @brief Verifica se a solução é valida
 * @param grafo Grafo a ser verificado
 * @param solucao Vetor de caracteres representando os vértices da solução
 * @return true se a solução é válida, false caso contrário
 * @note A solução é válida se todos os vértices estão dominados e não há arestas entre os vértices da solução
 */
bool Guloso::verifica(Grafo* grafo, vector<char> solucao) {
    // verifica se os vertices da solucao estao conexos
    for(char id : solucao) {
        No* no = grafo->buscar_no(id);
        for(Aresta* aresta : no->arestas) {
            if(find(solucao.begin(), solucao.end(), aresta->id_no_alvo) != solucao.end())
                return false;
        }
    }

    for(No* no : grafo->lista_adj) {
        if(!no->dominado)
            return false;
    }
    return true;
}

/***
 * @brief Gera uma heurística baseada nos vértices do grafo
 * @param grafo Grafo a ser utilizado
 * @param solucao Vetor de caracteres representando os vértices da solução (opcional)
 * @return Vetor de caracteres representando os vértices ordenados pela quantidade de arestas
 * @note A heurística é baseada na quantidade de arestas de cada vértice, ordenada em ordem decrescente
 */
vector<char> Guloso::heuristics(Grafo* grafo) {
    vector<pair<char, int>> parNoAresta;
    
    // Calcula a  quantidade de vértices afetados por cada nó, descartando os já dominados
    for(No* no : grafo->lista_adj) {
        if(no->dominado) continue;
        parNoAresta.push_back(make_pair(no->id, arestas_livres(no, grafo)));
    }
    
    // Ordena os parNoAresta pela quantidade de arestas em ordem decrescente
    sort(parNoAresta.begin(), parNoAresta.end(), [](const pair<char,int>& a, const pair<char,int>& b) {
        return a.second > b.second;
    });

    // Cria um vetor de caracteres ordenados
    vector<char> noArestaOrdenado;
    for(pair<char,int> par: parNoAresta) {
        noArestaOrdenado.push_back(par.first);
    }

    return noArestaOrdenado;
}


/***
 * @brief Conta o número de arestas livres de um nó
 * @param no Nó a ser verificado
 * @param grafo Grafo onde o nó está inserido
 * @return Número de arestas livres do nó
 * @note Uma aresta é considerada livre se o nó de destino não está dominado
 */
int Guloso::arestas_livres(No* no, Grafo* grafo) {
    int arestasLivres = 0;
    for(Aresta* aresta : no->arestas) {
        No* dest = grafo->buscar_no(aresta->id_no_alvo);
        if(dest && !dest->dominado)
            arestasLivres++;
    }
    return arestasLivres;
}


/***
 * @brief Limpa o estado de dominados de todos os nós do grafo
 * @param grafo Grafo a ser limpo
 * @note Reseta o atributo 'dominado' de todos os nós para false
 */
void Guloso::limpar_dominados(Grafo* grafo) {
    for(No* no : grafo->lista_adj) {
        no->dominado = false;
    }
}