#ifndef DAG_H
#define DAG_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

// Funções base do dígrafo
GrafoLista* criar_digrafo(int n);
void inserir_aresta_direcionada(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);

// Funções da Prática 03
int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
int eh_dag(GrafoLista *g);

#endif