#ifndef MINHABIB_H
#define MINHABIB_H

typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

// Funcao principal de diagnostico da atividade
void analisar_arvore(No* raiz, int valorBusca);

// Funcoes auxiliares obrigatorias para organizacao
void imprimir_nos_internos(No* raiz);
void imprimir_folhas(No* raiz);
void imprimir_niveis(No* raiz, int nivel_atual);
int calcular_altura(No* no);
int calcular_profundidade(No* raiz, int valor, int profundidade_atual);
void imprimir_ancestrais(No* raiz, int valor);
void imprimir_descendentes(No* no);

#endif
