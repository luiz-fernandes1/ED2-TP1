//
// Created by joaozenobio on 31/10/2021.
//

#ifndef ED2_TP1_ACESSOSEQUENCIALINDEXADO_H
#define ED2_TP1_ACESSOSEQUENCIALINDEXADO_H


#define TAMPAGINA 4
#define MAXTABELA 100

#include "ManipulacaoDeArquivos.h"

int* criar_indices(FILE* arquivo, int tamMax, int &transferencias);
int pesquisa(FILE *arquivo, int *indices, int tam, int situacao, int chave, TipoRegistro *reg, int &transferenciaBusca, int &comparacoes);
int busca_binaria(TipoRegistro *paginas, int chave, int esq, int dir, int situacao, int &comparacoesBusca);
int find(TipoRegistro *paginas, int chave, int tamanho, int situacao,  int &comparacoesBusca);

#endif //ED2_TP1_ACESSOSEQUENCIALINDEXADO_H
