/* 
 * File:   CelulaLGCA.cpp
 * Author: Rodrigo Marianni
 *
 * Created on 30 de Agosto de 2010, 21:44
 */

#include "CelulaLGCA.h"
#include "Celula.h"

CelulaLGCA::CelulaLGCA() {
    int i;
    for (i=0; i<L; i++) {
        // as células iniciam vazias
        estado[i] = V;
        canais[i] = V;
    }
}

CelulaLGCA::CelulaLGCA(const CelulaLGCA& orig) {
    int i;
    for (i=0; i<L; i++) {
        this->estado[i] = orig.estado[i];
        this->canais[i] = orig.canais[i];
    }
}

CelulaLGCA& CelulaLGCA::operator =(const CelulaLGCA& c) {
    int i;
    for (i=0; i<L; i++) {
        this->estado[i] = c.estado[i];
        this->canais[i] = c.canais[i];
    }
    return *this;
}

CelulaLGCA::~CelulaLGCA() {
}

// 'setter' para estado do indivíduo na célula
void CelulaLGCA::definirEstado(const int& canal, const int& estado) {
    this->estado[canal] = estado;
}

void CelulaLGCA::definirCanal(const int& canal, const int& estado) {
    canais[canal] = estado;
}

/* Para a etapa de randomização;
 * 
 */
void CelulaLGCA::embaralharIndividuos() {
    CelulaLGCA::embaralharVetorInteiro(estado, L);
}

/* Apenas embaralha (shuffle) um vetor por meio
 * do algoritmo de Fisher-Yates (variação de Durstenfeld).
 * */
void CelulaLGCA::embaralharVetorInteiro(int* vetor, const int &tamanho) {
    int i, aleatorio, tmp;

    for (i=(tamanho-1); i>=0; i--) {
        aleatorio = CelulaLGCA::gerarAleatorio(0, i);
        tmp = vetor[i];
        vetor[i] = vetor[aleatorio];
        vetor[aleatorio] = tmp;
    }
}

/* Gera um aleatório entre inf e sup, inclusive. */
int CelulaLGCA::gerarAleatorio(const int& inf, const int& sup) {
    return rand() % (sup-inf+1) + inf;
}

int CelulaLGCA::obterEstado(const int& canal) {
    return estado[canal];
}

int CelulaLGCA::obterCanal(const int& canal) {
    return canais[canal];
}

// Retorna o número de infectados atualmente na célula
int CelulaLGCA::obterNi() {
    int i, tot=0;
    for (i=0; i<L; i++) {
        if (estado[i] == I)
            tot++;
    }
    return tot;
}

/* Retorna o número de indivíduos suscetíveis, infectados e removidos na
   célula; útil para a visualização completa na janela SDL.
 */
void CelulaLGCA::obterQuantidadeIndividuos(int& i, int& r, int& s) {
    int k;
    i = r = s = 0;
    for (k=0; k<L; k++) {
        switch (estado[k]) {
            case I:
                i++;
                break;
            case S:
                s++;
                break;
            case R:
                r++;
        }
    }
}

/* Obtém o número do canal oposto ao canal dado.
 * O método foi generalizado APENAS para polígonos com L par...
 * e.g.: em um retângulo,   0 <=> 2 [2%4]
 *                          1 <=> 3 [3%4]
 *                          2 <=> 0 [4%4]
 *                          3 <=> 1 [5%4]
 *      em um hexágono,     0 <=> 3 [3%6]
 *                          1 <=> 4 [4%6]
 *                          etc.
 */
int CelulaLGCA::canalOposto(const int& canal) {
    return (canal + L/2) % L;
}

/* Obtém o indivíduo que está no canal oposto ao fornecido.
 * Método útil para que uma célula consulte o estado do canal correspondente
 * de sua vizinha.
 */
int CelulaLGCA::obterEstadoOposto(const int& canal) {
    return estado[canalOposto(canal)];
}

void CelulaLGCA::atualizarCelula() {
    int i;
    for (i=0; i<L; i++)
        estado[i] = canais[i];
}