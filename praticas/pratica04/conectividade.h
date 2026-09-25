#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

// Funções básicas do grafo (Não-direcionado)
GrafoLista* criar_grafo(int n);
void inserir_aresta(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);

// Funções de Conectividade (Tarjan)
void encontrar_articulacoes_e_pontes(GrafoLista *g);

#endif