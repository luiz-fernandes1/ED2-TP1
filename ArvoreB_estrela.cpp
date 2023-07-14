#include "ArvoreB_estrela.hpp"

int pesquisaBestrela(TipoRegistro *x, TipoAp Ap, int& comparacoes){
    int i;

    //Se a página for interna entra nesse if.
    if(Ap->internaOuExterna == Interna){
        i = 1;
        
        //Faz uma pesquisa sequencial na página interna.
        while(i < Ap->UU.U0PagInterna.numeroItensInterno && x->chave > Ap-> UU.U0PagInterna.indiceInterno[i - 1]){
            comparacoes++;
            i++;
        }

        //Faz a ativação recursiva em uma das sub-árvores, a pesquisa só vai parar quando encontrar uma página folha.
        if(x->chave < Ap-> UU.U0PagInterna.indiceInterno[i - 1]){
            comparacoes++;
            return pesquisaBestrela(x, Ap->UU.U0PagInterna.apontadorInterno[i - 1], comparacoes);
        }
        else {
            return pesquisaBestrela(x, Ap->UU.U0PagInterna.apontadorInterno[i], comparacoes);
        }
    }

    i = 1;

    //Faz uma pesquisa sequencial na página folha.
    while(i < Ap->UU.U1PagExterna.numeroItensExterno && x->chave > Ap->UU.U1PagExterna.registroExterno[i - 1].chave){
        comparacoes++;
        i++;
    }

    //Verifica se a chave desejada foi encontrada, se estiver presente ele retorna 1.
    if (x->chave == Ap->UU.U1PagExterna.registroExterno[i - 1].chave){
        comparacoes++;
        *x = Ap->UU.U1PagExterna.registroExterno[i - 1];
        return 1;
    }
    //Caso não seja encontrada ele retorna 0.
    else if (x->chave != Ap->UU.U1PagExterna.registroExterno[i - 1].chave) {
        return 0; 
    }

    return 0;
}

void insereNaPaginaBestrela(TipoAp apontador, int registro, TipoAp apontadorDireita){
    short NaoAchouPosicao;
    int k;
    k = apontador->UU.U0PagInterna.numeroItensInterno;

    //Variável quer mantém o controle da posição corrente.
    NaoAchouPosicao = (k > 0);

    while(NaoAchouPosicao) {
        if(registro >= apontador->UU.U0PagInterna.indiceInterno[k - 1]){
            break;
        }

        //Coloca o registro e o apontador uma posição à frente.
        apontador->UU.U0PagInterna.indiceInterno[k] = apontador->UU.U0PagInterna.indiceInterno[k - 1];
        apontador->UU.U0PagInterna.apontadorInterno[k + 1] = apontador->UU.U0PagInterna.apontadorInterno[k];
        k--;
        if (k < 1){
            NaoAchouPosicao = false;
        }
    }

    //Achou posição.
    apontador->UU.U0PagInterna.indiceInterno[k] = registro;
    apontador->UU.U0PagInterna.apontadorInterno[k + 1] = apontadorDireita;
    apontador->UU.U0PagInterna.numeroItensInterno++;
    return;
}

