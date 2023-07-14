#include "ArvoreB.hpp"
#include "AcessoSequencialIndexado.h"
#include "GeradorDeArquivo.h"
#include "ArvoreB_estrela.hpp"
#include "ArvoreBinaria.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

int validaEntrada(int argc, char *argv[], int *metodo, int *quantidade, int* situacao, int *chave, bool *p){

    if(argc < 5 || argc > 6) {
        std::cout << "Quantidade de parâmetros incorreta." << endl;
        return 0;
    }

    *metodo = std::atoi(argv[1]), *quantidade = std::atoi(argv[2]), *situacao = std::atoi(argv[3]), *chave = std::atoi(argv[4]);

    if(*metodo < 1 || *metodo > 4){
        std::cout << "Método inválido!" << endl;
        return 0;
    }

    if(*quantidade <= 0){
        std::cout << "Quantidade de registro inválida!" << endl;
        return 0;
    }

    if(*situacao < 1 || *situacao > 3){
        std::cout << "Situação inválida!" << endl;
        return 0;
    }

    if(*chave <= 0){
        std::cout << "A chave de pesquisa precisa ser um valor positivo!"  << endl;
        return 0;
    }

    if(argc == 6) {
        if(strcmp("[-P]", argv[5])){
            std::cout << "Parâmetro opcional inválido!" << endl;
            return 0;
        }
        *p = true;
    }

    return 1;
}

void print_dados(bool p, TipoRegistro *reg){
    if(p){
        std::cout << "Chave encontrada: " << reg->chave << endl;
        std::cout << "Dado 1: " << reg->dado1 << endl;
        std::cout << "Dado 2: " << reg->dado2 << endl;
        std::cout << "Dado 3: " << reg->dado3 << endl;
    } else 
        std::cout << "Chave encontrada: " << reg->chave << endl;
}

int main(int argc, char *argv[]){
    int metodo, quantidade, situacao, chave;
    bool p;

    clock_t tIniBusca, tIniCriacao;
    double tempoTotalCriacao, tempoTotalBusca;

    int comparacoesBusca, transferencias = 0, comparacoesInsercao = 0, transferencias_insercao = 0;

    if(validaEntrada(argc, argv, &metodo, &quantidade, &situacao, &chave, &p)) {
        if(gerar_arquivo_aleatorio(quantidade, situacao)) {
            FILE *arquivo = fopen("registro.bin", "rb");
            if (metodo == 1) {
                if (situacao == 3) {
                    cout << "O arquivo deve estar ordenado" << "\n";
                    return 0;
                }
                tIniCriacao = clock();
                int tamMax = quantidade / TAMPAGINA;
                int *indices;
                indices = criar_indices(arquivo, tamMax, transferencias);
                tempoTotalCriacao = ((double) (clock() - tIniCriacao) / (double) CLOCKS_PER_SEC);

                tIniBusca = clock();
                int transferenciaBusca = 0;
                TipoRegistro reg{};
                if (pesquisa(arquivo, indices, quantidade, situacao, chave, &reg, transferenciaBusca, comparacoesBusca)) {
                    print_dados(p, &reg);
                } else
                    std::cout << "O registro de chave = " << chave << " não encontra-se nas páginas!" << endl;
                tempoTotalBusca = ((double) (clock() - tIniBusca) / (double) CLOCKS_PER_SEC);

                free(indices);

                std::cout << "Tempo total de criação: " << tempoTotalCriacao << endl;
                std::cout << "Tempo total de busca: " << tempoTotalBusca << endl;


                std::cout << "Comparações na busca: " << comparacoesBusca << endl;

                std::cout << "Transferências na criação: " << transferencias << endl;
                std::cout << "Transferências na busca: " << transferenciaBusca << endl;
            }
            else if (metodo == 2) {
                tIniCriacao = clock();
                FILE *arv_simulada = cria_arvore_simulada(arquivo, &transferencias_insercao);
                tempoTotalCriacao = ((double) (clock() - tIniCriacao) / (double) CLOCKS_PER_SEC) * 100000;
                if ((arv_simulada = fopen("arv_simulada.bin", "rb")) == NULL) {
                    std::cout << "Erro na criação do arquivo." << "\n";
                }
                tIniBusca = clock();
                TipoRegistro registro{};
                transferencias = pesquisa_arvore_simulada(arv_simulada, chave, &registro);
                print_dados(p, &registro);
                tempoTotalBusca = ((double) (clock() - tIniBusca) / (double) CLOCKS_PER_SEC) * 100000;
                std::cout << "Transferências na cricao: " << transferencias_insercao << "\n";
                std::cout << "Tempo de criacao: " << tempoTotalCriacao << "\n";
                std::cout << "Transferências na pesquisa: " << transferencias << "\n";
                std::cout << "Tempo de pesquisa: " << tempoTotalBusca << "\n";
                fclose(arv_simulada);
            }
            else if (metodo == 3) {
                tIniCriacao = clock();
                TipoApontador arv{};
                Inicializa(arv);

                TipoRegistro *reg;
                for (int i = 0; i < quantidade; i++) {
                    reg = ler_arquivo(arquivo, 1);
                    transferencias++;
                    Insere(reg[0], &arv, comparacoesInsercao);
                }
                tempoTotalCriacao = ((double) (clock() - tIniCriacao) / (double) CLOCKS_PER_SEC);

                tIniBusca = clock();
                if (Pesquisa(chave, reg, arv, comparacoesBusca)) {
                    print_dados(p, reg);
                } else
                    std::cout << "O registro de chave = " << chave << " não encontra-se na árvore B!" << endl;

                tempoTotalBusca = ((double) (clock() - tIniBusca) / (double) CLOCKS_PER_SEC);

                std::cout << "Tempo total de criação: " << tempoTotalCriacao << endl;
                std::cout << "Tempo total de busca: " << tempoTotalBusca << endl;

                std::cout << "Comparações na busca: " << comparacoesBusca << endl;
                std::cout << "Comparações na inserção: " << comparacoesInsercao << endl;

                std::cout << "Transferências: " << transferencias << endl;
            }
            else if (metodo == 4) {
                tIniCriacao = clock();
                TipoAp arv{NULL};
                TipoRegistro *reg;

                for (int i = 0; i < quantidade; i++) {
                    reg = ler_arquivo(arquivo, 1);
                    transferencias++;
                    insereArvoreBStar(&reg[0], &arv, comparacoesInsercao);
                }
                tempoTotalCriacao = ((double) (clock() - tIniCriacao) / (double) CLOCKS_PER_SEC);


                tIniBusca = clock();
                reg->chave = chave;
                if (pesquisaBestrela(reg, arv, comparacoesBusca) == 1) {
                    imprimirDados(reg, p);
                } else
                std::cout << "O registro de chave = " << chave << " não encontra-se na árvore B*!" << endl;
                tempoTotalBusca = ((double) (clock() - tIniBusca) / (double) CLOCKS_PER_SEC);

                std::cout << "Tempo total de criação: " << tempoTotalCriacao << endl;
                std::cout << "Tempo total de busca: " << tempoTotalBusca << endl;

                std::cout << "Comparações na busca: " << comparacoesBusca << endl;
                std::cout << "Comparações na inserção: " << comparacoesInsercao << endl;

                std::cout << "Transferências: " << transferencias << endl;
            }
            salvar_arquivo(arquivo);
        }
    }
    return 0;
}