//
// Created by joaozenobio on 01/11/2021.
//

#ifndef ED2_TP1_ARVOREBINARIA_H
#define ED2_TP1_ARVOREBINARIA_H


#define TAMPAG 3

#include "ManipulacaoDeArquivos.h"

typedef struct {
    long direita;
    long esquerda;
    TipoRegistro reg;
}ArvBin;

FILE* insere_temp(FILE* arv_simulada, TipoRegistro reg, int* transferencias);           // Função auxiliar da criação do arquivo
FILE* cria_arvore_simulada(FILE* arquivo, int* transferencias);                         // Função de cria o arquivo da árvore binária simulada
ArvBin* ler_ArvBin(FILE* arquivo, int n);                                               // Leitura de um número n de estruturas ArvBin no arquivo de simulação
int pesquisa_arvore_simulada(FILE* arv_simulada, int chave, TipoRegistro* registro);    // Função de pesquisa na árvore simulada
void imprimir_arvore_simulada(FILE *arv_simulada);                                      // Função teste para imprimir o arquivo da árvore simulada


#endif //ED2_TP1_ARVOREBINARIA_H
