#include <stdio.h>
#include <stdlib.h>

typedef struct arvhuff
{
    int freq, bin;
    char letra;
    struct arvhuff *esq, *dir;
} TARVHUFF;

TARVHUFF *cria_huff(char l, int f, TARVHUFF *e, TARVHUFF *d)
{
    TARVHUFF *novo = (TARVHUFF *)malloc(sizeof(TARVHUFF));
    novo->freq = f;
    novo->letra = l;
    novo->esq = e;
    novo->dir = d;
    return novo;
}

TARVHUFF *insere_huff(char e, int f, TARVHUFF *t)
{
    if (!t)
        t = cria_huff(e, f, NULL, NULL);
    else if (f <= t->freq)
        t->esq = insere_huff(e, f, t->esq);
    else if (f > t->freq)
        t->dir = insere_huff(e, f, t->dir);
    return t;
}

void libera_huff(TARVHUFF *t)
{
    if (t)
    {
        libera_huff(t->esq);
        libera_huff(t->dir);
        free(t);
    }
}

TARVHUFF *busca_huff(int e, TARVHUFF *t)
{
    if ((!t) || ((int)t->letra == e))
        return t;
    TARVHUFF *resp = busca_huff(e, t->esq);
    if (resp)
        return resp;
    return busca_huff(e, t->dir);
}

void imprime_aux(TARVHUFF *a, int andar)
{
    if (a)
    {
        int j;
        imprime_aux(a->esq, andar + 1);
        for (j = 0; j <= andar; j++)
            printf("   ");
        printf("%c\n", a->letra);
        imprime_aux(a->dir, andar + 1);
    }
}

void imprime_huff(TARVHUFF *a)
{
    imprime_aux(a, 1);
}

TARVHUFF *inicializa()
{
    return NULL;
}

int main()
{
    TARVHUFF *teste = inicializa();
    char letras[9] = {'b', 'h', 'd', 'c', 'e', 'f', 'g', 'a', 'i'};
    int frequencias[9] = {23, 11, 14, 25, 24, 17, 36, 42, 55};
    int i;
    for (i = 0; i < 9; i++)
    {
        printf("%d\n", i);
        teste = insere_huff(letras[i], frequencias[i], teste);
    }

    imprime_huff(teste);
}