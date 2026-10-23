#include "dijkstra.h"

int main(void) {
    printf("=========================================\n");
    printf("  Pratica 06: Menor Caminho - Dijkstra   \n");
    printf("=========================================\n\n");

    int num_vertices = 7;
    GrafoListaPonderado *g = criar_grafo(num_vertices);
    if (g == NULL) {
        fprintf(stderr, "Erro ao criar grafo.\n");
        return 1;
    }

    /*
     * Montando o grafo ponderado de teste:
     * Vértices 0 a 5 conectados, vértice 6 desconectado para testar inacessibilidade
     */
    adicionar_aresta(g, 0, 1, 4);
    adicionar_aresta(g, 0, 2, 2);
    adicionar_aresta(g, 1, 2, 1);
    adicionar_aresta(g, 1, 3, 5);
    adicionar_aresta(g, 2, 3, 8);
    adicionar_aresta(g, 2, 4, 10);
    adicionar_aresta(g, 3, 4, 2);
    adicionar_aresta(g, 3, 5, 6);
    adicionar_aresta(g, 4, 5, 3);

    int *dist = (int *)malloc(num_vertices * sizeof(int));
    int *pred = (int *)malloc(num_vertices * sizeof(int));

    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para dist/pred.\n");
        liberar_grafo(g);
        if (dist) free(dist);
        if (pred) free(pred);
        return 1;
    }

    int origem = 0;
    printf("Executando Dijkstra a partir do vertice origem = %d...\n\n", origem);
    dijkstra(g, origem, dist, pred);

    printf("Tabela de Distancias e Predecessores:\n");
    printf("-----------------------------------------\n");
    printf("Vertice | Distancia Minima | Predecessor \n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < num_vertices; i++) {
        if (dist[i] == INF) {
            printf("   %2d   |       INF        |     %2d      \n", i, pred[i]);
        } else {
            printf("   %2d   |       %3d        |     %2d      \n", i, dist[i], pred[i]);
        }
    }
    printf("-----------------------------------------\n\n");

    printf("Reconstrucao dos Caminhos Minimos:\n");
    printf("-----------------------------------------\n");
    for (int destino = 0; destino < num_vertices; destino++) {
        reconstruir_caminho(origem, destino, pred);
    }
    printf("-----------------------------------------\n");

    free(dist);
    free(pred);
    liberar_grafo(g);

    return 0;
}