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

LH* cria_lista(AH* arvore);
AH* cria_arvore(int l, float f, AH* e, AH* d);
LH* insere_lista(AH* arvore, LH *t);
LH *inicializa();
void imprime_lista(LH* lista);
