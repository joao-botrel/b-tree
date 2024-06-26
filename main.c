#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

int main (void){
    btree *arv;
    arv = criarBTree();
    inserir(arv, 14, 0);
    inserir(arv, 39, 1);
    inserir(arv, 1, 2);
    inserir(arv, 6, 3);
    inserir(arv, 41, 4);
    inserir(arv, 32, 5);
    inserir(arv, 8, 6);
    inserir(arv, 38, 7);
    inserir(arv, 43, 8);
    inserir(arv, 3, 9);
    inserir(arv, 36, 10);
    inserir(arv, 30, 10);


    imprimirEmOrdem(getRaiz(arv));

    return 0;
}