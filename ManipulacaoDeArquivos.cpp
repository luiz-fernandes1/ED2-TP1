//
// Created by joaozenobio on 30/10/2021.
//

#include "ManipulacaoDeArquivos.h"

FILE *criar_arquivo() {
    FILE *arquivo;
    if ((arquivo = fopen("registro.bin", "wb")) == NULL) {
        std::cout << "Erro na criação do arquivo." << "\n";
    }
    return arquivo;
}

void escrever_registro(TipoRegistro registro, FILE *arquivo){
    fwrite(&registro, sizeof(TipoRegistro), 1, arquivo);
}

void imprimir_arquivo(FILE *arquivo) {
    rewind(arquivo);
    TipoRegistro reg;
    while(fread(&reg, sizeof(TipoRegistro), 1, arquivo)) {
        std::cout << "chave = " << reg.chave << "\n";
        //std::cout << "dado1 = " << reg.dado1 << "\n";
        //std::cout << "dado2 = " << reg.dado2 << "\n";
        //std::cout << "dado3 = " << reg.dado3 << "\n";
    }
    rewind(arquivo);
}

TipoRegistro* ler_arquivo(FILE* arquivo, int n) {
    TipoRegistro* registros = (TipoRegistro*) malloc(n * sizeof(TipoRegistro)); // Trocar pelo tamanho da pagina
    fread(registros, sizeof(TipoRegistro), n, arquivo);
    return registros;
}

void salvar_arquivo(FILE *arquivo) {
    fclose(arquivo);
}