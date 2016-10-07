/* 
 * File:   main.cpp
 * Author: Rodrigo Marianni
 *
 * Created on 3 de Agosto de 2010, 11:40
 */

#include <iostream>
#include <cstring>

#include "AutomatoCelular.h"
#include "LGCA.h"

// Função que executa o CA de Höhle
void ca_hohle(char** argv) {

    unsigned int totI;

    std::string nomeArq = argv[2];
    std::string temp;
    AutomatoCelular a;

    if (a.carregarConfig(nomeArq)) {
        std::cout << "Pressione uma tecla para iniciar o AC.";
        std::cin.get();
        do {
            totI = a.aplicarTransicao();
        } while (totI > 0);

        std::cout << "Pressione uma tecla para continuar.";
        std::cin.get();
    } else {
        std::cerr << "Erro ao carregar o arquivo de configuração " << nomeArq;
        std::cerr << " (AC de Höhle)." << std::endl;
    }

}

// Função que executa o LGCA (Fukś e Lawniczak) com novos parâmetros
void lgca_novo(const int &l, const int &c, const int &tamCel, const int &delay,
               const int &geracoes, const float &v, const float &r, const int &sus, const int &inf) {
    
    unsigned int totI;
    /* LGCA (Fukś e Lawniczak) */

    // Cria um novo
    LGCA lgca(l, c, tamCel, delay, geracoes, v, r);
    lgca.inicializar(inf, sus);

    std::cout << "Pressione uma tecla para iniciar o AC.";
    std::cin.get();

    do {
        totI = lgca.aplicarTransicao();
    } while (totI != 0);

    std::cout << "Pressione uma tecla para continuar.";
    std::cin.get();

}

// Função que executa o LGCA (Fukś e Lawniczak) com parâmetros salvos
void lgca_salvo(void) {

    unsigned int totI;
    /* LGCA (Fukś e Lawniczak) */

    // Carrega um LGCA salvo
    LGCA lgca;
    lgca.carregarEstado();

    std::cout << "Pressione uma tecla para iniciar o AC.";
    std::cin.get();

    do {
        totI = lgca.aplicarTransicao();
    } while (totI != 0);

    std::cout << "Pressione uma tecla para continuar.";
    std::cin.get();
}

// Função que calcula os valores de I, R e S no tempo com o "Campo Médio" (Fukś e Lawniczak)
void mean_field(const int &geracoes, const float &v, const float &r,
                const int &sus, const int &inf) {
    CampoMedio mf(inf, sus, 0, v, r);
    int i;
    unsigned int total;

    for (i=0; i<geracoes; i++) {
        total = mf.computarGeracao();
        if (total == 0)
            break;
    }
}

// Processa os parâmetros da linha de comando
bool processar_linha_comando(const int &argc, char** argv) {

    bool resp = true;

    // Höhle
    if (strcmp(argv[1], "-o") == 0) {
        ca_hohle(argv);
    } else if (strcmp(argv[1], "-ls") == 0) {
        lgca_salvo();
    } else if (strcmp(argv[1], "-ln") == 0) {
        if (argc != 11) {
            resp = false;
        } else {
            lgca_novo(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]),
                      atoi(argv[6]), atof(argv[7]), atof(argv[8]), atoi(argv[9]), atoi(argv[10]));
        }
    } else if (strcmp(argv[1], "-mf") == 0) {
        if (argc != 7) {
            resp = false;
        } else {
            mean_field(atoi(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]),
                       atoi(argv[6]));
        }
    } else if (strcmp(argv[1], "--help") == 0) {
        std::cout << "Autômatos celulares" << std::endl;
        std::cout << "===================" << std::endl << std::endl;
        std::cout << "Modo de uso: "<< argv[0] << " TIPO_AUTOMATO [OPCOES]" << std::endl << std::endl;
        std::cout << "TIPO_AUTOMATO:" << std::endl;
        std::cout << "\t-o\tAC de Michael Höhle (salvo em disco)" << std::endl;
        std::cout << "\t-ls\tLGCA de Fukś e Lawniczak (salvo em disco)" << std::endl;
        std::cout << "\t-ln [OPCOES]\tLGCA de Fukś e Lawniczak (com novos parâmetros)" << std::endl;
        std::cout << "\t-mf [OPCOES]\t\"Campo Médio\" de Fukś e Lawniczak (com novos parâmetros)" << std::endl;
        std::cout << "\t--help\tEsta tela de ajuda..." << std::endl << std::endl;
        std::cout << "TIPO_AUTOMATO = -o; OPCOES: nome_arquivo" << std::endl;
        std::cout << "\tnome_arquivo\tNome do arquivo de texto que contém as configurações do AC." << std::endl << std::endl;
        std::cout << "TIPO_AUTOMATO = -ln; OPCOES:" << "\tL, C, TC, A, G, V, R, S, I" << std::endl;
        std::cout << "\tL\tNúmero de linhas" << std::endl;
        std::cout << "\tC\tNúmero de colunas" << std::endl;
        std::cout << "\tTC\tTamanho da célula, em pixels" << std::endl;
        std::cout << "\tA\tAtraso, em ms, entre dois passos de tempo" << std::endl;
        std::cout << "\tG\tNúmero máximo de gerações a executar" << std::endl;
        std::cout << "\tV\tTaxa de virulência (0 <= v <= 1)" << std::endl;
        std::cout << "\tR\tTaxa de recuperação (0 <= v <= 1)" << std::endl;
        std::cout << "\tS\tQuantidade de indivíduos suscetíveis" << std::endl;
        std::cout << "\tI\tQuantidade de indivíduos infectados" << std::endl << std::endl;
        std::cout << "TIPO_AUTOMATO = -mf; OPCOES:" << "\tG, V, R, S, I" << std::endl;
        std::cout << "\tG\tNúmero máximo de gerações a executar" << std::endl;
        std::cout << "\tV\tTaxa de virulência (0 <= v <= 1)" << std::endl;
        std::cout << "\tR\tTaxa de recuperação (0 <= v <= 1)" << std::endl;
        std::cout << "\tS\tQuantidade de indivíduos suscetíveis" << std::endl;
        std::cout << "\tI\tQuantidade de indivíduos infectados" << std::endl;
    } else
        resp = false;

    return resp;
}

/*
 * Aplicação principal
 */
int main(int argc, char** argv) {


    if ((argc == 1) || (! processar_linha_comando(argc, argv))){
        std::cout << "Consulte o modo de utilização com: " << argv[0] << " --help" << std::endl;
    }

    return 0;
}

