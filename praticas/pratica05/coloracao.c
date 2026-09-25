#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

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

// --- Coloração Gulosa (Ordem Arbitrária) ---
int* coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int *cor = (int*) malloc(g->n * sizeof(int));
    for(int i = 0; i < g->n; i++) cor[i] = -1; // -1: sem cor
    
    int *disponivel = (int*) malloc(g->n * sizeof(int));
    *num_cores = 0;
    
    for(int u = 0; u < g->n; u++) {
        for(int c = 0; c < g->n; c++) disponivel[c] = 1; // 1: disponivel
        
        // Marca as cores dos vizinhos como indisponiveis
        No *atual = g->adj[u];
        while(atual != NULL) {
            if(cor[atual->destino] != -1) {
                disponivel[cor[atual->destino]] = 0;
            }
            atual = atual->prox;
        }
        
        // Acha a primeira cor disponivel
        int c;
        for(c = 0; c < g->n; c++) {
            if(disponivel[c]) break;
        }
        
        cor[u] = c;
        if(c + 1 > *num_cores) *num_cores = c + 1;
    }
    
    free(disponivel);
    return cor;
}

// --- Heurística Welsh-Powell ---
typedef struct {
    int id;
    int grau;
} VerticeGrau;

static int comparar_grau(const void *a, const void *b) {
    return ((VerticeGrau*)b)->grau - ((VerticeGrau*)a)->grau; // Ordem decrescente
}

int* coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    VerticeGrau *vg = (VerticeGrau*) malloc(g->n * sizeof(VerticeGrau));
    
    // Calcula o grau de todos os vértices
    for(int i = 0; i < g->n; i++) {
        vg[i].id = i;
        vg[i].grau = 0;
        No *atual = g->adj[i];
        while(atual != NULL) {
            vg[i].grau++;
            atual = atual->prox;
        }
    }
    
    // Ordena por grau (maior para o menor)
    qsort(vg, g->n, sizeof(VerticeGrau), comparar_grau);
    
    int *cor = (int*) malloc(g->n * sizeof(int));
    for(int i = 0; i < g->n; i++) cor[i] = -1;
    
    int *disponivel = (int*) malloc(g->n * sizeof(int));
    *num_cores = 0;
    
    // Aplica o algoritmo guloso seguindo a ordem ordenada
    for(int i = 0; i < g->n; i++) {
        int u = vg[i].id;
        for(int c = 0; c < g->n; c++) disponivel[c] = 1;
        
        No *atual = g->adj[u];
        while(atual != NULL) {
            if(cor[atual->destino] != -1) {
                disponivel[cor[atual->destino]] = 0;
            }
            atual = atual->prox;
        }
        
        int c;
        for(c = 0; c < g->n; c++) {
            if(disponivel[c]) break;
        }
        
        cor[u] = c;
        if(c + 1 > *num_cores) *num_cores = c + 1;
    }
    
    free(disponivel);
    free(vg);
    return cor;
}

// --- Verificação de Grafo Bipartido (2-coloração via BFS) ---
int eh_bipartido(GrafoLista *g) {
    int *cor = (int*) malloc(g->n * sizeof(int));
    for(int i = 0; i < g->n; i++) cor[i] = -1;
    
    int *fila = (int*) malloc(g->n * sizeof(int));
    
    for(int i = 0; i < g->n; i++) {
        if(cor[i] == -1) {
            int inicio = 0, fim = 0;
            cor[i] = 0;
            fila[fim++] = i;
            
            while(inicio < fim) {
                int u = fila[inicio++];
                No *atual = g->adj[u];
                while(atual != NULL) {
                    int v = atual->destino;
                    if(cor[v] == -1) {
                        cor[v] = 1 - cor[u]; // Alterna entre 0 e 1
                        fila[fim++] = v;
                    } else if(cor[v] == cor[u]) {
                        // Achou dois adjacentes com a mesma cor
                        free(cor);
                        free(fila);
                        return 0; 
                    }
                    atual = atual->prox;
                }
            }
        }
    }
    free(cor);
    free(fila);
    return 1;
}