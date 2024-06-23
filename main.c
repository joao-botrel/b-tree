#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

int main (void){
    btree *arv;
    arv = criarBTree();
    inserir(arv, 10);
    inserir(arv, 20);
    inserir(arv, 30);
    inserir(arv, 40);
    inserir(arv, 50);
    inserir(arv, 60);
    inserir(arv, 170);
    inserir(arv, 15320);
    inserir(arv, 112410);
    inserir(arv, 351230);
    inserir(arv, 11230);
    inserir(arv, 61230);
    inserir(arv, 65480);
    inserir(arv, 1245125);
    inserir(arv, 125612);
    inserir(arv, 61234);
    inserir(arv, 51251);
    inserir(arv, 11);

    imprimirEmOrdem(getRaiz(arv));

    return 0;
}