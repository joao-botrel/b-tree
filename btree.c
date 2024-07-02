#include "btree.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct chave {
  int chave;
  int indice;
} chave;

typedef struct no {
  int nroChaves;               // numero de chaves atuais no nó
  int m; //ordem
  chave chaves[ORDEM_MAX - 1]; // vetor de chaves
  struct no *pai;
  struct no *filhos[ORDEM_MAX]; // ponteiros para os filhos do nó
  int folha; // flag para indicar se o nó é folha ou não, se é folha é 1, e caso
             // contrário é 0
} no;

// estrutura da btree
typedef struct btree {
  struct no *raiz;
  int nroChaves;
} btree;

//struct do registro do arquivo
typedef struct registro{
    int matricula;
    char *aluno;
    char *curso;
    char periodo;
} registro;

// função para inicializar todos os dados do nó
no *inicializarNo(int folha) {
  no *novoNo;
  novoNo = (no *)malloc(sizeof(no));
  if (!novoNo)
    exit(1);
  novoNo->nroChaves = 0;
  novoNo->folha = folha;
  novoNo->pai = NULL;
  for (int i = 0; i < ORDEM_MAX; i++) {
    novoNo->filhos[i] = NULL;
  }
  return novoNo;
}

// função que inicializa a btree
btree *criarBTree() {
  btree *arv;
  arv = (btree *)malloc(sizeof(btree));
  arv->nroChaves = 0;
  arv->raiz = NULL;
  return arv;
}

// função que pega a raíz de uma árvore b
no *getRaiz(btree *arv) {
  if (arv != NULL)
    return arv->raiz;
  return NULL;
}

void gravarRegistro(char *nomeArquivo, registro reg) {
  FILE *arquivo = fopen(nomeArquivo, "a+"); 
  if (arquivo == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo.\n");
      return;
  }

  //indo para o final
    fseek(arquivo, 0, SEEK_END);
  
  if (ftell(arquivo) > 0) {
      fprintf(arquivo, "\n");
  }

    fprintf(arquivo, "%d,%s,%s,%c", reg.matricula, reg.aluno, reg.curso, reg.periodo);
  fclose(arquivo);
}

int contarLinhas(const char *nomearq) {
    FILE *file;
    int count = 0;

    // Abre o arquivo para leitura
    file = fopen(nomearq, "rb"); // Abre em modo binário
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1; // Retorna -1 em caso de erro
    }

    // Move para o final do arquivo para calcular o tamanho total
    fseek(file, 0, SEEK_END);

    // Calcula o número de linhas
    count = ftell(file) / TAMANHO_LINHA;

    // Fecha o arquivo
    fclose(file);

    return count-1;
}

registro *inicializaRegistro(){
  registro *novoRegistro;
  novoRegistro = (registro*)malloc(sizeof(registro));
  if (novoRegistro == NULL){
    printf("Não há memória suficiente.\n");
    return NULL;
  }
  novoRegistro->aluno = (char*)malloc(TAMANHO_NOME*sizeof(char));
  if (novoRegistro->aluno == NULL){
    printf("Não há memória suficiente.\n");
    return NULL;
  }

  novoRegistro->curso = (char*)malloc(TAMANHO_CURSO*sizeof(char));
  if (novoRegistro->curso == NULL){
    printf("Não há memória suficiente.\n");
    return NULL;
  }

  return novoRegistro;
}

void criaRegistro(btree *arv, char *nomeArquivo){
  registro *reg;
  reg = inicializaRegistro();
  printf("Primeiro, você deve inserir os dados do registro.\n");
      printf("Insira a matrícula (exatos 6 caracteres númericos); \n");
  scanf("%d", &reg->matricula);
  printf("Insira o nome (exatos 14 caracteres, contando espaços); \n");
  scanf("%s", reg->aluno);
  printf("Insira o curso (exatos 8 caracteres, contando espaços); \n");
  scanf("%s", reg->curso);
  printf("Insira o período (apenas 1 caracter); \n");
  scanf(" %c", &reg->periodo);
  insere(arv, nomeArquivo, reg->matricula, *reg);
  
}

//função do menu para inserir
void insere(btree *arv, char *nomeArquivo, int chave, registro registro){
    //primeiro, insere o registro no arquivo
    gravarRegistro(nomeArquivo, registro);
    //depois, pega o número de linhas
    int index = contarLinhas(nomeArquivo);
    //depois, insere na árvore
    inserir(arv, chave, index);
}

