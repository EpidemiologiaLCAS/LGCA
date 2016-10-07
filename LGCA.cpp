/* 
 * File:   LGCA.cpp
 * Author: Rodrigo Marianni
 *
 * Baseado no artigo
 * "Individual-based lattice model for spatial spread of epidemics",
 * de Henryk Fuks and Anna T. Lawniczak (2001).
 * 
 * Created on 30 de Agosto de 2010, 22:09
 */
#include "LGCA.h"

LGCA::LGCA() {
    linhas=colunas=0;
}

LGCA::LGCA(const int& i, const int& j, const int& pixel, const int &delay,
           const int& geracoes, const float& v, const float& r):linhas(i), colunas(j),
           maxGeracoes(geracoes), virulencia(v), recuperacao(r) {

    int k, l;

    srand((unsigned int) time(0)); // inicializa a semente de números aleatórios

    janela.definirDimensoes(linhas, colunas);
    janela.definirTamanhoCelula(pixel);
    janela.definirDelay(delay);
    geracao = 0;

    // inicializa a matriz
    for (k=0;k<linhas;k++) {
        matriz.push_back(std::vector<CelulaLGCA>());
        for (l=0;l<colunas;l++) {
            matriz[k].push_back(CelulaLGCA());
        }
    }

    inicializado = false; // falta definir o número de infectados;
    
}

// distribui os infectados pelas células do LGCA (p. 3, final)
void LGCA::inicializar(const unsigned int& infectados, const unsigned int& suscetiveis) {
    int k, l, m, n;
    unsigned int s;
    int vetor[linhas*colunas]; // vide passo 2, abaixo

    // define os dados de campo médio, para efeito de comparação
    meanField.definirParametros(infectados, suscetiveis, 0, virulencia, recuperacao);

    // passo 1: infectados
    s = infectados;
    while (s > 0) {
        m = CelulaLGCA::gerarAleatorio(0, linhas-1);
        n = CelulaLGCA::gerarAleatorio(0, colunas-1);
        for (k=0; k<L; k++)
            if (matriz[m][n].obterEstado(k) == V) {
                matriz[m][n].definirEstado(k, I);
                s--;
                break;
            }
    }

    // passo 2: suscetiveis
    // Como o número de suscetíveis é em geral muito grande, usa outro método (mais seguro)
    // para a disposição aleatória de suscetíveis: inicializa um vetor com o número de
    // células; define cada posição do vetor com um número sequencial; e então embaralha
    // esse vetor. Dispõe os suscetíveis, então, nas células vazias disponíveis
    for (k=0; k<(linhas*colunas); k++) {
        vetor[k] = k;
    }
    CelulaLGCA::embaralharVetorInteiro(vetor, (linhas*colunas));
    s = suscetiveis;

    while (s > 0) {
        for (k=0; k<(linhas*colunas); k++) {
            m = vetor[k]/linhas; // obtém o índice da linha... (divisão inteira)
            n = vetor[k]%colunas; // ... e da coluna (módulo)

            // tenta achar uma posição vazia para inserir o sujeito
            for (l=0; l<L; l++) {
                if (matriz[m][n].obterEstado(l) == V) {
                    matriz[m][n].definirEstado(l, S);
                    s--;
                    break; // rompe o laço para forçar a próxima célula
                }
            }
            if (s == 0)
                break;
        }
    }

    inicializado = true;
    salvarEstado();
    janela.inicializar();
    prepararJanelaAC();
    janela.atualizar();

    // grava a distribuição de suscetíveis
    calcularDistribuicaoIndividuos();

    // grava o estado inicial do AC; vide AutomatoCelular.h
    janela.gravarScreenshot(geracao);
}

LGCA::LGCA(const LGCA& orig) {

}

LGCA::~LGCA() {
    int i, j;
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i].clear();
        }
    }
    matriz.clear();
}

/* Efetua o contato entre os indivíduos (calculando novas infecções),
 * além de promover a recuperação; a etapa é feita em cada célula
 * independentemente das demais.
 * - p. 4:
 *      - cada suscetível pode se infectar com probabilidade 1-(1-r)^(ni),
 * sendo r a 'virulência' (probabilidade de infecção) e ni o número de infectados
 * na célula.
 *      - cada infectado pode se recuperar com probabilidade a, sendo 0 <= a <= 1
 *
 * Retorna o novo número de infectados no LGCA.
 */
