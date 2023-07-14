//
// Created by joaozenobio on 31/10/2021.
//

#include "AcessoSequencialIndexado.h"

int* criar_indices(FILE* arquivo, int tamMax, int &transferencias) {    
    rewind(arquivo);
    int* indices = (int*) malloc(sizeof(int) * tamMax);
    TipoRegistro* registros;
    for(int i = 0; i < tamMax; i++) {
        transferencias++;
        registros = ler_arquivo(arquivo, TAMPAGINA);
        indices[i] = registros[0].chave;
    }
    return indices;
}

int pesquisa(FILE *arquivo, int *indices, int tam, int situacao, int chave, TipoRegistro *reg, int &transferencias, int &comparacoesBusca){
    TipoRegistro pagina[TAMPAGINA];
    int posicao, quantidades;
    long desloc;

    posicao = 0;
    int cont = tam / TAMPAGINA;

    if(situacao == 1)
        while(posicao < cont && indices[posicao] <= chave){
            posicao++;
            comparacoesBusca++;
        }
    else 
        while(posicao < cont && indices[posicao] >= chave){
            posicao++;
            comparacoesBusca++;
        }
    

    if(posicao == 0){
        return 0;
    } else {
        if(posicao < tam) 
            quantidades = TAMPAGINA;
        else {
            fseek(arquivo, 0, SEEK_END);
            quantidades = (ftell(arquivo)/sizeof(TipoRegistro)%TAMPAGINA);
            if(quantidades == 0)
                quantidades = TAMPAGINA;
        }
    }

    posicao--;

    rewind(arquivo);
    desloc = posicao * TAMPAGINA * sizeof(TipoRegistro);
    fseek(arquivo, desloc, SEEK_SET);
    fread(&pagina, sizeof(TipoRegistro), quantidades, arquivo);

    transferencias++;
    
    int indice = find(pagina, chave, quantidades, situacao, comparacoesBusca);
    if(indice != -1){
        *reg = pagina[indice];
        return 1;
    }

    return 0;
}

int busca_binaria(TipoRegistro *paginas, int chave, int esq, int dir, int situacao, int &comparacoesBusca){
    int meio = (esq+dir) / 2;

    if(situacao == 1){
        if(paginas[meio].chave != chave && esq == dir){
            comparacoesBusca++;
            return -1;
        } else if(chave > paginas[meio].chave){
            comparacoesBusca++;
            return busca_binaria(paginas, chave, meio+1, dir, situacao, comparacoesBusca);
        } else if(chave < paginas[meio].chave){
            comparacoesBusca++;
            return busca_binaria(paginas, chave, esq, meio-1, situacao, comparacoesBusca);
        } else {
            comparacoesBusca++;
            return meio;
        }
    } else {
        if(paginas[meio].chave != chave && esq == dir){
            comparacoesBusca++;
            return -1;
        } else if(chave < paginas[meio].chave){
            comparacoesBusca++;
            return busca_binaria(paginas, chave, meio+1, dir, situacao, comparacoesBusca);
        } else if(chave > paginas[meio].chave){
            comparacoesBusca++;
            return busca_binaria(paginas, chave, esq, meio-1, situacao, comparacoesBusca);
        } else {
            comparacoesBusca++;
            return meio;
        }
    }
}

int find(TipoRegistro *paginas, int chave, int tamanho, int situacao, int &comparacaoBusca){
    return busca_binaria(paginas, chave, 0, tamanho, situacao, comparacaoBusca);
}
