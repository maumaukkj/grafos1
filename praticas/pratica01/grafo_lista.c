#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    if (!g) return NULL;

    g->n = n;
    g->adj = (No**) malloc(n * sizeof(No*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

static void aux_inserir_aresta(GrafoLista *g, int u, int v) {
    No *novo = (No*) malloc(sizeof(No));
    novo->destino = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (!g || u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    if (!sao_adjacentes_lista(g, u, v)) {
        aux_inserir_aresta(g, u, v);
        aux_inserir_aresta(g, v, u); // Grafo não direcionado
    }
}

static void aux_remover_aresta(GrafoLista *g, int u, int v) {
    No *atual = g->adj[u];
    No *ant = NULL;

    while (atual != NULL && atual->destino != v) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual != NULL) {
        if (ant == NULL) {
            g->adj[u] = atual->prox;
        } else {
            ant->prox = atual->prox;
        }
        free(atual);
    }
}

void remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (!g || u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    aux_remover_aresta(g, u, v);
    aux_remover_aresta(g, v, u);
}

int grau_lista(GrafoLista *g, int u) {
    if (!g || u < 0 || u >= g->n) return 0;
    int grau = 0;
    No *atual = g->adj[u];
    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    if (!g || u < 0 || u >= g->n || v < 0 || v >= g->n) return 0;
    No *atual = g->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }
    return 0;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}