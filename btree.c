#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct chave
{
    int chave;
    int indice;
} chave;

typedef struct no
{
    int nroChaves;               // numero de chaves atuais no nó
    chave chaves[ORDEM_MAX - 1]; // vetor de chaves
    struct no *pai;
    struct no *filhos[ORDEM_MAX]; // ponteiros para os filhos do nó
    int folha;                    // flag para indicar se o nó é folha ou não, se é folha é 1, e caso
                                  // contrário é 0
} no;

// estrutura da btree
typedef struct btree
{
    struct no *raiz;
    int nroChaves;
} btree;

// função para inicializar todos os dados do nó
no *inicializarNo(int folha)
{
    no *novoNo;
    novoNo = (no *)malloc(sizeof(no));
    if (!novoNo)
        exit(1);
    novoNo->nroChaves = 0;
    novoNo->folha = folha;
    novoNo->pai = NULL;
    for (int i = 0; i < ORDEM_MAX; i++)
    {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

// função que inicializa a btree
btree *criarBTree()
{
    btree *arv;
    arv = (btree *)malloc(sizeof(btree));
    arv->nroChaves = 0;
    arv->raiz = NULL;
    return arv;
}

// função que pega a raíz de uma árvore b
no *getRaiz(btree *arv)
{
    if (arv != NULL)
        return arv->raiz;
    return NULL;
}

// função para inserir na b-tree
void inserir(btree *arv, int chave, int index)
{
    if (arv->raiz == NULL)
    { // primeiro nó da árvore, é a raíz, então é
      // necessário ajustar todos detalhes quanto à isso
        no *novoNo = inicializarNo(1);
        novoNo->chaves[0].chave = chave;
        novoNo->chaves[0].indice = index;
        novoNo->nroChaves++;
        arv->raiz = novoNo;
    }
    else
    { // caso não for raíz,
        if (arv->raiz->nroChaves ==
            ORDEM_MAX - 1)
        { // vai ter que dar split na raiz caso ela esteja na
          // capacidade máxima
            no *novaRaiz = inicializarNo(0);
            novaRaiz->filhos[0] = arv->raiz;
            arv->raiz->pai = novaRaiz; // Atualiza o ponteiro pai
            split(novaRaiz, 0);
            arv->raiz = novaRaiz;
        }
        inserirNaoCheio(arv->raiz, chave,
                        index); // se não, ou é só inserir no nó ou é em outro nó
                                // (filhos da raíz)
        arv->nroChaves++;
    }
}

// função para inserir em um nó que não está cheio na b-tree
void inserirNaoCheio(no *no, int chave, int index)
{
    int i = no->nroChaves - 1;

    if (no->folha == 1)
    { // se o nó for folha, basta inserir nele
        while (i >= 0 &&
               no->chaves[i].chave >
                   chave)
        { // achando a posição correta do valor nas chaves do nó
            no->chaves[i + 1].chave = no->chaves[i].chave;
            no->chaves[i + 1].indice = no->chaves[i].indice;
            i--;
        }
        // consertando os outros dados do nó
        no->chaves[i + 1].chave = chave;
        no->chaves[i + 1].indice = index;
        no->nroChaves++;
    }
    else
    {
        // se o nó não for folha, procurar onde deve inserir
        while (i >= 0 && no->chaves[i].chave > chave)
        {
            i--;
        }
        i++;
        // se o nó que for inserir estiver cheio, é preciso dar split nele
        if (no->filhos[i]->nroChaves == ORDEM_MAX - 1)
        {
            split(no, i);
            if (no->chaves[i].chave < chave)
            {
                i++;
            }
        }
        // chama a função de novo para inserir o valor no seu nó correto
        inserirNaoCheio(no->filhos[i], chave, index);
    }
}

// função que realiza a operação de split na b-tree
void split(no *pai, int i)
{
    no *y = pai->filhos[i];
    no *novoNo = inicializarNo(y->folha);

    novoNo->nroChaves = ORDEM_MAX / 2 - 1;
    novoNo->pai = pai; // Atualiza o ponteiro pai do novo nó

    // Os dois primeiros loops são feitos para mover os dados dos filhos do nó pai
    // para o novo nó filho Loop para as chaves
    for (int j = 0; j < ORDEM_MAX / 2 - 1; j++)
    {
        novoNo->chaves[j].chave = y->chaves[j + ORDEM_MAX / 2].chave;
        novoNo->chaves[j].indice = y->chaves[j + ORDEM_MAX / 2].indice;
    }

    // Loop para os ponteiros dos filhos
    if (y->folha == 0)
    {
        for (int j = 0; j < ORDEM_MAX / 2; j++)
        {
            novoNo->filhos[j] = y->filhos[j + ORDEM_MAX / 2];
            if (novoNo->filhos[j] != NULL)
            {
                novoNo->filhos[j]->pai =
                    novoNo; // Atualiza o ponteiro pai dos filhos do novo nó
            }
        }
    }

    y->nroChaves = ORDEM_MAX / 2 - 1;

    // Os dois últimos loops são shifts para inserir o novo filho no seu lugar
    // certo no nó pai Neste loop, é para achar o lugar correto para inseri-lo nos
    // ponteiros de filhos
    for (int j = pai->nroChaves; j > i; j--)
    {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    // Lugar achado, inserindo:
    pai->filhos[i + 1] = novoNo;

    // Neste loop, é para achar o lugar correto para inseri-lo nas chaves
    for (int j = pai->nroChaves - 1; j >= i; j--)
    {
        pai->chaves[j + 1].chave = pai->chaves[j].chave;
        pai->chaves[j + 1].indice = pai->chaves[j].indice;
    }
    // Lugar achado, inserindo:
    pai->chaves[i].chave = y->chaves[ORDEM_MAX / 2 - 1].chave;
    pai->chaves[i].indice = y->chaves[ORDEM_MAX / 2 - 1].indice;
    pai->nroChaves++;
}

// função para imprimir a b-tree em ordem
void imprimirEmOrdem(no *raiz)
{
    if (raiz != NULL)
    {
        int i;
        for (i = 0; i < raiz->nroChaves; i++)
        {
            imprimirEmOrdem(raiz->filhos[i]);
            printf("Nó: %d ", raiz->chaves[i].chave);
            if (raiz->pai != NULL)
            {
                printf("(Pai: %d) ",
                       raiz->pai->chaves[0].chave); // Assumindo que estamos imprimindo
                                                    // a primeira chave do pai
            }
            else
            {
                printf("(Pai: NULL) ");
            }
            printf("\n");
        }
        imprimirEmOrdem(raiz->filhos[i]);
    }
}

void imprimirPreOrdem(no *raiz)
{
    if (raiz != NULL)
    {
        // Primeiro, imprima o nó atual
        int i;
        for (i = 0; i < raiz->nroChaves; i++)
        {
            printf("Nó: %d ", raiz->chaves[i].chave);
            if (raiz->pai != NULL)
            {
                printf("(Pai: %d) ",
                       raiz->pai->chaves[0].chave); // Assumindo que estamos imprimindo
                                                    // a primeira chave do pai
            }
            else
            {
                printf("(Pai: NULL) ");
            }
            printf("\n");
        }
        // Depois, chame recursivamente a função para cada filho
        for (i = 0; i <= raiz->nroChaves; i++)
        {
            imprimirPreOrdem(raiz->filhos[i]);
        }
    }
}

int buscar(no *no, int chave)
{
    int i = 0;
    while ((i < no->nroChaves) && (chave > no->chaves[i].chave))
    {
        i++;
    }
    if (i < no->nroChaves && chave == no->chaves[i].chave)
    {
        return no->chaves[i].indice;
    }
    else if (no->folha == 1)
    {
        return -1;
    }
    else
    {
        buscar(no->filhos[i], chave);
    }
}

int remover(btree *arv, no *no, int chave)
{
    int indice = -1;
    int i = 0;
    while (indice == -1)
    {
        while ((i < no->nroChaves) && (chave > no->chaves[i].chave))
        {
            i++;
        }
        if (i < no->nroChaves && chave == no->chaves[i].chave)
        {
            indice = i;
        }
        else if (no->folha == 1)
        {
            return -1;
        }
        else
        {
            no = no->filhos[i];
            i = 0;
        }
    }
    if (no->folha == 1)
    {
        removerFolha(arv, no, chave, indice);
    }
    else
    {
        removerNaoFolha(arv, no, chave, indice);
    }
    return 0;
}

void removerFolha(btree *arv, no *no, int chave, int indice)
{

    int j = indice;
    while (j < no->nroChaves - 1)
    {
        no->chaves[j].chave = no->chaves[j + 1].chave;
        no->chaves[j].indice = no->chaves[j + 1].indice;
        j++;
    }
    no->nroChaves--;

    if (no->nroChaves < ORDEM_MAX / 2 - 1 && no->pai != NULL)
    {
        balancear(arv, no);
    }
}

void removerNaoFolha(btree *arv, no *removido, int chave, int indice)
{
    no *y = removido->filhos[indice];
    while (y->folha == 0)
    {
        y = y->filhos[y->nroChaves];
    }
    removido->chaves[indice].chave = y->chaves[y->nroChaves - 1].chave;
    removido->chaves[indice].indice = y->chaves[y->nroChaves - 1].indice;
    removerFolha(arv, y, y->chaves[y->nroChaves - 1].chave, y->nroChaves - 1);
}

void unir(btree *arv, no *x, no *y, int indice)
{
    int i = 0;
    x->chaves[x->nroChaves].chave = y->pai->chaves[indice].chave;
    x->chaves[x->nroChaves].indice = y->pai->chaves[indice].indice;
    x->nroChaves++;
    for (int j = indice; j < y->pai->nroChaves - 1; j++)
    {
        y->pai->chaves[j].chave = y->pai->chaves[j + 1].chave;
        y->pai->chaves[j].indice = y->pai->chaves[j + 1].indice;
    }
    y->pai->nroChaves--;
    while (i < y->nroChaves)
    {
        x->chaves[x->nroChaves].chave = y->chaves[i].chave;
        x->chaves[x->nroChaves].indice = x->chaves[i].indice;
        x->nroChaves++;
        i++;
    }
    if (y->folha == 0)
    {
        x->filhos[x->nroChaves] = y->filhos[i];
        if (x->filhos[x->nroChaves] != NULL)
        {
            x->filhos[x->nroChaves]->pai = x;
        }
    }
    free(y);
    x->pai->filhos[indice + 1] = NULL;
    if (x->pai->pai == NULL)
    {
        x->pai = NULL;
        arv->raiz = x;
    }
    if (x->pai != NULL && x->pai->nroChaves < ORDEM_MAX / 2 - 1)
    {
        balancear(arv, x->pai);
    }
}

void emprestar(btree *arv, no *x, no *y, int indice, int pos)
{
    x->nroChaves++;
    x->chaves[x->nroChaves - 1].chave = x->pai->chaves[indice].chave;
    x->chaves[x->nroChaves - 1].indice = x->pai->chaves[indice].indice;
    x->pai->chaves[indice].chave = y->chaves[pos].chave;
    x->pai->chaves[indice].indice = y->chaves[pos].indice;

    if (pos == 0)
    {
        x->filhos[x->nroChaves] = y->filhos[pos];
        y->filhos[pos] = NULL;
        if (x->filhos[x->nroChaves] != NULL)
        {
            x->filhos[x->nroChaves]->pai = x;
        }
        for (int i = 0; i < y->nroChaves - 1; i++)
        {
            y->chaves[i].chave = y->chaves[i + 1].chave;
            y->chaves[i].indice = y->chaves[i + 1].indice;
            y->filhos[i] = y->filhos[i + 1];
        }
        y->filhos[y->nroChaves - 1] = y->filhos[y->nroChaves];
        y->filhos[y->nroChaves] = NULL;
    }
    else
    {
        x->filhos[x->nroChaves] = y->filhos[pos + 1];
        y->filhos[pos + 1] = NULL;
        if (x->filhos[x->nroChaves] != NULL)
        {
            x->filhos[x->nroChaves]->pai = x;
        }
        y->filhos[pos + 1] = NULL;
    }

    y->nroChaves--;
}

void balancear(btree *arv, no *no)
{
    int indice;
    int i = 0;
    while (i <= no->pai->nroChaves && no->pai->filhos[i] != no)
    {
        if (no->pai->filhos[i] == no)
        {
            indice = i;
            i--;
        }
        i++;
    }
    indice = i;
    if (indice > 0 && no->pai->filhos[indice - 1] != NULL)
    {
        if (no->pai->filhos[indice - 1]->nroChaves > ORDEM_MAX / 2 - 1)
        {
            emprestar(arv, no, no->pai->filhos[indice - 1], indice,
                      no->pai->filhos[indice - 1]->nroChaves - 1);
        }
        else if (indice < no->pai->nroChaves - 1 &&
                 no->pai->filhos[indice + 1] != NULL)
        {
            if (no->pai->filhos[indice + 1]->nroChaves > ORDEM_MAX / 2 - 1)
            {
                emprestar(arv, no, no->pai->filhos[indice + 1], indice, 0);
            }
            else
            {
                unir(arv, no, no->pai->filhos[indice + 1], indice);
            }
        }
        else
        {
            unir(arv, no->pai->filhos[indice - 1], no, indice - 1);
        }
    }
    else if (indice < no->pai->nroChaves &&
             no->pai->filhos[indice + 1] != NULL)
    {
        if (no->pai->filhos[indice + 1]->nroChaves > ORDEM_MAX / 2 - 1)
        {
            emprestar(arv, no, no->pai->filhos[indice + 1], indice, 0);
        }
        else
        {
            unir(arv, no, no->pai->filhos[indice + 1], indice);
        }
    }
    else
    {
        unir(arv, no->pai->filhos[indice - 1], no, indice - 1);
    }
}

void benchmarkBusca()
{
    // Construção do cabeçalho da tabela
    double tempo_total_btree = 0.0;
    double tempo_total_arquivo = 0.0;
    double maior_tempo_btree = 0.0;
    double maior_tempo_arquivo = 0.0;
    double menor_tempo_btree = 100000.0;
    double menor_tempo_arquivo = 100000.0;

    btree *arv = criarBTree();

    FILE *arq = fopen(criaArquivo("btree", "Busca"), "w");

    for (int i = 0; i < 10000; i++)
    {
        int elemento; // func que le cada linha do arquivo
        // inserir(arv, elemento.chave,elemento.indice);
    }

    // Ciclo de repetição de testes
    for (int i = 0; i < 30; i++)
    {
            srand(time(NULL));

            // Gera um número aleatório no intervalo de 1 a 10.000
            int num_busca = (rand() % 10000) + 1;
            clock_t start = clock(); // inicia a contagem de tempo
            int indice = buscar(arv->raiz, num_busca);
            //imprime linha indice do arquivo
            clock_t end = clock(); // finaliza contagem de tempo

            tempo_total_btree += ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            if(((double)(end - start)) * 1000 / CLOCKS_PER_SEC > maior_tempo_btree){
                maior_tempo_btree = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            }
            if(((double)(end - start)) * 1000 / CLOCKS_PER_SEC < menor_tempo_btree){
                menor_tempo_btree = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            }

            start = clock(); // inicia a contagem de tempo
            //busca num_busca no arquivo
            end = clock(); // finaliza contagem de tempo

            tempo_total_arquivo += ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            if(((double)(end - start)) * 1000 / CLOCKS_PER_SEC > maior_tempo_arquivo){
                maior_tempo_arquivo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            }
            if(((double)(end - start)) * 1000 / CLOCKS_PER_SEC < menor_tempo_arquivo){
                menor_tempo_arquivo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            }
    }

    FILE *result;
    result = fopen("resultados.txt", "a");
    fprintf(result, "\n\n\n-------------------------------------------------------------------------------Tabela 1: Busca na btree------------------------------------------------------------------------------------\n\n");

    fprintf(result, "Média de tempo de busca na btree: %f ms\n", tempo_total_btree / 30);
    fprintf(result, "Maior tempo de busca na btree: %f ms\n", maior_tempo_btree);
    fprintf(result, "Menor tempo de busca na btree: %f ms\n", menor_tempo_btree);

    fprintf(result, "\n\n\n-------------------------------------------------------------------------------Tabela 2: Busca no arquivo------------------------------------------------------------------------------------\n\n");

    fprintf(result, "Média de tempo de busca no arquivo: %f ms\n", tempo_total_arquivo / 30);
    fprintf(result, "Maior tempo de busca no arquivo: %f ms\n", maior_tempo_arquivo);
    fprintf(result, "Menor tempo de busca no arquivo: %f ms\n", menor_tempo_arquivo);

}

