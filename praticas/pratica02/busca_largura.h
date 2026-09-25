#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

// Estruturas Básicas do Grafo (necessárias para as buscas)
typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

// Fila (FIFO) para BFS
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

// Funções base do grafo
GrafoLista* criar_grafo(int n);
void inserir_aresta(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);

// Funções da Prática
void bfs(GrafoLista *g, int origem, int *dist, int *pred);
int eh_bipartido(GrafoLista *g);

#endif