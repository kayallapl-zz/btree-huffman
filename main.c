#include <stdio.h>
#include <stdlib.h>

#include "btree.h"
// #include "huffmantree.c"

int main()
{   
    // int controller = 0;
    // while(controller != -1){
        BTree *arvore = inicializa();
        int tam = 11, i, t = 2;
        char letras[] = {'b', 'h', 'd', 'c', 'e', 'f', 'g', 'a', 'i', '!', 'A'};
        float frequencias[] = {1.1, 1.3, 1.4, 2.5, 3.6, 2.8, 1.9, 4.0, 3.0, 4.5, 6.7};
        qs_simultaneo(letras, frequencias, 0, tam-1);
        for (i = 0; i < tam; i++) arvore = insere(arvore, letras[i], frequencias[i], t);

        // printf("1 - codificar palavra;\n\n");
        // scanf("%d",&controller);
        // if (controller == 1){
            char palavra[500];
            printf("Digite a palavra a ser codificada: ");
            scanf("%[^\n]%*c", palavra);
            char *codigo = codifica_palavra(arvore, palavra);
            printf("Código gerado: %s\n", codigo);

            printf("\nDesencriptando %s...\n", codigo);
            char *palavra_desencriptada = descodifica_palavra(arvore, codigo);
            printf("Palavra desencriptada: %s\n", palavra_desencriptada);

            int letra;
            printf("\nDigite a letra para buscar informações subordinadas: ");
            scanf("%lc", &letra);
            scanf("%*[^\n]"); scanf("%*c");
            busca_subordinadas(arvore, letra);

            printf("\nDigite a letra para remover da árvore: ");
            scanf("%lc", &letra);
            scanf("%*[^\n]"); scanf("%*c");
            arvore = remover(arvore, letra, t);
            imprime(arvore, 0);

            printf("\nDigite a letra para trocar a frequência: ");
            scanf("%lc", &letra);
            scanf("%*[^\n]"); scanf("%*c");
            float freq;
            printf("Digite a frequência: ");
            scanf("%f", &freq);
            scanf("%*[^\n]"); scanf("%*c");
            troca_frequencia(arvore, letra, freq);

            printf("\nDigite a categoria que quer ver (1 - maiúsculas, 2 - minúsculas, 3 - vogais, 4 - consoantes, 5 - outros caracteres): ");
            int cat;
            scanf("%d", &cat);
            scanf("%*[^\n]"); scanf("%*c");
            busca_categoria(arvore, cat);
            printf("\n");
        // }
    // }
    return 1;
}