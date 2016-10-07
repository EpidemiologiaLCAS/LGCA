/**
 * @file AutomatoCelular.cpp
 * @brief Classe que representa um AC.
 * 
 * @author Rodrigo Marianni
 * @date 3/09/2010
 */
#include "AutomatoCelular.h"
#include <iomanip>
#include <iostream>

// Protótipos
AutomatoCelular::AutomatoCelular() { }
AutomatoCelular::AutomatoCelular(const AutomatoCelular& orig) { }

/** 
 * @brief Destrutor; desaloca a matriz (usando o método clear() da classe vector) 
 */
AutomatoCelular::~AutomatoCelular() {
    int i, j;
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i].clear();
        }
    }
    geracao = 0;
    matriz.clear();
}

/** 
 * @brief Inicializa o AC, alocando as células (classe vector da STL).
 */
void AutomatoCelular::inicializarAC() {
    int k, l;
    for (k=0;k<linhas;k++) {
        matriz.push_back(std::vector<Celula>());
        for (l=0;l<colunas;l++) {
            matriz[k].push_back(Celula());
        }
    }
}

/** 
 * @brief Define a proporção de infectados em cada átomo (pixel) da janela.
 */
void AutomatoCelular::prepararJanelaAC() {
    int i, j;
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            janela.definirPixel(i, j, matriz[i][j].infectados);
        }
    }
}

/** 
 * @brief Percorre a vizinhança de uma célula, calculando o total de infectados por deslocamento
 * 
 * Operação realizada conforme a expressão de Michael Höhle (Sundsmit Course, 2002).
 * 
 * @param	i	Linha.
 * @param	j	Coluna.
 * @return	double	Total de infectados por deslocamento.
 */
double AutomatoCelular::calcularInfectDeslocamento(const int& i, const int& j) {
    int l, m, coord;
    double tot=0.0, parcial;

    for (l=(i-1); l<=(i+1); l++) {
        for (m=(j-1); m<=(j+1); m++) {
            if (!((l == i) && (m == j)) &&
                (l>=0) && (l<linhas) && (m>=0) && (m<colunas)) {
                coord = obterCoordenadaVizinho(i, j, l, m);
                parcial = matriz[l][m].infectados *
                          matriz[i][j].probViagem[coord];
                tot += parcial;
            }
        }
    }
    return (tot * virulencia * matriz[i][j].suscetiveis);
}

/** 
 * @brief Efetua a transição entre dois estados do AC.
 * 
 * @see AutomatoCelular.h
 * 
 * @return	int	Total de indivíduos infectados.
 */
unsigned int AutomatoCelular::aplicarTransicao() {
    int i, j;
    double totI=0, totR=0, totS=0;
    double sus, rec;
    std::stringstream ss;

    // Passo 1: atualização dos valores
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            sus = virulencia * matriz[i][j].suscetiveis * matriz[i][j].infectados +
                  calcularInfectDeslocamento(i, j);
            rec = recuperacao * matriz[i][j].infectados;

            matriz[i][j].sTmp = matriz[i][j].suscetiveis - sus;
            matriz[i][j].iTmp = matriz[i][j].infectados + sus - rec;
            matriz[i][j].rTmp = matriz[i][j].removidos + rec;
        }
    }

    // Passo 2: atualização do AC
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].suscetiveis = matriz[i][j].sTmp;
            matriz[i][j].infectados = matriz[i][j].iTmp;
            matriz[i][j].removidos = matriz[i][j].rTmp;
            totI += matriz[i][j].infectados * matriz[i][j].populacao;
            totR += matriz[i][j].removidos * matriz[i][j].populacao;
            totS += matriz[i][j].suscetiveis * matriz[i][j].populacao;
        }
    }

    geracao++;

    ss << geracao << "\t" << totI << "\t" << totS << "\t" << totR;
    gravarDadosGrafico(ss.str());

    prepararJanelaAC();
    janela.atualizar();

    // vide AutomatoCelular.h
    if (geracao % INTERVALO_SCREENSHOTS == 0)
        janela.gravarScreenshot(geracao);

    return totI;
}

/** 
 * @brief Retorna o valor (da constante) da coordenada geográfica do vizinho de uma célula.
 * 
 * @see AutomatoCelular.h
 * 
 * @return	int	Constante da coordenada geográfica.
 */
int AutomatoCelular::obterCoordenadaVizinho(const int& i, const int& j, const int& i2, const int& j2)  {
    int resp;

    // assume que a vizinhança de Moore abrange a célula central
    resp = ((i2-i+1)*3) + (j2-j+1);
    // logo, é preciso corrigir os índices da última linha...
    if (resp > 4)
            resp--;

    return (resp);
}

