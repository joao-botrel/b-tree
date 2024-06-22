
#define ORDEM_MAX 7

typedef struct no no;

//estrutura da b-tree
typedef struct btree btree;

btree* criarBTree ();
no *inicializarNo(int folha);
void inserir(btree *arv, int chave);
void inserirNaoCheio(no *no, int chave);
void split();
int remover();
void emprestar;
void unir();
void buscar();




