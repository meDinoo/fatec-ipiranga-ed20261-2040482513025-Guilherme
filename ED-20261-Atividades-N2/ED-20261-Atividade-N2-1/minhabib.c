#include <stdio.h>
#include "minhabib.h"

void analisar_arvore(No* raiz, int valorBusca) {
    No* encontrado;
    int altura_arvore;
    int altura_busca;
    int profundidade;
    int grau;
    int nivel;

    printf("============================================================\n");
    printf("ATIVIDADE N2-1 - DIAGNOSTICO DE ARVORE BINARIA DE BUSCA\n");
    printf("============================================================\n\n");

    if (raiz == NULL) {
        printf("A arvore esta vazia.\n");
        return;
    }

    printf("A. DIAGNOSTICO GERAL - VARREDURA COMPLETA\n");
    printf("------------------------------------------------------------\n");
    printf("Identificacao da raiz: %d\n", raiz->valor);

    printf("Nos internos: ");
    imprimir_nos_internos(raiz);
    printf("\n");

    printf("Nos externos / folhas: ");
    imprimir_folhas(raiz);
    printf("\n");

    altura_arvore = calcular_altura(raiz);
    printf("Exibicao por niveis:\n");
    for (nivel = 0; nivel <= altura_arvore; nivel++) {
        printf("Nivel %d: ", nivel);
        imprimir_niveis(raiz, nivel);
        printf("\n");
    }

    printf("\nB. DIAGNOSTICO ESPECIFICO - VALOR BUSCADO\n");
    printf("------------------------------------------------------------\n");
    printf("Valor buscado: %d\n", valorBusca);

    encontrado = raiz;
    while (encontrado != NULL && encontrado->valor != valorBusca) {
        if (valorBusca < encontrado->valor) {
            encontrado = encontrado->esq;
        } else {
            encontrado = encontrado->dir;
        }
    }

    if (encontrado == NULL) {
        printf("Resultado da busca: valor nao encontrado.\n");
        return;
    }

    grau = 0;
    if (encontrado->esq != NULL) {
        grau++;
    }
    if (encontrado->dir != NULL) {
        grau++;
    }

    altura_busca = calcular_altura(encontrado);
    profundidade = calcular_profundidade(raiz, valorBusca, 0);

    printf("No encontrado: %d\n", encontrado->valor);
    printf("Grau do no: %d\n", grau);

    printf("Ancestrais, do no encontrado ate a raiz: ");
    imprimir_ancestrais(raiz, valorBusca);
    printf("\n");

    printf("Descendentes: ");
    if (grau == 0) {
        printf("sem descendentes");
    } else {
        imprimir_descendentes(encontrado);
    }
    printf("\n");

    printf("Altura do no buscado: %d\n", altura_busca);
    printf("Profundidade do no buscado: %d\n", profundidade);

    printf("\nC. EXTRACAO DE SUB-ARVORE\n");
    printf("------------------------------------------------------------\n");
    printf("Nova raiz da sub-arvore: %d\n", encontrado->valor);
    for (nivel = 0; nivel <= altura_busca; nivel++) {
        printf("Nivel %d da sub-arvore: ", nivel);
        imprimir_niveis(encontrado, nivel);
        printf("\n");
    }
}

void imprimir_nos_internos(No* raiz) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->esq != NULL || raiz->dir != NULL) {
        printf("%d ", raiz->valor);
    }

    imprimir_nos_internos(raiz->esq);
    imprimir_nos_internos(raiz->dir);
}

void imprimir_folhas(No* raiz) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("%d ", raiz->valor);
        return;
    }

    imprimir_folhas(raiz->esq);
    imprimir_folhas(raiz->dir);
}

void imprimir_niveis(No* raiz, int nivel_atual) {
    if (raiz == NULL) {
        return;
    }

    if (nivel_atual == 0) {
        printf("%d ", raiz->valor);
        return;
    }

    imprimir_niveis(raiz->esq, nivel_atual - 1);
    imprimir_niveis(raiz->dir, nivel_atual - 1);
}

int calcular_altura(No* no) {
    int altura_esq;
    int altura_dir;

    if (no == NULL) {
        return -1;
    }

    altura_esq = calcular_altura(no->esq);
    altura_dir = calcular_altura(no->dir);

    if (altura_esq > altura_dir) {
        return altura_esq + 1;
    }

    return altura_dir + 1;
}

int calcular_profundidade(No* raiz, int valor, int profundidade_atual) {
    if (raiz == NULL) {
        return -1;
    }

    if (raiz->valor == valor) {
        return profundidade_atual;
    }

    if (valor < raiz->valor) {
        return calcular_profundidade(raiz->esq, valor, profundidade_atual + 1);
    }

    return calcular_profundidade(raiz->dir, valor, profundidade_atual + 1);
}

void imprimir_ancestrais(No* raiz, int valor) {
    No* atual;
    int caminho[100];
    int quantidade;
    int i;

    atual = raiz;
    quantidade = 0;

    while (atual != NULL && atual->valor != valor) {
        caminho[quantidade] = atual->valor;
        quantidade++;

        if (valor < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (atual == NULL) {
        printf("valor nao encontrado");
        return;
    }

    if (quantidade == 0) {
        printf("sem ancestrais");
        return;
    }

    for (i = quantidade - 1; i >= 0; i--) {
        printf("%d ", caminho[i]);
    }
}

void imprimir_descendentes(No* no) {
    if (no == NULL) {
        return;
    }

    if (no->esq != NULL) {
        printf("%d ", no->esq->valor);
        imprimir_descendentes(no->esq);
    }

    if (no->dir != NULL) {
        printf("%d ", no->dir->valor);
        imprimir_descendentes(no->dir);
    }
}
