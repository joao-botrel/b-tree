#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int ok = 0;
    // recebe o arquivo
    btree *arv = criarBTree();
    // insere arquivo na arvore
    while (ok != 0)
    {
        int escolha;
        printf("Menu\n");
        printf("1 - Criar índice\n");
        printf("2 - Procurar Elementos\n");
        printf("3 - Remover Registro\n");
        printf("4 - Sair\n");

        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            // criar índice
            break;
        case 2:
            printf("Digite o valor a ser procurado: ");
            int valor;
            scanf("%d", &valor);
            int indice = buscar(getRaiz(arv), valor);
            if (indice != -1)
            {
                printf("Valor não encontrado");
            }
            else
            {
                // buscar no arquivo
                // impimir o registro
            }
            break;
        case 3:
            printf("Digite o valor a ser removido: ");
            int valor;
            scanf("%d", &valor);
            int validacao = remover(arv, getRaiz(arv), valor);
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