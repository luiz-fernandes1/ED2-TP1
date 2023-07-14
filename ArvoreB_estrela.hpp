#ifndef ARVOREB_ESTRELA_HPP
#define ARVOREB_ESTRELA_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ArvoreB.hpp"

using namespace std;

#define ORDEM 4

typedef enum {Interna, Externa} TipoIntExt;
typedef struct TipoPag* TipoAp;
typedef struct TipoPag{
    TipoIntExt internaOuExterna;             //Variável que indica se a página é interna ou externa.
    union{
        struct{
            int numeroItensInterno;
            int indiceInterno[2 * ORDEM];
            TipoAp apontadorInterno[2 * ORDEM + 1];
        } U0PagInterna;
        struct{
            int numeroItensExterno;
            TipoRegistro registroExterno[3 * ORDEM];
            TipoAp proxPagina;
        }U1PagExterna;
    }UU;
}TipoPag;


int pesquisaBestrela (TipoRegistro *x, TipoAp Ap, int& comparacoes);
void insereNaPaginaBestrela (TipoAp apontador, int registro, TipoAp apontadorDireita);
void ins (TipoRegistro *registro, TipoAp apontador, short* cresceu, TipoRegistro *regRetorno, TipoAp *apRetorno, short* *Cresceu_No, int& comparacoes);
void insereArvoreBStar (TipoRegistro *registro, TipoAp* apontador, int& comparacoes);
bool insereNaFolha (TipoRegistro *registro, TipoAp NovaPagina, int& comparacoes);
void gerarArquivo (int ordem, int qntReg);
void imprimirDados (TipoRegistro *Registros, bool p);


#endif