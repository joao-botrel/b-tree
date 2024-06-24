#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

int main (void){
    btree *arv;
    arv = criarBTree();
    inserir(arv, 10, 0);
    inserir(arv, 20, 1);
    inserir(arv, 30, 2);
    inserir(arv, 40, 3);
    inserir(arv, 50, 4);
    inserir(arv, 60, 5);
    inserir(arv, 170, 6);
    inserir(arv, 15320, 7);
    inserir(arv, 112410, 8);
    inserir(arv, 351230, 9);
    inserir(arv, 11230, 10);
    inserir(arv, 61230, 11);
    inserir(arv, 65480, 12);
    inserir(arv, 1245125, 13);
    inserir(arv, 125612, 14);
    inserir(arv, 61234, 15);
    inserir(arv, 51251, 16);
    inserir(arv, 11, 17);

    imprimirEmOrdem(getRaiz(arv));

    no *raiz = getRaiz(arv);

    remover(raiz, 10);
    remover(raiz, 20);
    remover(raiz, 30);
    remover(raiz, 40);
    remover(raiz, 50);
    remover(raiz, 60);
    remover(raiz, 170);
    remover(raiz, 15320);
    remover(raiz, 112410);
    remover(raiz, 351230);
    remover(raiz, 11230);
    remover(raiz, 61230);
    remover(raiz, 65480);
    remover(raiz, 1245125);
    remover(raiz, 125612);
    remover(raiz, 61234);
    remover(raiz, 51251);
    remover(raiz, 11);

    imprimirEmOrdem(getRaiz(arv));

    return 0;
}