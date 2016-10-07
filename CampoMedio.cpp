/**
 * @file CampoMedio.cpp
 * @brief Classe para cálculo do campo médio.
 * 
 * @author Rodrigo Marianni
 * @date 15/09/2010
 */

#include <iostream>
#include "CampoMedio.h"

/** 
 * @brief Construtor sem parâmetros; inicializa as variáveis.
 */
CampoMedio::CampoMedio() {
    inicializado = false;
    infectados = removidos = suscetiveis = 0;
    populacao = 0;
    virulencia = recuperacao = 0.0;
    geracao = 0;
}

/** 
 * @brief Construtor que define os parâmetros a partir dos valores especificados.
 * 
 * @param	int	Número de indivíduos infectados.
 * @param	int	Número de indivíduos suscetíveis.
 * @param	int	Número de indivíduos removidos.
 * @param	float	Taxa de virulência.
 * @param	float	Taxa de recuperação.
 */
CampoMedio::CampoMedio(const unsigned int& inf, const unsigned int& sus,
                       const unsigned int& rem, const float& v, const float& r):
                       virulencia(v), recuperacao(r) {
    inicializado = true;
    populacao = inf + sus + rem;
    infectados = ((float) inf)/populacao;
    suscetiveis = ((float) sus)/populacao;
    removidos = ((float) rem)/populacao;
    geracao = 0;
}

CampoMedio::CampoMedio(const CampoMedio& orig) {
}

CampoMedio::~CampoMedio() {
}

/** 
 * @brief Método que permite a definição dos parâmetros a partir dos valores especificados.
 *
 * Útil quando o construtor usado for o vazio.
 * 
 * @param	int	Número de indivíduos infectados.
 * @param	int	Número de indivíduos suscetíveis.
 * @param	int	Número de indivíduos removidos.
 * @param	float	Taxa de virulência.
 * @param	float	Taxa de recuperação.
 */
void CampoMedio::definirParametros(const unsigned int &inf, const unsigned int &sus, const unsigned int &rem,
                                   const float &v, const float &r) {
    populacao = inf + sus + rem;
    infectados = ((float) inf)/populacao;
    suscetiveis = ((float) sus)/populacao;
    removidos = ((float) rem)/populacao;
    virulencia = v;
    recuperacao = r;
    geracao = 0;
    inicializado = true;
}

/** 
 * @brief Computa a geração e retorna os totais de suscetiveis, infectados e removidos por referência.
 *
 * Utiliza as equações (8) da p. 6 do artigo de Fuks e Lawniczak.
 * 
 * @param	int	Número de indivíduos infectados.
 * @param	int	Número de indivíduos suscetíveis.
 * @param	int	Número de indivíduos removidos.
 * @return	int	Inteiro que representa a geração atual do AC.
 */
unsigned int CampoMedio::computarGeracao(unsigned int& inf, unsigned int& rem, unsigned int& sus) {

    float novoS, novoI, novoR;

    if (inicializado == false)
        return 0;

    novoS = suscetiveis - virulencia * suscetiveis * infectados;
    novoI = infectados + virulencia * suscetiveis * infectados - recuperacao * infectados;
    novoR = removidos + recuperacao * infectados;

    sus = novoS * populacao;
    inf = novoI * populacao;
    rem = novoR * populacao;

    suscetiveis = novoS;
    infectados = novoI;
    removidos = novoR;

    return (++geracao);

}

/** 
 * @brief Computa a geração e grava os dados para o gráfico.
 *
 * Função sobrecarregada; útil para execuções "standalone" do campo médio.
 * 
 * @return	int	Novo número de infectados.
 */
unsigned int CampoMedio::computarGeracao() {
    unsigned int novoS=0, novoI=0, novoR=0;
    std::stringstream ss;

    // computa a geração...
    computarGeracao(novoI, novoR, novoS);

    // ... e gera os dados para o gráfico
    ss << geracao << "\t" << novoI << "\t" << novoS << "\t" << novoR;
    gravarDadosGrafico(ss.str());

    return novoI;
}

/** 
 * @brief Grava dados para a geração do gráfico.
 *
 * Útil quando o "campo médio" é executado diretamente
 * (i.e. de modo independente do LGCA)
 * 
 * @param	String	Linha a gravar no arquivo de gráfico.
 */
// grava dados para um gráfico; 
void CampoMedio::gravarDadosGrafico(const std::string &dado) {

    if (! arqGrafico.gravarLinha(FILE_DADOS_GRAFICO, dado))
        std::cerr << "Erro ao gravar dados para o gráfico do campo médio." << std::endl;
}
