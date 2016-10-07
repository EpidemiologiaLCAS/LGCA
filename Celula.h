/* 
 * File:   Celula.h
 * Author: Rodrigo Marianni
 *
 * Created on 3 de Agosto de 2010, 11:40
 */

#ifndef CELULA_H
#define	CELULA_H

#define TOT_VIZINHOS 8 ///< total de vizinhos (8 = vizinhança de Moore)

#define NW	99 ///< Coordenada noroeste
#define N	1 ///< Coordenada norte
#define NE	2 ///< Coordenada nordeste
#define W	3 ///< Coordenada oeste
#define E	4 ///< Coordenada leste
#define SW	5 ///< Coordenada sudoeste
// #define S	6 ///< Coordenada sul
#define SE	7 ///< Coordenada sudeste

// protótipo
class AutomatoCelular;

class Celula {
public:
    Celula();
    Celula(const Celula& orig);
    virtual ~Celula();
    // necessário para poder armazenar objetos em vetores (ou em classes STL)
    Celula& operator =(const Celula& c);

    friend class AutomatoCelular;
private:
    // atributos
    double suscetiveis, infectados, removidos;
    double sTmp, iTmp, rTmp;
    unsigned int populacao;
    float probViagem[TOT_VIZINHOS];
};

#endif	/* CELULA_H */

