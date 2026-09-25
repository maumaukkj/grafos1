#include <stdio.h>
#include <stdlib.h>
#include "busca_profundidade.h"

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo_entrada, int *tempo_saida, int *tempo) {
    visitado[u] = 1; // 1 = Cinza (Descoberto)
    tempo_entrada[u] = ++(*tempo);
    
    No *atual = g->adj[u];
    while(atual != NULL) {
        int v = atual->destino;
        if (visitado[v] == 0) { // 0 = Branco (Não visitado)
            dfs_recursiva(g, v, visitado, tempo_entrada, tempo_saida, tempo);
        }
        atual = atual->prox;
    }
    
    visitado[u] = 2; // 2 = Preto (Processado)
    tempo_saida[u] = ++(*tempo);
}

int contar_componentes(GrafoLista *g) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    int *tempo_entrada = (int*) calloc(g->n, sizeof(int));
    int *tempo_saida = (int*) calloc(g->n, sizeof(int));
    int tempo = 0;
    int componentes = 0;
    
    for(int i = 0; i < g->n; i++) {
        if(visitado[i] == 0) {
            componentes++;
            dfs_recursiva(g, i, visitado, tempo_entrada, tempo_saida, &tempo);
        }
    }
    
    free(visitado);
    free(tempo_entrada);
    free(tempo_saida);
    return componentes;
}

static int dfs_ciclo(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1; 
    
    No *atual = g->adj[u];
    while(atual != NULL) {
        int v = atual->destino;
        if (visitado[v] == 0) {
            if (dfs_ciclo(g, v, u, visitado)) return 1;
        } else if (v != pai && visitado[v] == 1) {
            // Aresta de retorno (back edge) num grafo não-direcionado indica ciclo
            return 1; 
        }
        atual = atual->prox;
    }
    visitado[u] = 2; 
    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    for(int i = 0; i < g->n; i++) {
        if(visitado[i] == 0) {
            if(dfs_ciclo(g, i, -1, visitado)) {
                free(visitado);
                return 1;
            }
        }
    }
    free(visitado);
    return 0;
}