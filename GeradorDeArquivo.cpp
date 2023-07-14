//
// Created by joaozenobio on 30/10/2021.
//

#include "GeradorDeArquivo.h"

#include <ctime>

int gerar_arquivo_aleatorio(int quantidade, int situacao) {
    srand(time(NULL));
    FILE* arquivo = criar_arquivo();
    TipoRegistro reg{};
    if(situacao == 1) {
        for (int i = 0; i < quantidade; i++) {
            reg.chave = i + 1;
            reg.dado1 =(int) rand();
            for(int j = 0; j < 1001; j++) reg.dado2[j] = 97 + (char)(rand()%26);
            for(int j = 0; j < 5001; j++) reg.dado3[j] = 97 + (char)(rand()%26);
            escrever_registro(reg, arquivo);
        }
    }
    else if(situacao == 2) {
        for (int i = quantidade-1; i >= 0; i--) {
            reg.chave = i + 1;
            reg.dado1 =(int) rand();
            for(int j = 0; j < 1001; j++) reg.dado2[j] = 97 + (char)(rand()%26);
            for(int j = 0; j < 5001; j++) reg.dado3[j] = 97 + (char)(rand()%26);
            escrever_registro(reg, arquivo);
        }
    }
    else if(situacao == 3) {
        int chaves[quantidade];
        int chave;
        for(int i = 0; i < quantidade; i++) {
            chave =(int) 1 + rand() % quantidade;
            while(pesquisar(chaves, quantidade, chave)) {
                chave =(int) 1 + rand() % quantidade;
            }
            chaves[i] = chave;
        }
        for (int i = 0; i < quantidade; i++) {
            reg.chave = chaves[i];
            reg.dado1 =(int) rand();
            for(int j = 0; j < 1001; j++) reg.dado2[j] = 97 + (char)(rand()%26);
            for(int j = 0; j < 5001; j++) reg.dado3[j] = 97 + (char)(rand()%26);
            escrever_registro(reg, arquivo);
        }
    }
    salvar_arquivo(arquivo);
    return 1;
}

int pesquisar(int* chaves, int quantidade, int chave) {
    for(int i=0; i<quantidade; i++) {
        if(chaves[i] == chave) {
            return 1;
        }
    }
    return 0;
}