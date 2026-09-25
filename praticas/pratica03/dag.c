#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

// --- Grafo Base (Direcionado) ---
GrafoLista* criar_digrafo(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) malloc(n * sizeof(No*));
    for(int i = 0; i < n; i++) g->adj[i] = NULL;
    return g;
}

void inserir_aresta_direcionada(GrafoLista *g, int u, int v) {
    No *novo = (No*) malloc(sizeof(No));
    novo->destino = v;
    novo->prox = g->adj[u];
    g->adj[u] = novo; // Apenas u -> v
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

// --- Algoritmo de Kahn (BFS com grau de entrada) ---
int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int *grau_entrada = (int*) calloc(g->n, sizeof(int));
    int *ordem = (int*) malloc(g->n * sizeof(int));
    int count = 0;

    // 1. Calcula o grau de entrada de todos os vértices
    for(int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while(atual != NULL) {
            grau_entrada[atual->destino]++;
            atual = atual->prox;
        }
    }

    // 2. Fila para armazenar vértices com grau de entrada 0
    int *fila = (int*) malloc(g->n * sizeof(int));
    int inicio = 0, fim = 0;

    for(int i = 0; i < g->n; i++) {
        if(grau_entrada[i] == 0) {
            fila[fim++] = i;
        }
    }

    // 3. Processa a fila
    while(inicio < fim) {
        int u = fila[inicio++];
        ordem[count++] = u;

        No *atual = g->adj[u];
        while(atual != NULL) {
            int v = atual->destino;
            grau_entrada[v]--;
            if(grau_entrada[v] == 0) {
                fila[fim++] = v;
            }
            atual = atual->prox;
        }
    }

    free(grau_entrada);
    free(fila);

    // Se não visitou todos, tem ciclo
    if(count != g->n) {
        free(ordem);
        *tamanho = 0;
        return NULL; 
    }

    *tamanho = count;
    return ordem;
}

// --- Algoritmo DFS ---
static int dfs_topo(GrafoLista *g, int u, int *estado, int *ordem, int *idx) {
    estado[u] = 1; // 1 = Visitando (Cinza)

    No *atual = g->adj[u];
    while(atual != NULL) {
        int v = atual->destino;
        if(estado[v] == 1) return 0; // Achou um ciclo (retornou para um nó cinza)
        if(estado[v] == 0) {
            if(!dfs_topo(g, v, estado, ordem, idx)) return 0;
        }
        atual = atual->prox;
    }

    estado[u] = 2; // 2 = Visitado (Preto)
    // Empilha na saída (preenche o array do final para o início)
    ordem[(*idx)--] = u; 
    return 1;
}

int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int *estado = (int*) calloc(g->n, sizeof(int)); // 0: Branco
    int *ordem = (int*) malloc(g->n * sizeof(int));
    int idx = g->n - 1; // Começa do fim do array

    for(int i = 0; i < g->n; i++) {
        if(estado[i] == 0) {
            if(!dfs_topo(g, i, estado, ordem, &idx)) {
                free(estado);
                free(ordem);
                *tamanho = 0;
                return NULL; // Achou ciclo
            }
        }
    }

    free(estado);
    *tamanho = g->n;
    return ordem;
}

// --- Verificação de DAG ---
int eh_dag(GrafoLista *g) {
    int tamanho;
    int *ordem = ordenacao_topologica_kahn(g, &tamanho);
    if(ordem == NULL) return 0; // Tem ciclo, não é DAG
    free(ordem);
    return 1; // Sem ciclo, é DAG
}