unsigned int LGCA::operacaoContato() {
    int i, j, k, ni;
    int numI, numR, numS, totI=0, totR=0, totS=0; // DEBUG
    unsigned int total=0;
    float r,  prob;

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            // só atua se houver infectados na célula
            ni = matriz[i][j].obterNi();
            // Obviamente, a ordem das operações afeta o resultado; é necessário
            // primeiro recuperar, para depois infectar.

            // recuperação:
            // para cada infectado na célula, sorteia um float entre zero e um;
            // se condizente com a probabilidade, recupera o sujeito;
            if (ni > 0) {
                for (k=0; k<L; k++) {
                    if (matriz[i][j].obterEstado(k) == I) {
                        r = ((float) CelulaLGCA::gerarAleatorio(0, 1000)) / 1000;
                        if (r < recuperacao)
                            matriz[i][j].definirEstado(k, R);
                    }
                }
            }

            // infecção:
            // para cada suscetível na célula, sorteia um float entre zero e um;
            // se condizente com a probabilidade, infecta o sujeito;
            ni = matriz[i][j].obterNi(); // obtém novamente o número de infectados
            prob = 1 - pow(1-virulencia, ni);
            for (k=0; k<L; k++) {
                if (matriz[i][j].obterEstado(k) == S) {
                    r = ((float) CelulaLGCA::gerarAleatorio(0, 1000)) / 1000;
                    
                    if (r < prob) {
                        matriz[i][j].definirEstado(k, I);
                        total++;
                    }
                }
            }
            // ni = matriz[i][j].obterNi(); // obtém novamente o número de infectados
            // total += ni;

            // ========
            // DEBUG
            matriz[i][j].obterQuantidadeIndividuos(numI, numR, numS);
            totI += numI;
            totR += numR;
            totS += numS;
            // ========
        }
    }
    return totI;
}

/* Efetua o contato entre os indivíduos (calculando novas infecções),
 * além de promover a recuperação; a etapa é feita em cada célula
 * independentemente das demais.
 * - p. 4:
 *      - cada suscetível pode se infectar com probabilidade 1-(1-r)^(ni),
 * sendo r a 'virulência' (probabilidade de infecção) e ni o número de infectados
 * na célula.
 *      - cada infectado pode se recuperar com probabilidade a, sendo 0 <= a <= 1
 *
 * Retorna o novo número de infectados no LGCA.
 */
unsigned int LGCA::operacaoContato_TESTE() {
    int i, j, k, ni;
    // int numI, numR, numS, totI=0, totR=0, totS=0; // DEBUG
    unsigned int total=0;
    float r,  prob;

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            // Obviamente, a ordem das operações afeta o resultado; é necessário
            // primeiro recuperar, para depois infectar (de modo a evitar "remoções instantâneas").

            // ====================================
            ni = matriz[i][j].obterNi(); // obtém o número de infectados
            // recuperação:
            // para cada infectado na célula, sorteia um float entre zero e um;
            // se condizente com a probabilidade, recupera o sujeito;
            if (ni > 0) {
                for (k=0; k<L; k++) {
                    if (matriz[i][j].obterEstado(k) == I) {
                        r = ((float) CelulaLGCA::gerarAleatorio(0, 1000)) / 1000;
                        if (r < recuperacao)
                            matriz[i][j].definirEstado(k, R);
                    }
                }
            }

            // ====================================
            // infecção:
            // para cada suscetível na célula, sorteia um float entre zero e um;
            // se condizente com a probabilidade, infecta o sujeito;
            ni = matriz[i][j].obterNi(); // obtém novamente o número de infectados
            prob = 1 - pow(1-virulencia, ni);
            for (k=0; k<L; k++) {
                if (matriz[i][j].obterEstado(k) == S) {
                    r = ((float) CelulaLGCA::gerarAleatorio(0, 1000)) / 1000;

                    if (r < prob) {
                        matriz[i][j].definirEstado(k, I);
                        total++;
                    }
                }
            }

            // ====================================
            ni = matriz[i][j].obterNi(); // obtém novamente o número de infectados
            total += ni;

            // ========
            // DEBUG
            /*
            matriz[i][j].obterQuantidadeIndividuos(numI, numR, numS);
            totI += numI;
            totR += numR;
            totS += numS; */
            // ========
        }
    }
    // ====
    // DEBUG
    // std::cout << "Totais (I, R, S) = " << totI << ", " << totR << ", " << totS << std::endl;
    // ====
    return total;
}

