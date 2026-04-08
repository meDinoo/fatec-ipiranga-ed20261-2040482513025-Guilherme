#include <stdio.h>
#include <stdlib.h>

/* Preparação e criação de enum e Structs */

typedef enum {
    INSERIR = 1,
    APAGAR = 2
} operacao;

struct No{
    char caracter;
    operacao op;
    struct No *prox;
} typedef No;

struct pilha
{
    No *topo;
    int tamanho;
}typedef Pilha;

struct texto{
    char texto[1001];
    int tamanho;
} typedef Texto;

/* Metodos a serem utilizados na pilha */
Pilha *IniciarPilha(){
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

void Pop(Pilha *pilha){
    if(pilha->tamanho == 0){
        printf("Pilha vazia\n");
        return;
    }
    No *aux = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(aux);
    pilha->tamanho--;
}

void Push(Pilha *pilha, char caracter, operacao op){
    No *novo = (No*)malloc(sizeof(No));
    novo->caracter = caracter;
    novo->op = op;
    novo->prox = pilha->topo;
    pilha->topo = novo;
    pilha->tamanho++;
}

operacao Topo(Pilha *pilha){
    if(pilha->tamanho == 0){
        printf("Pilha vazia\n");
        return;
    }
    return pilha->topo->op;
}

/* Metodos do Texto */

Texto *IniciarTexto(){
    Texto *texto = (Texto*)malloc(sizeof(Texto));
    texto->tamanho = 0;
    texto->texto[0] = '\0';
    return texto;
}

void Inserir(Texto *texto, char caracter){
    if(texto->tamanho == 1000){
        printf("Texto cheio\n");
        return;
    }

    texto->texto[texto->tamanho - 1] = caracter;
    texto->texto[texto->tamanho] = '\0';
    texto->tamanho++;
    printf("Caracter inserido: %c\n", caracter);
    printf("Texto atual: %s\n", texto->texto);
    return;
}

/* Metodo de Undo */
void Undo(Pilha *pilha, Texto *texto){
    if(pilha->tamanho == 0){
        printf("Nada para desfazer\n");
        return;
    }

    switch (pilha->topo->op)
    {
    case INSERIR :

    return;
        break;
    
    case APAGAR:

    return
        break;

    default:
        Pop(pilha);
        return;
        break;
    }
    
}

int main()
{
    char string[100];
    
   

}