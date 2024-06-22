#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

typedef struct no {
    int nroChaves; //numero de chaves atuais no nó
    //conteúdo do arquivo;
    int chaves[ORDEM_MAX]; //valor das chaves 
    struct no *filhos[ORDEM_MAX]; //ponteiros para os filhos do nó
    int folha; //flag para indicar se o nó é folha ou não
} no;

//estrutura da b-tree
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

//função para inserir na b-tree
void inserir(btree *arv, int chave){
    if (arv->raiz == NULL){ //primeiro nó da árvore, é a raíz
        no* novoNo = inicializarNo(1);
        novoNo->chaves[0] = chave;
        novoNo->nroChaves++;
        arv->raiz = novoNo;
    }
    if (arv->raiz->nroChaves  == ORDEM_MAX - 1){ //vai ter que dar split na raiz caso ela esteja na capacidade máxima
        no *novaRaiz =  inicializarNo(0);
        if (chave > arv->raiz->chaves[0]) {
            novaRaiz->filhos[1] = arv->raiz;
        } else {
            novaRaiz->filhos[0] = arv->raiz;
        }
        split(arv->raiz, 0);
        arv->raiz = novaRaiz;

    } else inserirNaoCheio(arv->raiz, chave); //se não, ou é só inserir no nó ou é em outro nó (filhos da raíz)
    arv->nroChaves++;
}

//função para inserir em um nó que não está cheio na b-tree
void inserirNaoCheio(no *no, int chave){
    int i = no->nroChaves;
    
    if (no->folha == 1){ //se o nó for folha, basta inserir nele
        while (i >= 0 && no->chaves[i] > chave){ //achando a posição correta do valor nas chaves do nó
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        //consertando os outros dados do nó
        no->chaves[i+ 1] = chave;
        no->nroChaves++;
    } else {
        //se o nó não for folha, procurar onde deve inserir
        while (i >= 0 && no->chaves[i] > chave){
            i--;
        } i++;
        //se o nó que for inserir estiver cheio, é preciso dar split nele
        if (no->filhos[i]->nroChaves == ORDEM_MAX - 1){
            split(no, i);
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        //chama a função de novo para inserir o valor no seu nó correto
        inserirNaoCheio(no->filhos[i], chave);
    }

}

//função que realiza a operação de split na b-tree
void split (no *pai, int i){
    no *y = pai->filhos[i];
    no *novoNo = inicializarNo(y->folha);

    novoNo->nroChaves = ORDEM_MAX/2 - 1;

    for (int i = 0; i < ORDEM_MAX/2; i++){
        novoNo->chaves[i] = y->chaves[i + ORDEM_MAX/2];
    }

    if (y->folha == 0){
       //continuar daqui
    }



}