/* Efetua a randomização dos indivíduos para os canais de saída;
 * tal etapa é feita em cada célula independentemente das demais.
 * - p. 4:
 *      - no máximo um indivíduo por canal.
 * De fato, esta implementação apenas embaralha os indivíduos;
 * posteriormente (na etapa de propagação), cada indivíduo será enviado
 * para o canal adequado (acessado sempre pelo índice).
 * Ex.:
 * Lattice retangular:
 * Canais:      | 0 | 1 | 2 | 3 |
 * Indivíduos:  | S | S | I | R | (antes da operação de randomização)
 * Indivíduos:  | R | S | S | I | (depois da operação de randomização)
  */
void LGCA::operacaoRandomizacao() {
    int i, j;

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].embaralharIndividuos();
        }
    }
}

/* Efetua a propagação dos indivíduos pelos canais selecionados.
 * O artigo não especifica as regras de colisão (esse conhecimento é, aparentemente,
 * pré-requisito); para lattices retangulares, a regra de colisão mais evidente
 * implica em devolver a partícula ao nó de origem (ignora-se, aqui, a velocidade de propagação).
 * Já para lattices hexagonais é possível definir diferentes regras de colisão,
 * dependendo do modo com que se processam os ângulos de incidência das partículas.
 * - p. 4:
 *      - os indivíduos se movem SIMULTANEAMENTE; é necessário preservar, portanto,
 *        o estado anterior da célula, processar todos os deslocamentos e então
 *        atualizar o estado da célula.
 *
 * Cada célula altera apenas seu próprio estado.
 * 
 * A propagação (em células retangulares) se dá pela seguinte regra, aplicada em cada célula:
 * - se ambos os canais são diferentes de V, copia o conteúdo do estado atual para o temporário (colisão);
 * - se o atual é V, copia o oposto do vizinho, independente do que for.
 * - do contrário, o canal é V.
 * Ao final disso, efetua-se a atualização do estado de cada célula (cópia do
 * temporário para o final).
 */
void LGCA::operacaoPropagacao() {
    int i, j, k;
    CelulaLGCA* cel = NULL;

    // ATENÇÃO: implementação APENAS para lattice retangular
    // passo 1: atualização dos estados temporários
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            // laço dos vizinhos
            for (k=0; k<L; k++) {
                // se o vizinho é válido, ...
                cel = obterCelulaVizinha(k, i, j);
                if (cel != NULL) {
                    // ... processa a regra:
                    // - se ambos os canais são diferentes de V, copia
                    // o conteúdo do estado atual para o temporário (colisão);
                    if ((cel->obterEstadoOposto(k) != V) && (matriz[i][j].obterEstado(k) != V))
                        matriz[i][j].definirCanal(k, matriz[i][j].obterEstado(k));
                    // se o atual é V, copia o oposto do vizinho, independente do que for.
                    else if (matriz[i][j].obterEstado(k) == V)
                        matriz[i][j].definirCanal(k, cel->obterEstadoOposto(k));
                    else
                        matriz[i][j].definirCanal(k, V);
                } else
                    std::cerr << "ERRO!!! CÉLULA NULA!!!!!!" << std::endl;
            }
        }
    }

    // passo 2: atualização do estado final
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].atualizarCelula();
        }
    }
}

/* Obtém a célula vizinha, dado um canal; 
 * retorna NULL se não houver vizinho naquela direção.
 * Escrito para retornar um ponteiro para célula, porque isso permite que
 * o método seja polimórfico e trate outros tipos de lattice.
 * 
 * ATENÇÃO: atualmente, válido apenas para lattices retangulares.
 */
CelulaLGCA* LGCA::obterCelulaVizinha(const int& canal, const int& i, const int& j) {
    int l=-1, m=-1;
    CelulaLGCA* c = NULL;

    // calcula os índices do vizinho
    if ((canal == 0) || (canal == 2)) {
        m = j;
        l = (canal == 0 ? (i-1) : (i+1));
    } else if ((canal == 1) || (canal == 3)) {
        l = i;
        m = (canal == 1 ? (j+1) : (j-1));
    }

    // valida os índices e, se válidos, retorna um ponteiro para a célula.
    if (l < 0)
        l = (linhas-1);
    else if (l == linhas)
        l = 0;

    if (m < 0)
        m = (colunas-1);
    else if (m == colunas)
        m = 0;

    c = &(matriz[l][m]);

    return c;
}

// Percorre a matriz, enviando os dados para a janela do AC.
// ATENÇÃO: como o método definirPixel() espera um float, obtém o número de
//          infectados e divide pela capacidade da célula (número de canais ou lados).
void LGCA::prepararJanelaAC() {
    int i, j, numI=0, numR=0, numS=0;
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            // Obtém os totais de indivíduos
            matriz[i][j].obterQuantidadeIndividuos(numI, numR, numS);
            // Para a visualização simultânea das três classes:
            janela.definirPixel(i, j, 
                                ((float) numI)/L, // infectados
                                ((float) numR)/L, // removidos
                                ((float) numS)/L // suscetíveis
                               );
            // Para a visualização de apenas uma classe:
            // janela.definirPixel(i, j, (float) numR/L);
        }
    }
}

