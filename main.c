#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
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

  imprimirPreOrdem(getRaiz(arv));

  remover(arv, getRaiz(arv), 6);
  printf("\n");

  imprimirPreOrdem(getRaiz(arv));
  remover(arv, getRaiz(arv), 41);
  printf("\n");

  imprimirPreOrdem(getRaiz(arv));
  remover(arv, getRaiz(arv), 14);
  printf("\n");

  imprimirPreOrdem(getRaiz(arv));
  remover(arv, getRaiz(arv), 38);
  printf("\n");

  imprimirPreOrdem(getRaiz(arv));
  remover(arv, getRaiz(arv), 1);
  printf("\n");

  inserir(arv, 144, 11);
  inserir(arv, 13, 12);

  imprimirPreOrdem(getRaiz(arv));
  remover(arv, getRaiz(arv), 3);
  printf("\n");

  imprimirPreOrdem(getRaiz(arv));

  return 0;
}