/** 
 * @brief Carrega as configurações do AC a partir de um arquivo.
 * 
 * @see AutomatoCelular.h
 *
 * @return	bool	Representa o sucesso da operação.
 */
bool AutomatoCelular::carregarConfig(const std::string& nomeArq) {

    std::ifstream arq;
    std::string linha, dado;
    int i=0, j=0, k, l, pop=0, tamCel=2, delay=10;
    float infect=0.0, prob[TOT_VIZINHOS];

    arq.open(nomeArq.c_str(), std::ios::in); // abre o arquivo

    if (!arq.is_open())
        return false;

    if (!arq.good())
        return false;

    // lê a primeira linha
    getline(arq, linha);

    // cria um stream a partir da linha
    std::istringstream iss(linha.c_str());

    // percorre o stream lendo as configurações;
    /*
     * Formato:
     * int linhas, int colunas, float virulência, float recuperacao,
     * int tamanhoCelula, int numGeracoes, int delay
    */
    k = 1;
    while (getline(iss, dado, '\t')) {
        switch (k) {
            case 1:
                linhas = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 2:
                colunas = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 3:
                virulencia = AutomatoCelular::stringParaDado<float>(dado);
                break;
            case 4:
                recuperacao = AutomatoCelular::stringParaDado<float>(dado);
                break;
            case 5:
                tamCel = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 6:
                maxGeracoes = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 7:
                delay = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
        }
        k++;
    }

    geracao = 0;

    // inicializa o AC
    this->inicializarAC();

    // lê as demais linhas
    while (getline(arq, linha)) {
        // cria um stream a partir da linha lida
        std::istringstream iss(linha.c_str());

        // tokeniza o stream e lê os dados
        /* Formato:
         * int linha, int coluna, int populacao, float infect,
         * 8 x o seguinte par: float conect[n], float mobil[n];
         * a ordem das coordenadas é a das constantes
         * (NW, N, NE, W, E, SW, S, SE)
         */
        k = 1;
        l = 0;
        while (getline(iss, dado, '\t')) {
            if (k == 1)
                i = AutomatoCelular::stringParaDado<unsigned int>(dado);
            else if (k == 2)
                j = AutomatoCelular::stringParaDado<unsigned int>(dado);
            else if (k == 3)
                pop = AutomatoCelular::stringParaDado<unsigned int>(dado);
            else if (k == 4)
                infect = AutomatoCelular::stringParaDado<float>(dado);
            else if (k%2 == 0) {
                prob[l] = AutomatoCelular::stringParaDado<float>(dado);
                l++;
            }
            k++;
        }

        // define os dados da célula
        matriz[i][j].populacao = pop;
        matriz[i][j].infectados = infect;
        matriz[i][j].suscetiveis = 1 - infect;
        matriz[i][j].removidos = 0;
        for (k=0; k<TOT_VIZINHOS; k++)
            matriz[i][j].probViagem[k] = prob[k];
    }

    // fecha o arquivo e finaliza
    arq.close();

    // tenta iniciar a janela SDL
    janela.definirDimensoes(colunas, linhas);
    janela.definirTamanhoCelula(tamCel);
    janela.definirDelay(delay);
    janela.inicializar();
    prepararJanelaAC();
    janela.atualizar();

    // grava o screenshot do estado inicial; vide AutomatoCelular.h
    janela.gravarScreenshot(geracao);

    return true;

}

/**
 * @brief Função que imprime dados de debug
*/
void AutomatoCelular::DEBUG_Imprimir() {
    int i, j;

    // S
    std::cout << "Suscetíveis:" << std::endl;
    for (i=0;i<linhas;i++) {
        for (j=0;j<colunas;j++) {
            std::cout << matriz[i][j].suscetiveis << " ";
        }
        std::cout << std::endl;
    }

    // I
    std::cout << "Infectados:" << std::endl;
    for (i=0;i<linhas;i++) {
        for (j=0;j<colunas;j++) {
            std::cout << matriz[i][j].infectados << " ";
        }
        std::cout << std::endl;
    }

    // R
    std::cout << "Removidos:" << std::endl;
    for (i=0;i<linhas;i++) {
        for (j=0;j<colunas;j++) {
            std::cout << matriz[i][j].removidos << " ";
        }
        std::cout << std::endl;
    }
}

/** 
 * @brief Grava dados em um arquivo para a geração de gráfico via gnuplot.
 * 
 * @see AutomatoCelular.h (caminho do arquivo de dados do gráfico)
 * 
 * @param	String	Linha com os dados a gravar.
 */
void AutomatoCelular::gravarDadosGrafico(const std::string& linha) {
    if (!arqGraf.gravarLinha(FILE_HOHLE_GRAFICO, linha))
        std::cerr << "Erro ao gravar dados no arquivo de gráfico." << std::endl;
}
