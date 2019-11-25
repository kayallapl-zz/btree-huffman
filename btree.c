#include "btree.h"

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

void qs_simultaneo(char *letras, float *freq, int esq, int dir){
  int i, j, x, y;
  float f;
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
      f = freq[i]; //realiza troca no array de frequencias
      freq[i] = freq[j];
      freq[j] = f;
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
  novo->frequencia =(float*)malloc(sizeof(float*)*((t*2)-1));
  novo->folha=1;
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

int busca_pos(BTree* arvore, int letra){
  int resp = -1;
  if(!arvore) return resp;
  int i = 0;
  while(i < arvore->nletras && letra > arvore->letra[i]) i++;
  if(i < arvore->nletras && letra == arvore->letra[i]) return i;
  if(arvore->folha) return resp;
  return busca_pos(arvore->filho[i], letra);
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
    if (!eh_vogal(letra)) vogal = "É consoante.";
    else if (eh_vogal(letra) == -1) vogal = "Não é alfabético.";
    char *maiuscula = "É maiúscula.";
    if (!eh_maiuscula(letra)) maiuscula = "É minúscula.";
    else if (eh_maiuscula(letra) == -1) maiuscula = "Não é alfabético.";
    printf("letra: %c \nfrequencia: %f\n%s\n%s\n", arvore->letra[i], arvore->frequencia[i], vogal, maiuscula);
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
  for(i = 0; i < t-1; i++) {
    nova->letra[i] = arvore_b->letra[i+t];
    nova->frequencia[i] = arvore_b->frequencia[i+t];
  }
  if(!arvore_b->folha){
    for(i = 0; i < t; i++){
      nova->filho[i] = arvore_b->filho[i+t];
      arvore_b->filho[i+t] = NULL;
    }
  }
  arvore_b->nletras = t-1;
  for(i = arvore_a->nletras; i >= pos; i--) arvore_a->filho[i+1] = arvore_a->filho[i];
  arvore_a->filho[pos] = nova;
  for(i = arvore_a->nletras; i >= pos; i--) {
    arvore_a->letra[i] = arvore_a->letra[i-1];
    arvore_a->frequencia[i] = arvore_a->frequencia[i-1];
  }
  arvore_a->letra[pos-1] = arvore_b->letra[t-1];
  arvore_a->frequencia[pos-1] = arvore_b->frequencia[t-1];
  arvore_a->nletras++;
  return arvore_a;
}

BTree *insere_nao_completo(BTree *arvore, int letra, float freq, int t){
  int i = arvore->nletras-1;
  if(arvore->folha){ //se é folha, vai procurando até achar o primeiro menor que ele mesmo, de trás pra frente. então insere
    while((i >= 0) && (letra < arvore->letra[i])){
      arvore->letra[i+1] = arvore->letra[i]; //vai passando a folha pro lado
      arvore->frequencia[i+1] = arvore->frequencia[i]; //vai passando a folha pro lado
      i--;
    }
    arvore->letra[i+1] = letra; //insiro na posição seguinte do primeiro menor
    arvore->frequencia[i+1] = freq; 
    arvore->nletras++;
    return arvore;
  }
  while((i >= 0) && (letra < arvore->letra[i])) i--;
  i++; //se não é folha, apenas acha a posição e coloca em i
  if(arvore->filho[i]->nletras == ((2*t)-1)){ //se está cheio, faz a divisão
    arvore = divisao(arvore, (i+1), arvore->filho[i], t);
    if(letra > arvore->letra[i]) i++; //pega a posição pra inserir o filho
  }
  arvore->filho[i] = insere_nao_completo(arvore->filho[i], letra, freq, t);
  return arvore;
}

BTree *insere(BTree *arvore, int letra, float freq, int t){
  if(busca_letra(arvore, letra)) return arvore;
  if(!arvore){ //caso não exista ainda, cria
    arvore=cria(t);
    arvore->letra[0] = letra;
    arvore->frequencia[0] = freq;
    arvore->nletras=1;
    return arvore;
  }
  if(arvore->nletras == (2*t)-1){ //caso esteja cheia, faz a divisão antes de inserir
    BTree *nova = cria(t);
    nova->nletras=0;
    nova->folha = 0;
    nova->filho[0] = arvore;
    nova = divisao(nova,1,arvore,t);
    nova = insere_nao_completo(nova,letra,freq,t);
    return nova;
  }
  arvore = insere_nao_completo(arvore,letra,freq,t); 
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
  char *codigo = malloc(sizeof(char) * tam_codigo);
  strcpy(codigo, "");
  for (i = 0; i<tam_palavra; i++){ //para formar o código
    if (!busca_letra(arvore, palavra[i])){
      strcat(codigo, "?");
    }else{
      strcat(codigo, codifica_letra(arvore, palavra[i]));
    }
  }
  // printf("palavra: %s\n", palavra);
  // printf("palavra encriptada: %s\n", codigo);
  // printf("tamanho final: %d\n", tam_codigo);
  return codigo;
}

char *descodifica_palavra(BTree *arvore, char *codigo){
  int tam_palavra = strlen(codigo), i = 0;
  char *palavra = malloc(sizeof(char) * tam_palavra);
  strcpy(palavra, "");
  while (i < tam_palavra){
    if (codigo[i] == '?'){
      strcat(palavra, "?");
    } else {
      int altura = codigo[i] - '0'; // para parsear o char em int - pq se não ele pega o int correspondente da ascii
      BTree *aux = arvore;
      int j = 0;
      while (j<altura){
        i++;
        int pos = codigo[i] - '0';
        aux = aux->filho[pos];
        j++;
      }
      i++;
      int pos = codigo[i] - '0';
      char *letra = malloc(sizeof(char)); //tem que alocar o novo char
      letra[0] = aux->letra[pos];
      strcat(palavra, letra);
    }
    i++;
  }

  // printf("codigo: %s\n", codigo);
  // printf("codigo desencriptado: %s\n", palavra);
  return palavra;
}

BTree* remover(BTree* arvore, int letra, int t){
  if(!arvore) return arvore;
  int i;
  printf("Removendo %d...\n", letra);
  for(i = 0; i<arvore->nletras && arvore->letra[i] < letra; i++);
  if(i < arvore->nletras && letra == arvore->letra[i]){ //CASOS 1, 2A, 2B e 2C
    if(arvore->folha){ //CASO 1
      printf("\nCASO 1\n");
      int j;
      for(j=i; j<arvore->nletras-1;j++) arvore->letra[j] = arvore->letra[j+1];
      arvore->nletras--;
      return arvore;      
    }
    if(!arvore->folha && arvore->filho[i]->nletras >= t){ //CASO 2A
      printf("\nCASO 2A\n");
      BTree *y = arvore->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[y->nletras];
      int temp = y->letra[y->nletras-1];
      arvore->filho[i] = remover(arvore->filho[i], temp, t); 
      //Eliminar recursivamente K e substitua K por K' em x
      arvore->letra[i] = temp;
      return arvore;
    }
    if(!arvore->folha && arvore->filho[i+1]->nletras >= t){ //CASO 2B
      printf("\nCASO 2B\n");
      BTree *y = arvore->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[0];
      int temp = y->letra[0];
      y = remover(arvore->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
      arvore->letra[i] = temp;
      return arvore;
    }
    if(!arvore->folha && arvore->filho[i+1]->nletras == t-1 && arvore->filho[i]->nletras == t-1){ //CASO 2C
      printf("\nCASO 2C\n");
      BTree *y = arvore->filho[i];
      BTree *z = arvore->filho[i+1];
      y->letra[y->nletras] = letra;          //colocar letra ao final de filho[i]
      int j;
      for(j=0; j<t-1; j++)                //juntar letra[i+1] com letra[i]
        y->letra[t+j] = z->letra[j];
      for(j=0; j<=t; j++)                 //juntar filho[i+1] com filho[i]
        y->filho[t+j] = z->filho[j];
      y->nletras = 2*t-1;
      for(j=i; j < arvore->nletras-1; j++)   //remover letra de arvore
        arvore->letra[j] = arvore->letra[j+1];
      for(j=i+1; j <= arvore->nletras; j++)  //remover ponteiro para filho[i+1]
        arvore->filho[j] = arvore->filho[j+1];
      arvore->filho[j] = NULL; //Campello
      arvore->nletras--;
      arvore->filho[i] = remover(arvore->filho[i], letra, t);
      return arvore;   
    }   
  }

  BTree *y = arvore->filho[i], *z = NULL;
  if (y->nletras == t-1){ //CASOS 3A e 3B
    if((i < arvore->nletras) && (arvore->filho[i+1]->nletras >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nletras\n");
      z = arvore->filho[i+1];
      y->letra[t-1] = arvore->letra[i];   //dar a y a letra i da arvore
      y->nletras++;
      arvore->letra[i] = z->letra[0];     //dar a arvore uma letra de z
      int j;
      for(j=0; j < z->nletras-1; j++)  //ajustar chaves de z
        z->letra[j] = z->letra[j+1];
      //z->letra[j] = 0; Rosseti
      y->filho[y->nletras] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nletras; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nletras--;
      arvore->filho[i] = remover(arvore->filho[i], letra, t);
      return arvore;
    }
    if((i > 0) && (!z) && (arvore->filho[i-1]->nletras >=t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nletras\n");
      z = arvore->filho[i-1];
      int j;
      for(j = y->nletras; j>0; j--)               //encaixar lugar da nova letra
        y->letra[j] = y->letra[j-1];
      for(j = y->nletras+1; j>0; j--)             //encaixar lugar dos filhos da nova letra
        y->filho[j] = y->filho[j-1];
      y->letra[0] = arvore->letra[i-1];              //dar a y a letra i da arvore
      y->nletras++;
      arvore->letra[i-1] = z->letra[z->nletras-1];   //dar a arvore uma letra de z
      y->filho[0] = z->filho[z->nletras];         //enviar ponteiro de z para o novo elemento em y
      z->nletras--;
      arvore->filho[i] = remover(y, letra, t);
      return arvore;
    }
    if(!z){ //CASO 3B
      if(i < arvore->nletras && arvore->filho[i+1]->nletras == t-1){
        printf("\nCASO 3B: i menor que nletras\n");
        z = arvore->filho[i+1];
        y->letra[t-1] = arvore->letra[i];     //pegar letra [i] e coloca ao final de filho[i]
        y->nletras++;
        int j;
        for(j=0; j < t-1; j++){
          y->letra[t+j] = z->letra[j];     //passar filho[i+1] para filho[i]
          y->nletras++;
        }
        if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
          }
        }
        for(j=i; j < arvore->nletras-1; j++){ //limpar referências de i
          arvore->letra[j] = arvore->letra[j+1];
          arvore->filho[j+1] = arvore->filho[j+2];
        }
        arvore->nletras--;
        arvore = remover(arvore, letra, t);
        return arvore;
      }
      if((i > 0) && (arvore->filho[i-1]->nletras == t-1)){ 
        printf("\nCASO 3B: i igual a nletras\n");
        z = arvore->filho[i-1];
        if(i == arvore->nletras)
          z->letra[t-1] = arvore->letra[i-1]; //pegar letra[i] e poe ao final de filho[i-1]
        else
          z->letra[t-1] = arvore->letra[i];   //pegar letra [i] e poe ao final de filho[i-1]
        z->nletras++;
        int j;
        for(j=0; j < t-1; j++){
          z->letra[t+j] = y->letra[j];     //passar filho[i+1] para filho[i]
          z->nletras++;
        }
        if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
          }
        }
        arvore->nletras--;
        arvore->filho[i-1] = z;
        arvore = remover(arvore, letra, t);
        return arvore;
      }
    }
  }  
  arvore->filho[i] = remover(arvore->filho[i], letra, t);
  return arvore;
}

