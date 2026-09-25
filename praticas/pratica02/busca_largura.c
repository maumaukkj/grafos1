#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"

// --- Implementação do Grafo Base ---
GrafoLista* criar_grafo(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) malloc(n * sizeof(No*));
    for(int i = 0; i < n; i++) g->adj[i] = NULL;
    return g;
}

void inserir_aresta(GrafoLista *g, int u, int v) {
    No *novo = (No*) malloc(sizeof(No));
    novo->destino = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
    
    // Grafo não-direcionado
    novo = (No*) malloc(sizeof(No));
    novo->destino = u;
    novo->prox = g->adj[v];
    g->adj[v] = novo;
}

void liberar_grafo(GrafoLista *g) {
    for(int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(g->adj);
    free(g);
}

// --- Fila para BFS ---
Fila* criar_fila(int capacidade) {
    Fila *f = (Fila*) malloc(sizeof(Fila));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    f->dados = (int*) malloc(capacidade * sizeof(int));
    return f;
}

void enfileirar(Fila *f, int valor) {
    if (f->tamanho < f->capacidade) {
        f->dados[f->fim] = valor;
        f->fim = (f->fim + 1) % f->capacidade;
        f->tamanho++;
    }
}

int desenfileirar(Fila *f) {
    int valor = -1;
    if (f->tamanho > 0) {
        valor = f->dados[f->inicio];
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->tamanho--;
    }
    return valor;
}

void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

// --- Busca em Largura e Aplicações ---
void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for(int i = 0; i < g->n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }
    
    Fila *f = criar_fila(g->n);
    dist[origem] = 0;
    enfileirar(f, origem);
    
    while(f->tamanho > 0) {
        int u = desenfileirar(f);
        No *atual = g->adj[u];
        while(atual != NULL) {
            int v = atual->destino;
            if(dist[v] == -1) { 
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(f, v);
            }
            atual = atual->prox;
        }
    }
    liberar_fila(f);
}

int eh_bipartido(GrafoLista *g) {
    int *cor = (int*) malloc(g->n * sizeof(int));
    for(int i = 0; i < g->n; i++) cor[i] = -1; // -1: sem cor
    
    for(int i = 0; i < g->n; i++) {
        if(cor[i] == -1) {
            Fila *f = criar_fila(g->n);
            cor[i] = 0;
            enfileirar(f, i);
            
            while(f->tamanho > 0) {
                int u = desenfileirar(f);
                No *atual = g->adj[u];
                while(atual != NULL) {
                    int v = atual->destino;
                    if(cor[v] == -1) {
                        cor[v] = 1 - cor[u]; // Alterna entre cor 0 e 1
                        enfileirar(f, v);
                    } else if (cor[v] == cor[u]) {
                        liberar_fila(f);
                        free(cor);
                        return 0; // Achou vizinhos com mesma cor, não é bipartido
                    }
                    atual = atual->prox;
                }
            }
            liberar_fila(f);
        }
    }
    free(cor);
    return 1; 
}