#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arvhuff{
	float freq;
	int *letra;
	struct arvhuff *esq, *dir;
}AH;

typedef struct lista{
	AH *arvore;
	struct lista *prox;
}LH;

LH *inicializa_huffman();
LH* cria_lista(AH* arvore);
AH* cria_arvore(int l, float f, AH* e, AH* d);
LH* insere_lista(AH* arvore, LH *t);
int tam_array(int *array);
void imprime_lista(LH* lista);
void imprime_aux(AH* a, int andar);
void imprime_arvore(AH* a);
int indice_menor(LH* lista, int menor_diferente);
AH* busca_elemento(LH* lista, int indice);
LH* remove_elemento(LH* lista, int pos);
AH* junta_elementos(LH* lista, AH* dir, AH* esq);
AH* build_huffman(LH* lista);
int busca_letra_huffman(AH* arv, int letra);
int altura_letra_huffman(AH* arv, int h, int letra);
char *codifica_letra_huffman(AH* arvore, int letra);
char *codifica_palavra_huffman(AH* arvore, char *palavra);
char *decripta_palavra(AH *arvore, char *palavra);