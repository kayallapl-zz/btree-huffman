#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct btree{
  int nletras, folha, *letra, frequencia, vogal, maiuscula;
  struct btree **filho;
}BTree;


int eh_vogal(int letra){
  char vogais[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
  int k;
  for (k = 0; k < 10; k++)
    if (letra == vogais[k]) return 1; //caso seja vogal
  if ((letra >= 'a' && letra <= 'z') || (letra >= 'A' && letra <= 'Z')) return 0; //caso não seja vogal
  return -1; //caso não seja alfabético
}


int eh_maiuscula(int letra){
  if (letra >= 'A' && letra <= 'Z') return 1; //caso seja maiuscula
  if (letra >= 'a' && letra <= 'z') return 0; //caso não seja maiuscula
  return -1; //caso não seja alfabético
}

void qs_simultaneo(char *letras, int *freq, int esq, int dir){
  int i, j, x, y;
  i = esq;
  j = dir;
  x = letras[(esq + dir) / 2];
  while (i <= j){
    while (letras[i] < x && i < dir) i++;
    while (letras[j] > x && j > esq) j--;
    if (i <= j){
      y = letras[i]; //realiza troca no array de letras
      letras[i] = letras[j];
      letras[j] = y;
      y = freq[i]; //realiza troca no array de frequencias
      freq[i] = freq[j];
      freq[j] = y;
      i++;
      j--;
    }
  }
  if (j > esq) qs_simultaneo(letras, freq, esq, j);
  if (i < dir) qs_simultaneo(letras, freq, i, dir);
}

BTree *inicializa(){
  return NULL;
}


BTree *cria(int t){
  BTree* novo = (BTree*)malloc(sizeof(BTree));
  novo->nletras = 0;
  novo->letra =(int*)malloc(sizeof(int*)*((t*2)-1));
  novo->folha=1;
  novo->vogal=-1;
  novo->maiuscula=-1;
  novo->filho = (BTree**)malloc(sizeof(BTree*)*t*2);
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


BTree *busca_letra(BTree* arvore, int letra){
  BTree *resp = NULL;
  if(!arvore) return resp;
  int i = 0;
  while(i < arvore->nletras && letra > arvore->letra[i]) i++;
  if(i < arvore->nletras && letra == arvore->letra[i]) return arvore;
  if(arvore->folha) return resp;
  return busca_letra(arvore->filho[i], letra);
}


void busca_subordinadas(BTree* arvore, int letra){
  if(!arvore){
    printf("Árvore não existe.\n");
    return;
  }
  int i = 0;
  while(i < arvore->nletras && letra > arvore->letra[i]) i++;
  if(i < arvore->nletras && letra == arvore->letra[i]){
    char *vogal = "É vogal.";
    if (!arvore->vogal) vogal = "É consoante.";
    else if (arvore->vogal == -1) vogal = "Não é alfabético.";
    char *maiuscula = "É maiúscula.";
    if (!arvore->maiuscula) maiuscula = "É minúscula.";
    else if (arvore->maiuscula == -1) maiuscula = "Não é alfabético.";
    printf("letra: %c \nfrequencia: %d\n%s\n%s\n", arvore->letra[0], arvore->frequencia, vogal, maiuscula);
    return;
  }
  if (!arvore->folha){
    busca_subordinadas(arvore->filho[i], letra);
  } else {
    printf("Letra não encontrada.\n");
  }
}


void imprime(BTree *arvore, int andar){
  if(arvore){
    int i,j;
    for(i = 0; i <= arvore->nletras-1; i++){
      imprime(arvore->filho[i], andar+1);
      for(j = 0; j <= andar; j++) printf("   ");
      printf("%c\n", arvore->letra[i]);
    }
    imprime(arvore->filho[i], andar+1);
  }
}


BTree *divisao(BTree *arvore_a, int pos, BTree* arvore_b, int t){
  BTree *nova = cria(t);
  nova->nletras = t-1;
  nova->folha = arvore_b->folha;
  int i;
  for(i = 0; i < t-1; i++) nova->letra[i] = arvore_b->letra[i+t];
  if(!arvore_b->folha){
    for(i = 0; i < t; i++){
      nova->filho[i] = arvore_b->filho[i+t];
      arvore_b->filho[i+t] = NULL;
    }
  }
  arvore_b->nletras = t-1;
  for(i = arvore_a->nletras; i >= pos; i--) arvore_a->filho[i+1] = arvore_a->filho[i];
  arvore_a->filho[pos] = nova;
  for(i = arvore_a->nletras; i >= pos; i--) arvore_a->letra[i] = arvore_a->letra[i-1];
  arvore_a->letra[pos-1] = arvore_b->letra[t-1];
  arvore_a->nletras++;
  return arvore_a;
}


BTree *insere_nao_completo(BTree *arvore, int letra, int t){
  int i = arvore->nletras-1;
  if(arvore->folha){ //se é folha, vai procurando até achar o primeiro menor que ele mesmo, de trás pra frente. então insere
    while((i >= 0) && (letra < arvore->letra[i])){
      arvore->letra[i+1] = arvore->letra[i]; //vai passando a folha pro lado
      i--;
    }
    arvore->letra[i+1] = letra; //insiro na posição seguinte do primeiro menor
    arvore->nletras++;
    return arvore;
  }
  while((i >= 0) && (letra < arvore->letra[i])) i--;
  i++; //se não é folha, apenas acha a posição e coloca em i
  if(arvore->filho[i]->nletras == ((2*t)-1)){ //se está cheio, faz a divisão
    arvore = divisao(arvore, (i+1), arvore->filho[i], t);
    if(letra > arvore->letra[i]) i++; //pega a posição pra inserir o filho
  }
  arvore->filho[i] = insere_nao_completo(arvore->filho[i], letra, t);
  return arvore;
}


BTree *insere(BTree *arvore, int letra, int freq, int t){
  if(busca_letra(arvore, letra)) return arvore;
  if(!arvore){ //caso não exista ainda, cria
    arvore=cria(t);
    arvore->letra[0] = letra;
    arvore->nletras=1;
    arvore->frequencia=freq;
    arvore->vogal=eh_vogal(letra);
    arvore->maiuscula=eh_maiuscula(letra);
    return arvore;
  }
  if(arvore->nletras == (2*t)-1){ //caso esteja cheia, faz a divisão antes de inserir
    BTree *nova = cria(t);
    nova->nletras=0;
    nova->folha = 0;
    nova->filho[0] = arvore;
    nova = divisao(nova,1,arvore,t);
    nova = insere_nao_completo(nova,letra,t);
    return nova;
  }
  arvore = insere_nao_completo(arvore,letra,t); 
  return arvore;
}


int altura_letra(BTree* arvore, int h, int letra){
  int i = 0;
  while(i < arvore->nletras && letra > arvore->letra[i]) i++;
  if(i < arvore->nletras && letra == arvore->letra[i]) return 0;
  h = 1 + altura_letra(arvore->filho[i], h, letra);
  return h;
}

char *codifica_letra(BTree* arvore, int letra){
  int altura = altura_letra(arvore, 0, letra), i;
  char *codigo = malloc(sizeof(char) * altura+1);
  codigo[0] = altura + '0'; //o primeiro caractere será a altura // o + '0' adiciona o dígito como caractere e não como int 
  BTree *aux = arvore;
  for (i = 1; i <= altura+1; i++){
    int contador = 0;
    while (contador < aux->nletras && letra > aux->letra[contador]) contador++;
    codigo[i] = contador + '0';
    aux = aux->filho[contador];
  }
  return codigo;
}

char *codifica_palavra(BTree* arvore, char *palavra){
  int tam_palavra = strlen(palavra), tam_codigo = 0, i;
  for (i = 0; i<tam_palavra; i++){ //para saber o tamanho final do código, criando assim uma array com o tamanho exato, sem necessidade de faltar ou sobrar espaço
    if (!busca_letra(arvore, palavra[i])){
      tam_codigo += 1;
    }else{
      int tam = strlen(codifica_letra(arvore, palavra[i]));
      tam_codigo += tam;
    }
  }
  char codigo[tam_codigo];
  strcpy(codigo, "");
  for (i = 0; i<tam_palavra; i++){ //para formar o código
    if (!busca_letra(arvore, palavra[i])){
      strcat(codigo, "?");
    }else{
      strcat(codigo, codifica_letra(arvore, palavra[i]));
    }
  }
  printf("palavra: %s\n", palavra);
  printf("palavra encriptada: %s\n", codigo);
  printf("tamanho final: %d\n", tam_codigo);
}


int main(){
  BTree *arvore = inicializa();
  char letras[9] = {'b', 'h', 'd', 'c', 'e', 'f', 'g', 'a', 'i'};
  int frequencias[9] = {1, 1, 1, 2, 3, 2, 1, 4, 3};
  qs_simultaneo(letras, frequencias, 0, 8);
  int i;
  int t = 2;
  for (i=0; i<9; i++){
    arvore = insere(arvore, letras[i], frequencias[i], t);
  }
  // busca_subordinadas(arvore, 'a');
  // int altura = altura_letra(arvore, 0, 'a');
  // char *codigo = codifica_letra(arvore, 'a');
  // printf("%s\n", codigo);
  // printf("altura: %d\n", altura);
  // printf("tam: %ld\n", strlen(codigo));

  char *codigo = codifica_palavra(arvore, "aaahslw");
  return 1;
}