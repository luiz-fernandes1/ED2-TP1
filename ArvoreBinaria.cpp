//
// Created by joaozenobio on 01/11/2021.
//

#include "ArvoreBinaria.h"
#include <cmath>

FILE* cria_arvore_simulada(FILE* arquivo, int* transferencias) {
    FILE *arv_simulada;
    if ((arv_simulada = fopen("arv_simulada.bin", "wb")) == NULL) {
        std::cout << "Erro na criação do arquivo." << "\n";
    }
    TipoRegistro* reg;
    rewind(arquivo);
    reg = ler_arquivo(arquivo, 1);
    (*transferencias)++;
    ArvBin arv_temp{-1, -1, reg[0]};
    fwrite(&arv_temp, sizeof(ArvBin), 1, arv_simulada);                 // Inserção do primeiro registro como raiz
    (*transferencias)++;
    fclose(arv_simulada);
    FILE* arv_simulada_temp;
    reg = ler_arquivo(arquivo, 1);
    (*transferencias)++;
    while(!feof(arquivo)) {
        if ((arv_simulada = fopen("arv_simulada.bin", "rb")) == NULL) {
            std::cout << "Erro na criação do arquivo." << "\n";
        }
        arv_simulada_temp = insere_temp(arv_simulada, reg[0], transferencias);                  // Inserção de um registro no arquivo da árvore binária
        fclose(arv_simulada_temp);
        fclose(arv_simulada);
        if ((arv_simulada = fopen("arv_simulada.bin", "wb")) == NULL) {
            std::cout << "Erro na criação do arquivo." << "\n";
        }
        if ((arv_simulada_temp = fopen("arv_simulada_temp.bin", "rb")) == NULL) {
            std::cout << "Erro na criação do arquivo." << "\n";
        }
        ArvBin* arv_temp2;
        arv_temp2 = ler_ArvBin(arv_simulada_temp, 1);
        while(!feof(arv_simulada_temp)) {                                                          // Reescrita do arquivo da árvore simulada a
            fwrite(&arv_temp2[0], sizeof(ArvBin), 1, arv_simulada);                         // a partir de outro arquivo temporário contendo
            (*transferencias)++;                                                                   // as modificações
            arv_temp2 = ler_ArvBin(arv_simulada_temp, 1);
            (*transferencias)++;
        }
        fclose(arv_simulada);
        fclose(arv_simulada_temp);
        reg = ler_arquivo(arquivo, 1);
        (*transferencias)++;
    }
    return arv_simulada;
}

