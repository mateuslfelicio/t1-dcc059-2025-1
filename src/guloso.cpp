#include "guloso.h"

vector<char> Guloso::guloso(Grafo* grafo) {
    cout << "Método não implementado" << endl;
    //atuzalizar o estado de dominados sempre que inserir nó a solução
    // Deixar essa função como ultima coisa antes do return
    limpar_dominados(grafo);
    return {};
}

vector<char> Guloso::guloso_randomizado(Grafo* grafo) {
    cout << "Método não implementado" << endl;
    //atuzalizar o estado de dominados sempre que inserir nó a solução
    // Deixar essa função como ultima coisa antes do return
    limpar_dominados(grafo);
    return {};
}

vector<char> Guloso::guloso_randomizado_reativo(Grafo* grafo) {
    cout << "Método não implementado" << endl;
    //atuzalizar o estado de dominados sempre que inserir nó a solução
    // Deixar essa função como ultima coisa antes do return
    limpar_dominados(grafo);
    return {};
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
    vector<pair<char, int>> pares;
    
    // Calcula a  quantidade de vértices afetados por cada nó, descartando os já dominados
    for(No* no : grafo->lista_adj) {
        if(no->dominado) continue;
        pares.push_back(make_pair(no->id, arestas_livres(no, grafo)));
    }
    
    // Ordena os pares pela quantidade de arestas em ordem decrescente
    sort(pares.begin(), pares.end(), [](const pair<char,int>& a, const pair<char,int>& b) {
        return a.second > b.second;
    });

    // Cria um vetor de caracteres ordenados
    vector<char> ordenados;
    for(pair<char,int> par: pares) {
        ordenados.push_back(par.first);
    }

    return ordenados;
}


/***
 * @brief Conta o número de arestas livres de um nó
 * @param no Nó a ser verificado
 * @param grafo Grafo onde o nó está inserido
 * @return Número de arestas livres do nó
 * @note Uma aresta é considerada livre se o nó de destino não está dominado
 */
int Guloso::arestas_livres(No* no, Grafo* grafo) {
    int livres = 0;
    for(Aresta* aresta : no->arestas) {
        No* dest = grafo->buscar_no(aresta->id_no_alvo);
        if(dest && !dest->dominado)
            livres++;
    }
    return livres;
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