BTree* retira(BTree* arvore, int k, int t){
  if(!arvore || !busca_letra(arvore, k)) return arvore;
  return remover(arvore, k, t);
}

void troca_frequencia(BTree* arvore, int letra, float freq){
  if(!arvore){
    printf("Árvore não existe.\n");
    return;
  }
  BTree *a = busca_letra(arvore, letra);
  if (!a){
    printf("\nLetra não existe");
  } else {
    int pos = busca_pos(arvore, letra);
    a->frequencia[pos] = freq;
    busca_subordinadas(arvore, letra);
  }
}


void busca_categoria(BTree* arvore, int categoria){
  if(arvore){
    int i,j,*vetor, contador=0;
    for(i = 0; i <= arvore->nletras-1; i++){
      busca_categoria(arvore->filho[i], categoria);
      if ((categoria == 1) && (eh_maiuscula(arvore->letra[i])) && (eh_maiuscula(arvore->letra[i])) != -1){ //maiusculas
        printf("%c ", arvore->letra[i]);
      }
      else if ((categoria == 2) && (!eh_maiuscula(arvore->letra[i]))){ //minusculas
        printf("%c ", arvore->letra[i]);
      }
      else if ((categoria == 3) && (eh_vogal(arvore->letra[i])) && (eh_vogal(arvore->letra[i])) != -1)
      { //vogais
        printf("%c ", arvore->letra[i]);
      }
      else if ((categoria == 4) && (!eh_vogal(arvore->letra[i]))){ //consoantes
        printf("%c ", arvore->letra[i]);
      }
      else if ((categoria == 5) && (eh_vogal(arvore->letra[i]) == -1)){ //outros caracteres
        printf("%c ", arvore->letra[i]);
      }
    }
    busca_categoria(arvore->filho[i], categoria);
  }
}
