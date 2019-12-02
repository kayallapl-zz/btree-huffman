#include "huffman.h"

LH *inicializa(){
	return NULL;
}

LH* cria_lista(AH* arvore){
	LH *lista = (LH*)malloc(sizeof(LH));
	lista->arvore = arvore;
	lista->prox = NULL;
	return lista;
}

AH* cria_arvore(int l, float f, AH* e, AH* d){
	AH *arvore = (AH*)malloc(sizeof(AH));
	int *letra = (int*)malloc(sizeof(int));
	letra[0] = l;
	arvore->freq = f;
	arvore->letra = letra;
	arvore->esq = e;
	arvore->dir = d;
	return arvore;
}

LH* insere_lista(AH* arvore, LH *t){
	if(!t) t = cria_lista (arvore);
	else t->prox = insere_lista(arvore, t->prox);
	return t;
}

int tam_array(int *array){
	return sizeof(array)/sizeof(array[0]);
}

void imprime_lista(LH* lista){
	LH* aux = lista;
	while(aux){
		printf("letra: ");
		int size = tam_array(aux->arvore->letra), i;
		for (i = 0; i <= size; i++){
			printf("%c", aux->arvore->letra[i]);
		}
		printf(", frequencia: %f\n", aux->arvore->freq);
		aux = aux->prox;
	}
	printf("\n");
}

int indice_menor(LH* lista, int menor_diferente){
	int menor;
	int i = 0;
	float menorfreq = lista->arvore->freq;
	menor = i;
	if(menor == menor_diferente){
		menor += 1;
		lista = lista->prox;
		menorfreq = lista->arvore->freq;
		i++;
	}
	while(lista->prox){
		i++;
		if(lista->prox->arvore->freq < menorfreq && i != menor_diferente){
			menor = i;
			menorfreq = lista->prox->arvore->freq;
		}
		lista = lista->prox;
	}
	return menor;
}

AH* busca_elemento(LH* lista, int indice){
	int i = 0;
	LH* aux = lista;
	while(i < indice){
		aux = aux->prox;
		i++;
	}
	return aux->arvore;
}

LH* remove_elemento(LH* lista, int pos){
	AH* elem = busca_elemento(lista, pos);
	LH *p = lista, *ant = NULL;
	while((p) && (p->arvore != elem)){
		ant = p;
		p = p->prox;
	}
	if(!p) return lista;
	if(!ant) lista = lista->prox;
	else ant->prox = p->prox;
	free(p);
	return lista;
}

AH* junta_elementos(LH* lista){
	int menor1 = indice_menor(lista, -1);
	int menor2 = indice_menor(lista, menor1);
	AH* dir = busca_elemento(lista, menor1);
	AH* esq = busca_elemento(lista, menor2);
	float nova_freq = dir->freq + esq->freq;
	int *chave1 = dir->letra;
	int *chave2 = esq->letra;
	int size1 = tam_array(chave1);
	int size2 = tam_array(chave2);
	int *nova_chave = (int*)malloc(sizeof(int)*(size1+size2)), i, j, k;
	for (i = 0; i < size1; i++){
		nova_chave[i] = chave1[i];
	}
	for (j = i; j < (size1+size2); j++){
		nova_chave[j] = chave2[j-size1];
	}
	AH* soma = (AH*)malloc(sizeof(AH));
	soma->freq = nova_freq;
	soma->letra = nova_chave;
	soma->dir = dir;
	soma->esq = esq;
	return soma;
}

AH* build_huffman(LH* lista){
	while(lista->prox){
		int menor1 = indice_menor(lista, -1);
		int menor2 = indice_menor(lista, menor1);
		printf("\nANTES DA REMOçAO: menor1: %d, menor2: %d\n", menor1, menor2);
		AH* dir = busca_elemento(lista, menor1);
		AH* esq = busca_elemento(lista, menor2);
		AH* arvore = junta_elementos(lista);
		lista = insere_lista(arvore, lista);
		int menor = indice_menor(lista, -1);
		printf("DEPOS DA REM\n");
		printf("menor1 b: %d\n", menor);
		lista = remove_elemento(lista, menor);
		menor = indice_menor(lista, -1);
		printf("menor2 b: %d\n", menor);
		lista = remove_elemento(lista, menor);

		imprime_lista(lista);
	}
	return lista->arvore;
}

void imprime_aux(AH* a, int andar){
  if(a){
    int j;
    imprime_aux(a->esq, andar + 1);
    for(j = 0; j <= andar; j++) printf("   ");
	int size = tam_array(a->letra), i;
	for (i = 0; i <= size; i++){
		printf("%c", a->letra[i]);
	}
	// printf("%f", a->freq);
    printf("\n");
    imprime_aux(a->dir, andar + 1);
  }
}

void imprime_arvore(AH* a){
  imprime_aux(a, 0);
}

int main(){
	LH *teste = inicializa();
	char letras[5] = {'b', 'h', 'd', 'c', 'e'};
  	float frequencias[5] = {0.2, 0.3, 0.4, 0.5, 0.6};
  	int i;
  	for(i = 0; i<5; i++){
		AH* arvore = cria_arvore(letras[i], frequencias[i], NULL, NULL);
  		teste = insere_lista(arvore, teste);
  	}
  	int menor = indice_menor(teste, -1);
  	int segundo_menor = indice_menor(teste, menor);

  	imprime_lista(teste);
  	AH* arv = build_huffman(teste);
  	// imprime_lista(teste);
  	imprime_arvore(arv);
  	printf("\n\n");



  	// AH* testezin = junta_elementos(teste);
}
