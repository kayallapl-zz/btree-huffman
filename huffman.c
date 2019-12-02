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
	int i = 0;
	while(array[i] && ((array[i] >= 'A' && array[i] <= 'Z') || (array[i] >= 'a' && array[i] <= 'z'))){
		i++; //testa se é uma letra, pq o array foi alocado dinamicamente, então ele acaba não dando certo
	}
	return i;
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

void imprime_aux(AH* a, int andar){
  if(a){
    int j;
    imprime_aux(a->esq, andar + 1);
    for(j = 0; j <= andar; j++) printf("       ");
	int size = tam_array(a->letra), i;
	for (i = 0; i < size; i++){
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

AH* junta_elementos(LH* lista, AH* dir, AH* esq){
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
		AH* dir = busca_elemento(lista, menor1);
		AH* esq = busca_elemento(lista, menor2);
		AH* arvore = junta_elementos(lista, dir, esq);
		lista = insere_lista(arvore, lista);
		int menor = indice_menor(lista, -1);
		lista = remove_elemento(lista, menor);
		menor = indice_menor(lista, -1);
		lista = remove_elemento(lista, menor);
	}
	return lista->arvore;
}

int busca_letra(AH* arv, int letra){
	if (!arv) return 0;
    if(arv->esq == NULL && arv->dir == NULL){
    	if (arv->letra[0] == letra) return 1;
    	else return 0;
    }
    else
        return busca_letra(arv->esq, letra) + busca_letra(arv->dir, letra); 
}

int altura_letra(AH* arv, int h, int letra){
	int altura = h;
    if(arv->esq == NULL && arv->dir == NULL){
    	if (arv->letra[0] == letra) return altura;
    	else return 0;
    }
    else
    	altura++;
    	return altura_letra(arv->esq, altura, letra) + altura_letra(arv->dir, altura, letra); 
}

char *codifica_letra(AH* arvore, int letra){
	if (busca_letra(arvore, letra)){
		int altura = altura_letra(arvore, 0, letra), i;
		char *codigo = malloc((sizeof(char) * altura));
		strcpy(codigo, "");
		AH *aux = arvore;
		for (i=0; i < altura; i++){
			if(busca_letra(aux->esq, letra)) {
		  		strcat(codigo, "0");
				aux = aux->esq;
			} else if(busca_letra(aux->dir, letra)) {
		  		strcat(codigo, "1");
				aux = aux->dir;
			}
		}
		return codigo;
 	}
 	return NULL;
}

char *codifica_palavra(AH* arvore, char *palavra){
  int tam_palavra = strlen(palavra), tam_codigo = 0, i, tam_temp;
  for (i = 0; i<tam_palavra; i++){ 
    if (!busca_letra(arvore, palavra[i])){
      tam_codigo += 1;
    }else{
      tam_temp = altura_letra(arvore, 0, palavra[i]);
      tam_codigo += tam_temp;
    }
  }
  char *codigo = malloc(sizeof(char) * tam_codigo);
  strcpy(codigo, "");
  for (i = 0; i<tam_palavra; i++){ //para formar o código
    if (!busca_letra(arvore, palavra[i])){
      strcat(codigo, "?");
    }else{
    	int tam = altura_letra(arvore, 0, palavra[i]);
      	strcat(codigo, codifica_letra(arvore, palavra[i]));
    }
  }
  printf("palavra: %s\n", palavra);
  printf("palavra encriptada: %s\n", codigo);
  return codigo;
}

char *decripta_palavra(AH *arvore, char *palavra){
	int tam_palavra = strlen(palavra);
	char *ans = malloc(sizeof(char)* tam_palavra);
	strcpy(ans, "");
	AH* aux = arvore;
	for (int i = 0; i<tam_palavra; i++){
		if(palavra[i] == '0') aux = aux->esq;
		else if(palavra[i] == '1') aux = aux->dir;
		else{
			strcat(ans, "?");
			aux = arvore;
		} 
		if (aux->esq == NULL && aux->dir == NULL){
			char *letra = malloc(sizeof(char));
			letra[0] = aux->letra[0];
			strcat(ans,letra);
			aux = arvore;
		}
	}
	printf("palavra: %s\n", palavra);
	printf("palavra decriptada: %s\n", ans);
	return ans;
}

int main(){
	LH *teste = inicializa();
	char letras[8] = {'b', 'h', 'd', 'c', 'e', 'j', 'k', 'l'};
  	float frequencias[8] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.4, 0.8, 0.9};
  	int i;
  	for(i = 0; i<8; i++){
		AH* arvore = cria_arvore(letras[i], frequencias[i], NULL, NULL);
  		teste = insere_lista(arvore, teste);
  	}
  	AH* arv = build_huffman(teste);
  	imprime_arvore(arv);
  	printf("\n\n");
    char *codigo = codifica_palavra(arv, "lhlaksdd");
    char *codigo2 = decripta_palavra(arv, "10010010?000?111111");
}