void LGCA::totalizarIndividuos(int& totI, int& totR, int& totS) {
    int i, j;
    int numI, numR, numS;

    totI = totR = totS = 0;

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].obterQuantidadeIndividuos(numI, numR, numS);
            totI += numI;
            totR += numR;
            totS += numS;
        }
    }
}

// aplica a transição de estados no LGCA
unsigned int LGCA::aplicarTransicao() {
    std::stringstream ss;
    int totI, totR, totS;
    unsigned int mfI=0, mfR=0, mfS=0;

    if (inicializado) {
        if (geracao <= maxGeracoes) {
            // =================================
            // Transição
            operacaoContato();
            operacaoPropagacao();
            operacaoRandomizacao();
            prepararJanelaAC();
            janela.atualizar();

            totalizarIndividuos(totI, totR, totS);

            // =================================
            // Aplicação da dinâmica do campo médio, para efeitos de comparação
            meanField.computarGeracao(mfI, mfR, mfS);

            // =================================
            // Gravação de dados para os gráficos
            ss << geracao << "\t" << totI << "\t" << totR;
            gravarDadosGraficoIR(ss.str());

            ss.str(""); // limpa a string

            ss << geracao << "\t" << totS;
            gravarDadosGraficoS(ss.str());

            ss.str("");

            ss << geracao << "\t" << totI << "\t" << mfI;
            gravarDadosGraficoMF(ss.str());

            geracao++;

            // vide AutomatoCelular.h
            if (geracao % INTERVALO_SCREENSHOTS == 0)
                janela.gravarScreenshot(geracao);
        }
    }

    return totI;
}

// grava os dados do gráfico; o nome de arquivo é padrão (vide constante em LGCA.h)
void LGCA::gravarDadosGraficoIR(const std::string &linha) {
    if (!arqGraficoIR.gravarLinha(FILE_GRAFICO_IR, linha))
        std::cerr << "Erro ao gravar dados no arquivo de gráfico (IR)." << std::endl;
}
// grava os dados do gráfico; o nome de arquivo é padrão (vide constante em LGCA.h)
void LGCA::gravarDadosGraficoS(const std::string &linha) {
    if (!arqGraficoS.gravarLinha(FILE_GRAFICO_S, linha))
        std::cerr << "Erro ao gravar dados no arquivo de gráfico (S)." << std::endl;
}
// grava os dados do gráfico; o nome de arquivo é padrão (vide constante em LGCA.h)
void LGCA::gravarDadosGraficoMF(const std::string &linha) {
    if (!arqGraficoMF.gravarLinha(FILE_GRAFICO_MF, linha))
        std::cerr << "Erro ao gravar dados no arquivo de gráfico (MF)." << std::endl;
}

