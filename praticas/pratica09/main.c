#include "cobertura.h"

void imprimir_cobertura(const char *nome, const int *cobertura, int n, int tamanho) {
    printf("%s (Tamanho %d): { ", nome, tamanho);
    int primeiro = 1;
    for (int i = 0; i < n; i++) {
        if (cobertura[i]) {
            if (!primeiro) printf(", ");
            printf("%d", i);
            primeiro = 0;
        }
    }
    printf(" }\n");
}

void executar_demonstracao(void) {
    printf("=====================================================\n");
    printf("  Demonstracao Inicial: Grafo Fixo com n = 6         \n");
    printf("=====================================================\n");

    int n = 6;
    Grafo *g = criar_grafo(n);
    if (g == NULL) return;

    /*
     * Grafo com 6 vértices e 7 arestas:
     * (0, 1), (0, 2), (1, 2), (1, 3), (2, 4), (3, 4), (4, 5)
     */
    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 0, 2);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 1, 3);
    adicionar_aresta(g, 2, 4);
    adicionar_aresta(g, 3, 4);
    adicionar_aresta(g, 4, 5);

    printf("Grafo criado com %d vertices e %d arestas.\n\n", g->num_vertices, g->num_arestas);

    int *cob_bt = (int *)calloc(n, sizeof(int));
    int *cob_gulosa = (int *)calloc(n, sizeof(int));

    int tam_bt = cobertura_minima_bt(g, cob_bt);
    int valido_bt = eh_cobertura(g, cob_bt);

    int tam_gulosa = cobertura_gulosa(g, cob_gulosa);
    int valido_gulosa = eh_cobertura(g, cob_gulosa);

    imprimir_cobertura("Backtracking (Otimo)", cob_bt, n, tam_bt);
    printf("  -> Valido segundo eh_cobertura: %s\n\n", valido_bt ? "SIM" : "NAO");

    imprimir_cobertura("Heuristica Gulosa   ", cob_gulosa, n, tam_gulosa);
    printf("  -> Valido segundo eh_cobertura: %s\n\n", valido_gulosa ? "SIM" : "NAO");

    free(cob_bt);
    free(cob_gulosa);
    liberar_grafo(g);
}

void executar_experimento(void) {
    printf("========================================================================================\n");
    printf("  Experimento Comparativo: Forca Bruta (Backtracking) vs Heuristica Gulosa              \n");
    printf("========================================================================================\n");
    printf("%-5s | %-7s | %-12s | %-14s | %-14s | %-14s | %-6s\n",
           "n", "|E|", "Tam (Otimo)", "Tempo BT (ms)", "Tam (Guloso)", "Tempo Guloso", "Razao");
    printf("----------------------------------------------------------------------------------------\n");

    int tamanhos[] = {5, 8, 10, 12, 15};
    int num_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    srand(42); // Semente fixa para reproducibilidade

    for (int t = 0; t < num_testes; t++) {
        int n = tamanhos[t];
        Grafo *g = gerar_grafo_aleatorio(n, 0.40); // Densidade de arestas ~40%
        if (g == NULL) continue;

        int *cob_bt = (int *)calloc(n, sizeof(int));
        int *cob_gulosa = (int *)calloc(n, sizeof(int));

        // Medição do tempo de Força Bruta com Backtracking
        clock_t inicio_bt = clock();
        int tam_bt = cobertura_minima_bt(g, cob_bt);
        clock_t fim_bt = clock();
        double tempo_bt_ms = ((double)(fim_bt - inicio_bt) / CLOCKS_PER_SEC) * 1000.0;

        // Medição do tempo da Heurística Gulosa
        clock_t inicio_g = clock();
        int tam_gulosa = cobertura_gulosa(g, cob_gulosa);
        clock_t fim_g = clock();
        double tempo_g_ms = ((double)(fim_g - inicio_g) / CLOCKS_PER_SEC) * 1000.0;

        double razao = (tam_bt > 0) ? (double)tam_gulosa / tam_bt : 1.0;

        printf("%-5d | %-7d | %-12d | %-14.4f | %-14d | %-14.4f | %-6.2f\n",
               n, g->num_arestas, tam_bt, tempo_bt_ms, tam_gulosa, tempo_g_ms, razao);

        free(cob_bt);
        free(cob_gulosa);
        liberar_grafo(g);
    }
    printf("========================================================================================\n\n");
}

int main(void) {
    executar_demonstracao();
    executar_experimento();
    return 0;
}