#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct btree{
  int nletras, folha, *letra;
  float *frequencia;
  struct btree **filho;
}BTree;

int eh_vogal(int letra);
int eh_maiuscula(int letra);
void qs_simultaneo(char *letras, float *freq, int esq, int dir);
BTree *inicializa();
BTree *cria(int t);
BTree *busca_letra(BTree* arvore, int letra);
int busca_pos(BTree* arvore, int letra);
void busca_subordinadas(BTree* arvore, int letra);
void imprime(BTree *arvore, int andar);
BTree *divisao(BTree *arvore_a, int pos, BTree* arvore_b, int t);
BTree *insere_nao_completo(BTree *arvore, int letra, float freq, int t);
BTree *insere(BTree *arvore, int letra, float freq, int t);
int altura_letra(BTree* arvore, int h, int letra);
char *codifica_letra(BTree* arvore, int letra);
char *codifica_palavra(BTree* arvore, char *palavra);
char *descodifica_palavra(BTree *arvore, char *codigo);
BTree* remover(BTree* arvore, int letra, int t);
BTree* retira(BTree* arvore, int k, int t);
void troca_frequencia(BTree* arvore, int letra, float freq);
void busca_categoria(BTree* arvore, int categoria);
