
#define ORDEM_MAX 6

typedef struct no no;

//estrutura da b-tree
typedef struct btree btree; 

btree* criarBTree ();
no *inicializarNo(int folha);
no* getRaiz(btree *arv);
void inserir(btree *arv, int chave, int index);
void inserirNaoCheio(no *no, int chave, int index);
void split(no *pai, int i);
void imprimirEmOrdem(no *raiz);
int remover(no *no, int chave);
void removerFolha(no *no, int chave, int indice);
void removerNaoFolha(no *removido, int chave, int indice);
void unir(no *x, int chave, int indice);
void emprestar(no *x, int chave, int indice);
int buscar(no *no, int chave);