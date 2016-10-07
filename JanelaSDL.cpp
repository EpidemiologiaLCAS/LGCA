/* 
 * File:   JanelaSDL.cpp
 * Author: Rodrigo Marianni
 * 
 * Created on 4 de Agosto de 2010, 09:38
 */

#include <sstream>

#include "JanelaSDL.h"

JanelaSDL::JanelaSDL() {
    janelaSDL = NULL;
    largura = altura = 0;
    bpp = 16; // cores de 16 bits são o padrão
    tituloJanela = "AC para simulação de epidemias - UNIOESTE";
    // inicializa a SDL e verifica erros
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro durante a inicialização da SDL.\n%s\nAbortando.\n." << SDL_GetError() << std::endl;
        exit(1);
    }
}

JanelaSDL::JanelaSDL(const JanelaSDL& orig) {
}

JanelaSDL::~JanelaSDL() {
    SDL_Quit();
}

void JanelaSDL::definirDimensoes(const unsigned int& largura, const unsigned int& altura) {
    this->largura = largura;
    this->altura = altura;
}

void JanelaSDL::definirTamanhoCelula(const unsigned int& tamanho) {
    this->tamanhoCelula = tamanho;
}

void JanelaSDL::definirDelay(const unsigned int& atraso) {
    this->delay = atraso;
}

void JanelaSDL::inicializar() {
    if ((largura != 0) && (altura != 0))
        janelaSDL = SDL_SetVideoMode(largura*tamanhoCelula, altura*tamanhoCelula, bpp, SDL_SWSURFACE);
    if (janelaEstaPronta())
        SDL_WM_SetCaption(tituloJanela.c_str(), NULL);
}

bool JanelaSDL::janelaEstaPronta() {
    return (janelaSDL != NULL);
}

// ATENÇÃO: O VALOR RECEBIDO DEVE SER UMA PROPORÇÃO (entre 0 e 1)
void JanelaSDL::definirPixel(const unsigned int& i, const unsigned int& j, const double& valor) {
    Uint32 cor;
    Uint8  estado;

    if (janelaEstaPronta()) {
        // Mapeia o estado da célula para a tela atual (RGB em hexa)
        estado = (Uint8) (255 - (valor * 255));
        // define os pixels RGB em relação ao estado da célula
        cor = SDL_MapRGB(janelaSDL->format, estado, estado, estado); // tons de cinza...
        desenharCelula(i, j, cor);
    }
}

// Método sobrecarregado; imprime suscetíveis, infectados e removidos com cores diferentes.
// Os valores recebidos ainda devem ser proporções (entre zero e um).
void JanelaSDL::definirPixel(const unsigned int& i, const unsigned int& j,
                             const double& infect, const double& remov, const double& suscet) {
    Uint32 cor;
    Uint8  r, g, b; // Suscetíveis: Azul; infectados: Vermelho; Removidos: Verde
    const Uint8 TOM = (Uint8) 230;
    const Uint8 RESTO = (Uint8) 100;

    if (janelaEstaPronta()) {
        // Mapeia o estado da célula para a tela atual (RGB em hexa)
        // Espaços vazios em branco:
        if (infect > 0) {
            // A célula tem infectados, que têm prioridade de visualização;
            // exibe-a em vermelho
            r = TOM;
            g = RESTO;
            b = RESTO;
        } else if (suscet >= remov) {
            // A célula tem mais suscetíveis do que removidos, infectados ou vazios;
            // exibe-a em azul
            r = RESTO;
            g = RESTO;
            b = TOM;
        } else if ((remov > suscet) || (remov > 0)) {
            // A célula tem removidos;
            // exibe-a em verde
            r = RESTO;
            g = TOM;
            b = RESTO;
        } else
            r = g = b = (Uint8) 255; // fundo branco (célula vazia)
        
        // define os pixels RGB em relação ao estado da célula
        cor = SDL_MapRGB(janelaSDL->format, r, g, b);
        desenharCelula(i, j, cor);
    }
}

void JanelaSDL::desenharCelula(const unsigned int& i, const unsigned int& j, const Uint32& pixel) {
    // Local da documentação das superfícies SDL (downloadable):
    // file:////usr/share/doc/libsdl1.2-dev/docs/html/sdlsurface.html
    if (janelaEstaPronta()) {
        // se necessário, trava a superfície
        if (SDL_MUSTLOCK(janelaSDL))
            if (SDL_LockSurface(janelaSDL) < 0) {
                std::cerr << "Erro ao bloquear a tela: " <<  SDL_GetError() << std::endl;
                exit(1);
            }

        // estabelece as dimensões e a posição do retângulo a definir
        SDL_Rect r;
        r.x = j*tamanhoCelula;
        r.y = i*tamanhoCelula;
        r.w = tamanhoCelula;
        r.h = tamanhoCelula;

        // define o retângulo (a tela ainda não é atualizada)
        SDL_FillRect(janelaSDL, &r, pixel);

        // se foi necessário bloquear a superfície, efetua o desbloqueio
        if (SDL_MUSTLOCK(janelaSDL))
            SDL_UnlockSurface(janelaSDL);
    }
}

void JanelaSDL::atualizar() {
    // para atualizar a tela, usa SDL_Flip
    if (janelaEstaPronta()) {
        SDL_Flip(janelaSDL);
        SDL_Delay(delay);
    }
}

void JanelaSDL::obterParametros(unsigned int& atraso, unsigned int& pixels) {
    atraso = delay;
    pixels = tamanhoCelula;
}

void JanelaSDL::gravarScreenshot(int geracao) {
    std::stringstream ss;

    // inicializa o nome do arquivo, com base no número da geração
    ss << std::setfill('0');
    ss << std::setw(4) << geracao << ".bmp";

    // grava o snapshot do AC no diretório atual, com o nome acima definido
    SDL_SaveBMP(janelaSDL, ss.str().c_str());
}