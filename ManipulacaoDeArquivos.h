//
// Created by joaozenobio on 30/10/2021.
//

#ifndef TP_ARVOREB_TESTE_MANIPULACAODEARQUIVOS_H
#define TP_ARVOREB_TESTE_MANIPULACAODEARQUIVOS_H


#include "ArvoreB.hpp"

#include <iostream>
#include <cstdio>

FILE *criar_arquivo();
void escrever_registro(TipoRegistro registro, FILE *arquivo);
void imprimir_arquivo(FILE *arquivo);
TipoRegistro* ler_arquivo(FILE* arquivo, int n);
void salvar_arquivo(FILE *arquivo);


#endif //TP_ARVOREB_TESTE_MANIPULACAODEARQUIVOS_H
