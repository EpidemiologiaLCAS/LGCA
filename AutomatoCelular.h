/* 
 * File:   AutomatoCelular.h
 * Author: Rodrigo Marianni
 *
 * Created on 3 de Agosto de 2010, 12:01
 */

#ifndef AUTOMATOCELULAR_H
#define	AUTOMATOCELULAR_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Celula.h"
#include "JanelaSDL.h"
#include "Arquivo.h"

#define FILE_HOHLE_GRAFICO "./dados_grafico_hohle.txt"
#define INTERVALO_SCREENSHOTS 5

class AutomatoCelular {
public:
    AutomatoCelular();
    AutomatoCelular(const AutomatoCelular& orig);
    virtual ~AutomatoCelular();

    bool carregarConfig(const std::string &nomeArq);
    void gravarDadosGrafico(const std::string &linha);
    unsigned int aplicarTransicao(void);
    void DEBUG_Imprimir();

    // função com template para conversão de dados
    template<typename T> static T stringParaDado(const std::string& s) {
        std::istringstream stream(s);
        T t;
        stream >> t;
        return t;
    }
private:
    float virulencia, recuperacao;
    int linhas, colunas;
    unsigned int maxGeracoes, geracao;
    std::vector<std::vector<Celula> > matriz;
    JanelaSDL janela;

    void inicializarAC();
    void prepararJanelaAC();
    int obterCoordenadaVizinho(const int &i, const int &j, const int &i2, const int &j2);
    double calcularInfectDeslocamento(const int &i, const int &j);
    Arquivo arqGraf;
};

#endif	/* AUTOMATOCELULAR_H */