// função para inserir na b-tree
void inserir(btree *arv, int chave, int index) {
  if (arv->raiz == NULL) { // primeiro nó da árvore, é a raíz, então é
                           // necessário ajustar todos detalhes quanto à isso
    no *novoNo = inicializarNo(1);
    novoNo->chaves[0].chave = chave;
    novoNo->chaves[0].indice = index;
    novoNo->nroChaves++;
    arv->raiz = novoNo;
  } else { // caso não for raíz,
    if (arv->raiz->nroChaves ==
        ORDEM_MAX - 1) { // vai ter que dar split na raiz caso ela esteja na
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
void inserirNaoCheio(no *no, int chave, int index) {
  int i = no->nroChaves - 1;

  if (no->folha == 1) { // se o nó for folha, basta inserir nele
    while (i >= 0 &&
           no->chaves[i].chave >
               chave) { // achando a posição correta do valor nas chaves do nó
      no->chaves[i + 1].chave = no->chaves[i].chave;
      no->chaves[i + 1].indice = no->chaves[i].indice;
      i--;
    }
    // consertando os outros dados do nó
    no->chaves[i + 1].chave = chave;
    no->chaves[i + 1].indice = index;
    no->nroChaves++;
  } else {
    // se o nó não for folha, procurar onde deve inserir
    while (i >= 0 && no->chaves[i].chave > chave) {
      i--;
    }
    i++;
    // se o nó que for inserir estiver cheio, é preciso dar split nele
    if (no->filhos[i]->nroChaves == ORDEM_MAX - 1) {
      split(no, i);
      if (no->chaves[i].chave < chave) {
        i++;
      }
    }
    // chama a função de novo para inserir o valor no seu nó correto
    inserirNaoCheio(no->filhos[i], chave, index);
  }
}

// função que realiza a operação de split na b-tree
void split(no *pai, int i) {
  no *y = pai->filhos[i];
  no *novoNo = inicializarNo(y->folha);

  novoNo->nroChaves = ORDEM_MAX / 2 - 1;
  novoNo->pai = pai; // Atualiza o ponteiro pai do novo nó

  // Os dois primeiros loops são feitos para mover os dados dos filhos do nó pai
  // para o novo nó filho Loop para as chaves
  for (int j = 0; j < ORDEM_MAX / 2 - 1; j++) {
    novoNo->chaves[j].chave = y->chaves[j + ORDEM_MAX / 2].chave;
    novoNo->chaves[j].indice = y->chaves[j + ORDEM_MAX / 2].indice;
  }

  // Loop para os ponteiros dos filhos
  if (y->folha == 0) {
    for (int j = 0; j < ORDEM_MAX / 2; j++) {
      novoNo->filhos[j] = y->filhos[j + ORDEM_MAX / 2];
      if (novoNo->filhos[j] != NULL) {
        novoNo->filhos[j]->pai =
            novoNo; // Atualiza o ponteiro pai dos filhos do novo nó
      }
    }
  }

  y->nroChaves = ORDEM_MAX / 2 - 1;

  // Os dois últimos loops são shifts para inserir o novo filho no seu lugar
  // certo no nó pai Neste loop, é para achar o lugar correto para inseri-lo nos
  // ponteiros de filhos
  for (int j = pai->nroChaves; j > i; j--) {
    pai->filhos[j + 1] = pai->filhos[j];
  }

  // Lugar achado, inserindo:
  pai->filhos[i + 1] = novoNo;

  // Neste loop, é para achar o lugar correto para inseri-lo nas chaves
  for (int j = pai->nroChaves - 1; j >= i; j--) {
    pai->chaves[j + 1].chave = pai->chaves[j].chave;
    pai->chaves[j + 1].indice = pai->chaves[j].indice;
  }
  // Lugar achado, inserindo:
  pai->chaves[i].chave = y->chaves[ORDEM_MAX / 2 - 1].chave;
  pai->chaves[i].indice = y->chaves[ORDEM_MAX / 2 - 1].indice;
  pai->nroChaves++;
}

// função para imprimir a b-tree em ordem
void imprimirEmOrdem(no *raiz) {
  if (raiz != NULL) {
    int i;
    for (i = 0; i < raiz->nroChaves; i++) {
      imprimirEmOrdem(raiz->filhos[i]);
      printf("Nó: %d Index %d", raiz->chaves[i].chave, raiz->chaves[i].indice);
      if (raiz->pai != NULL) {
        printf("(Pai: %d) ",
               raiz->pai->chaves[0].chave); // Assumindo que estamos imprimindo
                                            // a primeira chave do pai
      } else {
        printf("(Pai: NULL) ");
      }
      printf("\n");
    }
    imprimirEmOrdem(raiz->filhos[i]);
  }
}

void imprimirPreOrdem(no *raiz) {
  if (raiz != NULL) {
    // Primeiro, imprima o nó atual
    int i;
    for (i = 0; i < raiz->nroChaves; i++) {
      printf("Nó: %d ", raiz->chaves[i].chave);
      if (raiz->pai != NULL) {
        printf("(Pai: %d) ",
               raiz->pai->chaves[0].chave); // Assumindo que estamos imprimindo
                                            // a primeira chave do pai
      } else {
        printf("(Pai: NULL) ");
      }
      printf("\n");
    }
    // Depois, chame recursivamente a função para cada filho
    for (i = 0; i <= raiz->nroChaves; i++) {
      imprimirPreOrdem(raiz->filhos[i]);
    }
  }
}

//busca direto na b-tree para achar o índice
int buscar(no *no, int chave) {
  int i = 0;
  while ((i < no->nroChaves) && (chave > no->chaves[i].chave)) {
    i++;
  }
  if (i < no->nroChaves && chave == no->chaves[i].chave) {
    return no->chaves[i].indice;
  } else if (no->folha == 1) {
    return -1;
  } else {
    buscar(no->filhos[i], chave);
  }
}

int remover(no *no, int chave) {
  int indice = -1;
  int i = 0;
  while ((i < no->nroChaves) && (chave > no->chaves[i].chave)) {
    i++;
  }
  if (i < no->nroChaves && chave == no->chaves[i].chave) {
    indice = i;

  } else if (no->folha == 1) {
    return -1;
  }
  if (indice == -1) {
    remover(no->filhos[i], chave);
  } else {
    if (no->folha == 1) {
      removerFolha(no, chave, indice);
    } else {
      removerNaoFolha(no, chave, indice);
    }
  }
  if (no->nroChaves < ORDEM_MAX / 2 - 1 && no->pai != NULL) {
    i = 0;
    while (i < no->pai->nroChaves && no->pai->filhos[i] != no) {
      if (no->pai->filhos[i] == no) {
        indice = i;
        i--;
      }
      i++;
    }
    if (indice > 0 && no->pai->filhos[indice - 1] != NULL) {
      if (no->pai->filhos[indice - 1]->nroChaves > ORDEM_MAX / 2 - 1) {
        emprestar(no, no->pai->filhos[indice - 1], indice,
                  no->pai->filhos[indice - 1]->nroChaves - 1);
      } else if (indice < no->pai->nroChaves - 1 &&
                 no->pai->filhos[indice + 1] != NULL) {
        if (no->pai->filhos[indice + 1]->nroChaves > ORDEM_MAX / 2 - 1) {
          emprestar(no, no->pai->filhos[indice + 1], indice, 0);
        } else {
          unir(no, no->pai->filhos[indice + 1], indice);
        }
      } else {
        unir(no->pai->filhos[indice - 1], no, indice);
      }
    }
  }
}

void removerFolha(no *no, int chave, int indice) {

  int j = indice;
  while (j < no->nroChaves) {
    no->chaves[j].chave = no->chaves[j + 1].chave;
    no->chaves[j].indice = no->chaves[j + 1].indice;
    j++;
  }
  no->nroChaves--;
}

void removerNaoFolha(no *removido, int chave, int indice) {
  if (removido->filhos[indice]->nroChaves >
      removido->filhos[indice + 1]->nroChaves) {
    no *y = removido->filhos[indice];
    while (y->folha == 0) {
      y = y->filhos[y->nroChaves];
    }
    removido->chaves[indice].chave = y->chaves[y->nroChaves - 1].chave;
    removido->chaves[indice].indice = y->chaves[y->nroChaves - 1].indice;
    remover(removido->filhos[indice], y->chaves[y->nroChaves - 1].chave);
  } else {
    no *y = removido->filhos[indice + 1];
    while (y->folha == 0) {
      y = y->filhos[0];
    }
    removido->chaves[indice].chave = y->chaves[0].chave;
    removido->chaves[indice].indice = y->chaves[0].indice;
    remover(removido->filhos[indice+1], y->chaves[0].chave);
  }
}

void unir(no *x, no *y, int indice) {
  int i = 0;
  x->chaves[x->nroChaves].chave = y->pai->chaves[indice].chave;
  x->chaves[x->nroChaves].indice = y->pai->chaves[indice].indice;
  x->nroChaves++;
  for (int j = indice; j < y->pai->nroChaves - 1; j++) {
    y->pai->chaves[j].chave = y->pai->chaves[j + 1].chave;
    y->pai->chaves[j].indice = y->pai->chaves[j + 1].indice;
  }
  while (i < y->nroChaves) {
    x->chaves[x->nroChaves].chave = y->chaves[i].chave;
    x->chaves[x->nroChaves].indice = x->chaves[i].indice;
    x->nroChaves++;
    i++;
  }
  free(y);
}

void emprestar(no *x, no *y, int indice, int pos) {
  x->nroChaves++;
  x->chaves[x->nroChaves - 1].chave = x->pai->chaves[indice].chave;
  x->chaves[x->nroChaves - 1].indice = x->pai->chaves[indice].indice;
  x->pai->chaves[indice].chave = y->chaves[pos].chave;
  x->pai->chaves[indice].indice = y->chaves[pos].indice;
  if (pos == 0) {
    for (int i = 0; i < y->nroChaves - 1; i++) {
      y->chaves[i].chave = y->chaves[i + 1].chave;
      y->chaves[i].indice = y->chaves[i + 1].indice;
    }
  }
  y->nroChaves--;
}

void processaCarga(btree *arv, char *nomeArquivo)
{

    FILE *arquivo;
    int insere;
    char linha[TAMANHO_LINHA];
    int index = 0;

    arquivo = fopen(nomeArquivo, "rb");

    if (!arquivo){
        printf("Erro ao abrir o arquivo!\n");
        return; 
    } 

     while (fread(linha, TAMANHO_LINHA, 1, arquivo) == 1) {
        // Remove o caractere de nova linha, se existir
        linha[strcspn(linha, "\n")] = '\0';

        // Lê os 5 primeiros números até a vírgula
        sscanf(linha, "%6d", &insere);

        // Avança para o próximo registro no arquivo
        fseek(arquivo, 1, SEEK_CUR); // Avança um byte para pular o caractere de nova linha
        inserir(arv, insere, index);
        index++;
    }

    fclose(arquivo);
}

//função que com o índice da b-tree, faz o cálculo para abrir o arquivo diretamente na linha do registro desejado
void buscarNoArquivo(btree *arv, char *nomeArq, int index, int chave){
    char linha[100]; //assumindo um tamanho que seja suficiente para a linha
    long posicao; //posição no arquivo
    int tamanho = 33; //tamanho da linha
  
    //abrindo o arquivo
    FILE *arquivo = fopen(nomeArq, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    //calculando a posição no arquivo
    posicao = (index) * tamanho;

    //movendo o ponteiro do arquivo para a posição calculado
    fseek(arquivo, posicao, SEEK_SET);

    //lendo a linha desejada
    fgets(linha, sizeof(linha), arquivo);
    printf("Registro encontrado: %s\n", linha);

    //fecha o arquivo
    fclose(arquivo);

}

//função que percorre o arquivo para achar o registro desejado
void buscarRegistro(const char *arquivo, int chave) {
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[100];
    char chaveStr[7];  //string utilizada para fazer a busca no arquivo

    //convertendo a chave int para um valor char
    snprintf(chaveStr, sizeof(chaveStr), "%06d", chave);

    while (fgets(linha, sizeof(linha), arq)) {
        //comparando os primeiros 6 caracteres da linha com a chave
        if (strncmp(linha, chaveStr, 6) == 0) {
            printf("Registro encontrado: %s", linha);
            fclose(arq);
            return;
        }
    }

    printf("Registro com a chave %06d não encontrado.\n", chave);
    fclose(arq);
}

//função que é chamada no menu, nela é possível inserir o valor que quer ser procurado e escolhe o tipo de busca que é desejado
void buscaGeral(btree *arv, char *nomeArq, int valor, int indice){
  int opcao;
  printf("Qual tipo de pesquisa você quer fazer? \n");
  printf("1 - Pesquisar pela B-Tree\n");
  printf("2 - Pesquisar direto no Arquivo\n");
  scanf("%d", &opcao);

  if (opcao == 1){
    buscarNoArquivo(arv, nomeArq, indice, valor);
    
  }

  if (opcao == 2){
    buscarRegistro(nomeArq, valor);
  }
  
}