void ins(TipoRegistro* registro, TipoAp apontador, short *cresceu,TipoRegistro *regRetorno, TipoAp *apRetorno, short *Cresceu_No, int& comparacoes){
    long i = 1;
    long j;
    TipoAp novo = nullptr;

    TipoRegistro aux;

    if(apontador->internaOuExterna == Externa){
        //Entrar aqui significa que chegou ao último nível.
        //Verifica se tem espaço na página e tenta inserir na página externa.
        if(apontador->UU.U1PagExterna.numeroItensExterno < (3 * ORDEM)){
            insereNaFolha(registro, apontador, comparacoes);
            *cresceu = false;
            *Cresceu_No = false;
            //Não precisou crescer e nem aumentar nível.
        }
        else{
            //Separando um nó em dois.
            //Sobe a cópia da chave pro pai e divide.
            //regRetorno vai salver a chave que estiver com o pai.
            //ApRetorno vai salvar o novo nó a direita.
            novo = (TipoPag*)malloc(sizeof(TipoPag));
            novo->internaOuExterna = Externa;
            novo->UU.U1PagExterna.numeroItensExterno = 0;
            aux = apontador->UU.U1PagExterna.registroExterno[(3 * ORDEM) - 1];

            //Coloca o último elemento na página à direita.
            insereNaFolha(&aux, novo, comparacoes);

            //Decrementa o número de itens dessa página.
            apontador->UU.U1PagExterna.numeroItensExterno--;

            //Insere o item novo.
            bool inseriu = insereNaFolha(registro, apontador, comparacoes);

            if(!inseriu){
                apontador->UU.U1PagExterna.numeroItensExterno++;
                delete novo;
                *cresceu = false;
                *Cresceu_No = false;
                return;
            }

            //Passa metade para a nova folha.
            for(int i = (3*ORDEM)/2; i < (3*ORDEM); i++){
                insereNaFolha(&(apontador->UU.U1PagExterna.registroExterno[i]), novo, comparacoes);
                apontador->UU.U1PagExterna.numeroItensExterno--;
            }

            regRetorno->chave = novo->UU.U1PagExterna.registroExterno[0].chave;    
            *apRetorno = novo;
            apontador->UU.U1PagExterna.proxPagina = novo;
            novo->UU.U1PagExterna.proxPagina = nullptr;

            //Após inserir o novo registro na árvore, vamos inserir na árvore a chave que vai subir para o nó pai.
            registro->chave = novo->UU.U1PagExterna.registroExterno[0].chave;

            //Divide o nó folha.
            *Cresceu_No = true;
            return;
        }
    }
    //Se a página for interna.
    else{
        while(i < apontador->UU.U0PagInterna.numeroItensInterno && registro->chave > apontador->UU.U0PagInterna.indiceInterno[i-1]){
            i++;
        }

        //Verificar dentro do nó folha para ver se ainda existe.
        if(registro->chave == apontador->UU.U0PagInterna.indiceInterno[i - 1]){
            *cresceu = false;
            *Cresceu_No =false;
            return;
        }

        if(registro->chave < apontador->UU.U0PagInterna.indiceInterno[i - 1]){
            i--;
        }

        //Encontra o lugar em que a chave deve ser colocada.
        //Entra caso a próxima página seja interna.
        if(!*Cresceu_No){
            ins(registro, apontador->UU.U0PagInterna.apontadorInterno[i], cresceu, regRetorno, apRetorno,Cresceu_No, comparacoes);
            if(!*cresceu && !*Cresceu_No){
                return;
            }
            //Verifica se a página tem espaço.
            if(apontador->UU.U0PagInterna.numeroItensInterno < 2*ORDEM){
                insereNaPaginaBestrela(apontador, regRetorno->chave, *apRetorno);
                *cresceu = false;
                *Cresceu_No =false;
                return;
            }

            TipoAp Temporaria;

            //A página está cheia e deve ser dividida
            Temporaria = (TipoAp) malloc (sizeof(TipoPag));
            Temporaria->internaOuExterna = Interna;
            Temporaria->UU.U0PagInterna.numeroItensInterno = 0;
            Temporaria->UU.U0PagInterna.apontadorInterno[0] = nullptr;

            if(i < ((2*ORDEM)/2) + 1){
                insereNaPaginaBestrela(Temporaria, apontador->UU.U0PagInterna.indiceInterno[(2*ORDEM) - 1], apontador->UU.U0PagInterna.apontadorInterno[2*ORDEM]);
                apontador->UU.U0PagInterna.numeroItensInterno--;
                insereNaPaginaBestrela(apontador, regRetorno->chave, *apRetorno);
            }
            else{
                insereNaPaginaBestrela(Temporaria, regRetorno->chave, *apRetorno);
            }

            for(j = ((2*ORDEM)/2) + 2; j <= 2*ORDEM; j++){
                insereNaPaginaBestrela(Temporaria, apontador->UU.U0PagInterna.indiceInterno[j-1], apontador->UU.U0PagInterna.apontadorInterno[j]);
            }

            apontador->UU.U0PagInterna.numeroItensInterno = (2*ORDEM)/2;
            Temporaria->UU.U0PagInterna.apontadorInterno[0] = apontador->UU.U0PagInterna.apontadorInterno[((2*ORDEM)/2) + 1];
            regRetorno->chave= apontador->UU.U0PagInterna.indiceInterno[(2*ORDEM)/2];

            *apRetorno = Temporaria;
            *cresceu = true;
            *Cresceu_No = false;
            return;
        }
    } 
}

