/* 
 * File:   CampoMedio.h
 * Author: marianni
 *
 * Created on 15 de Setembro de 2010, 08:30
 */

#ifndef CAMPOMEDIO_H
#define	CAMPOMEDIO_H

#include <iostream>
#include "Arquivo.h"
#define FILE_DADOS_GRAFICO "dados_grafico_campo_medio.txt"

class CampoMedio {
public:
    // construtores
    CampoMedio();
    CampoMedio(const unsigned int &inf, const unsigned int &sus, const unsigned int &rem,
               const float &v, const float &r);
    CampoMedio(const CampoMedio& orig);
    virtual ~CampoMedio();
    void definirParametros(const unsigned int &inf, const unsigned int &sus, const unsigned int &rem,
                           const float &v, const float &r);
    unsigned int computarGeracao(unsigned int &inf, unsigned int &rem, unsigned int &sus);
    unsigned int computarGeracao(void); // função sobrecarregada; útil para execuções "standalone" do Campo Médio
private:
    float infectados, removidos, suscetiveis;
    long unsigned int populacao;
    float virulencia, recuperacao;
    unsigned int geracao;
    bool inicializado;
    Arquivo arqGrafico;

    void gravarDadosGrafico(const std::string &dado);
};

#endif	/* CAMPOMEDIO_H */

