#include "guloso.h"
#include <chrono>
#include <iomanip>
#include <climits>

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

vector<char> Guloso::guloso_randomizado(Grafo* grafo, double alpha, mt19937& rng, int num_iter) {
    vector<char> melhor_solucao;
    int melhor_custo = INT_MAX;

    for(int i = 0; i < num_iter; i++) {
        vector<char> solucao;
        set<char> dominados;
        int n = grafo->lista_adj.size();

        while ((int)dominados.size() < n) {
            vector<char> candidatos = Guloso::heuristics(grafo);
            if (candidatos.empty()) break;

            int lcr_size = std::max(1, int(std::ceil(candidatos.size() * alpha)));
            vector<char> LCR = candidatos;
            shuffle(LCR.begin(), LCR.end(), rng);
            if ((int)LCR.size() > lcr_size) LCR.resize(lcr_size);

            uniform_int_distribution<int> dist(0, LCR.size() - 1);
            char escolhido_id = LCR[dist(rng)];
            solucao.push_back(escolhido_id);

            dominados.insert(escolhido_id);
            grafo->buscar_no(escolhido_id)->dominado = true;
            No* escolhido = grafo->buscar_no(escolhido_id);
            for (Aresta* a : escolhido->arestas) {
                dominados.insert(a->id_no_alvo);
                grafo->buscar_no(a->id_no_alvo)->dominado = true;
            }
        }

        if (Guloso::verifica(grafo, solucao)) {
            // Atualiza melhor solução se encontrou uma válida menor
            if (solucao.size() < melhor_custo) {
                melhor_solucao = solucao;
                melhor_custo = solucao.size();
            }
        }
        Guloso::limpar_dominados(grafo);
    }

    return melhor_solucao;
}

vector<char> Guloso::guloso_randomizado_reativo( Grafo* grafo, const vector<double>& alphas, mt19937& rng, int iteracoes, int bloco) {
    vector<double> probabilidades(alphas.size(), 1.0 / alphas.size());
    vector<double> custos(alphas.size(), 0.0);
    vector<int> usos(alphas.size(), 0);

    vector<char> melhor_solucao;
    int melhor_custo = INT_MAX;

    for (int i = 0; i < iteracoes; ++i) {
        // Escolhe alpha conforme as probabilidades
        discrete_distribution<int> dist(probabilidades.begin(), probabilidades.end());
        int idx_alpha = dist(rng);
        double alpha = alphas[idx_alpha];

        // Executa o guloso randomizado normal
        vector<char> solucao = Guloso::guloso_randomizado(grafo, alpha, rng, 1);

        int custo = (int)solucao.size();
        if (custo > 0) {
            custos[idx_alpha] += custo;
            usos[idx_alpha]++;
            if (custo < melhor_custo) {
                melhor_custo = custo;
                melhor_solucao = solucao;
            }
        }

        // Atualiza probabilidades a cada bloco
        if ((i + 1) % bloco == 0) {
            // Calcula qualidade média (quanto menor, melhor)
            vector<double> qualidade(alphas.size(), 0.0);
            double soma = 0.0;
            for (size_t j = 0; j < alphas.size(); ++j) {
                if (usos[j] > 0)
                    qualidade[j] = 1.0 / (custos[j] / usos[j]);
                else
                    qualidade[j] = 0.0;
                soma += qualidade[j];
            }
            // Atualiza probabilidades proporcional à qualidade
            for (size_t j = 0; j < alphas.size(); ++j) {
                probabilidades[j] = (soma > 0) ? (qualidade[j] / soma) : (1.0 / alphas.size());
                custos[j] = 0.0;
                usos[j] = 0;
            }
        }
    }
    return melhor_solucao;
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