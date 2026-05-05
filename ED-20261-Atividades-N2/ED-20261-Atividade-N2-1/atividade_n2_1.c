#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minhabib.h"

#define QTD_VALORES 15
#define VALOR_BUSCA_PADRAO 30

static No* criar_no(int valor);
static No* inserir_bst(No* raiz, int valor);
static No* buscar_no(No* raiz, int valor);
static int grau_no(No* no);
static const char* tipo_no(No* raiz_principal, No* no);
static void imprimir_valores_no_nivel(No* raiz, int nivel_alvo);
static void imprimir_subarvore_pre_ordem(No* no);
static int imprimir_ancestrais_rec(No* raiz, int valor);
static void imprimir_arvore_lateral(No* no, int espaco);
static void montar_ancestrais_tabela(No* raiz, int valor, char* buffer, size_t tamanho);
static void salvar_dados_csv_rec(No* raiz_principal, No* no, int nivel, FILE* arquivo);
static void salvar_dados_csv(No* raiz, const char* nome_arquivo);
static void escrever_dot_rec(FILE* arquivo, No* raiz_principal, No* no);
static void salvar_dot(No* raiz_para_desenhar, No* raiz_colorida, const char* nome_arquivo);
static void liberar_arvore(No* raiz);

static No* criar_no(int valor) {
    No* novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria para o no %d.\n", valor);
        exit(1);
    }

    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

static No* inserir_bst(No* raiz, int valor) {
    if (raiz == NULL) {
        return criar_no(valor);
    }

    if (valor < raiz->valor) {
        raiz->esq = inserir_bst(raiz->esq, valor);
    } else if (valor > raiz->valor) {
        raiz->dir = inserir_bst(raiz->dir, valor);
    } else {
        printf("Valor duplicado ignorado: %d\n", valor);
    }

    return raiz;
}

static No* buscar_no(No* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    if (valor < raiz->valor) {
        return buscar_no(raiz->esq, valor);
    }

    return buscar_no(raiz->dir, valor);
}

static int grau_no(No* no) {
    if (no == NULL) {
        return -1;
    }

    int grau = 0;

    if (no->esq != NULL) {
        grau++;
    }

    if (no->dir != NULL) {
        grau++;
    }

    return grau;
}

static const char* tipo_no(No* raiz_principal, No* no) {
    if (no == NULL) {
        return "Inexistente";
    }

    if (no == raiz_principal) {
        return "Raiz";
    }

    if (grau_no(no) == 0) {
        return "Folha";
    }

    return "Interno";
}

void imprimir_nos_internos(No* raiz) {
    if (raiz == NULL) {
        return;
    }

    imprimir_nos_internos(raiz->esq);

    if (raiz->esq != NULL || raiz->dir != NULL) {
        printf("%d ", raiz->valor);
    }

    imprimir_nos_internos(raiz->dir);
}

void imprimir_folhas(No* raiz) {
    if (raiz == NULL) {
        return;
    }

    imprimir_folhas(raiz->esq);

    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("%d ", raiz->valor);
    }

    imprimir_folhas(raiz->dir);
}

static void imprimir_valores_no_nivel(No* raiz, int nivel_alvo) {
    if (raiz == NULL) {
        return;
    }

    if (nivel_alvo == 0) {
        printf("%d ", raiz->valor);
        return;
    }

    imprimir_valores_no_nivel(raiz->esq, nivel_alvo - 1);
    imprimir_valores_no_nivel(raiz->dir, nivel_alvo - 1);
}

void imprimir_niveis(No* raiz, int nivel_atual) {
    int altura = calcular_altura(raiz);

    if (raiz == NULL) {
        printf("Arvore vazia.\n");
        return;
    }

    for (int nivel = nivel_atual; nivel <= altura; nivel++) {
        printf("Nivel %d: ", nivel);
        imprimir_valores_no_nivel(raiz, nivel);
        printf("\n");
    }
}

