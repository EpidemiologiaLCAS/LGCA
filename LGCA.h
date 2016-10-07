/* 
 * File:   LGCA.h
 * Author: Rodrigo Marianni
 *
 * Baseado no artigo
 * "Individual-based lattice model for spatial spread of epidemics",
 * de Henryk Fuks and Anna T. Lawniczak (2001).
 *
 * Obs.: o usuário deve invocar o médodo inicializar(), para definir o número
 * de infectados, antes de tentar efetuar simulações com o LGCA.
 *
 * Created on 30 de Agosto de 2010, 22:09
 */

#ifndef LGCA_H
#define	LGCA_H

#include <vector>
#include <iostream>
#include <cmath>
#include "CelulaLGCA.h"
#include "JanelaSDL.h"
#include "AutomatoCelular.h"
#include "Arquivo.h"
#include "CampoMedio.h"

#define FILE_GRAFICO_IR "./lgca_dados_grafico_IR.txt"
#define FILE_GRAFICO_S "./lgca_dados_grafico_S.txt"
#define FILE_GRAFICO_MF "./lgca_dados_grafico_MF.txt"
#define FILE_ESTADO "./lgca_estado_inicial.txt"

/* TODO:
 * - reescrever as classes AC e LGCA para utilizar herança...
 *      - generalizar o código para lattices hexagonais...
 * - reescrever o acesso a matrizes usando a STL (iterators em vez de acesso indexado)
 */
class LGCA {
public:
    LGCA(); // construtor vazio; útil para recuperar estados salvos;
    LGCA(const int &i, const int &j, const int &pixel, const int &delay,
         const int &geracoes, const float &v, const float &r); // construtor para inicialização direta
    LGCA(const LGCA& orig);
    virtual ~LGCA();
    void inicializar(const unsigned int &infectados, const unsigned int &suscetiveis);
    unsigned int aplicarTransicao(void);
    // métodos que permitem o armazenamento e recuperação de LGCAs inicializados;
    // úteis para reprodução de resultados e testes comparativos com diferentes parâmetros
    void salvarEstado(void);
    void carregarEstado(void);
private:
    // operações (p. 4 do artigo):
    unsigned int operacaoContato(void);
    unsigned int operacaoContato_TESTE(void);
    void totalizarIndividuos(int &totI, int &totR, int &totS);
    void operacaoRandomizacao(void);
    void operacaoPropagacao(void);
    // obtém a célula vizinha, dado um canal; deve retornar NULL se não houver vizinho naquela direção.
    CelulaLGCA* obterCelulaVizinha(const int &canal, const int &i, const int &j);
    void prepararJanelaAC(void);
    // operações de Arquivo
    void gravarDadosGraficoIR(const std::string &linha);
    void gravarDadosGraficoS(const std::string &linha);
    void gravarDadosGraficoMF(const std::string &linha);
    void calcularDistribuicaoIndividuos(void);

    std::vector<std::vector<CelulaLGCA> > matriz;
    JanelaSDL janela;
    int linhas, colunas;
    int maxGeracoes, geracao;
    float virulencia, recuperacao;
    bool inicializado;
    Arquivo arqGraficoIR, arqGraficoS, arqGraficoMF;
    CampoMedio meanField;
};

#endif	/* LGCA_H */

