#ifndef ARVOREB_HPP
#define ARVOREB_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define M 2

using namespace std;

typedef int TipoChave;

typedef struct TipoRegistro{
    TipoChave chave;
    long int dado1;
    char dado2[1001];
    char dado3[5001];
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina{
    short n;
    TipoRegistro r[2*M];
    TipoApontador p[2*M + 1];
} TipoPagina;

void Inicializa(TipoApontador Arvore);
int Pesquisa(int chave, TipoRegistro *reg, TipoApontador ap, int &comparacoes);
void Imprime(TipoApontador arvore); 
void InsereNaPagina(TipoApontador ap, TipoRegistro reg, TipoApontador apDir, int &comparacoesInsercao);
void Ins(TipoRegistro reg, TipoApontador ap, short *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno, int &comparacoesInsercao);
void Insere(TipoRegistro reg, TipoApontador *ap, int &comparacoesInsercao);

#endif