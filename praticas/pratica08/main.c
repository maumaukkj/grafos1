#include "agm.h"

int main(void) {
    printf("=====================================================\n");
    printf("  Pratica 08: Arvore Geradora Minima (Kruskal & Prim)\n");
    printf("=====================================================\n\n");

    int num_vertices = 6;
    GrafoListaPonderado *g = criar_grafo(num_vertices);
    if (g == NULL) {
        fprintf(stderr, "Erro ao criar grafo.\n");
        return 1;
    }

    /*
     * Grafo ponderado não-orientado com 6 vértices (0 a 5):
     * (0, 1, 4), (0, 2, 4)
     * (1, 2, 2), (1, 3, 3)
     * (2, 3, 4), (2, 4, 2)
     * (3, 4, 3), (3, 5, 2)
     * (4, 5, 3)
     */
    adicionar_aresta(g, 0, 1, 4);
    adicionar_aresta(g, 0, 2, 4);
    adicionar_aresta(g, 1, 2, 2);
    adicionar_aresta(g, 1, 3, 3);
    adicionar_aresta(g, 2, 3, 4);
    adicionar_aresta(g, 2, 4, 2);
    adicionar_aresta(g, 3, 4, 3);
    adicionar_aresta(g, 3, 5, 2);
    adicionar_aresta(g, 4, 5, 3);

    printf("Grafo com %d vertices criado com sucesso.\n\n", num_vertices);

    /* ========================================================
     * Teste do Algoritmo de Kruskal com Union-Find (DSU)
     * ======================================================== */
    printf("-----------------------------------------------------\n");
    printf("1. Executando Algoritmo de Kruskal (O(E log E))...\n");
    printf("-----------------------------------------------------\n");

    Aresta *resultado_kruskal = (Aresta *)malloc((num_vertices - 1) * sizeof(Aresta));
    int tam_kruskal = 0;

    int peso_kruskal = kruskal(g, resultado_kruskal, &tam_kruskal);

    printf("Arestas selecionadas pelo Kruskal:\n");
    for (int i = 0; i < tam_kruskal; i++) {
        printf("  Aresta (%d, %d) com peso %d\n",
               resultado_kruskal[i].u, resultado_kruskal[i].v, resultado_kruskal[i].peso);
    }
    printf("Custo total da AGM (Kruskal): %d\n\n", peso_kruskal);

    /* ========================================================
     * Teste do Algoritmo de Prim com Min-Heap
     * ======================================================== */
    printf("-----------------------------------------------------\n");
    printf("2. Executando Algoritmo de Prim (O((V + E) log V))...\n");
    printf("-----------------------------------------------------\n");

    int *pai_prim = (int *)malloc(num_vertices * sizeof(int));
    int origem_prim = 0;

    int peso_prim = prim(g, origem_prim, pai_prim);

    printf("Arestas selecionadas pelo Prim (a partir da raiz %d):\n", origem_prim);
    for (int v = 0; v < num_vertices; v++) {
        if (pai_prim[v] != -1) {
            printf("  Aresta (%d, %d)\n", pai_prim[v], v);
        }
    }
    printf("Custo total da AGM (Prim): %d\n\n", peso_prim);

    /* ========================================================
     * Validação
     * ======================================================== */
    printf("-----------------------------------------------------\n");
    printf("Verificacao de Consistencia:\n");
    if (peso_kruskal == peso_prim) {
        printf("  [OK] Ambos os algoritmos produziram o mesmo custo total: %d!\n", peso_kruskal);
    } else {
        printf("  [ERRO] Divergencia nos custos: Kruskal = %d, Prim = %d\n", peso_kruskal, peso_prim);
    }
    printf("=====================================================\n");

    free(resultado_kruskal);
    free(pai_prim);
    liberar_grafo(g);

    return 0;
}