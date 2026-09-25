#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

// --- Grafo Base ---
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
    
    // Ida e volta
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

// --- Algoritmo de Tarjan ---
static int minimo(int a, int b) {
    return (a < b) ? a : b;
}

static void dfs_tarjan(GrafoLista *g, int u, int *visitado, int *descoberta, int *low, int *pai, int *articulacao, int *tempo) {
    visitado[u] = 1;
    descoberta[u] = low[u] = ++(*tempo);
    int filhos = 0;

    No *atual = g->adj[u];
    while(atual != NULL) {
        int v = atual->destino;
        
        if (!visitado[v]) {
            filhos++;
            pai[v] = u;
            dfs_tarjan(g, v, visitado, descoberta, low, pai, articulacao, tempo);
            
            low[u] = minimo(low[u], low[v]);
            
            // Condição 1: u é raiz e tem mais de 1 filho
            if (pai[u] == -1 && filhos > 1) {
                articulacao[u] = 1;
            }
            // Condição 2: u não é raiz e o valor low do filho v é >= tempo de descoberta de u
            if (pai[u] != -1 && low[v] >= descoberta[u]) {
                articulacao[u] = 1;
            }
            // Detecção de ponte
            if (low[v] > descoberta[u]) {
                printf("  -> Ponte encontrada: (%d, %d)\n", u, v);
            }
        } else if (v != pai[u]) {
            // Aresta de retorno (Back edge)
            low[u] = minimo(low[u], descoberta[v]);
        }
        atual = atual->prox;
    }
}

void encontrar_articulacoes_e_pontes(GrafoLista *g) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    int *descoberta = (int*) calloc(g->n, sizeof(int));
    int *low = (int*) calloc(g->n, sizeof(int));
    int *pai = (int*) malloc(g->n * sizeof(int));
    int *articulacao = (int*) calloc(g->n, sizeof(int));
    int tempo = 0;

    for(int i = 0; i < g->n; i++) pai[i] = -1;

    for(int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            dfs_tarjan(g, i, visitado, descoberta, low, pai, articulacao, &tempo);
        }
    }

    printf("  -> Vertices de corte (Articulacoes): ");
    int achou_art = 0;
    for(int i = 0; i < g->n; i++) {
        if(articulacao[i]) {
            printf("%d ", i);
            achou_art = 1;
        }
    }
    if(!achou_art) printf("Nenhum");
    printf("\n");

    free(visitado);
    free(descoberta);
    free(low);
    free(pai);
    free(articulacao);
}