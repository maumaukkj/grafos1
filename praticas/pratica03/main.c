#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

void imprimir_array(int *arr, int tamanho) {
    if (arr == NULL) {
        printf("Grafo contem ciclo!\n");
        return;
    }
    for(int i = 0; i < tamanho; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Testes: Ordenacao Topologica e DAG ===\n");
    
    // Grafo 1: DAG (Sem ciclos)
    // 5 -> 2, 5 -> 0, 4 -> 0, 4 -> 1, 2 -> 3, 3 -> 1
    GrafoLista *g1 = criar_digrafo(6);
    inserir_aresta_direcionada(g1, 5, 2);
    inserir_aresta_direcionada(g1, 5, 0);
    inserir_aresta_direcionada(g1, 4, 0);
    inserir_aresta_direcionada(g1, 4, 1);
    inserir_aresta_direcionada(g1, 2, 3);
    inserir_aresta_direcionada(g1, 3, 1);

    printf("\nGrafos 1 e um DAG? %s\n", eh_dag(g1) ? "Sim" : "Nao");
    
    int tam1;
    int *ordem_kahn = ordenacao_topologica_kahn(g1, &tam1);
    printf("Ordenacao Kahn: ");
    imprimir_array(ordem_kahn, tam1);
    free(ordem_kahn);

    int tam2;
    int *ordem_dfs = ordenacao_topologica_dfs(g1, &tam2);
    printf("Ordenacao DFS : ");
    imprimir_array(ordem_dfs, tam2);
    free(ordem_dfs);

    // Grafo 2: Com Ciclo (0 -> 1, 1 -> 2, 2 -> 0)
    printf("\n--- Testando Grafo com Ciclo ---\n");
    GrafoLista *g2 = criar_digrafo(3);
    inserir_aresta_direcionada(g2, 0, 1);
    inserir_aresta_direcionada(g2, 1, 2);
    inserir_aresta_direcionada(g2, 2, 0);

    printf("Grafos 2 e um DAG? %s\n", eh_dag(g2) ? "Sim" : "Nao");
    int tam3;
    int *ordem_ciclo = ordenacao_topologica_kahn(g2, &tam3);
    printf("Ordenacao Kahn com ciclo: ");
    imprimir_array(ordem_ciclo, tam3);

    liberar_grafo(g1);
    liberar_grafo(g2);

    return 0;
}