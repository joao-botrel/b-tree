#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "btree.h"

int main(void)
{
    int ok = -1;
    char nomearquivo[50];
    //cria árvore
    btree *arv = criarBTree();
    printf("Digite o nome do arquivo(com extensão .txt) para ser lido. \n");
    printf("ATENÇÃO! O arquivo deve seguir um padrão rigído para a busca direta no mesmo. Ele deve conter 6 números (chave primária), após isso uma vírgula, depois 14 caracteres (nome do aluno), após isso uma vírgula, depois 8 caracteres(nome do curso), após isso uma vírgula e por último um caracter de número (período atual do aluno).\n");
    scanf("%s", nomearquivo);
    //insere arquivo na arvore
    processaCarga(arv, nomearquivo);
    
    while (ok != 0)
    {
        int escolha;
        printf("Menu\n");
        printf("1 - Criar índice\n");
        printf("2 - Procurar Elementos\n");
        printf("3 - Remover Registro\n");
        printf("4 - Imprimir a árvore\n");                        printf("5 - Sair\n");


        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            criaRegistro(arv, nomearquivo);
            break;
        case 2:
            printf("Digite o valor a ser procurado: ");
            int valor;
            scanf("%d", &valor);
            int indice = buscar(getRaiz(arv), valor);
            if (indice == -1)
            {
                printf("Valor não encontrado");
            }
            else
            {
                buscaGeral(arv, nomearquivo, valor, indice);
            }
            break;
        case 3:
            printf("Digite o valor a ser removido: ");
            int removido;
            scanf("%d", &removido);
            int validacao = remover(getRaiz(arv), removido);
            if (validacao == -1)
            {
                printf("Valor não encontrado");
            }
            else
            {
                printf("Valor removido com sucesso");
            }
            break;
        case 4:
            printf("Você quer a impressão em que ordem?\n");
            int tipo;
            printf("1 - Em ordem\n");
            printf("2 - Pré ordem\n");
            scanf("%d", &tipo);
            if (tipo == 1){
                imprimirEmOrdem(getRaiz(arv));
            } 
            if (tipo == 2){
                imprimirPreOrdem(getRaiz(arv));
            }
        case 5:
            // sair
            ok = 0;
            break;
        default:
            printf("Opção inválida\n");
            break;
        }
    }

    return 0;
}