/* 
 * File:   CelulaLGCA.h
 * Author: Rodrigo Marianni
 *
 * Células retangulares... (TODO: generalizar)
 * As coordenadas das vizinhanças (canais) são definidas do seguinte modo:
 *           0
 *    3 | Célula  | 1
 *           2
 * I.e. 0 = N; as coordenadas crescem em sentido horário.
 *
 * Os canais são SEMPRE acessados por seus índices; o atributo "canal" serve,
 * de fato, para que o LGCA 'envie' um indivíduo para um canal; trata-se, assim,
 * de um estado temporário.
 *
 * Created on 30 de Agosto de 2010, 21:44
 */

#ifndef CELULALGCA_H
#define	CELULALGCA_H

#include <cstdlib>
#include <ctime>

// solução 'à la' C, com defines
#define L   4
#define I   1
#define S   0
#define R   2
#define V   3 // V indica canal vazio

class CelulaLGCA {
public:
    CelulaLGCA();
    CelulaLGCA(const CelulaLGCA& orig);
    virtual ~CelulaLGCA();
    // necessário para poder armazenar objetos em vetores (ou em classes STL)
    CelulaLGCA& operator =(const CelulaLGCA& c);

    // método para alteração do estado da célula (basicamente para a inicialização do LGCA)
    void definirEstado(const int &canal, const int &estado);
    // método para a definição do estado temporário
    void definirCanal(const int &canal, const int &estado);
    // método para redefinição dos canais de saída (etapa de randomização); de fato, apenas
    // embaralha os indivíduos
    int obterEstado(const int &canal);
    int obterNi(void); // retorna o número de infectados na célula
    void obterQuantidadeIndividuos(int &i, int &r, int &s); // obtém o número de indivíduos I, R e S
    void embaralharIndividuos(void); // redistribui os individuos para diferentes canais de saída
    int obterEstadoOposto(const int &canal); // obtém o indivíduo, dado o canal oposto
    int obterCanal(const int &canal);
    void atualizarCelula(void);

    static int canalOposto(const int &canal); // retorna o número do canal oposto
    static int gerarAleatorio(const int& inf, const int& sup);
    static void embaralharVetorInteiro(int* vetor, const int &tamanho);
private:
    int estado[L];
    int canais[L];
};

#endif	/* CELULALGCA_H */