FILE* insere_temp(FILE* arv_simulada, TipoRegistro reg, int* transferencias) {
    ArvBin* arv_temp = (ArvBin*) malloc(1 * sizeof(ArvBin));
    fread(arv_temp, sizeof(ArvBin), 1, arv_simulada);
    (*transferencias)++;
    if(reg.chave > arv_temp[0].reg.chave) {
        if(arv_temp[0].direita != -1) {
            fseek(arv_simulada, arv_temp[0].direita*sizeof(ArvBin), SEEK_SET);                     // Busca pela folha vazia onde será inserido o novo dado
            insere_temp(arv_simulada, reg, transferencias);
        }
        else {
            // Criação de um arquivo temporário onde se encontram as modificações que serão passadas ao arquivo original
            FILE *arv_simulada_temp;
            if ((arv_simulada_temp = fopen("arv_simulada_temp.bin", "wb")) == NULL) {
                std::cout << "Erro na criação do arquivo." << "\n";
            }
            long deslocamento = ftell(arv_simulada) - sizeof(ArvBin);
            long pos = 0;
            rewind(arv_simulada);
            ArvBin* arv_temp2;
            // Encontra o resgitro enquanto escreve a parte não modificada do arquivo
            while(pos != deslocamento) {
                arv_temp2 = ler_ArvBin(arv_simulada, 1);
                (*transferencias)++;
                fwrite(&arv_temp2[0], sizeof(ArvBin), 1, arv_simulada_temp);
                (*transferencias)++;
                pos = ftell(arv_simulada);
            }
            // Insere a posição do registro inferior na estrutura do registro superior
            fseek(arv_simulada, 0, SEEK_END);
            arv_temp[0].direita = ftell(arv_simulada)/sizeof(ArvBin);
            fwrite(&arv_temp[0], sizeof(ArvBin), 1, arv_simulada_temp);         // Modifica a chave da folha superior
            (*transferencias)++;
            fseek(arv_simulada, deslocamento + sizeof(ArvBin), SEEK_SET);
            arv_temp2 = ler_ArvBin(arv_simulada, 1);
            (*transferencias)++;
            // Escreve o restante do arquivo que permaneceu o  igual
            while(!feof(arv_simulada)) {
                fwrite(&arv_temp2[0], sizeof(ArvBin), 1, arv_simulada_temp);
                (*transferencias)++;
                arv_temp2 = ler_ArvBin(arv_simulada, 1);
                (*transferencias)++;
            }
            arv_temp[0] = {-1, -1, reg};
            fwrite(&arv_temp[0], sizeof(ArvBin), 1, arv_simulada_temp);         // Insere o novo dado
            (*transferencias)++;
            return arv_simulada_temp;
        }
    }
    else {
        if(arv_temp[0].esquerda != -1) {
            fseek(arv_simulada, arv_temp[0].esquerda*sizeof(ArvBin), SEEK_SET);                     // Busca pela folha vazia onde será inserido o novo dado
            insere_temp(arv_simulada, reg, transferencias);
        }
        else {
            // Criação de um arquivo temporário onde se encontram as modificações que serão passadas ao arquivo original
            FILE *arv_simulada_temp;
            if ((arv_simulada_temp = fopen("arv_simulada_temp.bin", "wb")) == NULL) {
                std::cout << "Erro na criação do arquivo." << "\n";
            }
            long deslocamento = ftell(arv_simulada) - sizeof(ArvBin);
            long pos = 0;
            rewind(arv_simulada);
            ArvBin* arv_temp2;
            while(pos != deslocamento) {
                arv_temp2 = ler_ArvBin(arv_simulada, 1);
                (*transferencias)++;
                fwrite(&arv_temp2[0], sizeof(ArvBin), 1, arv_simulada_temp);
                (*transferencias)++;
                pos = ftell(arv_simulada);
            }
            fseek(arv_simulada, 0, SEEK_END);
            arv_temp[0].esquerda = ftell(arv_simulada)/sizeof(ArvBin);
            fwrite(&arv_temp[0], sizeof(ArvBin), 1, arv_simulada_temp);        // Modifica a chave da folha superior
            (*transferencias)++;
            fseek(arv_simulada, deslocamento + sizeof(ArvBin), SEEK_SET);
            arv_temp2 = ler_ArvBin(arv_simulada, 1);
            (*transferencias)++;
            while(!feof(arv_simulada)) {
                fwrite(&arv_temp2[0], sizeof(ArvBin), 1, arv_simulada_temp);
                (*transferencias)++;
                arv_temp2 = ler_ArvBin(arv_simulada, 1);
                (*transferencias)++;
            }
            arv_temp[0] = {-1, -1, reg};
            fwrite(&arv_temp[0], sizeof(ArvBin), 1, arv_simulada_temp);       // Insere o novo dado
            (*transferencias)++;
            return arv_simulada_temp;
        }
    }
}

ArvBin* ler_ArvBin(FILE* arv_simulada, int n) {
    ArvBin* arv_bin = (ArvBin*) malloc(n * sizeof(ArvBin));
    fread(arv_bin, sizeof(ArvBin), n, arv_simulada);
    return arv_bin;
}

int pesquisa_arvore_simulada(FILE* arv_simulada, int chave, TipoRegistro* registro) {
    //Percorre sobre a árvore até encontrar o dado ou encontrar uma posição vazia, ou seja, não encontrá-lo
    ArvBin* arv_temp = (ArvBin*) malloc(1 * sizeof(ArvBin));
    fread(arv_temp, sizeof(ArvBin), 1, arv_simulada);
    if(chave > arv_temp[0].reg.chave) {
        if(arv_temp[0].direita != -1) {
            fseek(arv_simulada, arv_temp[0].direita*sizeof(ArvBin), SEEK_SET);
            return 1 + pesquisa_arvore_simulada(arv_simulada, chave, registro);
        }
        else {
            registro = NULL;
            return 1;
        }
    }
    else if(chave < arv_temp[0].reg.chave) {
        if(arv_temp[0].esquerda != -1) {
            fseek(arv_simulada, arv_temp[0].esquerda*sizeof(ArvBin), SEEK_SET);
            return 1 + pesquisa_arvore_simulada(arv_simulada, chave, registro);
        }
        else {
            registro = NULL;
            return 1;
        }
    }
    else {
        *registro = arv_temp[0].reg;
        return 1;
    }
}

void imprimir_arvore_simulada(FILE *arv_simulada) {
    long desloc = ftell(arv_simulada);
    rewind(arv_simulada);
    ArvBin arv_bin;
    int pos = 0;
    while(fread(&arv_bin, sizeof(ArvBin), 1, arv_simulada)) {
        std::cout << pos << " | ";
        std::cout << "chave = " << arv_bin.reg.chave << " / ";
        std::cout << "esquerda = " << arv_bin.esquerda << " / ";
        std::cout << "direita =  " << arv_bin.direita << "\n";
        pos++;
    }
    fseek(arv_simulada, desloc, SEEK_SET);
}
