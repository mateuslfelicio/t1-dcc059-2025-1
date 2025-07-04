#include "Gerenciador.h"
#include <fstream>


void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl<<endl;
            }


            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            if(caminho_minimo_dijkstra.empty()) {
                cout << "Caminho mínimo não encontrado." << endl;
                break;
            }

            for(size_t i = 0; i < caminho_minimo_dijkstra.size(); ++i) {
                cout << caminho_minimo_dijkstra[i];
                if(i != caminho_minimo_dijkstra.size() - 1)
                    cout << ",";
            }
            cout << endl << endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                fstream arquivo;
                arquivo.open("caminho_minimo_dijkstra.txt", ios::out);
                if(!arquivo.is_open()) {
                    cerr << "Erro ao abrir o arquivo: caminho_minimo_dijkstra.txt" << endl;
                    break;
                }

                
                for(size_t i = 0; i < caminho_minimo_dijkstra.size(); ++i) {
                    arquivo << caminho_minimo_dijkstra[i];
                    if(i != caminho_minimo_dijkstra.size() - 1)
                        arquivo << ",";
                }
                arquivo << endl;
                cout<< "Caminho mínimo gravado em caminho_minimo_dijkstra.txt" << endl;
                arquivo.close();
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            if(caminho_minimo_floyd.empty()) {
                cout << "Caminho mínimo não encontrado." << endl;
                break;
            }
            
            for(size_t i = 0; i < caminho_minimo_floyd.size(); ++i) {
                cout << caminho_minimo_floyd[i];
                if(i != caminho_minimo_floyd.size() - 1)
                    cout << ",";
            }
            cout << endl << endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                fstream arquivo;
                arquivo.open("caminho_minimo_floyd.txt", ios::out);
                if(!arquivo.is_open()) {
                    cerr << "Erro ao abrir o arquivo: caminho_minimo_floyd.txt" << endl;
                    break;
                }

                for(size_t i = 0; i < caminho_minimo_floyd.size(); ++i) {
                    arquivo << caminho_minimo_floyd[i];
                    if(i != caminho_minimo_floyd.size() - 1)
                        arquivo << ",";
                }
                arquivo << endl;
                cout<< "Caminho mínimo gravado em caminho_minimo_floyd.txt" << endl;
                arquivo.close();
            }


            break;
        }
        case 'e': {
            
            int tam;
            cout << "Digite o tamanho do subconjunto: ";
            cin >> tam;
        
            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                if (arvore_geradora_minima_prim == nullptr) {
                    cout << "Nao existe AGM: grafo desconexo." << endl;
                    cout << endl;
                    break;
                } else{
                cout << "Arvore Geradora Minima (Prim):" << endl;
                arvore_geradora_minima_prim->print();
                cout << endl;
                }
                if (pergunta_imprimir_arquivo("agm_prim.txt")) {
                    arvore_geradora_minima_prim->gravar("agm_prim.txt");
                    cout << "AGM (Prim) gravada em agm_prim.txt" << endl << endl;
                }
                delete arvore_geradora_minima_prim;
            } else {
                cout << "Algo deu errado." << endl;
                break;
            }
            break;
        }

        case 'f': {

            int tam;
            cout << "Digite o tamanho do subconjunto: ";
            cin >> tam;
        
            if (tam > 0 && tam <= grafo->ordem) {
                vector<char> ids = get_conjunto_ids(grafo, tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
               if(arvore_geradora_minima_kruskal == nullptr){
                cout << "Nao existe AGM: grafo desconexo." << endl;
                cout << endl;
                break;
               }
               else{
                cout << "Arvore Geradora Minima (Kruskal):" << endl;
                arvore_geradora_minima_kruskal->print();
                cout << endl;
               }
                if (pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    arvore_geradora_minima_kruskal->gravar("agm_kruskal.txt");
                    cout << "AGM (kruskal) gravada em agm_kruskal.txt" << endl << endl;
                }
                delete arvore_geradora_minima_kruskal;
            } else {
                cout << "Algo deu errado." << endl;
                break;
            }
            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            arvore_caminhamento_profundidade->print();

            cout << endl;


            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                arvore_caminhamento_profundidade->gravar("arvore_caminhamento_profundidade.txt");
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            /*
            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;
            */
            int raio, diametro;
            vector<char> centro, periferia;
            raio = grafo->raio();
            diametro = grafo->diametro();
            centro = grafo->centro();
            periferia = grafo->periferia();

            if(centro.empty() && periferia.empty()) {
                cout<<"Grafo vazio ou sem conexao"<<endl;
                break;
            } else {
                cout<<"Raio: "<<raio<<endl;
                cout<<"Diametro: "<<diametro<<endl;
                cout<<"Centro: ";
                if(centro.size() == 1)
                    cout << centro[0];
                else
                    for(char id : centro)
                        cout<<id<<",";
                cout<<endl;
                cout<<"Periferia: ";
                if(periferia.size() == 1)
                    cout << periferia[0] << endl;
                else
                    for(char id : periferia)
                        cout<<id<<",";
                cout<<endl<<endl;
            }
            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                fstream arquivo;
                arquivo.open("raio_diametro.txt", ios::out);
                if(!arquivo.is_open()) {
                    cerr << "Erro ao abrir o arquivo: raio_diametro.txt" << endl;
                    break;
                }
                if(centro.empty() && periferia.empty()) {
                    arquivo<<"Grafo vazio ou sem conexao"<<endl;
                    break;
                } else {
                    arquivo<<"Raio: "<<raio<<endl;
                    arquivo<<"Diametro: "<<diametro<<endl;
                    arquivo<<"Centro: ";
                    if(centro.size() == 1)
                        arquivo << centro[0];
                    else
                        for(char id : centro)
                            arquivo<<id<<",";
                    arquivo<<endl;
                    arquivo<<"Periferia: ";
                    if(periferia.size() == 1)
                        arquivo << periferia[0] << endl;
                    else
                        for(char id : periferia)
                            arquivo<<id<<",";
                    arquivo<<endl<<endl;
                }
                cout << "Dados gravados em raio_diametro.txt" << endl;
                arquivo.close();
            }

            break;
        }

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}