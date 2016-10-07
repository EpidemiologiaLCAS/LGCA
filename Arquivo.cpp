/**
 * @file Arquivo.cpp
 * @brief Classe para gerenciamento de arquivos.
 * 
 * @author Rodrigo Marianni
 * @date 3/09/2010
 */

#include "Arquivo.h"

// Protótipos
Arquivo::Arquivo() {
}

Arquivo::Arquivo(const Arquivo& orig) {
}

Arquivo::~Arquivo() {
    fechar();
}

/** 
 * @brief Abre um arquivo para gravação.
 * 
 * @param	caminho	String com o caminho do arquivo.
 * @return	bool	Representa o sucesso da operação.
 */
bool Arquivo::abrirParaGravacao(const std::string &caminho) {
    arquivo.open(caminho.c_str(), std::ios::out);

    return (arquivo.is_open());
}

/** 
 * @brief Abre um arquivo para leitura.
 * 
 * @param	caminho	String com o caminho do arquivo.
 * @return	bool	Representa o sucesso da operação.
 */
bool Arquivo::abrirParaLeitura(const std::string &caminho) {
    arquivo.open(caminho.c_str(), std::ios::in);

    return (arquivo.is_open());
}

/** 
 * @brief Grava uma linha em um arquivo.
 * 
 * @param	caminho	String com o caminho do arquivo (pois abre o arquivo, se necessário...).
 * @param	linha	String com a linha a gravar.
 * @return	bool	Representa o sucesso da operação.
 */
bool Arquivo::gravarLinha(const std::string &caminho, const std::string &linha) {
    if (! arquivo.is_open())
        if (!abrirParaGravacao(caminho)) {
            std::cerr << "Erro ao abrir o arquivo para gravação" << std::endl;
            return false;
        }

    arquivo << linha.c_str() << std::endl;

    return (arquivo.good());
}

/** 
 * @brief Recupera uma linha de um arquivo.
 * 
 * @param	caminho	String com o caminho do arquivo (pois abre o arquivo, se necessário...).
 * @param	linha	Ponteiro para a linha que será recuperada.
 * @return	bool	Representa o sucesso da operação.
 */
bool Arquivo::obterLinha(const std::string &caminho, std::string &linha) {
    
    if (! arquivo.is_open())
        if (!abrirParaLeitura(caminho)) {
            std::cerr << "Erro ao abrir o arquivo para leitura." << std::endl;
            return false;
        }

    getline(arquivo, linha);

    return (arquivo.good());
}

/** 
 * @brief Fecha um arquivo.
 */
void Arquivo::fechar() {
    if (arquivo.is_open())
        arquivo.close();
}