void insereArvoreBStar(TipoRegistro* registro, TipoAp* apontador, int& comparacoes){
    short cresceu = 0;
    short cresceu_No = 0;

    TipoRegistro regRetorno;
    TipoPag *apRetorno = nullptr;
    TipoPag *ApTemp = nullptr;
    bool nova = false;

    if(*apontador == NULL){
        //Aloca a primeira página.
        ApTemp = (TipoPag*)malloc(sizeof(TipoPag));
        ApTemp->internaOuExterna = Externa;
        ApTemp->UU.U1PagExterna.numeroItensExterno = 0;
        ApTemp->UU.U1PagExterna.proxPagina = nullptr;

        insereNaFolha(registro, ApTemp, comparacoes);
        *apontador = ApTemp;
        return;
    }
    else{
        if((*apontador)->internaOuExterna == Externa){
            nova = true;
        }
        ins(registro, *apontador, &cresceu, &regRetorno, &apRetorno, &cresceu_No, comparacoes);
    }

    //Verifica se é o primeiro nó a ser criado.
    if((cresceu_No && nova) || cresceu){
        ApTemp = (TipoPag*) malloc(sizeof(TipoPag));
        ApTemp->internaOuExterna = Interna;
        ApTemp->UU.U0PagInterna.numeroItensInterno = 1;
        ApTemp->UU.U0PagInterna.indiceInterno[0] = regRetorno.chave;
        ApTemp->UU.U0PagInterna.apontadorInterno[0] = *apontador;
        ApTemp->UU.U0PagInterna.apontadorInterno[1] = apRetorno;

        *apontador = ApTemp;

        return;
    }
}

bool insereNaFolha(TipoRegistro* registro, TipoAp NovaPagina, int& comparacoes){
    int quantItens = NovaPagina->UU.U1PagExterna.numeroItensExterno;
    int k = 0;
    int i = 0;

    while(registro->chave > NovaPagina->UU.U1PagExterna.registroExterno[i].chave && i < quantItens){
        comparacoes++;
        i++;
    }

    if(registro->chave == NovaPagina->UU.U1PagExterna.registroExterno[i].chave && quantItens != 0){
        comparacoes++;
        return false;
    }

    if(i < quantItens){
        k = quantItens;
        while(k >= 0 && k > i){
            NovaPagina->UU.U1PagExterna.registroExterno[k] = NovaPagina->UU.U1PagExterna.registroExterno[k - 1];
            k--;
        }

        //Insere na primeira posição.
        NovaPagina->UU.U1PagExterna.registroExterno[i] = *registro;
    }
    else{
        //Insere no final.
        NovaPagina->UU.U1PagExterna.registroExterno[i] = *registro;
    }

    NovaPagina->UU.U1PagExterna.numeroItensExterno++;

    return true;
}

void imprimirDados(TipoRegistro *Registros, bool p){
    if(p == true){
        std::cout << "Chave encontrada: " << Registros->chave << endl;
        std::cout << "Dado 1: " << Registros->dado1 << endl;
        std::cout << "Dado 2: " << Registros->dado2 << endl;
        std::cout << "Dado 3: " << Registros->dado3 << endl;
    }
    else{
        std::cout << "Chave encontrada: " << Registros->chave << endl;
    }
    
}