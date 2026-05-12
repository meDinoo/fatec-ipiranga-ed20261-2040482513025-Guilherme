#include <stdio.h>
#include <stdlib.h>

typedef struct No
{
    int valor;
    int altura;
    struct No *esq;
    struct No *dir;

} No;

int calcularAltura(No *n){
    if(n == NULL) return -1;

    int altEsq, altDir;
    if(n->esq == NULL) altEsq = -1;
    else altEsq = 1 + calcularAltura(n->esq);
    if(n->dir == NULL) altDir = -1;
    else altDir = 1 + calcularAltura(n->dir);

    return altEsq > altDir ? n->altura = altEsq : n->altura = altDir;
}

int obterFb(No *n){

    if(n == NULL) return;
    int altEsq = calcularAltura(n->esq);
    int altDir = calcularAltura(n->dir);

    return altEsq - altDir;
}

int main(){
    No *raiz = NULL;
   
    //primeira inserção
    No *novo = malloc(sizeof *No);
    novo->valor = 20;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;
    raiz = novo;

   novo = NULL;
   novo->valor = 20;
   novo->esq = NULL;  
   novo->dir = NULL;
   novo->altura = 0;

   raiz->esq = novo;

   novo = NULL;
    novo->valor = 30;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;

    raiz->esq->esq = novo;

    printf("Altura do nó raiz: %d\n", raiz->altura);
    printf("Fator de balanceamento do nó raiz: %d\n", obterFb(raiz));

    return 0;

}