// grava o estado do LGCA em um arquivo; o nome de arquivo é padrão (vide constante em LGCA.h)
void LGCA::salvarEstado() {
    int i, j, k, numI, numR, numS;
    Arquivo arqEstado;
    std::string s;
    std::stringstream ss;
    unsigned int pixels, atraso;

    // obtém informações adicionais da janela SDL
    janela.obterParametros(atraso, pixels);

    // grava uma linha com a descrição do LGCA (parâmetros de inicialização, em sequência)
    ss << linhas << "\t" << colunas << "\t" << pixels << "\t" << atraso << "\t";
    ss << maxGeracoes << "\t" << virulencia << "\t" << recuperacao;

    if (!arqEstado.gravarLinha(FILE_ESTADO, ss.str())) {
        std::cerr << "Erro ao gravar dados no arquivo de estados." << std::endl;
        return;
    }

    // percorre a matriz, gravando o estado; 
    // é de responsabilidade do usuário chamar o método apenas após a inicialização do LGCA;
    // armazena apenas células em que há algum indivíduo; 
    // guarda linha e coluna, a posição do indivíduo e o estado
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].obterQuantidadeIndividuos(numI, numR, numS);
            if ((numI > 0) || (numR != 0) || (numS != 0)) {
                for (k=0; k<L; k++) {
                    if (matriz[i][j].obterEstado(k) != V) {
                        ss.str("");
                        ss << i << "\t" << j << "\t" << k << "\t" << matriz[i][j].obterEstado(k);
                        if (!arqEstado.gravarLinha(FILE_ESTADO, ss.str())) {
                            std::cerr << "Erro ao gravar dados no arquivo de estados." << std::endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}

// lê o estado do LGCA a partir de um arquivo
void LGCA::carregarEstado() {
    int i, j=0, k, canal, estado;
    Arquivo arqEstado;
    std::string s, dado;
    unsigned int pixels, atraso, totI=0, totS=0;

    // obtém a primeira linha (configurações iniciais)
    arqEstado.obterLinha(FILE_ESTADO, s);
    // converte em input string stream
    std::istringstream iss(s.c_str());

    // obtém os tokens de inicialização;
    // ATENÇÃO: assume que o arquivo está no formato correto.
    // Obs.: está 'fora de ordem' pois é o mesmo código da carga de configurações do AC...
    k=1;
    while (getline(iss, dado, '\t')) {
        switch (k) {
            case 1:
                linhas = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 2:
                colunas = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 6:
                virulencia = AutomatoCelular::stringParaDado<float>(dado);
                break;
            case 7:
                recuperacao = AutomatoCelular::stringParaDado<float>(dado);
                break;
            case 3:
                pixels = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 5:
                maxGeracoes = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
            case 4:
                atraso = AutomatoCelular::stringParaDado<unsigned int>(dado);
                break;
        }
        k++;
    }
    // =============================================
    // Seção idêntica à do construtor
    janela.definirDimensoes(linhas, colunas);
    janela.definirTamanhoCelula(pixels);
    janela.definirDelay(atraso);
    geracao = 0;

    // inicializa a matriz
    for (k=0;k<linhas;k++) {
        matriz.push_back(std::vector<CelulaLGCA>());
        for (j=0;j<colunas;j++) {
            matriz[k].push_back(CelulaLGCA());
        }
    }

    // =============================================
    // primeiro, inicializa todos os canais com valores "vazios"
    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            for (k=0; k<L; k++)
                matriz[i][j].definirEstado(k, V);
        }
    }

    // depois, carrega os estados não vazios na matriz;
    // obtém uma linha...
    while (arqEstado.obterLinha(FILE_ESTADO, s)) {
        // ... e a converte em input string stream
        std::istringstream iss(s.c_str());
        // formato: linha coluna canal estado
        k=1;
        // obtém os dados de uma linha...
        while (getline(iss, dado, '\t')) {
            switch (k) {
                case 1:
                    i = AutomatoCelular::stringParaDado<unsigned int>(dado);
                    break;
                case 2:
                    j = AutomatoCelular::stringParaDado<unsigned int>(dado);
                    break;
                case 3:
                    canal = AutomatoCelular::stringParaDado<unsigned int>(dado);
                    break;
                case 4:
                    estado = AutomatoCelular::stringParaDado<unsigned int>(dado);
                    break;
            }
            k++;
        }
        // ... e carrega o estado lido na matriz
        matriz[i][j].definirEstado(canal, estado);
        // contabiliza infectados e suscetíveis, para a inicialização do Campo Médio
        if (estado == I)
            totI++;
        else if (estado == S)
            totS++;
    }

    // =============================================
    // Também define os dados de campo médio, para efeito de comparação
    meanField.definirParametros(totI, totS, 0, virulencia, recuperacao);
    
    inicializado = true;
    janela.inicializar();
    prepararJanelaAC();
    janela.atualizar();

    // grava o estado inicial do AC; vide AutomatoCelular.h
    janela.gravarScreenshot(geracao);
}

void LGCA::calcularDistribuicaoIndividuos() {
    unsigned int tot0=0, tot1=0, tot2=0, tot3=0, tot4=0;
    int qI, qR, qS;
    int i, j;
    std::stringstream ss;
    Arquivo dados;

    for (i=0; i<linhas; i++) {
        for (j=0; j<colunas; j++) {
            matriz[i][j].obterQuantidadeIndividuos(qI, qR, qS);
            switch (qS) {
                case 0:
                    tot0++;
                    break;
                case 1:
                    tot1++;
                    break;
                case 2:
                    tot2++;
                    break;
                case 3:
                    tot3++;
                    break;
                case 4:
                    tot4++;
            }
        }
    }

    ss << "Quantidade de suscetíveis por célula:" << std::endl;
    dados.gravarLinha("distribuicao.txt", ss.str());
    ss.str("");
    ss << "0: " << tot0 << "\t1:" << tot1 << "\t2:" << tot2 << "\t3:" << tot3 << "\t4:" << tot4;
    dados.gravarLinha("distribuicao.txt", ss.str());

}