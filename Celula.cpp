/* 
 * File:   Celula.cpp
 * Author: Rodrigo Marianni
 * 
 * Created on 3 de Agosto de 2010, 11:40
 */

#include "Celula.h"

// Construtor
Celula::Celula() {
    // inicializa a célula
    int k;

    sTmp = iTmp = rTmp = 0.0;
    for (k=0; k<TOT_VIZINHOS;k++) {
        probViagem[k] = 0.0;
    }
}

Celula::Celula(const Celula& orig) {
    int k;

    this->populacao = orig.populacao;
    this->suscetiveis = orig.suscetiveis;
    this->infectados = orig.infectados;
    this->removidos = orig.removidos;
    this->sTmp = orig.sTmp;
    this->iTmp = orig.iTmp;
    this->rTmp = orig.rTmp;

    for (k=0; k<TOT_VIZINHOS;k++) {
        this->probViagem[k] = orig.probViagem[k];
    }
}

Celula::~Celula() {
}

Celula& Celula::operator =(const Celula& orig) {
    int k;

    this->populacao = orig.populacao;
    this->suscetiveis = orig.suscetiveis;
    this->infectados = orig.infectados;
    this->removidos = orig.removidos;
    this->sTmp = orig.sTmp;
    this->iTmp = orig.iTmp;
    this->rTmp = orig.rTmp;

    for (k=0; k<TOT_VIZINHOS;k++) {
        this->probViagem[k] = orig.probViagem[k];
    }
    return *this;
}