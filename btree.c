#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

typedef struct no {
    int nroChaves; //numero de chaves atuais no nó
    int chaves[ORDEM_MAX-1]; //valor das chaves 
    int index[ORDEM_MAX-1];
    struct no *pai;
    struct no *filhos[ORDEM_MAX]; //ponteiros para os filhos do nó
    int folha; //flag para indicar se o nó é folha ou não, se é folha é 1, e caso contrário é 0
} no;

//estrutura da btree
typedef struct btree {
    struct no *raiz;
    int nroChaves;
} btree;

//função para inicializar todos os dados do nó
no *inicializarNo(int folha){
    no *novoNo;
    novoNo = (no*) malloc (sizeof(no));
    if (!novoNo) exit(1);
    novoNo->nroChaves = 0;
    novoNo->folha = folha;
    novoNo->pai = NULL;
    for (int i = 0; i < ORDEM_MAX; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

//função que inicializa a btree
btree* criarBTree (){
    btree *arv;
    arv = (btree*)malloc(sizeof(btree));
    arv->nroChaves = 0;
    arv->raiz = NULL;
    return arv;
}

//função que pega a raíz de uma árvore b
no *getRaiz(btree *arv){
    if (arv != NULL)
        return arv->raiz;
    return NULL;
}

//função para inserir na b-tree
void inserir(btree *arv, int chave, int index){
    if (arv->raiz == NULL){ //primeiro nó da árvore, é a raíz, então é necessário ajustar todos detalhes quanto à isso
        no* novoNo = inicializarNo(1);
        novoNo->chaves[0] = chave;
        novoNo->index[0] = index;
        novoNo->nroChaves++;
        arv->raiz = novoNo;
    } else { //caso não for raíz,
         if (arv->raiz->nroChaves == ORDEM_MAX - 1){ //vai ter que dar split na raiz caso ela esteja na capacidade máxima
            no *novaRaiz = inicializarNo(0);
            novaRaiz->filhos[0] = arv->raiz;
            arv->raiz->pai = novaRaiz; // Atualiza o ponteiro pai
            arv->raiz = novaRaiz;
            split(novaRaiz, 0);
            novaRaiz->nroChaves = 0;
            inserirNaoCheio(novaRaiz, chave, index); 

        } else inserirNaoCheio(arv->raiz, chave, index); //se não, ou é só inserir no nó ou é em outro nó (filhos da raíz)

    }
    arv->nroChaves++;
}

//função para inserir em um nó que não está cheio na b-tree
void inserirNaoCheio(no *no, int chave, int index){
    int i = no->nroChaves - 1;

    if (no->folha == 1){ //se o nó for folha, basta inserir nele
        while (i >= 0 && no->chaves[i] > chave){ //achando a posição correta do valor nas chaves do nó
            no->chaves[i + 1] = no->chaves[i];
            no->index[i + 1] = no->index[i];
            i--;
        }
        //consertando os outros dados do nó
        no->chaves[i + 1] = chave;
        no->index[i + 1] = index;
        no->nroChaves++;
    } else {
        //se o nó não for folha, procurar onde deve inserir
        while (i >= 0 && no->chaves[i] > chave){
            i--;
        }
        i++;
        //se o nó que for inserir estiver cheio, é preciso dar split nele
        if (no->filhos[i]->nroChaves == ORDEM_MAX - 1){
            split(no, i);
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        //chama a função de novo para inserir o valor no seu nó correto
        inserirNaoCheio(no->filhos[i], chave, index);
    }
}

//função que realiza a operação de split na b-tree
void split(no *pai, int i){
    no *y = pai->filhos[i];
    no *novoNo = inicializarNo(y->folha);

    novoNo->nroChaves = ORDEM_MAX / 2 - 1;
    novoNo->pai = pai; // Atualiza o ponteiro pai do novo nó

    //Os dois primeiros loops são feitos para mover os dados dos filhos do nó pai para o novo nó filho
    //Loop para as chaves
    for (int j = 0; j < ORDEM_MAX / 2 - 1; j++){
        novoNo->chaves[j] = y->chaves[j + ORDEM_MAX / 2];
        novoNo->index[j] = y->index[j + ORDEM_MAX / 2];
    }

    //Loop para os ponteiros dos filhos
    if (y->folha == 0){
        for (int j = 0; j < ORDEM_MAX / 2; j++){
            novoNo->filhos[j] = y->filhos[j + ORDEM_MAX / 2];
            if (novoNo->filhos[j] != NULL) {
                novoNo->filhos[j]->pai = novoNo; // Atualiza o ponteiro pai dos filhos do novo nó
            }
        }
    }

    y->nroChaves = ORDEM_MAX / 2 - 1;

    //Os dois últimos loops são shifts para inserir o novo filho no seu lugar certo no nó pai
    //Neste loop, é para achar o lugar correto para inseri-lo nos ponteiros de filhos
    for (int j = pai->nroChaves; j > i; j--){
        pai->filhos[j + 1] = pai->filhos[j];
    }

    //Lugar achado, inserindo:
    pai->filhos[i + 1] = novoNo;

    //Neste loop, é para achar o lugar correto para inseri-lo nas chaves
    for (int j = pai->nroChaves - 1; j >= i; j--){
        pai->chaves[j + 1] = pai->chaves[j];
        pai->index[j + 1] = pai->index[j];
    }
    //Lugar achado, inserindo:
    pai->chaves[i] = y->chaves[ORDEM_MAX / 2 - 1];  
    pai->index[i] = y->index[ORDEM_MAX / 2 - 1];  
    pai->nroChaves++;
}

//função para imprimir a b-tree em ordem
void imprimirEmOrdem(no *raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->nroChaves; i++) {
            imprimirEmOrdem(raiz->filhos[i]);
            printf("Nó: %d ", raiz->chaves[i]);
            if (raiz->pai != NULL) {
                printf("(Pai: %d) ", raiz->pai->chaves[0]); // Assumindo que estamos imprimindo a primeira chave do pai
            } else {
                printf("(Pai: NULL) ");
            }
            printf("\n");
        }
        imprimirEmOrdem(raiz->filhos[i]);
    }
}


int buscar(no *no, int chave){
    int i=1;
    while ((i <= no->nroChaves) && (chave > no->chaves[i])){
        i++;
    }
    if(i <= no->nroChaves && chave == no->chaves[i]){
        return no->index[i];
    } else if (no->folha == 1){
        return -1;
    } else {
        buscar(no->filhos[i], chave);
    }
}

int remover(no *no, int chave){
    int indice=-1;
    int i=0;
    while (indice==-1){
        while ((i <= no->nroChaves) && (chave > no->chaves[i])){
            i++;
        }
        if(i <= no->nroChaves && chave == no->chaves[i]){
            indice=i;

        } else if (no->folha == 1){
            return -1;
        } else {
            if(no->filhos[i]!=NULL && no->filhos[i]->nroChaves < ORDEM_MAX/2-1){
                if(i>0 && no->filhos[i-1]!=NULL && no->filhos[i-1]->nroChaves >ORDEM_MAX/2-1 ){
                    emprestar(no, chave, i-1);
                } else if(i<= ORDEM_MAX && no->filhos[i+1]!=NULL && no->filhos[i+1]->nroChaves >ORDEM_MAX/2-1){
                    emprestar(no, chave, i+1);
                } else {
                    unir(no, chave, i);
                }
            } else {
                no=no->filhos[i];
            }
           no = no->filhos[i];
        }
    }
    if (no->folha == 1){
        removerFolha(no, chave, indice);
    } else {
        removerNaoFolha(no, chave, indice);
    }
}

void removerFolha(no *no, int chave, int indice){

        int j=indice;
        while (j < no->nroChaves){
            no->chaves[j]=no->chaves[j+1];
            no->index[j]=no->index[j+1];
            j++;
        }
        no->nroChaves--;
}

void removerNaoFolha(no *removido, int chave, int indice){
    if (removido->filhos[indice]->nroChaves >= ORDEM_MAX/2){
        no *y=removido->filhos[indice];
        while (y->folha == 0){
            y=y->filhos[y->nroChaves];
        }
        removido->chaves[indice]=y->chaves[y->nroChaves];
        removido->index[indice]=y->index[y->nroChaves];
        remover(y, y->chaves[y->nroChaves]);
    } else if (removido->filhos[indice+1]->nroChaves >= ORDEM_MAX/2){
        no *y=removido->filhos[indice+1];
        while (y->folha == 0){
            y=y->filhos[0];
        }
        removido->chaves[indice]=y->chaves[0];
        removido->index[indice]=y->index[0];
        remover(y, y->chaves[0]);
    } else {
        unir(removido, chave, indice);
    }
}

void unir(no *x, int chave, int indice){
    no *y;
    no *z;

    if(x->filhos[indice-1]!=NULL){
        y=x->filhos[indice-1];
        z=x->filhos[indice];
        y->chaves[y->nroChaves]=x->chaves[indice-1];
        y->index[y->nroChaves]=x->index[indice-1];
    }else{
        y=x->filhos[indice];
        z=x->filhos[indice+1];
        y->chaves[y->nroChaves]=x->chaves[indice];
        y->index[y->nroChaves]=x->index[indice];
    }

    y->nroChaves++;
    for (int i=0; i< z->nroChaves; i++){
        y->chaves[y->nroChaves]=z->chaves[i];
        y->index[y->nroChaves]=z->index[i];
        y->nroChaves++;
    }
    for (int i=indice; i<x->nroChaves; i++){
        x->chaves[i]=x->chaves[i+1];
        x->index[i]=x->index[i+1];
    }
    for (int i=indice+1; i<x->nroChaves; i++){
        x->filhos[i]=x->filhos[i+1];
    }
    x->nroChaves--;
    free(z);
}

void emprestar(no *x, int chave, int indice){
    if(x->filhos[indice-1]!=NULL){
        no *y=x->filhos[indice-1];
        no *z=x->filhos[indice];
        y->chaves[y->nroChaves]=x->chaves[0];
        y->index[y->nroChaves]=x->index[0];
        y->nroChaves++;
        x->chaves[0]=y->chaves[0];
        x->index[0]=y->index[0];
        x->nroChaves--;
        for (int i=0; i<z->nroChaves; i++){
            z->chaves[i]=z->chaves[i+1];
            z->index[i]=z->index[i+1];
        }
        z->nroChaves--;
    }else{
        no *y=x->filhos[indice];
        no *z=x->filhos[indice+1];
         for (int i=z->nroChaves; i>0; i--){
            z->chaves[i]=z->chaves[i-1];
            z->index[i]=z->index[i-1];
        }
        z->chaves[0]=x->chaves[x->nroChaves-1];
        z->index[0]=x->index[x->nroChaves-1];
        z->nroChaves++;
        x->chaves[x->nroChaves-1]=y->chaves[y->nroChaves-1];
        x->index[x->nroChaves-1]=y->index[y->nroChaves-1];
        y->nroChaves--;
    }
}