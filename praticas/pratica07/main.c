#include "bellman_ford.h"

void testar_grafo_sem_ciclo_negativo(void) {
    printf("=====================================================\n");
    printf("  Cenario 1: Grafo com pesos negativos (Sem ciclo)   \n");
    printf("=====================================================\n\n");

    int num_vertices = 5;
    GrafoListaPonderado *g = criar_grafo(num_vertices);
    if (g == NULL) return;

    /*
     * Grafo ponderado orientado com custos negativos:
     * 0 -> 1 (-1), 0 -> 2 (4)
     * 1 -> 2 (3), 1 -> 3 (2), 1 -> 4 (2)
     * 3 -> 2 (5), 3 -> 1 (1)
     * 4 -> 3 (-3)
     */
    adicionar_aresta(g, 0, 1, -1);
    adicionar_aresta(g, 0, 2, 4);
    adicionar_aresta(g, 1, 2, 3);
    adicionar_aresta(g, 1, 3, 2);
    adicionar_aresta(g, 1, 4, 2);
    adicionar_aresta(g, 3, 2, 5);
    adicionar_aresta(g, 3, 1, 1);
    adicionar_aresta(g, 4, 3, -3);

    int *dist = (int *)malloc(num_vertices * sizeof(int));
    int *pred = (int *)malloc(num_vertices * sizeof(int));

    int origem = 0;
    int resultado = bellman_ford(g, origem, dist, pred);

    if (resultado == 1) {
        printf("Sucesso: Nao ha ciclo negativo alcancavel a partir do vertice %d.\n\n", origem);
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
        printf("-----------------------------------------\n\n");
    } else {
        printf("Ciclo de custo negativo detectado.\n\n");
    }

    free(dist);
    free(pred);
    liberar_grafo(g);
}

void testar_grafo_com_ciclo_negativo(void) {
    printf("=====================================================\n");
    printf("  Cenario 2: Grafo com Ciclo de Custo Negativo       \n");
    printf("=====================================================\n\n");

    int num_vertices = 4;
    GrafoListaPonderado *g = criar_grafo(num_vertices);
    if (g == NULL) return;

    /*
     * Vértices 0 a 3:
     * 0 -> 1 (1)
     * 1 -> 2 (2)
     * 2 -> 3 (-4)
     * 3 -> 1 (1)
     * Ciclo: 1 -> 2 -> 3 -> 1 (custo = 2 - 4 + 1 = -1 < 0)
     */
    adicionar_aresta(g, 0, 1, 1);
    adicionar_aresta(g, 1, 2, 2);
    adicionar_aresta(g, 2, 3, -4);
    adicionar_aresta(g, 3, 1, 1);

    int *dist = (int *)malloc(num_vertices * sizeof(int));
    int *pred = (int *)malloc(num_vertices * sizeof(int));

    int origem = 0;
    int resultado = bellman_ford(g, origem, dist, pred);

    if (resultado == 1) {
        printf("Nenhum ciclo negativo detectado.\n");
    } else {
        printf("Resultado: bellman_ford retornou 0 (Ciclo negativo confirmado).\n\n");
    }

    free(dist);
    free(pred);
    liberar_grafo(g);
}

int main(void) {
    testar_grafo_sem_ciclo_negativo();
    testar_grafo_com_ciclo_negativo();
    return 0;
}