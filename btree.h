
#define ORDEM_MAX 4
#define TAMANHO_LINHA 32
#define TAMANHO_NOME 14
#define TAMANHO_CURSO 8
#include <stdio.h>
#include <stdlib.h>

//estrutura do nó da b-tree
typedef struct no no;

//estrutura da b-tree
typedef struct btree btree; 

//estrutura da chave que fica no nó da b-tree
typedef struct chave chave;

//estrutura do registro que será gravado no arquivo
typedef struct registro registro;

//funções básicas da b-tree
btree* criarBTree ();
no *inicializarNo(int folha);
no* getRaiz(btree *arv);

//funções de inserir o registro no arquivo
void gravarRegistro(char *nomeArquivo, registro reg);
void criaRegistro(btree *arv, char *nomeArquivo);
registro *inicializaRegistro();
void insere(btree *arv, char *nomeArquivo, int chave, registro registro);

//funções de inserir o registro na árvore
void inserir(btree *arv, int chave, int index);
void inserirNaoCheio(no *no, int chave, int index);
void split(no *pai, int i);
int contarLinhas(const char *nomearq);

//funções de busca
void buscaGeral(btree *arv, char *nomeArq, int valor, int indice);
int buscar(no *no, int chave);
void buscarNoArquivo(btree *arv, char *nomeArq, int index, int chave);
void buscarRegistro(const char *arquivo, int chave);

//funções de impressão da b-tree
void imprimirEmOrdem(no *raiz);
void imprimirPreOrdem(no *raiz);

//funções de remoção da b-tree
int remover(no *no, int chave);
void removerFolha(no *no, int chave, int indice);
void removerNaoFolha(no *removido, int chave, int indice);
void unir(no *x, no *y, int indice);
void emprestar(no *x, no *y, int indice, int pos);

//processa o arquivo e coloca suas chaves na árvore
void processaCarga(btree *arv, char *nomeArquivo);