int calcular_altura(No* no) {
    if (no == NULL) {
        return -1;
    }

    int altura_esq = calcular_altura(no->esq);
    int altura_dir = calcular_altura(no->dir);

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

static int imprimir_ancestrais_rec(No* raiz, int valor) {
    if (raiz == NULL) {
        return 0;
    }

    if (raiz->valor == valor) {
        return 1;
    }

    if (imprimir_ancestrais_rec(raiz->esq, valor) || imprimir_ancestrais_rec(raiz->dir, valor)) {
        printf("%d ", raiz->valor);
        return 1;
    }

    return 0;
}

void imprimir_ancestrais(No* raiz, int valor) {
    if (raiz == NULL) {
        printf("Arvore vazia");
        return;
    }

    if (raiz->valor == valor) {
        printf("O no raiz nao possui ancestrais");
        return;
    }

    if (!imprimir_ancestrais_rec(raiz, valor)) {
        printf("Valor nao encontrado");
    }
}

static void imprimir_subarvore_pre_ordem(No* no) {
    if (no == NULL) {
        return;
    }

    printf("%d ", no->valor);
    imprimir_subarvore_pre_ordem(no->esq);
    imprimir_subarvore_pre_ordem(no->dir);
}

void imprimir_descendentes(No* no) {
    if (no == NULL) {
        printf("No inexistente");
        return;
    }

    if (no->esq == NULL && no->dir == NULL) {
        printf("Nao possui descendentes");
        return;
    }

    imprimir_subarvore_pre_ordem(no->esq);
    imprimir_subarvore_pre_ordem(no->dir);
}

static void imprimir_arvore_lateral(No* no, int espaco) {
    const int incremento = 6;

    if (no == NULL) {
        return;
    }

    espaco += incremento;
    imprimir_arvore_lateral(no->dir, espaco);

    printf("\n");
    for (int i = incremento; i < espaco; i++) {
        printf(" ");
    }
    printf("%d\n", no->valor);

    imprimir_arvore_lateral(no->esq, espaco);
}

void analisar_arvore(No* raiz, int valorBusca) {
    if (raiz == NULL) {
        printf("Arvore vazia. Nao ha diagnostico para processar.\n");
        return;
    }

    printf("============================================================\n");
    printf("ATIVIDADE N2-1 - DIAGNOSTICO DE ARVORE BINARIA DE BUSCA\n");
    printf("============================================================\n\n");

    printf("A. DIAGNOSTICO GERAL - VARREDURA COMPLETA\n");
    printf("Raiz da arvore: %d\n", raiz->valor);

    printf("Nos internos: ");
    imprimir_nos_internos(raiz);
    printf("\n");

    printf("Nos externos / folhas: ");
    imprimir_folhas(raiz);
    printf("\n");

    printf("Exibicao por niveis:\n");
    imprimir_niveis(raiz, 0);

    printf("\nB. DIAGNOSTICO ESPECIFICO - VALOR BUSCADO: %d\n", valorBusca);
    No* encontrado = buscar_no(raiz, valorBusca);

    if (encontrado == NULL) {
        printf("Valor %d nao encontrado na arvore.\n", valorBusca);
        return;
    }

    printf("No encontrado: %d\n", encontrado->valor);
    printf("Grau do no: %d\n", grau_no(encontrado));

    printf("Ancestrais (do no encontrado ate a raiz): ");
    imprimir_ancestrais(raiz, valorBusca);
    printf("\n");

    printf("Descendentes: ");
    imprimir_descendentes(encontrado);
    printf("\n");

    printf("Altura do no: %d\n", calcular_altura(encontrado));
    printf("Profundidade do no: %d\n", calcular_profundidade(raiz, valorBusca, 0));

    printf("\nC. EXTRACAO DE SUB-ARVORE\n");
    printf("Sub-arvore isolada tratando %d como nova raiz:\n", encontrado->valor);
    imprimir_arvore_lateral(encontrado, 0);
    printf("\n");
}

static void montar_ancestrais_tabela(No* raiz, int valor, char* buffer, size_t tamanho) {
    No* atual = raiz;
    char temp[32];

    if (tamanho == 0) {
        return;
    }

    buffer[0] = '\0';

    while (atual != NULL && atual->valor != valor) {
        snprintf(temp, sizeof(temp), "%d", atual->valor);

        if (buffer[0] != '\0') {
            strncat(buffer, " > ", tamanho - strlen(buffer) - 1);
        }

        strncat(buffer, temp, tamanho - strlen(buffer) - 1);

        if (valor < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (atual == NULL || buffer[0] == '\0') {
        snprintf(buffer, tamanho, "-");
    }
}

static void salvar_dados_csv_rec(No* raiz_principal, No* no, int nivel, FILE* arquivo) {
    if (no == NULL) {
        return;
    }

    char ancestrais[160];
    montar_ancestrais_tabela(raiz_principal, no->valor, ancestrais, sizeof(ancestrais));

    fprintf(
        arquivo,
        "%d;%d;%d;%s;%s;%p\n",
        no->valor,
        nivel,
        grau_no(no),
        tipo_no(raiz_principal, no),
        ancestrais,
        (void*) no
    );

    salvar_dados_csv_rec(raiz_principal, no->esq, nivel + 1, arquivo);
    salvar_dados_csv_rec(raiz_principal, no->dir, nivel + 1, arquivo);
}

static void salvar_dados_csv(No* raiz, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Nao foi possivel criar o arquivo %s.\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "valor;nivel;grau;tipo;ancestrais;endereco\n");
    salvar_dados_csv_rec(raiz, raiz, 0, arquivo);
    fclose(arquivo);
}

static void escrever_dot_rec(FILE* arquivo, No* raiz_principal, No* no) {
    if (no == NULL) {
        return;
    }

    const char* fillcolor;
    const char* fontcolor = "black";
    const char* label_tipo = tipo_no(raiz_principal, no);

    if (no == raiz_principal) {
        fillcolor = "#2F80ED";
        fontcolor = "white";
    } else if (grau_no(no) == 0) {
        fillcolor = "#27AE60";
        fontcolor = "white";
    } else {
        fillcolor = "#F2994A";
    }

    fprintf(
        arquivo,
        "    n%d [label=\"%d\\n%s\", fillcolor=\"%s\", fontcolor=\"%s\"];\n",
        no->valor,
        no->valor,
        label_tipo,
        fillcolor,
        fontcolor
    );

    if (no->esq != NULL) {
        fprintf(arquivo, "    n%d -> n%d [label=\"esq\"];\n", no->valor, no->esq->valor);
    }

    if (no->dir != NULL) {
        fprintf(arquivo, "    n%d -> n%d [label=\"dir\"];\n", no->valor, no->dir->valor);
    }

    escrever_dot_rec(arquivo, raiz_principal, no->esq);
    escrever_dot_rec(arquivo, raiz_principal, no->dir);
}

static void salvar_dot(No* raiz_para_desenhar, No* raiz_colorida, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Nao foi possivel criar o arquivo %s.\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "digraph BST {\n");
    fprintf(arquivo, "    graph [rankdir=TB, bgcolor=\"white\", labelloc=\"t\", fontsize=18, fontname=\"Helvetica\"];\n");
    fprintf(arquivo, "    node [shape=circle, style=filled, width=0.8, height=0.8, fontname=\"Helvetica\", fontsize=11, color=\"#333333\"];\n");
    fprintf(arquivo, "    edge [fontname=\"Helvetica\", fontsize=9, color=\"#555555\"];\n");

    escrever_dot_rec(arquivo, raiz_colorida, raiz_para_desenhar);

    fprintf(arquivo, "}\n");
    fclose(arquivo);
}

static void liberar_arvore(No* raiz) {
    if (raiz == NULL) {
        return;
    }

    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

int main(void) {
    int valores[QTD_VALORES] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90};
    int valorBusca = VALOR_BUSCA_PADRAO;
    char entrada[32];
    No* raiz = NULL;

    for (int i = 0; i < QTD_VALORES; i++) {
        raiz = inserir_bst(raiz, valores[i]);
    }

    printf("BST criada com os valores: ");
    for (int i = 0; i < QTD_VALORES; i++) {
        printf("%d ", valores[i]);
    }
    printf("\n");

    printf("Digite o valorBusca ou pressione ENTER para usar %d: ", VALOR_BUSCA_PADRAO);
    if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
        if (sscanf(entrada, "%d", &valorBusca) != 1) {
            valorBusca = VALOR_BUSCA_PADRAO;
        }
    }

    printf("\n");
    analisar_arvore(raiz, valorBusca);

    salvar_dados_csv(raiz, "dados_nos.csv");
    salvar_dot(raiz, raiz, "arvore_diagnostico.dot");

    No* encontrado = buscar_no(raiz, valorBusca);
    if (encontrado != NULL) {
        char nome_dot[80];
        snprintf(nome_dot, sizeof(nome_dot), "subarvore_valor_%d.dot", valorBusca);
        salvar_dot(encontrado, encontrado, nome_dot);
    }

    printf("\nArquivos gerados pelo programa:\n");
    printf("- dados_nos.csv\n");
    printf("- arvore_diagnostico.dot\n");
    if (encontrado != NULL) {
        printf("- subarvore_valor_%d.dot\n", valorBusca);
    }

    liberar_arvore(raiz);
    return 0;
}
