#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"
#include "btree.h"

int main(){
	int tam = 52, i, j, t = 2;
	char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',	'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    float frequencias[] = {11.602, 4.701, 3.511, 2.670, 2.001, 3.779, 1.950, 7.232, 6.286, 0.631, 0.690, 2.705, 4.374,
    	2.365, 6.264, 2.545, 0.173, 1.653, 7.755, 16.671, 1.487, 0.619, 6.661, 0.005, 1.620, 0.050, 11.602, 4.701, 
    	3.511, 2.670, 2.001, 3.779, 1.950, 7.232, 6.286, 0.631, 0.690, 2.705, 4.374, 2.365, 6.264, 2.545, 0.173, 
    	1.653, 7.755, 16.671, 1.487, 0.619, 6.661, 0.005, 1.620, 0.050};
    qs_simultaneo(letras, frequencias, 0, tam-1);

    //cria arv b
    BTree *arvore_b = inicializa();
    for (i = 0; i < tam; i++) arvore_b = insere(arvore_b, letras[i], frequencias[i], t);
    //cria arv huff
    LH *arvore_huff = inicializa_huffman();
    for(j = 0; j < tam; j++){
        AH* arvore_unica = cria_arvore(letras[j], frequencias[j], NULL, NULL);
        arvore_huff = insere_lista(arvore_unica, arvore_huff);
    }
    AH* arvore_huffman = build_huffman(arvore_huff);

    int controller = 0;
    while(controller != -1){
    	printf("1 - codificar palavra;\n2 - decodificar palavra\n3 - buscar informação subordinada\n4 - alterar frequencia de uma letra\n5 - remover uma letra\n-1 sair\n");
    	scanf("%d", &controller);
    	if(controller == 1){
    		printf("Digite a palavra a ser codificada: \n");
    		char palavra[200];
    		scanf("%*[^\n]"); scanf("%*c"); //pega o lixo
    		scanf("%[^\n]%*c", palavra);
    		char *codigo_b = codifica_palavra(arvore_b, palavra);
    		char *codigo_huff = codifica_palavra_huffman(arvore_huffman, palavra);
            printf("Código gerado (árvore B): %s\n", codigo_b);
            printf("Código gerado (árvore de Huffman): %s\n", codigo_huff);
            printf("\n");
    	}
    	if (controller == 2){
    		printf("Digite o código a ser decodificado: \n");
    		char codigo[500];
    		scanf("%*[^\n]"); scanf("%*c"); //pega o lixo
    		scanf("%[^\n]%*c", codigo);
    		int controller_arv = 0;
    		printf("Em qual árvore deseja decodificar? 1 - Arvore B, 2 - Huffman\n");
    		scanf("%d", &controller_arv);
    		if (controller_arv == 1){
    			char *palavra_decodificada = descodifica_palavra(arvore_b, codigo);
	            printf("Palavra decodificada na arvore B: %s\n", palavra_decodificada);
    		} else if (controller_arv == 2){
    			char *palavra_decodificada = decripta_palavra(arvore_huffman, codigo);
	            printf("Palavra decodificada na arvore Huffman: %s\n", palavra_decodificada);
    		} else {
    			printf("Digite zero (0) ou um (1): \n");
    		}
            printf("\n");
    	}
    	if (controller == 3){
    		printf("Digite a letra: \n");
    		char letra;
    		scanf(" %c", &letra);
	        busca_subordinadas(arvore_b, letra);
            printf("\n");
    	}
    	if (controller == 4){
    		printf("Digite a letra: \n");
    		char letra;
    		scanf(" %c", &letra);
    		printf("Digite a nova frequencia: \n");
    		float frequencia;
    		scanf(" %f", &frequencia);
    		int k = 0;
    		for (k = 0; k < tam; k++){
    			if (letras[k] == letra){
    				frequencias[k] = frequencia;
    				printf("%c - %f\n", letras[k], frequencias[k]);
    			}
    		}

    		// free(arvore_b);
    		// free(arvore_huff);
    		// free(arvore_huffman);

		    //cria arv b
		    arvore_b = inicializa();
		    for (i = 0; i < tam; i++) arvore_b = insere(arvore_b, letras[i], frequencias[i], t);
		    //cria arv huff
		    arvore_huff = inicializa_huffman();
		    for(j = 0; j < tam; j++){
		        AH* arvore_unica = cria_arvore(letras[j], frequencias[j], NULL, NULL);
		        arvore_huff = insere_lista(arvore_unica, arvore_huff);
		    }
		    arvore_huffman = build_huffman(arvore_huff);

            printf("\n");
    	}
    	if (controller == 5){
    		printf("Digite a letra: \n");
    		char letra;
    		scanf(" %c", &letra);
    		remover(arvore_b, letra, t);
    	}
    }

    return 1;
}