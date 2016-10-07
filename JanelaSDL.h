/* 
 * File:   JanelaSDL.h
 * Author: Rodrigo Marianni
 *
 * Created on 4 de Agosto de 2010, 09:38
 */

#ifndef JANELASDL_H
#define	JANELASDL_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include "SDL/SDL.h"

class JanelaSDL {
public:
    JanelaSDL();
    JanelaSDL(const JanelaSDL& orig);
    virtual ~JanelaSDL();

    void definirPixel(const unsigned int &i, const unsigned int &j, const double &valor);
    void definirPixel(const unsigned int &i, const unsigned int &j,
                      const double &infect, const double &remov, const double &suscet);
    void definirDimensoes(const unsigned int &largura, const unsigned int &altura);
    void definirTamanhoCelula(const unsigned int &tamanho);
    void definirDelay(const unsigned int &atraso);
    void obterParametros(unsigned int &atraso, unsigned int &pixels);
    void atualizar(void);
    void inicializar(void);
    void gravarScreenshot(int geracao);
private:
    SDL_Surface *janelaSDL;
    unsigned int largura, altura;
    unsigned int tamanhoCelula, bpp, delay;
    std::string tituloJanela;

    bool janelaEstaPronta(void);
    void desenharCelula(const unsigned int &i, const unsigned int &j, const Uint32 &pixel);
};

#endif	/* JANELASDL_H */

