#include "hamiltoniano.h"
#include "tps.h"

void imprimir_vetor(const char *label, const int *v, int tamanho) {
    printf("%s: ", label);
    for (int i = 0; i < tamanho; i++) {
        printf("%d", v[i]);
        if (i < tamanho - 1) printf(" -> ");
    }
    printf("\n");
}

void demonstrar_hamiltoniano_e_reducao(void) {
    printf("=========================================================================\n");
    printf("  Parte 1: Caminho/Ciclo Hamiltoniano e Reducao para TSP                 \n");
    printf("=========================================================================\n\n");

    int n = 5;
    GrafoHamiltoniano *gh = criar_grafo_h(n);

    /*
     * Ciclo hamiltoniano 0-1-2-3-4-0 com arestas adicionais:
     * 0-1, 1-2, 2-3, 3-4, 4-0, 0-2, 1-3
     */
    adicionar_aresta_h(gh, 0, 1);
    adicionar_aresta_h(gh, 1, 2);
    adicionar_aresta_h(gh, 2, 3);
    adicionar_aresta_h(gh, 3, 4);
    adicionar_aresta_h(gh, 4, 0);
    adicionar_aresta_h(gh, 0, 2);
    adicionar_aresta_h(gh, 1, 3);

    printf("Grafo criado com %d vertices.\n", n);

    int *caminho = (int *)malloc(n * sizeof(int));
    int *ciclo = (int *)malloc((n + 1) * sizeof(int));

    if (encontrar_caminho_hamiltoniano_bt(gh, caminho)) {
        imprimir_vetor("Caminho Hamiltoniano encontrado", caminho, n);
    } else {
        printf("Nenhum caminho hamiltoniano encontrado.\n");
    }

    if (encontrar_ciclo_hamiltoniano_bt(gh, ciclo)) {
        imprimir_vetor("Ciclo Hamiltoniano encontrado  ", ciclo, n + 1);
    } else {
        printf("Nenhum ciclo hamiltoniano encontrado.\n");
    }

    /* Redução para TSP: aresta no grafo = 1, aresta fora = 2 */
    printf("\nAplicando reducao grafo_para_tsp...\n");
    GrafoTSP *gt_reduzido = grafo_para_tsp(gh);

    int *circuito_tsp = (int *)malloc((n + 1) * sizeof(int));
    int custo_tsp = tsp_forca_bruta(gt_reduzido, circuito_tsp);

    printf("Custo otimo no TSP reduzido: %d\n", custo_tsp);
    imprimir_vetor("Circuito correspondente no TSP", circuito_tsp, n + 1);
    if (custo_tsp == n) {
        printf("-> Validacao: Custo == %d confirma existencia do Ciclo Hamiltoniano no grafo original!\n\n", n);
    } else {
        printf("-> Validacao: Custo > %d indica que nao existe Ciclo Hamiltoniano.\n\n", n);
    }

    free(caminho);
    free(ciclo);
    free(circuito_tsp);
    liberar_grafo_tsp(gt_reduzido);
    liberar_grafo_h(gh);
}

void executar_experimento_tsp(void) {
    printf("=========================================================================\n");
    printf("  Parte 2: Experimento Comparativo TSP (n = 5, 8, 10, 12)                \n");
    printf("=========================================================================\n");
    printf("%-4s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s\n",
           "n", "Custo (FB)", "Tempo FB(ms)", "Custo (VMP)", "Tempo VMP", "Custo (Ins)", "Tempo Ins");
    printf("-------------------------------------------------------------------------\n");

    int tamanhos[] = {5, 8, 10, 12};
    int total_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    srand(12345);

    for (int t = 0; t < total_tamanhos; t++) {
        int n = tamanhos[t];
        GrafoTSP *g = gerar_grafo_tsp_aleatorio(n, 100);
        if (g == NULL) continue;

        int *circuito_fb = (int *)malloc((n + 1) * sizeof(int));
        int *circuito_vmp = (int *)malloc((n + 1) * sizeof(int));
        int *circuito_ins = (int *)malloc((n + 1) * sizeof(int));

        // 1. Força Bruta
        clock_t t0 = clock();
        int custo_fb = tsp_forca_bruta(g, circuito_fb);
        clock_t t1 = clock();
        double tempo_fb = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

        // 2. Vizinho Mais Próximo
        t0 = clock();
        int custo_vmp = tsp_vizinho_mais_proximo(g, 0, circuito_vmp);
        t1 = clock();
        double tempo_vmp = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

        // 3. Inserção do Mais Próximo
        t0 = clock();
        int custo_ins = tsp_insercao(g, circuito_ins);
        t1 = clock();
        double tempo_ins = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

        printf("%-4d | %-12d | %-12.4f | %-12d | %-12.4f | %-12d | %-12.4f\n",
               n, custo_fb, tempo_fb, custo_vmp, tempo_vmp, custo_ins, tempo_ins);

        free(circuito_fb);
        free(circuito_vmp);
        free(circuito_ins);
        liberar_grafo_tsp(g);
    }
    printf("=========================================================================\n\n");
}

int main(void) {
    demonstrar_hamiltoniano_e_reducao();
    executar_experimento_tsp();
    return 0;
}