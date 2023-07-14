#include "ArvoreB.hpp"

void Inicializa(TipoApontador arvore){
    arvore == NULL;
}

int Pesquisa(int chave, TipoRegistro *reg, TipoApontador ap, int &comparacoesBusca){
    int i = 1;
    //Se o apontador é igual a NULL, então árvore está vazia
    if(ap == NULL){
        return 0;
    }

    //Se o valor de i é menor que a quantidade de itens da página não há
    //necessidade de realizar a pesquisa dentro dessa página
    while(i < ap->n && chave > ap->r[i-1].chave){
        ++i;
        comparacoesBusca++;
    }

    //verifica se o item na posição i-1 é igual a chave
    if(chave == ap->r[i-1].chave){
        *reg = ap->r[i-1];
        comparacoesBusca++;
        return 1;
    }

    //caso o item não seja encontrado é necessário decidir qual caminho será percorrido
    if(chave < ap->r[i-1].chave){
        //pega o filho a esquerda
        comparacoesBusca++;
        return Pesquisa(chave, reg, ap->p[i-1], comparacoesBusca);
    }
    else{
        //pega o filho a direita
        comparacoesBusca++;
        return Pesquisa(chave, reg, ap->p[i], comparacoesBusca);
    }
}

void Imprime(TipoApontador arvore){
    int i = 0;

    if(arvore == NULL) 
        return;
    
    while(i <= arvore->n){
        Imprime(arvore->p[i]);
        //o valor de i deve ser diferente da quantidade de itens da página
        //para não pegar um filho que encontra-se como nulo
        if(i != arvore->n)
            cout << arvore->r[i].chave << " ";
        i++;
    }
}

void InsereNaPagina(TipoApontador ap, TipoRegistro reg, TipoApontador apDir, int &comparacoesInsercao){
    short naoAchouPosicao;
    int k = ap->n;

    //Se não achou uma posição específica, podemos inserir diretamente 
    naoAchouPosicao = k > 0;

    while(naoAchouPosicao){
        if(reg.chave >= ap->r[k-1].chave){
            comparacoesInsercao++;
            naoAchouPosicao = false;
            break;
        }
        //empura os elementos da nova página para dar entrada ao novo elemento
        ap->r[k] = ap->r[k-1];
        ap->p[k+1] = ap->p[k];
        k--;

        if(k < 1) 
            naoAchouPosicao = false;
    }

    ap->r[k] = reg;
    ap->p[k+1] = apDir;
    ap->n++;
}

void Ins(TipoRegistro reg, TipoApontador ap, short *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno, int &comparacoesInsercao){
    long i = 1, j;
    TipoApontador apTemporario;

    //Se a página está vazia, devemos criar uma nova página
    if(ap == NULL){
        *cresceu = true;
        (*regRetorno) = reg;
        (*apRetorno) = NULL;
        return;
    }

    //Retorna a provável posição que um item pode estar 
    //Caso o valor i seja menor que o nº de itens significa que não precisamos mais avaliar o valor de i
    //Caso seja menor, incrementamos seu valor até que o registro anterior seja menor que a chave passada
    while(i < ap->n && reg.chave > ap->r[i-1].chave) {
        comparacoesInsercao++;
        i++;
    }
    //Caso o registro seja encontrado na posiçao, não podemos inserir
    if(reg.chave == ap->r[i-1].chave){
        comparacoesInsercao++;
        printf("Erro: Registro já está presente\n");
        *cresceu = false;
        return;
    }

    //Se o registro for menor, decrementa-se o i para acessar a subárvore a esquerda 
    if(reg.chave < ap->r[i-1].chave){
        comparacoesInsercao++;
        i--;
    }
    
    
    Ins(reg, ap->p[i], cresceu, regRetorno, apRetorno, comparacoesInsercao);

    //Se a página cresceu, precisamos continuar o processo de inserçao
    if(!*cresceu) 
        return;

    //Se o número de itens for menor que a 2*ordem signigica que temos espaço
    if(ap->n < 2*M){
        InsereNaPagina(ap, *regRetorno, *apRetorno, comparacoesInsercao);
        *cresceu = false;
        return;
    }

    /* Overflow: página tem que ser dividida */
    apTemporario = (TipoApontador) malloc(sizeof(TipoPagina));
    apTemporario->n = 0;
    apTemporario->p[0] = NULL;

    //Se o valor da posição onde vamos inserir é menor que a metade da pagina, entao podemos inserir
    if(i < M + 1){
        //insere o ultimo elemento na página temporária
        InsereNaPagina(apTemporario, ap->r[M*2 - 1], ap->p[M*2], comparacoesInsercao);
        ap->n--;
        //insere o novo registro na página que já eregistia
        InsereNaPagina(ap, *regRetorno, *apRetorno, comparacoesInsercao);
    } else 
        //passamos a página temporária e o filho a direita
        InsereNaPagina(apTemporario, *regRetorno, *apRetorno, comparacoesInsercao);

    //Insere os maiores elementos na nova página temporária
    for(j = M + 2; j <= M*2; j++){
        InsereNaPagina(apTemporario, ap->r[j-1], ap->p[j], comparacoesInsercao);
    }

    //Atualiza a quantidade de itens 
    ap->n = M;
    //Apontador 0 da nova página recebe o apontador antigo do elemento que irá sair
    apTemporario->p[0] = ap->p[M+1];
    //Retorna o elemento que será inserido na página acima
    *regRetorno = ap->r[M];
    //Retorna a nova página criada
    *apRetorno = apTemporario;

}

void Insere(TipoRegistro reg, TipoApontador *ap, int &comparacoesInsercao){
    short cresceu;
    TipoRegistro regRetorno;
    TipoPagina *apRetorno, *apTemporario;

    Ins(reg, *ap, &cresceu, &regRetorno, &apRetorno, comparacoesInsercao);

    //Se a árvore crescer, é necessário criar uma nova página raiz
    if(cresceu){
        apTemporario = (TipoPagina*) malloc(sizeof(TipoPagina));
        apTemporario->n = 1;
        apTemporario->r[0] = regRetorno;
        apTemporario->p[1] = apRetorno;
        apTemporario->p[0] = *ap;
        *ap = apTemporario;
    }
}