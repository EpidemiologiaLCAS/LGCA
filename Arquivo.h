/* 
 * File:   Arquivo.h
 * Author: Rodrigo Marianni
 *
 * Classe para gerenciamento de arquivos.
 *
 * Created on 3 de Setembro de 2010, 08:36
 */

#ifndef ARQUIVO_H
#define	ARQUIVO_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Arquivo {
public:
    Arquivo();
    Arquivo(const Arquivo& orig);
    virtual ~Arquivo();

    // grava uma linha no fim do arquivo; retorna falso em caso de erro
    bool gravarLinha(const std::string &caminho, const std::string &linha);
    // lê uma linha do arquivo
    bool obterLinha(const std::string &caminho, std::string &linha);
    void fechar(void);
private:
    // abre o arquivo para gravação (no final)
    bool abrirParaGravacao(const std::string &caminho);
    // abre o arquivo para leitura
    bool abrirParaLeitura(const std::string &caminho);
    // atributos
    std::fstream arquivo;
};

#endif	/* ARQUIVO_H */

